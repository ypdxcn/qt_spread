#pragma once

#include <QDialog>
#include "ui_frmMemberNoticeDlg.h"
#include <QStandardItemModel>
#include  "frmFramelessDialogBase.h"
class frmMemberNoticeDlg : public frmFramelessDialogBase
{
	Q_OBJECT

public:
	frmMemberNoticeDlg(QWidget *parent = Q_NULLPTR);
	~frmMemberNoticeDlg();


	QString ConvertCode(QString strUTF8);


public slots:
	void   OnBnClickedButtonQuery();
	void   OnDBClicked(const QModelIndex &);
	void   OnBnClickedButtonDetail();



private:
	Ui::frmMemberNoticeDlg ui;


	QStandardItemModel*  tableModel;
};
