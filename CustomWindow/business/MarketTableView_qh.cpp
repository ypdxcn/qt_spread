#include "MarketTableView_qh.h"

//#include "LxCustomSortModel.h"
#include <QHBoxLayout>
#include <time.h>

//#include "lock/Owner.hpp"
//#include "plugin/SessionCheck.h"
//#include "StrategySystem/SessionManager.h"
//#include "StrategySystem/FirstSession.h"
//#include "objects/ReportData.h"
//#include "MainFrm/LxSysThemeMgr.h"
#include "QuoteStandardItemModel.h"
#include "TableItemDelegate.h"
#include "macrodefs.h"
#include "BroadcastPacketAndStruct.h"
#include "HJGlobalFun.h"
#include "HJConstant.h"
#include "Global.h"
#include "IniFilesManager.h"
#include "app.h"
#include "TraderCpMgr.h"
#include "QParamEvent.h"
#include "Logger.h"

#define TABLE_ROW_HIGHT 30
#define LIST_INI_SPLITER "∧"


#pragma execution_character_set("utf-8")

MarketTableView_qh::MarketTableView_qh(QSplitter *spliter,QAbstractItemModel *model, QWidget *parent)   :QTableView(parent)
{
   
    init();
    GetHeadNames();
	
	setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\quotetable.css"));


    updateTimer = new QTimer(this);
    m_interval = new QTimer(this);
    m_interval->setTimerType(Qt::PreciseTimer);

    connect(updateTimer, &QTimer::timeout, this, &MarketTableView_qh::responseTimer);
    connect(m_interval, &QTimer::timeout, this,  &MarketTableView_qh::ClickedInterval);

    updateTimer->start(1500);
    m_bResetData = true;
	m_bDoubleClick = 0;
   
    m_nSrcollValue = 0;

    m_nSoftCol  = -1;
    m_dir       = SOFT_NO;
    m_bResetSel = true;
    m_bSoft     = false;
    m_pMenu     = nullptr;
    setItemDelegate(new TableItemDelegate(this));
    setSelectionMode(QAbstractItemView::SingleSelection);
    //CreateAndInitMenu();
    setColorFromTheme();

	connect(selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
		parent, SLOT(showPosiOperationDetails(const QModelIndex &, const QModelIndex &)));
}

MarketTableView_qh::~MarketTableView_qh()
{

}

void MarketTableView_qh::Connect()
{

}



void MarketTableView_qh::setMainWinId(QWidget * id)
{
	m_mainWndid = id;
}



void MarketTableView_qh::init()
{
    m_ClickRow = -1;
	//setMinimumHeight(100);
	setBaseSize(width(), 350);
    horizontalHeader()->setFixedHeight(32);//设置表头的高度
	verticalHeader()->hide();
	verticalHeader()->setDefaultSectionSize(TABLE_ROW_HIGHT);
    setShowGrid(false);////设置不显示格子线
	setFrameShape(QFrame::NoFrame); //设置无边框
    setSelectionBehavior(SelectRows);//设置选择行为时每次选择一行
	setSortingEnabled(false);
	setAlternatingRowColors(true);//隔行换色
	setSelectionMode(QAbstractItemView::SingleSelection);
	setEditTriggers(QAbstractItemView::NoEditTriggers);

	m_pmodel = new QuoteStandardItemModel();

	InitColumn();
	InitRowData();
	setModel(m_pmodel);

    setSortingEnabled(true);  /// 排序 以后要重写
    connect(horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(LastSortIndex(int, Qt::SortOrder)));
    connect(this, &QAbstractItemView::clicked, this, &MarketTableView_qh::RowClickSelected);

	connect(selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(showMarketChangeDetails(const QModelIndex &, const QModelIndex &)));


    m_nResize = 0;
}


void MarketTableView_qh::showMarketChangeDetails(const QModelIndex &current, const QModelIndex &previous)
{
	RowClickSelected(current);
}

