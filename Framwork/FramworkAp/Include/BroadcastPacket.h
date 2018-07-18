#ifndef _BROADCAST_PACKET_H
#define _BROADCAST_PACKET_H

#include <string>
#include <vector>
#include <map>
#include "pthread.h"
#include "CommAp.h"
#include "PairPacket.h"

#define BROADCAST_LENGTH_BYTES		8

class COMMAP_CLASS CBroadcastPacket:public CPairPacket
{
public:
	CBroadcastPacket();
	CBroadcastPacket(const char* pcApiName);
	~CBroadcastPacket();

public:
	const string& GetCmdID();
	std::string RouteKey()
	{
		std::string sNodeType(""),sNodeID("");
		GetParameterVal(GESS_NODE_TYPE,sNodeType);
		GetParameterVal(GESS_NODE_ID,sNodeID);
		return sNodeType + sNodeID;
	}

	//编码解码
	virtual const char* Encode(unsigned int & uiLength,CPairPacket & packet){return 0;}
	virtual const char* Encode(unsigned int & uiLength);
	virtual void  Decode(const char * pData, unsigned int uiLength);
		
private:
	std::string m_sCmdID;		//命令字 即ApiName
	std::string m_sEncode;		//编码后的字符串

	//获取递增序列号
	static long GetSID();

	static CGessMutex	m_csSID;
	static long m_uiSID;		//维护的序列号,用于心跳等通讯类报文
};
#endif