#pragma once

#include <QDialog>
#include "ui_frmCapitalSearchDialog.h"
#include "HJQueryBase.h"
#include "frmFramelessDialogBase.h"
class frmCapitalSearchDialog : public frmFramelessDialogBase, public CHJQueryBase
{
	Q_OBJECT

public:
	frmCapitalSearchDialog(QWidget *parent = Q_NULLPTR);
	~frmCapitalSearchDialog();
protected:
	virtual string GetQueryID();
	virtual void GetQueryPara(QVector< pair<QString, QString> >  &vecPara);
	virtual void customEvent(QEvent *event);

public slots:
	void comboBoxNumChanged(QString);
	void comboBoxPageChanged(QString str);
	//void OnCbnSelchangeComboProdCode(QString str);
	void OnBnClickedButtonQuery();
	void OnBnClickedButtonFirpage();
	void OnBnClickedButtonPrepage();
	void OnBnClickedButtonNexpage();
	void OnBnClickedButtonLaspage();
	void OnBnClickedOutTExcel();
	void OnBnClickedOption();
private:
	Ui::frmCapitalSearchDialog ui;
};