void  MarketTableView_qh::InitColumn()
{
	// 读取用户选择列的位置，保存到内存
	//m_vecColumnIndexs.clear();

	//重建表格
	m_pmodel->removeRows(0, m_pmodel->rowCount());
	m_pmodel->removeColumns(0, m_pmodel->columnCount());

	QVector<int> m_vecColumnIndexs;
	QVector<QString> vecAllNames;
	QVector<int>     vecAllWidths;
	QVector<int>     vecAllAligns;

	QString  qsPath = g_Global.GetListIniPath("QuotationQH");
	QString buf;
	// 读取用户选择列的ID，并且分割到vector
	App::GetPriProfileString(qsPath, "Info", "UserColumnIndexs", buf);

	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);

	// 读取全部字段名称，并分割到vector
	App::GetPriProfileString(qsPath, "Info", "AllColumnNames", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllNames);

	// 读取全部字段宽度，并分割到vector
	App::GetPriProfileString(qsPath, "Info", "AllColumnWidths", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllWidths);

	// 读取全部字段对其方式，并分割到vector
	App::GetPriProfileString(qsPath, "Info", "AllColumnAligns", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllAligns);

	QVector<QString> vecAllColumnIDs;
	QVector<QString> vecUserColumnIDs;
	App::GetPriProfileString(qsPath, "Info", "AllColumnIDs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllColumnIDs);

	((QuoteStandardItemModel *)m_pmodel)->setColumnCount(m_vecColumnIndexs.size());

	// 加载标题信息到list
	for (int i = 0; i < m_vecColumnIndexs.size(); i++)
	{
		int iPos = m_vecColumnIndexs.at(i);

		if (i == 0) // 将第一列的宽度加上一定量，解决第一列宽度比设置窄的问题
		{
			m_pmodel->setHeaderData(i, Qt::Horizontal, vecAllNames.at(iPos));//, vecAllAligns.at(iPos), vecAllWidths.at(iPos) + 3);
			setColumnWidth(i, vecAllWidths.at(iPos) + 10);
		}
		else
		{
			m_pmodel->setHeaderData(i, Qt::Horizontal, vecAllNames.at(iPos));//, vecAllAligns.at(iPos), vecAllWidths.at(iPos));
			setColumnWidth(i, vecAllWidths.at(iPos));
		}

		vecUserColumnIDs.push_back(vecAllColumnIDs.at(iPos));
	}

	// 初始化map
	//IniMap(vecUserColumnIDs);


}

