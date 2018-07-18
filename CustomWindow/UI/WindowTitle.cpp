
#include <cassert>

#include <QEvent>
#include <QHBoxLayout>
//#include <QToolButton>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QLabel>
#include <QMenu>
#include "WindowTitle.h"
#include "Macrodefs.h"

#include "mainWindow.h"

#pragma execution_character_set("utf-8")


WindowTitle::WindowTitle(QWidget *window)
	: StyleSheetWidget(window)
	, m_window(window), m_layout(NULL)
	, m_buttonMin(NULL), m_buttonMax(NULL), m_buttonRestore(NULL), m_buttonClose(NULL)
{
	m_layout = new QHBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
	

	//1.公司LOGO
	QLabel *label    = new QLabel(this);
	label->setPixmap(QPixmap::fromImage(QImage(":/res/logo")));
	m_layout->addWidget(label);

	setFixedHeight(40);
	//2.加入弹簧
	QSpacerItem* Spacer = new QSpacerItem(200, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_layout->addItem(Spacer);
	//3.加入功能按钮
	//initMenuButton();
	initMenu();
	//4.加上最大最小按钮
	initButton();	

	m_layout->addSpacing(5);

	setLayout(m_layout);

        setStyleSheet("QWidget { background-color: #181a1d;}");//不显示下拉图片

	m_window->installEventFilter(this);



}

bool WindowTitle::isCaption(int x, int y) const
{
	if (this->rect().contains(x, y)) 
	{
		//在放大，缩小，关闭等按钮位置，不需要拖动
		if (this->rect().x()+rect().width() - 210 < x)
			return false;

		
		//if (!this->childAt(x, y)) {
			return true;
		//}
	}
	return false;
}

bool WindowTitle::eventFilter(QObject* o, QEvent* e)
{
	if (e->type() == QEvent::WindowStateChange) {
		Qt::WindowStates states = m_window->windowState();
		if (states & Qt::WindowMaximized) {
			m_buttonMax->hide();
			m_buttonRestore->show();
		}
		else {
			m_buttonMax->show();
			m_buttonRestore->hide();
		}
		return true;
	}

	return QWidget::eventFilter(o, e);
}

void WindowTitle::initMenuButton()
{
	assert(!(m_buttonMin || m_buttonMax || m_buttonRestore || m_buttonClose));

	
	//// 系统
	m_buttonSys = new QPushButton(tr(""), this);
	m_buttonSys->setObjectName(tr("btnmenuSys"));
	m_buttonSys->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonSys->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/btnmenuSys);}"
		"QPushButton:hover{border-image: url(:/res/btnmenuSys_press);background-color: rgb(23,32,47);}"
		"QPushButton:pressed{border-image: url(:/res/btnmenuSys_press);}"
		"QPushButton::menu-indicator{ image:none; }");//不显示下拉图片
	m_buttonSys->setFixedWidth(97);
	m_buttonSys->setFixedHeight(40);

	//connect(m_buttonMin, &QPushButton::clicked, [this]() {
	//	emit buttonClick(ButtonMin);
	//});
	m_layout->addWidget(m_buttonSys);

	// 操作
	m_buttonOption = new QPushButton("", this);
	m_buttonOption->setObjectName("btnmenuOpt");
	m_buttonOption->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonOption->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/btnmenuOpt);}"
		"QPushButton:hover{border-image: url(:/res/btnmenuOpt_press);}"
		"QPushButton:pressed{border-image: url(:/res/btnmenuOpt_press);}"
		"QPushButton::menu-indicator{ image:none; }");//不显示下拉图片
	m_buttonOption->setFixedWidth(102);
	m_buttonOption->setFixedHeight(40);
	//connect(m_buttonMax, &QPushButton::clicked, [this](bool) {
	//	emit buttonClick(ButtonMax);
	//});
	m_layout->addWidget(m_buttonOption);


	m_buttonQuery = new QPushButton(this);
	m_buttonQuery->setObjectName("btnmenuQuery");
	m_buttonQuery->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonQuery->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/btnmenuQuery);}"
		"QPushButton:hover{border-image: url(:/res/btnmenuQuery_press);}"
		"QPushButton:pressed{border-image: url(:/res/btnmenuQuery_press);}"
		"QPushButton::menu-indicator{ image:none; }");
	m_buttonQuery->setFixedWidth(115);
	m_buttonQuery->setFixedHeight(40);

	//connect(m_buttonRestore, &QPushButton::clicked, [this]() {
	//	emit buttonClick(ButtonRestore);
	//});
	m_layout->addWidget(m_buttonQuery);

	// 设置按钮
	m_buttonSet = new QPushButton(this);
	m_buttonSet->setObjectName("btnmenuSet");
	m_buttonSet->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonSet->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/btnmenuSet);}"
		"QPushButton:hover{border-image: url(:/res/btnmenuSet_press);}"
		"QPushButton:pressed{border-image: url(:/res/btnmenuSet_press);}"
		"QPushButton::menu-indicator{ image:none; }");
	m_buttonSet->setFixedWidth(92);
	m_buttonSet->setFixedHeight(40);
	//connect(m_buttonSet, &QPushButton::clicked, [this]() {
	//	emit buttonClick(ButtonClose);
	//});
	m_layout->addWidget(m_buttonSet);

	// 帮助按钮
	m_buttonHelp = new QPushButton(this);
	m_buttonHelp->setObjectName("btnmenuHelp");
	m_buttonHelp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonHelp->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/btnmenuHelp);}"
		"QPushButton:hover{border-image: url(:/res/btnmenuHelp_press);}"
		"QPushButton:pressed{border-image: url(:/res/btnmenuHelp_press);}"
		"QPushButton::menu-indicator{ image:none; }");
	m_buttonHelp->setFixedWidth(96);
	m_buttonHelp->setFixedHeight(40);
	//connect(m_buttonSet, &QPushButton::clicked, [this]() {
	//	emit buttonClick(ButtonClose);
	//});
	m_layout->addWidget(m_buttonHelp);


	//2.加入弹簧
	QSpacerItem* Spacer = new QSpacerItem(100, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_layout->addItem(Spacer);

}

