#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QSplitter>
#include "QuoteStandardItemModel.h"
#include <QStatusbar>
#include "mainWindow.h"
#include "myHelper.h"
#include "macrodefs.h"
#include "HJConstant.h"
#include "HJGlobalFun.h"
#include "Mgr/CodeTableMgr.h"
#include "Mgr/PosiMgr.h"
#include "Mgr/QuotationMgr.h"
#include "frmConfirmDlg.h"
#include "Global.h"
#include "HJCommon.h"
#include "frmTipsDlg.h"
#include "IniFilesManager.h"
#include "CommonStandardItemModel.h"
#include "TraderCpMgr.h"
#include "ClientTips.h"
#include "HJConstant.h"
#include "QParamEvent.h"
#include "CustomInfoMgrEx.h"
#include "TableItemDelegate.h"
#include "frmQHLoginDlg.h"

//#include <string.h>
////#include "..\..\Library\UnitTest\UnitTest.h"
//#include "Combinator\Combinator.h"
//#include "Combinator\StringCombinator.h"
//#include "Collections\OperationString.h"
//#include "Combinator\TokenCombinator.h"









extern QUOTATION gpQuotation;

#pragma execution_character_set("utf-8")

const int tab_index_HJ = 0;//黄金页面
//const int tab_index_QH = 1;//期货页面
const int tab_index_TL = 1;//套利页面




//整个区域由 title  + center 构成
mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	bLoadAllData = false;
	m_loginQh    = false;
	
	m_pConfig = new CConfigImpl();
	//增加模糊效果  2018-1-2
	QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
	windowShadow->setBlurRadius(9.0);
	windowShadow->setColor(QColor("#227dc3"));
	windowShadow->setOffset(0.0);
	this->setGraphicsEffect(windowShadow);


}

mainWindow::~mainWindow()
{
	if (0 != m_pConfig)
	{
		delete m_pConfig;
		m_pConfig = 0;
	}
	// 取消订阅广播消息
	g_TraderCpMgr.Unsubscribe(E_ONNEWBOURSEBULLETIN, this);
	g_TraderCpMgr.Unsubscribe(E_ONNEWMEMBERBULLETIN, this);
	g_TraderCpMgr.Unsubscribe(E_ONRISKNOTIFY, this);
	g_TraderCpMgr.Unsubscribe(E_ONSYSINIT, this);
	g_TraderCpMgr.Unsubscribe(E_UPDATE_LIST, this);

	g_TraderCpMgr.Unsubscribe(E_REFRESHFUND, this);

	//客户账号信息
	g_TraderCpMgr.Unsubscribe(E_CUSTOM_INFO_CHANGE, this);
	g_TraderCpMgr.Unsubscribe(E_SURPLUS_CHANGE, this);//持仓盈亏
															   //状态条信息
	g_TraderCpMgr.Unsubscribe(E_ONSYSSTATCHANGE, this);
	g_TraderCpMgr.Unsubscribe(E_HQ_STATE, this);

	//订阅行情
	g_TraderCpMgr.Unsubscribe(E_ONRECVFORWARDQUOTATION, this);
	g_TraderCpMgr.Unsubscribe(E_ONRECVDEFERDELIVERYQUOTATION, this);

	g_TraderCpMgr.Unsubscribe(E_ONRECVRTNSPOTINSTSTATEUPDATE, this);
	g_TraderCpMgr.Unsubscribe(E_ONRECVRTNFORWARDINSTSTATEUPDATE, this);
	g_TraderCpMgr.Unsubscribe(E_ONRECVRTNDEFERINSTSTATEUPDATE, this);

	g_TraderCpMgr.Unsubscribe(E_ONRECVFUTUREQUOTATION, this);
	g_TraderCpMgr.Unsubscribe(E_ONRECVSPREADQUOTATION, this);

	g_TraderCpMgr.Unsubscribe(E_ONRECVSPOTQUOTATION, this);
	g_TraderCpMgr.Unsubscribe(E_ONRECVDEFERQUOTATION, this);
	g_TraderCpMgr.Unsubscribe(E_ONLOGINSUCC, this);
	g_TraderCpMgr.Unsubscribe(E_ONCUSTRISKDEGREECHANGE, this);

	g_TraderCpMgr.Unsubscribe(E_COMMIT_POSI, this);
	g_TraderCpMgr.Unsubscribe(E_COMMIT_STORE, this);

	g_TraderCpMgr.SubscribeAll(this, false);


	g_TraderCpMgr.UserLogout();
}


void mainWindow::InitUI()
{
	//1.初始化相关参数
	initParam();

	//2.添加资金区域
	InitAccountUI();
	//3.添加报价表
	InitTabViewUI();
	//4.添加下单板
	InitOrderUI();
	//添加报单回报
	InitOrderReturnUI();
	//添加持仓/资金/库存
	InitLocalDBUI();
	//添加成交流水
	InitTransReturnUI();
	//添加splitter
	InitSplitterUI();
	//添加状态
	InitStatusLabelUI();
	//初始化深度行情界面
	InitDeepQuoteUI();
	//初始化订阅消息
	InitSubscribe();
	//初始化右键菜单
	InitTableContextMenu();
}

void mainWindow::InitAccountUI()
{
	accountBarWidget = new QWidget(this);
	QVBoxLayout *accountvlayout = new QVBoxLayout();
	QHBoxLayout *accountlayout = new QHBoxLayout();
	accountlayout->addSpacing(20);

	accountlayout->addWidget(ui.label_account_change);
	accountlayout->addSpacing(10);
	ui.comboBox_account->setFixedWidth(110);

	QStringList list;
	int accIndex = 0;
	for (size_t i = 0; i < App::accMgr.size(); i++)
	{
		list << App::accMgr.value(i).alias;
		if (g_Global.m_strUserID == App::accMgr.value(i).user)
			accIndex = i;
	}
	ui.comboBox_account->addItems(list);
	ui.comboBox_account->setCurrentIndex(accIndex);

	accountlayout->addWidget(ui.comboBox_account);
	accountlayout->addSpacing(8);
	
	accountlayout->addWidget(ui.pushButton_add); ui.pushButton_add->setFixedWidth(60);
	accountlayout->addSpacing(15);

	accountlayout->addWidget(ui.label_account_name);
	accountlayout->addSpacing(10);
	accountlayout->addWidget(ui.label_accountname_val);
	accountlayout->addSpacing(20);

	accountlayout->addWidget(ui.label_capital);
	accountlayout->addSpacing(10);
	accountlayout->addWidget(ui.label_capital_val);
	accountlayout->addSpacing(20);

	accountlayout->addWidget(ui.label_gain);
	accountlayout->addSpacing(10);
	accountlayout->addWidget(ui.label_gain_val);
	accountlayout->addSpacing(20);

	accountlayout->addWidget(ui.label_right);
	accountlayout->addSpacing(10);
	accountlayout->addWidget(ui.label_right_val);
	accountlayout->addSpacing(20);

	accountlayout->addWidget(ui.label_static_right);
	accountlayout->addSpacing(10);
	accountlayout->addWidget(ui.label_static_right_val);
	accountlayout->addSpacing(20);

	accountlayout->addWidget(ui.label_account_risk);
	accountlayout->addSpacing(10);
	accountlayout->addWidget(ui.label_account_risk_val);

	accountlayout->addStretch();

	//一条黑色直线，作为分割
	QFrame * line = new QFrame(this);
	line->setStyleSheet("QFrame{border: none; background-color: #030303; width:3000px;}");//height:6px;
	line->setFixedHeight(6);

	QFrame * line2 = new QFrame(this);
	line2->setStyleSheet("QFrame{border: none; background-color: #030303; width:3000px;}");//height:6px;
	line2->setFixedHeight(6);

	accountvlayout->setSpacing(0);
	accountvlayout->setMargin(0);

	accountvlayout->addWidget(line);
	accountvlayout->addLayout(accountlayout);
	accountvlayout->addWidget(line2);
	
	//用widget封一层，便于重新布局
	accountBarWidget->setLayout(accountvlayout);

	m_layout->addWidget(accountBarWidget);
}

bool mainWindow::IsLoginQH()
{
	return  m_loginQh;
}

void mainWindow::SetLoginQHStatus(bool bstate)
{
	m_loginQh = bstate;
}

void mainWindow::InitTabViewUI()
{
	TabListQuotation = new QTabWidget;
	TabListQuotation->addTab(ui.tableViewMarketPrice, u8"上海金行情");
	ui.tableViewMarketPrice->setMainWinId(this);
	
	/*TabListQuotation->addTab(ui.tableViewMarketPrice_2, u8"期货行情");
	ui.tableViewMarketPrice_2->setMainWinId(this);*/

	tableViewMarketPrice_spread = new MarketTableView_qh(ui.splitter_2);
	TabListQuotation->addTab(tableViewMarketPrice_spread, u8"套利行情");
	tableViewMarketPrice_spread->setMainWinId(this);
	//期货业面，则检测期货账户
	connect(TabListQuotation, &QTabWidget::currentChanged, [this](int index) 
	{
		if (index == 0)
		{
			//使用上海金下单版
			//切换到报单流水
			TabOrderBoard->removeTab(1);
			TabOrderBoard->removeTab(0);
			TabOrderBoard->insertTab(0, leftMainWin, "下单板");
			quoteButton->show();
			//TabOrderBoard->addTab();
			//TabOrderBoard->setBaseSize(560, 280);
			//TabOrderBoard->setTabEnabled(1, true);
			
		}
		else if (index == 1)
		{
			//首先验证期货账户
			if (!this->IsLoginQH())
			{
			    //弹出期货登录框
				frmQHLoginDlg  dlg;
				int nRespone = dlg.exec();

				if (nRespone != QDialog::Accepted)
				{
					TabListQuotation->setCurrentIndex(0);
				}
				else
				{
					
				}
				dlg.close();
			}

			//切换到套利组合设置，套利组合监控
			TabOrderBoard->removeTab(0);
			TabOrderBoard->insertTab(0, spreadLeftMainWin, "套利下单板");
			TabOrderBoard->insertTab(1, spreadProdWidget, "套利合约设置");
			quoteButton->hide();
		
		}
		//TabListQuotation->setCurrentIndex(index);

	});

	

	//加入报价列表单
	ui.splitter->addWidget(TabListQuotation);
}

