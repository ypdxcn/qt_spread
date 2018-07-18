#include "mainWindow.h"
#include <QtWidgets/QApplication>
#include  "myhelper.h"
#include "frmLoginDialog.h"
#include "AppInt.h"
#include "Startup.h"
#include "Global.h"





//直接设置1字节对齐会报错，在需要对其的地方，协商次句
//#pragma pack(push, 1)  

//解决乱码，中文显示
#pragma execution_character_set("utf-8")

///创建dump，防止崩溃，会产生dump文件，便于分析
#include  "minidump.h"
LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	     char creashFile[100];
	     getcwd(creashFile, 100);
	    strcat(creashFile, "\\CreatFile.dmp");
	    CreateMiniDump(pExceptionInfo, (LPCTSTR)creashFile);
	     return EXCEPTION_EXECUTE_HANDLER;
 }


int main(int argc, char *argv[])
{

	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	QApplication a(argc, argv);

	//创建共享内存,判断是否已经运行程序
	QSharedMemory mem("gessClient");
	if (!mem.create(1))
	{
		QMessageBox::warning(nullptr, "提示", "程序已经在运行, 软件将自动关闭!");
		return 1;
	}


	qApp->setFont(QFont(App::AppFontName, App::AppFontSize));

	//QTextCodec::setCodecForLocale(QTextCodec::codecForName(QTextCodec::codecForLocale()->name()));
	a.setWindowIcon(QIcon(":/res/App.png"));    //设置应用程序图标
	a.setApplicationName("gessClient");         //设置应用程序名称
	a.setApplicationVersion("V201712");			//设置应用程序版本
	//myHelper::SetUTF8Code();					//设置程序编码为UTF-8
	myHelper::SetChinese();						//设置程序为中文字库

	//赋值当前应用程序路径和桌面宽度高度
	App::AppPath    = QApplication::applicationDirPath() + "/";
	App::DeskWidth  = qApp->desktop()->availableGeometry().width();
	App::DeskHeight = qApp->desktop()->availableGeometry().height();
	g_Global.m_strSystemPath = App::AppPath;
	g_Global.SetCommonPath();
	//程序加载时先加载所有配置信息
	App::ReadConfig();

	//加载和应用样式
	//myHelper::SetAppStyle(App::AppStyle);

	//安装事件过滤，需要设置“CanMove属性”，窗口即可拖拉【登录窗口，有用到】
	AppInit *pInit = new AppInit();
	
	//登录
	frmLoginDialog  *login = new frmLoginDialog();
	QStartup::GetInstance()->SetApplication(&a);
	QStartup::GetInstance()->SetLoginDlg(login);
	if (!QStartup::GetInstance()->Initialize())
		return 0;


	//pInit->installEventFilter(login);
	//login->show();

	//QStartup::GetInstance()->SetApplication(&a);
	//QStartup::GetInstance()->SetLoginDlg(login);
	//if (QStartup::GetInstance()->Initialize()/*&& login->exec() == QDialog::Accepted*/)//成功登录
	
	if(login->exec() == QDialog::Accepted)
	{
		QMessageBox *box = new QMessageBox(QMessageBox::Information, "登陆提示", "      登陆成功！      ");
		box->setModal(false);
		box->setAttribute(Qt::WA_DeleteOnClose);
		QTimer::singleShot(5000, box, SLOT(accept()));
		box->exec();

		mainWindow   w;
		w.InitUI();
		w.InitAllData();
		pInit->installEventFilter(&w);

		w.show();
		//w.showMaximized();
		//w.setGeometry(qApp->desktop()->availableGeometry());
		return a.exec();
	}
	else
		return 0;

}
