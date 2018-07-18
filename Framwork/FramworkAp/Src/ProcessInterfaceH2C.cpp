#include "ProcessInterfaceH2C.h"
#include "Logger.h"
#include "strutils.h"
using namespace strutils;

CProcessInterfaceH2C::GessPktInfo CProcessInterfaceH2C::m_GessPktInfo = 
{
	DEFAULT_IDLE_TIMEOUT,
	DEFAULT_HELLO_RESEND_INTERVAL,
	DEFAULT_HELLO_RESEND_COUNT,
	false
};
int CProcessInterfaceH2C::m_blGessPktInfoInited = false;

CProcessInterfaceH2C::CProcessInterfaceH2C()
:m_pCfg(0)
,m_uiCountNoAlive(0)
,m_uiCountSended(0)
{
}

CProcessInterfaceH2C::~CProcessInterfaceH2C(void)
{
}

int CProcessInterfaceH2C::Init(CConfig * pCfg)
{
	CTcpAppProcessClient::Init(pCfg);
	if (!m_blGessPktInfoInited)
	{
		m_blGessPktInfoInited = true;
		

		if (0 != pCfg)
		{
			string sLoginFlag;
			if (0 == pCfg->GetProperty("login",sLoginFlag))
			{
				if (1 == FromString<int>(sLoginFlag))
					m_GessPktInfo.blNeedLogin = true;
			}

			string sVal;
			if (0 == pCfg->GetProperty("max_idle",sVal))
			{
				m_GessPktInfo.ulIdleInterval = strutils::FromString<unsigned long>(sVal);
				if (m_GessPktInfo.ulIdleInterval > 300 || m_GessPktInfo.ulIdleInterval < 2)
					m_GessPktInfo.ulIdleInterval = DEFAULT_IDLE_TIMEOUT;
			}

			if (0 == pCfg->GetProperty("resend_interval",sVal))
			{
				m_GessPktInfo.ulIntervalReSend = strutils::FromString<unsigned long>(sVal);
				if (m_GessPktInfo.ulIntervalReSend > 60 || m_GessPktInfo.ulIntervalReSend < 2)
					m_GessPktInfo.ulIntervalReSend = DEFAULT_HELLO_RESEND_INTERVAL;
			}

			if (0 == pCfg->GetProperty("resend_count",sVal))
			{
				m_GessPktInfo.ulHelloReSend = strutils::FromString<unsigned long>(sVal);
				if (m_GessPktInfo.ulHelloReSend > 30 || m_GessPktInfo.ulHelloReSend < 1)
					m_GessPktInfo.ulHelloReSend = DEFAULT_HELLO_RESEND_COUNT;
			}
			CRLog(E_DEBUG, "MC:max_idle:%u, resend_interval:%u, resend_count:%u",m_GessPktInfo.ulIdleInterval, m_GessPktInfo.ulIntervalReSend, m_GessPktInfo.ulHelloReSend);


			pCfg->GetProperty("user_type",m_GessPktInfo.user_type);
			pCfg->GetProperty("user_id",m_GessPktInfo.user_id);
			pCfg->GetProperty("branch_id",m_GessPktInfo.branch_id);
			pCfg->GetProperty("user_pwd",m_GessPktInfo.user_pwd);
		}
	}

	m_pCfg = pCfg;
	m_uiCountNoAlive = 0;
	m_uiCountSended = 0;
	return 0;
}

