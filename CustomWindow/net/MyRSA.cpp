//#include "StdAfx.h"
#include "MyRSA.h"

#include <openssl/ossl_typ.h>
#include <openssl/pkcs12.h>
#include <openssl/pem.h>

CMyRSA::CMyRSA(void)
{
}

CMyRSA::~CMyRSA(void)
{
}

int CMyRSA::HandleRsaEncrypt( char *&pEncryptBuf, 
							 unsigned int &uiEncryptLen, 
							 const char* pOrgBuf,      // 
							 unsigned int uiOrgLen, // uiOrgLen
							 const QString &csCertFilePath )
{
	BIO *in = NULL;
	EVP_PKEY * pPubKeyPem = 0;
	X509 *x509Pem = NULL;

	if ((in = BIO_new_file (csCertFilePath.toStdString().c_str(), "r")) == 0)
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

	const char * pData = pOrgBuf;

	// 分配存储密文的缓存区
	unsigned int uiTotalSize = (uiOrgLen/100 + 2) * 128;
	pEncryptBuf = new char[uiTotalSize];
	memset(pEncryptBuf, 0, uiTotalSize);

	// 分配用来存储一段加密的临时缓冲区
	unsigned int nRsaSize = RSA_size(pPubKeyPem->pkey.rsa); // 获得临时缓存区的长度
	unsigned char* pEncrpted = new unsigned char[nRsaSize];


	int nInLen = 0;  // 需要加密的段的长度
	uiEncryptLen = 0; // 加密缓冲区的已经加密的长度
	for (size_t n = 100; n - 100 < uiOrgLen; n += 100)
	{
		// 获得这次加密的明文的长度（一般为100，除了最后一段）
		nInLen = uiOrgLen / n == 0 ? uiOrgLen - (n - 100) : 100;

		// 给这段明文加密到pEncrpted缓冲区，并获得加密后的实际长度
		int nTmp = RSA_public_encrypt(nInLen,(const unsigned char*)pData,pEncrpted,pPubKeyPem->pkey.rsa,RSA_PKCS1_PADDING);

		// 复制加密后的密文到加密缓存区
		memcpy(pEncryptBuf + uiEncryptLen, pEncrpted, nTmp);

		// 获得加密缓冲区的下一次加密的起始位置
		uiEncryptLen += nTmp;

		// 更新未加密的明文的起始位置
		pData += nInLen;
	}

	if (x509Pem) 
	{
		X509_free(x509Pem);
	}

	delete []pEncrpted;

	return 0;
}
