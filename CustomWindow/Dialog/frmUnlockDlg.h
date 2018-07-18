#pragma once

#include <QDialog>
#include "ui_frmUnlockDlg.h"
#include "StyleSheetDialog.h"
class CUnlockListener
{
public:
	virtual void OnSuccess() {}

	virtual void OnFailure() {}
};


class frmUnlockDlg : public StyleSheetDialog
{
	Q_OBJECT

public:
	frmUnlockDlg(QWidget *parent = Q_NULLPTR);
	~frmUnlockDlg();
	
	void Unlock();

	void SetListener(CUnlockListener* pListener);


	void changeEvent(QEvent *event);

public slots:

	void pushButton_ok();
	void pushButton_cancel();

private:
	Ui::frmUnlockDlg ui;

	CUnlockListener * mListener;
};
