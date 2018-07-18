#include "IniFilesManager.h"
#include "HJGlobalFun.h"
//#include "winbase.h"
#include "app.h"

QIniFilesManager::QIniFilesManager(void)
{
}

QIniFilesManager::~QIniFilesManager(void)
{
}

void QIniFilesManager::ReadUserChoose( const QString &csPath, const QString &csTitle, bool bName, vector<QString> &vecUserNames )
{
	QVector<QString> vecAllNames;
	QVector<int> vecColumnIndexs;
	QString buf;
	QString csAllTitle = bName ? "AllColumnNames" : "AllColumnIDs";
	// 读取全部字段名称，并分割到vector
	App::GetPriProfileString(csPath, "Info", csAllTitle, buf);

	QString &strSpliter = QStringLiteral(QUERY_INI_SPLITER);
	CHJGlobalFun::SplitStrToVector(buf, strSpliter, vecAllNames);

	// 读取位置字段，并且分割到vector
	App::GetPriProfileString(csPath, "Info", csTitle, buf);
	CHJGlobalFun::SplitStrToVector(buf, strSpliter, vecColumnIndexs);

	// 加载当前显示项
	for(int i = 0; i < vecColumnIndexs.size(); i++)
	{
		vecUserNames.push_back(vecAllNames.at(vecColumnIndexs.at(i)));
	}
}

void QIniFilesManager::GetIniValueToVector( const QString &csPath, const QString &csMainTitle, const QString &csTitle, const QString &csSplitter, const QString &csDefaulut, vector<QString> &vecValue )
{
	QString buf;
	App::GetPriProfileString(csPath, csMainTitle, csTitle, buf);
	QVector<QString> vecTmp;
	CHJGlobalFun::SplitStrToVector(buf, csSplitter, vecTmp);

	for (int i = 0; i < vecTmp.size(); i++)
	{
		vecValue.push_back(vecTmp.at(i));
	}

}
