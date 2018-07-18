#ifndef _PROTOCOL_PROCESS_H
#define _PROTOCOL_PROCESS_H

#include "ProtocolComm.h"
#include "GessTimerMgrPosix.h"
#include "GessTime.h"
#include "Comm.h"
#include <algorithm>

#define MAX_PACKET_SIZE						20480

enum eProcessStaticType
{
	eProcessPktRecv,		//接收到的流程报文数量
	eProcessPktRecvFwd,		//发送的转发报文数量
	eProcessPktSend,		//发送的流程报文数量
	eProcessPktSendFwd,		//发送的转发报文数量
	eProcessMaxStatic
};

enum eCommStaticType
{
	eCommBytesRecv = 0,		//接收数据 本模块收到正确报文数据字节
	eCommBytesRecvAll,		//接收数据 本模块收到总数据字节
	eCommCountRecvErr,		//接收数据错误次数 本模块收到乱包次数
	eCommBytesSend,			//发送数据 本模块发送的数据字节
	eCommCountSendErr,		//发送数据 本模块发送失败的次数
	eCommMaxStatic
};

//class CProtocolComm;

class COMM_CLASS CAppProcess:virtual public CProtocolProcess
{
private:
	class CProcessTimer: public CGessTimer
	{
	public:
		CProcessTimer():m_pParent(0){}
		virtual ~CProcessTimer(){}
		void Bind(CAppProcess* p)
		{
			m_pParent=p;
		}
		int TimeOut(const string& ulKey,unsigned long& ulTmSpan)
		{
			if (0 != m_pParent)
				return m_pParent->OnTimeout(ulKey,ulTmSpan);
			return -1;
		}
		void TimerCanceled(const string& ulKey)
		{
		}
	private:
		CAppProcess* m_pParent;
	};
public:
	CAppProcess():m_pComm(0),m_sSocket(INVALID_SOCKET),m_uiOutNowStat(0),m_uiOutAveBytes(0)
	{
		for (int i = 0; i < eProcessMaxStatic; i++)
		{
			m_uiStatics[i] = 0;
		}

		m_oProcessTimer.Bind(this);
	}
	virtual ~CAppProcess(){}

	
	
	//把协议通讯处理器绑定到协议流程处理器
	void Bind(CProtocolComm* p){m_pComm = p;}

	//设置/获取协议流程处理器对应的socket
	void SocketID(tsocket_t sScoket){m_sSocket = sScoket;}
	tsocket_t  SocketID(){ return m_sSocket;}
	void SetPeerInfo(const string& sLocalIp,int nLocalPort,const string& sPeerIp,int nPeerPort)
	{
		m_sLocalIp = sLocalIp;
		m_nLocalPort = nLocalPort;
		m_sPeerIp = sPeerIp;
		m_nPeerPort = nPeerPort;
	}
	void GetPeerInfo(string& sLocalIp,int& nLocalPort,string& sPeerIp,int& nPeerPort)
	{
		sLocalIp = m_sLocalIp;
		nLocalPort = m_nLocalPort;
		sPeerIp = m_sPeerIp;
		nPeerPort = m_nPeerPort;
	}
	
	//获取协议流程处理器网管key
	virtual  int GetNmKey(string& sKey) = 0;
protected:
	virtual int OnLogin(int nFlag) = 0;	

	//发送数据
	int SendData(const char* pBuf,int nSize)
	{
		assert(0 != m_pComm);
		if (0 == m_pComm)
		{
			CRLog(E_ERROR,"%s","0 Comm");
			return -1;
		}

		m_csStatics.Lock();
		int nInterval = m_oBeginStatTm.IntervalToNow();
		if (nInterval >= 5)
		{
			m_uiOutAveBytes = m_uiOutNowStat / nInterval;
			m_oBeginStatTm.ToNow();
			m_uiOutNowStat = 0;
		}
		m_uiOutNowStat += nSize;
		m_csStatics.Unlock();

		int nRtn = m_pComm->SendData(this,pBuf,nSize);
		return nRtn;
	}
	
	void ReqClose()
	{
		m_pComm->ReqClose(this);
	}

	//更新协议层报文统计数据
	void UpdateStatics(eProcessStaticType eType,int nCount = 1)
	{
		if (eProcessMaxStatic == eType)
			return;

		m_uiStatics[eType] += nCount;

		//do something?

		m_uiStatics[eType] += nCount;
	}

