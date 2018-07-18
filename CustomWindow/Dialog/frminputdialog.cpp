#include "frminputdialog.h"
#include "ui_frminputdialog.h"
#include "myhelper.h"


frmInputDialog::frmInputDialog(QWidget *parent) :
	StyleSheetDialog(parent),
    ui(new Ui::frmInputDialog)
{
    ui->setupUi(this);

	this->InitStyle();
	myHelper::FormInCenter(this);
}

frmInputDialog::~frmInputDialog()
{
    delete ui;
}

void frmInputDialog::InitStyle()
{
	

	this->setProperty("Form", true);
	this->setProperty("CanMove", true);
	//this->setWindowTitle(ui->lab_Title->text());
	////设置窗体标题栏隐藏
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	////设置图形字体
	//IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015));
	//IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d));
	////关联关闭按钮
	//connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
	//connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void frmInputDialog::SetMessage(QString title)
{
	//ui->labInfo->setText(title);
}

void frmInputDialog::on_btnOk_clicked()
{
	/*value = ui->txtValue->text();
	done(1);
	this->close();*/
}