//void  MarketTableView_qh::UpdateQuotationImp(QUOTATION * quote)
//{
//	QString buf;
//
//	App::GetPriProfileString(g_Global.GetListIniPath("Quotation"), "Info", "UserColumnIndexs", buf);
//	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);
//
//	// 加载默认的品种到行情列表
//	QIniFilesManager mag;
//	vector<QString> vecUserNames;
//	mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);
//
//	QuoteStandardItemModel* tableModel = (QuoteStandardItemModel *)m_pmodel;
//
//	for (size_t i = 0; i < vecUserNames.size(); i++)
//	{
//		QString csValue;
//		QString sInsID = vecUserNames.at(i);
//		tableModel->insertRow(i);
//		tableModel->setItem2(i, 0, QString::number(i + 1));//序号
//														   //tableModel->setItem2(i, 1, sInsID);//合约编码
//														   //tableModel->setItem2(i, 2, g_TraderCpMgr.GetProdCodeName(sInsID));//显示合约名称
//														   // 加载该品种的行情
//		const QUOTATION& qt = g_TraderCpMgr.m_QMapQuotation[CHJGlobalFun::qstr2str(vecUserNames.at(i))];
//		for (size_t j = 0; j < m_vecColumnIndexs.size(); j++)
//		{
//			int colPos = m_vecColumnIndexs.at(j);
//			if (colPos > 0)
//			{
//				tableModel->setItem2(i, m_vecColumnIndexs.at(j), GetQuotationValue(vecUserNames.at(i), qt, i, colPos));//序号
//			}
//		}
//
//	}
//
//}
void  MarketTableView_qh::InitRowData()
{
	QString buf;
	// 读取行情列表
	QVector<int> vecUserColumnIndexs;
	App::GetPriProfileString(g_Global.GetListIniPath("QuotationQH"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecUserColumnIndexs);

	buf.clear();
	QVector<int> vecAllColumnWidths;
	App::GetPriProfileString(g_Global.GetListIniPath("QuotationQH"), "Info", "AllColumnWidths", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllColumnWidths);


	//读取合约列表
	QVector<QString> vecAllNames;
	App::GetPriProfileString(g_Global.GetListIniPath("InsIDQH"), "Info", "AllColumnNames", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllNames);

	QVector<int> vecUserInsIndexs;
	App::GetPriProfileString(g_Global.GetListIniPath("InsIDQH"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecUserInsIndexs);


	QuoteStandardItemModel* tableModel = (QuoteStandardItemModel *)m_pmodel;
	
	for (size_t i = 0; i < vecUserInsIndexs.size(); i++)
	{
		tableModel->insertRow(i);

		// 加载该品种的行情
		QString  inst = vecAllNames.at(vecUserInsIndexs.at(i));
		const QUOTATION& qt = g_TraderCpMgr.m_QHMapQuotation[inst];
		for (size_t j = 0; j < vecUserColumnIndexs.size(); j++)
		{
			int col = vecUserColumnIndexs.at(j);

			tableModel->setItem2(i, col, GetQuotationValue(inst, qt, i,col));//序号
			this->setColumnWidth(col, vecAllColumnWidths.at(col));

		}

	}


}


QString MarketTableView_qh::GetQuotationValue(const QString &lpszInsId, const QUOTATION &quotation, const int rowIndex,const int &iPos)
{
	QString strValue = "";
	//索引值，见Quotation.ini的DefaultColumnIndexs
	switch (iPos)
	{
	case 0:
		strValue = QString::number(rowIndex + 1);
		break;
	case 1:
		strValue = lpszInsId;
		break;
	case 2:
		strValue = FormatPrice(quotation.m_Bid[0].m_uiPrice); // 买价1
		break;
	case 3:
		strValue = QString::number(quotation.m_Bid[0].m_uiVol); // 买量1
		break;
	case 4:
		strValue = FormatPrice(quotation.m_Ask[0].m_uiPrice); // 卖价1
		break;
	case 5:
		strValue = QString::number(quotation.m_Ask[0].m_uiVol); // 卖量1
		break;
	case 6:
		strValue ="T1-T2";  //计算公式    T1-T2  或者 T1/T2
		break;
	
	}

	return strValue;
}


//制作更新
void MarketTableView_qh::updateQuotation(QUOTATION *quote)
{
	//qDebug() << "Date:" << QString::fromStdString(quote->instID);
	m_vecColumnIndexs.clear();
	QString buf;

	//读取行情配置
	App::GetPriProfileString(g_Global.GetListIniPath("Quotation"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, m_vecColumnIndexs);

	// 读取合约配置
	//QIniFilesManager mag;
	//vector<QString> vecAllNames;
	//mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "AllColumnNames", true, vecAllNames);

	//vector<QString> vecUserIndexs;
	//mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserIndexs);


	QVector<QString> vecAllNames;
	App::GetPriProfileString(g_Global.GetListIniPath("InsIDQH"), "Info", "AllColumnNames", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecAllNames);

	QVector<int> vecUserIndexs;
	App::GetPriProfileString(g_Global.GetListIniPath("InsIDQH"), "Info", "UserColumnIndexs", buf);
	CHJGlobalFun::SplitStrToVector(buf, LIST_INI_SPLITER, vecUserIndexs);



	QuoteStandardItemModel* tableModel = (QuoteStandardItemModel *)m_pmodel;

	for (size_t i = 0; i < vecUserIndexs.size(); i++)
	{
		QString sInsId = vecAllNames.at(vecUserIndexs.at(i));
		//找到要更新的Row
		if (sInsId.contains(CHJGlobalFun::str2qstr(quote->instID)))
		{
			for (size_t j = 0; j < m_vecColumnIndexs.size(); j++)
			{
				int colPos = m_vecColumnIndexs.at(j);

				QString ttt = GetQuotationValue(vecAllNames.at(vecUserIndexs.at(i)), *quote, i, colPos);

				tableModel->setItem2(i, m_vecColumnIndexs.at(j), ttt);//序号

			}
			return;
		}

	}
}

/*
//更新实时行情值【自己管理的数据】
void MarketTableView_qh::updateQuotation(QUOTATION *quote)
{
	///////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////
	//加载字段配置
	QString strPath = QString("%1QueryConfig\\Quotation.ini").arg(g_Global.GetSystemPath());

	QString strValue;
	QString strAligns;
	int i;
	QVector<QString> vecAllNames;
	QVector<int> vecColumnIndexs;

	QString buf;

	// 读取全部字段名称，并分割到vector
	App::GetPriProfileString(strPath, "Info", "AllColumnNames", buf);
	strValue = buf;
	CHJGlobalFun::SplitStrToVector(strValue, QUERY_INI_SPLITER, vecAllNames);

	// 读取位置字段，并且分割到vector
	App::GetPriProfileString(strPath, "Info", "UserColumnIndexs", buf);
	strValue = buf;
	CHJGlobalFun::SplitStrToVector(strValue, QUERY_INI_SPLITER, vecColumnIndexs);

	// 加载当前显示项
	for (i = 0; i < vecColumnIndexs.size(); i++)
	{
		//添加进界面
		QListWidgetItem * item = new QListWidgetItem();
		item->setCheckState(Qt::Checked);
		item->setText(vecAllNames.at(vecColumnIndexs.at(i)));
		//ui.listWidget->addItem(item);

		//ui.listWidget->setItemWidget(item, list[list.count() - 1]);
		//设置自定义值
		item->setData(Qt::UserRole, vecColumnIndexs.at(i));

	}

	int rowIndex = 0;
	if (quote->instID.compare("Ag(T+D)") == 0)
	{
		rowIndex = 0;
	}
	else if (quote->instID.compare("Au(T+D)") == 0)
	{
		rowIndex = 1;
	}
	else if (quote->instID.compare("mAu(T+D)") == 0)
	{
		rowIndex = 2;
	}
	else if (quote->instID.compare("Au99.99") == 0)
	{
		rowIndex = 3;
	}
	else if (quote->instID.compare("Au99.95") == 0)
	{
		rowIndex = 4;
	}
	else if (quote->instID.compare("Au100g") == 0)
	{
		rowIndex = 5;
	}
	else if (quote->instID.compare("Au(T+N1)") == 0)
	{
		rowIndex = 6;
	}
	else if (quote->instID.compare("Au(T+N2)") == 0)
	{
		rowIndex = 7;
	}
	else if (quote->instID.compare("Au99.5") == 0)
	{
		rowIndex = 8;
	}

	QuoteStandardItemModel* p = (QuoteStandardItemModel *)m_pmodel;
	float oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 2, QModelIndex())).toString().toFloat();
	float val = (float)quote->m_uiLast / 100;
/*
	if (oldVal != val)
	{
		/*p->setItem2(rowIndex, 2, CHJGlobalFun::FormatFloatQuote(QString::number((float)quote->m_uiLast / 100,'f',2)));
		if(val > oldVal)
		p->item(rowIndex, 2)->setForeground(QBrush(QColor("#ff786e")));
		else
		p->item(rowIndex, 2)->setForeground(QBrush(QColor("#6cad3c")));
	}
*/
	//p->setItem2(rowIndex, 3, QString::number(quote->m_uiUpDown / g_Global.m_dQTFactor, 'f', 2));
	//p->setItem2(rowIndex, 4, QString::number(quote->m_dUpDownRate, 'f', 2));

	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 5, QModelIndex())).toString().toFloat();
	//val = (float)(quote->m_uiHigh - quote->m_uiLow) / quote->m_uilastClose;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 5, CHJGlobalFun::FormatFloatQuote(QString::number(val)));
	//	if (val > oldVal)
	//		p->item(rowIndex, 5)->setForeground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 5)->setForeground(QBrush(QColor("#6cad3c")));
	//}

	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 6, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_Bid[0].m_uiPrice / g_Global.m_dQTFactor;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 6, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 6)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 6)->setBackground(QBrush(QColor("#6cad3c")));
	//}

	//p->setItem2(rowIndex, 7, QString::number(quote->m_Bid[0].m_uiVol));
	//p->setItem2(rowIndex, 9, QString::number(quote->m_Ask[0].m_uiVol));



	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 8, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_Ask[0].m_uiPrice / g_Global.m_dQTFactor;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 8, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 8)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 8)->setBackground(QBrush(QColor("#6cad3c")));
	//}


	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 10, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_uiLastSettle / g_Global.m_dQTFactor;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 10, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 10)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 10)->setBackground(QBrush(QColor("#6cad3c")));
	//}

	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 11, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_uilastClose / g_Global.m_dQTFactor;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 11, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 11)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 11)->setBackground(QBrush(QColor("#6cad3c")));
	//}



	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 12, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_uiOpenPrice / g_Global.m_dQTFactor;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 12, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 12)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 12)->setBackground(QBrush(QColor("#6cad3c")));
	//}

	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 13, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_uiHigh / 100;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 13, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 13)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 13)->setBackground(QBrush(QColor("#6cad3c")));
	//}


	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 14, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_uiLow / g_Global.m_dQTFactor;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 14, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 14)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 14)->setBackground(QBrush(QColor("#6cad3c")));
	//}


	//oldVal = m_pmodel->data(m_pmodel->index(rowIndex, 15, QModelIndex())).toString().toFloat();
	//val = (float)quote->m_uiClose / g_Global.m_dQTFactor;
	//if (oldVal != val)
	//{
	//	p->setItem2(rowIndex, 15, CHJGlobalFun::FormatFloatQuote(QString::number(val)));

	//	if (val > oldVal)
	//		p->item(rowIndex, 15)->setBackground(QBrush(QColor("#ff786e")));
	//	else
	//		p->item(rowIndex, 15)->setBackground(QBrush(QColor("#6cad3c")));
	//}

