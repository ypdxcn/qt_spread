#include <cassert>
#include "Listener.h"
#include "Selector.h"
#include "strutils.h"

using namespace strutils;

CSelector::CSelector():
m_pCfg(0),
m_nMaxfds(AIO_MAX_FD),
m_nFds(0),
m_nWaitTimeOut(0),
m_nNumber(CONCUREENT_PROCESSOR),
m_nPos(0),
m_sSocketReadSig(INVALID_SOCKET),
m_sSocketFireSig(INVALID_SOCKET),
m_nPortSig(18008)
{
}

CSelector::~CSelector()
{
}


int CSelector::Init(CConfig * pCfg)
{
	if (!StateCheck(esInit))
		return 0;

	assert(0 != pCfg);
	if (0 == pCfg)
		return -1;

	m_pCfg = pCfg;
	CConfig* pConfig = m_pCfg->GetCfgGlobal();

	std::string sCpuNum("2");
	if (0 == pConfig->GetProperty("cpu_num",sCpuNum))
		m_nNumber = FromString<int>(sCpuNum);

	if (m_nNumber <= 0 || m_nNumber > 128)
		m_nNumber = 2;

	//绑定用于信号模拟的socket
	if (0 != UdpSocket(m_sSocketReadSig,m_nPortSig))
	{
		CRLog(E_ERROR,"%s","unbind udp sig");
		return -1;
	}
	m_sSocketFireSig = socket(AF_INET, SOCK_DGRAM, 0);

	//创建工作线程池,生成对应的就绪队列/互斥量
	for (int i = 0; i < m_nNumber; i++)
	{
		CSelectorWorker* pWorker = new CSelectorWorker(this,i);
		if (0 == pWorker)
		{
			CRLog(E_CRITICAL,"%s", "No mem");
			for (vector< CSelectorWorker* >::iterator it = m_vWorker.begin(); it != m_vWorker.end(); ++it)
			{
				delete (*it);
			}
			m_vWorker.clear();

			for (vector< CGessMutex* >::iterator itMutex = m_vcsQueue.begin(); itMutex != m_vcsQueue.end(); ++itMutex)
			{
				delete (*itMutex);
			}
			m_vcsQueue.clear();

			m_vqReady.clear();			
			return -1;
		}		
		m_vWorker.push_back(pWorker);

		CGessMutex* pcsMutex = new CGessMutex();
		if (0 == pcsMutex)
		{
			CRLog(E_CRITICAL,"%s", "No mem");
			for (vector< CSelectorWorker* >::iterator itWoker = m_vWorker.begin(); itWoker != m_vWorker.end(); ++itWoker)
			{
				delete (*itWoker);
			}
			m_vWorker.clear();

			for (vector< CGessMutex* >::iterator itMutex = m_vcsQueue.begin(); itMutex != m_vcsQueue.end(); ++itMutex)
			{
				delete (*itMutex);
			}
			m_vcsQueue.clear();

			m_vqReady.clear();
			return -1;
		}
		m_vcsQueue.push_back( pcsMutex );

		deque< PAIR_REG > dq;
		m_vqReady.push_back( dq );
	}

	//启动线程池
	for (int i = 0; i < m_nNumber; i++)
	{
		m_vWorker[i]->Start();
	}

	//启动调度线程
	BeginThread();

	//注册信号模拟socket
	Register(m_sSocketReadSig,0);

	return 0;
}

void CSelector::Finish()
{
	if (!StateCheck(esFinish))
		return;

	for (int i = 0; i < m_nNumber; i++)
	{
		m_vWorker[i]->Stop();
	}

	EndThread();
	UnRegister(m_sSocketReadSig);

	for (int i = 0; i < m_nNumber; i++)
	{
		delete m_vWorker[i];
	}
	m_vWorker.clear();

	for (int i = 0; i < m_nNumber; i++)
	{
		delete m_vcsQueue[i];
	}
	m_vcsQueue.clear();

	for (int i = 0; i < m_nNumber; i++)
	{
		m_vqReady[i].clear();
	}
	m_vqReady.clear();

	closesocket(m_sSocketReadSig);
	closesocket(m_sSocketFireSig);
}

int CSelector::Start()
{
	if (!StateCheck(esStart))
		return 0;


	return 0;
}

