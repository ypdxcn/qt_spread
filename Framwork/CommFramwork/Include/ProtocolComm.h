#ifndef _PROTOCOLCOMM_H
#define _PROTOCOLCOMM_H

#include "Connector.h"
#include "Listener.h"
#include "Comm.h"
#include "Logger.h"
#include "strutils.h"
#include "osdepend.h"
#include <cassert>

using namespace std;
using namespace strutils;

//协议通讯处理器基类 
class CProtocolProcess;
class CProtocolConnectPoint;
class COMM_CLASS CProtocolCommImp: public CProtocolComm
{

private:
	class CProtocolCommNm: public CNmoModule
	{
	public:
		CProtocolCommNm():m_pParent(0){}
		virtual ~CProtocolCommNm(){}
		void Bind(CProtocolCommImp* pParent){m_pParent = pParent;}
		//单个查询接口
		int Query(CNMO& oNmo) const
		{
			if (0 == m_pParent)
				return -1;

			return m_pParent->Query(oNmo);
		}
		//批量查询接口
		int Query(vector< CNMO > & vNmo) const
		{
			if (0 == m_pParent)
				return -1;

			for (vector< CNMO >::iterator it = vNmo.begin(); it != vNmo.end(); ++it)
			{
				m_pParent->Query(*it);
			}
			return 0;
		}
		//控制接口
		int Operate(const string &sOid, int nflag, const string &sDstValue, const string &sParamer) 
		{
			if (0 == m_pParent)
				return -1;
			return m_pParent->Operate(sOid, nflag, sDstValue, sParamer);
		}
	private:
		CProtocolCommImp* m_pParent;
	};

	class CProtocolCommWorker: public CWorkThreadNm
	{
	public:
		CProtocolCommWorker():m_pParent(0){}
		~CProtocolCommWorker(){}
		void Bind(CProtocolCommImp* p){m_pParent = p;}
		void Enque(PKEY_CONTEXT pKey)
		{
			m_condMutexWorker.Lock();
			PAIR_CLOSE pa;
			pa.first = time(0);
			pa.second = pKey;
			m_dqClose.push_back(pa);
			m_condMutexWorker.Unlock();
			m_condMutexWorker.Signal();
		}
	private:
		int ThreadEntry()
		{
			try
			{
				while(!m_bEndThread)
				{
					m_condMutexWorker.Lock();
					while (m_dqClose.empty() && !m_bEndThread)
						m_condMutexWorker.Wait();

					if (m_bEndThread)
					{
						try
						{
							for (deque<PAIR_CLOSE>::iterator it = m_dqClose.begin(); it != m_dqClose.end(); ++it)
							{
								m_pParent->Handle((*it).second);
							}
						}
						catch(...)
						{
							CRLog(E_ERROR,"%s","Unknown exception!");
						}
						m_condMutexWorker.Unlock();
						break;
					}

					PAIR_CLOSE pa = m_dqClose.front();
					m_dqClose.pop_front();
					m_condMutexWorker.Unlock();

					int nDiff = static_cast<int>(difftime(time(0), pa.first));
					if (abs(nDiff) < 4)
					{
						_msleep(4-nDiff);
					}

					PKEY_CONTEXT pKey = pa.second;
					try
					{
						m_pParent->Handle(pKey);
					}
					catch(...)
					{
						CRLog(E_ERROR,"%s","Unknown exception!");
					}
				}

				CRLog(E_SYSINFO,"%s","ProtocolThread exit!");
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
		int End()
		{
			m_condMutexWorker.Signal();
			Wait();
			return 0;
		}
		//主动类线程状态是否需要被网管
		bool IsNetManaged(string& sKeyName)
		{
			sKeyName = "通讯管理器线程";
			return true;
		}
	private:
		CProtocolCommImp*	m_pParent;

		typedef pair<time_t,PKEY_CONTEXT> PAIR_CLOSE;
		//待关闭队列
		std::deque<PAIR_CLOSE> m_dqClose;
		CCondMutex m_condMutexWorker;
	};

	friend class CProtocolCommNm;
	friend class CProtocolCommWorker;
public:
	CProtocolCommImp();
	virtual ~CProtocolCommImp();

	virtual int Init(CConfig * pCfg);
	virtual void Finish();
	virtual int Start();
	virtual void Stop();
	virtual void Bind(CConnectPointAsyn* pConnectPoint);

	//应用层报文上传回调函数 供协议流程处理器解析报文后调用
	virtual int OnRecvPacket(CPacket & packet);
	
	//请求清理协议流程处理器,供发送/接收线程及协议流程处理器自身定时器调用
	void ReqClose(CProtocolProcess* p);

	//真正通过SOCKET发送数据的接口函数 供协议处理器调用
	virtual int SendData(CProtocolProcess* p,const char* pBuf,int nSize);
	//真正通过SOCKET接收的接口函数,供Aio调用
	virtual int RecvData(PKEY_CONTEXT pKey);

	//登录成功或失败后处理
	virtual int OnLogin(const string& sLocalIp,int nLocalPort,const string& sPeerIp,int nPeerPort,int nFlag=0);
protected:
	virtual CAio* CreateAioInstance() = 0;
	//清理协议处理器,由具体实现类实现
	virtual void CloseProcess(CProtocolProcess* pProcess) = 0;

	int RegisterAio(CProtocolProcess* p);
	PKEY_CONTEXT UnRegisterAio(CProtocolProcess* p);

	//网管单个查询接口
	virtual int Query(CNMO& oNmo) const = 0;
	//网管控制接口
	virtual int Operate(const string &sOid, int nflag, const string &sDstValue, const string &sParamer) = 0;

	//作为TCP协议连接点需要实现的接口 连接关闭后处理
	virtual int OnClose(const string& sLocalIp,int nLocalPort,const string& sPeerIp,int nPeerPort);

	//收发异步IO
	CAio* m_pAioReadWrite;
	//协议连接点
	CProtocolConnectPoint*	m_pConnectPoint;
	//通讯处理器网管对象
	CProtocolCommNm m_oNmoModule;
private:
	//协议处理器清除线程函数,由协议处理器清除线程对象调用
	int Handle(PKEY_CONTEXT pKey);

	//协议处理器清除线程对象
	CProtocolCommWorker	m_oWorker;

	//已注册socket及协议处理器/通讯处理器对
	map<tsocket_t,PKEY_CONTEXT>	m_mapRegisted;
	CGessMutex	m_csComm;
};

//作为客户端的协议通讯处理器类
//客户端在多socket连接的情况下，主要路由方式为路由表路由，暂未考虑
class COMM_CLASS CProtocolCommClient: public CProtocolCommImp
{
	friend class CConnector;
public:
	CProtocolCommClient();
	virtual ~CProtocolCommClient();

