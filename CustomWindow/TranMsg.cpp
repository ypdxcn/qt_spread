#include "TranMsg.h"

#include "Template.h"
#include "Global.h"

CTranMsg::CTranMsg(void)
{
}

CTranMsg::~CTranMsg(void)
{
}

bool CTranMsg::Handle9010( Rsp9010 &rsp, Req9010 &req )
{
	return TranTemplate(rsp, req, "9010");
}

bool CTranMsg::Handle6002( Rsp6002 &rsp, Req6002 &req, QVector< pair<QString,QString> > &vecPara )
{
	// 初始化请求头
	g_HeaderReq.IniRequst("6002", g_SeqNo);

	req.oper_flag       = 1;
	req.login_branch_id = g_HeaderReq.branch_id;
	req.login_teller_id = g_Global.m_strUserID.toStdString();

	HEADER_RSP stHeaderRsp;
	if( 0 == g_TraderCpMgr.Tran6002Handle(g_HeaderReq, req, stHeaderRsp, rsp, vecPara, 20) )
	{
		if(strcmp(stHeaderRsp.rsp_code, RSP_SUCCESS_CODE) == 0)
			return true;
		/*else
			QMessageBox(rsp.rsp_msg);*/
	}
	//else
	//	QMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);

	return false;
}

bool CTranMsg::Handle1001( Rsp1001 &rsp )
{
	Req1001 req;
	req.oper_flag = 1;

	return TranTemplate( rsp, req, "1001" );
}


bool CTranMsg::Handle1002( Rsp1002 &rsp )
{
	Req1002 req;
	req.oper_flag = 1;

	return TranTemplate( rsp, req, "1002" );
}

bool CTranMsg::Handle1004( Rsp1004 &rsp )
{
	Req1004 req;
	req.oper_flag = 1;

	return TranTemplate( rsp, req, "1004" );
}

bool CTranMsg::Handle9030( Rsp9030 &rsp, const QString &sParaID )
{
	Req9030 req; //请求报文体

	req.oper_flag  = 1;
	req.para_id = sParaID.toStdString();

	return TranTemplate( rsp, req, "9030" );
}

bool CTranMsg::Handle5101( Rsp5101 &rsp, Req5101 &req )
{
	return TranTemplate( rsp, req, "5101" );
}


bool CTranMsg::Handle4074( Rsp4074 &rsp, Req4074 &req )
{
	req.acct_no = g_Global.m_strUserID.toStdString();

	bool bRet = TranTemplate( rsp, req, "4074" );

	// 如果发送成功，且是增加，则标记为当前客户有服务器预埋单
	if( bRet )
	{
		/*if( req.oper_flag == 2 )
			g_bHasServerPreOrder = true;*/
	}

	return bRet;
}

bool CTranMsg::Handle3201( Rsp3201 &rsp )
{
	Req3201 req;
	req.oper_flag = 1;
	req.acct_no = g_Global.m_strUserID.toStdString();

	return TranTemplate( rsp, req, "3201" );
}
