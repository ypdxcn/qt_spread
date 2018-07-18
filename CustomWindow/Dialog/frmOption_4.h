#pragma once

#include <QWidget>
#include "ui_frmOption_4.h"
#include "frmOption_base.h"

class frmOption_4 : public QWidget, public frmOption_base
{
	Q_OBJECT

public:
	frmOption_4(QWidget *parent = Q_NULLPTR);
	~frmOption_4();

	void saveIni();
	void LoadFromIni();
	void WriteSoundSetting(QString &csOldFilePath, const QComboBox &cmb, const QString &csTitle, const QString &csPath);
	void IniComboBox(QComboBox &cmb, const QString &csPath);
	void OnCbnSelchange(QComboBox &cmb);
public slots:
	void slotChange1(QString);
	void slotChange2(QString);
	void slotChange3(QString);
	void slotChange4(QString);
	void slotChange5(QString);

private:
	Ui::frmOption_4 ui;


	bool m_bTipsAskSus;
	bool m_bTipsAskFail;
	bool m_bTipsOrderMatch;
	bool m_bTipsCancelSus;
	bool m_bTipsCancelFail;

};
