
#include "MyExcel.h"
#include <QAxObject>
#include <QStandardItemModel>
CMyExcel::CMyExcel(void)
{
}

CMyExcel::~CMyExcel(void)
{

	delete excel;
}
/*
根据模板将CListCtrl的数据写入到excel
注意：模板的
*/
void CMyExcel::WriteListDataToExcel(QTableView &lst, QString csNewSheetName, QString csTitleName, const vector<int>  &vecAlign, const QString &csPath, QString csTemSheetName /*= "sheet1"*/)
{
	QString cColLineBeg,cColLineEnd; // 记录excel的字符串格式的行列


	// 获得CListCtrl的行数和列数
	int iColumn = lst.model()->columnCount();//lst.GetHeaderCtrl()->GetItemCount(); //列数
	int iLine   = lst.model()->rowCount();    //行数

	excel = new QExcel(csPath);
	
	//合并单元格  
	excel->selectSheet("Sheet1");
	excel->RenameSheet(csNewSheetName);
	excel->mergeCells("A1:P1");
	excel->mergeCells(1, 1, 1 ,10);

	//写标题
	excel->setCellString(1, 1, csNewSheetName);
	excel->setCellTextCenter(1, 1);
	excel->setCellFontSize(1, 1, 24);

	excel->setRowHeight(1, 80);
	excel->setCellFontBold( 1, 1 ,true);
	

	//写数据
	// 加载列标题（小标题列）
	for (int i = 0; i < iColumn; i++)
	{
		// 获取list中的列标题
		QString sTitle = lst.model()->headerData(i, Qt::Horizontal).toString();;

		excel->setCellString(2, i+1, sTitle);
		excel->setColumnWidth(i + 1,10);
	}

	for (int i = 0; i < iLine; i++)
	{
		for (int j = 0; j < iColumn; j++)
		{
			QString str = ((QStandardItemModel *)lst.model())->item(i,j)->text();
			excel->setCellString(i+3, j+1, str);
			//居中
			excel->setCellTextCenter(i+3,j+1);

		}
	}

	excel->show();

}
