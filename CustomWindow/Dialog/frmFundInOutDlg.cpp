#include "frmFundInOutDlg.h"
#include "Macrodefs.h"
#include "Global.h"
#include "TradeHandler.h"
#include "DelayReqHandler.h"
#include "HJGlobalFun.h"
#include "TraderCpMgr.h"
#include "CustomInfoMgrEx.h"
#include <QMessageBox>
#include <QGridLayout>

#include "TradeHandler.h"
#include "app.h"

frmFundInOutDlg::frmFundInOutDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	
	tableModel = new QStandardItemModel();

	//setWidgetStyleFromQssFile(ui.tableView, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

	////设置行
	tableModel->setColumnCount(7);
	int columnIndex = 0;
	tableModel->setHeaderData(columnIndex, Qt::Horizontal, QStringLiteral("交易流水号"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("银行账号"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("银行名称"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("存取方向"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("发生金额"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("复核状态"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("备注"));


	ui.tableView->horizontalHeader()->setFixedHeight(32);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->verticalHeader()->setDefaultSectionSize(30);
	ui.tableView->setShowGrid(false);
	ui.tableView->setSortingEnabled(false);
	ui.tableView->setAlternatingRowColors(true);//隔行换色
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableModel->setSortRole(1);
	ui.tableView->setModel(tableModel);
	ui.tableView->setColumnWidth(0, 150);
	ui.tableView->setColumnWidth(1, 150);



//	//IniAccessButton();
//
//	//if (m_bPopup)
//	//{
//	//	SetWindowText("出入金");
//	//}
//
//#if (defined _VERSION_JSZX) || (defined _VERSION_ZHLHY)
//	// 如果是自动出入金，则检查出入金的是否是否需要发送卡密码
//	if (g_Global.m_bAutoTransfer)
//	{
//		GetCheckCardPwdPara();
//	}
//#endif


	QString csResPath = g_Global.GetSystemPath() + "Trade\\res\\";
	
	//刷新流水
	connect(ui.pushButton_5, &QPushButton::clicked, this, &frmFundInOutDlg::slotCal);

	connect(ui.pushButton, &QPushButton::clicked, this, &frmFundInOutDlg::slotIn);//入金
	connect(ui.pushButton_2, &QPushButton::clicked, this, &frmFundInOutDlg::slotOut);//出金
	connect(ui.pushButton_3, &QPushButton::clicked, this, &frmFundInOutDlg::slotCapital);//资金
	connect(ui.pushButton_4, &QPushButton::clicked, this, &frmFundInOutDlg::slotCustomInfo);//客户信息

    ui.lineEdit_pswd->setEchoMode(QLineEdit::Password);
	ui.label_3->setVisible(false);

	setFixedSize(800, 601);
	ui.widget->setGeometry(0, 0, 800, 501);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("出入金"));

}

frmFundInOutDlg::~frmFundInOutDlg()
{
}


//刷新流水
void frmFundInOutDlg::slotCal()
{
	//
	g_DelayReqHandler.RequestFundInfo();
	//m_list1.DeleteAllItems();
	tableModel->clear();

	Rsp3010 rsp3010;
	Req3010 req3010;
	if (g_TradeHandler.GetCushAccessFlow(rsp3010, req3010, e_TipsShowType_Fail, false) == 0)
	{
		const ArrayListMsg &alm = rsp3010.alm_custtype_list;
		for (int i = 0; i < alm.size(); i++)
		{
			tableModel->insertRow(i);
			tableModel->setItem(i, 0, new QStandardItem(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 0))));// 交易流水号 
			//m_list1.SetItemText(i, 1, alm.GetStringEx(i, 6).c_str());   
			tableModel->setItem(i, 1, new QStandardItem(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 6)))); // 银行账号
			
			//m_list1.SetItemText(i, 2, alm.GetStringEx(i, 7).c_str());    
			tableModel->setItem(i, 2, new QStandardItem(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 7))));// 银行名称
			
			//m_list1.SetItemText(i, 3, alm.GetStringEx(i, 4).c_str());    // 存取方向
			tableModel->setItem(i, 3, new QStandardItem(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 4))));

			//m_list1.SetItemText(i, 4, CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 5)))); // 发生金额
			tableModel->setItem(i, 4, new QStandardItem(CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 5)))));

			//m_list1.SetItemText(i, 5, g_TraderCpMgr.GetCheck_staName(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 8)))); // 复核状态 
			tableModel->setItem(i, 5, new QStandardItem(g_TraderCpMgr.GetCheck_staName(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 8)))));

			//m_list1.SetItemText(i, 6, alm.GetStringEx(i, 13).c_str()); 	 
			tableModel->setItem(i, 6, new QStandardItem(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 13))));// 备注

		}
	}

}

