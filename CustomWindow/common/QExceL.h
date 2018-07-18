#pragma once

#include <QString>    
#include <QVariant>    
#include <QColor>


class QAxObject;

class QExcel : public QObject
{
	//Q_OBJECT
public:
	QExcel(QString xlsFilePath, QObject *parent = 0);
	~QExcel();

public:
	QAxObject * getWorkBooks();
	QAxObject * getWorkBook();
	QAxObject * getWorkSheets();
	QAxObject * getWorkSheet();

public:
	/**************************************************************************/
	/* 工作表                                                                 */
	/**************************************************************************/
	void selectSheet(const QString& sheetName);
	//sheetIndex 起始于 1    
	void selectSheet(int sheetIndex);
	void deleteSheet(const QString& sheetName);
	void deleteSheet(int sheetIndex);
	void insertSheet(QString sheetName);
	void RenameSheet(const QString& sheetName);//改名字
	int getSheetsCount();
	//在 selectSheet() 之后才可调用    
	QString getSheetName();
	QString getSheetName(int sheetIndex);

	/**************************************************************************/
	/* 单元格                                                                 */
	/**************************************************************************/
	void setCellString(int row, int column, const QString& value);
	//cell 例如 "A7"    
	void setCellString(const QString& cell, const QString& value);
	//range 例如 "A5:C7"    
	void mergeCells(const QString& range);
	void mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn);
	QVariant getCellValue(int row, int column);
	void clearCell(int row, int column);
	void clearCell(const QString& cell);

	/**************************************************************************/
	/* 布局格式                                                               */
	/**************************************************************************/
	void getUsedRange(int *topLeftRow, int *topLeftColumn, int *bottomRightRow, int *bottomRightColumn);
	void setColumnWidth(int column, int width);
	void setRowHeight(int row, int height);
	void setCellTextCenter(int row, int column);
	void setCellTextCenter(const QString& cell);
	void setCellTextWrap(int row, int column, bool isWrap);
	void setCellTextWrap(const QString& cell, bool isWrap);
	void setAutoFitRow(int row);
	void mergeSerialSameCellsInAColumn(int column, int topRow);
	int getUsedRowsCount();
	void setCellFontBold(int row, int column, bool isBold);
	void setCellFontBold(const QString& cell, bool isBold);
	void setCellFontSize(int row, int column, int size);
	void setCellFontSize(const QString& cell, int size);
	bool setFontBold(QString row, QString column, bool bBold);
	

	bool  setBorder(QString cell, int weight, QColor  color);
	bool  setBackColor(QString cell, QColor  color);
	bool  setRowHeight(QString  cell, int  height);
	bool  setFontAlignment(QString  cell, int  val);
	/**************************************************************************/
	/* 文件                                                                   */
	/**************************************************************************/
	void save();
	void close();
	void show();


	/*
	处理函数 */
	// 将字符串形式的列转换为数字形式的列
	int     Column_StrToInt(const QString &csColumn);
	QString FormatColLine(QString csColumn, int iLine);
	QString GeForColumn(QString csColumn);
	int     CharToInt(char cColumn);

//public slots:
//	void  saveLastError(int index, QString  str1, QString str2, QString str3);
private:
	QAxObject * excel;
	QAxObject * workBooks;
	QAxObject * workBook;
	QAxObject * sheets;
	QAxObject * sheet;
};