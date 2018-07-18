#pragma once

#include "Qexcel.h"
#include <vector>
#include <qtableview.h>

using namespace std;
class CMyExcel
{
public:
	CMyExcel(void);
public:
	~CMyExcel(void);

	// 根据模板将CListCtrl的数据写入到excel
	void WriteListDataToExcel(QTableView &lst, QString csNewSheetName, QString csTitleName, const vector<int>  &vecAlign, const QString &csPath, QString csTemSheetName="sheet1");




	QExcel *excel;

};