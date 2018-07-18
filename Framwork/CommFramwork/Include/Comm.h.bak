#ifndef _COMM_H
#define _COMM_H

#include "Logger.h"
#include "Gess.h"
#include "Config.h"
//#include <map>
//#include <vector>
//#include <deque>
//#include <list>
//#include <string>


#ifdef _WIN32
#ifdef COMM_EXPORT
#define COMM_API			__declspec(dllexport)
#define COMM_CLASS		__declspec(dllexport)
#else
#define COMM_API			__declspec(dllimport)
#define COMM_CLASS		__declspec(dllimport)
#endif
#else
#define COMM_API
#define COMM_CLASS
#endif

class CProtocolProcess;
class CProtocolComm;
//自定义结构 AIO注册KEY
typedef struct tagKeyContex
{
	CProtocolProcess *         pProcess;			//协议流程处理器
	CProtocolComm *            pProtocolComm;		//协议通讯处理器
	tsocket_t				   sSocket;				//
}KEY_CONTEXT, *PKEY_CONTEXT;

//报文接收缓存结构
typedef struct tagBuf
{
	unsigned int uiLen;
	char* pBuf;
	unsigned int uiUsed;
	tagBuf()
	{
		uiLen = 0;
		pBuf = 0;
		uiUsed = 0;
	}
} RCV_BUF,*PRCV_BUF;

//有明显启停步骤的基类
class COMM_CLASS CAction
{
public:
	enum eStage
	{
		esNotInit,
		esInit,
		esStart,
		esStop,
		esFinish
	};

	CAction():m_eStage(esNotInit) {}
	virtual ~CAction(){}

	virtual int Init(CConfig * pCfg) = 0;
	virtual void Finish() = 0;
	virtual int Start(void) = 0;
	virtual void Stop(void) = 0;
	
protected:
	eStage GetStage()
	{
		eStage e = esNotInit;
		m_mutexStage.Lock();
		e = m_eStage;
		m_mutexStage.Unlock();
		return e;
	}

	virtual bool StateCheck(eStage eNextStage)
	{
		bool blRtn = false;
		m_mutexStage.Lock();
		switch(eNextStage)
		{
		case esInit:
			if (m_eStage != esNotInit && m_eStage != esFinish)
			{
				blRtn = false;
			}
			else
			{
				m_eStage = esInit;
				blRtn = true;
			}
			break;
		case esStart:
			if (m_eStage == esStart)
			{
				blRtn = false;
			}
			else if (m_eStage == esInit || m_eStage == esStop)
			{
				m_eStage = esStart;
				blRtn = true;
			}
			else
			{
				blRtn = false;
			}
			break;
		case esStop:
			if (m_eStage == esStop)
				blRtn = false;
			else if (m_eStage != esStart)
				blRtn = false;
			else
			{
				m_eStage = esStop;
				blRtn = true;
			}
			break;
		case esFinish:
			if (m_eStage == esFinish)
				blRtn = false;
			else
			{
				if (m_eStage != esStop)
				{
					Stop();	
				}
				
				m_eStage = esFinish;
				blRtn = true;
			}
			break;
		default:
			break;
		}
		m_mutexStage.Unlock();

		return blRtn;
	}

	virtual void StateRollback()
	{
		m_mutexStage.Lock();
		switch(m_eStage)
		{
		case esInit:
			m_eStage = esNotInit;
			break;
		case esStart:
			m_eStage = esInit;
			break;
		case esStop:
			m_eStage = esStart;
			break;
		case esFinish:
			m_eStage = esStop;
			break;
		default:
			break;
		}
		m_mutexStage.Unlock();
	}
private:
	eStage m_eStage;
	CGessMutex m_mutexStage;
};

//报文基类 定义基本的编码和解码接口
class COMM_CLASS CPacket
{
public:
	virtual void  Decode(const char * pData, unsigned int nLength) = 0;
	virtual const char* Encode(unsigned int & usLen) = 0;
	virtual const string& GetCmdID() = 0;
};

//可路由报文即报文头或报文体携带路由信息的父类 定义获取路由键的接口
class COMM_CLASS CPacketRouteable:virtual public CPacket
{
public:
	virtual std::string RouteKey() = 0;
};

//连接点管理器基类 接口报文转发引擎
class COMM_CLASS CConnectPointManager
{
public:
	//用于报文转发的接口
	//GessPacket,转发的报文
	//sKey,接口标识
	virtual int Forward(CPacket &GessPacket,const unsigned long& ulKey) = 0;
};

//协议连接点管理器 接口报文转发引擎
class COMM_CLASS CProtocolCpMgr: virtual public CConnectPointManager
{
public:
	virtual int OnAccept(const unsigned long& ulKey,const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort) = 0;
	virtual int OnConnect(const unsigned long& ulKey,const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort,int nFlag) = 0;	
	virtual int OnLogin( const unsigned long& ulKey,const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort,int nFlag) = 0;
	virtual int OnClose( const unsigned long& ulKey,const string& sLocalIp, int nLocalPort, const string& sPeerIp, int nPeerPort) = 0;
};

//异步连接点基类 对应到各个接口抽象
class COMM_CLASS CConnectPointAsyn
{
public:
	virtual int Init(CConfig* pConfig) = 0;
	virtual int Start() = 0;
	virtual void Stop() = 0;
	virtual void Finish() = 0;

