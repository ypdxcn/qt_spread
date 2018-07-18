#include "reportDrawWidget.h"
#include <QPainter>
#include <QStyleOption>
#include "Global.h"
#include "HJGlobalFun.h"
#include <qapplication.h>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include  <QColor>
#include "frmReportDlg.h"


#pragma execution_character_set("utf-8")


reportDrawWidget::reportDrawWidget(QWidget* parent)
	:QWidget(parent)
{
	m_xRate = m_yRate = 1;
	m_xRate = (float)g_Global.m_ndpiX / DEFAULT_DPI_X;
	m_yRate = (float)g_Global.m_ndpiY / DEFAULT_DPI_Y;

	TABLE_LINE_HEIGHT = 20;   // 表格内的行的高度，横排的表格内的标题另外
	TABLE_TITLE_HEIGHT = 18;    // 表格的上面的标题的高度
	TABLE_WIDTH = 718;   // 表格的宽度
	LEFT_X = 20;    // 表格最左边的x轴点  56
	RIGHT_X = 738;   // 表格的最右边的x轴点，TABLE_WIDTH+LEFT_X
	TABLE_TO_TITLE_DIS = 8;     // 表格底端和下一个表格的标题的距离
	TABLE_BEGIN_Y = 82;    // 表格开始的在y轴上的位置（即第一个表格的第一行线的y值）

	TABLE_PAGE2_BEGIN_Y = 48;    // 第二页表格开始的y值

								 //标题数据
	TITLE_BEGIN_X = 232;
	TITLE_BEGIN_Y = 42;//kenny  2018-01-05
	LINE_Y = 48;      //标题下面那条横线的y值

					  //table1的数据（前三个表格）
	TABLE1_LINE_TITLE_COUNT = 3; //用于前三个表格，每个表格每一行有三个title
								 //#define TABLE1_TITLE_HEIGHT 114   //第一种表格标题的高度，比如客户信息，资金信息等这些字，表格内的标题的高度没有另外设置，理由很简单
								 //table2的数据（后三个表格）
	TABLE2_TITLE_HEIGHT = 35;  //第二种表格（即下面三个表格）标题行的高度

							   //客户信息表格
	CUS_INFO_COL_TITLE_WIDTH = 64;   //标题的宽度
	CUS_INFO_LINE_COUNT = 2;        //总共的行数
									//资金信息表格
	FUN_INFO_COL_TITLE_WIDTH = 102;   //标题的宽度
	FUN_INFO_LINE_COUNT = 5;        //总的行数
									//资金发生明细表格
	FUN_DETAIL_COL_TITLE_WIDTH = 80; //标题的宽度
	FUN_DETAIL_LINE_COUNT = 7;      //总的行数
									//后面的表格的数据
	STORE_INFO_COLUME_COUNT = 11;     //库存信息的列数
	CHICANG_INFO_COLUME_COUNT = 10;  //延期持仓信息的列数
	YINGKUI_INFO_COLUME_COUNT = 3;    //本期盈亏信息的列数
	MATCHFLOW_COLUME_COUNT = 8;   //成交流水的列数
	FUNINOUTFLOW_COLUME_COUNT = 6;  //出入金流水的列数
									//表格内文字信息
	TEXT_DISTANCE_X = 5;            //文字开始点在x轴上与表格的距离
	TEXT_DISTANCE_Y = 4;             //文字开始点在y轴上与表格的距离
	MY_FONT_SIZE = 13;            //实际的字体的大小，换行时用（多行文字时）     
								  /*************************************************************************************************************提货单打印相关配置*/
								  //标题数据
	TIHUO_TITLE_BEGIN_X = 128;
	TIHUO_TITLE_BEGIN_Y = 48;

	TITLE_TOP_DISTANCE = 16;
	TOP_BEGIN_X = 16;
	TOP_BEGIN_Y = 32;
	TOP_LINE_HEIGHT = 35;
	TOP_TITLE_WIDTH = 96;
	TOP_CONTENT_EIDTH = 192;

	TOP_MIDDLE_DISTANCE = 8;
	MIDDLE_BEGIN_X = 16;
	MIDDLE_BEGIN_Y = 32;
	MIDDLE_COLUMN_WIDTH = 272;
	MIDDLE_LINE_HEIGTH = 35;
	MIDDLE_TEXT_DISTANCE_Y = 6;             //文字开始点在y轴上与表格的距离

	MIDDLE_BOTTOM_DISTANCE = 8;
	BOTTOM_TITLE_WIDTH = 48;
	BOTTOM_CONTENT_EIDTH = 272;

	m_iMaxPage = 1;
	m_iType = E_DAY;

}
void reportDrawWidget::init()
{

	m_printInfo.m_nCurPage = 1;

	//m_bDay = true;

	m_Painter = new QPainter(this);
	m_Painter->setPen(QColor(0, 0, 0));
	// 报表字体
	QFont oldFont = qApp->font();
	QFont*newFont = new QFont("FixedSys", 12);
	m_Painter->setFont(*newFont);

	QFontMetrics fm = m_Painter->fontMetrics();
	int width = fm.width("---------1---------2---------3---------4---------5---------6---------7---------8---");

	m_Painter->setFont(oldFont);
	int cxOffset = (rect().width() - width) / 2;

	//判断页码的个数
	m_nHeight = 0;
	SetPage(&m_printInfo, m_nHeight);//必须是数据初始化之后才能正确计算画布的高度
	m_nHeight += 100;

	// 内存位图宽度
	m_nWidth = GetY((width + 100<rect().width()) ? rect().width() : (width + 100));
	//mPixmap = new QPixmap(m_nWidth, m_nHeight);

	setGeometry(0,0,m_nWidth, m_nHeight);

	image = new QImage(m_nWidth, m_nHeight, QImage::Format_RGB32);
	//m_Painter->fillRect(0, 0, m_nWidth, m_nHeight, Qt::white);
	//mPixmap = new QPixmap(m_nWidth, m_nHeight);
}


