#pragma once

#include <QWidget>
#include "ui_frmOption_3.h"
#include "frmOption_base.h"

class frmOption_3 : public QWidget, public frmOption_base
{
	Q_OBJECT

public:
	frmOption_3(QWidget *parent = Q_NULLPTR);
	~frmOption_3();

	void saveIni();
	void LoadFromIni();
private:
	Ui::frmOption_3 ui;
	bool  m_checkConfirmUnorder;
};
