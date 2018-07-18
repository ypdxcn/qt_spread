#include "TradePacket.h"

//递增序列号
long CTradePacket::m_uiSID = 0;
CGessMutex	CTradePacket::m_csSID;

CTradePacket::CTradePacket(void):
m_sEncode("")
{
	memset(&m_HeadMsg,0x00,sizeof(m_HeadMsg));
}

CTradePacket::~CTradePacket(void)
{
}

/******************************************************************************
函数描述:对外提供的接口函数,socket发送前按请求报文头格式进行组装
返回值  :编码后的字符串
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
std::string CTradePacket::AssembleReqHead(HEADER_REQ& v)
{
	std::string sEncode = "";
	char  tempcode[128];
	char  code[1024];
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',SEQNO);
	sprintf(code,tempcode,v.seq_no);  
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',MSGTYPE);
	sprintf(code,tempcode,v.msg_type);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',EXCHCODE);
	sprintf(code,tempcode,v.exch_code);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',MSGFLAG);
	sprintf(code,tempcode,v.msg_flag);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',TERMTYPE);
	sprintf(code,tempcode,v.term_type);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',USERTYPE);
	sprintf(code,tempcode,v.user_type);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',USERID);
	sprintf(code,tempcode,v.user_id); 
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',AERACODE);
	sprintf(code,tempcode,v.area_code); 
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',BRANCHID);
	sprintf(code,tempcode,v.branch_id); 
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',CTELLERID1);
	sprintf(code,tempcode,v.c_teller_id1); 
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',CTELLERID2);
	sprintf(code,tempcode,v.c_teller_id2); 
	sEncode+=code;
	return sEncode;
}

/******************************************************************************
函数描述:对外提供的接口函数,socket发送前按响应报文头格式进行组装
返回值  :编码后的字符串
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
std::string CTradePacket::AssembleRspHead(HEADER_RSP& v)
{
	std::string sEncode = "";
	char  tempcode[128];
	char  code[1024];
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',SEQNO);
	sprintf(code,tempcode,v.seq_no);  
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',MSGTYPE);
	sprintf(code,tempcode,v.msg_type);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',EXCHCODE);
	sprintf(code,tempcode,v.exch_code);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',MSGFLAG);
	sprintf(code,tempcode,v.msg_flag);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',TERMTYPE);
	sprintf(code,tempcode,v.term_type);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',USERTYPE);
	sprintf(code,tempcode,v.user_type);
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',USERID);
	sprintf(code,tempcode,v.user_id); 
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',AERACODE);
	sprintf(code,tempcode,v.area_code); 
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',BRANCHID);
	sprintf(code,tempcode,v.branch_id); 
	sEncode+=code;
	memset(tempcode,0x00,128);
	memset(code,0x00,1024);
	sprintf(tempcode,"%c%ds",'%',RSPCODE);
	sprintf(code,tempcode,v.rsp_code); 
	sEncode+=code;

	return sEncode;
}


/******************************************************************************
函数描述:对外提供的接口函数,socket发送前按报文格式进行组装
返回值  :编码后的内存块
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
const char* CTradePacket::Encode(unsigned int & usLen)
{
	int nMsgFlag = m_HeadMsg.m_type;
	if(nMsgFlag == HEAD_REQ)
	{
		m_sEncode = AssembleReqHead(m_HeadMsg.uheader.Req_Hmsg);
	}
	else if(nMsgFlag == HEAD_RSP)
	{
		m_sEncode = AssembleRspHead(m_HeadMsg.uheader.Rsp_Hmsg);
	}
	else
	{
		//CRLog
	}
	m_sEncode +=TAGCODE;
	m_sEncode += AssembleBody(GESS_FLD_SEPERATOR,GESS_VAL_SEPERATOR);

	char szTmp[128];
	memset(szTmp,0x00,128);
	sprintf(szTmp,"%c0%dd",'%',LENGTH_BYTES);
	char szLen[128];
	sprintf(szLen,szTmp,m_sEncode.length());
	szLen[LENGTH_BYTES] = '\0';
	
	m_sEncode = szLen + m_sEncode;
	usLen = m_sEncode.length();
	return m_sEncode.c_str();
}

/******************************************************************************
函数描述:对外提供的接口函数,拆分请求报文头
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CTradePacket::DecodeReqHead(const char * pData, HEADER_REQ& v )
{
	const char* pPos = pData;
	memcpy(v.msg_len,pPos,MSGLEN); 
	pPos += MSGLEN;
	memcpy(v.seq_no,pPos,SEQNO); 
	pPos += SEQNO;
	memcpy(v.msg_type,pPos,MSGTYPE);
	pPos += MSGTYPE;
	memcpy(v.exch_code,pPos,EXCHCODE);
	pPos += EXCHCODE;
	memcpy(v.msg_flag,pPos,MSGFLAG);
	pPos += MSGFLAG;
	memcpy(v.term_type,pPos,TERMTYPE);
	pPos += TERMTYPE;
	memcpy(v.user_type,pPos,USERTYPE);
	pPos += USERTYPE;
	memcpy(v.user_id,pPos,USERID);
	pPos += USERID;
	memcpy(v.area_code,pPos,AERACODE);
	pPos += AERACODE;
	memcpy(v.branch_id,pPos,BRANCHID);
	pPos += BRANCHID;
	memcpy(v.c_teller_id1,pPos,CTELLERID1);
	pPos += CTELLERID1;
	memcpy(v.c_teller_id1,pPos,CTELLERID2);
}


/******************************************************************************
函数描述:对外提供的接口函数,拆分响应报文头
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CTradePacket::DecodeRspHead(const char * pData, HEADER_RSP& v )
{
	const char* pPos = pData;
	memcpy(v.msg_len,pPos,MSGLEN); 
	pPos += MSGLEN;
	memcpy(v.seq_no,pPos,SEQNO); 
	pPos += SEQNO;
	memcpy(v.msg_type,pPos,MSGTYPE);
	pPos += MSGTYPE;
	memcpy(v.exch_code,pPos,EXCHCODE);
	pPos += EXCHCODE;
	memcpy(v.msg_flag,pPos,MSGFLAG);
	pPos += MSGFLAG;
	memcpy(v.term_type,pPos,TERMTYPE);
	pPos += TERMTYPE;
	memcpy(v.user_type,pPos,USERTYPE);
	pPos += USERTYPE;
	memcpy(v.user_id,pPos,USERID);
	pPos += USERID;
	memcpy(v.area_code,pPos,AERACODE);
	pPos += AERACODE;
	memcpy(v.branch_id,pPos,BRANCHID);
	pPos += BRANCHID;
	memcpy(v.rsp_code,pPos,8);
}


/******************************************************************************
函数描述:对外提供的接口函数,拆分响应报文
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void CTradePacket::Decode(const char * pData, unsigned int nLength)
{
	char cType[MSGFLAG+1];
	memset(cType,0x00,MSGFLAG+1);
    memcpy(cType,pData+MSGFLAG_OFFSET,MSGFLAG);
	std::string sMsgType=cType;
	m_HeadMsg.m_type = FromString<int>(sMsgType);
	string sBody;
    
	char* pcBuf = new char[nLength+1];
	if(m_HeadMsg.m_type == HEAD_REQ)
	{
		DecodeReqHead(pData, m_HeadMsg.uheader.Req_Hmsg);

		int nHeadLen = REQHEAD_BYTES;
		memcpy(pcBuf,pData + nHeadLen,nLength - nHeadLen );
		pcBuf[nLength-nHeadLen] = '\0';
	    sBody = pcBuf;
	}
	else if(m_HeadMsg.m_type == HEAD_RSP)
	{
		DecodeRspHead(pData,m_HeadMsg.uheader.Rsp_Hmsg);

		int nHeadLen = RSPHEAD_BYTES;
		memcpy(pcBuf,pData + nHeadLen,nLength -nHeadLen );
		pcBuf[nLength-nHeadLen] = '\0';
	    sBody = pcBuf;
	}
	else
	{
		//CRLog
	}
	delete []pcBuf;

	ParseBody(sBody,GESS_FLD_SEPERATOR,GESS_VAL_SEPERATOR);

	int nMsgFlag=m_HeadMsg.m_type;
	if(nMsgFlag==1)
	{
		m_sCmdID=m_HeadMsg.uheader.Req_Hmsg.exch_code;
	}
	else
	{
		m_sCmdID=m_HeadMsg.uheader.Rsp_Hmsg.exch_code;
	}
}

/******************************************************************************
函数描述:对外提供的接口函数,取交易代码
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
const string& CTradePacket::GetCmdID()
{
	return m_sCmdID;
}

/******************************************************************************
函数描述:对外提供的接口函数,取请求报文头
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int  CTradePacket::GetHeader(HEADER_REQ& stReq)
{
	if(m_HeadMsg.m_type==HEAD_REQ)
	{
		memset(&stReq,0x00,sizeof(stReq));
		strncpy(stReq.msg_len,m_HeadMsg.uheader.Req_Hmsg.msg_len,MSGLEN);
		strncpy(stReq.seq_no,m_HeadMsg.uheader.Req_Hmsg.seq_no,SEQNO);
		strncpy(stReq.msg_type,m_HeadMsg.uheader.Req_Hmsg.msg_type,MSGTYPE);
		strncpy(stReq.exch_code,m_HeadMsg.uheader.Req_Hmsg.exch_code,EXCHCODE);
		strncpy(stReq.msg_flag,m_HeadMsg.uheader.Req_Hmsg.msg_flag,MSGFLAG);
		strncpy(stReq.term_type,m_HeadMsg.uheader.Req_Hmsg.term_type,TERMTYPE);
		strncpy(stReq.user_type,m_HeadMsg.uheader.Req_Hmsg.user_type,USERTYPE);
		strncpy(stReq.user_id,m_HeadMsg.uheader.Req_Hmsg.user_id,USERID);
		strncpy(stReq.area_code,m_HeadMsg.uheader.Req_Hmsg.area_code,AERACODE);
		strncpy(stReq.branch_id,m_HeadMsg.uheader.Req_Hmsg.branch_id,BRANCHID);
		strncpy(stReq.c_teller_id1,m_HeadMsg.uheader.Req_Hmsg.c_teller_id1,CTELLERID1);
		strncpy(stReq.c_teller_id2,m_HeadMsg.uheader.Req_Hmsg.c_teller_id2,CTELLERID2);
		return 0;		
	}
	else
	{
		return -1;		
	}
}
/******************************************************************************
函数描述:对外提供的接口函数,取响应报文
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
int  CTradePacket::GetHeader(HEADER_RSP& stRsp)
{
	if(m_HeadMsg.m_type == HEAD_RSP)
	{
		memset(&stRsp,0x00,sizeof(stRsp));
		strncpy(stRsp.msg_len,m_HeadMsg.uheader.Rsp_Hmsg.msg_len,MSGLEN);
		strncpy(stRsp.seq_no,m_HeadMsg.uheader.Rsp_Hmsg.seq_no,SEQNO);
		strncpy(stRsp.msg_type,m_HeadMsg.uheader.Rsp_Hmsg.msg_type,MSGTYPE);
		strncpy(stRsp.exch_code,m_HeadMsg.uheader.Rsp_Hmsg.exch_code,EXCHCODE);
		strncpy(stRsp.msg_flag,m_HeadMsg.uheader.Rsp_Hmsg.msg_flag,MSGFLAG);
		strncpy(stRsp.term_type,m_HeadMsg.uheader.Rsp_Hmsg.term_type,TERMTYPE);
		strncpy(stRsp.user_type,m_HeadMsg.uheader.Rsp_Hmsg.user_type,USERTYPE);
		strncpy(stRsp.user_id,m_HeadMsg.uheader.Rsp_Hmsg.user_id,USERID);
		strncpy(stRsp.area_code,m_HeadMsg.uheader.Rsp_Hmsg.area_code,AERACODE);
		strncpy(stRsp.branch_id,m_HeadMsg.uheader.Rsp_Hmsg.branch_id,BRANCHID);
		strncpy(stRsp.rsp_code,m_HeadMsg.uheader.Rsp_Hmsg.rsp_code,RSPCODE);
		return 0;		
	}
	else
	{
		return -1;
	}
}

/******************************************************************************
函数描述:对外提供的接口函数,设置请求报文
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void  CTradePacket::SetHeader(HEADER_REQ& stReq)
{
	m_HeadMsg.m_type = HEAD_REQ;

	strncpy(m_HeadMsg.uheader.Req_Hmsg.msg_len,stReq.msg_len,MSGLEN);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.seq_no,stReq.seq_no,SEQNO);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.msg_type,stReq.msg_type,MSGTYPE);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.exch_code,stReq.exch_code,EXCHCODE);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.msg_flag,stReq.msg_flag,MSGFLAG);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.term_type,stReq.term_type,TERMTYPE);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.user_type,stReq.user_type,USERTYPE);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.user_id,stReq.user_id,USERID);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.area_code,stReq.area_code,AERACODE);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.branch_id,stReq.branch_id,BRANCHID);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.c_teller_id1,stReq.c_teller_id1,CTELLERID1);
	strncpy(m_HeadMsg.uheader.Req_Hmsg.c_teller_id2,stReq.c_teller_id2,CTELLERID2);
	
	m_sCmdID = stReq.exch_code;
}

/******************************************************************************
函数描述:对外提供的接口函数,设置响应报文
创建者  :张伟
创建日期:2008.07.22
修改记录:
******************************************************************************/
void  CTradePacket:: SetHeader(HEADER_RSP& stRsp)
{
	m_HeadMsg.m_type = HEAD_RSP;

	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.msg_len,stRsp.msg_len,MSGLEN);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.seq_no,stRsp.seq_no,SEQNO);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.msg_type,stRsp.msg_type,MSGTYPE);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.exch_code,stRsp.exch_code,EXCHCODE);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.msg_flag,stRsp.msg_flag,MSGFLAG);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.term_type,stRsp.term_type,TERMTYPE);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.user_type,stRsp.user_type,USERTYPE);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.user_id,stRsp.user_id,USERID);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.area_code,stRsp.area_code,AERACODE);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.branch_id,stRsp.branch_id,BRANCHID);
	strncpy(m_HeadMsg.uheader.Rsp_Hmsg.rsp_code,stRsp.rsp_code,RSPCODE);

	m_sCmdID = stRsp.exch_code;
}