//为了能够使用样式表则需要为自定义Widget提供paintEvent事件
void reportDrawWidget::paintEvent(QPaintEvent *)
{
	
	QPainter imagePainter(image);
	imagePainter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
	imagePainter.initFrom(image);
	imagePainter.setRenderHint(QPainter::Antialiasing, true);
	imagePainter.eraseRect(rect());
	Draw(&imagePainter, &m_printInfo);
	imagePainter.end();

	QPainter  painter(this);
	painter.drawImage(0, 0, *image);

	// 设置画笔颜色
	//QPainter imagePainter(mPixmap);
	//imagePainter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
	//imagePainter.initFrom(mPixmap);
	//imagePainter.setRenderHint(QPainter::Antialiasing, true);
	//imagePainter.eraseRect(rect());
	//Draw(&imagePainter, &m_printInfo);
	//imagePainter.end();

	//QPainter  painter(this);
	//painter.drawPixmap(0, 0, *mPixmap);
}



// 是否显示确认日结单按钮
void reportDrawWidget::ShowConfirmBtn()
{
	bShowBtn = false;

	// 如果是日结单则判断是否要显示日结单确认按钮
	if (m_bDay)
	{
		Req2201 req; //请求报文体
		req.oper_flag = 1;
		req.acct_no = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
		req.affirm_date = m_strDate;

		Rsp2201 rsp; //应答报文体
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "2201"))
		{
			//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
		}
		else
		{
			if (g_Global.m_strSysState == CONSTANT_SYSTEM_STAT_DZFINISHED)
			{
				if (CHJGlobalFun::qstr2str(g_Global.m_strExchDate) >= m_strDate && rsp.affirm_date < m_strDate)
				{
					bShowBtn = true;
				}
			}
			else
			{
				if (CHJGlobalFun::qstr2str(g_Global.m_strExchDate) > m_strDate && rsp.affirm_date < m_strDate)
				{
					bShowBtn = true;
				}
			}
		}
	}



}



