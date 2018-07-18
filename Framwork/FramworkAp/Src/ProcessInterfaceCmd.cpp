#include "ProcessInterfaceCmd.h"
#include "LinePacket.h"
#include "Logger.h"
#include "strutils.h"

using namespace strutils;

int CProcessInterfaceCmd::m_blInited = false;

CProcessInterfaceCmd::CProcessInterfaceCmd()
:m_nIndex(0)
{
	m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
	m_stRcvBuf.pBuf = m_szBuf;
}

CProcessInterfaceCmd::~CProcessInterfaceCmd()
{
}

int CProcessInterfaceCmd::Init(CConfig * pCfg)
{
	if (!m_blInited)
	{
		m_blInited = true;
		
		m_pCfg = pCfg;
	}
	return 0;
}

// 作服务端接收到连接后回调
int CProcessInterfaceCmd::OnAccept()
{
	SendData("RiskMgr->",9);
	return 0;
}		
/******************************************************************************
函数描述:socket从网络接收数据,判断报文是否接收完整,并通过回调将接收到的完整报文
         上传
被调函数:GetPacketLength
输入参数:char* pBuf已接收到数据缓存,nSize缓存长度
返回值  :0正确接收 -1接收错误
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CProcessInterfaceCmd::OnRecv(char* pBuf,int nSize)
{	
	return 0;
}

/******************************************************************************
函数描述:socket从网络接收数据,判断报文是否接收完整,并通过回调将接收到的完整报文
         上传,缓存及长度已经存入m_szBuf,m_nIndex
被调函数:GetPacketLength
返回值  :0正确接收 -1接收错误
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CProcessInterfaceCmd::OnRecv()
{
	try
	{
		//统计平均流量
		m_csStatics.Lock();
		int nInterval = m_oBeginStatTm.IntervalToNow();
		if (nInterval >= 5)
		{
			m_uiInAveBytes = m_uiInNowStat / nInterval;
			m_oBeginStatTm.ToNow();
			m_uiInNowStat = 0;
		}
		m_uiInNowStat += m_stRcvBuf.uiLen;
		m_csStatics.Unlock();

		m_nIndex += m_stRcvBuf.uiLen;
		if (m_nIndex < 2)
		{
			m_stRcvBuf.uiLen = MAX_PACKET_SIZE - m_nIndex;
			m_stRcvBuf.pBuf = m_szBuf + m_nIndex;

			return 0;
		}
	
		char * pcTmp = m_szBuf;
		for (unsigned int i = 1; i < m_nIndex; i++)
		{
			if (m_szBuf[i-1] == '\r' && m_szBuf[i] == '\n')
			{
				string sLocalIp,sPeerIp;
				int nLocalPort,nPeerPort;
				GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
				string sKey = sPeerIp + ToString(nPeerPort);

				if (i > 1)
				{
					CPacketLineReq pkt;
					pkt.Decode(pcTmp,static_cast<unsigned int>(m_szBuf+i-1-pcTmp));
					
					if (pkt.GetCmdID() == "exit")
					{
						ReqClose();
						return 0;
					}

					pkt.AddRouteKey(sKey);
					OnRecvPacket(sKey,pkt);
				}
				else
				{
					CPacketLineReq pkt("");
					pkt.AddRouteKey(sKey);
					OnRecvPacket(sKey,pkt);
				}

				pcTmp = pcTmp + i + 1;
			}
		}

		if (pcTmp > m_szBuf)
		{
			if (pcTmp < m_szBuf + m_nIndex)
			{
				memcpy(m_szBuf,pcTmp,m_szBuf + m_nIndex - pcTmp);
				m_nIndex = m_szBuf + m_nIndex - pcTmp + 1;
			}
			else
			{
				m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
				m_stRcvBuf.pBuf = m_szBuf;
				m_nIndex = 0;
			}
		}
		else
		{
			m_stRcvBuf.uiLen = MAX_PACKET_SIZE - m_nIndex;
			m_stRcvBuf.pBuf = m_szBuf + m_nIndex;
		}

		return 0;
	}
	catch(std::exception e)
	{
		CRLog(E_CRITICAL,"exception:%s", e.what());
		return -1;
	}
	catch(...)
	{
		CRLog(E_CRITICAL,"%s","Unknown exception");
		return -1;
	}
}

/******************************************************************************
函数描述:对外提供的主要接口函数,供协作对象发送报文时调用
调用函数:作为协作对象的包装类对象调用
被调函数:SendData,编码后直接通过socket发送
输入参数:CTradePacket & SendGessPacket待发送报文
输出参数:
输入输出:
返回值  :int 暂无特殊处理
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CProcessInterfaceCmd::SendPacket(CPacket & SendPacket)
{
	try
	{
		//if (!IsLogin())
		//	return -1;

		CPacketLineRsp & pkt = dynamic_cast<CPacketLineRsp&>(SendPacket);

		unsigned int  nLen = 0;
		const char* pcBuf = pkt.Encode(nLen);
		
		int nRtn = 0;
		int nSend = SendData(pcBuf,nLen);
		if (nSend < 0)
		{
			nRtn = -1;
			CRLog(E_ERROR,"%s","SendData err");
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

void CProcessInterfaceCmd::OnClose()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"Telnet CmdLine Long Connection Close %s %d",sPeerIp.c_str(),nPeerPort);
}	

int CProcessInterfaceCmd::GetNmKey(string& sKey)
{
	sKey = "远程Telnet接口服务端连接.";
	int nSockeID = static_cast<int>( SocketID());
	if (INVALID_SOCKET != nSockeID)
	{
		sKey += ToString<int>(nSockeID);
	}
	else
	{
		srand(static_cast<unsigned int>(time(0)));
		int RANGE_MIN = 1;
		int RANGE_MAX = 99;
		int nRandom = rand() % (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
		sKey += ToString<int>(nRandom);
	}

	return 0;
}