bool frmFundInOutDlg::HandleTransferByHand(const QString &accessWay)
{
	Req3010 req3010;
	//操作标识
	req3010.oper_flag = 1;
	//存取方式
	req3010.access_way = CHJGlobalFun::qstr2str(accessWay);
	//发生金额
	req3010.exch_bal = ui.lineEdit->text().toFloat();
	//资金密码
	QByteArray ba = ui.lineEdit_pswd->text().toLatin1();
	req3010.fund_pwd = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetEncryptPSW(ba.data(), false));//true
	Rsp1020 rsp1020 = g_CusInfo.Get1020();
	req3010.account_no = rsp1020.c_account_no;
	req3010.open_bank_name = rsp1020.c_open_bank_name;
	req3010.remark = "";
	//柜员编号
	req3010.o_teller_id = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
	//申请代理机构
	req3010.branch_id = g_HeaderReq.branch_id;
	//申请客户
	req3010.acct_no = CHJGlobalFun::qstr2str(g_Global.m_strUserID);

	Rsp3010 rsp3010;
	if (0 == g_TraderCpMgr.HandlePacket(rsp3010, req3010, "3010"))
	{
		if (strcmp(rsp3010.rsp_code.c_str(), RSP_SUCCESS_CODE) == 0)
		{
			return true;
		}
		else
		{
			//AfxMessageBox(rsp3010.rsp_msg.c_str());
			//QMessageBox::Information(CHJGlobalFun::str2qstr(rsp3010.rsp_msg));
			if (strcmp(rsp3010.rsp_code.c_str(), "HJ2002  ") == 0)
				rsp3010.rsp_msg = "资金密码不正确！";

			QMessageBox::information(nullptr, QStringLiteral("系统提示"), CHJGlobalFun::str2qstr(rsp3010.rsp_msg));
		}
	}
	else
	{
		//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS, MB_TOPMOST);
		QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS));
	}

	return false;
}


//检查界面输入是否正确，则错误的控件设置焦点
bool frmFundInOutDlg::CheckInput()
{
	//发生金额
	if ( ui.lineEdit->text() == QStringLiteral(""))
	{

		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("发生金额不能为空"), QMessageBox::Ok);

		ui.lineEdit->setFocus();
		return false;
	}
	//if (0)
	//	//if(!IsNumber(m_editbal))
	//{

	//	QMessageBox::information(nullptr, "提示", QStringLiteral("发生金额输入有误"), QMessageBox::Ok);
	//	GetDlgItem(IDC_EDIT_BALANCE)->SetFocus();
	//	return false;
	//}

	//资金密码
	if (ui.lineEdit_pswd->text() == QStringLiteral(""))
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("资金密码不能为空"), QMessageBox::Ok);
		ui.lineEdit_pswd->setFocus();
		return false;
	}

	return true;
}

//确定（转账）按钮的触发函数
void frmFundInOutDlg::OnBnClickedButtonTransfer(QString accessWay)
{
	
	//g_SysParaMgr.HideKeyBoard();
	if (CheckInput())
	{
		// 获取金额窗口数据
		QString notifyMsg;
		notifyMsg = QStringLiteral("    转账金额为：");
		notifyMsg += ui.lineEdit->text();//.GetBuffer();
		//ToDaXie();
		notifyMsg += QStringLiteral(", 即 ");
		notifyMsg += ui.label_daxie->text();
		notifyMsg += QStringLiteral(", 确实要转账吗?");

		if(QMessageBox::information(nullptr, QStringLiteral("提示"), notifyMsg, QMessageBox::Yes | QMessageBox::No)  == QMessageBox::Yes)
		//if (MessageBox(notifyMsg.GetBuffer(), "提示", MB_YESNO | MB_ICONINFORMATION) == IDYES) //弹出确认窗口 
		{
			bool bSus = false; // 是否请求数据成功
			bool bTips = true; // 是否需要提示
			//char *psw;
			//psw = m_editpsw.GetBuffer(m_editpsw.GetLength());


			bSus = HandleTransferByHand(accessWay);

			//m_editpsw.ReleaseBuffer();
			if (bSus)
			{
				if (bTips)
				{
					//AfxMessageBox("转账成功！");
					QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("转账成功！"), QMessageBox::Ok);
				}

				// 出入金成功后触发刷新按钮来[刷新资金信息]
				slotCal();

				ui.lineEdit->setText(QStringLiteral(""));
				ui.label_daxie->setText(QStringLiteral(""));
				ui.lineEdit_pswd->setText(QStringLiteral(""));
			
			
			}
		}

	}
}