void CSelector::Stop()
{
	if (!StateCheck(esStop))
		return;

}

//用于解除select 阻塞
void CSelector::Signal()
{
	struct sockaddr_in addr;
	memset(&addr, 0x00,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPortSig);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	char *p = "s";
	try
	{
		m_csReadSig.Lock();
		if (SOCKET_ERROR == sendto(m_sSocketFireSig,p,strlen(p),0,(struct sockaddr *)&addr, sizeof(addr)))
		{
			CRLog(E_ERROR,"%s","Sig Err sock");
		}
		m_csReadSig.Unlock();
	}
	catch(...)
	{
		m_csReadSig.Unlock();
		CRLog(E_ERROR,"%s","Unknown exception!");
	}
}

//注册
int CSelector::Register(tsocket_t sSocket,void* p)
{
	m_condRegisterRead.Lock();	
	if (m_nFds >= m_nMaxfds)
	{
		m_condRegisterRead.Unlock();
		CRLog(E_ERROR,"no fd,now num %d,max num %d",m_nFds, m_nMaxfds);
		return -1;
	}
	
	m_nFds++;
	if (m_mapRegisterRead.empty())
	{
		m_mapRegisterRead[sSocket] = p;
		m_condRegisterRead.Signal();
		m_condRegisterRead.Unlock();
	}
	else
	{
		m_mapRegisterRead[sSocket] = p;
		m_condRegisterRead.Unlock();
		Signal();
	}
	
	return 0;
}

//注销
void CSelector::UnRegister(tsocket_t sSocket)
{
	m_condRegisterRead.Lock();
	std::map<tsocket_t,void*>::iterator it = m_mapRegisterRead.find(sSocket);
	if (it != m_mapRegisterRead.end())
	{
		m_nFds--;
		m_mapRegisterRead.erase(it);
	}
	m_condRegisterRead.Unlock();

	Signal();
}

void CSelector::Lock(int nIndex)
{
	if (0 != m_vcsQueue[nIndex])
		m_vcsQueue[nIndex]->Lock();
}

void CSelector::Unlock(int nIndex)
{
	if (0 != m_vcsQueue[nIndex])
		m_vcsQueue[nIndex]->Unlock();
}

int CSelector::Push(tsocket_t s,void* p)
{
	if (m_nPos >= m_nNumber)
		m_nPos = 0;

	PAIR_REG pa;
	pa.first = s;
	pa.second = p;

	Lock(m_nPos);
	m_vqReady[m_nPos].push_back(pa);	
	Unlock(m_nPos);
	
	m_nPos++;
	return 0;
}

void CSelector::NotifyQueue()
{
	for (int i = 0; i < m_nNumber; i++)
	{
		Lock(i);
		bool blEmpty = m_vqReady[i].empty();
		Unlock(i);
		if (!blEmpty)
			m_vWorker[i]->Signal();
	}
}

int CSelector::ThreadEntry()
{
	try
	{
		while (!m_bEndThread)
		{
			m_condRegisterRead.Lock();
			while(m_mapRegisterRead.empty() && !m_bEndThread)
				m_condRegisterRead.Wait();

			if (m_bEndThread)
			{
				m_condRegisterRead.Unlock();
				break;
			}

			fd_set fdRead;
			//fd_set fdWrite;

			FD_ZERO(&fdRead);

			tsocket_t maxFd = 0;
			std::map<tsocket_t,void*>::iterator it;
			for (it = m_mapRegisterRead.begin(); it != m_mapRegisterRead.end(); ++it)
			{
				FD_SET((*it).first, &fdRead);
				if ((*it).first > maxFd)
					maxFd = (*it).first;
			}
			m_condRegisterRead.Unlock();

			int nRtn = select(static_cast<int>(maxFd+1),&fdRead,0,0,0);
 			if (SOCKET_ERROR == nRtn)
			{
				CRLog(E_ERROR,"%s","Select err");
				_msleep(1);
				continue;
			}
			else if (0 == nRtn)
			{
				continue;
			}

			if (0 != FD_ISSET(m_sSocketReadSig, &fdRead))
			{//用作信号套接字
				char szTmp[128];
				recv(m_sSocketReadSig,szTmp,sizeof(szTmp),0);
				continue;
			}

			//找出就绪套接字
			int nCount = 0;
			m_condRegisterRead.Lock();
			for (it = m_mapRegisterRead.begin(); it != m_mapRegisterRead.end(); )
			{
				if (nCount >= nRtn)
					break;

				if (0 != FD_ISSET((*it).first, &fdRead))
				{
					tsocket_t s = (*it).first;
					void* p = (*it).second;
					m_mapRegisterRead.erase(it++);
					m_nFds--;
					OnActive(s,p);

					nCount++;
				}
				else
				{
					++it;
				}				
			}
			m_condRegisterRead.Unlock();

			//
			NotifyQueue();
		}

		CRLog(E_SYSINFO,"%s","Selector thread exit");
		return 0;
	}
	catch(std::exception e)
	{
		CRLog(E_ERROR,"exception:%s!",e.what());
		return -1;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknown exception!");
		return -1;
	}
}

