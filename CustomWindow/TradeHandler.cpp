#include "TradeHandler.h"
#include "Const.h"
#include "TraderCpMgr.h"
#include "Global.h"
#include "CustomInfoMgrEx.h"

#include "HJGlobalFun.h"

CTradeHandler g_TradeHandler;

CTradeHandler::CTradeHandler(void)
{
}

CTradeHandler::~CTradeHandler(void)
{
}

bool CTradeHandler::IsTradeSus( const string &sRspID )
{
	return sRspID == CHJGlobalFun::qstr2str(gc_ErrCodeSus )? true : false;
}

int CTradeHandler::IsSuccess( int nRet, const string &sRspCode )
{
	if( nRet == 0 && IsTradeSus(sRspCode) )
		return 0;
	else
		return -1;
}

bool CTradeHandler::NeedShowTips( int nRet, ETipsShowType eType )
{
	if( nRet == 0 )
	{
		if( eType == e_TipsShowType_always || eType == e_TipsShowType_Suc )
			return true;
	}
	else
	{
		if( eType == e_TipsShowType_always || eType == e_TipsShowType_Fail )
			return true;
	}

	return false;
}

int CTradeHandler::GetCustomInfo(  Rsp1020 &rsp1020, Req1020& req1020, ETipsShowType eType, bool bBackground /* = false */ )
{
	req1020.oper_flag     = 1;
	req1020.acct_no       = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
	req1020.is_check_stat = "1";  // 这个标志是啥意思？

	// 发送请求
	int nRet = g_TraderCpMgr.HandlePacket(rsp1020, req1020, "1020", 0, bBackground);

	// 是否需要弹出提示
	int nMyRet = IsSuccess(nRet,rsp1020.rsp_code);
	if( NeedShowTips(nMyRet,eType) )
	{
		// 是否提示
	}

	if( nMyRet == 0 )
	{
		// 刷新内存信息
		g_CusInfo.UpdateCustomInfo(rsp1020,req1020);
	}

	return nMyRet;
}

int CTradeHandler::GetCushAccessFlow( Rsp3010 &rsp3010, Req3010& req3010, ETipsShowType eType, bool bBackground /* = false */ )
{
	req3010.oper_flag     = 4;
	req3010.acct_no       = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
	 

	// 发送请求
	int nRet = g_TraderCpMgr.HandlePacket(rsp3010, req3010, "3010", 0, bBackground);

	// 是否需要弹出提示
	int nMyRet = IsSuccess(nRet,rsp3010.rsp_code);
	if( NeedShowTips(nMyRet,eType) )
	{
		if(nMyRet == -1)
		{
			//QMessageBox::information(CONSTANT_CONNECT_SERVER_FAIL_TIPS, MB_TOPMOST);
		}
		else
		{
			g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp3010.rsp_code), CHJGlobalFun::str2qstr(rsp3010.rsp_msg));
		}
	}

	return nMyRet;
}
