#pragma once

#include <QDialog>
#include "ui_frmDeferPosiSearchDlg.h"
#include "HJQueryBase.h"
#include "frmFramelessDialogBase.h"

class frmDeferPosiSearchDlg : public frmFramelessDialogBase, public CHJQueryBase
{
	Q_OBJECT

public:
	frmDeferPosiSearchDlg(QWidget *parent = Q_NULLPTR);
	~frmDeferPosiSearchDlg();



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
	Ui::frmDeferPosiSearchDlg ui;
};
