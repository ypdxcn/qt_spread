#pragma once

#include <string>
#include <QString>
class CMyRSA
{
public:
	CMyRSA(void);
public:
	~CMyRSA(void);

	static int HandleRsaEncrypt(char *&pEncryptBuf, unsigned int &uiEncryptLen, const char* pOrgBuf, unsigned int uiOrgLen, const QString &csCertFilePath);

private:
	std::string m_sPriKeyPwd;
};