//
//	this->update();
//
//}


QString MarketTableView_qh::FormatPrice(unsigned int uiPrice)
{
	double dPrice = uiPrice / g_Global.m_dQTFactor;
	return CHJGlobalFun::FormatFloat(QString::number(dPrice));
}




void MarketTableView_qh::SelMenu(QAction *pAction)
{
    QList<QAction*> pActionlist = Option->menu()->actions();
    int col = 0;
    for (QList<QAction*>::iterator it = pActionlist.begin(); it != pActionlist.end(); ++it)
    {
        if (*it == pAction)
        {
            break;
        }
        ++col;
    }
    setColumnHidden(col, !pAction->isChecked());
    pAction->isChecked();
}


//当选，和下单板，持仓/库存/资金，2各板块联动
void  MarketTableView_qh::RowClickSelected(const QModelIndex &index)
{
    QString ContractName = m_pmodel->data(m_pmodel->index(index.row(), 1, QModelIndex())).toString();
	char  tmp[20] = {0};
	strcpy(tmp, ContractName.toStdString().c_str());

	DeferInstState state;
	state.instID = ContractName.toStdString();


	//SendMessage((HWND)m_mainWndid, WM_SelectTableRow, WPARAM(&state), 0);
	QParamEvent * msg = new  QParamEvent(WM_SelectTableRow);
	msg->setWParam(&state);
	QApplication::sendEvent(m_mainWndid, msg);// , WPARAM(&state), 0);

    //QString ExchangeName = m_pmodel->data(m_pmodel->index(index.row(), 1, QModelIndex())).toString();
    //QStringList ContractList = ContractName.split('(',QString::SkipEmptyParts);

    //if (2 == ContractList.size())
    //{
    //    ContractName = ContractList.at(0);  ///changed by chengp 1 changeinto 0
    //    ContractName.remove(')');
    //}
    //ContractName += '.';
    //if (ExchangeName == _MY_TEXT("中金所"))
    //{
    //    ContractName += "CFFEX";
    //}
    //else if (ExchangeName == _MY_TEXT("上期所"))
    //{
    //    ContractName += "SHFE";
    //}
    //else if (ExchangeName == _MY_TEXT("郑商所"))
    //{
    //    ContractName += "CZCE";
    //}
    //else if (ExchangeName == _MY_TEXT("大商所"))
    //{
    //    ContractName += "DCE";
    //}
    //emit    ContractLocked(ContractName);
    //emit   StateChanged(true);
}

