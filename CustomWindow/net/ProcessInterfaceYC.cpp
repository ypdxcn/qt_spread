
#include "ProcessInterfaceYC.h"
#include "Logger.h"
#include "strutils.h"
#include "Global.h"
#include "HJGlobalFun.h"
#include "TraderCpMgr.h"
#include "HJConstant.h"

using namespace strutils;

CProcessInterfaceYC::GessPktInfo CProcessInterfaceYC::m_GessPktInfo = 
{
	DEFAULT_IDLE_TIMEOUT,
	DEFAULT_HELLO_RESEND_INTERVAL,
	DEFAULT_HELLO_RESEND_COUNT,
	false
};
bool CProcessInterfaceYC::m_blGessPktInfoInited = false;

CProcessInterfaceYC::CProcessInterfaceYC()
:m_pCfg(0)
,m_blIsLogin(false)
,m_uiCountNoAlive(0)
,m_uiCountSended(0)
{
}

CProcessInterfaceYC::~CProcessInterfaceYC(void)
{
}

int CProcessInterfaceYC::Init(CConfig * pCfg)
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

			pCfg->GetProperty("user_type",m_GessPktInfo.user_type);
			pCfg->GetProperty("user_id",m_GessPktInfo.user_id);
			pCfg->GetProperty("branch_id",m_GessPktInfo.branch_id);
			pCfg->GetProperty("user_pwd",m_GessPktInfo.user_pwd);
		}
	}

	m_pCfg = pCfg;

	m_sTargetIP = g_Global.m_sTargetIP.toStdString();
	m_nTargetPort = g_Global.m_nTargetPort;
	m_nProxyType = g_Global.m_nProxyType;
	if (m_nProxyType > 0)
	{
		m_sProxyHost = g_Global.m_sProxyIP.toStdString();
		m_nProxyPort = g_Global.m_nProxyPort;
		m_sProxyUser = g_Global.m_sProxyUser.toStdString();
		m_sProxyPassword = g_Global.m_sProxyPassword.toStdString();
		if (m_sProxyUser.empty())
		{
			m_bUseLogon = FALSE;
		}
		else
		{
			m_bUseLogon = TRUE;
		}
	}
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
void CProcessInterfaceYC::GetPacketInfo(PacketInfo & stInfo)
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
被调函数:OnRecvPacket,上传报文
输入参数:char * pData接收缓存, size_t nSize缓存大小
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CProcessInterfaceYC::OnPacket(char * pData, size_t nSize)
{
	CBroadcastPacket GessPacket;
	GessPacket.Decode(pData, nSize);

	m_csYC.Lock();
	m_uiCountNoAlive = 0;
	m_csYC.Unlock();

	std::string sCmdID = GessPacket.GetCmdID();

	if ( sCmdID == "RspLogin" )
	{
		OnRspLogin(GessPacket);
	}
	else if (sCmdID == "ConnectTest")
	{
		return OnHello(GessPacket);
	}
	else if (sCmdID == "ConnectTestRsp")
	{
		return 0;
	}
	else
	{
		CRLog(E_PROINFO,"YC %s",GessPacket.Print().c_str());

		OnRecvPacket(GessPacket);
	}

	return 0;
}

/******************************************************************************
函数描述:socket连接成功后，作为客户端被通知,一般用于触发认证报文发送
调用函数:通讯处理器对象回调
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CProcessInterfaceYC::OnConnect()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"YC OnConnect socket (%d) %s:%d",SocketID(),sLocalIp.c_str(),nLocalPort);
	LoginProxy();
	SendLogin();

	CreateTimer(m_GessPktInfo.ulIdleInterval);

	int wparam = 1;
	g_TraderCpMgr.m_mapBdr[E_HQ_STATE].Broadcast(WM_HQ_STATE_CHANGE, &wparam, 0);

	return 0;
}

/*
由timer每隔一段时间来调用（当前设置为16秒）
实现机制：进入函数时判断m_uiCountNoAlive是否大于或等于1，是则证明在当前间隔时间内没有接受到报文（因为在报文的接受处已经将m_uiCountNoAlive重置为0）
*/
int CProcessInterfaceYC::HandleTimeout(unsigned long& ulTmSpan)
{
	int nRtn = 0;
	m_csYC.Lock();
	if (m_uiCountNoAlive >= 1)
	{
		//超过链路最大空闲时间未收到报文，则发送心跳
		//if (m_uiCountSended >= m_GessPktInfo.ulHelloReSend)
		//{//重发心跳次数超过规定次数则准备关闭
		//	nRtn = -1;
		//	ReqClose();
		//}
		//else
		//{//重置定时器间隔
		//	ulTmSpan = m_GessPktInfo.ulIntervalReSend;
		//	m_uiCountSended++;
		//}

		if (0 > SendHello())
			nRtn = -1;
	}
	else
	{
		m_uiCountSended = 0;
		ulTmSpan = m_GessPktInfo.ulIdleInterval;
	}
	m_uiCountNoAlive++;
	m_csYC.Unlock();
	return nRtn;
}

