#pragma once

#include <QDialog>
#include "ui_frmModifyOrderDlg.h"
#include  "frmFramelessDialogBase.h"
class frmModifyOrderDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmModifyOrderDlg(QWidget *parent = Q_NULLPTR);
	~frmModifyOrderDlg();

	void setVal(QString  &price,QString &num);

	void IniData(const QString &csOldOrderNo, const QString &sExchTypeID, const QString &csInsID,\
		const QString &csPrice, const QString &csAmount);


public slots:
	void slotOk();

private:
	Ui::frmModifyOrderDlg ui;


	QString     m_sInsID;
	QString     m_sExchTypeID;
	QString    m_csOldOrderNo;
	QString    m_csShouShu;
	QString    m_csPrice;


};