/******************************************************************************
函数描述:协议报文处理回调函数,获取协议定义的报文格式信息
调用函数:父类对象回调
输出参数:PacketInfo & stInfo 报文格式信息
        eLengthType 长度字段类型 整型/10进制字符串/16进制字符串
		nLengthBytes 消息包长度字段的长度,为整数时只支持 1 2 4字节
		nLengthPos 长度字段起始位置
		blLenIncludeHeader 长度是否含报文头
		nFixHeadLen  除长度字段指示的长度外,报文固定部分长度
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CProcessInterfaceH2C::GetPacketInfo(PacketInfo & stInfo)
{
	stInfo.eLengthType = ltCharactersDec;
	stInfo.nLengthPos = 0;
	stInfo.nLengthBytes = BROADCAST_LENGTH_BYTES;
	stInfo.blLenIncludeHeader = false;
	stInfo.nFixHeadLen = stInfo.nLengthPos + stInfo.nLengthBytes;
}

/******************************************************************************
函数描述:协议报文处理最主要函数,根据报文命令字进行直接处理或转发
调用函数:父类对象回调
被调函数:包装类接口函数,上传报文
输入参数:char * pData接收缓存, size_t nSize缓存大小
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CProcessInterfaceH2C::OnPacket(char * pData, size_t nSize)
{
	CBroadcastPacket GessPacket;
	GessPacket.Decode(pData, nSize);

	m_csH2C.Lock();
	m_uiCountNoAlive = 0;
	m_csH2C.Unlock();

	std::string sCmdID = GessPacket.GetCmdID();
	if (sCmdID == "ConnectTest")
	{
		return OnHello(GessPacket);
	}
	else if (sCmdID == "ConnectTestRsp")
	{
		return 0;
	}
	else
	{
		//CRLog(E_PROINFO,GessPacket.Print().c_str());
		OnRecvPacket(GessPacket);
	}
	return 0;
}


int CProcessInterfaceH2C::OnConnect()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"H2C OnConnect socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	CreateTimer(m_GessPktInfo.ulIdleInterval);
	return 0;
}

//处理定时器超时
int CProcessInterfaceH2C::HandleTimeout(unsigned long& ulTmSpan)
{
	int nRtn = 0;
	m_csH2C.Lock();
	if (m_uiCountNoAlive >= 1)
	{//超过链路最大空闲时间未收到报文，则发送心跳
		//if (m_uiCountSended >= m_GessPktInfo.ulHelloReSend)
		//{//重发心跳次数超过规定次数则准备关闭
		//	nRtn = -1;
		//	CRLog(E_PROINFO,"H2%s(%u-%u)","心跳超时!",m_uiCountNoAlive,m_uiCountSended);
		//	ReqClose();
		//}
		//else
		//{//重置定时器间隔
			ulTmSpan = m_GessPktInfo.ulIntervalReSend;
			m_uiCountSended++;

			//if (m_uiCountSended > 1)
			//	CRLog(E_PROINFO,"H2发送心跳,第%d次!",m_uiCountSended);
			
			if (0 > SendHello())
				nRtn = -1;
		//}
	}
	else
	{
		m_uiCountSended = 0;
		ulTmSpan = m_GessPktInfo.ulIdleInterval;
	}
	m_uiCountNoAlive++;
	m_csH2C.Unlock();
	return nRtn;
}

/******************************************************************************
函数描述:socket连接中断后则被通知,可根据协议要求进行处理
调用函数:通讯处理器对象回调
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CProcessInterfaceH2C::OnClose()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"H2C OnClose socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	DestroyTimer();
	return;
}

//对外提供的主要接口函数,供通讯处理器发送报文时调用
int CProcessInterfaceH2C::SendPacket(CPacket & SendGessPacket)
{
	try
	{
		CBroadcastPacket & pkt = dynamic_cast<CBroadcastPacket&>(SendGessPacket);
		
		unsigned int  nLen = 0;
		const char* pcBuf = pkt.Encode(nLen);
				
		CRLog(E_PROINFO, "H2C %s",pcBuf);
		m_csH2C.Lock();
		int nSend = SendData(pcBuf,nLen);
		m_csH2C.Unlock();
		if (nSend < 0)
		{
			//CRLog(E_ERROR,"SendData err:%s",pcBuf);
			return -1;
		}		
		return 0;
	}
	catch(std::bad_cast)
	{
		CRLog(E_ERROR,"%s","packet error!");
		return -1;
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

int CProcessInterfaceH2C::OnSend(int nSize)
{
	return 0;
}

//接口间心跳报文
int CProcessInterfaceH2C::SendHello()
{
	CBroadcastPacket pkt("Hello");

	unsigned int nLen = 0;
	const char* pcBuf = pkt.Encode(nLen);

	m_csH2C.Lock();
	int nSend = SendData( pcBuf, nLen);
	m_csH2C.Unlock();

	if (nSend < 0)
		return -1;

	return 0;
}

int CProcessInterfaceH2C::OnHello(CBroadcastPacket& pktHello)
{
	CBroadcastPacket pkt("ConnectTestRsp");

	unsigned int nLen = 0;
	const char* pcBuf = pkt.Encode(nLen);

	m_csH2C.Lock();
	int nSend = SendData( pcBuf, nLen);
	m_csH2C.Unlock();

	if (nSend < 0)
		return -1;

	return 0;
}


int CProcessInterfaceH2C::GetNmKey(string& sKey)
{
	sKey = "H2接口客户端连接";
	return 0;
}