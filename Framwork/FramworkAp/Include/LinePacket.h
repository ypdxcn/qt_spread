#ifndef _CMD_LINE_PACKET_H
#define _CMD_LINE_PACKET_H

#include <string>
#include <vector>
#include <map>
#include "pthread.h"
#include "CommAp.h"


class COMMAP_CLASS CPacketLineReq:public CPacketRouteable
{
public:
	CPacketLineReq();
	CPacketLineReq(const string& sLine);
	~CPacketLineReq();

public:
	const string& GetCmdID();
	void AddCmdID(const string& sCmdID);
	void AddPara(const string& sCmdID);
	int GetPara(vector<string>& vecPara);

	std::string RouteKey(){return m_sRouteKey;}
	void AddRouteKey(const string& sRouteKey){m_sRouteKey = sRouteKey;}

	//±àÂë½âÂë
	virtual const char* Encode(unsigned int & uiLength);
	virtual void  Decode(const char * pData, unsigned int uiLength);
		
private:
	std::string m_sCmdID;		//ÃüÁî×Ö ¼´ApiName
	std::string m_sEncode;		//±àÂëºóµÄ×Ö·û´®
	std::string m_sRouteKey;

	vector<string> m_vecPara;
};

class COMMAP_CLASS CPacketLineRsp:public CPacketRouteable
{
public:
	CPacketLineRsp();
	CPacketLineRsp(const string& sLine);
	~CPacketLineRsp();

public:
	void AddRspMsg(const string& sCmdID);
	string GetRspMsg();

	const string& GetCmdID(){return m_sCmdID;}
	std::string RouteKey(){return m_sRouteKey;}
	void AddRouteKey(const string& sRouteKey){m_sRouteKey = sRouteKey;}

	//±àÂë½âÂë
	virtual const char* Encode(unsigned int & uiLength);
	virtual void  Decode(const char * pData, unsigned int uiLength);
	
private:
	std::string m_sCmdID;		//ÃüÁî×Ö ¼´ApiName

	std::string m_sEncode;		//±àÂëºóµÄ×Ö·û´®
	std::string m_sRouteKey;
};


#endif