void frmFundInOutDlg::slotIn()
{
	if (2 != g_TraderCpMgr.m_vAccessWay.size())
	{
		return;
	}
	else
	{
		OnBnClickedButtonTransfer(g_TraderCpMgr.m_vAccessWay.at(0).code_id);
	}
}
void frmFundInOutDlg::slotOut()
{
	if (2 != g_TraderCpMgr.m_vAccessWay.size())
	{
		return;
	}
	else
	{
		OnBnClickedButtonTransfer(g_TraderCpMgr.m_vAccessWay.at(1).code_id);
	}

}

//资金信息
void frmFundInOutDlg::slotCapital()
{
	QDialog* win          = new QDialog(this);//资金信息
	QLabel* label_cur     = new QLabel(this);//当前余额
	QLabel* label_can_get = new QLabel(this);//可提现金
	QLabel* label_can_use = new QLabel(this);//可用金额

	QLabel* label_cur_val = new QLabel(this);//当前余额
	QLabel* label_can_get_val = new QLabel(this);//可提现金
	QLabel* label_can_use_val = new QLabel(this);//可用金额

	//
	QGridLayout *pLayout = new QGridLayout;
	pLayout->addWidget(label_cur, 0, 0, 1, 1);
	pLayout->addWidget(label_cur_val, 0, 1, 1, 2);
	pLayout->addWidget(label_can_get, 1, 0, 1, 1);
	pLayout->addWidget(label_can_get_val, 1, 1, 1, 2);
	pLayout->addWidget(label_can_use, 2, 0, 1, 1);
	pLayout->addWidget(label_can_use_val, 2, 1, 1, 2);

	pLayout->setMargin(0);
	pLayout->setHorizontalSpacing(10);//设置水平间距
	pLayout->setVerticalSpacing(10);
	pLayout->setContentsMargins(10, 10, 10, 10);//设置垂直间距
	// 显示基本资金信息
	label_cur->setText(QStringLiteral("当前余额："));
	label_can_get->setText(QStringLiteral("可提余额："));
	label_can_use->setText(QStringLiteral("可用余额："));
	label_cur_val->setText(QStringLiteral(" "));
	label_can_get_val->setText(QStringLiteral(" "));
	label_can_use_val->setText(QStringLiteral(" "));
		
	Req1020 req;
	req.qry_fund = CHJGlobalFun::qstr2str(gc_YesNo_Yes);

	Rsp1020 rsp;
	if (g_TradeHandler.GetCustomInfo(rsp, req, e_TipsShowType_Fail) == 0)
	{
		Rsp1020 rsp1020 = g_CusInfo.Get1020();

		label_cur_val->setText(CHJGlobalFun::str2qstr(rsp1020.f_curr_bal));
		label_can_get_val->setText(CHJGlobalFun::str2qstr(rsp1020.f_can_get_bal));
		label_can_use_val->setText(CHJGlobalFun::str2qstr(rsp1020.f_can_use_bal));
	}

	win->setWindowTitle(QStringLiteral("资金信息"));
	win->setLayout(pLayout);
	win->resize(300, 200);
	win->setModal(true);
	win->show();

}