/******************************************************************************
函数描述:socket连接中断后则被通知,可根据协议要求进行处理
调用函数:父类对象回调
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CProcessInterfaceYC::OnClose()
{
	m_blIsLogin = false;
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"YC OnClose socket (%d) %s:%d",SocketID(),sLocalIp.c_str(),nLocalPort);
	DestroyTimer();

	// 行情断开连接
	int wparam = 2;
	g_TraderCpMgr.m_mapBdr[E_HQ_STATE].Broadcast(WM_HQ_STATE_CHANGE, &wparam, 0);
	return;
}

//对外提供的主要接口函数,供通讯处理器发送报文时调用
int CProcessInterfaceYC::SendPacket(CPacket & SendGessPacket)
{
	try
	{
		CBroadcastPacket & pkt = dynamic_cast<CBroadcastPacket&>(SendGessPacket);
		
		unsigned int  nLen = 0;
		const char* pcBuf = pkt.Encode(nLen);
				
		int nRtn = 0;
		m_csYC.Lock();
		int nSend = SendData(pcBuf,nLen);
		m_csYC.Unlock();
		if (nSend < 0)
		{
			//CRLog(E_ERROR,"%s","SendData err");
			nRtn = -1;
		}
		return nRtn;
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

int CProcessInterfaceYC::OnSend(int nSize)
{
	return 0;
}

//接口间心跳报文
int CProcessInterfaceYC::SendHello()
{
	CBroadcastPacket pkt("ConnectTest");

	unsigned int nLen = 0;
	const char* pcBuf = pkt.Encode(nLen);

	m_csYC.Lock();
	int nSend = SendData( pcBuf, nLen);
	m_csYC.Unlock();

	if (nSend < 0)
		return -1;

	return 0;
}

int CProcessInterfaceYC::OnHello(CBroadcastPacket & GessPacket)
{
	CBroadcastPacket pkt("ConnectTestRsp");

	unsigned int nLen = 0;
	const char* pcBuf = pkt.Encode(nLen);

	m_csYC.Lock();
	int nSend = SendData( pcBuf, nLen);
	m_csYC.Unlock();

	if (nSend < 0)
		return -1;

	return 0;
}

//连接成功后发送的第一个登陆报文
int CProcessInterfaceYC::SendLogin()
{
	CBroadcastPacket pkt;

	pkt.AddParameter("user_key", CHJGlobalFun::GetRandNumber(9).c_str());
	pkt.AddParameter("user_type",m_GessPktInfo.user_type.c_str());
	pkt.AddParameter("user_id",m_GessPktInfo.user_id.c_str());
	pkt.AddParameter("again_flag","0");

	unsigned int nLen = 0;	
	const char* pcBuf = pkt.Encode(nLen);

	m_csYC.Lock();
	int nSend = SendData( pcBuf, nLen);
	m_csYC.Unlock();

	CRLog(E_PROINFO,"与服务器连接成功,发送认证报文,%s",pkt.Print().c_str());

	if (nSend < 0)
		return -1;

	return 0;
}

int CProcessInterfaceYC::OnRspLogin(CBroadcastPacket & GessPacket)
{
	//检查应答报文才置状态
	char szRsp[128],szRspMsg[128];
	memset(szRsp,0x00,128);
	memset(szRspMsg,0x00,128);
	GessPacket.GetParameterVal("RspCode",szRsp);
	GessPacket.GetParameterVal("RspMsg",szRspMsg);
	if (strcmp(szRsp,"0000") == 0)
	{
		m_blIsLogin = true;
	}
	else
	{
		m_blIsLogin = false;
		//RequestDisconnect();
	}

	CRLog(E_PROINFO,"TS认证应答:%s,%s",szRsp,szRspMsg);
	return 0;
}

int CProcessInterfaceYC::GetNmKey(string& sKey)
{
	sKey = "A1接口客户端连接";
	return 0;
}

void CProcessInterfaceYC::LoginProxy()
{
	if (m_nProxyType == 0) // 不用代理
	{
		return;
	}
	else if (m_nProxyType == 1) // HTTP11
	{
		stringstream ss;
		string str;

		if (!m_bUseLogon) // 无用户名/密码验证
		{
			ss<<"CONNECT "<<m_sTargetIP<<":"<<m_nTargetPort<<" HTTP/1.1\r\nHost: ";
			ss<<m_sTargetIP<<":"<<m_nTargetPort<<"\r\n\r\n";
			str = ss.str();
		}
		else // 要用户名/密码验证
		{
			ss<<"CONNECT "<<m_sTargetIP<<":"<<m_nTargetPort<<" HTTP/1.1\r\nHost: ";
			ss<<m_sTargetIP<<":"<<m_nTargetPort<<"\r\n";

			stringstream ssUserPassword;
			ssUserPassword<<m_sProxyUser<<":"<<m_sProxyPassword;
			CBase64CodingEx base64coding;
			string base64str;
			if (!base64coding.Encode(ssUserPassword.str(), base64str))
			{
			}
			ss<<"Authorization: Basic "<<base64str<<"\r\n";
			ss<<"Proxy-Authorization: Basic "<<base64str<<"\r\n\r\n";
		}
		string strSendData = ss.str();
		SendData(strSendData.c_str(), strSendData.length());
		Sleep(3000);
	}
	else if (m_nProxyType == 2) // socks4
	{
		//char *command = new char[9+m_sTargetIP.length()+1];
		//memset(command, 0, 9+m_sTargetIP.length()+1);
		//int len = 9;
		//command[0] = 4;
		//command[1] = 1;
		//int port = htons((u_short)m_nTargetPort);
		//memcpy(&(command[2]), &port, 2);
		//unsigned long ip = inet_addr(m_sTargetIP.c_str());
		//memcpy(&command[4], &ip, 4);
		//SendData(command, len); // 往代理服务器发送连接命令
		//delete []command;
		//Sleep(3000);
	}
	else if (m_nProxyType == 3) // sock5
	{
		// 发送请求报文到代理服务器
		char command[10];
		memset(command, 0, 10);
		command[0] = 5;
		command[1] = m_bUseLogon ? 2 : 1; // 登录类型的数目
		command[2] = m_bUseLogon ? 2 : 0; // 2=user/pass, 0=no logon
		int len = m_bUseLogon ? 4 : 3; // 请求报文的长度
		SendData(command, len);
		Sleep(3000);

		if (m_bUseLogon) // 代理服务器要验证用户名/密码
		{
			char *buffer = new char[3+m_sProxyUser.length()+m_sProxyPassword.length()+1];
			sprintf(buffer, "  %s %s", m_sProxyUser.c_str(), m_sProxyPassword.c_str());
			buffer[0] = 5;
			buffer[1] = static_cast<unsigned char>(m_sProxyUser.length());
			buffer[2+m_sProxyUser.length()] = static_cast<unsigned char>(m_sProxyPassword.length());
			int len = 3 + m_sProxyUser.length()+m_sProxyPassword.length();
			SendData(buffer, len);
			delete []buffer;
			Sleep(3000);
		}

		// 不用验证用户名/明码，发送连接请求
		//char *command_connect = new char[10+m_sTargetIP.length()+1];
		//memset(command_connect, 0, 10+m_sTargetIP.length()+1);
		//command_connect[0] = 5;
		//command_connect[1] = 1;
		//command_connect[2] = 0;
		//command_connect[3] = 1; // 固定只支持IPV4的地址，不支持域名；
		//len = 4;
		//unsigned long ip = inet_addr(m_sTargetIP.c_str());
		//memcpy(&command_connect[len], &ip, 4);
		//len += 4;
		//int port = htons(m_nTargetPort);
		//memcpy(&command_connect[len], &port, 2);
		//len += 2;
		//SendData(command_connect, len);
		//delete []command_connect;
		//Sleep(3000);
	}
}

// CBase64Coding

#define END_OF_BASE64_ENCODED_DATA           ('=')
#define BASE64_NUMBER_OF_CHARACTERS_PER_LINE (72)

#define CARRIAGE_RETURN (13)
#define LINE_FEED       (10)

CBase64CodingEx::CBase64CodingEx()
{

}

CBase64CodingEx::~CBase64CodingEx()
{

}

BOOL CBase64CodingEx::Encode(const string &src, string &des)
{
	const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int loop_index = 0;
	int number_of_bytes_to_encode = src.length();

	BYTE byte_to_add = 0;
	BYTE byte_1 = 0;
	BYTE byte_2 = 0;
	BYTE byte_3 = 0;

	const char * input_buffer = src.c_str();

	DWORD number_of_bytes_encoded = (DWORD) ( (double) number_of_bytes_to_encode / (double) 0.75 ) + 1;

	number_of_bytes_encoded += (DWORD)( ( ( number_of_bytes_encoded / BASE64_NUMBER_OF_CHARACTERS_PER_LINE ) + 1 ) * 2 );

	char * destination = new char[number_of_bytes_encoded+1];

	number_of_bytes_encoded = 0;

	while(loop_index < number_of_bytes_to_encode)
	{
		byte_1 = input_buffer[loop_index];
		byte_to_add = alphabet[(byte_1>>2)];

		destination[number_of_bytes_encoded] = static_cast<TCHAR>(byte_to_add);
		number_of_bytes_encoded++;

		loop_index++;
		if (loop_index >= number_of_bytes_to_encode)
		{
			byte_2 = 0;
			byte_to_add = alphabet[ ( ( ( byte_1 & 0x03 ) << 4 ) | ( ( byte_2 & 0xF0 ) >> 4 ) ) ];

			destination[number_of_bytes_encoded] = static_cast<TCHAR>(byte_to_add);
			number_of_bytes_encoded++;

			destination[number_of_bytes_encoded] = static_cast<TCHAR>(END_OF_BASE64_ENCODED_DATA);
			number_of_bytes_encoded++;

			destination[number_of_bytes_encoded] = static_cast<TCHAR>(END_OF_BASE64_ENCODED_DATA);

			destination[number_of_bytes_encoded+1] = 0;

			des = destination;

			delete []destination;

			return TRUE;
		}
		else
		{
			byte_2 = input_buffer[ loop_index ];
		}

		byte_to_add = alphabet[ ( ( ( byte_1 & 0x03 ) << 4 ) | ( ( byte_2 & 0xF0 ) >> 4 ) ) ];
		destination[number_of_bytes_encoded] = static_cast<TCHAR>(byte_to_add);
		number_of_bytes_encoded++;

		loop_index++;

		if ( loop_index >= number_of_bytes_to_encode )
		{
			byte_3 = 0;;

			byte_to_add = alphabet[ ( ( ( byte_2 & 0x0F ) << 2 ) | ( ( byte_3 & 0xC0 ) >> 6 ) ) ];

			destination[ number_of_bytes_encoded ] = static_cast< TCHAR >( byte_to_add );
			number_of_bytes_encoded++;

			destination[ number_of_bytes_encoded ] = static_cast< TCHAR >( END_OF_BASE64_ENCODED_DATA );

			destination[number_of_bytes_encoded+1] = 0;

			des = destination;

			delete []destination;

			return TRUE;
		}
		else
		{
			byte_3 = input_buffer[ loop_index ];
		}

		loop_index++;

		byte_to_add = alphabet[ ( ( ( byte_2 & 0x0F ) << 2 ) | ( ( byte_3 & 0xC0 ) >> 6 ) ) ];

		destination[ number_of_bytes_encoded ] = static_cast< TCHAR >( byte_to_add );
		number_of_bytes_encoded++;

		byte_to_add = alphabet[ ( byte_3 & 0x3F ) ];

		destination[ number_of_bytes_encoded ] = static_cast< TCHAR >( byte_to_add );
		number_of_bytes_encoded++;

		if ( ( number_of_bytes_encoded % BASE64_NUMBER_OF_CHARACTERS_PER_LINE ) == 0 )
		{
			destination[ number_of_bytes_encoded ] = static_cast< TCHAR >( CARRIAGE_RETURN );
			number_of_bytes_encoded++;

			destination[ number_of_bytes_encoded ] = static_cast< TCHAR >( LINE_FEED );
			number_of_bytes_encoded++;
		}	
	}
	destination[ number_of_bytes_encoded ] = static_cast< TCHAR >( END_OF_BASE64_ENCODED_DATA );

	destination[number_of_bytes_encoded+1] = 0;

	des = destination;

	delete []destination;

	return TRUE;
}
