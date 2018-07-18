#pragma once

#include <QDialog>
#include "ui_frmChangePswdDlg.h"
#include <QLineEdit>
#include "StyleSheetDialog.h"
#include "frmFramelessDialogBase.h"

class frmChangePswdDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmChangePswdDlg(QWidget *parent = Q_NULLPTR);
	~frmChangePswdDlg();

	bool CheckInput();

	void setMode(bool lgmode);
public slots:
	//void OnChange();
	void OnBnClickedButtonCommit();

private:
	Ui::frmChangePswdDlg ui;
	bool m_bLogin;
};
