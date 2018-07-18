#pragma once

#include <QDialog>
#include "ui_frmDeepQuoteDlg.h"
#include "StyleSheetDialog.h"
class frmDeepQuoteDlg : public StyleSheetDialog
{
	Q_OBJECT

public:
	frmDeepQuoteDlg(QWidget *parent = Q_NULLPTR);
	~frmDeepQuoteDlg();


	void closeEvent(QCloseEvent *event);

signals:
	void setHide( );

private:
	Ui::frmDeepQuoteDlg ui;
};