	//连接点下层实体收到报文向上(向内)转发的接口
	virtual int OnRecvPacket(CPacket &GessPacket) = 0;

	//连接点上层实体如连接点管理器向下(向外)转发报文的接口
	virtual int SendPacket(CPacket &GessPacket) = 0;

	//绑定连接点管理器接口
	virtual void Bind(CConnectPointManager* pCpMgr,const unsigned long& ulKey) = 0;
};

//同步连接点基类 对应到各个接口抽象
class COMM_CLASS CConnectPointSync
{
public:
	virtual int Init(CConfig* pConfig) = 0;
	virtual void Finish() = 0;

	//连接点上层实体如连接点管理器向下(向外)转发报文的接口
	virtual int SendPacket(CPacket &sndPacket,CPacket &rcvPacket,unsigned int uiTimeout) = 0;
	virtual void SetProxy(int nProxyType = 0, std::string sProxyHost = "", int nProxyPort = 0, std::string sProxyUser = "", std::string sProxyPassword = "") = 0;
};

//协议流程处理器基类
class COMM_CLASS CProtocolProcess
{
	friend class CProtocolComm;
public:
	CProtocolProcess(){}
	virtual ~CProtocolProcess(){}

	//真正向外(向下)转发报文前可能需处理，如缓存序列号，流控,供协议通讯处理器调用
	virtual int SendPacket(CPacket & packet) = 0;

	//断开连接时回调,以便协议流程处理
	virtual void OnClose() = 0;

	//
	virtual int Init(CConfig* pConfig) = 0;

	//同步接收数据时调用 供SOCKET AIO封装处理器调用
	virtual int OnRecv(char* pBuf,int nSize) = 0;

	//异步接收数据时调用 供SOCKET AIO封装处理器调用
	virtual int OnRecv() = 0;

	//获取接收缓存结构  供TCPSOCKET封装处理器调用
	virtual PRCV_BUF GetRcvBuf() = 0;

	//异步发送结果
	virtual int OnSend(int nSize) = 0;

	//把协议流程处理器绑定到协议通讯处理器
	virtual void Bind(CProtocolComm* p) = 0;

	//设置协议流程处理器对应的socket
	virtual void SocketID(tsocket_t sScoket) = 0;

	//获取协议流程处理器对应的socket
	virtual tsocket_t  SocketID() = 0;

	//设置对端协议实体信息
	virtual void SetPeerInfo(const string& sLocalIp,int nLocalPort,const string& sPeerIp,int nPeerPort) = 0;

	//获取对端协议实体信息
	virtual void GetPeerInfo(string& sLocalIp,int& nLocalPort,string& sPeerIp,int& nPeerPort) = 0;
};

//协议通讯处理器基类
class COMM_CLASS CProtocolComm: public CAction
{
	//friend class CProtocolProcess;
public:
	CProtocolComm(){}
	virtual ~CProtocolComm(){}

	//对上层应用层提供的下行报文主要接口函数 可能需要具备报文路由
	virtual int SendPacket(CPacket & packet) = 0;

	//应用层报文上传回调函数 供协议流程处理器解析报文后调用
	virtual int OnRecvPacket(CPacket & packet) = 0;

	//协议流程处理器要求中断时时调用
	virtual void ReqClose(CProtocolProcess* p) = 0;

	//对协议流程处理器提供的发送数据的接口
	virtual int SendData(CProtocolProcess* p,const char* pBuf,int nSize) = 0;

	//对AIO提供的接收数据的接口
	virtual int RecvData(PKEY_CONTEXT pKey) = 0;

	//绑定协议连接点 用于从远端收到的报文上传给应用层
	virtual void Bind(CConnectPointAsyn* pConnectPoint) = 0;

};

//异步IO基类 用于封装windows iocp,linux/unix epoll类异步机制
class COMM_CLASS CAio:virtual public CAction
{
public:
	CAio(){}
	virtual ~CAio()
	{
		while(!m_qKeyContexFree.empty())
		{
			PKEY_CONTEXT p = m_qKeyContexFree.front();
			m_qKeyContexFree.pop_front();
			delete p;
		}
		m_qKeyContexFree.clear();
	}

	virtual int Register(tsocket_t sSocket,void* p) = 0;
	virtual void UnRegister(tsocket_t sSocket) = 0;

	PKEY_CONTEXT GetKeyContex()
	{
		PKEY_CONTEXT p = 0;
		m_csKeyFreeLists.Lock();
		if (!m_qKeyContexFree.empty())
		{
			p = m_qKeyContexFree.front();
			m_qKeyContexFree.pop_front();
			m_csKeyFreeLists.Unlock();
			return p;
		}
		m_csKeyFreeLists.Unlock();

		p = new KEY_CONTEXT;
		return p;
	}

	void FreeKeyContex(PKEY_CONTEXT p)
	{
		if (0 == p)
			return;

		m_csKeyFreeLists.Lock();
		m_qKeyContexFree.push_back(p);
		m_csKeyFreeLists.Unlock();
		return;
	}
private:
	std::deque<PKEY_CONTEXT>	m_qKeyContexFree;
	CGessMutex	 m_csKeyFreeLists;
};

#endif