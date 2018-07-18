#pragma once

#include <QDialog>
#include "ui_frmTipsDlg.h"
#include "Def.h"
#include "frmFramelessDialogBase.h"

class frmTipsDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmTipsDlg(QWidget *parent = Q_NULLPTR);
	~frmTipsDlg();


	void    Show(int iPos);
	QString DealTips(const QString &csTips, int &iLineCount);
	QString DealOneLine(const QString &csLine);

public slots:
	void OnBnClickedOk();
	void OnBnClickedButtonPre();
	void OnBnClickedButtonNext();

private:
	Ui::frmTipsDlg ui;

	int  m_iShowPos;// 当前对话框要显示的提示在记录中的位置
	QString m_csTips;   // 提示语的映射变量
	QString m_csTitle;  // 记录对话框的title，没用到，用于兼容
	bool    m_bCheck;      // 是否下次不提示的check控件的映射变量

	int m_iAddLine;     // 提示字符串增加的行数（默认只有一行的高度）

	TipsType m_eType;   // 提示的类型

};