void mainWindow::createOrderUI()
{
	orderWidget           = new QWidget(this);
	hLayout_left          = new QVBoxLayout();	//报单局部模块的内容

	QHBoxLayout *highLine = new QHBoxLayout();//涨停布局
	QHBoxLayout *lowLine  = new QHBoxLayout();//跌停布局
	QVBoxLayout *com      = new QVBoxLayout();//组合布局

	auto * left_line1 = new QHBoxLayout;
	auto * left_line2 = new QHBoxLayout;
	auto * left_line3 = new QHBoxLayout;
	auto * left_line4 = new QHBoxLayout;
	auto * left_line5 = new QHBoxLayout;
	auto * left_line6 = new QHBoxLayout;
	auto * left_line7 = new QHBoxLayout;

	QIniFilesManager mag;
	vector<QString> vecUserNames;
	ui.securityComboBox->clear();
	mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);
	for (size_t i = 0; i < vecUserNames.size(); i++)
	{
		ui.securityComboBox->insertItem(i, vecUserNames.at(i));;
	}
	ui.securityComboBox->setView(new QListView());//加此句，CSS样式下拉高度，背景等样式才生效

	//限价报单、限价FOK、限价FAK、市价FOK、市价FAK、市价转限价
	QStringList list;
	list<< "限价报单" << "限价FOK" << "限价FAK"<< "市价FOK"<<"市价FAK" <<"市价转限价";
	ui.indiComboBox->addItems(list);
	ui.indiComboBox->setCurrentIndex(0);

	left_line1->addWidget(ui.label);
	left_line1->addSpacing(40);
	left_line1->addWidget(ui.securityComboBox);
	left_line1->addWidget(ui.label_security_tip);
	left_line1->addStretch();
	//一面二行
	left_line2->addWidget(ui.label_2);
	left_line2->addSpacing(40);
	left_line2->addWidget(ui.pushButton_yanqi);/*ui.pushButton_yanqi->setFixedSize(50, 23);*/
	left_line2->addWidget(ui.pushButton_jiaoshou);/*ui.pushButton_jiaoshou->setFixedSize(50, 23);*/
	left_line2->addWidget(ui.pushButton_zhonglicang);/*ui.pushButton_zhonglicang->setFixedSize(50, 23);*/
	left_line2->addStretch();

	//一面三行
	left_line3->addWidget(ui.label_6);
	left_line3->addSpacing(40);
	left_line3->addWidget(ui.pushButton_buy);
	left_line3->addWidget(ui.pushButton_sell);
	left_line3->addStretch();
	//一面四行
	left_line4->addWidget(ui.label_3);
	left_line4->addSpacing(40);
	left_line4->addWidget(ui.pushButton_kaicang);
	left_line4->addWidget(ui.pushButton_pingcang);
	left_line4->addStretch();
	//一面五行
	left_line5->setMargin(0);
	left_line5->addWidget(ui.label_4);
	left_line5->addSpacing(40);
	left_line5->addWidget(ui.spinBox_order_num);
	left_line5->addWidget(ui.label_7);
	left_line5->addWidget(ui.label_high_hand);

	left_line5->addStretch();
	//一面六行
	ui.label_pricetype->setText("指定价");
	ui.label_pricetype->mLastPriceMode = false;//true为最新价，false为指定价
	left_line6->setMargin(0);
	left_line6->addWidget(ui.label_pricetype);
	left_line6->addSpacing(20);
	left_line6->addWidget(ui.spinBox_order_price);


	//涨跌停价
	highLine->addWidget(ui.label_8);highLine->addWidget(ui.label_high_price);
	lowLine->addWidget(ui.label_9);lowLine->addWidget(ui.label_low_price);
	com->addLayout(highLine);com->addLayout(lowLine);
	left_line6->addLayout(com);

	left_line6->addStretch();
	//一面7行
	left_line7->addWidget(ui.pushButton_order);/*ui.pushButton_order->setFixedSize(75, 23);*/
	left_line7->addWidget(ui.pushButton_preOrder);/*ui.pushButton_preOrder->setFixedSize(75, 23);*/
	left_line7->addWidget(ui.indiComboBox);/*ui.pushButton_risk->setFixedSize(75, 23);*/
	left_line7->addStretch();

	//	//装配布局
	//hLayout_left->addSpacing(5);
	hLayout_left->addLayout(left_line1);
	hLayout_left->addLayout(left_line2);
	hLayout_left->addLayout(left_line3);
	hLayout_left->addLayout(left_line4);
	hLayout_left->addLayout(left_line5);
	hLayout_left->addLayout(left_line6);
	hLayout_left->addLayout(left_line7);

	hLayout_left->addStretch();//让上部的区域固定
	orderWidget->setFixedWidth(300);
	orderWidget->setLayout(hLayout_left);
}
void mainWindow::createSpreadProdSetUI()
{
	spreadProdWidget     = new QWidget();
	QHBoxLayout *up      = new QHBoxLayout();
	QVBoxLayout *upleft  = new QVBoxLayout();
	QVBoxLayout *upRight = new QVBoxLayout();
	//list+label
	QListView  *list = new QListView();
	QStandardItemModel *standardItemModel = new QStandardItemModel();

	ClickLabel     *Tip = new ClickLabel(nullptr);
	Tip->setText("帮助信息");
	//测试数据
	QStringList strList;
	strList.append("Ag1809-Ag(T+D)");
	strList.append("Au1809-Au(T+D)");
	
	int nCount = strList.size();
	for (int i = 0; i < nCount; i++)
	{
		QString string = static_cast<QString>(strList.at(i));
		QStandardItem *item = new QStandardItem(string);
		QBrush brush(QColor("200,200,200"));
		item->setBackground(brush);
		standardItemModel->appendRow(item);
	}
	list->setModel(standardItemModel);

	Tip->setStyleSheet("QLabel{border: none; color: rgb(225,138,84); margin:0px;Spacing:0px;}");
	upleft->addWidget(list);
	upleft->addWidget(Tip);
	//帮助的响应
	disconnect(Tip, &ClickLabel::clicked,Tip,&ClickLabel::slotClicked);
	connect(Tip, &ClickLabel::clicked, [&]()
	{

		QMessageBox msgBox;
		msgBox.setStyleSheet("QLabel{ color: rgb(255, 0, 0);}");
		msgBox.information(NULL, "设置说明", "方      向 : \"+\"表示组合方向和基础合约的买卖同向，而\"-\"表\t\t  示相反买卖方向。\n"
			"合约顺序 : 标识这个组合合约上的报单顺序，从小到大的顺\t\t  序报单。\n"
			"价格公式 : T1表示第一个合约买1价/卖1价(对价),T2表示第\t\t  二个合约买1价/卖1价(对价),"
			"以此类推。可加入以\t\t  下的算术运算符:+,-,*,/,().例如:T1*3.8-3*(T2+2)。\n"
			"手数配比 : 通过手数的配置，提升基础交易单位，使得各配\n"
			"\t   对标的合约价值相同，比如:由于白银期货为15千\n"
			"\t   克/手,白银延期为1千克/手,可以设置Ag1810的值\n"
			"\t   为1,Ag(T+D)的值为15,每手都是价值15千克。",
			QMessageBox::Ok /*| QMessageBox::No*/, QMessageBox::Ok);

	});

	//匹配的方法
	//1.公式： T1*9-2*(T2+3.2)
	//查找T1,替换值
	//查找T2,替换值
	//完成字符串表达式
/*
	init_option();
	string str = "(5+10.01)*3.8-3.1*(8+2.1)";

	QString  test = "T1+2*T2";
	test.replace("T1", "18");
	float price = calc(str.c_str());
	float bid1  = calc(str.c_str());//买1价
	float ask1  = calc(str.c_str());//卖1价
*/

	QHBoxLayout *line1 = new QHBoxLayout();
	line1->insertWidget(0,new QLabel("合约名称", this));
	line1->addSpacing(15);
	line1->insertWidget(2,new QLineEdit(this));
	line1->itemAt(2)->widget()->setEnabled(false);
	

	QHBoxLayout *line2 = new QHBoxLayout();
	line2->insertWidget(0,new QLabel("价格公式", this));
	line2->addSpacing(15);
	line2->insertWidget(2,new QLineEdit(this));
	line2->itemAt(2)->widget()->setEnabled(false);

	QHBoxLayout *line3 = new QHBoxLayout();
	line3->setMargin(0);
	line3->setSpacing(0);
	line3->insertWidget(0,new QLabel("现货合约", this));
	line3->insertWidget(1,new QComboBox(this));
	line3->insertWidget(2,new QLabel("期货合约", this));
	line3->insertWidget(3,new QComboBox(this));
	line3->itemAt(1)->widget()->setEnabled(false);
	line3->itemAt(3)->widget()->setEnabled(false);

	QHBoxLayout *line4 = new QHBoxLayout();
	line4->insertWidget(0,new QLabel("方    向", this));
	line4->insertWidget(1,new QComboBox(this));
	line4->insertWidget(2,new QLabel("方    向", this));
	line4->insertWidget(3,new QComboBox(this));
	line4->itemAt(1)->widget()->setEnabled(false);
	line4->itemAt(3)->widget()->setEnabled(false);

	QHBoxLayout *line5 = new QHBoxLayout();
	line5->insertWidget(0,new QLabel("手数配比", this));
	line5->addSpacing(15);
	line5->insertWidget(2,new QLineEdit(this));
	line5->insertWidget(3,new QLabel("手数配比", this));
	line5->addSpacing(15);
	line5->insertWidget(5,new QLineEdit(this));
	line5->itemAt(2)->widget()->setEnabled(false);
	line5->itemAt(5)->widget()->setEnabled(false);

	QHBoxLayout *line6 = new QHBoxLayout();
	line6->insertWidget(0,new QLabel("合约顺序", this));
	line6->insertWidget(1,new QComboBox(this));
	line6->insertWidget(2,new QLabel("合约顺序", this));
	line6->insertWidget(3,new QComboBox(this));
	line6->itemAt(1)->widget()->setEnabled(false);
	line6->itemAt(3)->widget()->setEnabled(false);

	QHBoxLayout *line7 = new QHBoxLayout();
	line7->insertWidget(0,new QLabel("追加深度", this));
	line7->addSpacing(15);
	line7->insertWidget(2,new QLineEdit(this));
	line7->insertWidget(3,new QLabel("追加深度", this));
	line7->addSpacing(15);
	line7->insertWidget(5,new QLineEdit(this));
	line7->itemAt(2)->widget()->setEnabled(false);
	line7->itemAt(5)->widget()->setEnabled(false);

	upRight->addLayout(line1);
	upRight->addLayout(line2);
	upRight->addLayout(line3);
	upRight->addLayout(line4);
	upRight->addLayout(line5);
	upRight->addLayout(line6);
	upRight->addLayout(line7);

	up->addSpacing(10);
	up->addLayout(upleft);
	up->addSpacing(10);
	up->addLayout(upRight);

	//
	QHBoxLayout *down       = new QHBoxLayout();
	down->setSpacing(2);
	down->insertWidget(0,new QPushButton("增加", this), 1);
	down->addSpacing(5);
	down->insertWidget(1,new QPushButton("修改", this), 1);
	down->addSpacing(5);
	down->insertWidget(2,new QPushButton("删除", this), 1);
	down->addSpacing(5);
	down->insertWidget(3,new QPushButton("保存", this), 1);
	down->addSpacing(5);
	down->insertWidget(4,new QPushButton("取消", this), 1);
	down->itemAt(1)->widget()->setEnabled(false);
	down->itemAt(2)->widget()->setEnabled(false);
	down->itemAt(3)->widget()->setEnabled(false); 
	down->itemAt(4)->widget()->setEnabled(false); 


	//响应列表选中
	connect(list,&QListView::clicked,[=](const QModelIndex &index)
	{
		int row = index.row();
		//读取值
		down->itemAt(0)->widget()->setEnabled(true);
		down->itemAt(1)->widget()->setEnabled(true);
		down->itemAt(2)->widget()->setEnabled(true);
		down->itemAt(3)->widget()->setEnabled(false);
		down->itemAt(4)->widget()->setEnabled(false);

	});

	//响应“增加”
	connect((QPushButton *)(down->itemAt(0)->widget()), &QPushButton::clicked, [=](bool)
	{
		line1->itemAt(2)->widget()->setEnabled(true); line1->itemAt(2)->widget()->setFocus();
		line2->itemAt(2)->widget()->setEnabled(true);
		line3->itemAt(1)->widget()->setEnabled(true);
		line3->itemAt(3)->widget()->setEnabled(true);
		line4->itemAt(1)->widget()->setEnabled(true);
		line4->itemAt(3)->widget()->setEnabled(true);
		line5->itemAt(2)->widget()->setEnabled(true);
		line5->itemAt(5)->widget()->setEnabled(true);
		line6->itemAt(1)->widget()->setEnabled(true);
		line6->itemAt(3)->widget()->setEnabled(true);
		line7->itemAt(2)->widget()->setEnabled(true);
		line7->itemAt(5)->widget()->setEnabled(true);

		down->itemAt(0)->widget()->setEnabled(false); 
		down->itemAt(1)->widget()->setEnabled(false); 
		down->itemAt(2)->widget()->setEnabled(false); 
		down->itemAt(3)->widget()->setEnabled(true);
		down->itemAt(4)->widget()->setEnabled(true); 
	});
	//响应“保存”
	connect((QPushButton *)(down->itemAt(3)->widget()), &QPushButton::clicked, [=](bool)
	{
		line1->itemAt(2)->widget()->setEnabled(false);
		line2->itemAt(2)->widget()->setEnabled(false);
		line3->itemAt(1)->widget()->setEnabled(false);
		line3->itemAt(3)->widget()->setEnabled(false);
		line4->itemAt(1)->widget()->setEnabled(false);
		line4->itemAt(3)->widget()->setEnabled(false);
		line5->itemAt(2)->widget()->setEnabled(false);
		line5->itemAt(5)->widget()->setEnabled(false);
		line6->itemAt(1)->widget()->setEnabled(false);
		line6->itemAt(3)->widget()->setEnabled(false);
		line7->itemAt(2)->widget()->setEnabled(false);
		line7->itemAt(5)->widget()->setEnabled(false);

		down->itemAt(0)->widget()->setEnabled(true);
		down->itemAt(1)->widget()->setEnabled(false);
		down->itemAt(2)->widget()->setEnabled(false);
		down->itemAt(3)->widget()->setEnabled(false);
		down->itemAt(4)->widget()->setEnabled(false);

	});
	//响应“取消”
	connect((QPushButton *)(down->itemAt(4)->widget()), &QPushButton::clicked, [=](bool)
	{
		line1->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line1->itemAt(2)->widget())->setText("");
		line2->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line2->itemAt(2)->widget())->setText("");
		line3->itemAt(1)->widget()->setEnabled(false);
		line3->itemAt(3)->widget()->setEnabled(false);
		line4->itemAt(1)->widget()->setEnabled(false);
		line4->itemAt(3)->widget()->setEnabled(false);
		line5->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line5->itemAt(2)->widget())->setText("");
		line5->itemAt(5)->widget()->setEnabled(false); ((QLineEdit *)line5->itemAt(5)->widget())->setText("");
		line6->itemAt(1)->widget()->setEnabled(false);
		line6->itemAt(3)->widget()->setEnabled(false);
		line7->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line7->itemAt(2)->widget())->setText("");
		line7->itemAt(5)->widget()->setEnabled(false); ((QLineEdit *)line7->itemAt(5)->widget())->setText("");

		down->itemAt(0)->widget()->setEnabled(true);
		down->itemAt(1)->widget()->setEnabled(false);
		down->itemAt(2)->widget()->setEnabled(false);
		down->itemAt(3)->widget()->setEnabled(false);
		down->itemAt(4)->widget()->setEnabled(false);
	});

	//响应“修改”
	connect((QPushButton *)(down->itemAt(4)->widget()), &QPushButton::clicked, [=](bool)
	{
		line1->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line1->itemAt(2)->widget())->setText("");
		line2->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line2->itemAt(2)->widget())->setText("");
		line3->itemAt(1)->widget()->setEnabled(false);
		line3->itemAt(3)->widget()->setEnabled(false);
		line4->itemAt(1)->widget()->setEnabled(false);
		line4->itemAt(3)->widget()->setEnabled(false);
		line5->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line5->itemAt(2)->widget())->setText("");
		line5->itemAt(5)->widget()->setEnabled(false); ((QLineEdit *)line5->itemAt(5)->widget())->setText("");
		line6->itemAt(1)->widget()->setEnabled(false);
		line6->itemAt(3)->widget()->setEnabled(false);
		line7->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line7->itemAt(2)->widget())->setText("");
		line7->itemAt(5)->widget()->setEnabled(false); ((QLineEdit *)line7->itemAt(5)->widget())->setText("");

		down->itemAt(0)->widget()->setEnabled(true);
		down->itemAt(1)->widget()->setEnabled(false);
		down->itemAt(2)->widget()->setEnabled(false);
		down->itemAt(3)->widget()->setEnabled(false);
		down->itemAt(4)->widget()->setEnabled(false);
	});

	//响应“删除”
	connect((QPushButton *)(down->itemAt(4)->widget()), &QPushButton::clicked, [=](bool)
	{
		line1->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line1->itemAt(2)->widget())->setText("");
		line2->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line2->itemAt(2)->widget())->setText("");
		line3->itemAt(1)->widget()->setEnabled(false);
		line3->itemAt(3)->widget()->setEnabled(false);
		line4->itemAt(1)->widget()->setEnabled(false);
		line4->itemAt(3)->widget()->setEnabled(false);
		line5->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line5->itemAt(2)->widget())->setText("");
		line5->itemAt(5)->widget()->setEnabled(false); ((QLineEdit *)line5->itemAt(5)->widget())->setText("");
		line6->itemAt(1)->widget()->setEnabled(false);
		line6->itemAt(3)->widget()->setEnabled(false);
		line7->itemAt(2)->widget()->setEnabled(false); ((QLineEdit *)line7->itemAt(2)->widget())->setText("");
		line7->itemAt(5)->widget()->setEnabled(false); ((QLineEdit *)line7->itemAt(5)->widget())->setText("");

		down->itemAt(0)->widget()->setEnabled(true);
		down->itemAt(1)->widget()->setEnabled(false);
		down->itemAt(2)->widget()->setEnabled(false);
		down->itemAt(3)->widget()->setEnabled(false);
		down->itemAt(4)->widget()->setEnabled(false);
	});
	QVBoxLayout *mainLayOut = new QVBoxLayout();
	mainLayOut->addLayout(up);
	mainLayOut->addSpacing(10);
	mainLayOut->addLayout(down);
	mainLayOut->addSpacing(15);
	spreadProdWidget->setLayout(mainLayOut);

}

