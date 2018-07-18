#include "frmTradeStatusDlg.h"
#include "TraderCpMgr.h"
#include "HJConstant.h"
#include "HJGlobalFun.h"
#include "Macrodefs.h"
#include "IniFilesManager.h"
#include "Global.h"

frmTradeStatusDlg::frmTradeStatusDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	tableModel = new QStandardItemModel();
	tableModel_2 = new QStandardItemModel();

	tableModel->setColumnCount(2);
	int columnIndex = 0;
	tableModel->setHeaderData(columnIndex, Qt::Horizontal, QStringLiteral("合约名称"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("交易状态"));
	tableModel->setSortRole(1);
	ui.tableView->setModel(tableModel);

	tableModel_2->setColumnCount(2);
	columnIndex = 0;
	tableModel_2->setHeaderData(columnIndex, Qt::Horizontal, QStringLiteral("合约名称"));
	tableModel_2->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("交易状态"));
	tableModel_2->setSortRole(1);
	ui.tableView_2->setModel(tableModel_2);



	QIniFilesManager mag;
	vector<QString> vecUserNames;
	mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "DefaultColumnIndexs", false, vecUserNames);
	for (size_t i = 0; i < vecUserNames.size(); i++)
	{
		QString sType;
		const QString sID = vecUserNames.at(i);
		QString sState;

		// 获得该品种的市场类型
		for (size_t j = 0; j < g_TraderCpMgr.m_vProdCode.size(); j++)
		{
			if (g_TraderCpMgr.m_vProdCode.at(j).prod_code == sID)
			{
				sType = g_TraderCpMgr.m_vProdCode.at(j).market_id;
				break;
			}
		}

		// 获得交易状态
		sState = GetInsStateNameFromID(g_TraderCpMgr.GetInsStateID(sID));

		//
		if (sType == CONSTANT_B_MARKET_ID_SPOT)
		{
			tableModel->insertRow(0, new QStandardItem(sID));
			tableModel->setItem(0, 1, new QStandardItem(sState));
			/*m_listSpot.InsertItem(0, sID.c_str());
			m_listSpot.SetItemText(0, 1, sState);*/
		}
		else if (sType == CONSTANT_B_MARKET_ID_DEFER)
		{
		/*	m_listDefer.InsertItem(0, sID.c_str());
			m_listDefer.SetItemText(0, 1, sState);*/

			tableModel_2->insertRow(0, new QStandardItem(sID));
			tableModel_2->setItem(0, 1, new QStandardItem(sState));
		}
	}

	//setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

	ui.tableView->horizontalHeader()->setFixedHeight(32);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->verticalHeader()->setDefaultSectionSize(30);
	ui.tableView->setShowGrid(false);
	ui.tableView->setSortingEnabled(false);
	ui.tableView->setAlternatingRowColors(true);//隔行换色
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);


	ui.tableView_2->horizontalHeader()->setFixedHeight(32);
	ui.tableView_2->verticalHeader()->hide();
	ui.tableView_2->verticalHeader()->setDefaultSectionSize(30);
	ui.tableView_2->setShowGrid(false);
	ui.tableView_2->setSortingEnabled(false);
	ui.tableView_2->setAlternatingRowColors(true);//隔行换色
	ui.tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);


	//关闭消息
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotCal()));

	setFixedSize(651, 461);
	ui.widget->setGeometry(0, 0, 651, 361);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("合约品种详细行情"));

}

frmTradeStatusDlg::~frmTradeStatusDlg()
{
}



// 根据行情状态ID获取状态名称
QString frmTradeStatusDlg::GetInsStateNameFromID(const QString &sInsID)
{
	for (size_t i = 0; i < g_TraderCpMgr.m_vInstState2.size(); i++)
	{
		if (g_TraderCpMgr.m_vInstState2.at(i).code_id == sInsID)
		{
			return g_TraderCpMgr.m_vInstState2.at(i).code_desc;
		}
	}

	return "";
}


void frmTradeStatusDlg::slotCal()
{

	close();
}