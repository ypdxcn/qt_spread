#include "frmNoticeDetailDlg.h"

frmNoticeDetailDlg::frmNoticeDetailDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	setFixedSize(751, 541);
	ui.widget->setGeometry(0, 0, 751, 441);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("公告详细行情"));

}

frmNoticeDetailDlg::~frmNoticeDetailDlg()
{
}

void frmNoticeDetailDlg::InitData(QString &strTitle, QString &strTime, QString &strContent)
{
	
	ui.label_title->setText(strTitle);
	ui.label_time->setText(strTime);
	ui.textEdit_content->setText(strContent);
}