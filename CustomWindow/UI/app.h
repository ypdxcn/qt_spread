#ifndef APP_H
#define APP_H

#include <QVector>

class QString;

struct AccountMgr
{
	QString alias;
	QString user;
	QString psw;
};

class App
{
public:
    static int AppFontSize;                 //字体大小
	static int DeskWidth;                   //桌面可用宽度
	static int DeskHeight;                  //桌面可用高度

    static QString AppPath;                 //应用程序路径
    static QString AppFontName;             //字体名称
	static QString AppTitle;                //界面标题
	static QString AppStyle;                //界面样式
	static QString LastConfig;              //软件最后配置信息

	static QString ip;
	static QString port;
	static QString user;
	static QString psw;

	static QString net_agent;			//  网络运营商id    
	static QString bank_no;				// 银行账号
	static QString net_envionment;		// 网络环境id 

	static QString ver_num;				// 银行账号
	static QString is_down_para;		// 网络环境id 

	static QString order_showTip;				// 下单提示
	static QString order_return_showTip;		// 下单返回提示 

	static QVector <AccountMgr> accMgr;                //固定暂时支持10个账户
    static void ReadConfig();           
    static void WriteConfig();         

	//config.ini
	static void WriteConfigProperty(QString  &strField,QString &strVal);
	static int  GetProperty(QString  &strField, QString &strVal);

	//server.ini
	static int GetServerProperty(const QString  &strKey, const QString  &strField, QString &strVal);
	static int WriteServerProperty(const QString  &strKey, const QString  &strField, const QString &strVal);


	static int GetPriProfileString(const QString  & lpAppPath,const QString  & lpKeyName,const QString  & strField,	QString  & strVal);
	static int WritePriProfileString(const QString  & lpAppPath,const QString  & lpKeyName,	const QString  & strField,const QString  & strVal);
};

#endif // APP_H
