#pragma once

#define g_SufRecvHandler CSufRecvHandler::GetInstance()

class CPacket;
#include <string>
using namespace std;

class CSufRecvHandler
{
private:
	CSufRecvHandler( void );
	virtual ~CSufRecvHandler( void) ;

	CSufRecvHandler( const CSufRecvHandler&) ;            // 不实现.
	CSufRecvHandler& operator= ( const CSufRecvHandler& ); // 不实现.

public:
	static CSufRecvHandler& GetInstance();

	// 接收数据特别的处理 SufRecvHandle具体处理可能是解密,解压缩, 若无需任何处理则返回0,若出错返回-1,否则返回1 
	int SufRecvHandle(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket);

private:
    // 解压缩
	int Unzip(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket);

	// 3DES解密
	int Decrypt(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket, char cMode);

	// 获取固定3DESKey
	string GetFix3DESKey();

	// 获取动态3DESKey
	string GetDys3DESKey();

	// 获取向量
	string Get3DESIV();

	string GetSessionID();

private:
	int        m_nLengthBytes;
	int        m_nMaxZipPercent; // 最大压缩比例

	string m_sFix3DESKey;
	string m_sDys3DESKey;
	string m_s3DESIV;
};
