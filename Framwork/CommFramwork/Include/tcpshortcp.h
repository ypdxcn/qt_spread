#ifndef _TCP_SHORT_CONNECT_POINT_H
#define _TCP_SHORT_CONNECT_POINT_H

#include "ProtocolComm.h"
#include "Comm.h"
#include <cassert>

using namespace std;


//TCP短连接封装的同步连接点
template<typename PROCESS>
class CTcpShortCpCli: public CConnectPointSync
{
public:
	CTcpShortCpCli(void):m_Process(0),m_pCfg(0){}
	virtual ~CTcpShortCpCli(void){}
public:
	int Init(CConfig* pConfig)
	{
        // 修正没有赋值的bug, 2011-8-18
        m_pCfg = pConfig;

		if (0 != m_oComm.Init(pConfig))
			return -1;

		m_Process = new PROCESS;
		if (0 == m_Process)
		{
			return -1;
		}

		m_Process->Init(m_pCfg);
		m_Process->Bind(&m_oComm);
		return 0;
	}

	void SetIp( const string& sIp, int nPort )
	{
		m_oComm.SetIp(sIp,nPort);
	}

	void SetProxy(int nProxyType /* = 0 */, 
		std::string sProxyHost /* = "" */, int nProxyPort /* = 0 */, 
		std::string sProxyUser /* = "" */, std::string sProxyPassword /* = "" */)
	{
		m_oComm.SetProxy(nProxyType, sProxyHost, nProxyPort, sProxyUser, sProxyPassword);
	}

	void Finish(void)
	{
		m_oComm.DisConnect();
		delete m_Process;
		m_Process = 0;
	}

	//对外提供的收发报文主要接口函数
	int SendPacket(CPacket &sndPacket,CPacket &rcvPacket,unsigned int uiTimeout = 10)
	{
		CGessGuard guard(m_SocketLock);

		int nCommRet = m_oComm.Connect();
		if (0 != nCommRet)
		{
			return (nCommRet != -100 ? -1 : -100);
		}

		if (0 == m_Process)
			return -1;

		int nRtn = m_Process->SendPacket(sndPacket,rcvPacket,uiTimeout);
		m_oComm.DisConnect();
		return nRtn;
	}
private:
	PROCESS*	m_Process;	
	CConfig *	m_pCfg;
	CTcpShortComm	m_oComm;
	CGessMutex  m_SocketLock;
};

#endif