void MarketTableView_qh::RowRightClicked(const QModelIndex &index)
{
    QString ContractName = m_pmodel->data(m_pmodel->index(index.row(), 3, QModelIndex())).toString();
    QString ExchangeName = m_pmodel->data(m_pmodel->index(index.row(), 1, QModelIndex())).toString();
    QStringList ContractList = ContractName.split('(',QString::SkipEmptyParts);

    if (2 == ContractList.size())
    {
        ContractName = ContractList.at(1);
        ContractName.remove(')');
    }
    ContractName += '.';
  /*  if (ExchangeName == _MY_TEXT("中金所"))
    {
        ContractName += "CFFEX";
    }
    else if (ExchangeName == _MY_TEXT("上期所"))
    {
        ContractName += "SHFE";
    }
    else if (ExchangeName == _MY_TEXT("郑商所"))
    {
        ContractName += "CZCE";
    }
    else if (ExchangeName == _MY_TEXT("大商所"))
    {
        ContractName += "DCE";
    }*/
    m_Contract = ContractName;
    if (m_pMenu)
        m_pMenu->exec(QCursor::pos());
}

void MarketTableView_qh::HeaderSectionMoved(int /*l*/, int o, int n)
{
    if (n == 32)
    {
        horizontalHeader()->moveSection(n, o);
    }
}

void MarketTableView_qh::HeaderActived(const QModelIndex &/*index*/)
{
    emit   StateChanged(true);
}

void MarketTableView_qh::OnMenu(QAction *pAction)
{
    QList<QAction*> actionList = m_pMenu->actions();
    switch (actionList.indexOf(pAction))
    {
    case 2:
        emit    ContractSwitch(m_Contract);
        break;
    default:
        break;
    }
}

void MarketTableView_qh::GetHeadNames()
{
    QStandardItemModel* pmodel = dynamic_cast<QStandardItemModel*>(m_pmodel/*model()*/);
    int nCnt = pmodel->columnCount();
    for (int col = 0; col < nCnt - 1; ++col)
    {
		QStandardItem *ps = pmodel->horizontalHeaderItem(col);
		if (nullptr != ps)
		{
			m_NameList.push_back(ps->text());
		}
       
    }
}

