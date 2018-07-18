#include "mainWindow.h"
#include "macrodefs.h"
#include "TraderCpMgr.h"
#include "Def.h"

#include "frmDeferCloseCalDlg.h"
#include "frmDefersettlementQuoteDlg.h"
#include "frmTradeStatusDlg.h"
#include "frmFundInOutDlg.h"
#include "frmCurrentCustomOwnDlg.h"
#include "frmDaySettlementDlg.h"
#include "frmMonthSettlementDlg.h"
#include "frmStopOutDialog.h"
#include "frmMemberNoticeDlg.h"
#include "frmWTSearchDialog.h"
#include "frmMatchSearchDlg.h"
#include "frmDeferPosiSearchDlg.h"
#include "frmFunInOutSearchDlg.h"
#include "frmHistoryWTSearchDialog.h"
#include "frmHistoryMatchSearchDlg.h"
#include "frmHistoryDeferPosiSearchDlg.h"
#include "frmStoreSearchDialog.h"
#include "frmCapitalChangeSearchDialog.h"
#include "frmCapitalSearchDialog.h"
#include "frmFeeDetailSearchDialog.h"
#include "frmGetGoodsDlg.h"
#include "frmChangePswdDlg.h"
#include "frmUnlockDlg.h"
#include "frmChangeAccountDlg.h"
#include "QtTest.h"

#include "TradeHandler.h"
#include "Global.h"
#include "app.h"

#pragma execution_character_set("utf-8")

//响应菜单逻辑【显示资金信息】
void mainWindow::ShowAccountBar()
{
	if (accountBarWidget->isHidden())
	{
		m_layout->insertWidget(1,accountBarWidget);
		accountBarWidget->show();
		showCapital = true;
	}
	else
	{
		m_layout->removeWidget(accountBarWidget);
		accountBarWidget->hide();
		showCapital = false;
	}

}

//改回到传统布局
void mainWindow::changToOldLayout()
{
	m_layout->removeWidget(accountBarWidget);
	accountBarWidget->hide();

	m_layout->removeWidget(ui.splitter);
	ui.splitter->hide();


	m_layout->removeWidget(m_pSplitterOrderAndOrderReturn);
	m_pSplitterOrderAndOrderReturn->hide();

	m_layout->removeWidget(m_pSplitterLocalDBAndTransReturn);
	m_pSplitterLocalDBAndTransReturn->hide();

	m_layout->removeWidget(m_pSplitterAllTithoutLabel);
	m_pSplitterAllTithoutLabel->hide();

	m_layout->removeWidget(ui.line_status);
	ui.line_status->hide();

	m_layout->removeWidget(statusbar);
	statusbar->hide();



	TabRight->addTab(orderReturnWidget, "报单流水");
	TabRight->addTab(preOrderReturnWidget, "预埋单");

	int index = 1;
	if (showCapital)
	{
		m_layout->insertWidget(index++, accountBarWidget);
		accountBarWidget->show();
	}

	m_pSplitterOrderAndOrderReturn->addWidget(TabOrderBoard);
	m_pSplitterOrderAndOrderReturn->addWidget(TabRight);


	tabWidget_own->addTab(posiWidget, "持仓");
	tabWidget_own->addTab(storeWidget, "库存");
	tabWidget_own->addTab(capitalWidget, "资金");
	m_pSplitterLocalDBAndTransReturn->addWidget(tabWidget_own);


	TabTrans->addTab(TabView_trans, "成交流水");
	m_pSplitterLocalDBAndTransReturn->addWidget(TabTrans);

	//加入报价
	m_pSplitterAllTithoutLabel->addWidget(ui.splitter);
	ui.splitter->show();
	//加入下单板和行情报价
	m_pSplitterAllTithoutLabel->addWidget(m_pSplitterOrderAndOrderReturn);
	m_pSplitterOrderAndOrderReturn->show();
	m_pSplitterAllTithoutLabel->addWidget(m_pSplitterLocalDBAndTransReturn);
	m_pSplitterLocalDBAndTransReturn->show();

	m_layout->insertWidget(index++, m_pSplitterAllTithoutLabel);

	//调整初始化高度
	m_pSplitterAllTithoutLabel->setStretchFactor(0, 20);
	m_pSplitterAllTithoutLabel->setStretchFactor(1, 1);
	m_pSplitterAllTithoutLabel->setStretchFactor(2, 15);

	m_layout->addWidget(m_pSplitterAllTithoutLabel);
	m_pSplitterAllTithoutLabel->show();

	m_layout->insertWidget(index++, ui.line_status);
	m_layout->insertWidget(index++, statusbar);

	ui.line_status->show();
	statusbar->show();

}


