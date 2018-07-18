
#include "TradePacket.h"
#include "ProcessInterfaceB2C.h"
#include "Logger.h"
#include "Gess.h"

#include <openssl/ossl_typ.h>
#include <openssl/evp.h>

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
CProcessInterfaceB2C::CProcessInterfaceB2C(void)
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

CProcessInterfaceB2C::~CProcessInterfaceB2C(void)
{
}

void CProcessInterfaceB2C::GetPacketInfo(PacketInfo & stInfo)
{
	stInfo = m_stInfo;
	//stInfo.eLengthType = ltCharactersDec;
	//stInfo.nLengthPos = 0;
	//stInfo.nLengthBytes = 8;
	//stInfo.blLenIncludeHeader = false;
	//stInfo.nFixHeadLen = stInfo.nLengthPos + stInfo.nLengthBytes;
}

int CProcessInterfaceB2C::Init(CConfig* pCfg)
{
	CTcpShortProcess::Init(pCfg);

	if (0 == pCfg)
		return 0;

	string sKey = "3DesKey";
	//pCfg->GetProperty(sKey, m_s3DesKey);

	//sKey = "3DesIV";
	//pCfg->GetProperty(sKey, m_s3DesIV);

	sKey = "SessionID";
	pCfg->GetProperty(sKey, m_sSessionID);

	return 0;
}

//发送报文特别的处理 PreSendHandle具体处理可能是加密,压缩, 若无需任何处理则返回0,若出错返回-1,否则返回实际发送的字节数
int CProcessInterfaceB2C::PreSendHandle(CPacket &sndPacket)
{
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_des_ede3_cbc(), NULL, (const unsigned char*)m_s3DesKey.c_str(), (const unsigned char*)m_s3DesIV.c_str());

	unsigned int uiLen = 0;
	const char* pPktData = sndPacket.Encode(uiLen);

	unsigned int uiHeadLen = m_stInfo.nLengthBytes + 1 + m_sSessionID.length();
	unsigned char* pEccrypted = new unsigned char[uiHeadLen + uiLen + 128];
	int nEncryptedLen = 0;
	int nRtn = 0;
	do
	{
		nRtn = EVP_EncryptUpdate(&ctx,pEccrypted + uiHeadLen,&nEncryptedLen,(const unsigned char*)pPktData,uiLen);
	} while(false);
	
	int nTmpLen = 0;
	nRtn = EVP_EncryptFinal_ex(&ctx, pEccrypted + uiHeadLen + nEncryptedLen, &nTmpLen);
	nEncryptedLen += nTmpLen;
	
	char szHeader[128] = {0};
	sprintf(szHeader,"%08d%d%s", 1 + m_sSessionID.length() + nEncryptedLen,0x02,m_sSessionID.c_str());
	szHeader[8] = 0x02;

	memcpy(pEccrypted,szHeader,uiHeadLen); 	
	int nSendLen = SendData((const char*)pEccrypted,nEncryptedLen + uiHeadLen);
	if (nSendLen < 0)
	{
		nSendLen = -1;
	}
	else if (nSendLen == 0)
	{
		nSendLen = 1;
	}

	delete []pEccrypted;
	return nSendLen;
}

//接收数据特别的处理 SufRecvHandle具体处理可能是解密,解压缩, 若无需任何处理则返回0,若出错返回-1,否则返回1 
int CProcessInterfaceB2C::SufRecvHandle(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket)
{
	return g_SufRecvHandler.SufRecvHandle(pRecvBuf,ulLen,rcvPacket);
}