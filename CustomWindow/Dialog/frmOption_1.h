#pragma once

#include <QWidget>
#include "ui_frmOption_1.h"
#include "frmOption_base.h"

class frmOption_1 : public QWidget,public frmOption_base
{
	Q_OBJECT

public:
	frmOption_1(QWidget *parent = Q_NULLPTR);
	~frmOption_1();

	virtual void saveIni() override;
	void LoadFromIni();

	bool m_bCommitLeftOld; // 下单面板的旧位置
	int m_iInterfaceStyleOld; // 旧的界面风格


	int m_radioOrderPosition;
	QString m_strScreenTimeout;
	
	
	QWidget *m_mainwindow;
	void setMainWin(QWidget *win);
	bool CheckInput();

private:
	Ui::frmOption_1 ui;


};
