#pragma once

#include <QDialog>
#include "ui_frmOptionDlg.h"
#include "frmFramelessDialogBase.h"
#include <QStackedWidget>

class frmOptionDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmOptionDlg(QWidget *parent = Q_NULLPTR);
	~frmOptionDlg();


	void switchWidget( QWidget* newWin);

public slots:
	void showWidgetChanged(QTreeWidgetItem*  item,int index);

	void slotApply();
	
	
public:
	QWidget* m_mainwindow;

	//供外面调用

	bool selectTreeItem(const QString& str);
	//bool findTreeItem(const QString& str);

	void setMainWin(QWidget* win);
private:
	Ui::frmOptionDlg ui;
	QVBoxLayout  *contentLayout;
	//QVector<QWidget *>vecWidget;
	//QWidget* curWidget;
	QVector<QWidget *>vecWidget;
	QWidget* curWidget;


	int mindex;


};
