#include "frmFeeDetailSearchDialog.h"
#include "TraderCpMgr.h"
#include "HJConstant.h"
#include "Macrodefs.h"
#include "HJGlobalFun.h"
#include <QDate>
#include <qdatetime.h>
#include "Global.h"
#include "frmOptionDlg.h"

frmFeeDetailSearchDialog::frmFeeDetailSearchDialog(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);  
	
	m_list    = ui.tableView;
	m_cmbNum  = ui.comboBox_num;
	m_cmbPage = ui.comboBox_page;
	m_tableModle = new QStandardItemModel();
	//setWidgetStyleFromQssFile(ui.tableView, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

	//setWindowFlags(Qt::CustomizeWindowHint);//语句A
	LoadList();

	ui.tableView->horizontalHeader()->setFixedHeight(32);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->verticalHeader()->setDefaultSectionSize(30);
	ui.tableView->setShowGrid(false);
	ui.tableView->setSortingEnabled(false);
	ui.tableView->setAlternatingRowColors(true);//隔行换色
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	//tableModel->setSortRole(1);
	ui.tableView->setModel(m_tableModle);

	// 初始化页码
	InitPage();

	// 初始化每页数目
	InitPageNum();

	//// 初始化查询条件的
    IniFeeCode(*ui.comboBox_code);


	ui.dateEdit_st->setDate(QDate::fromString(g_Global.m_strLast_Exch_Date,"yyyyMMdd"));
	ui.dateEdit_ed->setDate(QDate::fromString(g_Global.m_strLast_Exch_Date,"yyyyMMdd"));

	g_TraderCpMgr.Subscribe(E_UPDATE_LIST, this);

	connect(ui.pushButton_query, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonQuery()));
	connect(ui.pushButton_first, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonFirpage()));
	connect(ui.pushButton_pre, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonPrepage()));
	connect(ui.pushButton_next, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonNexpage()));
	connect(ui.pushButton_last, SIGNAL(clicked()), this, SLOT(OnBnClickedButtonLaspage()));
	connect(ui.pushButton_output_excel, SIGNAL(clicked()), this, SLOT(OnBnClickedOutTExcel()));
	connect(ui.pushButton_option, SIGNAL(clicked()), this, SLOT(OnBnClickedOption()));

	connect(ui.comboBox_num, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxNumChanged(QString)));
	connect(ui.comboBox_page, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxPageChanged(QString)));
	//connect(ui.comboBox_code, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnCbnSelchangeComboProdCode(QString)));

	setFixedSize(1031, 461);
	ui.widget->setGeometry(0, 0, 1031, 361);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("费用明细查询"));

}

frmFeeDetailSearchDialog::~frmFeeDetailSearchDialog()
{
}
void frmFeeDetailSearchDialog::OnBnClickedOption()
{
	frmOptionDlg dlg;
	dlg.selectTreeItem("26");
	dlg.exec();
}

void frmFeeDetailSearchDialog::OnBnClickedOutTExcel()
{
	OnOutputToExce(QStringLiteral("费用明细查询"));
}

void frmFeeDetailSearchDialog::comboBoxNumChanged(QString)
{
	OnSelchangeComboNum();
}


//查询按钮的处罚函数
void frmFeeDetailSearchDialog::comboBoxPageChanged(QString str)
{
	OnBnClickedButtonQuery();
}


void frmFeeDetailSearchDialog::OnBnClickedButtonQuery()
{
	OnButtonQuery();
}

//第一页按钮的触发函数
void frmFeeDetailSearchDialog::OnBnClickedButtonFirpage()
{
	OnButtonFirst();
}
//上一页按钮的触发函数
void frmFeeDetailSearchDialog::OnBnClickedButtonPrepage()
{
	OnButtonPrev();
}

//下一页按钮的触发函数
void frmFeeDetailSearchDialog::OnBnClickedButtonNexpage()
{
	OnButtonNext();
}

//最后一页按钮的触发函数
void frmFeeDetailSearchDialog::OnBnClickedButtonLaspage()
{
	OnButtonLast();
}


//接收自定义消息
void frmFeeDetailSearchDialog::customEvent(QEvent * event)
{
	if (event->type() == WM_UPDATE_LIST)
	{
		ClearList();
		LoadList();
	}
}


string frmFeeDetailSearchDialog::GetQueryID()
{
	return "AcctFareDetail";
}

void frmFeeDetailSearchDialog::GetQueryPara(QVector< pair<QString, QString> >  &vecPara)
{


	PairAdd(vecPara,"fee_code",CHJGlobalFun::str2qstr(GetFeeCode(*ui.comboBox_code))); //添加合约代码条件
	PairAdd(vecPara,"sub_fee_code",CHJGlobalFun::str2qstr(GetSubFeeCode(*ui.comboBox_sub_code))); //添加交易类型条件

	PairAdd(vecPara, "start_date", ui.dateEdit_st->date().toString("yyyyMMdd")); //添加开始日期查询条件对
	PairAdd(vecPara, "end_date", ui.dateEdit_ed->date().toString("yyyyMMdd"));    //添加结束日期查询条件对


}