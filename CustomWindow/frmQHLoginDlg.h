#pragma once

#include <QDialog>
#include "ui_frmQHLoginDlg.h"
#include "StyleSheetDialog.h"

class frmQHLoginDlg : public StyleSheetDialog //: public QDialog
{
	Q_OBJECT

public:
	frmQHLoginDlg(QWidget *parent = Q_NULLPTR);
	~frmQHLoginDlg();

private:
	Ui::frmQHLoginDlg ui;
};
