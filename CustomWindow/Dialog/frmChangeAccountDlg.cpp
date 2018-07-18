#include "frmChangeAccountDlg.h"
#include "HJDef.h"
#include "TraderCpMgr.h"
#include "Global.h"
#include "app.h"



frmChangeAccountDlg::frmChangeAccountDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);
	//m_parent = (mainWindow *)parent;
	//setFixedSize(360, 361);
	ui.widget->setGeometry(0,0,420, 310);
	setContent(ui.widget);

	setWindowTitle(QString::fromLocal8Bit("多账户设置"));

	
	ui.tableView->horizontalHeader()->setFixedHeight(32);//设置表头的高度
	ui.tableView->horizontalHeader()->setFixedWidth(420);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->verticalHeader()->setDefaultSectionSize(25);//设置默认行高
	ui.tableView->setShowGrid(false);////设置不显示格子线
	ui.tableView->setFrameShape(QFrame::NoFrame); //设置无边框
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);//设置选择行为时每次选择一行
	ui.tableView->setSortingEnabled(false);
	ui.tableView->setAlternatingRowColors(true);//隔行换色
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setEditTriggers(QAbstractItemView::SelectedClicked);


	m_pmodel = new QStandardItemModel();

	m_pmodel->setColumnCount(3);

	m_pmodel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("用户别名"));
	m_pmodel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("交易编码"));
	m_pmodel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("密码"));

	//ui.tableView->setColumnWidth(0, 150);
	//ui.tableView->setColumnWidth(1, 180);
	//ui.tableView->setColumnWidth(2, 150);
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	ui.tableView->setModel(m_pmodel);

	init();

	connect(ui.Button_add, &QPushButton::clicked, this, &frmChangeAccountDlg::slotAdd);
	connect(ui.Button_delete, &QPushButton::clicked, this, &frmChangeAccountDlg::slotDelete);
	connect(ui.okButton, &QPushButton::clicked, this, &frmChangeAccountDlg::slotOk);
}


frmChangeAccountDlg::~frmChangeAccountDlg()
{
}


void frmChangeAccountDlg::slotAdd()
{
	int count = m_pmodel->rowCount();
	m_pmodel->insertRow(count);
	m_pmodel->setItem(count, 0, new QStandardItem("ALIAS"));
	m_pmodel->setItem(count, 1, new QStandardItem("USER_SAMPLE"));
	m_pmodel->setItem(count, 2, new QStandardItem("PAWD_SAMPLE"));
	
}

void frmChangeAccountDlg::slotDelete()
{
	m_pmodel->removeRow(ui.tableView->currentIndex().row());
}

//初始化读入数据
void frmChangeAccountDlg::init()
{
	for (int i = 0; i < App::accMgr.size(); i++)
	{
		m_pmodel->insertRow(i);
		m_pmodel->setItem(i, 0, new QStandardItem(App::accMgr[i].alias));
		m_pmodel->setItem(i, 1, new QStandardItem(App::accMgr[i].user));
		m_pmodel->setItem(i, 2, new QStandardItem(App::accMgr[i].psw));
	}
	
}


void frmChangeAccountDlg::slotOk()
{
	App::accMgr.clear();

	App::WriteServerProperty("account","ACCOUNT.count", QString::number(m_pmodel->rowCount()));
	//保存到INI
	for (int i = 0; i < m_pmodel->rowCount(); i++)
	{
		AccountMgr acc;
		acc.alias = m_pmodel->item(i, 0)->text();
		acc.user  = m_pmodel->item(i, 1)->text();
		acc.psw   = m_pmodel->item(i, 2)->text();
		QString strTmp = m_pmodel->item(i, 0)->text();
		
		App::WriteServerProperty("account", QString("ACCOUNT.account%1.alias").arg(QString::number(i + 1)), acc.alias);
		App::WriteServerProperty("account", QString("ACCOUNT.account%1.user").arg(QString::number(i + 1)), acc.user);
		App::WriteServerProperty("account", QString("ACCOUNT.account%1.pswd").arg(QString::number(i + 1)), acc.psw);

		App::accMgr.push_back(acc);
	}
	
	accept();
}
void frmChangeAccountDlg::slotCancel()
{
	reject();
}

