#pragma once

#include <QDialog>
#include "ui_frmTradeStatusDlg.h"
#include <QStandardItemModel>
#include <QString>
#include "frmFramelessDialogBase.h"

class frmTradeStatusDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmTradeStatusDlg(QWidget *parent = Q_NULLPTR);
	~frmTradeStatusDlg();

	QString GetInsStateNameFromID(const QString &sInsID);

public slots:
	void slotCal();

	
private:
	Ui::frmTradeStatusDlg ui;
	QStandardItemModel *	tableModel;

	QStandardItemModel *	tableModel_2;
};