// 格式化日结单日期1
QString reportDrawWidget::FormatDate(QString strValue)
{
	int len = strValue.length();
	if (len != 8)//如果不是标准格式的字符串，不做处理
	{
		return strValue;
	}
	else
	{
		return strValue.left(4) + ("年") + strValue.mid(4, 2) + ("月") + strValue.mid(6) + ("日");
	}
}
// 格式化日结单日期2
QString reportDrawWidget::FormatDate()
{
	QString strValue = CHJGlobalFun::str2qstr(m_strDate);

	int len = strValue.length();
	if (len != 8)//如果不是标准格式的字符串，不做处理
	{
		return strValue;
	}
	else
	{
		return strValue.left(4) + ("年") + strValue.mid(4, 2) + ("月") + strValue.mid(6) + ("日");
	}
}
// 格式化月结单的日期
QString reportDrawWidget::FormatDate2()
{
	QString strDate = CHJGlobalFun::str2qstr(m_strDate);

	// 仅在长度符合规范的时候做处理
	if (strDate.length() >= 6)
	{
		// 获取年份和两位的月份
		QString strYear, strMonth;

		strYear = strDate.left(4);
		strMonth = strDate.mid(4);
		if (strMonth.length() == 1)
		{
			// 格式化月份
			strMonth = "0" + strMonth;
		}
		strDate = QString("%1年%2月").arg(strYear).arg(strMonth);
		//strDate.sprintf("%s年%s月", strYear, strMonth);
	}

	return strDate;
}



// 初始化对话框
void reportDrawWidget::ShowReportDlg(const bool &bMode, Rsp6007 &rsp6007, const string &strDate)
{
	// 设置对话框模式（日结单还是月结单）
	SetMode(bMode);
	// 保存日期
	m_strDate = strDate;
	// 保存数据
	IniAllData(rsp6007);
	
	//20187-1-5
	init();
	// 创建报表视图
	//CreateView();
	//DrawReport();
	// 	// 设置报表的title
	// 	SetReportViewTitle(GetReportTitle());
	m_strTitle = GetReportTitle();
	// 显示日结单确认按钮
	ShowConfirmBtn();

	//SetWindowPos(NULL, 300, 150, 780, 500, SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE); //SetWindowPos 用的是相对与电脑屏蔽的比较
	show();//触发重新绘制

}


// 获取报表和打印阅览的标题
QString reportDrawWidget::GetReportTitle()
{
	QString strTitle;
	if (m_bDay)
	{
		strTitle = FormatDate(CHJGlobalFun::str2qstr(m_strDate)) + "客户日结单";
	}
	else
	{
		strTitle = FormatDate2() + "客户月结单";
	}

	return strTitle;
}


//// 设置对话框的模式，是日结单还是月结单
void reportDrawWidget::SetMode(bool bDay)
{
	m_bDay = bDay;

	if (m_bDay) // 如果是日结单窗口
	{
		//SetWindowText("日结单查询");
		setWindowTitle(("日结单提醒"));
	}
	else
	{
		//SetWindowText("月结单查询");
		setWindowTitle(("月结单提醒"));
	}
}



void reportDrawWidget::DrawReport()
{
	//Draw(m_Painter, &m_printInfo);
}

//打印预览
void reportDrawWidget::PrintView(QWidget *parent)
{
	QPrinter printer(QPrinter::HighResolution);
	//自定义纸张大小  
	printer.setPageSize(QPrinter::Custom);
	printer.setPaperSize(QSizeF(height(), width()),	QPrinter::Point);
	QPrintPreviewDialog preview(&printer, this);
	preview.setMinimumSize(1000, 600);

	//为什么这个信号槽，不响应【2018-1-5】？？
	//this改为parent，就能够触发了，查了下，触发的信号槽，好像都必须继承自QDlalog[2018-1-6]
	//connect(&preview, SIGNAL(paintRequested(QPrinter*)), parent,SLOT(printPreviewSlot(QPrinter*)));
	connect(&preview, &QPrintPreviewDialog::paintRequested, (frmReportDlg *)parent,&frmReportDlg::printPreviewSlot);


	preview.exec();//paintRequested(QPrinter *printer)是系统提供的，当preview.exec()执行时该信号被触发
}