int CSelector::End()
{
	m_condRegisterRead.Signal();
	Signal();

	Wait();
	return 0;
}

string CSelector::ToString()
{
	string sRtn = "";
	for (int i = 0; i < m_nNumber; i++)
	{
		sRtn += "\r\nQue";
		sRtn += strutils::ToString<int>(i) + ":";
		deque<PAIR_REG>::iterator it;
		for (it = m_vqReady[i].begin(); it != m_vqReady[i].end(); ++it)
		{
			sRtn += strutils::ToString<int>(static_cast<int>((*it).first)) + ";";
		}
	}

	sRtn += "\r\n";
	map<tsocket_t,void*>::iterator itMap;
	for (itMap = m_mapRegisterRead.begin(); itMap != m_mapRegisterRead.end(); ++itMap)
	{
		sRtn += strutils::ToString<int>(static_cast<int>((*itMap).first)) + ";";
	}

	return sRtn;
}

int CSelectorListen::Handle(int nIndex)
{
	std::vector<PAIR_REG> events;

	Lock(nIndex);
	while (!m_vqReady[nIndex].empty())
	{
		events.push_back(m_vqReady[nIndex].front());
		m_vqReady[nIndex].pop_front();
	}
	Unlock(nIndex);

	if (0 == events.size())
		return 0;

	CListener* p = 0;
	try
	{
		for (std::vector<PAIR_REG>::iterator it = events.begin(); it != events.end(); ++it)
		{
			int nPort = 0;
			string sIp("");
			struct sockaddr_in stPeerAddr;
			socklen_t nAddrLen = sizeof(sockaddr);
			tsocket_t sAccept = accept((*it).first,reinterpret_cast<sockaddr*>(&stPeerAddr),&nAddrLen);
			if (INVALID_SOCKET == sAccept)
				continue;
			
			nPort = ntohs(stPeerAddr.sin_port);
			sIp = inet_ntoa(stPeerAddr.sin_addr);


			p = reinterpret_cast<CListener*>((*it).second);
			if (0 != p)
				p->OnAccept(sAccept,sIp,nPort);
		}
	}
	catch(std::exception e)
	{
		CRLog(E_ERROR,"exception:%s!Thread  no %d",e.what(),nIndex);
	}
	catch(...)
	{
		CRLog(E_ERROR,"Unknown exception!Thread  no %d",nIndex);
	}

	for (std::vector<PAIR_REG>::iterator it = events.begin(); it != events.end(); ++it)
	{
		//重新注册侦听
		Register((*it).first,(*it).second);
	}
	events.clear();
	return 0;
}

void CSelectorListen::OnActive(tsocket_t sSocket, void* p)
{

	Push(sSocket,p);
}

int CSelectorListen::UdpSocket(tsocket_t& sUdp,int& nUdpPort)
{
	tsocket_t s = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0x00,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;

	vector<string> v;
	v.clear();
	CConfig* pConfig = m_pCfg->GetCfgGlobal();
	if (0 != pConfig)
	{
		pConfig->GetProperty("aio.listener",v);
	}
	
	for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
	{
		int nPort = FromString<int>(*it);
		addr.sin_port = htons(nPort);
		if (SOCKET_ERROR != bind(s, (struct sockaddr *)&addr, sizeof(addr)))
		{
			nUdpPort = nPort;
			sUdp = s;
			return 0;
		}
	}

	int RANGE_MIN = 30000;
    int RANGE_MAX = 50000;
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < 100; i++)
	{		
		int nPort = rand() % (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
		addr.sin_port = htons(nPort);
		if (SOCKET_ERROR != bind(s, (struct sockaddr *)&addr, sizeof(addr)))
		{
			nUdpPort = nPort;
			sUdp = s;
			return 0;
		}

		_msleep(1);
	}

	return -1;
}