void mainWindow::changToClassicLayout()
{
	m_layout->removeWidget(accountBarWidget);
	accountBarWidget->hide();
	m_layout->removeWidget(ui.splitter);
	ui.splitter->hide();
	m_layout->removeWidget(m_pSplitterOrderAndOrderReturn);
	m_pSplitterOrderAndOrderReturn->hide();
	m_layout->removeWidget(m_pSplitterLocalDBAndTransReturn);
	m_pSplitterLocalDBAndTransReturn->hide();
	m_layout->removeWidget(m_pSplitterAllTithoutLabel);
	m_pSplitterAllTithoutLabel->hide();
	m_layout->removeWidget(ui.line_status);
	ui.line_status->hide();
	m_layout->removeWidget(statusbar);
	statusbar->hide();
	m_layout->removeWidget(orderReturnWidget);
	m_layout->removeWidget(preOrderReturnWidget);

	//合并tab页面，统一下按钮的布局
        //setWidgetStyleFromQssFile(TabRight, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\positable.css"));

#ifdef  _WIN32

        setWidgetStyleFromQssFile(TabRight, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\positable.css"));
#else
        setWidgetStyleFromQssFile(TabRight, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/positable.css"));
#endif

	TabRight->addTab(posiWidget, "持仓");
	TabRight->addTab(storeWidget, "库存");
	TabRight->addTab(capitalWidget, "资金");
	TabRight->addTab(TabView_trans, "成交流水");
    TabRight->addTab(orderReturnWidget, "报单流水");
	TabRight->addTab(preOrderReturnWidget, "预埋单");

	int index = 1;
	if (showCapital)
	{
		m_layout->insertWidget(index++, accountBarWidget);
		accountBarWidget->show();
	}


	m_pSplitterAllTithoutLabel->addWidget(ui.splitter);
	ui.splitter->show();
	m_pSplitterAllTithoutLabel->addWidget(m_pSplitterOrderAndOrderReturn);
	m_pSplitterOrderAndOrderReturn->show();

	m_layout->insertWidget(index++, m_pSplitterAllTithoutLabel);
	m_pSplitterAllTithoutLabel->show();
	//调整初始化高度
	m_pSplitterAllTithoutLabel->setStretchFactor(0, 30);
	m_pSplitterAllTithoutLabel->setStretchFactor(1, 1);
	m_pSplitterAllTithoutLabel->setStretchFactor(2, 15);

	m_layout->insertWidget(index++, ui.line_status);
	m_layout->insertWidget(index++, statusbar);

	ui.line_status->show();
	statusbar->show();

}

//展现深度行情
void mainWindow::slotDeepQuote()
{
	//show非模态展示，open是模态，exec是ApplicationModal
	deepQuoteDlg->show();
	showDeepQuote = true;
	ShowDeepQuoteUI();

	quoteButton->setGeometry(240, 6, 75, 30);
}

//关闭十档行情
void mainWindow::slotCloseDeepQuoteDlg()
{
	deepQuoteDlg->hide();
	showDeepQuote = false;
	ShowNormalQuoteUI();

	quoteButton->setGeometry(400, 6, 75, 30);
}

//账户管理
void mainWindow::slotChangeAccount()
{
	//弹出账户管理功能
	frmChangeAccountDlg dlg;
	int respone = dlg.exec();
	if(respone == QDialog::Accepted)
		RefreshMultiAccount();
}

void mainWindow::RefreshMultiAccount()
{
	ui.comboBox_account->clear();
	QStringList list;
	for (size_t i = 0; i < App::accMgr.size(); i++)
	{
		list << App::accMgr.value(i).alias;
	}
	ui.comboBox_account->addItems(list);
}

void mainWindow::InitTableContextMenu()
{
	//右键菜单信号槽---持仓区域
	Tabview_1->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(Tabview_1, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_contextmenuPosi(const QPoint&)));
	connect(Tabview_1, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_contextmenuStore(const QPoint&)));
	connect(Tabview_1, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_contextmenuCapital(const QPoint&)));

	menuPosi = new QMenu(Tabview_1);

	m_actionPosiOne   = new QAction(menuPosi);
	m_actionPosiTwo   = new QAction(menuPosi);
	m_actionPosiThree = new QAction(menuPosi);

	m_actionPosiOne->setText("反手平仓");
	m_actionPosiTwo->setText("平仓试算");
	m_actionPosiThree->setText("刷新持仓信息");

	menuPosi->addAction(m_actionPosiOne);
	menuPosi->addAction(m_actionPosiTwo);
	menuPosi->addAction(m_actionPosiThree);
	connect(m_actionPosiOne, &QAction::triggered, this, &mainWindow::actionPosiOneSlot);
	connect(m_actionPosiTwo, &QAction::triggered, this, &mainWindow::actionPosiTwoSlot);
	connect(m_actionPosiThree, &QAction::triggered, this, &mainWindow::actionPosiThreeSlot);

	//[库存]右键
	Tabview_2->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(Tabview_2, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_contextmenuStore(const QPoint&)));

	menuStore = new QMenu(Tabview_2);

	m_actionStoreOne = new QAction(menuStore);
	m_actionStoreOne->setText("刷新库存信息");

	menuStore->addAction(m_actionStoreOne);
	connect(m_actionStoreOne, &QAction::triggered, this, &mainWindow::actionStoreOneSlot);


	//[资金]右键
	Tabview_3->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(Tabview_3, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_contextmenuCapital(const QPoint&)));

	menuCapital = new QMenu(Tabview_3);

	m_actionCapitalOne = new QAction(menuCapital);
	m_actionCapitalOne->setText("刷新资金信息");

	menuCapital->addAction(m_actionCapitalOne);
	connect(m_actionCapitalOne, &QAction::triggered, this, &mainWindow::actionCapitalOneSlot);

	//报单回报区域右键
	orderReturnTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(orderReturnTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_contextmenuOrderReturn(const QPoint&)));

	menuOrderReturn   = new QMenu(orderReturnTable);
	m_actionOrderOne  = new QAction(menuOrderReturn);

	m_actionOrderOne->setText("改单");

	menuOrderReturn->addAction(m_actionOrderOne);

	connect(m_actionOrderOne, &QAction::triggered, this, &mainWindow::actionOrderReturnOneSlot);
}


