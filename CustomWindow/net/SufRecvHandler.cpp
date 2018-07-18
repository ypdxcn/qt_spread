#include "SufRecvHandler.h"

#include "zlib.h"
#include "Comm.h"
#include <openssl/ossl_typ.h>
#include <openssl/evp.h>
#include "Global.h"

CSufRecvHandler::CSufRecvHandler(void)
{
	m_nLengthBytes = 8;
	m_nMaxZipPercent = 20;

	m_sFix3DESKey = "240262447423713749922240";
	m_sDys3DESKey = m_sFix3DESKey;
	m_s3DESIV = "12345678";
}

CSufRecvHandler::~CSufRecvHandler(void)
{
}

CSufRecvHandler& CSufRecvHandler::GetInstance()
{
	static CSufRecvHandler theSingleton;
	return theSingleton;
}

int CSufRecvHandler::Unzip( char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket )
{
	int nFlagLen = 1;
	int nFlag = static_cast<int>(*(pRecvBuf + m_nLengthBytes + nFlagLen));
	int nHeadLen = m_nLengthBytes + nFlagLen;

	unsigned int uiBufLen = ulLen*m_nMaxZipPercent;
	char *pPlainBuff = new char[uiBufLen];	
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree = 0;
	stream.opaque = 0;
	stream.avail_in = ulLen - nHeadLen;	
	stream.next_in = (Bytef*)(pRecvBuf + nHeadLen);

	int nRtnZip = 0;
	if (Z_OK != inflateInit2(&stream, 16 + MAX_WBITS))
	{
		return -1;
	}

	stream.avail_out = uiBufLen;
	stream.next_out = (Bytef*)pPlainBuff;
	do
	{
		nRtnZip = inflate(&stream,Z_NO_FLUSH);
		if (Z_OK != nRtnZip && Z_STREAM_END != nRtnZip)
		{
			return -1;
		}
	} while (false);//nRtnZip != Z_STREAM_END);
	inflateEnd(&stream);

	rcvPacket.Decode(pPlainBuff,stream.total_out);

	if( NULL != pPlainBuff )
	{
		delete[] pPlainBuff;
		pPlainBuff = NULL;
	}

	return 1;
}

int CSufRecvHandler::Decrypt( char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket, char cMode )
{
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);

	if (cMode == 0x02)
	{
		EVP_DecryptInit_ex(&ctx, EVP_des_ede3_cbc(), NULL, (const unsigned char*)GetFix3DESKey().c_str(), (const unsigned char*)Get3DESIV().c_str());
	}
	else
	{
		EVP_DecryptInit_ex(&ctx, EVP_des_ede3_cbc(), NULL, (const unsigned char*)GetDys3DESKey().c_str(), (const unsigned char*)Get3DESIV().c_str());
	}

	unsigned int uiHeadLen = m_nLengthBytes + 1 + GetSessionID().length();
	const char* pPktData = pRecvBuf + uiHeadLen;

	unsigned char* pDecrypted = new unsigned char[ulLen - uiHeadLen + 128];
	int nDecryptedLen = 0;
	int nRtn = 0;
	do
	{
		nRtn = EVP_DecryptUpdate(&ctx,pDecrypted,&nDecryptedLen,(const unsigned char*)pPktData,ulLen - uiHeadLen);
	} while(false);

	int nTmpLen = 0;
	nRtn = EVP_DecryptFinal_ex(&ctx, pDecrypted + nDecryptedLen, &nTmpLen);
	nDecryptedLen += nTmpLen;

	rcvPacket.Decode((const char*)pDecrypted,nDecryptedLen);

	if( NULL != pDecrypted )
	{
		delete[] pDecrypted;
		pDecrypted = NULL;
	}

	return 1;
}

std::string CSufRecvHandler::GetFix3DESKey()
{
	return m_sFix3DESKey;
}

std::string CSufRecvHandler::GetDys3DESKey()
{
	return m_sDys3DESKey;
}

std::string CSufRecvHandler::Get3DESIV()
{
	return m_s3DESIV;
}

std::string CSufRecvHandler::GetSessionID()
{
	return g_Global.m_strSessionID.toStdString();
}

int CSufRecvHandler::SufRecvHandle( char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket )
{
	if (m_nLengthBytes + 1 > ulLen)
		return -1;

	char cFlag = *(pRecvBuf + m_nLengthBytes);
	if (cFlag == 0x01)
	{
		return Unzip(pRecvBuf,ulLen,rcvPacket);
	}
	else if (cFlag == 0x02 || cFlag == 0x03)
	{
		return Decrypt(pRecvBuf,ulLen,rcvPacket,cFlag);
	}
	else
	{
		return 0;
	}
}
