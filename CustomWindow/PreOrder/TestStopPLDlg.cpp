
// TestStopPLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GDTradeForm.h"
#include "TestStopPLDlg.h"

#include "StopPLMgr.h"
#include "TraderCpMgr.h"
#include "HJGlobalFun.h"
#include "HJConstant.h"
#include "IniFilesManager.h"
#include "Global.h"
#include "CustomInfoMgrEx.h"

IMPLEMENT_DYNAMIC(CTestStopPLDlg, CDialog)

CTestStopPLDlg::CTestStopPLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestStopPLDlg::IDD, pParent)
	, m_csHand(_T(""))
	, m_csPrice(_T(""))
	, m_csProdCode(_T(""))
	, m_uiLast(0)
	, m_uiBuy1(0)
	, m_uiSell1(0)
	, m_csOrderNum(_T(""))
	, m_csExchID(_T(""))
	, m_csMatchNo(_T(""))
{

}

CTestStopPLDlg::~CTestStopPLDlg()
{
}

void CTestStopPLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbLongShort);
	DDX_Control(pDX, IDC_COMBO_PRODCODE, m_cmbInst);
	DDX_Text(pDX, IDC_EDIT2, m_csHand);
	DDX_Text(pDX, IDC_EDIT4, m_csPrice);
	DDX_Text(pDX, IDC_EDIT5, m_uiLast);
	DDX_Text(pDX, IDC_EDIT6, m_uiBuy1);
	DDX_Text(pDX, IDC_EDIT7, m_uiSell1);
	DDX_Text(pDX, IDC_EDIT3, m_csOrderNum);
	DDX_Text(pDX, IDC_EDIT1, m_csExchID);
	DDX_Text(pDX, IDC_EDIT8, m_csMatchNo);
}


BEGIN_MESSAGE_MAP(CTestStopPLDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestStopPLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestStopPLDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODCODE, &CTestStopPLDlg::OnCbnSelchangeComboProdcode)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestStopPLDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

void CTestStopPLDlg::OnBnClickedButton1()
{
	//CString csValue;
	//static int i = 0;
	//csValue.Format("%d", i++);
 //   TipsType eType;
	//if( i%5 == 0 )
	//	eType = E_Tips2_Asksus;
	//else if( i%5 == 1 )
	//	eType = E_Tips2_AskFail;
	//else if( i%5 == 2 )
	//	eType = E_Tips2_OrderMatch;
	//else if( i%5 == 3 )
	//	eType = E_Tips2_CancelSus;
	//else
	//	eType = E_Tips2_CancelFail;

	//TipsOrderInfo TipsInfo;
	//TipsInfo.IniData(eType, "Au100g", "4011", "100.00", "3", "挂单成交", csValue);
	//g_TipsManager.m_vecTips.push_back(TipsInfo);
	//::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_SHOW_ORDER_TIPS, g_TipsManager.m_vecTips.size()-1, 0);
	//return;

	UpdateData(TRUE);

	DeferMatch body;
	CString csInsID;
	m_cmbInst.GetLBText( m_cmbInst.GetCurSel(), csInsID);
	body.instID = csInsID;
	body.volume = atoi(m_csHand);
	body.price = atof(m_csPrice);

	static int i = 0;
	if( m_csMatchNo.IsEmpty() )
	{
		CString csValue;
		csValue.Format("%d", i++);
		body.matchNo = CHJGlobalFun::CStringToString(csValue);
	}
	else
	{
		body.matchNo = CHJGlobalFun::CStringToString(m_csMatchNo);
	}
	

	bool bLong = m_cmbLongShort.GetCurSel();
	body.sExchTypeCode = bLong ? CONSTANT_EXCH_CODE_DEFER_OPEN_LONG : CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT;

	g_StopPLMgr.HandleOpenPosiMatch( body, bLong );
}

void CTestStopPLDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);

	CString csInsID;
	m_cmbInst.GetLBText( m_cmbInst.GetCurSel(), csInsID);
	string sInstID = CHJGlobalFun::CStringToString(csInsID);
	QUOTATION& qt = g_TraderCpMgr.m_mapQuotation[sInstID];
	if( m_uiLast != 0 )
		qt.m_uiLast = m_uiLast;
	if( m_uiBuy1 != 0 )
		qt.m_Bid[0].m_uiPrice = m_uiBuy1; // 买一价
	if( m_uiSell1 != 0 )
		qt.m_Ask[0].m_uiPrice = m_uiSell1; // 卖一价

	qt.m_uiLowLimit = 100.00;
	qt.m_uiHighLimit = 10000000.00;
	g_TraderCpMgr.m_mapBdr[E_ONRECVDEFERQUOTATION].Broadcast(MSG_RECV_QUOTATION, (WPARAM)&sInstID, 0, false);
	g_TraderCpMgr.m_cPreOrder.HandleRecQuotation( sInstID, qt.m_uiLast );

	// 计算持仓盈亏
	g_CusInfo.HandleRecvQuotation(qt);
}

BOOL CTestStopPLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CIniFilesManager mgr;
	vector<CString> vecUserNames;
	mgr.ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);
	for(int i = 0; i < vecUserNames.size(); i++)
		m_cmbInst.InsertString(i, vecUserNames.at(i));

	m_cmbInst.SetCurSel(0);
	OnCbnSelchangeComboProdcode();

	m_cmbLongShort.InsertString(0,"空");
	m_cmbLongShort.InsertString(1,"多"); 
	m_cmbLongShort.SetCurSel(1);

	m_csHand = "2";

	UpdateData(FALSE);

	return TRUE;
}

void CTestStopPLDlg::OnCbnSelchangeComboProdcode()
{
	CString csInsID;
	m_cmbInst.GetLBText( m_cmbInst.GetCurSel(), csInsID);
	string sInstID = CHJGlobalFun::CStringToString(csInsID);
	const QUOTATION& qt = g_TraderCpMgr.m_mapQuotation[sInstID];
	m_uiLast = qt.m_uiLast;
	m_uiBuy1 = qt.m_Bid[0].m_uiPrice;
	m_uiSell1 = qt.m_Ask[0].m_uiPrice;
	m_csPrice.Format("%.2f", qt.m_uiLast/g_Global.m_dQTFactor);
	UpdateData(FALSE);
}

void CTestStopPLDlg::OnBnClickedButton3()
{
	CString csInsID;
	m_cmbInst.GetLBText( m_cmbInst.GetCurSel(), csInsID);
	string sInstID = CHJGlobalFun::CStringToString(csInsID);

	//
	UpdateData(TRUE);
	g_TraderCpMgr.m_cPreOrder.HandleReceiveCovPosi(sInstID, CHJGlobalFun::CStringToString(m_csExchID), atoi(m_csOrderNum));
}
