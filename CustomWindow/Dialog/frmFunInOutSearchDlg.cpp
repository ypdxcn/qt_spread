#include "frmFunInOutSearchDlg.h"
#include "TraderCpMgr.h"
#include "HJConstant.h"
#include "Macrodefs.h"
#include "HJGlobalFun.h"
#include "frmOptionDlg.h"

frmFunInOutSearchDlg::frmFunInOutSearchDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	m_list = ui.tableView;
	m_cmbNum = ui.comboBox_num;
	m_cmbPage = ui.comboBox_page;
	m_tableModle = new QStandardItemModel();
	//setWidgetStyleFromQssFile(ui.tableView, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

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

	//// 初始化查询条件的combo
	//IniProdCode(*ui.comboBox_code);
	//IniExchCode(*ui.comboBox_type);
	//初始化查询条件的combo
	//IniProdCode(*ui.comboBox_code, "02");
	//IniLongShort(*ui.comboBox__buysell);

	//初始化查询条件的combo
	IniAccessWay(*ui.comboBox_inout);

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
	//connect(ui.comboBox_inout, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnCbnSelchangeComboProdCode(QString)));


	setFixedSize(1031, 461);
	ui.widget->setGeometry(0, 0, 1031, 361);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("出入金查询"));
}

frmFunInOutSearchDlg::~frmFunInOutSearchDlg()
{
}

void frmFunInOutSearchDlg::comboBoxNumChanged(QString)
{
	OnSelchangeComboNum();
}
void   frmFunInOutSearchDlg::OnBnClickedOption()
{
	frmOptionDlg dlg;
	dlg.selectTreeItem("17");
	dlg.exec();
}
void frmFunInOutSearchDlg::OnBnClickedOutTExcel()
{
	OnOutputToExce(QStringLiteral("出入金明细查询"));
}
//查询按钮的处罚函数
void frmFunInOutSearchDlg::comboBoxPageChanged(QString str)
{
	OnBnClickedButtonQuery();
}

// 合约代码下拉菜单的触发函数，对应的是交易类型的变化
//void frmFunInOutSearchDlg::OnCbnSelchangeComboProdCode(QString str)
//{
//	ProdCodeSelectChange(*ui.comboBox_code, *ui.comboBox__buysell);
//}

//void frmWTSearchDialog::OnCbnSelchangeComboTypeCode()
//{
//	ProdCodeSelectChange(ui.comboBox_code, ui.comboBox_type);
//}

void frmFunInOutSearchDlg::OnBnClickedButtonQuery()
{
	OnButtonQuery();
}

//第一页按钮的触发函数
void frmFunInOutSearchDlg::OnBnClickedButtonFirpage()
{
	OnButtonFirst();
}
//上一页按钮的触发函数
void frmFunInOutSearchDlg::OnBnClickedButtonPrepage()
{
	OnButtonPrev();
}

//下一页按钮的触发函数
void frmFunInOutSearchDlg::OnBnClickedButtonNexpage()
{
	OnButtonNext();
}

//最后一页按钮的触发函数
void frmFunInOutSearchDlg::OnBnClickedButtonLaspage()
{
	OnButtonLast();
}


//接收自定义消息
void frmFunInOutSearchDlg::customEvent(QEvent * event)
{
	if (event->type() == WM_UPDATE_LIST)
	{
		ClearList();
		LoadList();
	}
}


string frmFunInOutSearchDlg::GetQueryID()
{
	return "AcctFundAutoInOutFlow";
}

void frmFunInOutSearchDlg::GetQueryPara(QVector< pair<QString, QString> >  &vecPara)
{
	PairAdd(vecPara, "access_way", CHJGlobalFun::str2qstr(GetAccessWay(*ui.comboBox_inout))); //添加合约代码条件

}


void frmFunInOutSearchDlg::QueryReserver(const Req6002 &stReq6002, const Rsp6002 &stRsp6002)
{
	int iRewriteCount = 0;
	string sInFlag, sAccessWay, sSerialNo, sExchBal;
	for (int i = 0; i < stRsp6002.alm_result.size(); i++) //遍历返回结果
	{
		const ArrayListMsg &aMsg = stRsp6002.alm_result.GetValue(i); //获取结果的第一个组数据
		size_t count = m_almUserColumnIDs.size();  //aMsg.size();//获取每组数据的字段数
		for (int j = 0; j < count; j++) //插入该行的其他条
		{
			if (m_almUserColumnIDs.GetStringEx(j) == "in_account_flag")
			{
				sInFlag = aMsg.GetStringEx(j);
			}

			if (m_almUserColumnIDs.GetStringEx(j) == "access_way")
			{
				sAccessWay = aMsg.GetStringEx(j);
			}

			if (m_almUserColumnIDs.GetStringEx(j) == "serial_no")
			{
				sSerialNo = aMsg.GetStringEx(j);
			}

			if (m_almUserColumnIDs.GetStringEx(j) == "exch_bal")
			{
				sExchBal = aMsg.GetStringEx(j);
			}
		}

		// 如果该笔出入金为入账成功，则做处理
		if (sInFlag == "1")
		{
			bool bIn = sAccessWay == "1" ? true : false;

			if (g_TraderCpMgr.DealOneHandTransfer(CHJGlobalFun::str2qstr(sSerialNo), CHJGlobalFun::str2qstr(sExchBal), bIn))
			{
				iRewriteCount++;
			}
		}
	}

	if (iRewriteCount > 0)
	{
		g_TraderCpMgr.WriteTodayTransferToFile();
	}
}