#pragma once

#include <QDialog>
#include "ui_frmStoreSearchDialog.h"
#include "HJQueryBase.h"
#include  "frmFramelessDialogBase.h"


class frmStoreSearchDialog : public frmFramelessDialogBase, public CHJQueryBase
{
	Q_OBJECT

public:
	frmStoreSearchDialog(QWidget *parent = Q_NULLPTR);
	~frmStoreSearchDialog();
protected:
	virtual string GetQueryID();
	virtual void GetQueryPara(QVector< pair<QString, QString> >  &vecPara);
	virtual void customEvent(QEvent *event);
public slots:
	void comboBoxNumChanged(QString);
	void comboBoxPageChanged(QString str);
	void OnBnClickedButtonQuery();
	void OnBnClickedButtonFirpage();
	void OnBnClickedButtonPrepage();
	void OnBnClickedButtonNexpage();
	void OnBnClickedButtonLaspage();
	void OnBnClickedOutTExcel();
	void OnBnClickedOption();
private:
	Ui::frmStoreSearchDialog ui;
};
