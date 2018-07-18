#ifndef FRMINPUTDIALOG_H
#define FRMINPUTDIALOG_H

#include <QDialog>
#include  "StyleSheetDialog.h"

namespace Ui {
class frmInputDialog;
}

class frmInputDialog : public StyleSheetDialog
{
    Q_OBJECT

public:
    explicit frmInputDialog(QWidget *parent = 0);
    ~frmInputDialog();


	void SetMessage(QString title);
	QString GetValue()const {	return value;}

	private slots:
	void on_btnOk_clicked();

private:
    Ui::frmInputDialog *ui;


	void InitStyle();   //初始化无边框窗体
	QString value;
};

#endif // FRMINPUTDIALOG_H
