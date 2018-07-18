#pragma once

#include <QDialog>
#include "ui_frmNoticeDetailDlg.h"
#include  "frmFramelessDialogBase.h"
class frmNoticeDetailDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmNoticeDetailDlg(QWidget *parent = Q_NULLPTR);
	~frmNoticeDetailDlg();

public:
	void InitData(QString &strTitle, QString &strTime, QString &strContent);
private:
	Ui::frmNoticeDetailDlg ui;
};
