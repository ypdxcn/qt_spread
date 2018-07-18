#pragma once

#include <QDialog>
#include "ui_frmReportDlg.h"
#include "frmFramelessDialogBase.h"
#include "reportDrawWidget.h"
#include <QScrollArea>


class frmReportDlg : public /*QDialog*/frmFramelessDialogBase/*,  public QReportBase*/
{
	Q_OBJECT

public:
	frmReportDlg(QWidget *parent = Q_NULLPTR);
	~frmReportDlg();


	//void DrawReport();
	
	void ShowReportDlg(const bool &bMode, Rsp6007 &rsp6007, const string &strDate);

	//void WriteExcelModal(_Worksheet &_workSheet, int &iCurLine, int iTitleCount, QString *pcsColIDBeg, QString *pcsBigTitle, int *piAlign, const QString &csFinalNextCol, const QStringList &m_arrayValue, int iDataLineCount);
	//void WriteExcelAddEmptyLine(_Worksheet &_workSheet, ExcelEx &myExcelEx, Range &range, int &iCurLine);
	//void ExcelClear(_Application &_app, _Worksheet &_workSheet, ExcelEx &myExcelEx, int iCurLine);



public slots:
	void slotPrint();
	void slotOutExcel();
	void printPreviewSlot(QPrinter* printerImage);
	//virtual void paintEvent(QPaintEvent *event);
public:
	//bool	m_bPrint;

	//QPainter     m_dcMem;
	//QPixmap      *m_PixMem;


	QScrollArea *scrollArea;
	//QVBoxLayout* m_pSCVLayout;
private:
	Ui::frmReportDlg ui;
};
