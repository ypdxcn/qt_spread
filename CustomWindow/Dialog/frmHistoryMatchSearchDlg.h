#pragma once

#include <QDialog>
#include "ui_frmHistoryMatchSearchDlg.h"
#include "HJQueryBase.h"
#include  "frmFramelessDialogBase.h"

class frmHistoryMatchSearchDlg : public frmFramelessDialogBase,public CHJQueryBase
{
	Q_OBJECT

public:
	frmHistoryMatchSearchDlg(QWidget *parent = Q_NULLPTR);
	~frmHistoryMatchSearchDlg();

protected:
	virtual string GetQueryID();
	virtual void GetQueryPara(QVector< pair<QString, QString> >  &vecPara);
	virtual void customEvent(QEvent *event);
public slots:
     void comboBoxNumChanged(QString);
	 void comboBoxPageChanged(QString str);
	 void OnCbnSelchangeComboProdCode(QString str);
	 void OnBnClickedButtonQuery();
	 void OnBnClickedButtonFirpage();
	 void OnBnClickedButtonPrepage();
	 void OnBnClickedButtonNexpage();
	 void OnBnClickedButtonLaspage();
	 void OnBnClickedOutTExcel();
	 void OnBnClickedOption();
private:
	Ui::frmHistoryMatchSearchDlg ui;
};