std::string  CTradePacket::Print(std::string sParaSeperator,std::string sValSeperator)
{
	stringstream ss;
	if (m_HeadMsg.m_type == HEAD_REQ)
	{
		ss << m_HeadMsg.uheader.Req_Hmsg.msg_len;
		ss << m_HeadMsg.uheader.Req_Hmsg.seq_no;
		ss << m_HeadMsg.uheader.Req_Hmsg.msg_type;
		ss << m_HeadMsg.uheader.Req_Hmsg.exch_code;
		ss << m_HeadMsg.uheader.Req_Hmsg.msg_flag;
		ss << m_HeadMsg.uheader.Req_Hmsg.term_type;
		ss << m_HeadMsg.uheader.Req_Hmsg.user_type;
		ss << m_HeadMsg.uheader.Req_Hmsg.user_id;
		ss << m_HeadMsg.uheader.Req_Hmsg.area_code;
		ss << m_HeadMsg.uheader.Req_Hmsg.branch_id;
		ss << m_HeadMsg.uheader.Req_Hmsg.c_teller_id1;
		ss << m_HeadMsg.uheader.Req_Hmsg.c_teller_id2;
	}
	else
	{
		ss << m_HeadMsg.uheader.Rsp_Hmsg.msg_len;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.seq_no;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.msg_type;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.exch_code;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.msg_flag;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.term_type;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.user_type;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.user_id;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.area_code;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.branch_id;
		ss << m_HeadMsg.uheader.Rsp_Hmsg.rsp_code;
	}

	string sStr = ss.str();
	sStr += CPairPacket::Print(sParaSeperator,sValSeperator);
	return sStr;
}