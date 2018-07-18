#include "frmDefersettlementQuoteDlg.h"
#include "ui_frmDefersettlementQuoteDlg.h"
#include "TraderCpMgr.h"
#include "HJConstant.h"
#include "HJGlobalFun.h"
#include "Macrodefs.h"
#include "QParamEvent.h"


frmDefersettlementQuoteDlg::frmDefersettlementQuoteDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui = new Ui::frmDefersettlementQuoteDlg();
	ui->setupUi(this);


	//设置报单表
	tableModel = new QStandardItemModel();

	//setWidgetStyleFromQssFile(ui->tableView, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

	//TabView_trans->setBaseSize(TabTrans->width(), TabTrans->height());
	ui->tableView->horizontalHeader()->setFixedHeight(32);
	ui->tableView->verticalHeader()->hide();
	ui->tableView->verticalHeader()->setDefaultSectionSize(30);
	ui->tableView->setShowGrid(false);
	ui->tableView->setSortingEnabled(false);
	ui->tableView->setAlternatingRowColors(true);//隔行换色
	ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);


	////设置行
	tableModel->setColumnCount(7);
	int columnIndex = 0;
	tableModel->setHeaderData(columnIndex, Qt::Horizontal, QStringLiteral(" "));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("合约代码"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("合约名称"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("收金数量"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("交金数量"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("中立仓收金"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("中立仓交金"));


	tableModel->setSortRole(1);
	ui->tableView->setModel(tableModel);
	ui->tableView->setColumnWidth(0, 30);
	ui->tableView->setColumnWidth(2, 150);

	//关闭消息
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotCal()));

	
	int iListPos = 0;
	// 遍历所有的品种
	for (size_t i = 0; i < g_TraderCpMgr.m_vProdCode.size(); i++)
	{
		// 如果是延期，则加载
		if (g_TraderCpMgr.m_vProdCode[i].market_id == CONSTANT_B_MARKET_ID_DEFER)
		{
			// 获得品种代码
			QString strpProd_code = g_TraderCpMgr.m_vProdCode[i].prod_code;

			// 在交收行情中查找该品种的位置
			size_t j;
			for (j = 0; j < g_TraderCpMgr.m_vecDDQuotation.size(); j++)
			{
				if (g_TraderCpMgr.m_vecDDQuotation.at(j).instID == CHJGlobalFun::qstr2str(strpProd_code))
				{
					break;
				}
			}

			// 如果存在
			if (j < g_TraderCpMgr.m_vecDDQuotation.size())
			{
				SetListOneLineData(g_TraderCpMgr.m_vecDDQuotation.at(j), iListPos,true);
			}
			else
			{
				// 如果不存在
				DeferDeliveryQuotation body;
				body.instID = CHJGlobalFun::qstr2str(strpProd_code);
				body.askLot = 0;
				body.bidLot = 0;
				body.midAskLot = 0;
				body.midBidLot = 0;
				SetListOneLineData(body, iListPos,true);
			}

			iListPos++;
		}
	}


	// 订阅广播消息
	g_TraderCpMgr.Subscribe(E_ONRECVDEFERDELIVERYQUOTATION, this);

	setFixedSize(811, 504);
	ui->widget->setGeometry(0, 0, 811, 404);
	setContent(ui->widget);
	setWindowTitle(QStringLiteral("延期市场交收行情"));
}

frmDefersettlementQuoteDlg::~frmDefersettlementQuoteDlg()
{
	delete ui;
}



void frmDefersettlementQuoteDlg::slotCal()
{

	g_TraderCpMgr.Unsubscribe(E_ONRECVDEFERDELIVERYQUOTATION, this);

	close();
}


//自定义消息
void  frmDefersettlementQuoteDlg::customEvent(QEvent *e)
{
	QParamEvent *msg = static_cast<QParamEvent *>(e);
	if (msg->type() == MSG_RECV_DEFERDELIVERY_QUOTATION)//行情广播
	{
		OnDeferDeliveryQuotation(*((int *)msg->wparam()));
	}

}



void frmDefersettlementQuoteDlg::OnDeferDeliveryQuotation(int wParam)
{
	int iPos = (int)wParam;
	DeferDeliveryQuotation quotation = g_TraderCpMgr.m_vecDDQuotation.at(iPos);

	bool bExist = false;
	int i;
	for (i = 0; i < tableModel->rowCount(); i++)
	{
		
		if (tableModel->item(i, 0)->text() == CHJGlobalFun::str2qstr( quotation.instID) )
		{
			bExist = true;
			break;
		}
	}

	if (bExist)
	{
		SetListOneLineData(quotation, i, false);
	}
	else
	{
		SetListOneLineData(quotation, tableModel->rowCount(),true);
	}

	return ;

}



void frmDefersettlementQuoteDlg::SetListOneLineData(const DeferDeliveryQuotation &stQuotation, int iLine, bool bAdd )
{
	QString strValue;
	if (bAdd)
	{
		tableModel->insertRow(iLine);
	}
	tableModel->setItem(iLine, 0, new QStandardItem(QString::number(iLine+1)));
	// 插入合约代码
	tableModel->setItem(iLine, 1, new QStandardItem(CHJGlobalFun::str2qstr(stQuotation.instID)));

	// 插入合约名称
	tableModel->setItem(iLine, 2, new QStandardItem(g_TraderCpMgr.GetProdCodeName(CHJGlobalFun::str2qstr(stQuotation.instID))));

	// 插入收金数量
	tableModel->setItem(iLine, 3, new QStandardItem(QString::number(stQuotation.bidLot)));

	// 插入交金数量
	tableModel->setItem(iLine, 4, new QStandardItem(QString::number(stQuotation.askLot)));

	// 插入中立仓收金
	tableModel->setItem(iLine, 5, new QStandardItem(QString::number(stQuotation.midBidLot)));

	// 插入中立仓交金
	tableModel->setItem(iLine, 6, new QStandardItem(QString::number(stQuotation.midAskLot)));

}

