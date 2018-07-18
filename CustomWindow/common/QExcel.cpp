#include <QAxObject>    
#include <QFile>    
#include <QStringList>    
#include <QDebug>    


#include "qexcel.h"    
#pragma execution_character_set("utf-8")


QExcel::QExcel(QString xlsFilePath, QObject *parent)
{

	excel     = nullptr;
	workBooks = nullptr;
	workBook  = nullptr;
	sheets    = nullptr;
	sheet     = nullptr;

	excel     = new QAxObject("Excel.Application", parent);
	
	excel->setProperty("Visible", false); //不显示Excel界面，如果为true会看到启动的Excel界面   
	//excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。

	workBooks = excel->querySubObject("Workbooks");

	QFile file(xlsFilePath);//xlsFilePath   "E:\\trade.xls"
	//2018-1-12  添加OPEN，打开已经存在的文件
	if (file.exists() )
	{
		workBooks->dynamicCall("Open(const QString &)", xlsFilePath);//"E:\\trade.xls"

		workBook = excel->querySubObject("ActiveWorkBook");
		if (nullptr == workBook)
		{
			workBooks->dynamicCall("Add");//空文档则加入workbook
			workBook = excel->querySubObject("ActiveWorkBook");
		}
		sheets   = workBook->querySubObject("WorkSheets");
		
	}

}


//void  QExcel::saveLastError(int index, QString  str1, QString str2, QString str3)
//{
//	int  t = index;
//}

QExcel::~QExcel()
{
	close();
}

void QExcel::close()
{
	excel->dynamicCall("Quit()");

	delete sheet;
	delete sheets;
	delete workBook;
	delete workBooks;
	delete excel;

	excel     = nullptr;
	workBooks = nullptr;
	workBook  = nullptr;
	sheets    = nullptr;
	sheet     = nullptr;
}

void QExcel::show()
{
	excel->setProperty("Visible", true);
}
QAxObject *QExcel::getWorkBooks()
{
	return workBooks;
}

QAxObject *QExcel::getWorkBook()
{
	return workBook;
}

QAxObject *QExcel::getWorkSheets()
{
	return sheets;
}

QAxObject *QExcel::getWorkSheet()
{
	return sheet;
}

void QExcel::selectSheet(const QString& sheetName)
{
	sheet = sheets->querySubObject("Item(const QString&)", sheetName);
}

void QExcel::RenameSheet(const QString& sheetName)
{
	sheet->setProperty("Name", sheetName);
}

void QExcel::deleteSheet(const QString& sheetName)
{
	QAxObject * a = sheets->querySubObject("Item(const QString&)", sheetName);
	a->dynamicCall("delete");
}

void QExcel::deleteSheet(int sheetIndex)
{
	QAxObject * a = sheets->querySubObject("Item(int)", sheetIndex);
	a->dynamicCall("delete");
}

void QExcel::selectSheet(int sheetIndex)
{
	sheet = sheets->querySubObject("Item(int)", sheetIndex);
}

void QExcel::setCellString(int row, int column, const QString& value)
{
	if (column < 1 || row < 1)
		return;

	QAxObject *range = sheet->querySubObject("Cells(int,int)", row, column);//5，3，即第5行第3列
	range->dynamicCall("SetValue(const QString&)", value);
	//range->setProperty("Value", value);
}

void QExcel::setCellFontBold(int row, int column, bool isBold)
{
	QString cell;
	cell.append(QChar(column - 1 + 'A'));
	cell.append(QString::number(row));

	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range = range->querySubObject("Font");
	range->setProperty("Bold", isBold);
}

void QExcel::setCellFontSize(int row, int column, int size)
{
	QString cell;
	cell.append(QChar(column - 1 + 'A'));
	cell.append(QString::number(row));

	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range = range->querySubObject("Font");
	range->setProperty("Size", size);
}

void QExcel::mergeCells(const QString& cell)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->setProperty("VerticalAlignment", -4108);//xlCenter    
	range->setProperty("WrapText", true);
	range->setProperty("MergeCells", true);
}

void QExcel::mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn)
{
	QString cell;
	cell.append(QChar(topLeftColumn - 1 + 'A'));
	cell.append(QString::number(topLeftRow));
	cell.append(":");
	cell.append(QChar(bottomRightColumn - 1 + 'A'));
	cell.append(QString::number(bottomRightRow));

	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->setProperty("VerticalAlignment", -4108);//xlCenter    
	range->setProperty("WrapText", true);
	range->setProperty("MergeCells", true);
}

