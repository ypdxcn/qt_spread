#pragma once

#include <string> 
#include <vector>
#include <QString>

using namespace std;

#define QUERY_INI_SPLITER "¡Ä"
#define NEW_SPLITER "^"

class QIniFilesManager
{
public:
	QIniFilesManager(void);
public:
	~QIniFilesManager(void);

	// 
	void ReadUserChoose(const QString &csPath, const QString &csTitle, bool bName, vector<QString> &vecUserNames);

	//
	void GetIniValueToVector(const QString &csPath, const QString &csMainTitle, const QString &csTitle, const QString &csSplitter, const QString &csDefaulut, vector<QString> &vecValue);
};
