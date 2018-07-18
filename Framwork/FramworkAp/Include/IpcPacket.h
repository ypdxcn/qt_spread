#ifndef _IPC_PACKET_H
#define _IPC_PACKET_H

#include <string>
#include <vector>
#include <map>
#include "CommAp.h"
#include "PairPacket.h"

#define IPC_LENGTH_BYTES		8	//长度占用字节数

class COMMAP_CLASS CIpcPacket:public CPairPacket
{
public:
	CIpcPacket();
	CIpcPacket(const string& sCmdID);
	virtual ~CIpcPacket();

public:
	//编码解码
	virtual const char* Encode(unsigned int & uiLength,CPairPacket & packet){return 0;}
	virtual const char* Encode(unsigned int & uiLength);
	virtual void  Decode(const char * pData, unsigned int uiLength);
	
	const string& GetCmdID();
	std::string RouteKey()
	{
		std::string sNodeID("");
		GetParameterVal("node_id",sNodeID);
		return sNodeID;
	}

private:
	std::string m_sCmdID;		//命令字
	std::string m_sEncode;		//编码后的字符串

	//获取递增序列号
	static long GetSID();

	static CGessMutex	m_csSID;
	static unsigned int m_uiSID;		//维护的序列号,用于心跳等通讯类报文
};
#endif