void mainWindow::createSpreadOrderUI()
{
	spreadOrderWidget  = new QWidget(this);
	spreadHLayout_left = new QVBoxLayout();	//报单局部模块的内容

	QHBoxLayout *highLine = new QHBoxLayout();//涨停布局
	QHBoxLayout *lowLine  = new QHBoxLayout();//跌停布局
	QVBoxLayout *com      = new QVBoxLayout();//组合布局

	auto * left_line1 = new QHBoxLayout;
	auto * left_line2 = new QHBoxLayout;
	auto * left_line3 = new QHBoxLayout;
	auto * left_line4 = new QHBoxLayout;
	auto * left_line5 = new QHBoxLayout;
	auto * left_line6 = new QHBoxLayout;
	auto * left_line7 = new QHBoxLayout;
	auto * left_line8 = new QHBoxLayout;
	auto * left_line9 = new QHBoxLayout;

	//QIniFilesManager mag;
	//vector<QString> vecUserNames;
	//ui.securityComboBox->clear();
	//mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);
	//for (size_t i = 0; i < vecUserNames.size(); i++)
	//{
	//	ui.securityComboBox->insertItem(i, vecUserNames.at(i));;
	//}
	//ui.securityComboBox->setView(new QListView());//加此句，CSS样式下拉高度，背景等样式才生效

	//											  //限价报单、限价FOK、限价FAK、市价FOK、市价FAK、市价转限价
	//QStringList list;
	//list << "限价报单" << "限价FOK" << "限价FAK" << "市价FOK" << "市价FAK" << "市价转限价";
	//ui.indiComboBox->addItems(list);
	//ui.indiComboBox->setCurrentIndex(0);

	left_line1->setMargin(0);
	left_line1->addWidget(new QLabel(u8"组合合约", this));
	left_line1->addSpacing(40);
	QComboBox *pCombox = new QComboBox(this);
	pCombox->setFixedWidth(150);

	left_line1->addWidget(pCombox);
	left_line1->addStretch();
	//一面二行
	left_line2->setMargin(0);
	left_line2->addWidget(new QLabel("买卖方向", this));
	left_line2->addSpacing(40);

	QRadioButton *pbtn11 = new QRadioButton("买入", this);
	QRadioButton *pbtn12 = new QRadioButton("卖出", this);
	QButtonGroup *group = new QButtonGroup(this);
	group->addButton(pbtn11);
	group->addButton(pbtn12);
	pbtn11->setChecked(true);
	left_line2->addWidget(pbtn11);
	left_line2->addWidget(pbtn12);
	left_line2->addStretch();

	//一面三行
	left_line3->setMargin(0);
	left_line3->addWidget(new QLabel("买卖类型", this));
	left_line3->addSpacing(40);
	QRadioButton *pbtn21 = new QRadioButton("开仓", this);
	QRadioButton *pbtn22 = new QRadioButton("平仓", this);
	QRadioButton *pbtn23 = new QRadioButton("平今", this);

	QString qss = "QRadioButton::indicator:unchecked {image:url(:/res/rbUnfocus)}"
		"QRadioButton::indicator:checked{ image:url(:/res/rbNormal) }";
	pbtn21->setStyleSheet(qss);
	pbtn22->setStyleSheet(qss);
	pbtn23->setStyleSheet(qss);


	QButtonGroup *group2 = new QButtonGroup(this);
	group2->addButton(pbtn21);
	group2->addButton(pbtn22);
	group2->addButton(pbtn23);
	pbtn21->setChecked(true);


	left_line3->addWidget(pbtn21);
	left_line3->addWidget(pbtn22);
	left_line3->addWidget(pbtn23);
	left_line3->addStretch();

	//一面四行
	left_line4->setMargin(0);
	left_line4->addWidget(new QCheckBox("止盈", this));
	left_line4->addSpacing(40);
	QLineEdit *pedit1 = new QLineEdit("", this);
	pedit1->setFixedWidth(150);
	left_line4->addWidget(pedit1);
	left_line4->addStretch();
	//一面五行
	left_line5->setMargin(0);
	left_line5->addWidget(new QCheckBox("止损", this));
	left_line5->addSpacing(40);

	QLineEdit *pedit2 = new QLineEdit("", this);
	pedit2->setFixedWidth(150);
	left_line5->addWidget(pedit2);
	left_line5->addStretch();
	//一面六行
	left_line6->setMargin(0);
	QRadioButton *pbtn31 = new QRadioButton("单次", this);
	QRadioButton *pbtn32 = new QRadioButton("循环", this);
	QButtonGroup *group3 = new QButtonGroup(this);
	group3->addButton(pbtn31);
	group3->addButton(pbtn32);
	pbtn31->setChecked(true);
	pbtn31->setStyleSheet(qss);
	pbtn32->setStyleSheet(qss);

	left_line6->addWidget(pbtn31);
	left_line6->addSpacing(40);
	left_line6->addWidget(pbtn32);
	left_line6->addStretch();
	
	//一面7行
	left_line7->setMargin(0);
	left_line7->addWidget(new QLabel("手数", this));
	left_line7->addSpacing(67);
	QSpinBox *pSSpinBox = new QSpinBox(this);
	pSSpinBox->setFixedWidth(150);
	left_line7->addWidget(pSSpinBox);
	left_line7->addStretch();

	left_line8->setMargin(0);
	left_line8->addWidget(new QLabel("指定价", this));
	left_line8->addSpacing(55);
	QSpinBox *pSpinBox = new QSpinBox(this);
	pSpinBox->setFixedWidth(150);
	left_line8->addWidget(pSpinBox);;
	left_line8->addStretch();

	left_line9->setMargin(0);
	QPushButton *pPushBtn = new QPushButton("新增", this);
	pPushBtn->setFixedWidth(120);
	left_line9->addWidget(pPushBtn);
	left_line9->addStretch();
	//	//装配布局
	//hLayout_left->addSpacing(5);
	spreadHLayout_left->addLayout(left_line1);
	spreadHLayout_left->addLayout(left_line2);
	spreadHLayout_left->addLayout(left_line3);
	spreadHLayout_left->addLayout(left_line4);
	spreadHLayout_left->addLayout(left_line5);
	spreadHLayout_left->addLayout(left_line6);
	spreadHLayout_left->addLayout(left_line7);
	spreadHLayout_left->addLayout(left_line8);
	spreadHLayout_left->addLayout(left_line9);

	spreadHLayout_left->addStretch();//让上部的区域固定
	spreadOrderWidget->setFixedWidth(300);
	spreadOrderWidget->setLayout(spreadHLayout_left);
}


void mainWindow::InitOrderUI()
{
	TabOrderBoard              = new QTabWidget;//下单板的TAB控件
	OrderBoardMainLayout       = new QHBoxLayout;//下单板快整体布局【报价加行情】
	spreadOrderBoardMainLayout = new QHBoxLayout;//套利下单板布局

	createOrderUI();
	createQuoteUI();

	createSpreadOrderUI();
	createSpreadQuoteUI();
	createSpreadProdSetUI();

    InitNormalQuoteUI();
	InitSpreadNormalQuoteUI();

	//初始化状态【可通过配置，记住上次的退出的选择】
	slotChangeOrderType();
	slotChangeOpenCloseType();
	slotChangeBuyType();

	//深度行情按钮位置
	quoteButton = new QPushButton(TabOrderBoard);
	quoteButton->setGeometry(400, 6, 75, 30);
	quoteButton->setBackgroundRole(QPalette::Base);
	quoteButton->setText("深度行情");

	connect(quoteButton,&QPushButton::clicked,this, &mainWindow::slotDeepQuote);
	connect(ui.pushButton_add, &QPushButton::clicked, this, &mainWindow::slotChangeAccount);
	connect(ui.comboBox_account, &QComboBox::currentTextChanged, this, &mainWindow::slotLoginAccount);

	m_pSplitterOrderAndOrderReturn->addWidget(TabOrderBoard);

	//添加本控件的消息
	//connect(ui.securityComboBox, SIGNAL(clicked()), this, SLOT(changeQuote()));
}

void mainWindow::InitDeepQuoteUI()
{
	showDeepQuote = false;//默认五档行情
	deepQuoteDlg  = new frmDeepQuoteDlg(this);
	deepQuoteDlg->setWindowTitle("深度行情");
	deepQuoteDlg->setGeometry(300, 200, 200, 550);
	deepQuoteDlg->setFixedSize(200, 550);

	connect(deepQuoteDlg, SIGNAL(setHide()), this, SLOT(slotCloseDeepQuoteDlg()));
}

void mainWindow::createQuoteUI()
{
	hLayout_right = new QVBoxLayout;
	quoteWidget   = new QWidget(this);

	auto * right_line1 = new QHBoxLayout;
	auto * right_line2 = new QHBoxLayout;
	auto * right_line3 = new QHBoxLayout;
	auto * right_line4 = new QHBoxLayout;
	auto * right_line5 = new QHBoxLayout;
	auto * right_line6 = new QHBoxLayout;
	auto * right_line7 = new QHBoxLayout;
	auto * right_line8 = new QHBoxLayout;
	auto * right_line9 = new QHBoxLayout;
	auto * right_line10 = new QHBoxLayout;

	auto * right_lineSpace = new QHBoxLayout;

	auto * right_line11 = new QHBoxLayout;
	auto * right_line12 = new QHBoxLayout;
	auto * right_line13 = new QHBoxLayout;
	auto * right_line14 = new QHBoxLayout;
	auto * right_line15 = new QHBoxLayout;
	auto * right_line16 = new QHBoxLayout;
	auto * right_line17 = new QHBoxLayout;
	auto * right_line18 = new QHBoxLayout;
	auto * right_line19 = new QHBoxLayout;
	auto * right_line20 = new QHBoxLayout;


	right_lineSpace->setMargin(0);
	right_lineSpace->setSpacing(0);//买卖无间隙
	//设置line值
	right_line1->addWidget(ui.label_sell10);right_line1->addWidget(ui.label_sell_price10);right_line1->addSpacing(10); right_line1->addWidget(ui.label_sell_vol10);	right_line1->addStretch();
	right_line2->addWidget(ui.label_sell9);right_line2->addWidget(ui.label_sell_price9);right_line2->addSpacing(10); right_line2->addWidget(ui.label_sell_vol9);right_line2->addStretch();
	right_line3->addWidget(ui.label_sell8);right_line3->addWidget(ui.label_sell_price8);right_line3->addSpacing(10); right_line3->addWidget(ui.label_sell_vol8);right_line3->addStretch();
	right_line4->addWidget(ui.label_sell7);right_line4->addWidget(ui.label_sell_price7);right_line4->addSpacing(10); ;right_line4->addWidget(ui.label_sell_vol7);right_line4->addStretch();
	right_line5->addWidget(ui.label_sell6);right_line5->addWidget(ui.label_sell_price6);right_line5->addSpacing(10); ;right_line5->addWidget(ui.label_sell_vol6);right_line5->addStretch();
	right_line6->addWidget(ui.label_sell5);right_line6->addWidget(ui.label_sell_price5);right_line6->addSpacing(10); ;right_line6->addWidget(ui.label_sell_vol5);right_line6->addStretch();
	right_line7->addWidget(ui.label_sell4);right_line7->addWidget(ui.label_sell_price4);right_line7->addSpacing(10); right_line7->addWidget(ui.label_sell_vol4);right_line7->addStretch();
	right_line8->addWidget(ui.label_sell3);right_line8->addWidget(ui.label_sell_price3);right_line8->addSpacing(10); right_line8->addWidget(ui.label_sell_vol3);right_line8->addStretch();
	right_line9->addWidget(ui.label_sell2);right_line9->addWidget(ui.label_sell_price2);right_line9->addSpacing(10); ;right_line9->addWidget(ui.label_sell_vol2);right_line9->addStretch();
	right_line10->addWidget(ui.label_sell1);right_line10->addWidget(ui.label_sell_price1);right_line10->addSpacing(10); ;right_line10->addWidget(ui.label_sell_vol1);right_line10->addStretch();

	right_lineSpace->addWidget(ui.label_quote_space, 1);right_lineSpace->addWidget(ui.label_quote_space_2, 1);right_lineSpace->addStretch(0);

	right_line11->addWidget(ui.label_buy1);right_line11->addWidget(ui.label_buy_price1);right_line11->addSpacing(10); ;right_line11->addWidget(ui.label_buy_vol1);right_line11->addStretch();
	right_line12->addWidget(ui.label_buy2);right_line12->addWidget(ui.label_buy_price2);right_line12->addSpacing(10); ;right_line12->addWidget(ui.label_buy_vol2);right_line12->addStretch();
	right_line13->addWidget(ui.label_buy3);right_line13->addWidget(ui.label_buy_price3);right_line13->addSpacing(10); ;right_line13->addWidget(ui.label_buy_vol3);right_line13->addStretch();
	right_line14->addWidget(ui.label_buy4);right_line14->addWidget(ui.label_buy_price4);right_line14->addSpacing(10); ;right_line14->addWidget(ui.label_buy_vol4);right_line14->addStretch();
	right_line15->addWidget(ui.label_buy5);right_line15->addWidget(ui.label_buy_price5);right_line15->addSpacing(10); ;right_line15->addWidget(ui.label_buy_vol5);right_line15->addStretch();
	right_line16->addWidget(ui.label_buy6);right_line16->addWidget(ui.label_buy_price6);right_line16->addSpacing(10); ;right_line16->addWidget(ui.label_buy_vol6);right_line16->addStretch();
	right_line17->addWidget(ui.label_buy7);right_line17->addWidget(ui.label_buy_price7);right_line17->addSpacing(10); ;right_line17->addWidget(ui.label_buy_vol7);right_line17->addStretch();
	right_line18->addWidget(ui.label_buy8);right_line18->addWidget(ui.label_buy_price8);right_line18->addSpacing(10); ;right_line18->addWidget(ui.label_buy_vol8);right_line18->addStretch();
	right_line19->addWidget(ui.label_buy9);right_line19->addWidget(ui.label_buy_price9);right_line19->addSpacing(10); ;right_line19->addWidget(ui.label_buy_vol9);right_line19->addStretch();
	right_line20->addWidget(ui.label_buy10);right_line20->addWidget(ui.label_buy_price10);right_line20->addSpacing(10); right_line20->addWidget(ui.label_buy_vol10);right_line20->addStretch();

	hLayout_right->addSpacing(10);

	//10档卖盘
	hLayout_right->addLayout(right_line1);
	hLayout_right->addLayout(right_line2);
	hLayout_right->addLayout(right_line3);
	hLayout_right->addLayout(right_line4);
	hLayout_right->addLayout(right_line5);

	hLayout_right->addLayout(right_line6);
	hLayout_right->addLayout(right_line7);
	hLayout_right->addLayout(right_line8);
	hLayout_right->addLayout(right_line9);
	hLayout_right->addLayout(right_line10);

	hLayout_right->addLayout(right_lineSpace);

	/////////////////////////////买卖力度///////////////////////////////////////////	
	right_lineSpace->setDirection(QBoxLayout::LeftToRight);
	//增加间隔样式
	ui.label_quote_space->setText("");
	ui.label_quote_space->setStyleSheet("QLabel{border: none; background-color: rgb(255,43,26); margin:0px;Spacing:0px;}");//height:6px;
	ui.label_quote_space->setFixedHeight(6);
	ui.label_quote_space->setBaseSize(80, 5);

	//增加间隔样式2
	ui.label_quote_space_2->setText("");
	ui.label_quote_space_2->setStyleSheet("QLabel{border: none; background-color: #6cad3c; margin:0px;Spacing:0px;}");//height:6px;
	ui.label_quote_space_2->setFixedHeight(6);
	ui.label_quote_space_2->setBaseSize(80,5);
	//////////////////////////////////////////////////////////////////////////////

	//10档买盘
	hLayout_right->addLayout(right_line11);
	hLayout_right->addLayout(right_line12);
	hLayout_right->addLayout(right_line13);
	hLayout_right->addLayout(right_line14);
	hLayout_right->addLayout(right_line15);

	//待屏蔽
	hLayout_right->addLayout(right_line16);
	hLayout_right->addLayout(right_line17);
	hLayout_right->addLayout(right_line18);
	hLayout_right->addLayout(right_line19);
	hLayout_right->addLayout(right_line20);

	hLayout_right->addStretch();//让上面的报价区域固定
}