	virtual int Init(CConfig * pCfg);
	virtual void Finish();
	virtual int Start();
	virtual void Stop();
protected:
	//作为客户端需要实现的接口 连接服务端成功后处理
	virtual int OnConnect(tsocket_t sSocket,string sLocalIp,int nLocalPort,string sPeerIp,int nPeerPort,int nFlag=0);	
	
	//通知Conector重连
	void NotifyConnect();
	//连接状态
	int GetConnState() const;

	//配置信息
	CConfig *	m_pCfg;	
private:
	//目前连接状态
	int m_nConnState;

	//作为客户端的负责连接的对象 目前未考虑客户端和服务端支持多socket连接情况
	CConnector* m_pConnector;
};

///////////////////////////////////////////////////////////////////////////////

//作为服务端的协议通讯处理器类
class CProtocolConnectPointServer;
class COMM_CLASS CProtocolCommServer: public CProtocolCommImp
{
	friend  class CListener;
public:
	CProtocolCommServer();
	virtual ~CProtocolCommServer();
	
	virtual int Init(CConfig * pCfg);
	virtual void Finish();
	virtual int Start();
	virtual void Stop();

	//作为服务端需要实现的接口 用于源地址路由信息处理
	//服务端主要路由方式为源地址路由
	virtual void On1stPacket(CProtocolProcess* p,const string& sRouteKey) = 0;

protected:
	//作为服务端需要实现的接口 侦听器接受到新连接后处理
	virtual int OnAccept(tsocket_t sSocket,string sLocalIp,int nLocalPort,string sPeerIp,int nPeerPort);
	virtual CListener* CreateListener(CProtocolCommServer* p) = 0;
	virtual void DestroyListener(CListener* p) = 0;
	
	CConfig *	m_pCfg;
private:
	//作为服务端的侦听对象
	CListener* m_pListener;
};

//封装TCP短连接客户端的通信层
class COMM_CLASS CTcpShortComm
{
public: 
	CTcpShortComm():m_sSocket(INVALID_SOCKET),m_nState(E_DISCONNECTED)
	{
		m_ProxyData.nProxyType = PROXYTYPE_NOPROXY;
		m_ProxyData.sProxyHost = "";
		m_ProxyData.nProxyPort = 0;
		m_ProxyData.sProxyUser = "";
		m_ProxyData.sProxyPassword = "";
		m_ProxyData.bUseLogon = FALSE;
	}
	virtual ~CTcpShortComm(){}
public:	
	//连接状态
	typedef enum ConnectionState
	{
		E_CONNECTED,
		E_DISCONNECTED,
	} E_CONN_STATE;

	// 代理类型
	typedef enum tagProxyType
	{
		PROXYTYPE_NOPROXY,
		PROXYTYPE_HTTP11,
		PROXYTYPE_SOCKS4,
		PROXYTYPE_SOCKS5,
	}ProxyType;

	// 代理信息
	typedef struct tagProxyData
	{
		int nProxyType;
		std::string sProxyHost;
		int nProxyPort;
		std::string sProxyUser,sProxyPassword;
		BOOL bUseLogon;
	}ProxyData;

	//初始化 获对待IP和端口
	int Init(CConfig *	pConfig);

	void SetIp( const string& sIp, int nPort );

	//连接 0成功 -1失败 -100连接服务器失败
	int Connect(const string& sIp, int nPort);

	//连接 0成功 -1失败
	int Connect();

	//连接状态
	bool IsConnected();

	//断开
	void DisConnect();

	//发送解码后的数据
	int SendData(const char* pcBuf, int nLen);

	//接收数据 超时返回-1 正常返回接收到的字节数
	int RecvData(char* pcBuf,int& nLen,unsigned int uiTimeout);
	// 设置代理
	void SetProxy(int nProxyType = 0, std::string sProxyHost = "", int nProxyPort = 0, std::string sProxyUser = "", std::string sProxyPassword = "");
private:
	std::string m_sIp;
	int m_nPort;
	tsocket_t m_sSocket;
	int m_nState;
	CGessMutex m_mutexConnector;

	// 跟代理服务器的协商过程
	int LoginProxy();

	ProxyData m_ProxyData; // 记录代理信息的结构变量；
};

class CBase64Coding
{
public:

	// Construction

	CBase64Coding();
	virtual ~CBase64Coding();

	virtual BOOL Encode(const string &src, string &des); 
};

#endif