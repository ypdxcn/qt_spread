#pragma once

#include <QVector>
#include <QString>
#include <QTime>
#include  <QCHAR>
#include <QNetworkInterface>
#include <QList>
#include <QHostAddress>
//using namespace std;

//#include "GuiGridCtrl.h"

#define QUERY_INI_SPLITER "∧"
#define NEW_SPLITER "^"
#define LIST_INI_SPLITER "∧"

typedef enum
{
	ROUND_HALF_UP  = 0,
	ROUND_UP,
} EDoubleFormatMode;

class CHJGlobalFun
{
public: // 界面
	//
	//static void SetListCtrl(CGuiGridCtrl& listCtrl, int iItemHeight = 24);
	// 添加list的标题，并设置标题的宽度和对齐方式
	//static void AddListColumn( CGuiGridCtrl& listCtrl, int iCount, QString* psTitle,int* piAlign, int* piWidth );

	//处理INI文件
	static void ReadUserChoose(const QString &csPath, const QString &csTitle, bool bName, QVector<QString> &vecUserNames);

	//
	static void GetIniValueToVector(const QString &csPath, const QString &csMainTitle, const QString &csTitle, const QString &csSplitter, const QString &csDefaulut, QVector<QString> &vecValue);
public:
	//
	static QString QStringToQString(QString &cstr);
	//
	static QString IntToQString(int iValue);

	// 以splitword为分隔符，将字符串str中的字符串分割进vec;
	//static void SplitStrToVector( QString Str, QString SplitWord , QVector<QString> &Vec, bool bAddSplit = true, bool bRemoveBlank = true);

	// 以splitword为分隔符，将字符串str中的字符串分割进vec;
	static void SplitStrToVector( QString &sOrg, const QString &sSplitter , QVector<QString> &Vec, bool bAddSplit = true, bool bRemoveBlank = true);

	static void SplitStrToVector( QString Str, QString SplitWord , QVector<int> &Vec, bool bAddSplit = true, bool bRemoveBlank = true);

	// 格式化浮点数相关
	static QString FormatFloatQuote(QString strFloat, bool b45 = false); // 格式化报价
	static QString FormatFloat(QString strFloat, bool b45 = false); // 格式化浮点数
	static QString DoubleToQString(double dValue, bool b45 = true);
	static bool CheckFloat(QString &sFloat);
	static QString DealKeXue(QString &strFloatALL, int iFindIndexE); // 处理黄金服务器返回的科学计数法的转换
	static QString Float45(QString &strZheng, QString &strXiao, int iReserve); // 处理浮点数的四射五入
	static int QStringXiaoAdd1(QString &strFloat); //

	// 判断一个字符串是否全部是由数字组成
	static bool IsNumber(const QString &sNum); 

	// 功能：获取Datetimepicker控件的时间，并转换为"20100201"格式的字符串
	//static QString GetFormatTimeStr(CDateTimeCtrl &dtctrl);

	// 格式化日期字符串，使得为1980-02-01的格式
	static QString FormatDate( QString sDateOrg );

	// 格式化时间字符串，格式化时间为12:06:01的形式
	static QString FormatTime( QString sTimeOrg );

	// 将一个日期字符串转换成CTime类型
	static QTime TranslateDateStrToCTime(QString sDate);
	static QTime TranslateDateStrToCTime( const QString &sDate);

	// 根据买卖方向ID获取买卖方向名称
	static QString GetBuySellName(const QString &sID);

	//static void PairAdd(QQVector< QPair<QString,QString> > & v, QString sTitle, QString sValue);

	static bool IsDoubleZero(double dValue);


	static double DoubleFormat(double dValue, int iScale = 2, EDoubleFormatMode eMode = ROUND_HALF_UP);

    static double DoubleFormatNoNine(double dValue, int iScale = 2);

	static int DoubleToInt(double dValue);

	static bool IsContinuousNumber(const QString &sNum);

	static bool IsHasContinuousSameWord(const QString &sNum, int iCount);

	static int CompareDouble(double dValue1, double dValue2);

	// 判断一个字符串是否是邮箱（邮箱）
	static bool IsEmail(const QString &strValue);

	// 判断一个字符串是否是联系电话（传真，电话号码）
	static bool IsPhone(const QString &strValue);

	// 生成nCount位的随机数
	static std::string GetRandNumber( short nCount );

	static QString GetCurTimeFormat();

	static QString GetCurDateFormat();

	static bool RunFileExe( const QString &csFullPath );

	//static bool RunCreateProcess( const LPSTR pPath );

	//static bool RunShellExecute( const char* pPath, const char* pPara = "" );
	static QString getHostIpAddress();

	static QString str2qstr(const std::string& str);
	static std::string qstr2str(const QString & qstr);






};