	//取协议层报文统计数据
	void GetProcessStatics(unsigned int	uiStatics[eProcessMaxStatic])
	{
		for (int i = 0; i < eProcessMaxStatic; i++)
		{
			uiStatics[i] = m_uiStatics[i];
		}
	}

	//取通信出流量数据
	void GetOutFlowStatics(unsigned int & uiOutNowStat,unsigned int & uiOutAveBytes)
	{
		m_csStatics.Lock();
		int nInterval = m_oBeginStatTm.IntervalToNow();
		if (nInterval >= 5)
		{
			m_uiOutAveBytes = m_uiOutNowStat / nInterval;
			m_oBeginStatTm.ToNow();
			m_uiOutNowStat = 0;
		}

		uiOutAveBytes = m_uiOutAveBytes;
		uiOutNowStat = m_uiOutNowStat;
		m_csStatics.Unlock();
	}

	int CreateTimer(unsigned long& ulTmSpan){return CGessTimerMgrImp::Instance()->CreateTimer(&m_oProcessTimer,ulTmSpan,"PTimer");}
	void DestroyTimer() {CGessTimerMgrImp::Instance()->DestroyTimer(&m_oProcessTimer,"PTimer");}
	virtual int HandleTimeout(unsigned long& ulTmSpan) = 0;

	CProtocolComm* m_pComm;
private:
	tsocket_t m_sSocket;
	string m_sLocalIp;
	int m_nLocalPort;
	string m_sPeerIp;
	int m_nPeerPort;

	//接口心跳状态定时器
	int OnTimeout(const string& ulKey,unsigned long& ulTmSpan){return HandleTimeout(ulTmSpan);}

	//定时器
	CProcessTimer	m_oProcessTimer;

	//协议业务层报文统计处理
	unsigned int	m_uiLastStatics[eProcessMaxStatic];
	unsigned int	m_uiStatics[eProcessMaxStatic];

	unsigned int	m_uiOutNowStat;
	unsigned int	m_uiOutAveBytes;
	CGessTime		m_oBeginStatTm;
	CGessMutex		m_csStatics;
};

class COMM_CLASS CAppProcessClient:virtual public CAppProcess
{
public:
	CAppProcessClient():CAppProcess(){}
	virtual ~CAppProcessClient(){}
protected:
	//收到的完整报文需要向上转发
	int OnRecvPacket(CPacket& pkt){return m_pComm->OnRecvPacket(pkt);}
	
	int OnLogin(int nFlag)
	{
		CProtocolCommClient* p = dynamic_cast<CProtocolCommClient*>(m_pComm);
		if (0 == p)
			return -1;

		string sLocalIp;
		int nLocalPort;
		string sPeerIp;
		int nPeerPort;
		GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
		return p->OnLogin(sLocalIp, nLocalPort, sPeerIp, nPeerPort, nFlag);
	}

	//作客户端连接成功后回调
	virtual int OnConnect() = 0;
};

class COMM_CLASS CAppProcessServer:virtual public CAppProcess
{
public:
	CAppProcessServer():CAppProcess(){}
	virtual ~CAppProcessServer(){m_lstKeys.clear();}

	//作服务端接收到连接后回调
	virtual int OnAccept() = 0;

	//获取key列表
	const std::list<std::string>& Keys(){return m_lstKeys;}
	//删除key
	void RemoveKey(std::string sKey){m_lstKeys.remove(sKey);}
	
protected:
	
	//收到的完整报文需要向上转发
	int OnRecvPacket(std::string sRouteKey,CPacket& pkt)
	{
		//路由信息维护
		if (Is1stPacket(sRouteKey))
		{
			CProtocolCommServer* p = dynamic_cast<CProtocolCommServer*>(m_pComm);
			if (p != 0)
			{
				p->On1stPacket(this,sRouteKey);
			}
		}

		//上层转发
		return m_pComm->OnRecvPacket(pkt);
	}

	int OnLogin(int nFlag)
	{
		CProtocolCommServer* p = dynamic_cast<CProtocolCommServer*>(m_pComm);
		if (0 == p)
			return -1;

		string sLocalIp;
		int nLocalPort;
		string sPeerIp;
		int nPeerPort;
		GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
		return p->OnLogin(sLocalIp, nLocalPort, sPeerIp, nPeerPort, nFlag);
	}
private:
	//根据关键字判断是否首个报文1是0否
	bool Is1stPacket(const std::string & sKey)
	{
		bool blRtn = false;
		std::list<std::string>::const_iterator it;
		it = find(m_lstKeys.begin(),m_lstKeys.end(),sKey);
		if (it == m_lstKeys.end())
		{
			m_lstKeys.push_back(sKey);
			blRtn = true;
		}
		return blRtn;
	}