void mainWindow::createSpreadQuoteUI()
{
	spreadHLayout_right = new QVBoxLayout;
	spreadQuoteWidget   = new QWidget(this);

	auto * right_line1 = new QHBoxLayout;
	auto * right_line2 = new QHBoxLayout;
	auto * right_line3 = new QHBoxLayout;
	auto * right_line4 = new QHBoxLayout;
	auto * right_line5 = new QHBoxLayout;
	auto * right_line6 = new QHBoxLayout;
	auto * right_line7 = new QHBoxLayout;
	auto * right_line8 = new QHBoxLayout;
	auto * right_line9 = new QHBoxLayout;
	auto * right_line10 = new QHBoxLayout;

	auto * right_lineSpace = new QHBoxLayout;

	auto * right_line11 = new QHBoxLayout;
	auto * right_line12 = new QHBoxLayout;
	auto * right_line13 = new QHBoxLayout;
	auto * right_line14 = new QHBoxLayout;
	auto * right_line15 = new QHBoxLayout;
	auto * right_line16 = new QHBoxLayout;
	auto * right_line17 = new QHBoxLayout;
	auto * right_line18 = new QHBoxLayout;
	auto * right_line19 = new QHBoxLayout;
	auto * right_line20 = new QHBoxLayout;


	right_lineSpace->setMargin(0);
	right_lineSpace->setSpacing(0);//买卖无间隙
								   //设置line值
	/*right_line1->addWidget(ui.label_sell10); right_line1->addWidget(ui.label_sell_price10); right_line1->addSpacing(10); right_line1->addWidget(ui.label_sell_vol10);	right_line1->addStretch();
	right_line2->addWidget(ui.label_sell9); right_line2->addWidget(ui.label_sell_price9); right_line2->addSpacing(10); right_line2->addWidget(ui.label_sell_vol9); right_line2->addStretch();
	right_line3->addWidget(ui.label_sell8); right_line3->addWidget(ui.label_sell_price8); right_line3->addSpacing(10); right_line3->addWidget(ui.label_sell_vol8); right_line3->addStretch();
	right_line4->addWidget(ui.label_sell7); right_line4->addWidget(ui.label_sell_price7); right_line4->addSpacing(10); ; right_line4->addWidget(ui.label_sell_vol7); right_line4->addStretch();
	right_line5->addWidget(ui.label_sell6); right_line5->addWidget(ui.label_sell_price6); right_line5->addSpacing(10); ; right_line5->addWidget(ui.label_sell_vol6); right_line5->addStretch();*/
	right_line6->addWidget(new QLabel("卖五",this)); right_line6->addWidget(new QLabel("0.00", this)); right_line6->addSpacing(10); ; right_line6->addWidget(new QLabel("0", this)); right_line6->addStretch();
	right_line7->addWidget(new QLabel("卖四", this)); right_line7->addWidget(new QLabel("0.00", this)); right_line7->addSpacing(10); right_line7->addWidget(new QLabel("0", this)); right_line7->addStretch();
	right_line8->addWidget(new QLabel("卖三", this)); right_line8->addWidget(new QLabel("0.00", this)); right_line8->addSpacing(10); right_line8->addWidget(new QLabel("0", this)); right_line8->addStretch();
	right_line9->addWidget(new QLabel("卖二", this)); right_line9->addWidget(new QLabel("0.00", this)); right_line9->addSpacing(10); ; right_line9->addWidget(new QLabel("0", this)); right_line9->addStretch();
	right_line10->addWidget(new QLabel("卖一", this)); right_line10->addWidget(new QLabel("0.00", this)); right_line10->addSpacing(10); ; right_line10->addWidget(new QLabel("0", this)); right_line10->addStretch();

	right_lineSpace->addWidget(ui.label_quote_space, 1); right_lineSpace->addWidget(ui.label_quote_space_2, 1); right_lineSpace->addStretch(0);

	right_line11->addWidget(new QLabel("买一", this)); right_line11->addWidget(new QLabel("0.00", this)); right_line11->addSpacing(10); ; right_line11->addWidget(new QLabel("0", this)); right_line11->addStretch();
	right_line12->addWidget(new QLabel("买二", this)); right_line12->addWidget(new QLabel("0.00", this)); right_line12->addSpacing(10); ; right_line12->addWidget(new QLabel("0", this)); right_line12->addStretch();
	right_line13->addWidget(new QLabel("买三", this)); right_line13->addWidget(new QLabel("0.00", this)); right_line13->addSpacing(10); ; right_line13->addWidget(new QLabel("0", this)); right_line13->addStretch();
	right_line14->addWidget(new QLabel("买四", this)); right_line14->addWidget(new QLabel("0.00", this)); right_line14->addSpacing(10); ; right_line14->addWidget(new QLabel("0", this)); right_line14->addStretch();
	right_line15->addWidget(new QLabel("买五", this)); right_line15->addWidget(new QLabel("0.00", this)); right_line15->addSpacing(10); ; right_line15->addWidget(new QLabel("0", this)); right_line15->addStretch();
	//right_line16->addWidget(ui.label_buy6); right_line16->addWidget(ui.label_buy_price6); right_line16->addSpacing(10); ; right_line16->addWidget(ui.label_buy_vol6); right_line16->addStretch();
	//right_line17->addWidget(ui.label_buy7); right_line17->addWidget(ui.label_buy_price7); right_line17->addSpacing(10); ; right_line17->addWidget(ui.label_buy_vol7); right_line17->addStretch();
	//right_line18->addWidget(ui.label_buy8); right_line18->addWidget(ui.label_buy_price8); right_line18->addSpacing(10); ; right_line18->addWidget(ui.label_buy_vol8); right_line18->addStretch();
	//right_line19->addWidget(ui.label_buy9); right_line19->addWidget(ui.label_buy_price9); right_line19->addSpacing(10); ; right_line19->addWidget(ui.label_buy_vol9); right_line19->addStretch();
	//right_line20->addWidget(ui.label_buy10); right_line20->addWidget(ui.label_buy_price10); right_line20->addSpacing(10); ; right_line20->addWidget(ui.label_buy_vol10); right_line20->addStretch();

	spreadHLayout_right->addSpacing(10);

	//10档卖盘
	/*hLayout_right->addLayout(right_line1);
	hLayout_right->addLayout(right_line2);
	hLayout_right->addLayout(right_line3);
	hLayout_right->addLayout(right_line4);
	hLayout_right->addLayout(right_line5);*/

	spreadHLayout_right->addLayout(right_line6);
	spreadHLayout_right->addLayout(right_line7);
	spreadHLayout_right->addLayout(right_line8);
	spreadHLayout_right->addLayout(right_line9);
	spreadHLayout_right->addLayout(right_line10);

	spreadHLayout_right->addLayout(right_lineSpace);

	/////////////////////////////买卖力度///////////////////////////////////////////	
	//right_lineSpace->setDirection(QBoxLayout::LeftToRight);
	////增加间隔样式
	//ui.label_quote_space->setText("");
	//ui.label_quote_space->setStyleSheet("QLabel{border: none; background-color: rgb(255,43,26); margin:0px;Spacing:0px;}");//height:6px;
	//ui.label_quote_space->setFixedHeight(6);
	//ui.label_quote_space->setBaseSize(80, 5);

	////增加间隔样式2
	//ui.label_quote_space_2->setText("");
	//ui.label_quote_space_2->setStyleSheet("QLabel{border: none; background-color: #6cad3c; margin:0px;Spacing:0px;}");//height:6px;
	//ui.label_quote_space_2->setFixedHeight(6);
	//ui.label_quote_space_2->setBaseSize(80, 5);
	//////////////////////////////////////////////////////////////////////////////

	//10档买盘
	spreadHLayout_right->addLayout(right_line11);
	spreadHLayout_right->addLayout(right_line12);
	spreadHLayout_right->addLayout(right_line13);
	spreadHLayout_right->addLayout(right_line14);
	spreadHLayout_right->addLayout(right_line15);

	//待屏蔽
	//hLayout_right->addLayout(right_line16);
	//hLayout_right->addLayout(right_line17);
	//hLayout_right->addLayout(right_line18);
	//hLayout_right->addLayout(right_line19);
	//hLayout_right->addLayout(right_line20);

	spreadHLayout_right->addStretch();//让上面的报价区域固定
}


void mainWindow::InitNormalQuoteUI()
{
	//创建一个动态组件，容纳下单模块
	leftMainWin = new QWidget;

	hLayout_right->removeWidget(ui.label_sell10);ui.label_sell10->hide();
	hLayout_right->removeWidget(ui.label_sell9);ui.label_sell9->hide();
	hLayout_right->removeWidget(ui.label_sell8);ui.label_sell8->hide();
	hLayout_right->removeWidget(ui.label_sell7);ui.label_sell7->hide();
	hLayout_right->removeWidget(ui.label_sell6);ui.label_sell6->hide();

	hLayout_right->removeWidget(ui.label_sell_price10);ui.label_sell_price10->hide();
	hLayout_right->removeWidget(ui.label_sell_price9);ui.label_sell_price9->hide();
	hLayout_right->removeWidget(ui.label_sell_price8);ui.label_sell_price8->hide();
	hLayout_right->removeWidget(ui.label_sell_price7);ui.label_sell_price7->hide();
	hLayout_right->removeWidget(ui.label_sell_price6);ui.label_sell_price6->hide();

	hLayout_right->removeWidget(ui.label_sell_vol10);ui.label_sell_vol10->hide();
	hLayout_right->removeWidget(ui.label_sell_vol9);ui.label_sell_vol9->hide();
	hLayout_right->removeWidget(ui.label_sell_vol8);ui.label_sell_vol8->hide();
	hLayout_right->removeWidget(ui.label_sell_vol7);ui.label_sell_vol7->hide();
	hLayout_right->removeWidget(ui.label_sell_vol6);ui.label_sell_vol6->hide();

	hLayout_right->removeWidget(ui.label_buy10);ui.label_buy10->hide();
	hLayout_right->removeWidget(ui.label_buy9);ui.label_buy9->hide();
	hLayout_right->removeWidget(ui.label_buy8);ui.label_buy8->hide();
	hLayout_right->removeWidget(ui.label_buy7);ui.label_buy7->hide();
	hLayout_right->removeWidget(ui.label_buy6);ui.label_buy6->hide();

	hLayout_right->removeWidget(ui.label_buy_price10);ui.label_buy_price10->hide();
	hLayout_right->removeWidget(ui.label_buy_price9);ui.label_buy_price9->hide();
	hLayout_right->removeWidget(ui.label_buy_price8);ui.label_buy_price8->hide();
	hLayout_right->removeWidget(ui.label_buy_price7);ui.label_buy_price7->hide();
	hLayout_right->removeWidget(ui.label_buy_price6);ui.label_buy_price6->hide();

	hLayout_right->removeWidget(ui.label_buy_vol10);ui.label_buy_vol10->hide();
	hLayout_right->removeWidget(ui.label_buy_vol9);ui.label_buy_vol9->hide();
	hLayout_right->removeWidget(ui.label_buy_vol8);ui.label_buy_vol8->hide();
	hLayout_right->removeWidget(ui.label_buy_vol7);ui.label_buy_vol7->hide();
	hLayout_right->removeWidget(ui.label_buy_vol6);ui.label_buy_vol6->hide();

	//为5档报价设置布局
	quoteWidget->setLayout(hLayout_right);

	OrderBoardMainLayout->addSpacing(20);
	OrderBoardMainLayout->addWidget(orderWidget);//加报盘区域
	OrderBoardMainLayout->addSpacing(20);
	OrderBoardMainLayout->addWidget(quoteWidget);//加报价区域
	OrderBoardMainLayout->addStretch();

	//再次用窗口封了一层
	//leftMainWin->setBaseSize(600, 320);
	leftMainWin->setLayout(OrderBoardMainLayout);

	TabOrderBoard->insertTab(0,leftMainWin, "下单板");
	//TabOrderBoard->addTab(leftMainWin, "下单板");
	//TabOrderBoard->setBaseSize(560, 280);
}

void mainWindow::InitSpreadNormalQuoteUI()
{
	//创建一个动态组件，容纳下单模块
	spreadLeftMainWin = new QWidget;

	//为5档报价设置布局
	spreadQuoteWidget->setLayout(spreadHLayout_right);

	spreadOrderBoardMainLayout->addSpacing(20);
	spreadOrderBoardMainLayout->addWidget(spreadOrderWidget);//加报盘区域
	spreadOrderBoardMainLayout->addSpacing(20);
	spreadOrderBoardMainLayout->addWidget(spreadQuoteWidget);//加报价区域
	spreadOrderBoardMainLayout->addStretch();

	//再次用窗口封了一层
	spreadLeftMainWin->setBaseSize(600, 320);
	spreadLeftMainWin->setLayout(spreadOrderBoardMainLayout);

	//TabOrderBoard->insertTab(1, spreadLeftMainWin, "套利下单板");
	//TabOrderBoard->addTab();
	//bOrderBoard->setBaseSize(560, 280);
	//TabOrderBoard->setTabEnabled(1, true);
	//TabOrderBoard->removeTab(1);
}


