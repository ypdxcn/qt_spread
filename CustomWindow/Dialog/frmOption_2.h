#pragma once

#include <QWidget>
#include "ui_frmOption_2.h"
#include "frmOption_base.h"
#include "frmOption_base.h"

class frmOption_2 : public QWidget, public frmOption_base
{
	Q_OBJECT

public:
	frmOption_2(QWidget *parent = Q_NULLPTR);
	~frmOption_2();

	void saveIni();
	void LoadFromIni();
private:
	Ui::frmOption_2 ui;


public:
	bool m_bShowBS5;

	bool m_bChangeBS;

	bool m_bOppCovConfirm;
	bool m_bShowDeferMode;
	bool m_bCovPosiConfirm;
	bool m_bPreOrderConfirm;
	bool m_bAskOFCfm;
	bool m_bAskPreOrder;


	bool m_checkConfirmOrder;
	bool m_checkChiCang;
	bool m_checkKuCun;

	int m_radioChiCang;
};
