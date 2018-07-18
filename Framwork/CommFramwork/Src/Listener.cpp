#include <cassert>
#include "Listener.h"
#include "ProtocolComm.h"
#include "strutils.h"

using namespace strutils;

CListener::CListener(CProtocolCommServer *p):
m_pCommServer(p),
m_pAioListen(0),
m_sIp(""),
m_nPort(9999),
m_nWaitConnNum(10),
m_sListen(INVALID_SOCKET)
{
}

CListener::~CListener()
{
	if (GetStage() != CAction::esFinish)
		Finish();
}

int CListener::Init(CConfig *	pConfig)
{
	if (!StateCheck(esInit))
		return -1;

	assert(0 != pConfig);
	if (0 == pConfig)
		return -1;

	std::string sPort("");
	if (0 == pConfig->GetProperty("listen_port",sPort))
	{
		m_nPort = FromString<int>(sPort);
	}
	else
	{
		CRLog(E_SYSINFO,"%s","No Lisent port!");
		return -1;
	}

	if (0 != pConfig->GetProperty("listen_ip",m_sIp))
	{
		CRLog(E_SYSINFO,"%s","No Lisent ip,bind any");
	}

	std::string sWaitQueue("");
	if (0 == pConfig->GetProperty("wait_conn_num",sWaitQueue))
	{
		m_nWaitConnNum = FromString<int>(sWaitQueue);
	}
	else
	{
		CRLog(E_SYSINFO,"No wait_conn_num,set default val %d",m_nWaitConnNum);
	}

	m_pAioListen = CreateAioListen();
	if (0 == m_pAioListen)
	{
		CRLog(E_ERROR,"%s","No mem!");
		return -1;
	}

	m_pAioListen->Init(pConfig);
	return 0;
}

void CListener::Finish()
{
	if (!StateCheck(CAction::esFinish))
		return ;
	
	//closesocket(m_sListen);

	m_pAioListen->Finish();
}

int CListener::Start()
{
	if (!StateCheck(esStart))
		return -1;

	m_sListen = socket(AF_INET, SOCK_STREAM, 0); 
	if (INVALID_SOCKET == m_sListen)
	{
		CRLog(E_ERROR,"%s","socket() fail");
		return -1;
	}

	int nValOn = 1;
	if (0 != setsockopt(m_sListen, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&nValOn), sizeof(nValOn)))
	{
		CRLog(E_SYSINFO,"%s","setsockopt SO_REUSEADDR fail");
	}
	
	//unix平台 fork/exec调用需要如下调用进行关闭
	fcntl(m_sListen,F_SETFD,25);

	struct sockaddr_in addr;
	memset(&addr, 0x00,sizeof(addr));
	addr.sin_family = AF_INET;
	if (m_sIp == "")
	{
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(m_sIp.c_str());
	}
	
	addr.sin_port = htons(m_nPort);
	if (SOCKET_ERROR == bind(m_sListen, (struct sockaddr *)&addr, sizeof(addr)))
	{
		CRLog(E_ERROR,"bind(%s:%d) fail!",m_sIp.c_str(),m_nPort);
		closesocket(m_sListen);
		return -1;
	}

	if (SOCKET_ERROR == listen(m_sListen, m_nWaitConnNum))
	{
		CRLog(E_ERROR,"listen(%s:%d) fail!",m_sIp.c_str(),m_nPort);
		closesocket(m_sListen);
		return -1;
	}

	//注册
	m_pAioListen->Register(m_sListen,reinterpret_cast<void*>(this));
	m_pAioListen->Start();

	CRLog(E_SYSINFO,"socket %d listening on %d,queue len %d!",m_sListen,m_nPort,m_nWaitConnNum);
	return 0;
}

void CListener::Stop()
{
	if (!StateCheck(esStop))
		return ;

	m_pAioListen->UnRegister(m_sListen);
	CRLog(E_SYSINFO,"close Listen socket:%d",m_sListen);
	closesocket(m_sListen);

	m_pAioListen->Stop();
}

int CListener::OnAccept(tsocket_t sSocket,const string& sIp,int nPort)
{
	//int nPort = 0;
	//string sIp("");
	//struct sockaddr_in stPeerAddr;
	//socklen_t nAddrLen = sizeof(sockaddr);
	//if (SOCKET_ERROR != getpeername(sSocket,reinterpret_cast<sockaddr*>(&stPeerAddr),&nAddrLen))
	//{
	//	nPort = stPeerAddr.sin_port;
	//	sIp = inet_ntoa(stPeerAddr.sin_addr);
	//}
	return m_pCommServer->OnAccept(sSocket,m_sIp,m_nPort,sIp,nPort);
}

int CListener::OnAccept()
{	
	int nPort = 0;
	string sIp("");
	struct sockaddr_in stPeerAddr;
	socklen_t nAddrLen = sizeof(sockaddr);

	tsocket_t sAccept = accept(m_sListen,reinterpret_cast<sockaddr*>(&stPeerAddr),&nAddrLen);
	if (INVALID_SOCKET == sAccept)
		return -1;

	nPort = ntohs(stPeerAddr.sin_port);
	sIp = inet_ntoa(stPeerAddr.sin_addr);

	return m_pCommServer->OnAccept(sAccept,m_sIp,m_nPort,sIp,nPort);
}