#include "frmMonthSettlementDlg.h"
#include <QDateTime>
#include "global.h"
#include <QMessageBox>
#include "TradePacketAndStruct.h"
#include "TraderCpMgr.h"
#include "HJGlobalFun.h"


frmMonthSettlementDlg::frmMonthSettlementDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);


	m_bAuto = false;
	//m_bAuto = bAuto;

	// 设置开始和结束时间为上一交易日
	QDate ctime = QDate::fromString(g_Global.m_strLast_Exch_Date, ("yyyyMMdd"));
	if (!ctime.isNull())// != NULL)
	{
		ui.dateEdit->setDate(ctime);
		//m_dtctrl.SetTime(&ctime);
	}

	//SetWindowText("日结单查询");

	// 自动弹出日结单
	if (m_bAuto)
		OnBnClickedButtonQuery();


	connect(ui.pushButton, &QPushButton::clicked, this, &frmMonthSettlementDlg::OnBnClickedButtonQuery);

	setFixedSize(561, 300);
	ui.widget->setGeometry(0, 0, 561, 200);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("月结单查询"));
	
	//pDlgReport = nullptr;
}

frmMonthSettlementDlg::~frmMonthSettlementDlg()
{
	//if (pDlgReport != nullptr)
	//	delete pDlgReport;
}





//日结单查询按钮的触发函数
void frmMonthSettlementDlg::OnBnClickedButtonQuery()
{
	frmReportDlg  pDlgReport;
	//if (pDlgReport != nullptr )
	//{
	//	QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("报表窗口已经打开！"));
	//	//AfxMessageBox("报表窗口已经打开！");
	//	return;
	//}

	// 获取查询日期
	//string strDate;
	QDateTime tt;
	QString strDate = ui.dateEdit->dateTime().toString("yyyyMM");// GetTime(tt);
														 //strDate = CHJGlobalFun::CStringToString(tt.Format("%Y%m%d"));   //测试数据 strDate = "20130622";

	Req6005 req6005; //请求报文体
	req6005.oper_flag = 1;
	req6005.report_id = "RptAcctSettleCollectMonthTrader";

	Rsp6005 rsp6005; //应答报文体
	if (0 != g_TraderCpMgr.HandlePacket(rsp6005, req6005, "6005"))
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS));
		//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
		return;
	}

	Req6007 req6007; //请求报文体
	req6007.oper_flag = 1;
	req6007.alm_ds_list = rsp6005.alm_data_source;
	req6007.exch_date = CHJGlobalFun::qstr2str(strDate);

	Rsp6007 rsp6007; //应答报文体
	if (0 != g_TraderCpMgr.HandlePacket(rsp6007, req6007, "6007"))
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS));
		//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
		return;
	}

	if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp6007.rsp_code), CHJGlobalFun::str2qstr(rsp6007.rsp_msg)))
	{
		//if (pDlgReport == nullptr)
		//{
		//	pDlgReport = new frmReportDlg;
		//	//pDlgReport->Create(IDD_DLG_HJ_REPORTDLG, this);
		//	// 设置父窗口，用于发送消息
		//	//pDlgReport->SetParentEx(this);
		//}

		//pDlgReport->ShowReportDlg(true, rsp6007, strDate);
		pDlgReport.ShowReportDlg(false, rsp6007, req6007.exch_date);
		pDlgReport.exec();


	}
}

