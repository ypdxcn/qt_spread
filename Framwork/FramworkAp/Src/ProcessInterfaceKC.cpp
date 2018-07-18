#include "ProcessInterfaceKC.h"
#include "IpcPacket.h"
#include "Logger.h"
#include "ApConstant.h"
using namespace ApConst;

CProcessInterfaceKC::GessPktInfo CProcessInterfaceKC::m_GessPktInfo = 
{
	DEFAULT_IDLE_TIMEOUT,
	DEFAULT_HELLO_RESEND_INTERVAL,
	DEFAULT_HELLO_RESEND_COUNT,
	false
};
int CProcessInterfaceKC::m_blGessPktInfoInited = false;

CProcessInterfaceKC::CProcessInterfaceKC()
:m_pCfg(0)
,m_uiPacketSnd(0)
,m_uiCountNoAlive(0)
,m_uiCountSended(0)
{
}

CProcessInterfaceKC::~CProcessInterfaceKC(void)
{
}

int CProcessInterfaceKC::Init(CConfig * pCfg)
{
	CTcpAppProcessClient::Init(pCfg);
	if (!m_blGessPktInfoInited)
	{
		m_blGessPktInfoInited = true;
		

		
		string sLoginFlag;
		if (0 == pCfg->GetProperty("login",sLoginFlag))
		{
			if (1 == FromString<int>(sLoginFlag))
				m_GessPktInfo.blNeedLogin = true;
		}

		CConfig* pCfgGlobal = pCfg->GetCfgGlobal();
		pCfgGlobal->GetProperty("node_type",m_GessPktInfo.node_type);
		pCfgGlobal->GetProperty("node_id",m_GessPktInfo.node_id);
		pCfgGlobal->GetProperty("host_id",m_GessPktInfo.node_name);
		pCfgGlobal->GetProperty("host_id",m_GessPktInfo.host_id);	
	}

	m_pCfg = pCfg;	
	return 0;
}

/******************************************************************************
函数描述:获取协议定义的报文格式信息
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
void CProcessInterfaceKC::GetPacketInfo(PacketInfo & stInfo)
{
	stInfo.eLengthType = ltCharactersDec;
	stInfo.nLengthPos = 0;
	stInfo.nLengthBytes = IPC_LENGTH_BYTES;
	stInfo.blLenIncludeHeader = false;
	stInfo.nFixHeadLen = stInfo.nLengthPos + stInfo.nLengthBytes;
}

/******************************************************************************
函数描述:协议报文处理最主要函数,根据报文命令字进行直接处理或转发
调用函数:父类对象回调
被调函数:OnRecvPacket,上传报文
输入参数:char * pData接收缓存, size_t nSize缓存大小
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CProcessInterfaceKC::OnPacket(char * pData, size_t nSize)
{
	CIpcPacket GessPacket;
	GessPacket.Decode(pData, nSize);

	m_csKC.Lock();
	m_uiCountNoAlive = 0;
	m_csKC.Unlock();

	std::string sCmdID = GessPacket.GetCmdID();
	if (sCmdID == "Hello")
	{		
		return OnHello(GessPacket);
	}
	else if (sCmdID == "HelloRsp")
	{
		//CRLog(E_PROINFO,"P3C收到心跳应答:%s",GessPacket.Print().c_str());
		return 0;
	}

	CRLog(E_PROINFO,"Recv:%s",GessPacket.Print().c_str());

	OnRecvPacket(GessPacket);
	return 0;
}

int CProcessInterfaceKC::OnConnect()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"KC OnConnect socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	//CRLog(E_PROINFO,"P3C OnConnect and CreateTimer");
	//CreateTimer(m_GessPktInfo.ulIdleInterval);
	return 0;
}

/******************************************************************************
函数描述:socket连接中断后则被通知,可根据协议要求进行处理
调用函数:通讯处理器对象回调
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CProcessInterfaceKC::OnClose()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"KC OnClose socket (%d) Peer(%s:%d),Me(%s:%d)",SocketID(),sPeerIp.c_str(),nPeerPort,sLocalIp.c_str(),nLocalPort);
	//DestroyTimer();
}

//对外提供的主要接口函数,供通讯处理器发送报文时调用
int CProcessInterfaceKC::SendPacket(CPacket & SendGessPacket)
{
	try
	{
		CIpcPacket & pkt = dynamic_cast<CIpcPacket&>(SendGessPacket);

		unsigned int  nLen = 0;
		const char* pcBuf = pkt.Encode(nLen);
		
		bool blLog = false;
		m_csKC.Lock();
		int nSend = SendData(pcBuf,nLen);
		m_uiPacketSnd++;
		if (m_uiPacketSnd % 30 == 0)
		{
			m_uiPacketSnd = 0;
			blLog = true;
		}
		m_csKC.Unlock();

		if (blLog)
		{
			CRLog(E_PROINFO,"Send:%s",pcBuf);
		}

		if (nSend < 0)
		{
			CRLog(E_ERROR,"%s","SendData err");
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

int CProcessInterfaceKC::OnSend(int nSize)
{

	return 0;
}

//处理定时器超时
int CProcessInterfaceKC::HandleTimeout(unsigned long& ulTmSpan)
{
	int nRtn = 0;
	m_csKC.Lock();
	if (m_uiCountNoAlive >= 1)
	{//超过链路最大空闲时间未收到报文，则发送心跳
		if (m_uiCountSended >= m_GessPktInfo.ulHelloReSend)
		{//重发心跳次数超过规定次数则准备关闭
			nRtn = -1;
			CRLog(E_ERROR,"%s","心跳超时!");
			ReqClose();
		}
		else
		{//重置定时器间隔
			ulTmSpan = m_GessPktInfo.ulHelloReSend;
			m_uiCountSended++;

			if (m_uiCountSended > 1)
				CRLog(E_PROINFO,"P3C发送心跳,第%d次!",m_uiCountSended);

			if (0 > SendHello())
				nRtn = -1;
		}
	}
	else
	{
		m_uiCountSended = 0;
		ulTmSpan = m_GessPktInfo.ulIdleInterval;
	}
	m_uiCountNoAlive++;
	m_csKC.Unlock();
	return nRtn;
}

//发送心跳
int CProcessInterfaceKC::SendHello()
{
	CIpcPacket pkt("Hello");

	unsigned int nLen = 0;
	const char* pcBuf = pkt.Encode(nLen);

	m_csKC.Lock();
	int nSend = SendData( pcBuf, nLen);
	m_csKC.Unlock();

	//CRLog(E_PROINFO,"P3C发送心跳:%s",pkt.Print().c_str());
	if (nSend < 0)
		return -1;

	return 0;
}

//发送心跳应答
int CProcessInterfaceKC::OnHello(CIpcPacket& pktHello)
{
	CIpcPacket pkt("HelloRsp");

	unsigned int nLen = 0;
	const char* pcBuf = pkt.Encode(nLen);

	m_csKC.Lock();
	int nSend = SendData( pcBuf, nLen);
	m_csKC.Unlock();

	//CRLog(E_PROINFO,"P3C收到心跳,发送心跳应答:%s",pkt.Print().c_str());
	if (nSend < 0)
		return -1;

	return 0;
}


int CProcessInterfaceKC::GetNmKey(string& sKey)
{
	sKey = "K接口客户端连接";
	return 0;
}