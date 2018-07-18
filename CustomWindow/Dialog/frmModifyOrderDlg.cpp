#include "frmModifyOrderDlg.h"
#include "TraderCpMgr.h"
#include <QThread>
#include <QMessageBox>

frmModifyOrderDlg::frmModifyOrderDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);


	setFixedSize(261, 251);
	ui.widget->setGeometry(0, 0, 261, 151);
	setContent(ui.widget);

	setWindowTitle(QStringLiteral("修改报单"));


	connect(ui.pushButton_ok,SIGNAL(clicked()),this,SLOT(slotOk()));
}

frmModifyOrderDlg::~frmModifyOrderDlg()
{
}


void frmModifyOrderDlg::setVal(QString  &price, QString &num)
{
	ui.lineEdit_price->setText(price);
	ui.lineEdit_num->setText(num);
}



void frmModifyOrderDlg::IniData(const QString &csOldOrderNo, const QString &sExchTypeID, const QString &csInsID,
	const QString &csPrice, const QString &csAmount)
{
	m_csOldOrderNo = csOldOrderNo;
	m_sExchTypeID = sExchTypeID;
	m_csShouShu = csAmount;
	m_sInsID = csInsID;
	m_csPrice = csPrice;
}

void frmModifyOrderDlg::slotOk()
{
	if (g_TraderCpMgr.CancelOrder(m_csOldOrderNo) == 0)
	{
		// 延迟半秒发送，解决服务器未来得及更新撤单的信息导致改单失败的问题
		QThread::sleep(1);
		int nCommiOrderRsp = g_TraderCpMgr.CommitOrder(m_sInsID, ui.lineEdit_price->text(), ui.lineEdit_num->text(), m_sExchTypeID,0);
		if (nCommiOrderRsp != 0)
		{

			QMessageBox::information(nullptr, "系统提示", ("改单失败！原报单已经被撤销，如需恢复，请重新在报单窗口下单！"));

		}
		else
		{

		}
	}
	else
	{

		QMessageBox::information(nullptr, "系统提示", ("由于撤单不成功，所以无法改单！"));
	}

	close();
}