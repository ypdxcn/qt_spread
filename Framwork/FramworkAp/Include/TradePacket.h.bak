#ifndef _TRADE_PACKET_H
#define _TRADE_PACKET_H

#include "CommAp.h"
#include "PairPacket.h"
#include "strutils.h"
#include <algorithm>
using namespace strutils;


#define MSGLEN		         8U
#define SEQNO                8U
#define MSGTYPE              1U
#define EXCHCODE             4U
#define MSGFLAG              1U
#define TERMTYPE             2U
#define USERTYPE             2U
#define USERID              10U
#define AERACODE            4U
#define BRANCHID             12U
#define CTELLERID1           10U
#define CTELLERID2           10U
#define RSPCODE              8U

#define LENGTH_BYTES		MSGLEN

#define MSGFLAG_OFFSET		MSGLEN+SEQNO+MSGTYPE+EXCHCODE
#define REQHEAD_BYTES		MSGLEN+SEQNO+MSGTYPE+EXCHCODE+MSGFLAG+TERMTYPE+USERTYPE+USERID+AERACODE+BRANCHID+CTELLERID1+CTELLERID2
#define RSPHEAD_BYTES		MSGLEN+SEQNO+MSGTYPE+EXCHCODE+MSGFLAG+TERMTYPE+USERTYPE+USERID+AERACODE+BRANCHID+RSPCODE

#define HEAD_REQ             1U
#define HEAD_RSP             2U

#define TAGCODE                 "#"

