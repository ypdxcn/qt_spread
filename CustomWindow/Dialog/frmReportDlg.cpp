#include "frmReportDlg.h"
#include "Global.h"
#include "HJGlobalFun.h"
#include <QScrollArea>
#include <QDesktopWidget>


#pragma execution_character_set("utf-8")

frmReportDlg::frmReportDlg(QWidget *parent)	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);
	//setGeometry(0, 0, 700, 600);
	setFixedSize(800, 1030);
	ui.widget_main->setGeometry(0, 0, 780, 621);//父窗口的content
	setContent(ui.widget_main);
	setWindowTitle(("结算提醒"));

	//widget上绘图
	ui.widget->setGeometry(0, 0, 801, 521);

	//居中显示
	QDesktopWidget *desk = QApplication::desktop();
	int wd = desk->width();
	int ht = desk->height();
	this->move((wd - width()) / 2, (ht - height()) / 2);
	
	//加入滚动条
	scrollArea = new  QScrollArea(ui.widget_main);
	scrollArea->setWidget(ui.widget);
	scrollArea->viewport()->setBackgroundRole(QPalette::Dark);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->show();

}

frmReportDlg::~frmReportDlg()
{
	
}

//打印
void frmReportDlg::slotPrint()
{
	ui.widget->PrintView(this);


}

// 初始化对话框
void frmReportDlg::ShowReportDlg(const bool &bMode, Rsp6007 &rsp6007, const string &strDate)
{
	ui.widget->ShowReportDlg(bMode, rsp6007, strDate);
	if(! ui.widget->bShowBtn)
		ui.pushButton_ok->hide();
	else
		ui.pushButton_ok->show();
	
	//ui.widget->IniAllData(rsp6007);
}



void frmReportDlg::printPreviewSlot(QPrinter* printerImage)
{
	printerImage->setOrientation(QPrinter::Landscape);
	//获取界面的图片  
	QPainter painterImage(this);
	painterImage.begin(printerImage);
	QRect rect = painterImage.viewport();
	int x = rect.width() / ui.widget->image->width();
	int y = rect.height() / ui.widget->image->height();
	painterImage.scale(x, y);
	painterImage.drawImage(0, 0, *ui.widget->image);
	painterImage.end();

}

//报表输出
void frmReportDlg::slotOutExcel()
{
	ui.widget->OutToExcel();
}



