#pragma once

#include <QWidget>
#include "ui_frmOption_6.h"
#include "frmOption_base.h"
#include <QVBoxlayout>
#include <QVector>
#include <QCheckBox>
#include <QList>

class frmOption_6 : public QWidget, public frmOption_base
{
	Q_OBJECT

public:
	frmOption_6(QWidget *parent = Q_NULLPTR);
	~frmOption_6();


	void saveIni();
	void loadIni();

	void LoadInfo(const QString &csUserTitle);
	bool IsLoad(const QVector<int> &vecColumnIndexs, int iPos);
	void setFileName(const QString &csFileTitle);
	bool CompareData();
	void MoveUp(int iPos);
	virtual void SetMessageID(uint uiMes) override;

	QVector<QString> m_vShowFlag;
	QString m_strUserColumnIndexsOld; // 用户修改前的索引
	QString m_strUserColumnIndexsNew; // 用户修改后的索引
	QString m_strFileName;

	//本地保存
	QList<QCheckBox  *>list;

	//uint m_uiMessage;
public slots:
	void ProcessSelect(int row, int col);

	void slotUp();
	void slotDown();
	void slotAll();
	void slotUnAll();
	void slotDefault();
private:
	Ui::frmOption_6 ui;
};
