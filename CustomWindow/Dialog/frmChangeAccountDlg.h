#pragma once

#include <QString>
#include <QDialog>
#include "ui_frmChangeAccountDlg.h"
#include "frmFramelessDialogBase.h"
#include <QAbstractItemView>
#include <QStandardItemModel>
//#include "mainWindow.h"
using namespace std;




class frmChangeAccountDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmChangeAccountDlg(QWidget *parent = Q_NULLPTR);
	~frmChangeAccountDlg();

	void init();

public slots:
	void slotOk();
	void slotCancel();
	void slotAdd();
	void slotDelete();
private:
	Ui::frmChangeAccountDlg ui;

	QStandardItemModel*   m_pmodel;
	//mainWindow * m_parent;
};