void WindowTitle::initButton()
{
	assert(!(m_buttonMin || m_buttonMax || m_buttonRestore || m_buttonClose));

	//// 最小化按钮
	m_buttonMin = new QPushButton( tr(""),this);
	m_buttonMin->setObjectName(tr(""));
	m_buttonMin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonMin->setStyleSheet("QPushButton { border: none; border-left: 1px solid white; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/minnormal);}"
		"QPushButton:hover{border-image: url(:/res/minhover);}"
		"QPushButton:pressed{border-image: url(:/res/minselect);}");
	m_buttonMin->setFixedWidth(28);
	m_buttonMin->setFixedHeight(28);

	connect(m_buttonMin, &QPushButton::clicked, [this]() {
					emit buttonClick(ButtonMin);
					});
	m_layout->addWidget(m_buttonMin);

	// 最大化按钮
	m_buttonMax = new QPushButton("",this);
	m_buttonMax->setObjectName("max");
	m_buttonMax->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonMax->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/maxnormal);}"
		"QPushButton:hover{border-image: url(:/res/maxhover);}"
		"QPushButton:pressed{border-image: url(:/res/maxselect);}");
	m_buttonMax->setFixedWidth(28);
	m_buttonMax->setFixedHeight(28);
	connect(m_buttonMax, &QPushButton::clicked, [this](bool) {
		emit buttonClick(ButtonMax);
				});
	m_layout->addWidget(m_buttonMax);

	// 还原按钮
	m_buttonRestore = new QPushButton( this);
	m_buttonRestore->setObjectName("Restore");
	m_buttonRestore->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonRestore->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/zoomnormal);}"
		"QPushButton:hover{border-image: url(:/res/zoomhover);}"
		"QPushButton:pressed{border-image: url(:/res/zoomselect);}");
	m_buttonRestore->setFixedWidth(28);
	m_buttonRestore->setFixedHeight(28);

	m_buttonRestore->hide();
	connect(m_buttonRestore, &QPushButton::clicked, [this]() {
		emit buttonClick(ButtonRestore);
	});
	m_layout->addWidget(m_buttonRestore);

	// 关闭按钮
	m_buttonClose = new QPushButton( this);
	m_buttonClose->setObjectName("Close");
	m_buttonClose->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_buttonClose->setStyleSheet("QPushButton { border: none; border-left: none; background-color: rgb(23,32,47);}"
		"QPushButton{border-image: url(:/res/closenormal);}"
		"QPushButton:hover{border-image: url(:/res/closehover);}"
		"QPushButton:pressed{border-image: url(:/res/closeselect);}");
	m_buttonClose->setFixedWidth(28);
	m_buttonClose->setFixedHeight(28);
	connect(m_buttonClose, &QPushButton::clicked, [this]() {
		emit buttonClick(ButtonClose);
	});
	m_layout->addWidget(m_buttonClose);

}



