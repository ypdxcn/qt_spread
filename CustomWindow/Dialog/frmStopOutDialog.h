#pragma once

#include <QDialog>
#include "ui_frmStopOutDialog.h"
#include <QStandardItemModel>
#include  "frmFramelessDialogBase.h"

class frmStopOutDialog : public frmFramelessDialogBase 
{
	Q_OBJECT

public:
	frmStopOutDialog(QWidget *parent = Q_NULLPTR);
	~frmStopOutDialog();



	QString GetProdCode();

public slots:

	void OnBnClickedButtonQuery();



private:
	Ui::frmStopOutDialog ui;

	QStandardItemModel* tableModel;
};
