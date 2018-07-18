
#include "TradePacket.h"
#include "ProcessInterfaceB1C.h"
#include "Logger.h"
#include "Gess.h"
#include "HJGlobalFun.h"
#include  "Global.h"

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/pkcs12.h>
#include <openssl/ossl_typ.h>
#include <cassert>

extern "C"
{
#include <openssl/applink.c>
};


#include "MyRSA.h"


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
CProcessInterfaceB1C::CProcessInterfaceB1C(void)
{
	m_sPriKeyPath = "client.pfx";
	m_sPubKeyPath = "server.crt";
	m_sPriKeyPwd = "123456";

	m_stInfo.eLengthType = ltCharactersDec;
	m_stInfo.nLengthPos = 0;
	m_stInfo.nLengthBytes = 8;
	m_stInfo.blLenIncludeHeader = false;
	m_stInfo.nFixHeadLen = m_stInfo.nLengthPos + m_stInfo.nLengthBytes;
}

CProcessInterfaceB1C::~CProcessInterfaceB1C(void)
{
}

void CProcessInterfaceB1C::GetPacketInfo(PacketInfo & stInfo)
{
	stInfo = m_stInfo;
	//stInfo.eLengthType = ltCharactersDec;
	//stInfo.nLengthPos = 0;
	//stInfo.nLengthBytes = 8;
	//stInfo.blLenIncludeHeader = false;
	//stInfo.nFixHeadLen = stInfo.nLengthPos + stInfo.nLengthBytes;
}

int CProcessInterfaceB1C::Init(CConfig* pCfg)
{
	CTcpShortProcess::Init(pCfg);

	if (0 == pCfg)
		return 0;

	string sSysPath = g_Global.GetSystemPath().toStdString();
	
	string sPriKeyPath;
	pCfg->GetProperty("private_key", sPriKeyPath);
	if( sPriKeyPath.empty() )
	{
		sPriKeyPath = "client.pfx";
	}
	m_sPriKeyPath = sSysPath + sPriKeyPath;

	string sPubKeyPath;
	pCfg->GetProperty("public_key", sPubKeyPath);
	if( sPubKeyPath.empty() )
	{
		sPubKeyPath = "server.crt";
	}
	m_sPubKeyPath = sSysPath + sPubKeyPath;

	pCfg->GetProperty("private_pwd", m_sPriKeyPwd);
	if( m_sPriKeyPwd.empty() )
	{
		m_sPriKeyPwd = "123456";
	}

	return 0;
}

//发送报文特别的处理 PreSendHandle具体处理可能是加密,压缩, 若无需任何处理则返回0,若出错返回-1,否则返回实际发送的字节数
int CProcessInterfaceB1C::PreSendHandle(CPacket &sndPacket)
{
	/*BIO *in = NULL;
	EVP_PKEY * pPubKeyPem = 0;
	X509 *x509Pem = NULL;

	if ((in = BIO_new_file (m_sPubKeyPath.c_str(), "r")) == 0)
	{
		return -1;
	}
	
	x509Pem = PEM_read_bio_X509(in, NULL, 0, NULL);
    if (x509Pem == 0)
	{
		return -1;
	}

	pPubKeyPem = X509_get_pubkey(x509Pem);
	if (pPubKeyPem == 0) 
	{
		if (x509Pem) 
		{
		   X509_free(x509Pem);
		}
		return -1;
	}
	else if (pPubKeyPem->pkey.rsa == 0)
	{
		if (x509Pem) 
		{
		   X509_free(x509Pem);
		}
		return -1;
	}

	unsigned int uiTotalLen = 0;
	const char * pPktData = sndPacket.Encode(uiTotalLen);
	const char * pData = pPktData + MSGLEN;
	unsigned int uiLen = uiTotalLen - MSGLEN;

	unsigned int uiTotalSize = (uiLen/100 + 2) * 128;
	char* pEncryptedTotal = new char[uiTotalSize];
	unsigned int nRsaSize = RSA_size(pPubKeyPem->pkey.rsa);
	unsigned char* pEncrpted = new unsigned char[nRsaSize];

	int nInLen = 0;
	int nOutLen = 0;
	for (size_t n = 100; n - 100 < uiLen; n += 100)
	{
		nInLen = uiLen / n == 0 ? uiLen - (n - 100) : 100;
		int nTmp = RSA_public_encrypt(nInLen,(const unsigned char*)pData,pEncrpted,pPubKeyPem->pkey.rsa,RSA_PKCS1_PADDING);
		memcpy(pEncryptedTotal + m_stInfo.nLengthBytes + nOutLen,pEncrpted,nTmp);
		nOutLen += nTmp;
		pData += nInLen;
	}
	
	if (x509Pem) 
	{
	   X509_free(x509Pem);
	}

	char szLenFmt[64] = {0};
	sprintf(szLenFmt,"%c0%dd",'%',m_stInfo.nLengthBytes);
	char szLen[128] = {0};
	sprintf(szLen,szLenFmt,nOutLen);
	memcpy(pEncryptedTotal,szLen,m_stInfo.nLengthBytes);

	int nSendLen = SendData(pEncryptedTotal,nOutLen + m_stInfo.nLengthBytes);
	if (nSendLen < 0)
	{
		nSendLen = -1;
	}
	else if (nSendLen == 0)
	{
		nSendLen = 1;
	}

	delete []pEncrpted;
	delete []pEncryptedTotal;
	return nSendLen;*/

	unsigned int uiTotalLen = 0;
	const char * pPktData = sndPacket.Encode(uiTotalLen);
	const char * pData = pPktData + MSGLEN;
	unsigned int uiLen = uiTotalLen - MSGLEN;
	char *pEncryptedPart;
	unsigned int nOutLen;
	CMyRSA::HandleRsaEncrypt(pEncryptedPart, nOutLen, pData, uiLen, m_sPubKeyPath.c_str());

	// 分配加上长度部分的内存
	char *pEncryptedTotal = new char[nOutLen+m_stInfo.nLengthBytes];
	memset(pEncryptedTotal,0,nOutLen+m_stInfo.nLengthBytes);

	// 记录报文长度，这部分是不用加密的
	char szLenFmt[64] = {0};
	sprintf(szLenFmt,"%c0%dd",'%',m_stInfo.nLengthBytes);
	char szLen[128] = {0};
	sprintf(szLen,szLenFmt,nOutLen);
	memcpy(pEncryptedTotal,szLen,m_stInfo.nLengthBytes);

	// 复制加密后的报文
	memcpy(pEncryptedTotal+m_stInfo.nLengthBytes, pEncryptedPart, nOutLen);

	// 发送报文
	int nSendLen = SendData(pEncryptedTotal, nOutLen + m_stInfo.nLengthBytes);
	if (nSendLen < 0)
	{
		nSendLen = -1;
	}
	else if (nSendLen == 0)
	{
		nSendLen = 1;
	}

	delete []pEncryptedPart;
	delete []pEncryptedTotal;

	return nSendLen;
}