//removeWidget，并没有真正删除，需要hide
void mainWindow::ShowNormalQuoteUI()
{
	hLayout_right->removeWidget(ui.label_sell10);ui.label_sell10->hide();
	hLayout_right->removeWidget(ui.label_sell9);ui.label_sell9->hide();
	hLayout_right->removeWidget(ui.label_sell8);ui.label_sell8->hide();
	hLayout_right->removeWidget(ui.label_sell7);ui.label_sell7->hide();
	hLayout_right->removeWidget(ui.label_sell6);ui.label_sell6->hide();

	hLayout_right->removeWidget(ui.label_sell_price10);ui.label_sell_price10->hide();
	hLayout_right->removeWidget(ui.label_sell_price9);ui.label_sell_price9->hide();
	hLayout_right->removeWidget(ui.label_sell_price8);ui.label_sell_price8->hide();
	hLayout_right->removeWidget(ui.label_sell_price7);ui.label_sell_price7->hide();
	hLayout_right->removeWidget(ui.label_sell_price6);ui.label_sell_price6->hide();

	hLayout_right->removeWidget(ui.label_sell_vol10);ui.label_sell_vol10->hide();
	hLayout_right->removeWidget(ui.label_sell_vol9);ui.label_sell_vol9->hide();
	hLayout_right->removeWidget(ui.label_sell_vol8);ui.label_sell_vol8->hide();
	hLayout_right->removeWidget(ui.label_sell_vol7);ui.label_sell_vol7->hide();
	hLayout_right->removeWidget(ui.label_sell_vol6);ui.label_sell_vol6->hide();

	hLayout_right->removeWidget(ui.label_buy10);ui.label_buy10->hide();
	hLayout_right->removeWidget(ui.label_buy9);ui.label_buy9->hide();
	hLayout_right->removeWidget(ui.label_buy8);ui.label_buy8->hide();
	hLayout_right->removeWidget(ui.label_buy7);ui.label_buy7->hide();
	hLayout_right->removeWidget(ui.label_buy6);ui.label_buy6->hide();

	hLayout_right->removeWidget(ui.label_buy_price10);ui.label_buy_price10->hide();
	hLayout_right->removeWidget(ui.label_buy_price9);ui.label_buy_price9->hide();
	hLayout_right->removeWidget(ui.label_buy_price8);ui.label_buy_price8->hide();
	hLayout_right->removeWidget(ui.label_buy_price7);ui.label_buy_price7->hide();
	hLayout_right->removeWidget(ui.label_buy_price6);ui.label_buy_price6->hide();

	hLayout_right->removeWidget(ui.label_buy_vol10);ui.label_buy_vol10->hide();
	hLayout_right->removeWidget(ui.label_buy_vol9);ui.label_buy_vol9->hide();
	hLayout_right->removeWidget(ui.label_buy_vol8);ui.label_buy_vol8->hide();
	hLayout_right->removeWidget(ui.label_buy_vol7);ui.label_buy_vol7->hide();
	hLayout_right->removeWidget(ui.label_buy_vol6);ui.label_buy_vol6->hide();

	OrderBoardMainLayout->addWidget(quoteWidget);
	quoteWidget->setLayout(hLayout_right);
	quoteWidget->show();
}

//removeWidget，并没有真正删除，需要hide
void mainWindow::ShowDeepQuoteUI()
{	
	//格式化界面
	OrderBoardMainLayout->removeWidget(quoteWidget);
	quoteWidget->hide();
	OrderBoardMainLayout->setSizeConstraint(QLayout::SetMinimumSize);

	deepQuoteDlg->setLayout(hLayout_right);
	quoteWidget->show();
	//报价部分
	ui.label_sell10->show();ui.label_sell_vol10->show();
	ui.label_sell9->show();ui.label_sell9->show();
	ui.label_sell8->show();ui.label_sell8->show();
	ui.label_sell7->show();ui.label_sell7->show();
	ui.label_sell6->show();ui.label_sell6->show();

	ui.label_sell_price10->show();ui.label_sell_price10->show();
	ui.label_sell_price9->show();ui.label_sell_price9->show();
	ui.label_sell_price8->show();ui.label_sell_price8->show();
	ui.label_sell_price7->show();ui.label_sell_price7->show();
	ui.label_sell_price6->show();ui.label_sell_price6->show();

	ui.label_sell_vol10->show();ui.label_sell_vol10->show();
	ui.label_sell_vol9->show();ui.label_sell_vol9->show();
	ui.label_sell_vol8->show();ui.label_sell_vol8->show();
	ui.label_sell_vol7->show();ui.label_sell_vol7->show();
	ui.label_sell_vol6->show();ui.label_sell_vol6->show();

	ui.label_buy10->show();ui.label_buy_vol10->show();
	ui.label_buy9->show();ui.label_buy9->show();
	ui.label_buy8->show();ui.label_buy8->show();
	ui.label_buy7->show();ui.label_buy7->show();
	ui.label_buy6->show();ui.label_buy6->show();

	ui.label_buy_price10->show();ui.label_buy_price10->show();
	ui.label_buy_price9->show();ui.label_buy_price9->show();
	ui.label_buy_price8->show();ui.label_buy_price8->show();
	ui.label_buy_price7->show();ui.label_buy_price7->show();
	ui.label_buy_price6->show();ui.label_buy_price6->show();

	ui.label_buy_vol10->show();ui.label_buy_vol10->show();
	ui.label_buy_vol9->show();ui.label_buy_vol9->show();
	ui.label_buy_vol8->show();ui.label_buy_vol8->show();
	ui.label_buy_vol7->show();ui.label_buy_vol7->show();
	ui.label_buy_vol6->show();ui.label_buy_vol6->show();
}

class QStrategyRunning;
void mainWindow::InitOrderReturnUI()
{
	TabRight = new QTabWidget(this);

	orderReturnWidget = new QWidget(this);
	orderReturnTable = new QTableView(this);
	preorderReturnTable = new QTableView(this);
	preOrderReturnWidget = new QWidget(this);
	spreadRunWidget = new QWidget(this);

	spreadTable = new QTableView(this);//new QTableWidget();

	orderReturnTable->setItemDelegate(new TableItemDelegate(orderReturnTable));
	preorderReturnTable->setItemDelegate(new TableItemDelegate(preorderReturnTable));
	spreadTable->setItemDelegate(new TableItemDelegate(spreadTable));

	orderReturnTable->setMinimumWidth(300);


	//加入报单的按钮以及布局
	QHBoxLayout * layout = new QHBoxLayout();
	QVBoxLayout *vlayout = new QVBoxLayout();

	vlayout->addSpacing(25);
	vlayout->addWidget(ui.pushButton_cancelorder);
	vlayout->addWidget(ui.pushButton_cancelallorder);
	vlayout->addWidget(ui.pushButton_changeorder);
	vlayout->addStretch();

	layout->setMargin(0);
	layout->addWidget(orderReturnTable);
	layout->addSpacing(5);
	layout->addLayout(vlayout);
	layout->addSpacing(15);
	//报单页面加上布局
	orderReturnWidget->setLayout(layout);

#ifdef  _WIN32

	setWidgetStyleFromQssFile(orderReturnWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));
#else
	setWidgetStyleFromQssFile(orderReturnWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/ordertable.css"));
#endif

	//加入预埋单的按钮以及布局
	QVBoxLayout * prelayout = new QVBoxLayout();
	QHBoxLayout *hlayout = new QHBoxLayout();

	hlayout->addSpacing(20);
	hlayout->addWidget(ui.pushButton_pre_delete);
	hlayout->addWidget(ui.pushButton_pre_send);
	hlayout->addWidget(ui.pushButton_clear_send);
	hlayout->addStretch();

	prelayout->setMargin(0);
	prelayout->addWidget(preorderReturnTable);
	prelayout->addLayout(hlayout);
	//持仓页面加上布局
	preOrderReturnWidget->setLayout(prelayout);

#ifdef  _WIN32

	setWidgetStyleFromQssFile(preOrderReturnWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));
#else
	setWidgetStyleFromQssFile(preOrderReturnWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/ordertable.css"));
#endif


	QVBoxLayout *spreadlayout = new QVBoxLayout();
	spreadlayout->setMargin(0);
	spreadlayout->addWidget(spreadTable);
	spreadRunWidget->setLayout(spreadlayout);

#ifdef  _WIN32

	setWidgetStyleFromQssFile(spreadRunWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));