void MarketTableView_qh::SetTableColor()
{
    if (model() && this)
    {
        int nCol = model()->columnCount();
        int nRow = model()->rowCount();
        QStandardItemModel* p_model = dynamic_cast<QStandardItemModel*>(m_pmodel/*model()*/);
        for (int col = 0; col < nCol; ++col)
        {
            for (int row = 0; row < nRow; ++row)
            {
                if (col == 3 || col == 4 || col == 5 || col == 6 || col == 7 || col == 11 || col == 18 || col == 27)
                {
                    p_model->item(row, col)->setForeground(QBrush(QColor(141, 194, 31)));//绿
                }
                else if (col == 8 || col == 26)
                {
                    p_model->item(row, col)->setForeground(QBrush(QColor(233, 85, 19)));//红
                }
            }

        }
    }
}

void MarketTableView_qh::LastSortIndex(int index, Qt::SortOrder order)
{
    m_LastSortIndex.index = index;
    m_LastSortIndex.order = order;
    if (order == Qt::AscendingOrder)
    {
        m_dir = SORT_DOWN;
    }
    else
    {
        m_dir = SORT_UP;
    }
    m_nSoftCol = index;
    m_bSoft = true;
    m_bResetData = true;
    this->horizontalHeader()->setSortIndicatorShown(true);

    // 切换数据后排序
    CloseTimer();
    {
        //SESSION_CHECK sc = CFirstSession::GetInstance()->GetSessionCheck();
        //CSessionOwner o(sc.sessionid);
        //CSession *pSession = o.GetSession();
        //if (!pSession)
        //    return;
        //int nSize = m_pVecSecurity->size();
        //m_vecAll.clear();
        //TransData(m_pVecSecurity, m_vecAll, 0, nSize - 1, false, true);			// 获得所有的数据
        SortMarketData(m_vecAll, m_nSoftCol, m_dir);							// 对目前的数据进行所有的排序
        m_bResetData = false;
    }
    StartTimer();
}

void MarketTableView_qh::responseTimer()
{
	//还原所有背景色
	//6列，8列，10到15列
	QuoteStandardItemModel* p = (QuoteStandardItemModel *)m_pmodel;
	//kenny  2018-1-15  该处报错  ，线屏蔽
	//for (int i = 0; i < 9; ++i)
	//{
	//	p->item(i, 6)->setBackground(QBrush(QColor("#181a1d")));
	//	p->item(i, 8)->setBackground(QBrush(QColor("#181a1d")));
	//	p->item(i, 10)->setBackground(QBrush(QColor("#181a1d")));
	//	p->item(i, 11)->setBackground(QBrush(QColor("#181a1d")));
	//	p->item(i, 12)->setBackground(QBrush(QColor("#181a1d")));
	//	p->item(i, 13)->setBackground(QBrush(QColor("#181a1d")));
	//	p->item(i, 14)->setBackground(QBrush(QColor("#181a1d")));
	//	p->item(i, 15)->setBackground(QBrush(QColor("#181a1d")));
	//}
	this->update();

    //SESSION_CHECK sc = CFirstSession::GetInstance()->GetSessionCheck();
    //CSessionOwner o(sc.sessionid);
    //CSession *pSession = o.GetSession();
    //if (!pSession)
    //    return;

    //int nSize = m_pVecSecurity->size();
    //int row = m_pmodel->rowCount();


    //int nStart = m_nSrcollValue;									// 起始位置
    //int nRowCount = this->rect().height() / TABLE_ROW_HIGHT - 1;	// 行数
    //nStart -= RESERVE_ROW_COUNT;
    //if (nStart < 0)
    //    nStart = 0;
    //int nEnd = m_nSrcollValue + nRowCount + RESERVE_ROW_COUNT;
    //nEnd = qMin(nEnd, nSize - 1);									// 比较小的值

    //std::vector<TableRow> vecTmp;

  
    //TransData(m_pVecSecurity, vecTmp, nStart, nEnd, m_bSoft);
    //// 赋值当前显示数据
    ////	m_vecData.clear();
    ////	m_vecData.swap(vecTmp);
    //// 重新排序号
    //if (m_bResetSel)
    //{
    //    nSize = vecTmp.size();
    //    for (int i = 0; i < nSize; i++)
    //    {
    //        if (m_nSrcollValue - RESERVE_ROW_COUNT >= 0)
    //            vecTmp[i].arrVar[0] = m_nSrcollValue + i + 1 - RESERVE_ROW_COUNT;
    //        else
    //            vecTmp[i].arrVar[0] = i + 1;
    //    }
    //}
    //// 插入需要的新行
    //nSize = m_pVecSecurity->size();
    //if (row > nSize)
    //{
    //    m_pmodel->removeRows(0, row - nSize);
    //}
    //else
    //{
    //    m_pmodel->insertRows(0, nSize - row);
    //}
    //// 设置显示数据
    //dynamic_cast<QuoteStandardItemModel*>(m_pmodel)->SetMarketData(vecTmp, m_nSrcollValue);
    //this->update();
    //m_nResize++;
    //if(m_nResize < 10)
    //{
    //    this->resizeColumnsToContents();
    //}
}