	std::list<std::string> m_lstKeys;
};

//报文头用途:0 帧对齐 1协议版本
enum HeadFlag
{
	hfFrameHeader = 0,
	hfProtocalVer = 1,
	hfNoUsed
};

//报文尾是否校验
enum TailFlag
{
	tfFrameCheck = 0,
	tfNoUsed
};

//报文长度字段类型
enum LengthType
{
	ltInterger = 0,		//整数
	ltCharactersDec,	//字符串10进制
	ltCharactersHex		//字符串16进制
};

//报文头信息 由派生的具体协议类通过回调提供
typedef struct stPacketInfo
{
	size_t		nLengthBytes;		// 消息包长度字段的长度,为整数时只支持 1 2 4字节
	size_t		nLengthPos;			// 消息包长度字段在包头的位置 从0开始计数
	LengthType	eLengthType;		// 消息包长度字段的类型
	int			blLenIncludeHeader;	// 长度是否含报文头
	size_t		nFixHeadLen;		// 长度字段表示的长度之外的固定报文头长度(含长度字段本身)

	HeadFlag	eHeadFlag;			// 报文头用途标识
	size_t		nLengthHeadFlag;	// 报文头标识占用字节
	TailFlag	eTailFlag;			// 报文尾用途标识
	size_t		nLengthTailFlag;	// 报文尾标识占用字节
} PacketInfo,*PPacketInfo;

//TCP处理基类
class COMM_CLASS CTcpHandler
{
public:
	CTcpHandler(){}
	virtual ~CTcpHandler(){}

	//只初始化一次报文头信息
	int InitPacketInfo()
	{
		//if (!m_blPacketInfoInited)
			GetPacketInfo(m_PacketInfo);

		return 0;
	}
protected:
	virtual void GetPacketInfo(PacketInfo & stInfo) = 0;	// 子类必须提供报文有关长度格式信息
	
	//根据收到的部分数据计算报文长度
	virtual size_t GetPacketLength(const char* pPacketData);

	/*static*/ PacketInfo	m_PacketInfo;			//报文头信息
	//static bool			m_blPacketInfoInited;	//是否已初始化
private:

};


class COMM_CLASS CTcpHandlerLong: virtual public CTcpHandler
{
public:
	CTcpHandlerLong();
	virtual ~CTcpHandlerLong();
	
	int OnRecvTcp(char* pBuf,int nSize);
	int OnRecvTcp();
	PRCV_BUF GetRcvBufTcp(){return &m_stRcvBuf;}

protected:
	virtual int OnPacket(char * pData, size_t nSize) = 0;	// 收到完整报文后回调
	
	//通信层报文统计处理
	void UpdateStatics(eCommStaticType eType, int nCount = 1); 
	void GetInFlowStatics(unsigned int & uiInNowStat,unsigned int & uiInAveBytes)
	{
		m_csStatics.Lock();
		int nInterval = m_oBeginStatTm.IntervalToNow();
		if (nInterval >= 5)
		{
			m_uiInAveBytes = m_uiInNowStat / nInterval;
			m_oBeginStatTm.ToNow();
			m_uiInNowStat = 0;
		}

		uiInNowStat = m_uiInNowStat;
		uiInAveBytes = m_uiInAveBytes;
		m_csStatics.Unlock();
	}
private:
	unsigned int	m_uiLastStatics[eCommMaxStatic];
	unsigned int	m_uiStatics[eCommMaxStatic];

	unsigned int	m_uiInNowStat;
	unsigned int	m_uiInAveBytes;
	CGessTime	m_oBeginStatTm;
	CGessMutex		m_csStatics;


	RCV_BUF     m_stRcvBuf;				// 接收缓冲结构，指向真正缓冲地址
	size_t m_nLength;					// 待接收报文长度
	size_t m_nIndex;					// 已接收报文位置
	char m_szBuf[MAX_PACKET_SIZE];		// 接收缓冲
};

class COMM_CLASS CTcpAppProcessClient:virtual public CAppProcessClient,virtual public CTcpHandlerLong
{
public:
	CTcpAppProcessClient():CAppProcessClient(),CTcpHandler(){};
	virtual ~CTcpAppProcessClient(){};

