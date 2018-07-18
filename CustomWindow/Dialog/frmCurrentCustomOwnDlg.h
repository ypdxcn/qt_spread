#pragma once

#include <QDialog>
#include "ui_frmCurrentCustomOwnDlg.h"
#include "frmFramelessDialogBase.h"

class frmCurrentCustomOwnDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmCurrentCustomOwnDlg(QWidget *parent = Q_NULLPTR);
	~frmCurrentCustomOwnDlg();

private:
	Ui::frmCurrentCustomOwnDlg ui;
};
