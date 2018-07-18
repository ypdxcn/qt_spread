#pragma once

#include <QDialog>
#include "frmFramelessDialogBase.H"
#include "ui_frmWTSearchDialog.h"
#include "HJQueryBase.h"

class frmWTSearchDialog : public frmFramelessDialogBase,public CHJQueryBase
{
	Q_OBJECT

public:
	frmWTSearchDialog(QWidget *parent = Q_NULLPTR);
	~frmWTSearchDialog();

protected:

	virtual void customEvent(QEvent *event);

	virtual string GetQueryID();
	// 获取查询的条件，在子类实现
	virtual void GetQueryPara(QVector< pair<QString, QString> >  &vecPara);
	
public slots:
	void OnBnClickedButtonQuery();
	//第一页按钮的触发函数
	void OnBnClickedButtonFirpage();
	//上一页按钮的触发函数
	void OnBnClickedButtonPrepage();
	//下一页按钮的触发函数
	void OnBnClickedButtonNexpage();
	//最后一页按钮的触发函数
	void OnBnClickedButtonLaspage();
	
	void OnBnClickedOutTExcel();
	void OnBnClickedOption();

	void comboBoxNumChanged(QString str);
	//查询按钮的处罚函数
	void comboBoxPageChanged(QString str);
	void OnCbnSelchangeComboProdCode(QString str);
private:
	Ui::frmWTSearchDialog ui;
};