	int OnRecv(char* pBuf,int nSize){return OnRecvTcp(pBuf,nSize);}
	int OnRecv(){return OnRecvTcp();}
	PRCV_BUF GetRcvBuf(){return GetRcvBufTcp();}

	virtual int Init(CConfig * pCfg){return InitPacketInfo();}

	//取流量统计接口
	void GetFlowStatics(unsigned int & uiInNowStat,unsigned int & uiInAveBytes,unsigned int & uiOutNowStat,unsigned int & uiOutAveBytes)
	{
		GetInFlowStatics(uiInNowStat,uiInAveBytes);
		GetOutFlowStatics(uiOutNowStat,uiOutAveBytes);
	}
};


class COMM_CLASS CTcpAppProcessServer:virtual public CAppProcessServer,virtual public CTcpHandlerLong
{
public:
		CTcpAppProcessServer():CAppProcessServer(),CTcpHandler(){};
		virtual ~CTcpAppProcessServer(){};

		int OnRecv(char* pBuf,int nSize){return OnRecvTcp(pBuf,nSize);}
		int OnRecv(){return OnRecvTcp();}
		PRCV_BUF GetRcvBuf(){return GetRcvBufTcp();}

		virtual int Init(CConfig * pCfg){return InitPacketInfo();}

		//取流量统计接口
		void GetFlowStatics(unsigned int & uiInNowStat,unsigned int & uiInAveBytes,unsigned int & uiOutNowStat,unsigned int & uiOutAveBytes)
		{
			GetInFlowStatics(uiInNowStat,uiInAveBytes);
			GetOutFlowStatics(uiOutNowStat,uiOutAveBytes);
		}
protected:
	
};


