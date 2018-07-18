#pragma once

#include <QDialog>
#include "ui_frmMonthSettlementDlg.h"
#include "frmReportDlg.h"
#include  "frmFramelessDialogBase.h"

class frmMonthSettlementDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmMonthSettlementDlg(QWidget *parent = Q_NULLPTR);
	~frmMonthSettlementDlg();
	

public slots:
	void OnBnClickedButtonQuery();

private:
	Ui::frmMonthSettlementDlg ui;

	bool   m_bAuto;


	// 报表相关
	//frmReportDlg  *pDlgReport;


};