#else
	setWidgetStyleFromQssFile(spreadRunWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/ordertable.css"));
#endif

	TabRight->addTab(orderReturnWidget, "报单流水");
	TabRight->addTab(preOrderReturnWidget, "预埋单");
	TabRight->addTab(spreadRunWidget, "套利组合监控");

	m_pSplitterOrderAndOrderReturn->addWidget(TabRight);

	//调整初始化高度
	m_pSplitterOrderAndOrderReturn->setStretchFactor(0, 1);
	m_pSplitterOrderAndOrderReturn->setStretchFactor(1, 7);


	//设置报单表
	//tableModel_order = new QStandardItemModel();
	tableModel_order = new CommonStandardItemModel();
	((CommonStandardItemModel *)tableModel_order)->LoadList(g_Global.GetListIniPath("EntrFlow"));

	//tableModel_order->sort(0,Qt::DescendingOrder);
	tableModel_order->setSortRole(1);
	orderReturnTable->setModel(tableModel_order);

	orderReturnTable->setBaseSize(width() + 50, 350);
	orderReturnTable->horizontalHeader()->setFixedHeight(32);
	orderReturnTable->verticalHeader()->hide();
	orderReturnTable->verticalHeader()->setDefaultSectionSize(30);
	orderReturnTable->setShowGrid(false);
	orderReturnTable->setSortingEnabled(false);
	orderReturnTable->setAlternatingRowColors(true);//隔行换色
	orderReturnTable->setSelectionMode(QAbstractItemView::SingleSelection);
	orderReturnTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	orderReturnTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	//设置预埋单表
	//设置报单表
	//tableModel_preorder = new QStandardItemModel();
	tableModel_preorder = new CommonStandardItemModel();
	((CommonStandardItemModel *)tableModel_preorder)->LoadList(g_Global.GetListIniPath("PreOrder"));

	tableModel_preorder->setSortRole(1);
	preorderReturnTable->setModel(tableModel_preorder);

	preorderReturnTable->setBaseSize(width() + 50, 350);
	preorderReturnTable->horizontalHeader()->setFixedHeight(32);
	preorderReturnTable->verticalHeader()->hide();
	preorderReturnTable->verticalHeader()->setDefaultSectionSize(30);
	preorderReturnTable->setShowGrid(false);
	preorderReturnTable->setSortingEnabled(false);
	preorderReturnTable->setAlternatingRowColors(true);//隔行换色
	preorderReturnTable->setSelectionMode(QAbstractItemView::SingleSelection);
	preorderReturnTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	preorderReturnTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	//撤单
	connect(ui.pushButton_cancelorder, &QPushButton::clicked, this, &mainWindow::OnBnClickedButtonCancel);
	//全撤
	connect(ui.pushButton_cancelallorder, &QPushButton::clicked, this, &mainWindow::OnBnClickedButtonCancelAll);
	//改单
	connect(ui.pushButton_changeorder, &QPushButton::clicked, this, &mainWindow::OnBnClickedButtonModify);
	//删除
	connect(ui.pushButton_pre_delete, &QPushButton::clicked, this, &mainWindow::OnBnClickedButtonPreDelete);
	//立即发送
	connect(ui.pushButton_pre_send, &QPushButton::clicked, this, &mainWindow::OnBnClickedButtonPreSend);
	//清空已发送
	connect(ui.pushButton_clear_send, &QPushButton::clicked, this, &mainWindow::OnBnClickedButtonPreClear);


	//套利组合的数据
	tableModel_spread = new CommonStandardItemModel();
	((CommonStandardItemModel *)tableModel_spread)->LoadList(g_Global.GetListIniPath("SpreadTbl"));

	tableModel_spread->setSortRole(1);
	spreadTable->setModel(tableModel_spread);

	spreadTable->setBaseSize(width() + 50, 350);
	spreadTable->horizontalHeader()->setFixedHeight(32);
	spreadTable->verticalHeader()->hide();
	spreadTable->verticalHeader()->setDefaultSectionSize(30);
	spreadTable->setShowGrid(false);
	spreadTable->setSortingEnabled(false);
	spreadTable->setAlternatingRowColors(true);//隔行换色
	spreadTable->setSelectionMode(QAbstractItemView::SingleSelection);
	spreadTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	spreadTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	//kenny 20180626  test
	QStandardItem *item = new QStandardItem("");

	tableModel_spread->insertRow(0);

	//测试数据
	//for (int i = 0; i < 13; i++)
	{
		tableModel_spread->setItem(0, 0,new QStandardItem(QString::number(1)));
		QHBoxLayout *layout = new QHBoxLayout();
		QPushButton *btnDelete = new QPushButton("");
		QPushButton *btnStart = new QPushButton("");
		QPushButton *btnClose = new QPushButton("");

		//设置按钮状态样式
		btnDelete->setStyleSheet("QPushButton{ border: none; border-left: none; background-color: transparent; }"
			"QPushButton{image: url(:/res/deletenormal);}"
			/*"QPushButton:hover{image: url(:/res/deletehover);}"*/
			"QPushButton:pressed{image: url(:/res/deletehover);}");

		btnStart->setStyleSheet("QPushButton{ border: none; border-left: none; background-color: transparent;}"
			"QPushButton{image: url(:/res/startnormal);}"
			/*"QPushButton:hover{image: url(:/res/pausehover);}"*/
			"QPushButton:pressed{image: url(:/res/pausenormal);}");

		btnClose->setStyleSheet("QPushButton{ border: none; border-left: none; background-color: transparent;}"
			"QPushButton{image: url(:/res/stopnormal);}"
			/*"QPushButton:hover{image: url(:/res/stophover);}"*/
			"QPushButton:pressed{image: url(:/res/stophover);}");
		
		//设置按钮消息响应
		connect(btnDelete, &QPushButton::clicked, [&](bool checked)
		{

			//(QPushButton *)(spreadTable->indexWidget(tableModel_spread->index(0, 1))->layout()->itemAt(0)->widget());
			//btnDelete->setChecked(checked);
		});
		connect(btnStart, &QPushButton::clicked, [&](bool checked)
		{

			//(QPushButton *)(spreadTable->indexWidget(tableModel_spread->index(0, 1))->layout()->itemAt(0)->widget());
			//btnDelete->setChecked(checked);


			strategy_conditionOrder order;
			g_TraderCpMgr.runStrategyConditionOrder(std::move(tableModel_spread->item(0, 3)->text()));

		});
		connect(btnClose, &QPushButton::clicked, [&](bool checked)
		{
			//1.提示操作确认
			//2.删除Task
			g_TraderCpMgr.stopStrategyConditionOrder(std::move(tableModel_spread->item(0, 3)->text()));
			//3.界面删除
		});

		QWidget* rect = new QWidget();
		rect->setLayout(layout);
		layout->setMargin(1);
		layout->setSpacing(5);
		layout->addWidget(btnDelete);
		layout->addWidget(btnStart);
		layout->addWidget(btnClose);

		spreadTable->setIndexWidget(tableModel_spread->index(0, 1), rect);


		tableModel_spread->setItem(0, 2, new QStandardItem("单次执行"));
		tableModel_spread->setItem(0, 3, new QStandardItem("ag1809-Ag(T+D)"));
		tableModel_spread->setItem(0, 4, new QStandardItem("初始"));
		tableModel_spread->setItem(0, 5, new QStandardItem("买"));
		tableModel_spread->setItem(0, 6, new QStandardItem("9.00"));
		tableModel_spread->setItem(0, 7, new QStandardItem("10"));
	}

	//测试-启动策略/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//1.获取设置

	strategy_conditionOrder  cond;
	cond.id = tableModel_spread->item(0, 3)->text();
	cond.obj[0].prodID    = "ag1809";
	cond.obj[0].direction = 0;
	cond.obj[0].flag = 0;
	cond.obj[0].rate = 1;
	cond.obj[0].market = "QH";

	cond.obj[1].prodID = "Ag(T+D)";
	cond.obj[1].direction = 1;
	cond.obj[1].flag = 1;
	cond.obj[1].rate = 15;
	cond.obj[1].market = "HJ";

	//cond.flag = tableModel_spread->item(0, 1)->text();
	cond.status = tableModel_spread->item(0, 2)->text();
	cond.direction = tableModel_spread->item(0, 3)->text();
	cond.price = tableModel_spread->item(0, 4)->text().toDouble();
	cond.volume = tableModel_spread->item(0, 5)->text().toDouble();
	cond.positionPrice = tableModel_spread->item(0, 6)->text().toDouble();
	cond.positionPrice = tableModel_spread->item(0, 7)->text().toDouble();

	cond.express = "T1-T2";//在table中保存，便于取出
	/*cond.gainPrice = tableModel_spread->item(0, 8)->text().toDouble();
	cond.lossPrice = tableModel_spread->item(0, 9)->text().toDouble();
	cond.avgPrice = tableModel_spread->item(0, 10)->text().toDouble();
	cond.profitloss = tableModel_spread->item(0, 11)->text().toDouble();
*/
	//for (int i = 0; i < row; ++i)
	{
		//QStrtegy_ConditionOrder *condorder = new QStrtegy_ConditionOrder();
		//condorder->sethandle(&g_TraderCpMgr);
		//condorder->setCondition(cond);
		//QThreadPool::globalInstance()->start(condorder);
	}
	
	g_TraderCpMgr.addStrategyConditionOrder(std::move(cond.id), cond);
	/////测试结束///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void mainWindow::loadPosiUI()
{
	tableModel_posi = new CommonStandardItemModel();
	((CommonStandardItemModel *)tableModel_posi)->LoadList(g_Global.GetListIniPath("DeferPosi"));
	
	tableModel_posi->setSortRole(1);
	Tabview_1->setModel(tableModel_posi);
	Tabview_1->setMinimumWidth(200);

	Tabview_1->horizontalHeader()->setFixedHeight(32);
	Tabview_1->verticalHeader()->hide();
	Tabview_1->verticalHeader()->setDefaultSectionSize(30);
	Tabview_1->setShowGrid(false);
	Tabview_1->setSortingEnabled(false);
	Tabview_1->setAlternatingRowColors(true);//隔行换色
	Tabview_1->setSelectionMode(QAbstractItemView::SingleSelection);
	Tabview_1->setSelectionBehavior(QAbstractItemView::SelectRows);
	Tabview_1->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许编辑

	Tabview_1->setItemDelegate(new TableItemDelegate(Tabview_1));
	
	posiWidget = new QWidget(this);
	//加入平仓，反手按钮以及布局
	QHBoxLayout * layout = new QHBoxLayout();
	QVBoxLayout *vlayout = new QVBoxLayout();

	vlayout->addSpacing(25);
	vlayout->addWidget(ui.pushButton_closePosi);
	vlayout->addWidget(ui.pushButton_reverse_order);
	vlayout->addStretch();

	layout->setMargin(0);
	layout->addWidget(Tabview_1);
	layout->addLayout(vlayout);
	layout->addSpacing(10);
	//持仓页面加上布局
	posiWidget->setLayout(layout);

#ifdef  _WIN32

        setWidgetStyleFromQssFile(posiWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\positable.css"));
#else
        setWidgetStyleFromQssFile(posiWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/positable.css"));
#endif


	connect(Tabview_1->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(showPosiOperationDetails(const QModelIndex &, const QModelIndex &)));


	//平仓
	connect(ui.pushButton_closePosi, &QPushButton::clicked,	this, &mainWindow::OnBnClickedButtonCov);
	//反手
	connect(ui.pushButton_reverse_order, &QPushButton::clicked,	this, &mainWindow::OnBnClickedButtonOpp);
}


void mainWindow::loadStoreUI()
{
	tableModel_store = new CommonStandardItemModel();
	((CommonStandardItemModel *)tableModel_store)->LoadList(g_Global.GetListIniPath("Store"));

	tableModel_store->setSortRole(1);
	Tabview_2->setModel(tableModel_store);
	Tabview_2->setMinimumWidth(200);

	Tabview_2->horizontalHeader()->setFixedHeight(32);
	Tabview_2->verticalHeader()->hide();
	Tabview_2->verticalHeader()->setDefaultSectionSize(30);
	Tabview_2->setShowGrid(false);
	Tabview_2->setSortingEnabled(false);
	Tabview_2->setAlternatingRowColors(true);//隔行换色
	Tabview_2->setSelectionMode(QAbstractItemView::SingleSelection);
	Tabview_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	Tabview_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许编辑
	Tabview_2->setItemDelegate(new TableItemDelegate(Tabview_2));

	storeWidget = new QWidget(this);
	//加入平仓，反手按钮以及布局
	QHBoxLayout * layout = new QHBoxLayout();
	QVBoxLayout *vlayout = new QVBoxLayout();

	layout->addSpacing(20);
	layout->addWidget(ui.pushButton_getgoods);
	layout->addStretch();
	
	vlayout->setMargin(0);
	vlayout->addWidget(Tabview_2);
	vlayout->addLayout(layout);
	//持仓页面加上布局
	storeWidget->setLayout(vlayout);

#ifdef  _WIN32
        setWidgetStyleFromQssFile(storeWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\positable.css"));
#else
        setWidgetStyleFromQssFile(storeWidget, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/positable.css"));
#endif

	connect(ui.pushButton_getgoods, &QPushButton::clicked, this, &mainWindow::handle2FiveSlot);

}

void mainWindow::loadCapitalUI()
{
	//tableModel_capital = new QStandardItemModel();
	tableModel_capital = new CommonStandardItemModel();
	((CommonStandardItemModel *)tableModel_capital)->LoadList(g_Global.GetListIniPath("Fund"));

	tableModel_capital->setSortRole(1);
	Tabview_3->setModel(tableModel_capital);
	Tabview_3->setMinimumWidth(200);

	Tabview_3->horizontalHeader()->setFixedHeight(32);
	Tabview_3->verticalHeader()->hide();
	Tabview_3->verticalHeader()->setDefaultSectionSize(30);
	Tabview_3->setShowGrid(false);
	Tabview_3->setSortingEnabled(false);
	Tabview_3->setAlternatingRowColors(true);//隔行换色
	Tabview_3->setSelectionMode(QAbstractItemView::SingleSelection);
	Tabview_3->setSelectionBehavior(QAbstractItemView::SelectRows);
	Tabview_3->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许编辑
	Tabview_3->setItemDelegate(new TableItemDelegate(Tabview_3));

	capitalWidget = new QWidget(this);
	//加入平仓，反手按钮以及布局
	QHBoxLayout * layout = new QHBoxLayout();
	QVBoxLayout *vlayout = new QVBoxLayout();

	layout->addSpacing(20);
	layout->addWidget(ui.pushButton_FunfInOut);
	layout->addStretch();

	vlayout->setMargin(0);
	vlayout->addWidget(Tabview_3);
	vlayout->addLayout(layout);
	//持仓页面加上布局
	capitalWidget->setLayout(vlayout);

	//给出入金添加响应
	connect(ui.pushButton_FunfInOut, &QPushButton::clicked, this, &mainWindow::handle2FourSlot);

#ifdef  _WIN32
        setWidgetStyleFromQssFile(Tabview_3, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\positable.css"));
#else
        setWidgetStyleFromQssFile(Tabview_3, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/positable.css"));
#endif

	//自适应宽度  20180404
	Tabview_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	Tabview_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}
//持仓/库存/资金
void mainWindow::InitLocalDBUI()
{
	tabWidget_own = new QTabWidget(this);

#ifdef  _WIN32

        setWidgetStyleFromQssFile(tabWidget_own, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\positable.css"));
#else
        setWidgetStyleFromQssFile(tabWidget_own, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/positable.css"));
#endif

	Tabview_1 = new QTableView(this);
	Tabview_2 = new QTableView(this);
	Tabview_3 = new QTableView(this);

	loadPosiUI();
	loadStoreUI();
	loadCapitalUI();
	

	tabWidget_own->addTab(posiWidget, "持仓");
	tabWidget_own->addTab(storeWidget, "库存");
	tabWidget_own->addTab(capitalWidget, "资金");

	m_pSplitterLocalDBAndTransReturn->addWidget(tabWidget_own);
}


void mainWindow::InitTransReturnUI()
{
	TabTrans            = new QTabWidget;
	TabView_trans       = new QTableView(this);
		
	tableModel_match = new CommonStandardItemModel();
	((CommonStandardItemModel *)tableModel_match)->LoadList(g_Global.GetListIniPath("MatchFlow"));

	tableModel_match->setSortRole(1);
	TabView_trans->setModel(tableModel_match);
	TabTrans->setMinimumWidth(200);

#ifdef  _WIN32
        setWidgetStyleFromQssFile(TabView_trans, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\transtable.css"));
#else
        setWidgetStyleFromQssFile(TabView_trans, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/transtable.css"));
#endif

	TabView_trans->setBaseSize(TabTrans->width(), TabTrans->height());
	TabView_trans->horizontalHeader()->setFixedHeight(32);
	TabView_trans->verticalHeader()->hide();
	TabView_trans->verticalHeader()->setDefaultSectionSize(30);
	TabView_trans->setShowGrid(false);
	TabView_trans->setSortingEnabled(false);
	TabView_trans->setAlternatingRowColors(true);//隔行换色
	TabView_trans->setSelectionMode(QAbstractItemView::SingleSelection);
	TabView_trans->setEditTriggers(QAbstractItemView::NoEditTriggers);
	TabView_trans->setSelectionBehavior(QAbstractItemView::SelectRows);
	TabView_trans->setItemDelegate(new TableItemDelegate(TabView_trans));
	TabView_trans->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
	TabView_trans->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列


	TabTrans->addTab(TabView_trans, "成交流水");


	m_pSplitterLocalDBAndTransReturn->addWidget(TabTrans);
}

void mainWindow::InitSplitterUI()
{
	m_pSplitterAllTithoutLabel->addWidget(ui.splitter);
	m_pSplitterAllTithoutLabel->addWidget(m_pSplitterOrderAndOrderReturn);
	m_pSplitterAllTithoutLabel->addWidget(m_pSplitterLocalDBAndTransReturn);

	//调整初始化高度
	m_pSplitterAllTithoutLabel->setStretchFactor(0, 20);
	m_pSplitterAllTithoutLabel->setStretchFactor(1, 1);
	m_pSplitterAllTithoutLabel->setStretchFactor(2, 15);

	m_layout->addWidget(m_pSplitterAllTithoutLabel);
}

void mainWindow::InitStatusLabelUI()
{
	statusbar = new QStatusBar(this);
	statusbar->setFixedHeight(50);

	QHBoxLayout* statuslayout = new QHBoxLayout();
	statuslayout->addSpacing(120);

	ui.label_date_val->setText(CHJGlobalFun::FormatDate(g_Global.m_strExchDate));// 系统交易日
	statuslayout->addWidget(ui.label_date);
	statuslayout->addWidget(ui.label_date_val);
	statuslayout->addSpacing(100);
	
	ui.label_sysstatus_val->setText(g_CodeTableMgr.GetCodeNameFromID("m_sys_stat", g_Global.m_strSysState));// 系统状态
	statuslayout->addWidget(ui.label_sysstatus);
	statuslayout->addWidget(ui.label_sysstatus_val);
	statuslayout->addSpacing(100);


	ui.label_branch_val->setText(g_Global.m_strBranchName);
	statuslayout->addWidget(ui.label_branch);
	statuslayout->addWidget(ui.label_branch_val);
	statuslayout->addSpacing(100);

	//服务器状态
	CCommHandler* pHandler = CCommHandler::Instance();
	QString strStatusVal;
	if (pHandler->m_mapServerGroup.size() > 0) // 防止异常
	{
		const map<string, string>& pmServer = pHandler->m_mapServerGroup.GetMap(pHandler->m_nCurServerPos);
		strStatusVal = QString::fromLocal8Bit(pmServer.find("server_name")->second.data());
	}
	ui.label_server_val->setText(strStatusVal);

	statuslayout->addWidget(ui.label_server);
	statuslayout->addWidget(ui.label_server_val);
	statuslayout->addSpacing(100);

	ui.label_server_status_val->setText("连接");
	statuslayout->addWidget(ui.label_server_status);
	statuslayout->addWidget(ui.label_server_status_val);
	statuslayout->addSpacing(100);

    //建一个窗口装入布局
	QWidget * win = new QWidget();
	win->setLayout(statuslayout);

#ifdef  _WIN32
        setWidgetStyleFromQssFile(win, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainwindow.css"));
#else
        setWidgetStyleFromQssFile(win, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainwindow.css"));
#endif


	statusbar->addWidget(win);

	//一条黑色直线，作为分割
	ui.line_status->setFixedHeight(5);
	ui.line_status->setStyleSheet("QFrame{border: none; background-color: #030303;}");
	
	m_layout->addWidget(ui.line_status);
	m_layout->addWidget(statusbar);
	
}

void mainWindow::RefreshStatusLabelUI()
{
	ui.label_date_val->setText(CHJGlobalFun::FormatDate(g_Global.m_strExchDate));// 系统交易日
	ui.label_sysstatus_val->setText(g_CodeTableMgr.GetCodeNameFromID("m_sys_stat", g_Global.m_strSysState));// 系统状态

	//服务器状态
	CCommHandler* pHandler = CCommHandler::Instance();
	QString strStatusVal;
	if (pHandler->m_mapServerGroup.size() > 0) // 防止异常
	{
		const map<string, string>& pmServer = pHandler->m_mapServerGroup.GetMap(pHandler->m_nCurServerPos);
		strStatusVal = QString::fromLocal8Bit(pmServer.find("server_name")->second.data());
	}
	
	ui.label_server_val->setText(strStatusVal);

	if(strStatusVal != "")
		ui.label_server_status_val->setText("连接");
	else
		ui.label_server_status_val->setText("网络异常");


}

//暂时在这里设置初始化值，以后保存或者读取配置获得
void mainWindow::initParam()
{
	//报价区初始值
	type = 0;
	openclose = 0;
	bullsell = 0;

	showCapital   = true;
	classiclayout = 0;

	this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowMinimizeButtonHint*/);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	this->setProperty("CanMove", true);
	this->setProperty("CanResize", true);

#ifdef  _WIN32

	setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainwindow.css"));
#else
        setWidgetStyleFromQssFile(this, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainwindow.css"));
#endif

	StyleSheetWidget* center = new StyleSheetWidget(this);
	this->setCentralWidget(center);

	m_layout = new QVBoxLayout(center);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	center->setLayout(m_layout);

	this->resize(1058, 900);

	//分割器
	m_pSplitterOrderAndOrderReturn = new QSplitter(Qt::Orientation::Horizontal);//水平  
	m_pSplitterLocalDBAndTransReturn = new QSplitter(Qt::Orientation::Horizontal);//水平  
	m_pSplitterAllTithoutLabel = new QSplitter(Qt::Orientation::Vertical);//垂直  

	 //1.添加标头
	m_title = new WindowTitle(this);
	connect(m_title, SIGNAL(buttonClick(WindowTitle::ButtonStatus)), this, SLOT(titleButtonClick(WindowTitle::ButtonStatus)));
	m_layout->addWidget(m_title);
}


void 	mainWindow::InitSubscribe()
{

	g_TraderCpMgr.Reigister(this);

	g_TraderCpMgr.SubscribeAll(this, true);
	g_TraderCpMgr.Subscribe(E_REFRESHFUND, this);

	//客户账号信息
	g_TraderCpMgr.Subscribe(E_CUSTOM_INFO_CHANGE, this);
	g_TraderCpMgr.Subscribe(E_SURPLUS_CHANGE, this);//持仓盈亏
															 
	//订阅行情
	g_TraderCpMgr.Subscribe(E_ONRECVFORWARDQUOTATION, this);
	g_TraderCpMgr.Subscribe(E_ONRECVDEFERDELIVERYQUOTATION, this);
	g_TraderCpMgr.Subscribe(E_ONRECVSPOTQUOTATION, this);
	g_TraderCpMgr.Subscribe(E_ONRECVDEFERQUOTATION, this);
	g_TraderCpMgr.Subscribe(E_ONRECVFUTUREQUOTATION, this);
	g_TraderCpMgr.Subscribe(E_ONRECVSPREADQUOTATION, this);

	g_TraderCpMgr.Subscribe(E_ONRECVRTNSPOTINSTSTATEUPDATE, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNFORWARDINSTSTATEUPDATE, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNDEFERINSTSTATEUPDATE, this);


	//交易广播
	g_TraderCpMgr.Subscribe(E_ONRECVDEFERORDER, this);//延期报单
	g_TraderCpMgr.Subscribe(E_ONRECVSPOTORDER, this);//现货报单
	g_TraderCpMgr.Subscribe(E_ONRECVFORWARDORDER, this);//远期报单
	g_TraderCpMgr.Subscribe(E_ONRECVDEFERDELIVERYAPPORDER, this);//报单
	g_TraderCpMgr.Subscribe(E_ONRECVMIDDLEAPPORDER, this);//报单

																   //交易回报
	g_TraderCpMgr.Subscribe(E_ONRECVRTNSPOTORDER, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRSPSPOTORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRSPFORWARDORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRTNFORWARDORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRTNDEFERORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRSPDEFERORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRTNDEFERDELIVERYAPPORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRSPDEFERDELIVERYAPPORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRTNMIDDLEAPPORDER, this);//
	g_TraderCpMgr.Subscribe(E_ONRECVRSPMIDDLEAPPORDER, this);//

																	  //撤单
	g_TraderCpMgr.Subscribe(E_ONRECVRTNSPOTORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRSPSPOTORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNFORWARDORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRSPFORWARDORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNDEFERORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRSPDEFERORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNDEFERDELIVERYAPPORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRSPDEFERDELIVERYAPPORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNMIDDLEAPPORDERCANCEL, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRSPMIDDLEAPPORDERCANCEL, this);

	//成交流水
	g_TraderCpMgr.Subscribe(E_ONRECVRTNSPOTMATCH, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNFORWARDMATCH, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNDEFERMATCH, this);
	g_TraderCpMgr.Subscribe(E_ONRECVRTNDEFERDELIVERYAPPMATCH, this);


	g_TraderCpMgr.Subscribe(E_ONLOGINSUCC, this);
	g_TraderCpMgr.Subscribe(E_ONCUSTRISKDEGREECHANGE, this);

	g_TraderCpMgr.Subscribe(E_COMMIT_POSI, this);
	g_TraderCpMgr.Subscribe(E_COMMIT_STORE, this);

	g_TraderCpMgr.Subscribe(E_CUSTOM_INFO_CHANGE, this);

	//20180402 增加一条预埋单，界面更新消息
	g_TraderCpMgr.Subscribe(E_PREORDER_CHANGE, this);

	g_TraderCpMgr.Subscribe(E_ONNEWBOURSEBULLETIN, this);
	g_TraderCpMgr.Subscribe(E_ONNEWMEMBERBULLETIN, this);
	g_TraderCpMgr.Subscribe(E_ONRISKNOTIFY, this);
	g_TraderCpMgr.Subscribe(E_ONSYSINIT, this);
	g_TraderCpMgr.Subscribe(E_UPDATE_LIST, this);
	g_TraderCpMgr.Subscribe(E_ONSYSSTATCHANGE, this);
	g_TraderCpMgr.Subscribe(E_HQ_STATE, this);

}
//系统按钮响应
void mainWindow::titleButtonClick(WindowTitle::ButtonStatus status)
{
	switch (status) {
	case WindowTitle::ButtonMin:
		this->showMinimized();
		break;

	case WindowTitle::ButtonMax:
		this->showMaximized();
		break;

	case WindowTitle::ButtonRestore:
		this->showNormal();
		break;

	case WindowTitle::ButtonClose:
		this->close();
		break;

	default:
		assert(false);
		break;
	}
}



template<class F, class... Args>
auto mainWindow::doUpdateUiTask(F&& f, Args&&... args)
->std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);

		tasks.emplace([task]() { (*task)(); });
	}


	QApplication::postEvent(this, new QEvent(WM_MY_UI_TASHK));

	return res;
}



int mainWindow::OnUITask(void * wparam, void * lparam)
{
	std::unique_lock<std::mutex> lock(queue_mutex);
	while (tasks.size() > 0)
	{
		std::function<void()> task = std::move(this->tasks.front());
		tasks.pop();
		task();
	}

	return 1;
}


//自定义消息
void  mainWindow::customEvent(QEvent *e)
{
	//qDebug("mainWindow::customEvent");
	QParamEvent *msg = static_cast<QParamEvent *>(e);
	if (msg->type() == MSG_RECV_QUOTATION)//上海金行情广播
	{
		SetQuotation((QUOTATION* )msg->wparam());
	}
	else if (msg->type() == MSG_RECV_FUTURE_QUOTATION)//期货行情广播
	{
		//SetQHQuotation((QUOTATION*)msg->wparam());
	}
	else if (msg->type() == MSG_RECV_SPREAD_QUOTATION)//套利行情广播
	{
		//SetSpreadQuotation((QUOTATION*)msg->wparam());
	}
	/*else if (msg->type() == WM_FORCE_LOGOUT)
	{
		TForceLogout *stLogout = (TForceLogout *)msg->wparam();
		//QMessageBox::information(nullptr, "系统提示", CHJGlobalFun::str2qstr(stLogout->hint_msg));
		QMessageBox::information(nullptr, "系统提示", "系统强制退出！");
		//kenny 20180320 
		//退出底层网络端口
		//CCommHandler::Instance()->Finish();

		//退出系统，自动清理管理层
		this->close();
	}*/
	//界面表格展示更新
	else if (msg->type() == WM_UPDATE_ORDER)//下单，表头重建
	{
		//下单界面  重新加载
		QIniFilesManager mag;
		vector<QString> vecUserNames;
		ui.securityComboBox->clear();
		mag.ReadUserChoose(g_Global.GetListIniPath("InsID"), "UserColumnIndexs", true, vecUserNames);
		for (size_t i = 0; i < vecUserNames.size(); i++)
		{
			ui.securityComboBox->insertItem(i, vecUserNames.at(i));;
		}

		//ui.securityComboBox->setView(new QListView());
		//行情报价界面 重新加载
		ui.tableViewMarketPrice->InitColumn();
		ui.tableViewMarketPrice->InitRowData();
	}
	else if (msg->type() == WM_UPDATE_QUOTATION_LIST)//行情展示列表，重建
	{
		ui.tableViewMarketPrice->InitColumn();
		ui.tableViewMarketPrice->InitRowData();
	}
	else if (msg->type() == WM_UPDATE_ENTR_FLOW)//报单流水，表头重建
	{
		tableModel_order->clear();
		((CommonStandardItemModel *)tableModel_order)->LoadList(g_Global.GetListIniPath("EntrFlow"));
		RefreshOrder();
	}
	else if (msg->type() == WM_UPDATE_DEFER_POSI)//持仓，表头重建
	{
		tableModel_posi->clear();
		((CommonStandardItemModel *)tableModel_posi)->LoadList(g_Global.GetListIniPath("DeferPosi"));
		RefreshPosi();//加载持仓
	}
	else if (msg->type() == WM_UPDATE_STORE)//库存，表头重建
	{
		tableModel_store->clear();
		((CommonStandardItemModel *)tableModel_store)->LoadList(g_Global.GetListIniPath("Store"));
		RefreshStore();//加载库存
	}
	else if (msg->type() == WM_UPDATE_FUN)//资金，表头重建
	{
		tableModel_capital->clear();
		((CommonStandardItemModel *)tableModel_capital)->LoadList(g_Global.GetListIniPath("Fund"));
		RefreshCapital();
	}
	else if (msg->type() == WM_UPDATE_MATCH_FLOW)
	{
		tableModel_match->clear();
		((CommonStandardItemModel *)tableModel_match)->LoadList(g_Global.GetListIniPath("MatchFlow"));
		RefreshMatch();

	}
	else if (msg->type() == WM_SYS_STAT_CHANGE)
	{
		//g_Global.m_strSysState = body.m_sys_stat.c_str();
		////获取交易日期
		//g_Global.m_strExchDate = body.exch_date.c_str();
		////获取系统日期
		//g_Global.m_strSysDate = body.sys_date.c_str();

		//SysStat *sys = (SysStat *)msg->wparam();
		RefreshStatusLabelUI();
	}
	else if (msg->type() == WM_ON_RISK_NOTIFY)//风险度
	{
		TRiskNotity *sys = (TRiskNotity *)msg->wparam();
		QMessageBox::information(nullptr, "系统提示", CHJGlobalFun::str2qstr(sys->risk_idx1));
	}
	else if (msg->type() == WM_SelectTableRow)//选中行情的行
	{
		DeferInstState *state = (DeferInstState *)msg->wparam();
		if (state->instID == "")
			return ;
		//Set
		if (ui.securityComboBox->currentText().compare(CHJGlobalFun::str2qstr(state->instID)) != 0)
		{
			ui.securityComboBox->setCurrentText(CHJGlobalFun::str2qstr(state->instID));
			slotChangeQuote(CHJGlobalFun::str2qstr(state->instID));
		}

	}
	else if (msg->type() == WM_PREORDER_CHANGE)//预埋单  界面更新
	{

		OnRecvPreOrderChange(msg->OrderId(), msg->getInt());

	}
	//////////////报单流水相关//////////////////////////////////////////////////////////////////
	else if (msg->type() == MSG_RECV_DEFER_ORDER)//延期报单回报
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		HandleDeferOrder(msg->wparam());
	}
	else if (msg->type() == WM_SHOW_TD_TO_TRADE)//kenny  20180329  报单同时发送来的消息，更新持仓界面
	{	
	/*	QString &csProdCode  = *(QString*)msg->wparam();
		bool    &csLong  = *(bool *)msg->lparam();

		UpdatePosiInfo(csProdCode,csLong);*/
	
	}
	else if (msg->type() == MSG_RECV_MA_ORDER)//报单流水
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		HandleMAOrder(msg->wparam());
	}
	else if (msg->type() == MSG_RECV_FORWARD_ORDER)//
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		HandleForwardOrder(msg->wparam());
	}
	else if (msg->type() == MSG_RECV_DDA_ORDER)//报单流水
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		HandleDDAOrder(msg->wparam());

	}
	else if (msg->type() == MSG_RECV_SPOT_ORDER)//报单流水广播
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		HandleSpotOrder(msg->wparam());//只处理报单回报

	}
	else if (msg->type() == MSG_MA_ORDER_CANCEL)
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		//HandleMAOrderCancel(msg->wparam());
	}
	else if (msg->type() == MSG_RECV_ORDER_CANCEL)
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		//HandleSpotOrderCancel(msg->wparam());
		//HandleOrderCancel(msg->wparam());

		RefreshOrder();
	}
	else if (msg->type() == MSG_DEFER_ORDER_CANCEL)//延期撤单
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		//HandleOrderCancel(msg->wparam());
		RefreshOrder();
	}
	else if (msg->type() == MSG_DDA_ORDER_CANCEL)
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		//HandleSpotOrder(msg->wparam());
	}
	/////////////////////成交//////////////////////////////////////////////////////////////////////
	else if (msg->type() == MSG_RECV_RTN_DDA_MATCH)//成交流水
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		//HandleRtnDDAMatch(msg->wparam());
		tableModel_order->removeRows(0, tableModel_order->rowCount());
		tableModel_match->removeRows(0, tableModel_match->rowCount());
		RefreshOrder();
		RefreshMatch();

	}
	else if (msg->type() == MSG_RECV_RTN_DEFER_MATCH)//成交流水
	{
		//HandleRtnDeferMatch(msg->wparam());
		tableModel_order->removeRows(0,tableModel_order->rowCount());
		tableModel_match->removeRows(0, tableModel_match->rowCount());
		RefreshOrder();
		RefreshMatch();
	}
	else if (msg->type() == MSG_RECV_RTN_SPOT_MATCH)//成交流水
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		//HandleRtnSpotMatch(msg->wparam());
		tableModel_order->removeRows(0, tableModel_order->rowCount());
		tableModel_match->removeRows(0, tableModel_match->rowCount());
		RefreshOrder();
		RefreshMatch();
	}
	/////////////////////远期相关//////////////////////////////////////////////////////////////////////
	else if (msg->type() == MSG_RECV_RTN_FORWARD_MATCH)//成交流水
	{
		// DeferOrder *pDeferOrder= (DeferOrder *)msg->wparam();
		//HandleRtnForwardMatch(msg->wparam());

		tableModel_order->removeRows(0, tableModel_order->rowCount());
		tableModel_match->removeRows(0, tableModel_match->rowCount());
		RefreshOrder();
		RefreshMatch();
	}
	/////////////////////////////////持仓/库存/资金////////////////////////////////
	else if (msg->type() == WM_REFRESH_POSI)//持仓
	{
		//直接在内存去，不能再向服务器申请了
		RefreshPosi();
	}
	else if (msg->type() == WM_REFRESH_STORE)//库存
	{
		//直接在内存去，不能再向服务器申请了
		RefreshStore();
	}
	else if (msg->type() == WM_REFRESH_FUND)//资金变化
	{

		RefreshCapital();
		//直接在内存去，不能再向服务器申请了
	}
	else if (msg->type() == WM_CUS_BASE_INFO || msg->type() == WM_CUSTOM_INFO_CHANGE)//客户基本信息
	{
		// 更新当前品种的持仓或者库存信息
		RefreshAccount();
		ProdCodeInfo info;
		GetCurProdCodeInfo(info);
		ShowCurStore(info);

		RefreshPosi();
		//// 显示可委托手数
		CalculateMax();
	}
	else if (msg->type() == WM_MY_UI_TASHK)
	{
		OnUITask(0, 0);
	}
	else if (msg->type() == WM_SURPLUS_CHANGE)
	{
		//
		int test = 0;
	}

	else if (msg->type() == WM_INST_STATE_UPDATE)//合约交易状态改变
	{
		OnInstStateUpdate(msg->wparam());

	}
	else if (msg->type() == WM_SHOW_ORDER_TIPS)//1.报单回报信息提示    2.预埋单回报显示信息
	{
		//int nAddPos = *((int *)(msg->wparam()));
		int nAddPos = (msg->getInt());
		//qDebug("mainWindow::customEvent:nAddPos");
		// bool bShow= false;
		//if (g_Global.m_bPreOrderConfirm )
		//{
		//	frmTipsDlg dlg;
		//	dlg.Show(nAddPos);

		//	dlg.exec();
		//	bShow = true;
		//}

		//if (!bShow)
		//{
			if (g_Global.m_bTipsAskFail || g_Global.m_bTipsAskSus)
			{
				frmTipsDlg dlg;
				dlg.Show(nAddPos);

				dlg.exec();
				
			}
		//}

	}
	else if (msg->type() == WM_SHOW_ORDER_CANCEL_TIPS)//1.撤销报单回报信息提示
	{

		int nAddPos = (msg->getInt());

		frmTipsDlg dlg;
		dlg.Show(nAddPos);

	    dlg.exec();

	}
	else if (msg->type() == WM_SHOW_ORDER_RESULT_TIPS)//1.报单回报信息提示    2.预埋单回报显示信息
	{
		int nAddPos = (msg->getInt());

		if (g_Global.m_bTipsOrderMatch)
		{
			frmTipsDlg dlg;
			dlg.Show(nAddPos);

			dlg.exec();
		}

	}
	//else if (msg->type() == WM_LOG_OUT)//签退广播
	//{
	//	PTForceLogout stLogout = (PTForceLogout)(msg->wparam());
	//	QMessageBox *box = new QMessageBox(QMessageBox::Information, "提示信息", QString::fromStdString(stLogout->hint_msg));
	//	box->setModal(false);
	//	box->setAttribute(Qt::WA_DeleteOnClose);
	//	QTimer::singleShot(5000, box, SLOT(accept()));
	//	box->show();

	//	this->close();
	//}


}