#define MAX_TCP_SHORT_PACKET_SIZE			4096
//封装TCP短连接协议流程
class COMM_CLASS CTcpShortProcess: virtual public CTcpHandler
{
public:
	CTcpShortProcess(){}
	virtual ~CTcpShortProcess(){}
	//初始化
	virtual int Init(CConfig* pCfg){m_pCfg = pCfg; return InitPacketInfo();}
	//发送报文特别的处理 PreSendHandle具体处理可能是加密,压缩, 若无需任何处理则返回0,若出错返回-1,否则返回实际发送的字节数
	virtual int PreSendHandle(CPacket &sndPacket) = 0;
	//接收数据特别的处理 SufRecvHandle具体处理可能是解密,解压缩, 若无需任何处理则返回0,若出错返回-1,否则返回1 
	virtual int SufRecvHandle(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket) = 0;
	//发送报文
	virtual int SendPacket(CPacket &sndPacket,CPacket &rcvPacket,unsigned int uiTimeout)
	{
		assert(0 != m_pComm);
		if (0 == m_pComm)
			return -1;

		int nRtn = PreSendHandle(sndPacket);

        // modified by Jerry Lee, 2011-5-12, 修正预处理成功后无法接收报文的bug
        /*
		if (0 > nRtn)
		{
			return -1;
		}
		else if (0 < nRtn)
		{
			return 0;
		}
		else
		{
			//发送请求报文
			unsigned int  nLen = 0;
			const char* pcBuf = sndPacket.Encode(nLen);
			nRtn = m_pComm->SendData(pcBuf,nLen);
			if (-1 == nRtn)
			{
				CRLog(E_ERROR,"%s","SendData err");
				return -1;
			}
		}
        */

        //
        if (0 > nRtn)
        {
            return -1;
        }
        else if (0 < nRtn)
        {
            // 继续执行以接收报文
        }
        else
        {
            //发送请求报文
            unsigned int  nLen = 0;
            const char* pcBuf = sndPacket.Encode(nLen);
            nRtn = m_pComm->SendData(pcBuf,nLen);
            if (-1 == nRtn)
            {
                CRLog(E_ERROR,"%s","SendData err");
                return -1;
            }
        }
	
		//接收应答报文
		nRtn = -1;
		time_t tmLast,tmNow;
		time(&tmLast); 
		unsigned int nTimeLeft = uiTimeout;
		size_t nPktLen = 0;
		
		std::list<RCV_BUF*> lstBuf;
		RCV_BUF * pBufNow = new RCV_BUF;
		pBufNow->pBuf = new char[MAX_TCP_SHORT_PACKET_SIZE];
		pBufNow->uiLen = MAX_TCP_SHORT_PACKET_SIZE;
		pBufNow->uiUsed = 0;
		lstBuf.push_back(pBufNow);

		char* pBuf = pBufNow->pBuf;
		int nLenLeft = pBufNow->uiLen;
		size_t nReadTotal = 0;

		CRLog(E_DEBUG,"%s","Begin Recv Data");
		do
		{
			//modify by zhangwei 2010.04.20 清算文件下载时金交所若socket不关闭则永远等待问题
			//int nRecv = m_pComm->RecvData(pBuf,nLenLeft,uiTimeout);

			//单次socket接收数据超时时间
// 			unsigned int uiTimeoutRcv = 30;
			int nRecv = m_pComm->RecvData(pBuf,nLenLeft, uiTimeout);
			if (-1 == nRecv)
			{
				break;
			}

			time(&tmNow);
			nTimeLeft = uiTimeout - static_cast<int>(difftime(tmNow,tmLast));
			if ( nTimeLeft <= 0 )
			{

				break;
			}

			CRLog(E_DEBUG,"Recv Bytes:%d,Total:%d,Left time:%d ",nRecv,nReadTotal,nTimeLeft);

			pBuf += nRecv;
			nReadTotal += nRecv;
			pBufNow->uiUsed += nRecv;
			nLenLeft -= nRecv;
			if (nLenLeft <= 0)
			{				
				pBufNow = new RCV_BUF;
				pBufNow->pBuf = new char[MAX_TCP_SHORT_PACKET_SIZE];
				pBufNow->uiLen = MAX_TCP_SHORT_PACKET_SIZE;
				pBufNow->uiUsed = 0;
				lstBuf.push_back(pBufNow);

				pBuf = pBufNow->pBuf;
				nLenLeft = pBufNow->uiLen;
			}

			if (nReadTotal < m_PacketInfo.nLengthBytes)
			{//需要收到至少报文长度部分字节
				continue;
			}

			if (0 == nPktLen)
			{//根据收到的数据部分确定包长
				char * pLenBuf = new char[m_PacketInfo.nLengthBytes];
				char * pTmp = pLenBuf;
				unsigned int nCopyLeft = m_PacketInfo.nLengthBytes;

				std::list<RCV_BUF*>::iterator it;				
				for (it = lstBuf.begin(); it != lstBuf.end();++it)
				{
					if ((*it)->uiUsed >= nCopyLeft)
					{
						memcpy(pTmp,(*it)->pBuf,nCopyLeft);
						pTmp += nCopyLeft;
						nCopyLeft = 0;
						break;
					}
					else
					{
						memcpy(pTmp,(*it)->pBuf,(*it)->uiUsed);
						pTmp += (*it)->uiUsed;
						nCopyLeft -= (*it)->uiUsed;

						if (nCopyLeft <= 0)
						{
							break;
						}
					}
				}

				nPktLen = GetPacketLength(pLenBuf);
				delete []pLenBuf;
				if (0 == nPktLen)
				{
					break;
				}
			}

			if (nReadTotal >= nPktLen)
			{				
				break;
			}
		} while (true);

		CRLog(E_DEBUG,"Recv Bytes Total:%d",nReadTotal);

		char * pBufTotal = new char[nReadTotal];
		char * pTmp = pBufTotal;
		std::list<RCV_BUF*>::iterator it;				
		for (it = lstBuf.begin(); it != lstBuf.end();)
		{
			if ((*it)->uiUsed > 0)
			{
				memcpy(pTmp,(*it)->pBuf,(*it)->uiUsed);
				pTmp += (*it)->uiUsed;
			}
			
			delete []((*it)->pBuf);
			delete (*it);
			lstBuf.erase(it++);
		}

		if (nReadTotal > 0)
		{
			nRtn = SufRecvHandle(pBufTotal,nReadTotal,rcvPacket);
			if (0 > nRtn)
			{
				nRtn = -1;
			}
			else if (0 < nRtn)
			{
				nRtn = 0;
			}
			else
			{
				rcvPacket.Decode(pBufTotal,nReadTotal);
				nRtn = 0;
			}
		}
		delete []pBufTotal;
		return nRtn;
	}
	void Bind(CTcpShortComm* pComm){m_pComm=pComm;}
protected:
	int SendData(const char* pcBuf, int nLen) {return m_pComm->SendData(pcBuf,nLen);}
	CConfig* m_pCfg;
private:
	CTcpShortComm*	m_pComm;

};

#endif