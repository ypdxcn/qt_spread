#include "ModifySetting.h"
#include "Global.h"
#include  "app.h"

CModifySetting::CModifySetting(void)
{
}

CModifySetting::~CModifySetting(void)
{
}

void CModifySetting::ModifyTradeConfirm( bool bConfirm )
{
	g_Global.m_bAskConfirm = bConfirm;
	QString str = bConfirm ? "1" : "0";
	//WritePrivateProfileString("Order", "ConfirmOrder", str, g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "ConfirmOrder", std::move(str));
}

void CModifySetting::ModifyCancelConfirm( bool bConfirm )
{
	g_Global.m_bConfirmUnorder = bConfirm;
	QString str = bConfirm ? "1" : "0";
	//WritePrivateProfileString("OrderModify", "ConfirmUnorder", str, g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "OrderModify", "ConfirmUnorder", std::move(str));

}

void CModifySetting::ModifyOppCovConfirm( bool bConfirm )
{
	g_Global.m_bOppCovConfirm = bConfirm;
	QString str = bConfirm ? "1" : "0";
	//WritePrivateProfileString("OrderModify", "ConfirmOppCov", str, g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "OrderModify", "ConfirmOppCov", std::move(str));
}

void CModifySetting::ModifyCovPosiConfirm( bool bConfirm )
{
	g_Global.m_bCovPosiConfirm = bConfirm;
	QString str = bConfirm ? "1" : "0";
	//WritePrivateProfileString("Order", "CovPosiConfirm", str, g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "CovPosiConfirm", std::move(str));


}

void CModifySetting::ModifyTipsAskSus( bool bTips )
{
	g_Global.m_bTipsAskSus = bTips;

	//WritePrivateProfileString("Order", "TipsAskSus", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "TipsAskSus", bTips ? "1" : "0");


}

void CModifySetting::ModifyTipsAskFail( bool bTips )
{
	g_Global.m_bTipsAskFail = bTips;

	//WritePrivateProfileString("Order", "TipsAskFail", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "TipsAskFail", bTips ? "1" : "0");

}

void CModifySetting::ModifyTipsOrderMatch( bool bTips )
{
	g_Global.m_bTipsOrderMatch = bTips;

	//WritePrivateProfileString("Order", "TipsOrderMatch", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "TipsOrderMatch", bTips ? "1" : "0");
}

void CModifySetting::ModifyTipsCancelSus( bool bTips )
{
	g_Global.m_bTipsCancelSus = bTips;

	//WritePrivateProfileString("Order", "TipsCancelSus", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "TipsCancelSus", bTips ? "1" : "0");
}

void CModifySetting::ModifyTipsCancelFail( bool bTips )
{
	g_Global.m_bTipsCancelFail = bTips;

	//WritePrivateProfileString("Order", "TipsCancelFail", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "TipsCancelFail", bTips ? "1" : "0");
}

void CModifySetting::ModifyTipsPreOrder( bool bTips )
{
	g_Global.m_bPreOrderConfirm = bTips;

	//WritePrivateProfileString("Order", "PreOrderConfirm", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "PreOrderConfirm", bTips ? "1" : "0");
}

void CModifySetting::ModifyTipsAskOverFlow( bool bTips )
{
	g_Global.m_bPosiOFConfirm = bTips;

	//WritePrivateProfileString("Order", "AskOFCfm", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "AskOFCfm", bTips ? "1" : "0");
}

void CModifySetting::ModifyTipsAskPreOrder( bool bTips )
{
	g_Global.m_bTipsAddPreOrder = bTips;

	//WritePrivateProfileString("Order", "CreatePreOrderTip", bTips ? "1" : "0", g_Global.GetUserIniPath());
	App::WritePriProfileString(g_Global.GetUserIniPath(), "Order", "CreatePreOrderTip", bTips ? "1" : "0");
}
