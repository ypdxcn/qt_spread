#include "frmOption_2.h"
#include "ModifySetting.h"
#include "Global.h"
#include "app.h"
#include "TraderCpMgr.h"
#include  "HJConstant.h"


frmOption_2::frmOption_2(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	LoadFromIni();


}

frmOption_2::~frmOption_2()
{
}
void frmOption_2::saveIni()
{

	QString sfile = g_Global.GetUserIniPath();
	QString str;

	// 修改报单前的确认 modify by xrs，因为有多个地方都做了相同的动作，所以采用公共类修改
	CModifySetting::ModifyTradeConfirm(ui.checkBox->isChecked());

	g_Global.m_bAlwaysShowBS5 = ui.checkBox_2->isChecked();
	m_bShowBS5 = g_Global.m_bAlwaysShowBS5;
	str = m_bShowBS5 ? "1" : "0";
	//WritePrivateProfileString("Order", "AlwayShowBS5", str, sfile);
	App::WritePriProfileString(sfile,"Order", "AlwayShowBS5", str);

	//kenny  2018-1-6   需要继续添加资源
	//CGDTradeForm2Dlg* pMainEx = (CGDTradeForm2Dlg*)AfxGetMainWndEx();
	//pMainEx->m_MainPanel.m_dlgUpPanel.ResetOrderConstraint();

	g_Global.m_bChangeBS = ui.checkBox_3->isChecked();
	m_bChangeBS = g_Global.m_bChangeBS;
	str = m_bChangeBS ? "1" : "0";
	//WritePrivateProfileString("Order", "ChangeBS", str, sfile);
	App::WritePriProfileString(sfile, "Order", "ChangeBS", str);

		
	// 平仓确认
	g_Global.m_bCovPosiConfirm = ui.checkBox_4->isChecked();
	m_bCovPosiConfirm = g_Global.m_bCovPosiConfirm;
	str = m_bCovPosiConfirm ? "1" : "0";
	//WritePrivateProfileString("Order", "CovPosiConfirm", str, sfile);
	App::WritePriProfileString(sfile, "Order", "CovPosiConfirm", str);

	// 市价平仓确认
	g_Global.m_bOppCovConfirm = ui.checkBox_5->isChecked();
	m_bOppCovConfirm = g_Global.m_bOppCovConfirm;
	str = m_bOppCovConfirm ? "1" : "0";
	//WritePrivateProfileString("Order", "OppCovConfirm", str, sfile);
	App::WritePriProfileString(sfile, "Order", "OppCovConfirm", str);


	// 预埋单确认
	g_Global.m_bPreOrderConfirm = ui.checkBox_8->isChecked();
	m_bPreOrderConfirm = g_Global.m_bPreOrderConfirm;

	str = m_bPreOrderConfirm ? "1" : "0";
	//WritePrivateProfileString("Order", "PreOrderConfirm", str, sfile);
	App::WritePriProfileString(sfile, "Order", "PreOrderConfirm", str);

	// 报单持仓或者库存溢出提示
	g_Global.m_bPosiOFConfirm = ui.checkBox_9->isChecked();
	m_bAskOFCfm = g_Global.m_bPosiOFConfirm;

	str = m_bAskOFCfm ? "1" : "0";
	//WritePrivateProfileString("Order", "AskOFCfm", str, sfile);
	App::WritePriProfileString(sfile, "Order", "AskOFCfm", str);

	// 报单持仓或者库存溢出提示
	g_Global.m_bTipsAddPreOrder = ui.checkBox_10->isChecked();
	m_bAskPreOrder = g_Global.m_bTipsAddPreOrder;

	str = m_bAskPreOrder ? "1" : "0";
	//WritePrivateProfileString("Order", "CreatePreOrderTip", str, sfile);
	App::WritePriProfileString(sfile, "Order", "CreatePreOrderTip", str);

	// 显示延期方式行
	g_Global.m_bShowDeferMode = ui.checkBox_7->isChecked();
	m_bShowDeferMode = g_Global.m_bShowDeferMode;
	str = m_bShowDeferMode ? "1" : "0";
	//WritePrivateProfileString("Order", "ShowDeferMode", str, sfile);
	App::WritePriProfileString(sfile, "Order", "ShowDeferMode", str);
	
	// 更新报单界面
	int hTradeDlg = g_TraderCpMgr.GetDlgHandleFromID(E_DlgOrder);
	if (hTradeDlg)
	{
		//::SendMessage(hTradeDlg, WM_CHANGE_DEFER_SHOW_MODE, 0, 0);
	}

	g_Global.m_bInsTriggerInfo = ui.checkBox_6->isChecked();
	m_checkChiCang = g_Global.m_bInsTriggerInfo;
	str = m_checkChiCang ? "1" : "0";
	App::WritePriProfileString(sfile, "Order", "ChiCang", str);

	g_Global.m_bChiCang_Click = (ui.radioButton->isChecked() == true) ? true : false;
	str.sprintf("%d", ui.radioButton->isChecked() == true ? 1: 0);
	//WritePrivateProfileString("Order", "ChiCnag_Click", str, sfile);
	App::WritePriProfileString(sfile, "Order", "ChiCnag_Click", str);

}


void frmOption_2::LoadFromIni()
{
	m_checkConfirmOrder = g_Global.m_bAskConfirm;
	ui.checkBox->setChecked(m_checkConfirmOrder);
	m_checkChiCang = g_Global.m_bInsTriggerInfo;
	ui.checkBox_6->setChecked(m_checkChiCang);


	m_bShowBS5 = g_Global.m_bAlwaysShowBS5;
	ui.checkBox_2->setChecked(m_bShowBS5);

	m_bChangeBS = g_Global.m_bChangeBS;
	ui.checkBox_3->setChecked(m_bChangeBS);

	m_bOppCovConfirm = g_Global.m_bOppCovConfirm;
	ui.checkBox_5->setChecked(m_bOppCovConfirm);

	m_bShowDeferMode = g_Global.m_bShowDeferMode;
	ui.checkBox_7->setChecked(m_bShowDeferMode);

	m_bCovPosiConfirm = g_Global.m_bCovPosiConfirm;
	ui.checkBox_4->setChecked(m_bCovPosiConfirm);

	m_bPreOrderConfirm = g_Global.m_bPreOrderConfirm;
	ui.checkBox_8->setChecked(m_bPreOrderConfirm);

	m_bAskOFCfm = g_Global.m_bPosiOFConfirm;
	ui.checkBox_9->setChecked(m_bAskOFCfm);

	m_bAskPreOrder = g_Global.m_bTipsAddPreOrder;
	ui.checkBox_10->setChecked(m_bAskPreOrder);

	m_radioChiCang = g_Global.m_bChiCang_Click ? 0 : 1;
	if (m_radioChiCang == 0)
		ui.radioButton->setChecked(true);
	else
		ui.radioButton_2->setChecked(true);

}