//打印[暂时不用，打印预览已经包含了打印]
void reportDrawWidget::Print()
{
	QPrinter print(QPrinter::HighResolution);
	QPrintDialog  printDialog(&print, this);

	if (printDialog.exec())
	{
		QPainter  painter(&print);
		QRect rect = painter.viewport();
		QSize  size = image->size();
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(image->rect());
		painter.drawImage(0, 0, *image);

	}

}



void reportDrawWidget::OutToExcel()
{

	QString sPath = QString("%1Trade\\settle.xls").arg(g_Global.GetSystemPath());
	QString csSheetName = (m_bDay == true) ? "日结单" : "月结单";
	QExcel *excel = new QExcel(sPath);
	excel->selectSheet("Sheet1");
	excel->RenameSheet(csSheetName);
	int iCurLine;
	excel->setCellString(2, 2, m_strTitle);
	
	if (m_vecRisk.size() != 0)
	{
		const int iCount = 6;
		QString csColumns[iCount] = { "E", "U", "AM", "E", "U", "AM" };
		int        iLines[iCount] = { 7, 7, 7, 8, 8, 8 };
		for (int i = 0; i < iCount; i++)
		{
			excel->setCellString(iLines[i], excel->Column_StrToInt(csColumns[i]), m_vecRisk.at(i).strContent);
		}
	}
	else
	{
		ExcelClear( *excel, 9);
		return;
	}

	if (m_vecFund.size() != 0)
	{
		const int iCount = 14;
		QString csColumns[iCount] = { "H", "Y", "AQ", "H", "Y", "AQ", "H", "Y", "AQ", "H", "Y", "AQ", "H", "Y" };
		int        iLines[iCount] = { 11, 11, 11, 12, 12, 12, 13, 13, 13 ,14, 14, 14, 15, 15 };
		for (int i = 0; i < iCount; i++)
		{
			excel->setCellString(iLines[i], excel->Column_StrToInt(csColumns[i]), m_vecFund.at(i).strContent);
		}

		QString csColumnsName[iCount] = { "C", "Q", "AH", "C", "Q", "AH", "C", "Q", "AH", "C", "Q", "AH", "C", "Q" };
		for (int i = 0; i < iCount; i++)
		{

			excel->setCellString(iLines[i], excel->Column_StrToInt(csColumnsName[i]), m_vecFund.at(i).strTitle);
	
		}
	}
	else
	{
		//ExcelClear(_app, _workSheet, myExcelEx, 16);
		ExcelClear(*excel, 16);
		return;
	}

	if (m_vecFundDetail.size() != 0)
	{
		const int iCount = 21;
		QString csColumns[iCount] = { "G", "X", "AO", "G", "X", "AO", "G", "X", "AO", "G", "X", "AO", "G", "X", "AO", "G", "X", "AO", "G", "X", "AO" };
		int        iLines[iCount] = { 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 24,24, 24 };
		for (int i = 0; i < iCount; i++)
		{
			excel->setCellString(iLines[i], excel->Column_StrToInt(csColumns[i]), m_vecFundDetail.at(i).strContent);
		}

		//range.SetItem(25, myExcelEx.Column_StrToInt("G"), m_strGaCha);
		excel->setCellString(25, excel->Column_StrToInt("G"), m_strGaCha);
	}
	else
	{
		ExcelClear(*excel, 16);
		return;
	}

	iCurLine = 26;
	if (m_iStorage > 0)
	{
		WriteExcelAddEmptyLine(*excel, iCurLine);
		QString csColIDBeg[11] = { "B", "D", "J", "N", "S", "W", "AA", "AD", "AJ", "AN", "AR" };
		QString csBigTitle[2] = { "库存信息", "单位：克" };
		QString csFinalNextCol = "AY";
		//QString csTitle[11] = {"交割品种",  "上期库存","可用库存",    "待提库存",    "质押库存",  "冻结库存","本期买入","本期卖出","本期存入","本期提出","库存总量"};
		int iAlign[11] = { Qt::AlignLeft, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight };
		WriteExcelModal(*excel, iCurLine, 11, csColIDBeg, csBigTitle, iAlign, csFinalNextCol, m_arrayStorage, m_iStorage);
	}

	if (m_iDefer > 0)
	{
		WriteExcelAddEmptyLine(*excel, iCurLine);
		QString csColIDBeg[10] = { "B", "F", "K", "R", "V", "Z", "AC", "AI", "AP", "AT" };
		QString csBigTitle[2] = { "延期持仓信息", "数量:手  金额:元" };
		QString csFinalNextCol = "BA";
		//QString csTitle[10] = {"合约代码", "持仓方向",  "上期持仓","本期开仓","本期平仓","本期交收","中立仓建仓",  "本期持仓量","开仓均价",  "持仓均价"};
		int iAlign[10] = { Qt::AlignLeft, Qt::AlignLeft, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight };
		WriteExcelModal(*excel, iCurLine, 10, csColIDBeg, csBigTitle, iAlign, csFinalNextCol, m_arrayDefer, m_iDefer);
	}

	if (m_iSurplus > 0)
	{
		WriteExcelAddEmptyLine(*excel, iCurLine);
		QString csColIDBeg[3] = { "B", "L", "AK" };
		QString csBigTitle[2] = { "本期盈亏信息", "单位:元" };
		QString csFinalNextCol = "AZ";
		//QString csTitle[3] = {"合约代码", "本期结算价",  "本期盈亏"};
		int iAlign[3] = { Qt::AlignLeft, Qt::AlignRight, Qt::AlignRight };
		WriteExcelModal(*excel, iCurLine, 3, csColIDBeg, csBigTitle, iAlign, csFinalNextCol, m_arraySurplus, m_iSurplus);

		// mod by Jerry Lee, 修改excel报表多输出一行“本期结算价”的问题, 2013-3-7
		//range.SetItem(iCurLine+1, myExcelEx.Column_StrToInt(csColIDBeg[1]), m_arraySurplus[1]);
	}

	if (m_iMatchFlow > 0)
	{
		WriteExcelAddEmptyLine(*excel, iCurLine);
		QString csColIDBeg[8] = { "B", "I", "M", "T", "AB", "AF", "AL", "AS" };
		QString csBigTitle[2] = { "成交流水", "数量:手  金额:元" };
		QString csFinalNextCol = "AX";
		//QString csTitle[8] = {"成交编号","报单号",  "合约代码", "交易方向", "成交价格",   "成交数量", "成交金额", "手续费"};
		int iAlign[8] = { Qt::AlignLeft, Qt::AlignLeft, Qt::AlignLeft, Qt::AlignLeft, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight, Qt::AlignRight };
		WriteExcelModal(*excel, iCurLine, 8, csColIDBeg, csBigTitle, iAlign, csFinalNextCol, m_arrayMatchFlow, m_iMatchFlow);
	}

	if (m_iFundInOutFlow > 0)
	{
		WriteExcelAddEmptyLine(*excel, iCurLine);
		QString csColIDBeg[6] = { "B", "G", "N", "U", "AA", "AJ" };
		QString csBigTitle[2] = { "出入金流水", "" };
		QString csFinalNextCol = "AP";
		//QString csTitle[6] = {"交易日期", "交易流水号","存取方向",  "发生金额","银行日期","银行流水号"};
		int iAlign[6] = { Qt::AlignLeft, Qt::AlignLeft, Qt::AlignLeft, Qt::AlignRight, Qt::AlignLeft, Qt::AlignRight };
		WriteExcelModal(*excel, iCurLine, 6, csColIDBeg, csBigTitle, iAlign, csFinalNextCol, m_arrayFundInOutFlow, m_iFundInOutFlow);
	}

	excel->show();

}


