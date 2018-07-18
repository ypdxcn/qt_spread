#pragma once

#include <QWidget>
#include <QAbstractNativeEventFilter>
#include "ReportBase.h"
#include <QDialog>
#include <QPrinter>
#include "QExcel.H"

#define CONSTANT_SYSTEM_STAT_DZFINISHED   "7"  // 系统状态 日终登帐完成

class reportDrawWidget : public QWidget, public QReportBase
{
public:
	reportDrawWidget(QWidget* parent = nullptr);

	void init();
	virtual void paintEvent(QPaintEvent *) override;
	void OutToExcel();
	void ExcelClear(QExcel &myExcelEx, int iCurLine);
	void WriteExcelAddEmptyLine(QExcel &myExcelEx, int &iCurLine);
	void WriteExcelModal(QExcel &myExcelEx, int &iCurLine, int iTitleCount, QString *pcsColIDBeg, QString *pcsBigTitle, int *piAlign, const QString &csFinalNextCol, const QStringList &m_arrayValue, int iDataLineCount);

	float m_xRate;
	float m_yRate;

	int m_nWidth;
	int m_nHeight;

	bool        m_bDay;
	string      m_strDate;
	Rsp6007     m_rsp6007;     // 报表数据


	QPainter     *m_Painter;

	// 是否显示确认日结单按钮
	void ShowConfirmBtn();
	QString GetReportTitle();

	// 格式化日结单的日期
	QString FormatDate(QString strValue);
	// 
	QString FormatDate();
	// 格式化月结单的日期
	QString FormatDate2();
	void  ShowReportDlg(const bool &bMode, Rsp6007 &rsp6007, const string &strDate);

	void SetMode(bool bDay);
	void DrawReport();


	void PrintView(QWidget *parent);
	void Print();
	QPixmap   *mPixmap;

//public slots:
//	void printPreviewSlot(QPrinter* printerImage);
public:
	QImage    *image;
	bool     bShowBtn;
};