//响应菜单【设置】-【重新布局】
void mainWindow::ResetLayoutSlot()
{
	if (classiclayout == 0)
	{
		classiclayout = 1;
		changToClassicLayout();
	}
	else
	{
		classiclayout = 0;
		changToOldLayout();
	}

	//保存当前设置
	QString sfile = g_Global.GetUserIniPath();
	QString str   = QString::number(g_Global.m_iInterfaceStyle);
	g_Global.m_iInterfaceStyle = classiclayout;

	App::WritePriProfileString(sfile, ("Display"), ("UIStyle"), str);
}



//单类型
void mainWindow::slotChangeOrderType()
{
	type = 0;

	ui.pushButton_yanqi->setChecked(true);
	ui.pushButton_zhonglicang->setChecked(false);
	ui.pushButton_jiaoshou->setChecked(false);

	ui.pushButton_yanqi->setStyleSheet("background-color: #227dc3;");
	ui.pushButton_zhonglicang->setStyleSheet("background-color: #2a3a57;");
	ui.pushButton_jiaoshou->setStyleSheet("background-color: #2a3a57;");

	//打开开平仓和
	enableKaiPingOrderCtl();

	AskModeChange();
}

void mainWindow::slotChangeOrderType2()
{
	type = 1;
	ui.pushButton_yanqi->setChecked(false);
	ui.pushButton_zhonglicang->setChecked(true);
	ui.pushButton_jiaoshou->setChecked(false);

	ui.pushButton_jiaoshou->setStyleSheet("background-color: #227dc3;");
	ui.pushButton_yanqi->setStyleSheet("background-color: #2a3a57;");
	ui.pushButton_zhonglicang->setStyleSheet("background-color: #2a3a57;");

	//灰掉【开平/指定价】
	disableKaipingOrderCtl();

	AskModeChange();
}

