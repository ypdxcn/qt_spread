/******************************************************************************
版    权:深圳市雁联计算系统有限公司.
模块名称:GenericPacketHandler.cpp
创建者	:张伟
创建日期:2008.07.22
版    本:1.0				
模块描述:封装基于TCP报文接收，由于基于TCP的应用层报文定义千变万化，所以设置了众
         多选项参数用于适应
主要函数:OnPacket(...) 虚函数收到完整报文后回调
		 GetPacketLength(...) 根据报文定义和已收到的部分报文头计算待接收报文长
		 SendData(...) 发送函数
	     SendPacket(..) 发送报文接口函数
修改记录:
******************************************************************************/
#include <cassert>
#include "ProtocolProcess.h"
//#include "ProtocolComm.h"
#include "Logger.h"

//using namespace std;

//初始化为4字节整数标识长度,长度包含报文头
//PacketInfo CTcpHandler::m_PacketInfo = {4,0,ltInterger,true,0,hfNoUsed,0,tfNoUsed,0};
//bool CTcpHandler::m_blPacketInfoInited = false;

CTcpHandlerLong::CTcpHandlerLong()
:m_nIndex(0)
,m_nLength(0)
,m_uiInNowStat(0)
,m_uiInAveBytes(0)
{
	m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
	m_stRcvBuf.pBuf = m_szBuf;
	m_stRcvBuf.uiUsed = 0;

	for (int i = 0; i < eCommMaxStatic; i++)
	{
		m_uiLastStatics[i] = 0;;
		m_uiStatics[i] = 0;
	}
}

CTcpHandlerLong::~CTcpHandlerLong(void)
{
}

