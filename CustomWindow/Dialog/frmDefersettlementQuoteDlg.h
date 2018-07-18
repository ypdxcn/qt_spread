#pragma once

#include <QDialog>
#include <QStandardItemModel>
#include "BroadcastPacketAndStruct.h"
#include "frmFramelessDialogBase.h"

namespace Ui { class frmDefersettlementQuoteDlg; };

class frmDefersettlementQuoteDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmDefersettlementQuoteDlg(QWidget *parent = Q_NULLPTR);
	~frmDefersettlementQuoteDlg();

	
	void   OnDeferDeliveryQuotation(int wParam);
	void   SetListOneLineData(const DeferDeliveryQuotation &stQuotation, int iLine, bool bAdd );
public slots:
	void slotCal();

	

protected:
	virtual void  customEvent(QEvent *e);
private:
	Ui::frmDefersettlementQuoteDlg *ui;

	QStandardItemModel *	tableModel;
};
