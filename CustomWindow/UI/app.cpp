#include "app.h"
#include "myhelper.h"

QString App::AppPath = "";

int App::AppFontSize = 12;
int App::DeskWidth = 1024;
int App::DeskHeight = 768;

QString App::AppTitle = QStringLiteral("黄金交易平台");
QString App::AppFontName = "Microsoft YaHei";
QString App::AppStyle = "";
QString App::ip   = "127.0.0.1";
QString App::port = 6019;
QString App::user = "";
QString App::psw = "";
QString App::net_agent = "";			//  网络运营商id    
QString App::bank_no = "";				// 银行账号
QString App::net_envionment = "";		// 网络环境id 
QString App::ver_num = "";				// 银行账号
QString App::is_down_para = "";		// 网络环境id 
QString App::order_showTip = "";//下单提示
QString App::order_return_showTip = "";//下单返回提示
QVector <AccountMgr> App::accMgr;




void App::ReadConfig()
{
    QString fileName = App::AppPath + "config.ini";
	//如果配置文件不存在,则以初始值继续运行
	if (!myHelper::FileIsExist(fileName))
	{
		//对应中文转成正确的编码
		App::AppTitle = App::AppTitle.toLatin1();
		App::AppStyle = App::AppStyle.toLatin1();

		return;
	}

    QSettings *set = new QSettings(fileName, QSettings::IniFormat);
	//读取的文本是ansi编码，需要转换【解决乱码】
	set->setIniCodec(QTextCodec::codecForName("UTF8"));

    set->beginGroup("AppConfig");
	App::AppTitle = set->value("AppTitle").toString();
	App::AppStyle = set->value("AppStyle").toString();
    set->endGroup();

	set->beginGroup("info");
	App::port			= set->value("LOGIN.port").toString();
	App::ip				= set->value("LOGIN.ip").toString();
	App::net_agent		= set->value("LOGIN.net_agent").toString();;			//  网络运营商id    
	App::bank_no		= set->value("LOGIN.bank_no").toString();;				// 银行账号
	App::net_envionment = set->value("LOGIN.net_envionment").toString();;		// 网络环境id 
	App::user           = set->value("LOGIN.cust_code").toString();
	App::ver_num        = set->value("HJ.ver_num").toString();
	App::is_down_para   = set->value("HJ.is_down_para").toString();
	//此处没用到，配置里面已经删除kenny  20180329
	App::order_showTip = set->value("Order.showTip").toString();
	App::order_return_showTip = set->value("Order.showReturnTip").toString();
	set->endGroup();


	set->beginGroup("account");
	for (int i = 0; i < set->value("ACCOUNT.count").toInt(); ++i)
	{
		AccountMgr acc;
		acc.alias = set->value(QString("ACCOUNT.account%1.alias").arg(QString::number(i + 1))).toString();
		acc.user = set->value(QString("ACCOUNT.account%1.user").arg(QString::number(i + 1))).toString();
		acc.psw = set->value(QString("ACCOUNT.account%1.pswd").arg(QString::number(i + 1))).toString();
		App::accMgr.push_back(acc);
	}
	set->endGroup();

}

//这个写死的意义不大，待删除
//写入配置文件
void App::WriteConfig()
{
    QString fileName =  App::AppPath + "config.ini";
    QSettings *set   =  new QSettings(fileName, QSettings::IniFormat);
   
	//读取的文本是ansi编码，需要转换【解决乱码】
	set->setIniCodec("UTF8");

	set->beginGroup("AppConfig");

	set->setValue("AppTitle", App::AppTitle);
	set->setValue("AppStyle", App::AppStyle);

    set->endGroup();
}



void App::WriteConfigProperty(QString  &strField, QString &strVal)
{
	QString fileName = App::AppPath + "config.ini";
	QSettings *set = new QSettings(fileName, QSettings::IniFormat);

	//读取的文本是ansi编码，需要转换【解决乱码】
	set->setIniCodec("UTF8");

	set->beginGroup("info");
	set->setValue(strField, strVal);
	set->endGroup();
}



int App::GetProperty(QString  &strField, QString &strVal)
{
	QString fileName = App::AppPath + "config.ini";
	QSettings *set = new QSettings(fileName, QSettings::IniFormat);
	//读取的文本是ansi编码，需要转换【解决乱码】
	set->setIniCodec("UTF8");


	QStringList groupList = set->childGroups();
	foreach(QString group, groupList)
	{
		set->beginGroup(group);
		QStringList keyList = set->childKeys();
		foreach(QString key, keyList)
		{
			if (key == strField)
			{
				strVal = set->value(key).toString();

				set->endGroup();
				return 1;
			}
		}
		set->endGroup();
	}

	return 0;

}


//编码  ，代码已经验证过
int App::GetServerProperty(const QString  &strKey, const QString  &strField, QString &strVal)
{
	QString fileName = App::AppPath + "serverdata.ini";
	QSettings *set = new QSettings(fileName, QSettings::IniFormat);
	//读取的文本是ansi编码，需要转换【解决乱码】
	set->setIniCodec("UTF8");//需要此代码

	//QStringList groupList = set->childGroups();
	//foreach(QString group, groupList)
	{
		//set->beginGroup(group);
		set->beginGroup(strKey);
		QStringList keyList = set->childKeys();
		//foreach(QString key, keyList)
		{
			//if (key == strField)
			{
				strVal = set->value(strField).toString();
				strVal = QString::fromUtf8(set->value(strField).toByteArray());

				set->endGroup();
				return 1;
			}
		}
		set->endGroup();
	}

	return 0;

}
int App::WriteServerProperty(const QString  &strKey,const QString  &strField, const QString &strVal)
{
	QString fileName = App::AppPath + "config.ini";
	QSettings *set = new QSettings(fileName, QSettings::IniFormat);
	//写的文本是ansi编码，需要转换【解决乱码】  20180329
	set->setIniCodec("UTF8");

	set->beginGroup(strKey);
	set->setValue(strField, strVal);
	set->endGroup();

	return 0;
}


int App::GetPriProfileString(const QString  & lpAppPath,  const QString  & lpKeyName, const QString  & strField, QString  & strVal)
{
	QString fileName = lpAppPath;
	QSettings *set   = new QSettings(fileName, QSettings::IniFormat);
	//读取的文本是ansi编码，需要转换【解决乱码】
	set->setIniCodec("UTF8");

	set->beginGroup(lpKeyName);
	QStringList keyList = set->childKeys();
	foreach(QString key, keyList)
	{
		if (key == strField)
		{
			strVal = set->value(key).toString();

			set->endGroup();
			return 1;
		}
	}
	set->endGroup();


	return 0;
}



int App::WritePriProfileString(const QString  & lpAppPath,
								const QString  & lpKeyName,
								const QString  & strField,
								const QString  & strVal)
{

	QString fileName = lpAppPath;//App::AppPath + lpAppName;
	QSettings *set = new QSettings(fileName, QSettings::IniFormat);
	//kenny  20180329
	set->setIniCodec(QTextCodec::codecForName("UTF8"));

	set->beginGroup(lpKeyName);
	set->setValue(strField, strVal);
	set->endGroup();

	return 0;
}