//////////////////////////////右键菜单逻辑////////////////////////////////////////////////////////////////////////

//弹出右键菜单
void mainWindow::show_contextmenuPosi(const QPoint& pos)
{
	QModelIndex index = Tabview_1->indexAt(pos);

	if (index.isValid())
	{
		menuPosi->exec(QCursor::pos());//在当前鼠标位置显示
	}
}

//反手平仓-逻辑处理
void mainWindow::actionPosiOneSlot()
{
	OnBnClickedButtonOpp();
}
//平仓试算-逻辑处理
void mainWindow::actionPosiTwoSlot()
{
	handle2OneSlot();
}
//刷新持仓信息-逻辑处理
void mainWindow::actionPosiThreeSlot()
{
	Req1020 req;
	req.qry_defer = gc_YesNo_Yes.toStdString();

	Rsp1020 rsp1020;
	if (g_TradeHandler.GetCustomInfo(rsp1020, req, e_TipsShowType_Fail) == 0)
	{
		RefreshPosi();
	}
}

//弹出库存右键菜单
void mainWindow::show_contextmenuStore(const QPoint& pos)
{
	QModelIndex index = Tabview_2->indexAt(pos);

	if (index.isValid())
	{
		menuStore->exec(QCursor::pos());//在当前鼠标位置显示
	}
}

//刷新库存信息-逻辑代码
void mainWindow::actionStoreOneSlot()
{
	Req1020 req;
	req.qry_defer = gc_YesNo_Yes.toStdString();

	Rsp1020 rsp1020;
	if (g_TradeHandler.GetCustomInfo(rsp1020, req, e_TipsShowType_Fail) == 0)
	{
		RefreshStore();
	}
}

//弹出库存右键菜单
void mainWindow::show_contextmenuCapital(const QPoint& pos)
{
	QModelIndex index = Tabview_3->indexAt(pos);

	if (index.isValid())
	{

		menuCapital->exec(QCursor::pos());//在当前鼠标位置显示
	}
}

//刷新资金信息-逻辑代码
void mainWindow::actionCapitalOneSlot()
{
	Req1020 req;
	req.qry_defer = gc_YesNo_Yes.toStdString();

	Rsp1020 rsp1020;
	if (g_TradeHandler.GetCustomInfo(rsp1020, req, e_TipsShowType_Fail) == 0)
	{
		RefreshCapital();
	}
}


//回报的右键
void mainWindow::show_contextmenuOrderReturn(const QPoint& pos)
{
	QModelIndex index = orderReturnTable->indexAt(pos);

	if (index.isValid())
	{
		menuOrderReturn->exec(QCursor::pos());//在当前鼠标位置显示
	}
}

//报单流水-改单逻辑
void mainWindow::actionOrderReturnOneSlot()
{

}


/////////////////////////////end/右键菜单/////////////////////////////////////////