void reportDrawWidget::WriteExcelModal(QExcel &myExcelEx, int &iCurLine, int iTitleCount, QString *pcsColIDBeg, QString *pcsBigTitle, int *piAlign, const QString &csFinalNextCol, const QStringList &m_arrayValue, int iDataLineCount)
{
	int iLine; // 记录当前操作excel的行
	int i; // 临时变量
	int j;
	QString cColLineBeg, cColLineEnd; // 记录excel的字符串格式的行列

	iLine = iCurLine;
	// 合并”库存信息“
	cColLineBeg = "b"; cColLineEnd = "v";
	cColLineBeg = myExcelEx.FormatColLine(cColLineBeg, iLine);
	cColLineEnd = myExcelEx.FormatColLine(cColLineEnd, iLine);

	myExcelEx.mergeCells(cColLineBeg + ":" + cColLineEnd);
	//range = _workSheet.GetRange(_variant_t(cColLineBeg), _variant_t(cColLineEnd));
	//range.Merge(_variant_t(0));  //合并单元格
	//range.SetRowHeight(_variant_t(20));
	myExcelEx.setRowHeight(cColLineBeg, 20);
	// 合并"单位:克"
	cColLineBeg = "w"; cColLineEnd = "AX";
	cColLineBeg = myExcelEx.FormatColLine(cColLineBeg, iLine);
	cColLineEnd = myExcelEx.FormatColLine(cColLineEnd, iLine);
	myExcelEx.mergeCells(cColLineBeg + ":" + cColLineEnd);

	//range = _workSheet.GetRange(_variant_t(cColLineBeg), _variant_t(cColLineEnd));
	//range.Merge(_variant_t(0));  //合并单元格
	//range.SetHorizontalAlignment(_variant_t(EXCEL_ALIGN_RIGHT)); // 设置右边对齐
	//range.SetRowHeight(_variant_t(20));
	myExcelEx.setRowHeight(cColLineBeg, 20);
	myExcelEx.setFontAlignment(cColLineBeg, -4152);
	// 处理title的列的合并
	iLine = iCurLine + 1;
	for (j = 0; j <= iDataLineCount; j++)
	{
		for (i = 0; i < iTitleCount; i++)
		{
			// 获取合并单元格的开始列
			cColLineBeg = myExcelEx.FormatColLine(*(pcsColIDBeg + i), iLine);
			// 获取合并单元格的结束列
			QString sTemp;
			sTemp = (i == iTitleCount - 1) ? csFinalNextCol : *(pcsColIDBeg + i + 1);
			cColLineEnd = myExcelEx.GeForColumn(sTemp);
			cColLineEnd = myExcelEx.FormatColLine(cColLineEnd, iLine);
			// 合并单元格
			//range = _workSheet.GetRange(_variant_t(cColLineBeg), _variant_t(cColLineEnd));
			//range.Merge(_variant_t(0));  //合并单元格
			
			myExcelEx.mergeCells(cColLineBeg + ":" + cColLineEnd);
			int val = *(piAlign + i) == 1 ? -4131 : -4152;
			myExcelEx.setFontAlignment(cColLineBeg, j == 0 ? -4108 : val);
			// 设置对齐方式
			//range.SetHorizontalAlignment(_variant_t(j == 0 ? EXCEL_ALIGN_CENTER : *(piAlign + i)));

			if (j == 0)
			{
				myExcelEx.setCellFontBold(cColLineEnd, true); // 设置为粗体
				//myExcelEx.setFontBold(cColLineEnd, sTemp,true);
			}
			else
			{
				myExcelEx.setCellFontSize(cColLineEnd, 9);
			}

			// 则设置标题字体（大小，粗体）
			//font.AttachDispatch(range.GetFont());
			//if (font.m_lpDispatch)
			//{
			//	if (j == 0)
			//	{
			//	font.SetBold(_variant_t(true)); // 设置为粗体
			//	}
			//	else
			//	{
			//	font.SetSize(_variant_t(9));

			//	}
			//}

			//if (j == 0)
			//{
			//	// 设置背景色
			//	Interior m_nterior = range.GetInterior();
			//	m_nterior.SetColor(_variant_t(RGB(192, 192, 192)));
			//}

			if (j == 0)
			{
				myExcelEx.setBackColor(cColLineEnd, QColor(192, 192, 192,255)); 
															  
			}

			//weight = 3  ,color
			myExcelEx.setBorder(cColLineEnd,3, QColor(0, 0, 0));

			//// 边框
			//range.BorderAround(COleVariant((short)1), 3, 1, _variant_t(RGB(0, 0, 0)));
		}

		// 设置行高
		j == 0 ? myExcelEx.setRowHeight(cColLineBeg, 28.25) : myExcelEx.setRowHeight(cColLineBeg, 20.25);
		

		iLine++;
	}

	// 恢复到默认的全部单元格，否则下面的修改会货不对版
	//range.AttachDispatch(_workSheet.GetCells());

	// 设置整个表格的标题
	iLine = iCurLine;
	//range.SetItem(iLine, 2, *pcsBigTitle);
	//range.SetItem(iLine, myExcelEx.CharToInt('W'), *(pcsBigTitle + 1));

	myExcelEx.setCellString(iLine, 2, *pcsBigTitle);
	myExcelEx.setCellString(iLine, myExcelEx.CharToInt('W'), *(pcsBigTitle + 1));


	// 处理Title的值
	iLine = iCurLine + 1;
	for (i = 0; i < iTitleCount; i++)
	{
		//range.SetItem(iLine, myExcelEx.Column_StrToInt(*(pcsColIDBeg+i)), *(pcsTitle+i));
		//range.SetItem(iLine, myExcelEx.Column_StrToInt(*(pcsColIDBeg + i)), m_arrayValue.GetAt(i));

		myExcelEx.setCellString(iLine, myExcelEx.Column_StrToInt(*(pcsColIDBeg + i)), m_arrayValue.at(i));

	}

	// 添加值
	iLine = iCurLine + 2; // 移到数据行
	for (i = 0; i < iDataLineCount; i++)
	{
		for (j = 0; j < iTitleCount; j++)
		{
		//range.SetItem(iLine, myExcelEx.Column_StrToInt(*(pcsColIDBeg + j)), m_arrayValue.GetAt((i + 1)*iTitleCount + j));
			myExcelEx.setCellString(iLine, myExcelEx.Column_StrToInt(*(pcsColIDBeg + j)), m_arrayValue.at((i + 1)*iTitleCount + j));

		}
		iLine++;
	}

	iCurLine = iLine--;
}




void reportDrawWidget::WriteExcelAddEmptyLine(QExcel &myExcelEx,  int &iCurLine)
{
//QString cColLine = myExcelEx.FormatColLine("b", iCurLine);
//range = _workSheet.GetRange(_variant_t(cColLine), _variant_t(cColLine));
//range.SetRowHeight(_variant_t(10));
//?????????2018-1-12
myExcelEx.setCellString(iCurLine,QString("b").toInt(), "");
myExcelEx.setRowHeight(iCurLine, 10);

iCurLine++;
}

// 清空iCurLine行下面的内容
void reportDrawWidget::ExcelClear(QExcel &myExcelEx, int iCurLine)
{

	for (int j = iCurLine; j < 26; j++)
	{
		for (int i = 0; i< 52; i++)
		{
			myExcelEx.clearCell(j, i);
		}
	}

	myExcelEx.save();

}