void CSelectorListen::Finish()
{
	//std::map<tsocket_t,void*>::iterator it;
	m_condRegisterRead.Lock();
	//for (it = m_mapRegisterRead.begin(); it != m_mapRegisterRead.end(); ++it)
	//{
	//	closesocket((*it).first);
	//}
	m_mapRegisterRead.clear();
	m_condRegisterRead.Unlock();
}

bool CSelectorListen::IsNetManaged(string& sKeyName)
{
	sKeyName = "套接字监听调度线程";
	return true;
}

int CSelectorIo::Handle(int nIndex)
{
	std::vector<PAIR_REG> events;
	PKEY_CONTEXT lpKeyContext = 0;

	Lock(nIndex);
	while (!m_vqReady[nIndex].empty())
	{
		events.push_back(m_vqReady[nIndex].front());
		m_vqReady[nIndex].pop_front();
	}
	Unlock(nIndex);

	if (0 == events.size())
		return 0;

	CProtocolComm* pComm = 0;
	
	for (std::vector<PAIR_REG>::iterator it = events.begin(); it != events.end(); ++it)
	{
		try
		{
			lpKeyContext = reinterpret_cast<PKEY_CONTEXT>((*it).second);
			if (0 == lpKeyContext)
			{
				CRLog(E_ERROR,"NULL KEY,Thread %d",nIndex);
				continue;
			}

			pComm = lpKeyContext->pProtocolComm;
			if (0 == pComm)
			{				
				CRLog(E_ERROR,"NULL Comm,socke %d,Thread %d",lpKeyContext->sSocket,nIndex);
				closesocket(lpKeyContext->sSocket);
				continue;
			}
			
			pComm->RecvData(lpKeyContext);
		}
		catch(std::exception e)
		{
			CRLog(E_ERROR,"exception:%s!Thread  no %d",e.what(),nIndex);
			continue;
		}
		catch(...)
		{
			CRLog(E_ERROR,"Unknown exception!Thread  no %d",nIndex);
			continue;
		}
	}
	
	events.clear();
	return 0;
}

void CSelectorIo::OnActive(tsocket_t sSocket, void* p)
{
	Push(sSocket,p);
}


int CSelectorIo::UdpSocket(tsocket_t& sUdp,int& nUdpPort)
{
	tsocket_t s = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0x00,sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;

	vector<string> v;
	v.clear();
	CConfig* pConfig = m_pCfg->GetCfgGlobal();
	if (0 != pConfig)
	{
		vector<string> v;
		pConfig->GetProperty("aio.io",v);
	}
	
	for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
	{
		int nPort = FromString<int>(*it);
		addr.sin_port = htons(nPort);
		if (SOCKET_ERROR != bind(s, (struct sockaddr *)&addr, sizeof(addr)))
		{
			nUdpPort = nPort;
			sUdp = s;
			return 0;
		}
	}

	int RANGE_MIN = 30000;
    int RANGE_MAX = 50000;
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < 100; i++)
	{
		int nPort = rand() % (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
		addr.sin_port = htons(nPort);

		if (SOCKET_ERROR != bind(s, (struct sockaddr *)&addr, sizeof(addr)))
		{
			nUdpPort = nPort;
			sUdp = s;
			return 0;
		}

		_msleep(1);
	}

	return -1;
}

void CSelectorIo::Finish()
{
//	std::map<tsocket_t,void*>::iterator it;
	m_condRegisterRead.Lock();
//	for (it = m_mapRegisterRead.begin(); it != m_mapRegisterRead.end(); ++it)
//	{
//		closesocket((*it).first);
//		FreeKeyContex(reinterpret_cast<PKEY_CONTEXT>((*it).second));
//	}
	m_mapRegisterRead.clear();
	m_condRegisterRead.Unlock();
}


bool CSelectorIo::IsNetManaged(string& sKeyName)
{
	sKeyName = "套接字收发调度线程";
	return true;
}