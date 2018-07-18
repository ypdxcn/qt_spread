#include "frmQHLoginDlg.h"
#include "Macrodefs.h"
#include "TraderCpMgr.h"

frmQHLoginDlg::frmQHLoginDlg(QWidget *parent)
	: StyleSheetDialog(parent)
{
	ui.setupUi(this);

#ifdef  _WIN32
	setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\logindialog.css"));
#else
	//kenny  20180305  ?????????
	QString strPath = QCoreApplication::applicationDirPath();
	QDir  dir;
	dir.setCurrent(strPath);

	setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/logindialog.css"));
#endif

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	this->setProperty("Form", true);
	this->setProperty("CanMove", true);
	
	ui.lineEdit_2->setEchoMode(QLineEdit::Password);


	//ui.horizontalLayout_4->setGeometry(QRect(0, 0, 450, 395));// width(), height()));

	//this->setGeometry(QRect(0, 0, 471, 395));
	//setFixedWidth(471);
	//setFixedHeight(395);
	//resize(300, 200);
	connect(ui.pushButton_login, &QPushButton::clicked, [this]()
	{
		//登陆
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		//切换到期货下单版
		g_TraderCpMgr.LoginCTP(ui.lineEdit->text().toLatin1(), ui.lineEdit_2->text().toLatin1(), ui.lineEdit_2->text().toLatin1());

		QApplication::restoreOverrideCursor();

		this->accept();
	});

	connect(ui.pushButton_exit, &QPushButton::clicked, [this]()
	{
		//退出
		this->reject();
		this->close();
	});

    connect(ui.pushButton_close, &QPushButton::clicked, [this]()
	{
		//退出
		this->reject();
		this->close();
	});
}

frmQHLoginDlg::~frmQHLoginDlg()
{
}
