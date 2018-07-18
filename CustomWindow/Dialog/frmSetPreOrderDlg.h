#pragma once

#include <QDialog>
#include "ui_frmSetPreOrderDlg.h"
#include "frmFramelessDialogBase.h"
#include "PreOrderDef.h"


class frmSetPreOrderDlg : public frmFramelessDialogBase//QDialog
{
	Q_OBJECT

public:
	frmSetPreOrderDlg(QWidget *parent = Q_NULLPTR);
	~frmSetPreOrderDlg();


	void initLayOut();
	bool GetConditionValue(EQTType &eQTType, ECompareType &eCmpType, double &dPrice);

	QString m_csAskPrice;
	QString m_sProdCode;
	QString m_sExchId;
	QString m_iHand;

	QString m_csAskInfo;


	int  spinType;
	int  radioindex = 0;
public  slots:
	void  slotReLayout();
	void  slotRadio0();
	void  slotRadio1();
	void  slotOk();

private:
	Ui::frmSetPreOrderDlg ui;
	QVBoxLayout   *layout;
	bool     m_bUseServer;
};