/////////////////////////////////start 菜单逻辑////////////////////////////////////////////
//解锁回报
void mainWindow::OnSuccess() 
{
	show();
}

void mainWindow::OnFailure() 
{
	close();
}

//锁屏
void mainWindow::handleOneSlot()
{
	m_uiLockCount = 0;
        g_Global.m_bLocked = 1;


	frmUnlockDlg dlg;
	dlg.SetListener(this);
	dlg.showMaximized();

	dlg.move((width() - dlg.width()) / 2, (height() - dlg.height()) / 2);

	dlg.exec();
}

//登录密码修改
void mainWindow::handleTwoSlot()
{
	frmChangePswdDlg dlg;
	dlg.exec();
}

//资金密码修改
void mainWindow::handleThreeSlot()
{
	frmChangePswdDlg dlg;
	dlg.setMode(false);
	dlg.exec();
}



//操作
//延期平仓试算
void mainWindow::handle2OneSlot()
{
	frmDeferCloseCalDlg dlg;
	dlg.exec();
}

//显示交收行情
void mainWindow::handle2TwoSlot()
{
	frmDefersettlementQuoteDlg dlg;
	dlg.exec();
}

//显示交易状态
void mainWindow::handle2ThreeSlot()
{
	frmTradeStatusDlg dlg;
	dlg.exec();
}

//出入金
void mainWindow::handle2FourSlot()
{
	frmFundInOutDlg dlg;
	dlg.exec();

}

//提货
void mainWindow::handle2FiveSlot()
{
	frmGetGoodsDlg dlg;
	dlg.exec();
}



//查询///////////////////////////////////////////////////////////////////////////////////////////
void mainWindow::handle3OneSlot()
{
	frmCurrentCustomOwnDlg dlg(this);
	dlg.exec();
}

//日结单查询
void mainWindow::handle3TwoSlot()
{
	frmDaySettlementDlg dlg;
	dlg.exec();
}

//月结单查询
void mainWindow::handle3ThreeSlot()
{
	frmMonthSettlementDlg dlg;
	dlg.exec();
}

//强平单查询 
void mainWindow::handle3FourSlot()
{
	frmStopOutDialog dlg;
	dlg.exec();
}

//会员公告查询
void mainWindow::handle3FiveSlot()
{
	frmMemberNoticeDlg dlg;
	dlg.exec();
}

//委托报单查询
void mainWindow::handle3Sixsub1Slot()
{
	frmWTSearchDialog dlg;
	//dlg.setModal(true);
	dlg.exec();
}

//成交查询
void mainWindow::handle3Sixsub2Slot()
{
	frmMatchSearchDlg dlg;
	dlg.exec();
}

//延期持仓查询
void mainWindow::handle3Sixsub3Slot()
{
	frmDeferPosiSearchDlg dlg;
	dlg.exec();
}

//出入金明细
void mainWindow::handle3Sixsub4Slot()
{
	frmFunInOutSearchDlg dlg;
	dlg.exec();
}


/////////////////历史查询///////////////////////////
//委托报单查询
void mainWindow::handle3Sevensub1Slot()
{
	frmHistoryWTSearchDialog dlg;
	dlg.exec();
}
//成交查询
void mainWindow::handle3Sevensub2Slot()
{
	frmHistoryMatchSearchDlg dlg;
	dlg.exec();
}

//延期持仓查询
void mainWindow::handle3Sevensub3Slot()
{
	frmHistoryDeferPosiSearchDlg dlg;
	dlg.exec();
}
//库存查询
void mainWindow::handle3Sevensub4Slot()
{
	frmStoreSearchDialog dlg;
	dlg.exec();
}

//资金变动流水查询
void mainWindow::handle3Sevensub5Slot()
{
	frmCapitalChangeSearchDialog dlg;
	dlg.exec();
}

//资金查询
void mainWindow::handle3Sevensub6Slot()
{
	frmCapitalSearchDialog dlg;
	dlg.exec();
}

//费用明细
void mainWindow::handle3Sevensub7Slot()
{
	frmFeeDetailSearchDialog dlg;
	dlg.exec();
}


////////////////////////////////////////////end  查询//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////start  设置///////////////////////////////////////////////////////////////////
//设置
//清除缓存设置
void mainWindow::handle4OneSlot()
{
}

#include "frmOptionDlg.h"
//选项设置
void mainWindow::handle4TwoSlot()
{
	frmOptionDlg dlg;
	dlg.setMainWin(this);
	dlg.exec();

}

//帮助
void mainWindow::handle5OneSlot()
{
	QtTest dlg;

	dlg.exec();
}