//客户信息
void frmFundInOutDlg::slotCustomInfo()
{
	QDialog* win = new QDialog(this);//资金信息
	QLabel* label_code   = new QLabel(this);//交易编码
	QLabel* label_acc    = new QLabel(this);//银行账户
	QLabel* label_name   = new QLabel(this);//客户名称
	QLabel* label_status = new QLabel(this);//账户状态
	QLabel* label_branch = new QLabel(this);//所属代理机构


	QLabel* label_code_val   = new QLabel(this);//交易编码
	QLabel* label_acc_val    = new QLabel(this);//银行账户
	QLabel* label_name_val   = new QLabel(this);//客户名称
	QLabel* label_status_val = new QLabel(this);//账户状态
	QLabel* label_branch_val = new QLabel(this);//所属代理机构
												 //
	QGridLayout *pLayout = new QGridLayout;
	pLayout->addWidget(label_code, 0, 0, 1, 1);
	pLayout->addWidget(label_code_val, 0, 1, 1, 2);
	pLayout->addWidget(label_acc, 1, 0, 1, 1);
	pLayout->addWidget(label_acc_val, 1, 1, 1, 2);
	pLayout->addWidget(label_name, 2, 0, 1, 1);
	pLayout->addWidget(label_name_val, 2, 1, 1, 2);
	pLayout->addWidget(label_status, 3, 0, 1, 1);
	pLayout->addWidget(label_status_val, 3, 1, 1, 2);
	pLayout->addWidget(label_branch, 4, 0, 1, 1);
	pLayout->addWidget(label_branch_val, 4, 1, 1, 2);

	// 显示基本资金信息
	label_code->setText(QStringLiteral("交易编码："));
	label_acc->setText(QStringLiteral("银行账户："));
	label_name->setText(QStringLiteral("客户名称："));
	label_status->setText(QStringLiteral("账户状态："));
	label_branch->setText(QStringLiteral("所属代理机构："));
	
	label_code_val->setText(QStringLiteral(""));
	label_acc_val->setText(QStringLiteral(""));
	label_name_val->setText(QStringLiteral(""));
	label_status_val->setText(QStringLiteral(""));
	label_branch_val->setText(QStringLiteral(""));


	Req1020 stReq;
	stReq.qry_cust_info = CHJGlobalFun::qstr2str(gc_YesNo_Yes);;

	Rsp1020 rsp;
	if (g_TradeHandler.GetCustomInfo(rsp, stReq, e_TipsShowType_Fail) == 0)
	{
		Rsp1020 rsp1020 = g_CusInfo.Get1020();
	
		label_code_val->setText(CHJGlobalFun::str2qstr(rsp1020.c_acct_no));
		label_acc_val->setText(CHJGlobalFun::str2qstr(rsp1020.c_account_no));
		label_name_val->setText(CHJGlobalFun::str2qstr(rsp1020.c_cust_abbr));
		label_status_val->setText(g_TraderCpMgr.GetAcct_statName(CHJGlobalFun::str2qstr(rsp1020.c_acct_stat)));
		label_branch_val->setText(CHJGlobalFun::str2qstr(GetBranchName(rsp1020.c_branch_id)));

	}
	

	win->setWindowTitle(QStringLiteral("客户信息"));
	win->setLayout(pLayout);
	win->resize(300, 200);
	win->setModal(true);
	win->show();

}




// 根据银行ID（sBranchID）在HashtableMsg里面获取机构名称（sBranchName）
bool frmFundInOutDlg::GetBranchNameFromID(HashtableMsg &htmBranch, const string &sBranchID, string &sBranchName)
{
	// 找到机构ID对应的机构名称
	for (int i = 0; i < htmBranch.size(); i++)
	{
		if (htmBranch.GetString(i, "branch_id") == sBranchID)
		{
			sBranchName = htmBranch.GetString(i, "branch_name");
			return true;
		}
	}

	return false;
}

/*
刷新信息时根据代理机构id获取代理机构名称
input：sBranchID代理机构id
*/
string frmFundInOutDlg::GetBranchName(const std::string &sBranchID)
{
	HashtableMsg htmBranch;
	string sBranchName;
	bool bReload = true;

	//数据配置文件路径
	QString strIniPath = g_Global.GetSystemPath() + CONSTANT_DATA_INI_NAME;

	// 如果配置文件中得代理机构信息不为空，则读取配置文件中的代理机构信息
	//char buf[10000] = { 0 };
	//::GetPrivateProfileString("Info", "Branch", "", buf, sizeof(buf), strIniPath);

	QString buf;
	App::GetPriProfileString(strIniPath, QStringLiteral("Info"), QStringLiteral(""), buf);

	if (buf.size() != 0)
	{

		// 如果解析成功而且能找到对应的名称，则不下载
		if (htmBranch.Parse(CHJGlobalFun::qstr2str(buf)) != -1)
		{
			if (GetBranchNameFromID(htmBranch, sBranchID, sBranchName))
			{
				bReload = false;
			}
		}
	}

	if (bReload)
	{
		Req9010 req;
		req.oper_flag = 1;

		Rsp9010 rsp;
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "9010"))
		{
			
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS), QMessageBox::Ok);
			sBranchName = sBranchID;
		}

		if (!g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
		{
			sBranchName = sBranchID;
		}
		else
		{
			htmBranch = rsp.htm_branch_info;
			//写入结构信息到文件
			//::WritePrivateProfileString("Info", "Branch", htmBranch.ToString().c_str(), strIniPath);

			App::GetPriProfileString(strIniPath, QStringLiteral("Info"), QStringLiteral("Branch"), CHJGlobalFun::str2qstr(htmBranch.ToString()));

			if (!GetBranchNameFromID(htmBranch, sBranchID, sBranchName))
			{
				sBranchName = sBranchID;
			}
		}
	}

	return sBranchName;
}