/******************************************************************************
函数描述:根据报文定义和已收到的部分报文头计算待接收报文长
调用函数:线程函数
输入参数:char* pPacketData已接收到数据缓存
返回值  :size_t 待接收报文总长度
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
#define MAX_CHARACTER_LEN	128
size_t CTcpHandler::GetPacketLength(const char* pPacketData)
{
	assert(pPacketData != 0);
	if (0 == pPacketData)
		return 0;

	size_t nLength = 0;
	if (m_PacketInfo.eLengthType == ltInterger)
	{//整数标识长度
		if (m_PacketInfo.nLengthBytes == 1)
		{
			nLength = *(pPacketData + m_PacketInfo.nLengthPos);
		}
		else if (m_PacketInfo.nLengthBytes == 2)
		{
			nLength = ntohs( *(reinterpret_cast<const unsigned short*>(pPacketData + m_PacketInfo.nLengthPos)));
		}
		else if (m_PacketInfo.nLengthBytes == 4)
		{
			nLength = ntohl( *(reinterpret_cast<const unsigned int*>(pPacketData + m_PacketInfo.nLengthPos)));
		}
		else
		{
			CRLog(E_CRITICAL, "非法的长度字段尺寸: %d", m_PacketInfo.nLengthBytes);
			return 0;
		}
	}
	else if (m_PacketInfo.eLengthType == ltCharactersDec)
	{//10进制字符串标识长度
		if (MAX_CHARACTER_LEN > m_PacketInfo.nLengthBytes)
		{
			char szLen[MAX_CHARACTER_LEN];
			memset(szLen,0x00,MAX_CHARACTER_LEN);
			memcpy(szLen, &pPacketData[m_PacketInfo.nLengthPos], m_PacketInfo.nLengthBytes);
			nLength = strtoul(szLen,0,10);
		}
		else
		{
			CRLog(E_CRITICAL, "非法的长度字段尺寸: %d", m_PacketInfo.nLengthBytes);
			return 0;
		}
	}
	else if (m_PacketInfo.eLengthType == ltCharactersHex)
	{//16进制字符串标识长度
		if (MAX_CHARACTER_LEN > m_PacketInfo.nLengthBytes)
		{
			char szLen[MAX_CHARACTER_LEN];
			memset(szLen,0x00,MAX_CHARACTER_LEN);
			memcpy(szLen, &pPacketData[m_PacketInfo.nLengthPos], m_PacketInfo.nLengthBytes);
			nLength = strtoul(szLen,0,16);
		}
		else
		{
			CRLog(E_CRITICAL, "非法的长度字段尺寸: %d", m_PacketInfo.nLengthBytes);
			return 0;
		}
	}
	else
	{
		CRLog(E_CRITICAL, "非法的长度字段类型: %d", m_PacketInfo.eLengthType);
		return 0;
	}
	
	//若长度不包含报文头,则需加上报文头长度
	if (!m_PacketInfo.blLenIncludeHeader && nLength!=0)
		nLength+=m_PacketInfo.nFixHeadLen;

	if ( nLength > MAX_PACKET_SIZE )
	{   //失步
		CRLog(E_ERROR,"Bad MSG PACK m_nLength = %d!", nLength );				
		return 0;
	}

	return nLength;
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
int CTcpHandlerLong::OnRecvTcp(char* pBuf,int nSize)
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
		m_uiInNowStat += nSize;
		m_csStatics.Unlock();

		//统计累计数据
		UpdateStatics(eCommBytesRecvAll,nSize);

		int nLastIndex = m_nIndex;
		m_nIndex += nSize ; 
		if( m_nLength == 0 && m_nIndex < m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
		{
			CRLog(E_DEBUG,"本次数据未收齐，nSize = %d, m_nIndex = %d m_nLength = %d !", nSize, m_nIndex , m_nLength);	
			memcpy(&m_szBuf[nLastIndex],pBuf,nSize);
			return 0;
		}

		//frame begin
		if ( m_nLength == 0 && m_nIndex >= m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
		{
			m_nLength = GetPacketLength(m_szBuf);
			if ( m_nLength == 0 )
			{   //长度异常为乱包则初始化
				//若有帧头，则需要先帧头同步...//
				CRLog(E_ERROR,"%s","长度异常");
				UpdateStatics(eCommCountRecvErr);
				m_nIndex = m_nLength = 0;
				return -1;
			}
		}

		if ( m_nIndex <= m_nLength )
		{
			memcpy(&m_szBuf[nLastIndex],pBuf,nSize);
			if ( m_nIndex == m_nLength )
			{   //收到完整的一个报文 直接处理并返回
				//若有帧头，则需要先检测...//
				//若有帧尾，则按规定算法校验类处理，截去帧尾，再上送...//
				UpdateStatics(eCommBytesRecv,m_nLength);
				if (0 > OnPacket(m_szBuf,m_nLength))
				{
					return -1;
				}
				m_nIndex = m_nLength = 0;
			}

			return 0;
		}

		//如下是对一次收到多于一个报文的处理
		//CRLog(E_DEBUG,"recv packet > 1: nSize = %d, m_nIndex = %d m_nLength = %d !", nSize, m_nIndex , m_nLength );
		memcpy(&m_szBuf[nLastIndex],pBuf,m_nLength - nLastIndex);
		if (0 > OnPacket(m_szBuf,m_nLength))
		{
			return -1;
		}
		UpdateStatics(eCommBytesRecv,m_nLength);

		pBuf += m_nLength - nLastIndex;
		unsigned int nLeaveByte = nSize - (m_nLength - nLastIndex);
		if( nLeaveByte < m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
		{
			memcpy(m_szBuf,pBuf,nLeaveByte);
			m_nIndex = nLeaveByte;
			m_nLength = 0;
			return 0;
		}

		//frame begin
		unsigned int nLength = 0;
		if ( nLeaveByte >= m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
		{
			nLength = GetPacketLength(pBuf);
			if ( m_nLength == 0 )
			{   //长度异常为乱包则初始化
				//若有帧头，则需要先帧头同步...//
				UpdateStatics(eCommCountRecvErr);
				m_nIndex = m_nLength = 0;
				return -1;
			}
		}
		
		if ( nLeaveByte <= nLength )
		{
			if ( nLeaveByte == nLength )
			{   //收到完整的一个报文 直接处理并返回
				//若有帧头，则需要先检测...//
				//若有帧尾，则按规定算法校验类处理，截去帧尾，再上送...//
				UpdateStatics(eCommBytesRecv,nLength);
				if (0 > OnPacket(pBuf,nLength))
				{
					return -1;
				}
				m_nIndex = m_nLength = 0;
			}
			else
			{
				memcpy(m_szBuf,pBuf,nLeaveByte);
				m_nIndex = nLeaveByte;
				m_nLength = nLength;
			}
			return 0;
		}
		
		do
		{
			//正确接收命令
			//若有帧头，则需要先检测...//
			//若有帧尾，则按规定算法校验类处理，截去帧尾，再上送...//
			UpdateStatics(eCommBytesRecv,nLength);
			if (0 > OnPacket(pBuf,nLength))
			{
				return -1;
			}
			
			nLeaveByte -= nLength;
			pBuf += nLength;
			if ( nLeaveByte >=  m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
			{
				nLength = GetPacketLength(pBuf);
				if ( nLength == 0 )
				{   //长度异常为乱包则初始化
					//若有帧头，则需要先帧头同步...//
					//IncreaseStatics(CommUpInErr);
					m_nIndex = m_nLength = 0;
					nLeaveByte = 0;
					return -1;
				}
				if ( nLeaveByte < nLength )
				{
					break;
				}
			}
			else
			{
				m_nLength = 0;
				break;
			}
		} while ( nLeaveByte > 0 );

		if ( nLeaveByte > 0 )
		{
			memcpy( m_szBuf, pBuf, nLeaveByte );
			m_nIndex = nLeaveByte;
		}
		else
		{
			m_nIndex = 0;
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
函数描述:socket从网络接收数据,判断报文是否接收完整,并通过回调将接收到的完整报文
         上传,缓存及长度已经存入m_szBuf,m_nIndex
被调函数:GetPacketLength
返回值  :0正确接收 -1接收错误
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int CTcpHandlerLong::OnRecvTcp()
{
	try
	{
		//统计接收平均流量 有流量时每5秒以上间隔统计一次
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

		//统计累计数据
		UpdateStatics(eCommBytesRecvAll,m_stRcvBuf.uiLen);

		int nLastIndex = m_nIndex;
 		m_nIndex += m_stRcvBuf.uiLen; 
		if( m_nLength == 0 && m_nIndex < m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
		{
			//CRLog(E_DEBUG,"本次数据未收齐，nSize = %d, m_nIndex = %d m_nLength = %d !", m_stRcvBuf.uiLen, m_nIndex , m_nLength);
			m_stRcvBuf.uiLen = MAX_PACKET_SIZE - m_nIndex;
			m_stRcvBuf.pBuf = m_szBuf + m_nIndex;
			return 0;
		}

		//frame begin
		if ( m_nLength == 0 && m_nIndex >= m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
		{
			m_nLength = GetPacketLength(m_szBuf);
			if ( m_nLength == 0 )
			{   //长度异常为乱包则初始化
				//若有帧头，则需要先帧头同步...//
				CRLog(E_ERROR,"%s","长度异常");
				UpdateStatics(eCommCountRecvErr);
				m_nIndex = m_nLength = 0;
				m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
				m_stRcvBuf.pBuf = m_szBuf;
				return -1;
			}
		}

		if ( m_nIndex <= m_nLength )
		{
			int nRtn = 0;
			if ( m_nIndex == m_nLength )
			{   //收到完整的一个报文 直接处理并返回
				//若有帧头，则需要先检测...//
				//若有帧尾，则按规定算法校验类处理，截去帧尾，再上送...//
				UpdateStatics(eCommBytesRecv,m_nLength);
				if (0 > OnPacket(m_szBuf,m_nLength))
				{
					return -1;
				}
				m_nIndex = m_nLength = 0;
				m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
				m_stRcvBuf.pBuf = m_szBuf;
			}
			else
			{
				m_stRcvBuf.uiLen = MAX_PACKET_SIZE - m_nIndex;
				m_stRcvBuf.pBuf = m_szBuf + m_nIndex;
			}

			return nRtn;
		}

		//如下是对一次收到多于一个报文的处理
		//CRLog(E_DEBUG,"recv packet > 1: nSize = %d, m_nIndex = %d m_nLength = %d !", nSize, m_nIndex , m_nLength );

		char* pszPos = m_szBuf;
		unsigned int nLeaveByte = m_nIndex;
		do
		{
			//正确接收命令
			//若有帧头，则需要先检测...//
			//若有帧尾，则按规定算法校验类处理，截去帧尾，再上送...//
			UpdateStatics(eCommBytesRecv,m_nLength);
			if (0 > OnPacket(pszPos,m_nLength))
			{
				return -1;
			}
			nLeaveByte -= m_nLength;
			pszPos += m_nLength;
			if ( nLeaveByte >=  m_PacketInfo.nLengthPos + m_PacketInfo.nLengthBytes )
			{
				m_nLength = GetPacketLength(pszPos);
				if ( m_nLength == 0 )
				{   //长度异常为乱包则初始化
					//若有帧头，则需要先帧头同步...//
					UpdateStatics(eCommCountRecvErr);
					m_nIndex = m_nLength = 0;
					m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
					m_stRcvBuf.pBuf = m_szBuf;
					nLeaveByte = 0;
					return -1;
				}
				if ( nLeaveByte < m_nLength )
				{
					break;
				}
			}
			else
			{
				m_nLength = 0;
				break;
			}
		} while ( nLeaveByte > 0 );

		if ( nLeaveByte > 0 )
		{
			memcpy( m_szBuf, pszPos, nLeaveByte );
			m_nIndex = nLeaveByte;
			m_stRcvBuf.uiLen = MAX_PACKET_SIZE - m_nIndex;
			m_stRcvBuf.pBuf = m_szBuf + m_nIndex;
		}
		else
		{
			m_nIndex = 0;
			m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
			m_stRcvBuf.pBuf = m_szBuf;
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

void CTcpHandlerLong::UpdateStatics(eCommStaticType eType, int nCount)
{
	if (eCommMaxStatic == eType)
		return;

	m_uiStatics[eType] += nCount;

	//do something?

	m_uiLastStatics[eType] += nCount;
}