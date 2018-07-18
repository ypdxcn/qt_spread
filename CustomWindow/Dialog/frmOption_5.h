#pragma once

#include <QWidget>
#include "ui_frmOption_5.h"
#include "frmOption_base.h"
#include <QStandardItemModel>
class frmOption_5 : public QWidget, public frmOption_base
{
	Q_OBJECT

public:
	frmOption_5(QWidget *parent = Q_NULLPTR);
	~frmOption_5();
	void saveIni();
	void LoadSetting();

	void WriteInstSetting();
	void WriteStopPLSetting(void);
	void WriteStopPLSettingLOG(int iPosCmp, int iPosAsk, const QString csAjust);

	bool IsInList(const QString &csInst);
public slots:
	void slotAdd();
	void slotModify();
	void slotDelete();

private:
	Ui::frmOption_5 ui;

	bool  m_bModified;
	bool  m_bUseServer;

	QStandardItemModel *	tableModel;
};