QVariant QExcel::getCellValue(int row, int column)
{
	QAxObject *range = sheet->querySubObject("Cells(int,int)", row, column);
	return range->property("Value");
}

//设置行高
bool  QExcel::setRowHeight(QString  cell,int  height)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	return range->setProperty("RowHeight", height);
}

//设置行字体对齐
////左对齐（xlLeft）：-4131  居中（xlCenter）：-4108  右对齐（xlRight）：-4152

//cell->setProperty("HorizontalAlignment", -4108);//左对齐（xlLeft）：-4131  居中（xlCenter）：-4108 右对齐（xlRight）：-4152
//cell->setProperty("VerticalAlignment", -4108); //上对齐（xlTop）-4160 居中（xlCenter）：-4108 下对齐（xlBottom）：-4107

bool  QExcel::setFontAlignment(QString  cell, int  val)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	return  range->setProperty("HorizontalAlignment", val);  
}

bool QExcel::setFontBold(QString row, QString column, bool bBold)
{
	QString cell;
	cell.append(row);
	cell.append(column);

	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range = range->querySubObject("Font");
	return range->setProperty("Bold", bBold);
}


bool  QExcel::setBackColor(QString cell,QColor  color)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	QAxObject* interior = range->querySubObject("Interior");
	return interior->setProperty("Color", color);   //设置单元格背景色（绿色）


}


bool  QExcel::setBorder(QString cell, int weight ,QColor  color)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	QAxObject* border = range->querySubObject("Borders");
	border->setProperty("Color", color);  
	return border->setProperty("Font", weight);   


}


void QExcel::save()
{
	workBook->dynamicCall("Save()");
}

int QExcel::getSheetsCount()
{
	return sheets->property("Count").toInt();
}

QString QExcel::getSheetName()
{
	return sheet->property("Name").toString();
}

QString QExcel::getSheetName(int sheetIndex)
{
	QAxObject * a = sheets->querySubObject("Item(int)", sheetIndex);
	return a->property("Name").toString();
}

void QExcel::getUsedRange(int *topLeftRow, int *topLeftColumn, int *bottomRightRow, int *bottomRightColumn)
{
	QAxObject *usedRange = sheet->querySubObject("UsedRange");
	*topLeftRow = usedRange->property("Row").toInt();
	*topLeftColumn = usedRange->property("Column").toInt();

	QAxObject *rows = usedRange->querySubObject("Rows");
	*bottomRightRow = *topLeftRow + rows->property("Count").toInt() - 1;

	QAxObject *columns = usedRange->querySubObject("Columns");
	*bottomRightColumn = *topLeftColumn + columns->property("Count").toInt() - 1;
}

void QExcel::setColumnWidth(int column, int width)
{
	QString columnName;
	if (column >= 27)
		columnName.append(('A')+QChar(column - 27 + 'A'));
	else
		columnName.append(QChar(column - 1 + 'A'));
	columnName.append(":");

	if (column >= 27)
		columnName.append(('A') + QChar(column - 27 + 'A'));
	else
		columnName.append(QChar(column - 1 + 'A'));

	QAxObject * col = sheet->querySubObject("Columns(const QString&)", columnName);
	col->setProperty("ColumnWidth", width);
}

void QExcel::setCellTextCenter(int row, int column)
{
	QString cell;
	if (column >= 27)
		cell.append(('A') + QChar(column - 27 + 'A'));
	else
		cell.append(QChar(column - 1 + 'A'));

	//cell.append(QChar(column - 1 + 'A'));
	cell.append(QString::number(row));

	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->setProperty("HorizontalAlignment", -4108);//xlCenter    
}

void QExcel::setCellTextWrap(int row, int column, bool isWrap)
{
	QString cell;
	cell.append(QChar(column - 1 + 'A'));
	cell.append(QString::number(row));

	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->setProperty("WrapText", isWrap);
}

void QExcel::setAutoFitRow(int row)
{
	QString rowsName;
	rowsName.append(QString::number(row));
	rowsName.append(":");
	rowsName.append(QString::number(row));

	QAxObject * rows = sheet->querySubObject("Rows(const QString &)", rowsName);
	rows->dynamicCall("AutoFit()");
}

