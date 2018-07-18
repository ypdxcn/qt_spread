#include "frmmessagedialog.h"
#include "ui_frmmessagedialog.h"

frmMessageDialog::frmMessageDialog(QWidget *parent) :
	frmFramelessDialogBase(parent),
    ui(new Ui::frmMessageDialog)
{
    ui->setupUi(this);


}

frmMessageDialog::~frmMessageDialog()
{
    delete ui;
}