//接收数据特别的处理 SufRecvHandle具体处理可能是解密,解压缩, 若无需任何处理则返回0,若出错返回-1,否则返回1 
int CProcessInterfaceB1C::SufRecvHandle(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket)
{
	X509 * x509Pfx = 0;
	EVP_PKEY * pPubKeyPfx = 0;
	EVP_PKEY * pPriKeyPfx = 0;
	
	FILE *fp = 0;
	PKCS12 *p12 = 0;


	/// 读入P12
	if (!(fp = fopen (m_sPriKeyPath.c_str(), "rb")))
		return -1;

	SSLeay_add_all_algorithms();
	p12 = d2i_PKCS12_fp (fp, nullptr);
	fclose(fp);
	if (!(p12))
		return -1;

	int nRtn = PKCS12_parse(p12, m_sPriKeyPwd.c_str(), &pPriKeyPfx, 0, 0);
	if (nRtn != 1)
	{
		PKCS12_free(p12);
		return -1;
	}

	PKCS12_free(p12);
	if (pPriKeyPfx == 0) 
	{
		return -1;
	}
	else if (RSA_size(pPriKeyPfx->pkey.rsa) == 0)
	{
		return -1;
	}



	//BIO* in2 = nullptr;
	////生成IO对象
	//in2 = BIO_new(BIO_s_file());
	////将Key文件读入IO
	//BIO_read_filename(in2, m_sPriKeyPath.c_str());
	//if (in2 == nullptr)
	//{
	//	perror(m_sPriKeyPath.c_str());
	//	return -1;
	//}
	////将IO中数据以PEM格式读入EVP_PKEY结构中
	//pPriKeyPfx = PEM_read_bio_PrivateKey(in2, nullptr, nullptr, (void *)m_sPriKeyPwd.c_str());
	//if (in2 != nullptr)
	//	BIO_free(in2);
	//if (pPriKeyPfx == nullptr)
	//{
	//	//cerr << "读取Key文件失败! " << keyFileName << endl;
	//	return -1;
	//}




	int nInLen = 0;
	int nOutLen = 0;
	unsigned char* pDecrpted = new unsigned char[RSA_size(pPriKeyPfx->pkey.rsa)];
	unsigned char* pDecryptedTotal = new unsigned char[ulLen+128];
	//memcpy(pDecryptedTotal,szRecvBuf,m_stInfo.nLengthBytes);
		
	char * pData = pRecvBuf + m_stInfo.nLengthBytes;
	unsigned int uiBodyLen = ulLen - m_stInfo.nLengthBytes;
    bool bOK = true;
	for (int m = 128; m - 128 < uiBodyLen; m+=128)
	{
		nInLen = uiBodyLen / m == 0 ? uiBodyLen - (m - 128) : 128;
		int nTmp = RSA_private_decrypt(nInLen,(const unsigned char*)pData,pDecrpted,pPriKeyPfx->pkey.rsa,RSA_PKCS1_PADDING);
        if (nTmp > 0)
        {
            memcpy(pDecryptedTotal + m_stInfo.nLengthBytes + nOutLen,pDecrpted,nTmp);
            nOutLen += nTmp;
            pData += nInLen;
        }
        else
        {
            bOK = false;
            break;
        }
	}

    if (bOK)
    {
        char szLenFmt[64] = {0};
        sprintf(szLenFmt,"%c0%dd",'%',m_stInfo.nLengthBytes);
        char szLen[128] = {0};
        sprintf(szLen,szLenFmt,nOutLen);
        memcpy(pDecryptedTotal,szLen,m_stInfo.nLengthBytes);

        rcvPacket.Decode((const char*)pDecryptedTotal,m_stInfo.nLengthBytes + nOutLen);
        delete []pDecrpted;
        delete []pDecryptedTotal;
    }

    return 1;
}

//QString CProcessInterfaceB1C::GetSystemPath()
//{
//	QString strIniPath;
//	TCHAR exeFullPath[MAX_PATH];
//	memset(exeFullPath, 0, MAX_PATH);
//
//	GetModuleFileName(AfxGetInstanceHandle(), exeFullPath, MAX_PATH);
//	strIniPath = (QString)exeFullPath;
//
//	//获得exe所在的目录
//	int position = strIniPath.ReverseFind('\\');
//	strIniPath = strIniPath.Left(position + 1);
//	strIniPath += "\\";
//	return strIniPath;
//}