void QExcel::insertSheet(QString sheetName)
{
	sheets->querySubObject("Add()");
	QAxObject * a = sheets->querySubObject("Item(int)", 1);
	a->setProperty("Name", sheetName);
}

void QExcel::mergeSerialSameCellsInAColumn(int column, int topRow)
{
	int a, b, c, rowsCount;
	getUsedRange(&a, &b, &rowsCount, &c);

	int aMergeStart = topRow, aMergeEnd = topRow + 1;

	QString value;
	while (aMergeEnd <= rowsCount)
	{
		value = getCellValue(aMergeStart, column).toString();
		while (value == getCellValue(aMergeEnd, column).toString())
		{
			clearCell(aMergeEnd, column);
			aMergeEnd++;
		}
		aMergeEnd--;
		mergeCells(aMergeStart, column, aMergeEnd, column);

		aMergeStart = aMergeEnd + 1;
		aMergeEnd = aMergeStart + 1;
	}
}

void QExcel::clearCell(int row, int column)
{
	QString cell;
	//cell.append(QChar(column - 1 + 'A'));
	//cell.append(QString::number(row));

	if (column >= 26*2+1)
		cell.append(('B') + QChar(column - (26 * 2 + 1) + 'A'));
	else	if(column >= 26*1 +1)
		cell.append(('A') + QChar(column - (26 * 1 + 1) + 'A'));
	else
		cell.append(QChar(column - 1 + 'A'));

	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->dynamicCall("ClearContents()");
}

void QExcel::clearCell(const QString& cell)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->dynamicCall("ClearContents()");
}

int QExcel::getUsedRowsCount()
{
	QAxObject *usedRange = sheet->querySubObject("UsedRange");
	int topRow = usedRange->property("Row").toInt();
	QAxObject *rows = usedRange->querySubObject("Rows");
	int bottomRow = topRow + rows->property("Count").toInt() - 1;
	return bottomRow;
}

void QExcel::setCellString(const QString& cell, const QString& value)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->dynamicCall("SetValue(const QString&)", value);
}

void QExcel::setCellFontSize(const QString &cell, int size)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range = range->querySubObject("Font");
	range->setProperty("Size", size);
}

void QExcel::setCellTextCenter(const QString &cell)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->setProperty("HorizontalAlignment", -4108);//xlCenter    
}

void QExcel::setCellFontBold(const QString &cell, bool isBold)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range = range->querySubObject("Font");
	range->setProperty("Bold", isBold);
}

void QExcel::setCellTextWrap(const QString &cell, bool isWrap)
{
	QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
	range->setProperty("WrapText", isWrap);
}

void QExcel::setRowHeight(int row, int height)
{
	QString rowsName;
	rowsName.append(QString::number(row));
	rowsName.append(":");
	rowsName.append(QString::number(row));

	QAxObject * r = sheet->querySubObject("Rows(const QString &)", rowsName);
	r->setProperty("RowHeight", height);
}


int QExcel::Column_StrToInt(const QString &csColumn)
{
	int iLen = csColumn.length();
	if (iLen == 1 || iLen == 2)
	{
		int iReturn = csColumn.at(iLen - 1).toLatin1() - 'A' + 1;
		if (iLen == 2)
		{
			iReturn += 26 * (csColumn.at(0).toLatin1() - 'A' + 1);
		}
		return iReturn;
	}
	else
	{
		//AfxMessageBox("暂时不支持，如果需要，则改进算法！");
		return -1;
	}
}


// 合并行列
QString QExcel::FormatColLine(QString csColumn, int iLine)
{
	QString csColumnNew = QString("%1%2").arg(csColumn).arg(iLine);

	return csColumnNew;
}


// 获取一列的前一列的字符串形式
QString QExcel::GeForColumn(QString csColumn)
{
	int iLen = csColumn.length();
	for (int i = iLen - 1; i > -1; i--)
	{
		char cTemp = csColumn.at(i).toLatin1();
		if (cTemp != 'A')
		{
			cTemp -= 1;
			csColumn[i] =  QChar::fromLatin1(cTemp);
			break;
		}
		else
		{
			if (i != 0)
			{
				cTemp = 'Z';
				csColumn[i] = QChar::fromLatin1(cTemp);
			}
			else
			{
				csColumn = csColumn.mid(1);
			}
		}
	}

	return csColumn;
}


int QExcel::CharToInt(char cColumn)
{
	char c = cColumn - 'A' + 1;
	return c;
}