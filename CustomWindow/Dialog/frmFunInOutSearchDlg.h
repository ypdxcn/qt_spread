#pragma once

#include <QDialog>
#include "ui_frmFunInOutSearchDlg.h"
#include "HJQueryBase.h"
#include  "frmFramelessDialogBase.h"

class frmFunInOutSearchDlg : public frmFramelessDialogBase, public CHJQueryBase
{
	Q_OBJECT

public:
	frmFunInOutSearchDlg(QWidget *parent = Q_NULLPTR);
	~frmFunInOutSearchDlg();
protected:
	virtual string GetQueryID();
	virtual void GetQueryPara(QVector< pair<QString, QString> >  &vecPara);
	virtual void QueryReserver(const Req6002 &stReq6002, const Rsp6002 &stRsp6002);

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
	Ui::frmFunInOutSearchDlg ui;
};


