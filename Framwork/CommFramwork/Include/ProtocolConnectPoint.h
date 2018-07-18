#ifndef _PROTOCOL_CONNECT_POINT_H
#define _PROTOCOL_CONNECT_POINT_H

#ifdef AIO_IOCP
#include "ProtocolCommWin.h"
#else
#include "ProtocolCommSelector.h"
#endif
#include "Comm.h"
#include "Logger.h"
#include <cassert>

using namespace std;

class CProtocolConnectPoint: public CConnectPointAsyn
{
public:
	CProtocolConnectPoint(void)
	:m_pCpMgr(0)
	,m_pProtocolComm(0)
	,m_ulKey(0xFFFFFFFF)
	,m_pCfg(0)
	{
		for(int i=0;i<ServiceMaxStatic;i++)
			m_uiStatics[i] = 0;
	}

	virtual ~CProtocolConnectPoint(void)
	{
	}

public:
	//连接点初始化
	int Init(CConfig* pConfig)
	{
		assert(pConfig != 0);
		if (pConfig == 0)
			return -1;

		m_pCfg = pConfig;

		m_pProtocolComm = CreateCommInst();	
		if (m_pProtocolComm == 0)
			return -1;

		m_pProtocolComm->Bind(this);
		m_pProtocolComm->Init(m_pCfg);
		return 0;
	}

	//连接点结束服务
	void Finish(void)
	{
		m_pProtocolComm->Finish();

		DestroyCommInst(m_pProtocolComm);
		m_pProtocolComm = 0;
	}

	//连接点开始服务
	int Start()
	{
		return m_pProtocolComm->Start();
	}

	//连接点停止服务
	void Stop()
	{
		m_pProtocolComm->Stop();
	}

	//
	void Bind(CConnectPointManager* pCpMgr,const unsigned long& ulKey)
	{
		m_pCpMgr = dynamic_cast<CProtocolCpMgr*>(pCpMgr);
		m_ulKey = ulKey;

		if (0 == m_pCpMgr)
			CRLog(E_ERROR,"%s","cast null");
	}

	//对外提供的下行报文主要接口函数
	int SendPacket(CPacket &GessPacket)
	{
		assert(m_pProtocolComm != 0);
		if (0 == m_pProtocolComm)
			return -1;

		//统计下行报文 目前未加保护
		m_uiStatics[ServiceDown]++;
		
		assert(0 != m_pProtocolComm);
		if (0 == m_pProtocolComm)
		{
			CRLog(E_ERROR,"%s","0 ProtocolCom");
			return -1;
		}

		return m_pProtocolComm->SendPacket(GessPacket);
	}

	//回调类,连接点给通讯处理器提供的报文上传回调函数 用于报文上传
	int OnRecvPacket(CPacket &GessPacket)
	{
		assert(m_pCpMgr != 0);
		if (0 == m_pCpMgr)
			return -1;

		//统计上行报文 目前未加保护
		m_uiStatics[ServiceUp]++;
		return m_pCpMgr->Forward(GessPacket, m_ulKey);
	}

	int OnLogin(const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort,int nFlag)
	{
		if (0 == m_pCpMgr)
			return -1;

		return m_pCpMgr->OnLogin(m_ulKey,sLocalIp,nLocalPort,sPeerIp,nPeerPort,nFlag);
	}

	int OnClose(const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort)
	{
		if (0 == m_pCpMgr)
			return -1;

		return m_pCpMgr->OnClose(m_ulKey,sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	}

	//报文统计
	enum eServiceStaticType
	{
		ServiceUp,		//上行报文 本模块转发请求数		
		ServiceDown,			//下行报文 本模块收到的报文数量
		ServiceMaxStatic
	};

protected:
	virtual CProtocolComm* CreateCommInst() = 0;
	virtual void DestroyCommInst(CProtocolComm* p) = 0;

	CProtocolCpMgr*		m_pCpMgr;				//连接点管理器
	unsigned long		m_ulKey;				//本连接点的key标识
private:	
	CProtocolComm*		m_pProtocolComm;		//通讯处理器
	CConfig *	m_pCfg;							//配置接口

	unsigned int m_uiStatics[ServiceMaxStatic]; //报文统计
};

class CProtocolConnectPointClient:  public CProtocolConnectPoint
{
public:
	CProtocolConnectPointClient(){}
	virtual ~CProtocolConnectPointClient(){}

	int OnConnect(const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort, int nFlag){return m_pCpMgr->OnConnect(m_ulKey,sLocalIp,nLocalPort,sPeerIp,nPeerPort,nFlag);}
};

template<typename PROCESS>
class CProtocolCpCli:  public CProtocolConnectPointClient
{
public:
	CProtocolCpCli(){}
	~CProtocolCpCli(){}
private:
	CProtocolComm* CreateCommInst()
	{
		CProtocolComm* pProtocolComm = new CProtocolCommClientImp<PROCESS>();
		return pProtocolComm;
	}
	void DestroyCommInst(CProtocolComm* p){delete p;}
};

class CProtocolConnectPointServer:  public CProtocolConnectPoint
{
public:
	CProtocolConnectPointServer(){}
	virtual ~CProtocolConnectPointServer(){}

	int OnAccept(const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort){return m_pCpMgr->OnAccept(m_ulKey,sLocalIp,nLocalPort,sPeerIp,nPeerPort);}
};

template<typename PROCESS>
class CProtocolCpSvr:  public CProtocolConnectPointServer
{
public:
	CProtocolCpSvr(){}
	~CProtocolCpSvr(){}
private:
	CProtocolComm* CreateCommInst()
	{
		CProtocolComm* pProtocolComm = new CProtocolCommServerImp<PROCESS>();
		return pProtocolComm;
	}
	void DestroyCommInst(CProtocolComm* p){delete p;}
};
#endif