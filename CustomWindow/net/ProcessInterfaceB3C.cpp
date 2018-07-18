
#include "ProcessInterfaceB3C.h"
#include "SufRecvHandler.h"

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
CProcessInterfaceB3C::CProcessInterfaceB3C(void)
{
	m_s3DesKey = "240262447423713749922240";
	m_s3DesIV = "12345678";
	m_sSessionID = "12345678";

	m_stInfo.eLengthType = ltCharactersDec;
	m_stInfo.nLengthPos = 0;
	m_stInfo.nLengthBytes = 8;
	m_stInfo.blLenIncludeHeader = false;
	m_stInfo.nFixHeadLen = m_stInfo.nLengthPos + m_stInfo.nLengthBytes;
}

CProcessInterfaceB3C::~CProcessInterfaceB3C(void)
{
}

void CProcessInterfaceB3C::GetPacketInfo(PacketInfo & stInfo)
{
	stInfo = m_stInfo;
	//stInfo.eLengthType = ltCharactersDec;
	//stInfo.nLengthPos = 0;
	//stInfo.nLengthBytes = 8;
	//stInfo.blLenIncludeHeader = false;
	//stInfo.nFixHeadLen = stInfo.nLengthPos + stInfo.nLengthBytes;
}

int CProcessInterfaceB3C::Init(CConfig* pCfg)
{
	CTcpShortProcess::Init(pCfg);

	if (0 == pCfg)
		return 0;

	string sKey = "3DesKey";
	//pCfg->GetProperty(sKey, m_s3DesKey);

	sKey = "3DesIV";
	//pCfg->GetProperty(sKey, m_s3DesIV);

	sKey = "SessionID";
	pCfg->GetProperty(sKey, m_sSessionID);

	return 0;
}

//发送报文特别的处理 PreSendHandle具体处理可能是加密,压缩, 若无需任何处理则返回0,若出错返回-1,否则返回实际发送的字节数
int CProcessInterfaceB3C::PreSendHandle(CPacket &sndPacket)
{
	return 0;
}

//接收数据特别的处理 SufRecvHandle具体处理可能是解密,解压缩, 若无需任何处理则返回0,若出错返回-1,否则返回1 
int CProcessInterfaceB3C::SufRecvHandle(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket)
{
	return g_SufRecvHandler.SufRecvHandle(pRecvBuf,ulLen,rcvPacket);
}