typedef struct tagReqHead   //请求报文头
{

	char msg_len[MSGLEN+1];   //报文长度
    char seq_no[SEQNO+1];    //流水号
	char msg_type[MSGTYPE+1];  //报文类型
	char exch_code[EXCHCODE+1]; //交易代码
	char msg_flag[MSGFLAG+1];  //报文标识
	char term_type[TERMTYPE+1]; //终端来源
	char user_type[USERTYPE+1]; //用户类型
	char user_id[USERID+1];   //用户ID
	char area_code[AERACODE+1]; //地区代码
	char branch_id[BRANCHID+1];  //代理机构
	char c_teller_id1[CTELLERID1+1];  //一级复核操作员
	char c_teller_id2[CTELLERID2+1];  //一级复核操作员	
	
	std::string SeqNo() const
	{
		string sTmp;
		for (int i = 0; i < SEQNO; i++)
			sTmp.append(1,seq_no[i]);

		return trim(sTmp);
	}

	std::string MsgType() const
	{
		string sTmp;
		for (int i = 0; i < MSGTYPE; i++)
			sTmp.append(1,msg_type[i]);

		return trim(sTmp);
	}

	std::string MsgFlag() const
	{
		string sTmp;
		for (int i = 0; i < MSGFLAG; i++)
			sTmp.append(1,msg_flag[i]);

		return trim(sTmp);
	}

	std::string TermType() const
	{
		string sTmp;
		for (int i = 0; i < TERMTYPE; i++)
			sTmp.append(1,term_type[i]);

		return trim(sTmp);
	}

	std::string UserType() const
	{
		string sTmp;
		for (int i = 0; i < USERTYPE; i++)
			sTmp.append(1,user_type[i]);

		return trim(sTmp);
	}

	std::string UserID() const
	{
		string sTmp;
		for (int i = 0; i < USERID; i++)
			sTmp.append(1,user_id[i]);

		return trim(sTmp);
	}

	std::string AreaCode() const
	{
		string sTmp;
		for (int i = 0; i < AERACODE; i++)
			sTmp.append(1,area_code[i]);

		return trim(sTmp);
	}


	std::string BranchID() const
	{
		string sTmp;
		for (int i = 0; i < BRANCHID; i++)
			sTmp.append(1,branch_id[i]);

		return trim(sTmp);
	}

	std::string CTellerID1() const
	{
		string sTmp;
		for (int i = 0; i < CTELLERID1; i++)
			sTmp.append(1,c_teller_id1[i]);

		return trim(sTmp);
	}

	std::string CTellerID2() const
	{
		string sTmp;
		for (int i = 0; i < CTELLERID2; i++)
			sTmp.append(1,c_teller_id2[i]);

		return trim(sTmp);
	}

    // added by Jerry Lee, 2011-3-30
    // begin
    ////////////////////////////
	void SetSeqNo(const string& sSeqNo)
	{
		if (sSeqNo.empty())
			return;

		memcpy(seq_no,sSeqNo.c_str(),min(SEQNO,(unsigned int)sSeqNo.length()));
	}

	void SetMsgType(const string& sMsgType)
	{
		if (sMsgType.empty())
			return;

		memcpy(msg_type,sMsgType.c_str(), min(MSGTYPE, (unsigned int)sMsgType.length()));
	}

	void SetExchCode(const string& sExchCode)
	{
		if (sExchCode.empty())
			return;

		strncpy(exch_code,sExchCode.c_str(),min(EXCHCODE, (unsigned int)sExchCode.length()));
	}

	void SetMsgFlag(const string& sMsgFlag)
	{
		if (sMsgFlag.empty())
			return;

		memcpy(msg_flag,sMsgFlag.c_str(),min(MSGFLAG, (unsigned int)sMsgFlag.length()));
	}

	void SetTermType(const string& sTermType)
	{
		if (sTermType.empty())
			return;

		memcpy(term_type,sTermType.c_str(),min(TERMTYPE, (unsigned int)sTermType.length()));
	}

	void SetUserType(const string& sUserType)
	{
		if (sUserType.empty())
			return;

		memcpy(user_type,sUserType.c_str(),min(USERTYPE, (unsigned int)sUserType.length()));
	}

	void SetUserID(const string& sUserID)
	{
		if (sUserID.empty())
			return;

		/*if (sUserID.length() < USERID)
		{
			string sNewStr = sUserID;
			int nLen = sNewStr.length();
			int i = 0;
			for (i = 0; i < (USERID - nLen); i ++)
			    sNewStr = sNewStr + " ";
			 memcpy(user_id,sNewStr.c_str(),min(USERID,sNewStr.length()));
		}
		else*/
		    memcpy(user_id,sUserID.c_str(),min(USERID, (unsigned int)sUserID.length()));
	}

	void SetAreaCode(const string& sAreaCode)
	{
		if (sAreaCode.empty())
			return;

		memcpy(area_code,sAreaCode.c_str(),min(AERACODE, (unsigned int)sAreaCode.length()));
	}


	void SetBranchID(const string& sBranchID)
	{
		if (sBranchID.empty())
			return;

		memcpy(branch_id,sBranchID.c_str(),min(BRANCHID, (unsigned int)sBranchID.length()));
	}

	void SetCTellerID1(const string& sTellerID1)
	{
		if (sTellerID1.empty())
			return;

		memcpy(c_teller_id1,sTellerID1.c_str(),min(CTELLERID1, (unsigned int)sTellerID1.length()));
	}

	void SetCTellerID2(const string& sTellerID2)
	{
		if (sTellerID2.empty())
			return;

		memcpy(c_teller_id2,sTellerID2.c_str(),min(CTELLERID2, (unsigned int)sTellerID2.length()));
	}

	////
	void SetSeqNo(const UINT64 uiSeqNo)
	{

		char acSeq[32];
		_ui64toa(uiSeqNo, acSeq, 16);

		string sTmp = acSeq;
		return SetSeqNo(sTmp);
	}

	////
	void SetSeqNo(const char* sSeqNo)
	{
		string sTmp = sSeqNo;
		return SetSeqNo(sTmp);
	}

	void SetMsgType(const char* sMsgType)
	{
		string sTmp = sMsgType;
		return SetMsgType(sTmp);
	}

	void SetExchCode(const char* sExchCode)
	{
		string sTmp = sExchCode;
		return SetExchCode(sTmp);
	}

	void SetMsgFlag(const char* sMsgFlag)
	{
		string sTmp = sMsgFlag;
		return SetMsgFlag(sTmp);
	}

	void SetTermType(const char* sTermType)
	{
		string sTmp = sTermType;
		return SetTermType(sTmp);
	}

	void SetUserType(const char* sUserType)
	{
		string sTmp = sUserType;
		return SetUserType(sTmp);
	}

	void SetUserID(const char* sUserID)
	{
		string sTmp = sUserID;
		return SetUserID(sTmp);
	}

	void SetAreaCode(const char* sAreaCode)
	{
		string sTmp = sAreaCode;
		return SetAreaCode(sTmp);
	}


	void SetBranchID(const char* sBranchID)
	{
		string sTmp = sBranchID;
		return SetBranchID(sTmp);
	}

	void SetCTellerID1(const char* sTellerID1)
	{
		string sTmp = sTellerID1;
		return SetCTellerID1(sTmp);
	}

	void SetCTellerID2(const char* sTellerID2)
	{
		string sTmp = sTellerID2;
		return SetCTellerID2(sTmp);
	}

	void IniRequst(const string& sExchCode, UINT64 &uiSeqNo)
	{
		SetExchCode(sExchCode);
		SetSeqNo(uiSeqNo++);
	}

    // end
} HEADER_REQ,*PHEADER_REQ ;

