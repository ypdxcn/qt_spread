#ifndef MYHELPER_H
#define MYHELPER_H
//////////////////////////////////////////////////////////////////////////////////////////////////
//全局处理函数
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////


#include <QtCore>
#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif
#include "Dialog/frminputdialog.h"
#include "Dialog/frmmessagedialog.h"
#include "iconhelper.h"
#include "app.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include "osdepend.h"
#include <string>
#include <QString>
#include <codecvt>
#include <locale>

class myHelper : public QObject
{

public:

	static void AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath)
	{
		QSettings *reg = new QSettings(
			"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
			QSettings::NativeFormat);

		if (IsAutoRun)
		{
			reg->setValue(AppName, AppPath);
		}
		else
		{
			reg->setValue(AppName, "");
		}
	}

	//设置编码为UTF8
	static void SetUTF8Code()
	{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
		QTextCodec *codec = QTextCodec::codecForName("UTF-8");
		QTextCodec::setCodecForLocale(codec);
		QTextCodec::setCodecForCStrings(codec);
		QTextCodec::setCodecForTr(codec);
#endif


	}

	//设置为中文字符
	static void SetChinese()
	{
		QTranslator *translator = new QTranslator(qApp);
		translator->load(":/image/qt_zh_CN.qm");
		qApp->installTranslator(translator);
	}

	//设置指定样式
	static void SetAppStyle(const QString &qssFile)
	{
		QFile file(qssFile);
		if (file.open(QFile::ReadOnly)) {
			QString qss = QLatin1String(file.readAll());
			qApp->setStyleSheet(qss);
			QString PaletteColor = qss.mid(20, 7);
			qApp->setPalette(QPalette(QColor(PaletteColor)));
			file.close();
		}
	}

	//设置指定样式
	//本函数没有被用到，各个widget用的Macrodefs.h文件里面的"setWidgetStyleFromQssFile"
	//待删除.........
	static void setWidgetStyle(const QString &style, QWidget*wg = nullptr)
	{
		QFile qss(style);
		if (qss.open(QFile::ReadOnly))
		{
			if (nullptr == wg)
				qApp->setStyleSheet(qss.readAll());
			else
				wg->setStyleSheet(qss.readAll());

			qss.close();
		}
		else
		{
			qDebug("无法打开样式表");
		}
	}

	//判断是否是IP地址
	static bool IsIP(QString IP)
	{
		QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
		return RegExp.exactMatch(IP);
	}

	//显示输入框
	static QString ShowInputBox(QString info, bool &ok)
	{
		frmInputDialog input;
		input.SetMessage(info);
		ok = input.exec();
		return input.GetValue();
	}

	//显示信息框,仅确定按钮
	static void ShowMessageBoxInfo(QString info)
	{
		frmMessageDialog msg;
		//msg.SetMessage(info, 0);
		msg.exec();
	}


	static void ShowMessageBoxError(QString info)
	{
		frmMessageDialog msg;
		//msg.SetMessage(info, 2);
		msg.exec();
	}

	//显示询问框,确定和取消按钮
	static int ShowMessageBoxQuesion(QString info)
	{
		frmMessageDialog msg;
		//msg.SetMessage(info, 1);
		return msg.exec();
	}


	//窗口居中
	static void FormInCenter(QWidget *frm)
	{
		int frmX = frm->width();
		int frmY = frm->height();
		QDesktopWidget w;
		int deskWidth = w.availableGeometry().width();
		int deskHeight = w.availableGeometry().height();
		QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
		frm->move(movePoint);
	}


	//16进制字符串转字节数组
	static QByteArray HexStrToByteArray(QString str)
	{
		QByteArray senddata;
		int hexdata, lowhexdata;
		int hexdatalen = 0;
		int len = str.length();
		senddata.resize(len / 2);
		char lstr, hstr;
		for (int i = 0; i < len; )
		{
			hstr = str[i].toLatin1();
			if (hstr == ' ')
			{
				i++;
				continue;
			}
			i++;

			if (i >= len)
			{
				break;
			}
			lstr = str[i].toLatin1();
			hexdata = ConvertHexChar(hstr);
			lowhexdata = ConvertHexChar(lstr);

			if ((hexdata == 16) || (lowhexdata == 16))
			{
				break;
			}
			else
			{
				hexdata = hexdata * 16 + lowhexdata;
			}
			i++;
			senddata[hexdatalen] = (char)hexdata;
			hexdatalen++;
		}
		senddata.resize(hexdatalen);
		return senddata;
	}

	static char ConvertHexChar(char ch)
	{
		if ((ch >= '0') && (ch <= '9'))
		{
			return ch - 0x30;
		}
		else if ((ch >= 'A') && (ch <= 'F'))
		{
			return ch - 'A' + 10;
		}
		else if ((ch >= 'a') && (ch <= 'f'))
		{
			return ch - 'a' + 10;
		}
		else
		{
			return (-1);
		}
	}

	//字节数组转16进制字符串
	static QString ByteArrayToHexStr(QByteArray data)
	{
		QString temp = "";
		QString hex = data.toHex();
		for (int i = 0; i < hex.length(); i = i + 2)
		{
			temp += hex.mid(i, 2) + " ";
		}
		return temp.trimmed().toUpper();
	}

	//16进制字符串转10进制
	static int StrHexToDecimal(QString strHex)
	{
		bool ok;
		return strHex.toInt(&ok, 16);
	}

	//10进制字符串转10进制
	static int StrDecimalToDecimal(QString strDecimal)
	{
		bool ok;
		return strDecimal.toInt(&ok, 10);
	}

	//2进制字符串转10进制
	static int StrBinToDecimal(QString strBin)
	{
		bool ok;
		return strBin.toInt(&ok, 2);
	}

	//16进制字符串转2进制字符串
	static QString StrHexToStrBin(QString strHex)
	{
		uchar decimal = StrHexToDecimal(strHex);
		QString bin = QString::number(decimal, 2);
		uchar len = bin.length();
		if (len < 8)
		{
			for (int i = 0; i < 8 - len; i++)
			{
				bin = "0" + bin;
			}
		}
		return bin;
	}

	//10进制转2进制字符串一个字节
	static QString DecimalToStrBin1(int decimal)
	{
		QString bin = QString::number(decimal, 2);
		uchar len = bin.length();
		if (len <= 8)
		{
			for (int i = 0; i < 8 - len; i++)
			{
				bin = "0" + bin;
			}
		}
		return bin;
	}

	//10进制转2进制字符串两个字节
	static QString DecimalToStrBin2(int decimal)
	{
		QString bin = QString::number(decimal, 2);
		uchar len = bin.length();
		if (len <= 16)
		{
			for (int i = 0; i < 16 - len; i++)
			{
				bin = "0" + bin;
			}
		}
		return bin;
	}

	//10进制转16进制字符串,补零.
	static QString DecimalToStrHex(int decimal)
	{
		QString temp = QString::number(decimal, 16);
		if (temp.length() == 1)
		{
			temp = "0" + temp;
		}
		return temp;
	}

	//延时
	static void Sleep(int sec)
	{
		QTime dieTime = QTime::currentTime().addMSecs(sec);
		while (QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}
	}

	//设置系统日期时间
	static void SetSystemDateTime(int year, int month, int day, int hour, int min, int sec)
	{
		QProcess p(0);

		p.start("cmd");
		p.waitForStarted();
		p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
		p.closeWriteChannel();
		p.waitForFinished(1000);
		p.close();

		p.start("cmd");
		p.waitForStarted();
		p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
		p.closeWriteChannel();
		p.waitForFinished(1000);
		p.close();
	}

	//窗体居中显示
	static void FormInCenter(QWidget *frm, int deskWidth, int deskHeigth)
	{
		int frmX = frm->width();
		int frmY = frm->height();
		QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeigth / 2 - frmY / 2);
		frm->move(movePoint);
	}

	//获取选择的文件
	static QString GetFileName(QString filter)
	{
		return QFileDialog::getOpenFileName(0, "选择文件", QCoreApplication::applicationDirPath(), filter);
	}

	//获取选择的文件集合
	static QStringList GetFileNames(QString filter)
	{
		return QFileDialog::getOpenFileNames(0, "选择文件", QCoreApplication::applicationDirPath(), filter);
	}

	//获取选择的目录
	static QString GetFolderName()
	{
		return QFileDialog::getExistingDirectory();;
	}

	//获取文件名,含拓展名
	static QString GetFileNameWithExtension(QString strFilePath)
	{
		QFileInfo fileInfo(strFilePath);
		return fileInfo.fileName();
	}

	//获取选择文件夹中的文件
	static QStringList GetFolderFileNames(QStringList filter)
	{
		QStringList fileList;
		QString strFolder = QFileDialog::getExistingDirectory();
		if (!strFolder.length() == 0)
		{
			QDir myFolder(strFolder);
			if (myFolder.exists())
			{
				fileList = myFolder.entryList(filter);
			}
		}
		return fileList;
	}

	//文件夹是否存在
	static bool FolderIsExist(QString strFolder)
	{
		QDir tempFolder(strFolder);
		return tempFolder.exists();
	}

	//文件是否存在
	static bool FileIsExist(QString strFile)
	{
		QFile tempFile(strFile);
		return tempFile.exists();
	}

	//复制文件
	static bool CopyFile(QString sourceFile, QString targetFile)
	{
		if (FileIsExist(targetFile))
		{
			if (ShowMessageBoxQuesion("文件已经存在,是否替换?") == 1)
			{
				return false;
			}
		}
		return QFile::copy(sourceFile, targetFile);
	}

	//异或加密算法
	static QString getXorEncryptDecrypt(QString str, char key)
	{
		QByteArray data = str.toLatin1();
		int size = data.size();
		for (int i = 0; i < size; i++)
		{
			data[i] = data[i] ^ key;
		}
		return QLatin1String(data);
	}

	//得到本地IP
	static QString getHostIpAddress()
	{
		QString strIpAddress;
		QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
		// 获取第一个本主机的IPv4地址  
		int nListSize = ipAddressesList.size();
		for (int i = 0; i < nListSize; ++i)
		{
			if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
				ipAddressesList.at(i).toIPv4Address()) {
				strIpAddress = ipAddressesList.at(i).toString();
				break;
			}
		}
		// 如果没有找到，则以本地IP地址为IP  
		if (strIpAddress.isEmpty())
			strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
		return strIpAddress;
	}


	static void GetSystemIniPath(QString &strVal)
	{
		strVal = App::AppPath + "config.ini";
	}

	//Ws2_32.lib 在连接库包含进去了
	static int initLocalNet()
	{
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR)
		{
			return -1;
		}

		return  1;

	}


};

#endif // MYHELPER_H
