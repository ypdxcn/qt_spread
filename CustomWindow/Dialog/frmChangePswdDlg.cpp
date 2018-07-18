#include "frmChangePswdDlg.h"
#include "HJGlobalFun.h"
#include "tradepacketandstruct.h"
#include "Global.h"
#include "TraderCpMgr.h"
#include <QMessageBox>

frmChangePswdDlg::frmChangePswdDlg(QWidget *parent)
	//: StyleSheetDialog(parent)
	:frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	m_bLogin = true;
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonCommit()));
	ui.lineEdit_new->setEchoMode(QLineEdit::Password);
	ui.lineEdit_old->setEchoMode(QLineEdit::Password);
	ui.lineEdit_confirm->setEchoMode(QLineEdit::Password);

	//this->setWindowTitle(QStringLiteral("登录密码修改"));
	
	//setCustomLayout();
	//QVBoxLayout* m_layout = new QVBoxLayout();//;
	//m_layout->setMargin(0);
	//m_layout->addLayout(ui.horizontalLayout);
	//m_layout->addLayout(ui.horizontalLayout_2);
	//m_layout->addLayout(ui.horizontalLayout_3);
	//m_layout->addLayout(ui.horizontalLayout_7);
	//加入关闭按钮
	//setLayout(m_layout);
	//setDialogSize(325,312);

	//ui.widget->setFixedSize(220,220);//200,200
	setFixedSize(250, 220);
	ui.widget->setGeometry(0, 0, 250, 180);//170小于自身dialog的大小200

	setContent(ui.widget);


	setMode(m_bLogin);

}

frmChangePswdDlg::~frmChangePswdDlg()
{
}


bool frmChangePswdDlg::CheckInput()
{
	//UpdateData(true);

	//#ifdef _VERSION_GF
	//	// 金联通版本密码（登录和资金密码）只能为6位数字
	//	if (m_strOld.GetLength() != 6 || m_strNew.GetLength() != 6 || m_strConfirm.GetLength() != 6)
	//	{
	//		AfxMessageBox("密码只能为6位的数字");
	//		return false;
	//	}
	//#else
	if (ui.lineEdit_old->text().length() < 6 || ui.lineEdit_new->text().length() < 6 || ui.lineEdit_confirm->text().length() < 6)
	{
		//AfxMessageBox("密码不能小于6位");
		QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral("密码不能小于！"));
		return false;
	}
	//#endif

	if (ui.lineEdit_new->text() != ui.lineEdit_confirm->text())
	{
		//AfxMessageBox("两次输入的密码不一致！");
		QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral("两次输入的密码不一致！"));
		return false;
	}

	// 旧密码与新密码不能相同
	if (ui.lineEdit_old->text() == ui.lineEdit_new->text())
	{
		//AfxMessageBox("新密码不能和旧的密码相同！");
		QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral("新密码不能和旧的密码相同！"));
		return false;
	}

	// 如果是数字的话，不能为连续的数字
	if (CHJGlobalFun::IsNumber(ui.lineEdit_new->text()))
	{
		if (CHJGlobalFun::IsContinuousNumber(ui.lineEdit_new->text()))
		{
			//AfxMessageBox("密码不能为连续的数字！");
				QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral("密码不能为连续的数字！"));	
				return false;
		}

		if (CHJGlobalFun::IsHasContinuousSameWord(ui.lineEdit_new->text(), 3))
		{
			//AfxMessageBox("密码不能有连续三位相同的数字！");
			QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral("密码不能有连续三位相同的数字！"));
			return false;
		}
	}

	return true;
}

void frmChangePswdDlg::setMode(bool lgmode)
{
	m_bLogin = lgmode;

	if (!m_bLogin)
	{
		setWindowTitle(QStringLiteral("资金密码修改"));
		//setCustomTitle(QStringLiteral("资金密码修改"));
	}
	else
	{
		setWindowTitle(QStringLiteral("登录密码修改"));
		//setCustomTitle(QStringLiteral("登录密码修改"));
	}
}


void frmChangePswdDlg::OnBnClickedButtonCommit()
{
	//DestroyKeyBoard();

	if (CheckInput())
	{
		Req2022 req;
		req.oper_flag = 1;
		req.acct_no = CHJGlobalFun::qstr2str( g_Global.m_strUserID);

		string strOldPSW, strNewPSW;

		if (m_bLogin)
		{
			QByteArray ba = ui.lineEdit_old->text().toLatin1();
			strOldPSW = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetEncryptPSW(ba.data()));
			ba = ui.lineEdit_new->text().toLatin1();
			strNewPSW = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetEncryptPSW(ba.data())); //MD5::ToMD5(m_strNew, strlen(m_strNew)); // MD5加密
		}
		else
		{
			bool bShortPsw = false;//短MD5 true   ,长MD5 FALSE
			QByteArray ba = ui.lineEdit_old->text().toLatin1();
			strOldPSW = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetEncryptPSW(ba.data(), bShortPsw));
			ba = ui.lineEdit_new->text().toLatin1();
			strNewPSW = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetEncryptPSW(ba.data(), bShortPsw)); //MD5::ToMD5(m_strNew, strlen(m_strNew)); // MD5加密
		}

		if (m_bLogin) // 如果是修改登陆密码
		{
			req.old_exch_pwd = strOldPSW;
			req.exch_pwd = strNewPSW;
		}
		else // 如果是修改资金密码
		{
			req.old_fund_pwd = strOldPSW;
			req.fund_pwd = strNewPSW;
		}

		Rsp2022 rsp;
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "2022"))
		{
			//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
			QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral("CONSTANT_CONNECT_SERVER_FAIL_TIPS"));
			return;
		}

		//报文检测
		if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
		{
			//AfxMessageBox("修改密码成功！");
			QMessageBox::information(nullptr, QStringLiteral("系统提示"), QStringLiteral("修改密码成功！"));

			ui.lineEdit_old->clear();
			ui.lineEdit_new->clear();
			ui.lineEdit_confirm->clear();


			close();
		}
	}
}