typedef struct tagRspHead   //响应报文头
{
	char msg_len[MSGLEN+1];   //报文长度
    char seq_no[SEQNO+1];    //流水号
	char msg_type[MSGTYPE+1];  //报文类型
	char exch_code[EXCHCODE+1]; //交易代码
	char msg_flag[MSGFLAG+1];  //报文标识
	char term_type[TERMTYPE+1]; //终端来源
	char user_type[USERTYPE+1]; //用户类型
	char user_id[USERID+1];   //用户ID
	char area_code[AERACODE+1]; //地区代码
	char branch_id[BRANCHID+1];  //代理机构
	char rsp_code[RSPCODE+1];      //响应代码

} HEADER_RSP,*PHEADER_RSP;


class COMMAP_CLASS CTradePacket:public CPairPacket
{
public:
	CTradePacket(void);
	~CTradePacket(void);

	const char* Encode(unsigned int & uiLength,CPairPacket & packet){return 0;}
	const char* Encode(unsigned int & uiLength);
	void  Decode(const char * pData, unsigned int uiLength);

	std::string RouteKey()
	{
		std::string sRouteKey("");
		if (m_HeadMsg.m_type == 1)
		{
			sRouteKey=trim(m_HeadMsg.uheader.Req_Hmsg.term_type);
			sRouteKey+=trim(m_HeadMsg.uheader.Req_Hmsg.user_type);
			sRouteKey+=trim(m_HeadMsg.uheader.Req_Hmsg.user_id);
		}
		else
		{
			sRouteKey=trim(m_HeadMsg.uheader.Rsp_Hmsg.term_type);
			sRouteKey+=trim(m_HeadMsg.uheader.Rsp_Hmsg.user_type);
			sRouteKey+=trim(m_HeadMsg.uheader.Rsp_Hmsg.user_id);
		}
		return sRouteKey;
	}
	const string& GetCmdID();

	// add by xrs 20131104 直接获取响应码
	int GetRspCode( string &sCode );

	int GetHeader(HEADER_REQ& reqHeader);
	int GetHeader(HEADER_RSP& reqHeader);
	void SetHeader(HEADER_REQ& reqHeader);
	void SetHeader(HEADER_RSP& reqHeader);
	int PktType() {return m_HeadMsg.m_type;}

	std::string  Print(std::string sParaSeperator = "#",std::string sValSeperator = "=");
private:
	void  DecodeReqHead(const char * pData,HEADER_REQ& v);
	void  DecodeRspHead(const char * pData,HEADER_RSP& v);

	std::string AssembleReqHead(HEADER_REQ& v);
	std::string AssembleRspHead(HEADER_RSP& v);

    typedef struct tagHeader
	{
		int m_type ;
		union
		{
			HEADER_REQ Req_Hmsg;
			HEADER_RSP Rsp_Hmsg;
		} uheader;
	} Head;

    Head m_HeadMsg;

	std::string m_sCmdID;		//命令字 即ApiName
	std::string m_sEncode;		//编码后的字符串

	//获取递增序列号
	static long GetSID();

	static CGessMutex	m_csSID;
	static long m_uiSID;		//维护的序列号,用于心跳等通讯类报文
};
#endif