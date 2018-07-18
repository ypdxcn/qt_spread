#include "Connector.h"
#include "Logger.h"
#include "ProtocolComm.h"
#include "strutils.h"

using namespace strutils;

unsigned long CConnector::m_ulSID = 0;
CGessMutex CConnector::m_mutexSID;

CConnector::CConnector(CProtocolCommClient *p)
:m_ulKey(0)
,m_nIndex(0)
,m_pCommClient(p)
{
	m_mutexSID.Lock();
	m_ulKey = ++m_ulSID;
	m_mutexSID.Unlock();
}

CConnector::~CConnector()
{
	if (GetStage() != CAction::esFinish)
		Finish();
}

int CConnector::Init(CConfig *	pConfig)
{
	if (!StateCheck(esInit))
		return -1;

	assert(0 !=  pConfig);
	if (0 == pConfig)
		return -1;

	string sIp;
	vector<string> vIp;
	if (0 == pConfig->GetProperty("ip",sIp))
	{
		vector<string> v = explodeQuoted(",", sIp);
		for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
		{
			// modify by xrs 20140503 解决字符串可能为添加了空格的问题
			string sTrimValue = trim(*it);
			if (!sTrimValue.empty())
				vIp.push_back(sTrimValue);
		}
	}


	std::string sPort("");
	vector<int> vPort;
	if (0 == pConfig->GetProperty("port",sPort))
	{
		vector<string> v = explodeQuoted(",", sPort);
		for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
		{
			// modify by xrs 20140503 解决字符串可能为添加了空格的问题
			string sTrimValue = trim(*it);
			if (!sTrimValue.empty())
			{
				vPort.push_back(FromString<int>(sTrimValue));
			}
		}
	}

	pair<string,int> pa;
	size_t nSize = min(vIp.size(),vPort.size());
	for (size_t nIndex = 0; nIndex < nSize; nIndex++)
	{
		pa.first = vIp[nIndex];
		pa.second = vPort[nIndex];
		m_vPair.push_back(pa);
	}
	return 0;
}

void CConnector::Finish()
{
	if (!StateCheck(esFinish))
		return ;
}

int CConnector::Start()
{
	if (!StateCheck(esStart))
		return -1;

	BeginThread();

	//同步线程启动运行,以保证不丢失信号
	P();
	return 0;
}

void CConnector::Stop()
{
	if (!StateCheck(esStop))
		return ;

	EndThread();
}

void  CConnector::NotifyConnect()
{
	CRLog(E_SYSINFO,"%s","NotifyConnect");

	//trylock
	m_condMutexConnect.Lock();
	m_condMutexConnect.Signal();
	m_condMutexConnect.Unlock();
}

int CConnector::ThreadEntry()
{
	bool blOk = false;
	tsocket_t sSocket;
	time_t tmLast,tmNow;
	unsigned int uiSleep = 5;
	size_t nSize = m_vPair.size();
	if (nSize == 0)
	{
		CRLog(E_ERROR,"%s","配置错误,线程退出!");
		return -1;
	}

	try
	{
		m_condMutexConnect.Lock();

		//通知"连接线程已经启动"
		V();
		while(!m_bEndThread)
		{
			//等待连接信号
			m_condMutexConnect.Wait();
			blOk = false;
			
			time(&tmLast);
			uiSleep = 0;
			sSocket = socket(AF_INET, SOCK_STREAM, 0);
			//一直尝试连接，直到成功为止或线程收退出通知
			while(!m_bEndThread && !blOk)
			{				
				if (INVALID_SOCKET == sSocket)
				{
					sSocket = socket(AF_INET, SOCK_STREAM, 0);
					if (INVALID_SOCKET == sSocket)
					{
						CRLog(E_ERROR,"%s","Error at Connect socket()\n");
						_msleep(5);
						continue;
					}
				}

				time(&tmNow);
				unsigned int nDiff = static_cast<unsigned int>(difftime(tmNow,tmLast));
				if (nDiff < uiSleep)
				{
					_msleep(2);
					continue;
				}

				sockaddr_in addrServer; 
				addrServer.sin_family = AF_INET;
				addrServer.sin_addr.s_addr = inet_addr(m_vPair[m_nIndex].first.c_str());
				addrServer.sin_port = htons(m_vPair[m_nIndex].second);
				if (SOCKET_ERROR == connect( sSocket, (struct sockaddr*) &addrServer, sizeof(addrServer) ))
				{
					closesocket(sSocket);
					sSocket = INVALID_SOCKET;
					CRLog(E_WARNING,"连[%s:%d]失败",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);

					//通知一次失败的连接
					try
					{
						m_pCommClient->OnConnect(INVALID_SOCKET,"127.0.0.1",0,m_vPair[m_nIndex].first,m_vPair[m_nIndex].second,-1);
					}
					catch(...)
					{
						CRLog(E_ERROR,"Unknown exception!%s:%d",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);							
					}

					uiSleep += 5;
					if (uiSleep > 30)
						uiSleep = 5;

					time(&tmLast);

					m_nIndex++;
					if (m_nIndex >= nSize)
						m_nIndex = 0;
					continue;
				}

				CRLog(E_SYSINFO, "与[%s:%d]的连接成功", m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);
				blOk = true;
				uiSleep = 5;

				int nLocalPort = 0;
				string sLocalIp("");				
				struct sockaddr_in stLocalAddr;
				socklen_t nAddrLen = sizeof(sockaddr);
				if (SOCKET_ERROR != getsockname(sSocket,reinterpret_cast<sockaddr*>(&stLocalAddr),&nAddrLen))
				{
					nLocalPort = ntohs(stLocalAddr.sin_port);
					sLocalIp = inet_ntoa(stLocalAddr.sin_addr);
				}

				try
				{
					m_pCommClient->OnConnect(sSocket,sLocalIp,nLocalPort,m_vPair[m_nIndex].first,m_vPair[m_nIndex].second);
				}
				catch(...)
				{
					closesocket(sSocket);
					sSocket = INVALID_SOCKET;
					blOk = false;
					CRLog(E_ERROR,"Unknown exception!%s:%d",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);							
				}
			}
		}
		m_condMutexConnect.Unlock();

		CRLog(E_SYSINFO,"Connector[%s:%d] Thread exit!",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);
		return 0;	
	}
	catch(std::exception e)
	{
		m_condMutexConnect.Unlock();
		CRLog(E_ERROR,"exception:%s! m_bEndThread=%s,%s:%d",e.what(),m_bEndThread?"true":"false",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);
		return -1;
	}
	catch(...)
	{
		m_condMutexConnect.Unlock();
		CRLog(E_ERROR,"Unknown exception! m_bEndThread=%s,%s:%d",m_bEndThread?"true":"false",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);		
		return -1;
	}
}

int CConnector::End()
{
	CRLog(E_SYSINFO,"Signal Connector[%s:%d] end!",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);
	//m_condMutexConnect.Lock();
	m_condMutexConnect.Signal();
	//m_condMutexConnect.Unlock();

	CRLog(E_SYSINFO,"Wait Connector[%s:%d] end!",m_vPair[m_nIndex].first.c_str(), m_vPair[m_nIndex].second);
	Wait();
	return 0;
}

bool CConnector::IsNetManaged(string& sKeyName)
{
	sKeyName = "客户端连接(" + m_vPair[m_nIndex].first + ":" + ToString<int>(m_vPair[m_nIndex].second) + ")";
	return true;
}