void MarketTableView_qh::ClickedInterval()
{
    m_bDoubleClick = 0; //先初始化
    if (m_interval->isActive())
        m_interval->stop();
    RowClickSelected(m_CurIndex);
    m_CurIndex = QModelIndex();
}

void MarketTableView_qh::onColorChange()
{
  /*  LxSysThemeMgr::ColorScheme cs = LxSysThemeMgr::GetInstance()->getCurrentUseColorScheme();
    QColor color = LxSysThemeMgr::findColor(cs.General,_MY_TEXT("窗口标签"));
    QStringList areaList;
    QStringList nameList;
    QStringList valueList;
    areaList<<"QHeaderView::section \n{";
    nameList<<"background-color:";
    valueList<<color.name();

    color = LxSysThemeMgr::findColor(cs.General,_MY_TEXT("窗口底色1"));
    areaList<<"QTableView\n{";
    nameList<<"background-color:";
    valueList<<color.name();

    color = LxSysThemeMgr::findColor(cs.General,_MY_TEXT("窗口底色2"));
    areaList<<"QTableView\n{";
    nameList<<"alternate-background-color:";
    valueList<<color.name();

    color = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("报价抬头"));
    areaList<<"QHeaderView::section \n{";
    nameList<<"color:";
    valueList<<color.name();

    QColor up = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表报价上涨"));
    QColor down = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表报价下跌"));
    QColor none = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表报价字符"));
    QColor normal = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表普通字段"));
    setModelColor(up,down,none,normal);

    up = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("选中行"));
    down = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("选中行边框"));

    QString strTemp = "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "+down.name()+", stop: 0.15 "+up.name()+" ,stop: 0.85 "+up.name()+",stop: 1 "+down.name()+")";
    areaList<<"QTableView\n{";
    nameList<<"selection-background-color:";
    valueList<<strTemp;

    SET_CSSFILE( _RES_STYLES_PATH+"table.css",areaList,nameList,valueList);
    SET_WIDGET_STYLE(this, _RES_STYLES_PATH + _MY_TEXT("table.css"));*/
}

void MarketTableView_qh::CloseTimer()
{
    if (updateTimer->isActive())
        updateTimer->stop();
}

void MarketTableView_qh::StartTimer()
{
    if (!updateTimer->isActive())
        updateTimer->start();
}

void MarketTableView_qh::focusInEvent(QFocusEvent *event)
{
}

void MarketTableView_qh::focusOutEvent(QFocusEvent *event)
{
    //	CloseTimer();
}

void MarketTableView_qh::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
    QModelIndex index = indexAt(event->pos());
    if (index.row() < 0 && index.column() < 0)
        return;                     // 如果点击表格空白处直接返回
    if (event->button() == Qt::RightButton)
        emit rightClicked(index);
}

//void MarketTableView_qh::SetMarket(std::vector<TSecurity*> *pVecSecurity)
//{
//    m_pVecSecurity = pVecSecurity;
//}

