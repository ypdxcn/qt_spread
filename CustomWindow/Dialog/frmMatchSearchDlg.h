#pragma once

#include <QDialog>
#include "ui_frmMatchSearchDlg.h"
#include "HJQueryBase.h"
#include  "frmFramelessDialogBase.h"
class frmMatchSearchDlg : public frmFramelessDialogBase,public CHJQueryBase
{
	Q_OBJECT

public:
	frmMatchSearchDlg(QWidget *parent = Q_NULLPTR);
	~frmMatchSearchDlg();


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
	Ui::frmMatchSearchDlg ui;
};
