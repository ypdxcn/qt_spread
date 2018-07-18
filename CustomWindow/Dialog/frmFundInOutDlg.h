#pragma once

#include <QDialog>
#include "ui_frmFundInOutDlg.h"
#include <QStandardItemModel>
#include "HashtableMsg.h"
#include "frmFramelessDialogBase.h"
class frmFundInOutDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmFundInOutDlg(QWidget *parent = Q_NULLPTR);
	~frmFundInOutDlg();

	bool CheckInput();
	bool HandleTransferByHand(const QString &accessWay);
	void OnBnClickedButtonTransfer(QString accessWay);


	bool GetBranchNameFromID(HashtableMsg &htmBranch, const string &sBranchID, string &sBranchName);
	string GetBranchName(const std::string &sBranchID);


public slots:
	void slotCal();

	void slotIn();
	void slotOut();
	void slotCapital();
	void slotCustomInfo();
private:
	Ui::frmFundInOutDlg ui;

	QStandardItemModel*   tableModel;
};
