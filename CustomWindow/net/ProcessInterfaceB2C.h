#ifndef _PROCESS_INTERFACE_B2C_H
#define _PROCESS_INTERFACE_B2C_H

#include "CommApCln.h"
#include "TcpShortCp.h"
#include "ProtocolProcess.h"

using namespace std;

class COMMAPCLIENT_CLASS CProcessInterfaceB2C : public CTcpShortProcess
{
public:
	CProcessInterfaceB2C(void);
	virtual ~CProcessInterfaceB2C(void);
	int Init(CConfig* pCfg);

	//发送报文特别的处理 PreSendHandle具体处理可能是加密,压缩, 若无需任何处理则返回0,若出错返回-1,否则返回实际发送的字节数
	int PreSendHandle(CPacket &sndPacket);
	//接收数据特别的处理 SufRecvHandle具体处理可能是解密,解压缩, 若无需任何处理则返回0,若出错返回-1,否则返回1 
	int SufRecvHandle(char* pRecvBuf, unsigned int ulLen, CPacket &rcvPacket);

protected:
	//父类定义的回调函数实现
	void GetPacketInfo(PacketInfo & stInfo);//报文格式信息
private:
	PacketInfo m_stInfo;
	string m_s3DesKey;
	string m_s3DesIV;
	string m_sSessionID;
};
#endif