//响应双击标头,主界面最大化或者恢复一般大小
void WindowTitle::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		Qt::WindowStates states = m_window->windowState();
		if (states & Qt::WindowMaximized) 
			emit buttonClick(ButtonRestore);
		else
			emit buttonClick(ButtonMax);
	}
}


void WindowTitle::initMenu()
{
	//系统
	//加入菜单

	setContextMenuPolicy(Qt::ActionsContextMenu);

	//系统
	m_actionOne = new QAction(this);
	m_actionTwo = new QAction(this);
	m_actionThree = new QAction(this);
	m_actionFour = new QAction(this);

	m_actionOne->setText(tr("锁屏"));
	m_actionTwo->setText(tr("登录密码修改"));
	m_actionThree->setText(tr("资金密码修改"));
	m_actionFour->setText(tr("退出登录"));

	QMenu *pMenu = new QMenu("系统");
	pMenu->addAction(m_actionOne);
	pMenu->addAction(m_actionTwo);
	pMenu->addAction(m_actionThree);
	pMenu->addAction(m_actionFour);

	//2.加入弹簧
	QSpacerItem* Spacer = new QSpacerItem(100, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_layout->addItem(Spacer);

	//m_buttonSys->setMenu(pMenu);
	menuBar = new QMenuBar(this);
	menuBar->setStyleSheet("QMenuBar { font:12pt bold;font-family:'Microsoft YaHei';border: none; border-left: none; padding-top: 8px; background-color: #181a1d;}"
	"QMenuBar::item:enabled:selected {	background: rgba(255, 255, 255, 40);}"
	);

	menuBar->addMenu(pMenu);
	m_layout->addWidget(menuBar);
	menuBar->setGeometry(0, 0, 300, 40);
	menuBar->resize(300, 40);

	//操作
	m_action2One = new QAction(this);
	m_action2Two = new QAction(this);
	m_action2Three = new QAction(this);
	m_action2Four = new QAction(this);
	m_action2Five = new QAction(this);

	m_action2One->setText(tr("延期平仓试算"));
	m_action2Two->setText(tr("显示交收行情"));
	m_action2Three->setText(tr("显示交易状态"));
	m_action2Four->setText(tr("出入金"));
	m_action2Five->setText(tr("提货"));

	QMenu *pMenu2 = new QMenu("选项");
	pMenu2->addAction(m_action2One);
	pMenu2->addAction(m_action2Two);
	pMenu2->addAction(m_action2Three);
	pMenu2->addAction(m_action2Four);
	pMenu2->addAction(m_action2Five);

	//m_buttonOption->setMenu(pMenu2);
	menuBar->addMenu(pMenu2);

	//数据查询
	m_action3One = new QAction(this);
	m_action3Two = new QAction(this);
	m_action3Three = new QAction(this);
	m_action3Four = new QAction(this);
	m_action3Five = new QAction(this);
	//m_action3Six = new QAction(this);
	//m_action3Seven = new QAction(this);

	m_action3One->setText(tr("即时资金库存及持仓"));
	m_action3Two->setText(tr("日结单查询"));
	m_action3Three->setText(tr("月结单查询"));
	m_action3Four->setText(tr("强平单查询"));
	m_action3Five->setText(tr("会员公告查询"));
	//m_action3Six->setText(tr("当日查询"));
	//m_action3Seven->setText(tr("历史查询"));

	QMenu *pMenu3 = new QMenu("查询");
	
	//kenny  20171225  不重要的功能暂时屏蔽
	//pMenu3->addAction(m_action3One);
	pMenu3->addAction(m_action3Two);
	pMenu3->addAction(m_action3Three);
	pMenu3->addAction(m_action3Four);
	pMenu3->addAction(m_action3Five);
	/*pMenu3->addAction(m_action3Six);
	pMenu3->addAction(m_action3Seven);*/

	QMenu *pMenu_current = new QMenu;
	pMenu_current->setTitle("当日查询");
	m_action3Six_sub1 = new QAction(this);
	m_action3Six_sub2 = new QAction(this);
	m_action3Six_sub3 = new QAction(this);
	m_action3Six_sub4 = new QAction(this);

	m_action3Six_sub1->setText(tr("委托报单"));
	m_action3Six_sub2->setText(tr("成交查询查询"));
	m_action3Six_sub3->setText(tr("延期持仓明细"));
	m_action3Six_sub4->setText(tr("出入金明细"));
	pMenu_current->addAction(m_action3Six_sub1);
	pMenu_current->addAction(m_action3Six_sub2);
	pMenu_current->addAction(m_action3Six_sub3);
	pMenu_current->addAction(m_action3Six_sub4);


	QMenu *pMenu_history = new QMenu;
	pMenu_history->setTitle("历史查询");
	m_action3Seven_sub1 = new QAction(this);
	m_action3Seven_sub2 = new QAction(this);
	m_action3Seven_sub3 = new QAction(this);
	m_action3Seven_sub4 = new QAction(this);
	m_action3Seven_sub5 = new QAction(this);
	m_action3Seven_sub6 = new QAction(this);
	m_action3Seven_sub7 = new QAction(this);


	m_action3Seven_sub1->setText(tr("委托报单"));
	m_action3Seven_sub2->setText(tr("成交查询"));
	m_action3Seven_sub3->setText(tr("延期持仓明细"));
	m_action3Seven_sub4->setText(tr("库存查询"));
	m_action3Seven_sub5->setText(tr("资金变动流水查询"));
	m_action3Seven_sub6->setText(tr("资金查询"));
	m_action3Seven_sub7->setText(tr("费用明细"));

	pMenu_history->addAction(m_action3Seven_sub1);
	pMenu_history->addAction(m_action3Seven_sub2);
	pMenu_history->addAction(m_action3Seven_sub3);
	pMenu_history->addAction(m_action3Seven_sub4);
	pMenu_history->addAction(m_action3Seven_sub5);
	pMenu_history->addAction(m_action3Seven_sub6);
	pMenu_history->addAction(m_action3Seven_sub7);

	pMenu3->addMenu(pMenu_current);//当日查询
	pMenu3->addMenu(pMenu_history);//历史查询

	//m_buttonQuery->setMenu(pMenu3);
	menuBar->addMenu(pMenu3);
	//设置菜单
	m_action4One   = new QAction(this);
	m_action4Two   = new QAction(this);
	m_action4Three = new QAction(this);
	m_action4Four  = new QAction(this);


	//m_action4One->setText(tr("清除缓存数据"));
	m_action4Two->setText(tr("选项设置"));
	m_action4Three->setText(tr("隐藏资金信息"));
	m_action4Four->setText(tr("紧凑布局"));
	
	m_action4Three->setCheckable(true);
	connect(m_action4Three, &QAction::triggered, this, &WindowTitle::checkedAction);

	QMenu *pMenu4 = new QMenu("设置");
	//pMenu4->addAction(m_action4One);
	pMenu4->addAction(m_action4Two);
	pMenu4->addAction(m_action4Three);
	pMenu4->addAction(m_action4Four);

	//m_buttonSet->setMenu(pMenu4);
	menuBar->addMenu(pMenu4);
	//帮助
	m_action5One = new QAction(this);
	m_action5Two = new QAction(this);
	m_action5One->setText(tr("使用说明"));	
	m_action5Two->setText(tr("关于金通联"));

	QMenu *pMenu5 = new QMenu("帮助");
	pMenu5->addAction(m_action5One);
	pMenu5->addAction(m_action5Two);
	//m_buttonHelp->setMenu(pMenu5);
	menuBar->addMenu(pMenu5);
	//设置MEnu风格



#ifdef  _WIN32

        setWidgetStyleFromQssFile(pMenu, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu2, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu3, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu4, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu5, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu_current, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu_history, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\mainWindow.css"));
#else
        setWidgetStyleFromQssFile(pMenu,  _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu2, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu3, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu4, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu5, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu_current, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainWindow.css"));
        setWidgetStyleFromQssFile(pMenu_history, _RES_STYLES_PATH + _MY_TEXT("DeepBlack/mainWindow.css"));
#endif


	connect(m_actionOne, &QAction::triggered, this, &WindowTitle::actionOneSlot);
	connect(m_actionTwo, &QAction::triggered, this, &WindowTitle::actionTwoSlot);
	connect(m_actionThree, &QAction::triggered, this, &WindowTitle::actionThreeSlot);
	connect(m_actionFour, &QAction::triggered, this, &WindowTitle::actionFourSlot);

	connect(m_action2One, &QAction::triggered, this, &WindowTitle::action2OneSlot);
	connect(m_action2Two, &QAction::triggered, this, &WindowTitle::action2TwoSlot);
	connect(m_action2Three, &QAction::triggered, this, &WindowTitle::action2ThreeSlot);
	connect(m_action2Four, &QAction::triggered, this, &WindowTitle::action2FourSlot);
	connect(m_action2Five, &QAction::triggered, this, &WindowTitle::action2FiveSlot);

	connect(m_action3One, &QAction::triggered, this, &WindowTitle::action3OneSlot);
	connect(m_action3Two, &QAction::triggered, this, &WindowTitle::action3TwoSlot);
	connect(m_action3Three, &QAction::triggered, this, &WindowTitle::action3ThreeSlot);
	connect(m_action3Four, &QAction::triggered, this, &WindowTitle::action3FourSlot);
	connect(m_action3Five, &QAction::triggered, this, &WindowTitle::action3FiveSlot);
	//2个二级子菜单
	connect(m_action3Six_sub1, &QAction::triggered, this, &WindowTitle::action3Sixsub1Slot);
	connect(m_action3Six_sub2, &QAction::triggered, this, &WindowTitle::action3Sixsub2Slot);
	connect(m_action3Six_sub3, &QAction::triggered, this, &WindowTitle::action3Sixsub3Slot);
	connect(m_action3Six_sub4, &QAction::triggered, this, &WindowTitle::action3Sixsub4Slot);

	connect(m_action3Seven_sub1, &QAction::triggered, this, &WindowTitle::action3Sevensub1Slot);
	connect(m_action3Seven_sub2, &QAction::triggered, this, &WindowTitle::action3Sevensub2Slot);
	connect(m_action3Seven_sub3, &QAction::triggered, this, &WindowTitle::action3Sevensub3Slot);
	connect(m_action3Seven_sub4, &QAction::triggered, this, &WindowTitle::action3Sevensub4Slot);
	connect(m_action3Seven_sub5, &QAction::triggered, this, &WindowTitle::action3Sevensub5Slot);
	connect(m_action3Seven_sub6, &QAction::triggered, this, &WindowTitle::action3Sevensub6Slot);
	connect(m_action3Seven_sub7, &QAction::triggered, this, &WindowTitle::action3Sevensub7Slot);



	connect(m_action4One, &QAction::triggered, this, &WindowTitle::action4OneSlot);
	connect(m_action4Two, &QAction::triggered, this, &WindowTitle::action4TwoSlot);
	connect(m_action4Three, &QAction::triggered, this, &WindowTitle::action4ThreeSlot);
	connect(m_action4Four, &QAction::triggered, (mainWindow *)m_window, &mainWindow::ResetLayoutSlot);
	connect(m_action4Four, &QAction::triggered, this, &WindowTitle::action4FourSlot);

	connect(m_action5One, &QAction::triggered, this, &WindowTitle::action5OneSlot);
}


void WindowTitle::checkedAction()
{
	//
	if (m_action4Three->isChecked())
	{	
		m_action4Three->setText("显示资金信息");
		m_action4Three->setChecked(false);
	}
	else
	{
		m_action4Three->setText("隐藏资金信息");
		m_action4Three->setChecked(true);
	}
	
}

void WindowTitle::action4FourSlot()
{
	QString strtmp = m_action4Four->text();
	QString strcmp = "紧凑布局";
	if (strtmp.compare(strcmp) == 0)
	{
		m_action4Four->setText("传统布局");

	}
	else
	{
		m_action4Four->setText("紧凑布局");
	}

}
//锁屏
void WindowTitle::actionOneSlot()
{
	((mainWindow *)m_window)->handleOneSlot();
}

//登录密码修改
void WindowTitle::actionTwoSlot()
{
	((mainWindow *)m_window)->handleTwoSlot();
}

//资金密码修改
void WindowTitle::actionThreeSlot()
{
	((mainWindow *)m_window)->handleThreeSlot();
}

//退出
void WindowTitle::actionFourSlot()
{
	emit buttonClick(ButtonClose);
}




//操作
//延期平仓试算
void WindowTitle::action2OneSlot()
{
	((mainWindow *)m_window)->handle2OneSlot();
}

//显示交收行情
void WindowTitle::action2TwoSlot()
{
	((mainWindow *)m_window)->handle2TwoSlot();
}

//显示交易状态
void WindowTitle::action2ThreeSlot()
{
	((mainWindow *)m_window)->handle2ThreeSlot();
}

//出入金
void WindowTitle::action2FourSlot()
{
	((mainWindow *)m_window)->handle2FourSlot();

}

//提货
void WindowTitle::action2FiveSlot()
{
	((mainWindow *)m_window)->handle2FiveSlot();
}



//查询///////////////////////////////////////////////////////////////////////////////////////////
void WindowTitle::action3OneSlot()
{
	((mainWindow *)m_window)->handle3OneSlot();
}

//日结单查询
void WindowTitle::action3TwoSlot()
{
	((mainWindow *)m_window)->handle3TwoSlot();
}

//月结单查询
void WindowTitle::action3ThreeSlot()
{
	((mainWindow *)m_window)->handle3ThreeSlot();
}

//强平单查询 
void WindowTitle::action3FourSlot()
{
	((mainWindow *)m_window)->handle3FourSlot();
}

//会员公告查询
void WindowTitle::action3FiveSlot()
{
	((mainWindow *)m_window)->handle3FiveSlot();
}

//委托报单查询
void WindowTitle::action3Sixsub1Slot()
{
	((mainWindow *)m_window)->handle3Sixsub1Slot();
}

//成交查询
void WindowTitle::action3Sixsub2Slot()
{
	((mainWindow *)m_window)->handle3Sixsub2Slot();
}

//延期持仓查询
void WindowTitle::action3Sixsub3Slot()
{
	((mainWindow *)m_window)->handle3Sixsub3Slot();
}

//出入金明细
void WindowTitle::action3Sixsub4Slot()
{
	((mainWindow *)m_window)->handle3Sixsub4Slot();
}


/////////////////历史查询///////////////////////////
//委托报单查询
void WindowTitle::action3Sevensub1Slot()
{
	((mainWindow *)m_window)->handle3Sevensub1Slot();
}
//成交查询
void WindowTitle::action3Sevensub2Slot()
{
	((mainWindow *)m_window)->handle3Sevensub2Slot();
}

//延期持仓查询
void WindowTitle::action3Sevensub3Slot()
{
	((mainWindow *)m_window)->handle3Sevensub3Slot();
}
//库存查询
void WindowTitle::action3Sevensub4Slot()
{
	((mainWindow *)m_window)->handle3Sevensub4Slot();
}

//资金变动流水查询
void WindowTitle::action3Sevensub5Slot()
{
		((mainWindow *)m_window)->handle3Sevensub5Slot();
}

//资金查询
void WindowTitle::action3Sevensub6Slot()
{
	((mainWindow *)m_window)->handle3Sevensub6Slot();
}

//费用明细
void WindowTitle::action3Sevensub7Slot()
{
	((mainWindow *)m_window)->handle3Sevensub7Slot();
}


////////////////////////////////////////////end  查询//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////start  设置///////////////////////////////////////////////////////////////////
//设置
//清除缓存设置
void WindowTitle::action4OneSlot()
{
	((mainWindow *)m_window)->handle4OneSlot();
}

//选项设置
void WindowTitle::action4TwoSlot()
{
	((mainWindow *)m_window)->handle4TwoSlot();
}

//显示资金信息
void WindowTitle::action4ThreeSlot()
{
	if (m_action4Three->isChecked())
		m_action4Three->setChecked(false);
	else
		m_action4Three->setChecked(true);

	((mainWindow *)m_window)->ShowAccountBar();
}



//帮助
void WindowTitle::action5OneSlot()
{
	//((mainWindow *)m_window)->handle5OneSlot();
}