void MarketTableView_qh::SortMarketData(std::vector<TableRow> &vecData, int nCol, SORT_DIRECTOR dir)
{
    if (nCol > 31)
        return;

    int nRowCount = vecData.size();

    std::vector<QVariant> vecTmp;
    std::vector<int> vecInvalidSel;
    std::vector<TableRow> vecUse;
    // 复制要排序的那一列
    for (int row = 0; row < nRowCount; row++)
    {
        //if (vecData[row].arrVar[nCol].toString() != _MY_TEXT("--"))
        //{
        //    vecTmp.push_back(vecData[row].arrVar[nCol]);
        //    vecUse.push_back(vecData[row]);
        //}
        //else
        //    vecInvalidSel.push_back(row);			// 记录不需要排序的
    }
    QVariant var1, varMin;
    bool bSwap = false;
    // 选择排序
    int nCount = vecUse.size();
    for (int i = 0; i < nCount - 1; i++)
    {
        int nMin = i;

        for (int j = i + 1; j < nCount; j++)
        {
            var1 = vecTmp.at(j);
            varMin = vecTmp.at(nMin);
            switch (var1.type())
            {
            case QVariant::Double:
            {
                if (dir == SORT_DOWN)
                {
                    if (var1.toDouble() < varMin.toDouble())
                        bSwap = true;
                }
                else
                {
                    if (var1.toDouble() > varMin.toDouble())
                        bSwap = true;
                }
                break;
            }
            case QVariant::Int:
            {
                if (dir == SORT_DOWN)
                {
                    if (var1.toInt() < varMin.toInt())
                        bSwap = true;
                }
                else
                {
                    if (var1.toInt() > varMin.toInt())
                        bSwap = true;
                }
                break;
            }
            case QVariant::LongLong:
            {
                if (dir == SORT_DOWN)
                {
                    if (var1.toLongLong() < varMin.toLongLong())
                        bSwap = true;
                }
                else
                {
                    if (var1.toLongLong() > varMin.toLongLong())
                        bSwap = true;
                }
                break;
            }
            case QVariant::UInt:
            {
                if (dir == SORT_DOWN)
                {
                    if (var1.toUInt() < varMin.toUInt())
                        bSwap = true;
                }
                else
                {
                    if (var1.toUInt() > varMin.toUInt())
                        bSwap = true;
                }
                break;
            }
            case QVariant::String:
            {
                break;
            }
            default:
                break;
            }
            if (bSwap)
            {
                QVariant tmp;
                tmp = vecTmp[j];
                vecTmp[j] = vecTmp[nMin];
                vecTmp[nMin] = tmp;
                SwapRow(vecUse, j, nMin);
                bSwap = false;
            }
        }
    }
    unsigned int uSize = (unsigned int)vecInvalidSel.size();
    for (int i = 0; i < uSize; i++)
    {
        int nSel = vecInvalidSel[i];
        vecUse.push_back(vecData[nSel]);
    }
    vecData.clear();
    vecData.swap(vecUse);
}

void MarketTableView_qh::SwapRow(std::vector<TableRow> &vecData, int nSrc, int nDest)
{
    std::swap(vecData[nSrc], vecData[nDest]);
}

void MarketTableView_qh::ClearData()
{
    m_bResetData = true;
    m_bResetSel = true;
    m_nSoftCol = -1;			// 切换板块，默认不排序
    m_dir = SOFT_NO;
    m_bSoft = false;
    m_vecAll.clear();
    this->horizontalHeader()->setSortIndicatorShown(false);
    m_nResize = 0;
}

//垂直滑条变动
void MarketTableView_qh::verticalScrollbarValueChanged(int value)
{
    m_nSrcollValue = value;
    m_bResetSel = true;
	m_nResize = 5;
}

//水平滑条变动
void  MarketTableView_qh::horizontalScrollbarValueChanged(int value)
{
    m_nHSrcollValue = 32 - value;
	m_nResize = 5;
}

void MarketTableView_qh::horizontalScrollbarAction(int action)
{
    qDebug()<<action;
}


void MarketTableView_qh::setModelColor(const QColor &up, const QColor &down, const QColor &none,const QColor &normal)
{
    QuoteStandardItemModel *model = qobject_cast<QuoteStandardItemModel *>(m_pmodel);
    model->setUpDownColor(up,down,none,normal);
}

void MarketTableView_qh::setColorFromTheme()
{
    //LxSysThemeMgr::ColorScheme cs = LxSysThemeMgr::GetInstance()->getCurrentUseColorScheme();
    /*QColor up = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表报价上涨"));
    QColor down = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表报价下跌"));
    QColor none = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表报价字符"));
    QColor normal = LxSysThemeMgr::findColor(cs.Market,_MY_TEXT("列表普通字段"));*/
	setModelColor(QColor("#ff786e"),QColor("#6cad3c"), QColor("#e8e8e8"), QColor("#e8e8e8"));
}


//双击行
void MarketTableView_qh::slotRowDoubleClicked(const QModelIndex& index)
{
	//通过Model获取一行  

	QModelIndex idx = currentIndex();
	if (idx.isValid())
	{
		//<span style = "white-space: pre;">    < / span>//也可以通过自定义的Model中获取  
		//	QSqlRecord record = model->record(index.row());
		QString ExchangeName = m_pmodel->data(m_pmodel->index(index.row(), 1, QModelIndex())).toString();
		//QString value = m_pmodel->->value("xxxxxxx").toString();
		//.................
	}
}






//更新实时行情值【自己管理的数据】
void MarketTableView_qh::updateCell(int rowIndex,int columnIndex,QString& val)
{
	QuoteStandardItemModel* p = (QuoteStandardItemModel *)m_pmodel;
	QString oldVal = m_pmodel->data(m_pmodel->index(rowIndex, columnIndex, QModelIndex())).toString();

	if (oldVal != val)
	{
		p->setItem2(rowIndex, columnIndex, val);
	}
}