void mainWindow::slotChangeOrderType3()
{
	type = 2;
	ui.pushButton_yanqi->setChecked(false);
	ui.pushButton_zhonglicang->setChecked(false);
	ui.pushButton_jiaoshou->setChecked(true);

	ui.pushButton_zhonglicang->setStyleSheet("background-color: #227dc3;");
	ui.pushButton_yanqi->setStyleSheet("background-color: #2a3a57;");
	ui.pushButton_jiaoshou->setStyleSheet("background-color: #2a3a57;");
	
	//灰掉【开平/指定价】
	disableKaipingOrderCtl();

	AskModeChange();
}

//开或平
void mainWindow::slotChangeOpenCloseType()
{
	openclose = 0;

	ui.pushButton_kaicang->setChecked(true);
	ui.pushButton_pingcang->setChecked(false);

	ui.pushButton_kaicang->setStyleSheet("background-color: #227dc3;");
	ui.pushButton_pingcang->setStyleSheet("background-color: #2a3a57;");

	AskModeChange();
}

void mainWindow::slotChangeOpenCloseType2()
{
	openclose = 1;

	ui.pushButton_kaicang->setChecked(false);
	ui.pushButton_pingcang->setChecked(true);

	ui.pushButton_kaicang->setStyleSheet("background-color:#2a3a57;");
	ui.pushButton_pingcang->setStyleSheet("background-color: #227dc3;");

	AskModeChange();
}


void mainWindow::slotChangeBuyType()
{
	bullsell = 0;

	ui.pushButton_buy->setChecked(true);
	ui.pushButton_sell->setChecked(false);

	ui.pushButton_buy->setStyleSheet("background-color: #227dc3;");
	ui.pushButton_sell->setStyleSheet("background-color: #2a3a57;");

	AskModeChange();
}

void mainWindow::slotChangeSellType()
{
	bullsell = 1;

	ui.pushButton_buy->setChecked(false);
	ui.pushButton_sell->setChecked(true);

	ui.pushButton_buy->setStyleSheet("background-color:#2a3a57;");
	ui.pushButton_sell->setStyleSheet("background-color: #227dc3;");

	AskModeChange();
}



void mainWindow::slotLabelClicked()
{


}

void mainWindow::slotPosiTableDoubleClick(const QModelIndex & QMindex)
{
	   //判断选择改变
	   //Tabview_1->changed
	   //根据当前合约名，切换报价区域
	  
}

//响应持仓表的选择改变
void mainWindow::showPosiOperationDetails(const QModelIndex &current, const QModelIndex &previous)
{
	QModelIndex codeIndex = tableModel_posi->index(current.row(), 0);//合约名称
	QModelIndex BSindex = tableModel_posi->index(current.row(), 1);//持仓方向
	QModelIndex handindex = tableModel_posi->index(current.row(), 2);//持仓手数

	QString code = codeIndex.data().toString();
	QString bs   = BSindex.data().toString();
	
	//改变合约选择和处理
	ui.securityComboBox->setCurrentText(code);
	slotChangeQuote(code);

	if (bs.contains(QString("多")))
		slotChangeSellType();
	else
		slotChangeBuyType();
	//设置为平仓
	slotChangeOpenCloseType2();

	// 设置手数为1
	ui.label_high_hand->setText(handindex.data().toString());
	ui.spinBox_order_num->setValue(handindex.data().toString().toInt());

	AskModeChange();
}

//响应持仓表的选择改变
void mainWindow::showStoreOperationDetails(const QModelIndex &current, const QModelIndex &previous)
{
	QModelIndex codeIndex = tableModel_store->index(current.row(), 0);//合约名称
	QModelIndex handindex = tableModel_store->index(current.row(), 2);//手数

	QString code = codeIndex.data().toString();

	//改变合约选择和处理
	ui.securityComboBox->setCurrentText(code);
	slotChangeQuote(std::move(code));
	//屏蔽方式
	disableYqZlJsOrderCtl();
	//设置为卖
	slotChangeSellType();

	//设置为平仓
	disableKaipingOrderCtl();

	// 设置手数为1
	ProdCodeInfo stProdinfo;
	if (g_TraderCpMgr.GetProdCodeInfo(code, stProdinfo))
	{
		int hand = handindex.data().toInt();
		hand = hand / stProdinfo.measure_unit+0.1;
		ui.label_high_hand->setText(QString::number(hand));
		ui.spinBox_order_num->setValue(hand);
	}

	AskModeChange();
}


void mainWindow::disableKaipingOrderCtl()
{
	ui.pushButton_kaicang->setEnabled(false);
	ui.pushButton_pingcang->setEnabled(false);
	ui.pushButton_kaicang->setStyleSheet("QPushButton{background-color:rgb(100, 100, 100);}");
	ui.pushButton_pingcang->setStyleSheet("QPushButton{background-color:rgb(100, 100, 100);}");


	ui.spinBox_order_price->setEnabled(false);
}

void mainWindow::enableKaiPingOrderCtl()
{
	ui.pushButton_kaicang->setEnabled(true);
	ui.pushButton_pingcang->setEnabled(true);

	ui.pushButton_kaicang->setStyleSheet("background-color: #2a3a57;");
	ui.pushButton_pingcang->setStyleSheet("background-color: #2a3a57;");
	if (openclose == 0)
		ui.pushButton_kaicang->setStyleSheet("background-color: #227dc3;");
	else
		ui.pushButton_pingcang->setStyleSheet("background-color: #227dc3;");

	ui.spinBox_order_price->setEnabled(true);

}

//灰掉延期.交收，中立仓的样式
void mainWindow::disableYqZlJsOrderCtl()
{
	ui.pushButton_yanqi->setEnabled(false);
	ui.pushButton_zhonglicang->setEnabled(false);
	ui.pushButton_jiaoshou->setEnabled(false);

	ui.pushButton_yanqi->setStyleSheet("background-color: rgb(100, 100, 100);");
	ui.pushButton_zhonglicang->setStyleSheet("background-color: rgb(100, 100, 100);");
	ui.pushButton_jiaoshou->setStyleSheet("background-color: rgb(100, 100, 100);");

}

//初始化延期.交收，中立仓的样式
void mainWindow::enableYqZlJsOrderCtl()
{
	ui.pushButton_yanqi->setEnabled(true);
	ui.pushButton_zhonglicang->setEnabled(true);
	ui.pushButton_jiaoshou->setEnabled(true);

	ui.pushButton_yanqi->setStyleSheet("background-color: #2a3a57;");
	ui.pushButton_zhonglicang->setStyleSheet("background-color: #2a3a57;");
	ui.pushButton_jiaoshou->setStyleSheet("background-color: #2a3a57;");
	if (type == 0)
		ui.pushButton_yanqi->setStyleSheet("background-color: #227dc3;");
	else if (type == 1)
		ui.pushButton_jiaoshou->setStyleSheet("background-color: #227dc3;");
	else
		ui.pushButton_zhonglicang->setStyleSheet("background-color: #227dc3;");

}


//信号槽:响应combobox选择的改变
void mainWindow::slotChangeQuote(const QString & string)
{
	ProdCodeInfo info;
	GetCurProdCodeInfo(info);

	// 切换界面模式
	if (info.market_id == CONSTANT_B_MARKET_ID_SPOT) // 现货
	{
		SwitchMode(0);
	}
	else if (info.market_id == CONSTANT_B_MARKET_ID_FORWARD) // 远期
	{
		SwitchMode(2);
	}
	else if (info.market_id == CONSTANT_B_MARKET_ID_DEFER) // 延期
	{
		SwitchMode(1);
	}



	QUOTATION* qt = &(g_TraderCpMgr.m_QMapQuotation[CHJGlobalFun::qstr2str(info.prod_code)]);

	// 显示价格信息
	ShowPriceInfo(CHJGlobalFun::qstr2str(info.prod_code), std::move(*qt));

	// 显示买卖1-5
	SetOrderQuotation(qt);

	// 显示当前品种的仓库信息
	ShowCurStore(info);

	// 设置手数为1
	ui.label_high_hand->setText("1");

	// 是否切换tab界面
	if (g_Global.m_bInsTriggerInfo)
	{
		// 根据当前品种类型获取tab控件的标题
		QString csTitle = (info.market_id == CONSTANT_B_MARKET_ID_SPOT) ? "库存" : "持仓";
		// 根据界面风格获取所在的窗口
		//EDLGID eDlg = (g_Global.m_iInterfaceStyle == Interface_Style_Classic) ? E_DlgDownLeft : E_DlgUpRight;
		// 获取窗口句柄

		QParamEvent *msg = new QParamEvent(WM_CHANGE_TAB_SEL);
		msg->setWParam(&csTitle);
		QApplication::sendEvent(this, new QEvent(QEvent::Type(WM_CHANGE_TAB_SEL)));

	}

	//要切换
	if ( bLoadAllData)
	{
		//if(非经典布局)
		if (info.market_id == CONSTANT_B_MARKET_ID_SPOT)
			tabWidget_own->setCurrentIndex(1);
		else
			tabWidget_own->setCurrentIndex(0);
	}

	AskModeChange();

}


//边框阴影效果   2018-1-2
void mainWindow::paintEvent(QPaintEvent *event)
{

	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(10, 10, this->width() - 20, this->height() - 20);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::green));

	QColor color(0, 0, 0, 50);
	for (int i = 0; i<10; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
		color.setAlpha(150 - qSqrt(i) * 50);
		painter.setPen(color);
		painter.drawPath(path);
	}
	
}

void mainWindow::cleanWidgetContent()
{
	//ui.label_accountname_val->setText("");
	ui.label_capital_val->setText("");
	ui.label_gain_val->setText("");
	ui.label_right_val->setText("");
	ui.label_static_right_val->setText("");
	ui.label_account_risk_val->setText("");

	tableModel_posi->removeRows(0,tableModel_posi->rowCount());
	tableModel_store->removeRows(0,tableModel_store->rowCount());
	tableModel_capital->removeRows(0, tableModel_capital->rowCount());
	tableModel_match->removeRows(0, tableModel_match->rowCount());
	tableModel_preorder->removeRows(0, tableModel_preorder->rowCount());
	tableModel_order->removeRows(0, tableModel_order->rowCount());

}
//切换账户
void mainWindow::slotLoginAccount(const QString & str)
{
	doUpdateUiTask(std::bind(&mainWindow::cleanWidgetContent, this));
	//避免切换闪烁先填入账户别名
	ui.label_accountname_val->setText(str);
	g_TraderCpMgr.resetAccount(str.toStdString().c_str());
		
}