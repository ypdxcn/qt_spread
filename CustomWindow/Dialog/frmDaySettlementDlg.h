#pragma once

#include <QDialog>
#include "ui_frmDaySettlementDlg.h"
#include "frmReportDlg.h"
#include "frmFramelessDialogBase.h"

class frmDaySettlementDlg : public frmFramelessDialogBase

{
	Q_OBJECT

public:
	frmDaySettlementDlg(QWidget *parent = Q_NULLPTR);
	~frmDaySettlementDlg();


public slots:
	void OnBnClickedButtonQuery();

private:
	Ui::frmDaySettlementDlg ui;

	bool   m_bAuto;

	// 报表相关
	frmReportDlg  *pDlgReport;
};
