#include "frmGetGoodsDlg.h"
#include "app.h"
#include "Global.h"
#include "TraderCpMgr.h"
#include "HJGlobalFun.h"
#include  <QMessageBox>
#include "Macrodefs.h"
#include "Mgr/TiHuoMgr.h"
#include "HJConstant.h"

frmGetGoodsDlg::frmGetGoodsDlg(QWidget *parent)
	: frmFramelessDialogBase(parent)
{
	ui.setupUi(this);

	//SetWindowText("提货");
	m_vCity.clear();
	m_nTakeDayDis = 5;
	// 是否显示提货日
	m_bShowTakeSheetDate = IsShowTakeSheetData();

	// 获取提货间隔日期
	//char cBuf[3] = { 0 };
	//::GetPrivateProfileString("info", "HJ.TakeDayDis", "5", cBuf, sizeof(cBuf), g_Global.GetSystemIniPath());
	//m_nTakeDayDis = atoi(cBuf);

	QString  buf;
	App::GetPriProfileString(g_Global.GetSystemIniPath(), QStringLiteral("info"), QStringLiteral("HJ.TakeDayDis"), buf);
	m_nTakeDayDis = buf.toInt();

	//CString csValueDis;
	//if( CIniMgr::GetStringValue( csValueDis, "info", "HJ.TakeDayDis", g_Global.GetSystemIniPath(), 3 ) )
	//	m_nTakeDayDis = atoi(csValueDis);

	// 初始化提货品种下拉菜单
	IniVarietyComboBox();

	// 初始化提货人下拉菜单，并设置第一个被选中
	//m_combperson.ResetContent();
	ui.comboBox_person->clear();
	for (size_t i = 0; i < g_Global.m_almTake_Man_List.size(); i++)
	{
		ArrayListMsg &aMsg = g_Global.m_almTake_Man_List.GetValue(i); // 获取结果的第一个组数据
		//ui.comboBox_person.InsertString(i, aMsg.GetStringEx(0).c_str());

		ui.comboBox_person->insertItem(i,CHJGlobalFun::str2qstr(aMsg.GetStringEx(0)));
	}
	QComboBox  test;
	test.setCurrentIndex(0);
	test.setEnabled(false);

	ui.comboBox_person->setCurrentIndex(0);

	if (ui.comboBox_person->count() < 2)
	{
		ui.comboBox_person->setEnabled(false);
		//m_combperson.EnableWindow(FALSE);
	}

	OnCbnSelchangeComboPerson();
	ui.lineEdit_pswd->setEchoMode(QLineEdit::Password);
	ui.lineEdit_ensure->setEchoMode(QLineEdit::Password);

	//m_ctlEditWeight.SetInt();
	//m_ctlEditPSW.SetMode(E_NUM);    
	//m_ctlEditCONPWD.SetMode(E_NUM);
	ui.label_info->setText("");
	//GetDlgItem(IDC_STATIC_INFO)->SetWindowText("");

	//GetDlgItem(IDC_STATIC_TAKE_DATE)->ShowWindow(m_bShowTakeSheetDate);
	//GetDlgItem(IDC_COMBO_TAKE_DATE)->ShowWindow(m_bShowTakeSheetDate);

	ui.comboBox_last_getgoods_day->setEnabled(m_bShowTakeSheetDate);
	if (m_bShowTakeSheetDate)
	{
		//m_cmbTakeDate.ResetContent();

		ui.comboBox_last_getgoods_day->clear();
		// 如果当前没有，则从服务器获取日货日
		if (((ArrayListMsg*)GetTakeShhetDate())->SizeEx2() < 1)
		{
			GetTakeSheetDateFromServer();
		}

		// 过滤合法的提货日加载到提货日下拉框
		LoadTakeSheetDateToComBox();

		// 如果不存在合法的提货日，则提示
		if (ui.comboBox_last_getgoods_day->count() == 0)
		{
			//QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("当前不是提货日，不能提货"));

			//AfxMessageBox("当前不是提货日，不能提货");
			//GetDlgItem(IDC_BUTTON_APPLYGOODS)->EnableWindow(FALSE);
			ui.pushButton_Apply->setEnabled(false);

			//m_cmbTakeDate.EnableWindow(FALSE);
			ui.comboBox_last_getgoods_day->setEnabled(false);
		}
		
	}

	// 查询相关----------------------------------------------------------------

	// 配置列表外观
	//CHJGlobalFun::SetListCtrl(m_1list1);

	//设置和添加list标题
	//const int iCount = 19;
	//CString sTitle[iCount] = { "提货流水号","提货单编号","提货品种", "提货重量(克)","提货城市", "提货仓库", 
	//	"提货状态", "提货申请日期","提货单到期日","提货保证金", "撤销日期", "撤销时间", "提货类型",
	//	"自提重量(克)","交提重量(克)","提货申请人","证件类型", "证件号码", "操作信息" };//2
	//int iAlign[iCount] = { LVCFMT_LEFT,  LVCFMT_LEFT, LVCFMT_LEFT,LVCFMT_RIGHT,  LVCFMT_LEFT,LVCFMT_LEFT,LVCFMT_LEFT,LVCFMT_LEFT,   LVCFMT_LEFT,   LVCFMT_RIGHT, LVCFMT_LEFT,LVCFMT_LEFT,LVCFMT_LEFT,LVCFMT_RIGHT,  LVCFMT_RIGHT,  LVCFMT_LEFT, LVCFMT_LEFT,LVCFMT_LEFT,LVCFMT_LEFT };
	//int iWidth[iCount] = { 80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80 };
	//CHJGlobalFun::AddListColumn(m_1list1, iCount, sTitle, iAlign, iWidth);

	tableModel = new QStandardItemModel();

	//setWidgetStyleFromQssFile(ui.tableView, _RES_STYLES_PATH + _MY_TEXT("DeepBlack\\ordertable.css"));

	////设置行
	tableModel->setColumnCount(19);
	int columnIndex = 0;
	tableModel->setHeaderData(columnIndex, Qt::Horizontal, QStringLiteral("提货流水号"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货单编号"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货品种"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货重量(克)"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货城市"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货仓库"));

	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货状态"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货申请日期"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货单到期日"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货保证金"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("撤销日期"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("撤销时间"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货类型"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("自提重量(克)"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("交提重量(克)"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("提货申请人"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("证件类型"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("证件号码"));
	tableModel->setHeaderData(++columnIndex, Qt::Horizontal, QStringLiteral("操作信息"));


	ui.tableView->horizontalHeader()->setFixedHeight(32);
	ui.tableView->verticalHeader()->hide();
	ui.tableView->verticalHeader()->setDefaultSectionSize(30);
	ui.tableView->setShowGrid(false);
	ui.tableView->setSortingEnabled(false);
	ui.tableView->setAlternatingRowColors(true);//隔行换色
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableModel->setSortRole(1);
	ui.tableView->setModel(tableModel);
	ui.tableView->setColumnWidth(0, 150);

	loadStore();

	// 从配置文件读取城市信息到内存
	loadCityFromFile();

	// 初始化提货城市下拉菜单
	IniCityComboBox();

	ui.comboBox_city->setCurrentIndex(0);
	OnChangeCity("");
	OnChangeType("");
	// 设置开始和结束时间为上一交易日
	//CTime ctime = CHJGlobalFun::TranslateDateStrToCTime(g_Global.m_strExchDate);

	QDateTime ctime = QDateTime::fromString(g_Global.m_strExchDate,"yyyyMMdd");
	if (!ctime.isNull())// ctime != NULL)
	{
		ui.dateEdit_st->setDateTime(ctime);
		ui.dateEdit_end->setDateTime(ctime);
		//m_timebegin.SetTime(&ctime);
		//m_timeend.SetTime(&ctime);
	}

	connect(ui.pushButton_Apply, SIGNAL(clicked()), this,SLOT(OnApplyGoods()));
	connect(ui.pushButton_search, SIGNAL(clicked()), this, SLOT(OnSearch()));
	connect(ui.pushButton_print, SIGNAL(clicked()), this, SLOT(OnPrint()));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

	connect(ui.comboBox_city, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnChangeCity(QString)));
	connect(ui.comboBox_type, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnChangeType(QString)));


	setFixedSize(1001, 661);
	ui.widget->setGeometry(0, 0, 1001, 561);
	setContent(ui.widget);
	setWindowTitle(QStringLiteral("提货"));
}

frmGetGoodsDlg::~frmGetGoodsDlg()
{
}

void frmGetGoodsDlg::OnChangeType(QString city)
{
	//根据选择品种获取该品种最小提货量和提货步长
	QString cstr;
	int min_pickup  = 0;
	int pickup_base = 0;

	cstr = ui.comboBox_type->currentText();//.GetLBText(m_combgoodstype.GetCurSel(), cstr);
	/*string str(cstr.GetBuffer());*/
	for (size_t i = 0; i < g_TraderCpMgr.m_vVariety.size(); i++)
	{
		if (cstr == g_TraderCpMgr.m_vVariety.at(i).name)
		{
			min_pickup = (int)g_TraderCpMgr.m_vVariety.at(i).min_pickup;
			pickup_base = (int)g_TraderCpMgr.m_vVariety.at(i).pickup_base;
			break;
		}
	}

	QString cstrTips = QStringLiteral("提货重量最小为%1克，且为%2的整数倍。").arg(min_pickup).arg(pickup_base);
	//cstrTips.Format("提货重量最小为%d克，且为%d的整数倍。", min_pickup, pickup_base);
	//GetDlgItem(IDC_STATIC_INFO)->SetWindowText(cstrTips);
	ui.label_info->setText(cstrTips);
}

void frmGetGoodsDlg::OnChangeCity(QString city)
{
	QString cstr;

	// 加载所有的仓库信息到内存
	if (m_vStore.size() == 0)
	{
		if (!loadStore())
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("加载仓库信息失败"));
		}
	}

	// 获取选中的城市代码
	int index = ui.comboBox_city->currentIndex();//.GetCurSel();
	QString citycode = m_vCity.at(index).city_code.c_str();

	// 清空仓库下拉菜单

	ui.comboBox_db->clear();
	// 遍历仓库列表，将在城市的仓库添加到仓库列表
	for (size_t i = 0; i < m_vStore.size(); i++)
	{
		cstr = CHJGlobalFun::str2qstr(m_vStore.at(i).city_code);//获得当前仓库的对应的城市id列表（以逗号隔开，注意这里可以包含多个id，容易误解）
		if (cstr.contains(citycode)  && m_vStore.at(i).is_take == "1")//如果包含城市id，且允许提货，则添加到仓库的下拉菜单
			ui.comboBox_db->addItem(  CHJGlobalFun::str2qstr(m_vStore.at(i).stor_name));
	}

}


bool frmGetGoodsDlg::IsShowTakeSheetData()
{
	/*char cBuf[2] = { 0 };
	::GetPrivateProfileString("info", "HJ.ShowTakeSheetDate", "0", cBuf, sizeof(cBuf), );*/
	//QString  strIniPath = g_Global.GetSystemPath() + CONSTANT_SYSTEM_INI_NAME;
	QString  buf;
	App::GetPriProfileString(g_Global.GetSystemIniPath(), QStringLiteral("info"), QStringLiteral("HJ.ShowTakeSheetDate"), buf);
	return buf.contains("1") ? true : false;
}



//初始化交割品种下拉菜单
void frmGetGoodsDlg::IniVarietyComboBox()
{
	ui.comboBox_type->clear();
	//m_combgoodstype.ResetContent();

	int n = 0;
	for (size_t i = 0; i < g_TraderCpMgr.m_vVariety.size(); i++)
	{
		// 根据金联通测试的要求，去掉该品种
		if (g_TraderCpMgr.m_vVariety.at(i).name != "Ag99.9")
		{
			ui.comboBox_type->insertItem(n, g_TraderCpMgr.m_vVariety.at(i).name);
			ui.comboBox_type->setItemData(n, QVariant::fromValue<int>(i));
			//m_combgoodstype.InsertString(n, g_TraderCpMgr.m_vVariety.at(i).name.c_str());
			//m_combgoodstype.SetItemData(n, (DWORD)i);

			n++;
		}
	}
}
// 提货人改变时的触发函数，改变证件编号和证件类型
void frmGetGoodsDlg::OnCbnSelchangeComboPerson()
{
	int index = ui.comboBox_person->currentIndex();//.GetCurSel();

	if (index == -1 || g_Global.m_almTake_Man_List.size() == 0)
	{
		return;
	}

	// 获得当前提货人的提货信息
	ArrayListMsg aMsg = g_Global.m_almTake_Man_List.GetValue(index);

	// 设置当前提货人的证件编号
	//m_editidcode = aMsg.GetStringEx(2).c_str();
	ui.lineEdit_seq->setText(CHJGlobalFun::str2qstr(aMsg.GetStringEx(2)));
	//UpdateData(false);

	// 获得当前提货人的证件类型id，可能有多个，用逗号分隔（注：一个人怎么可能有多个id呢？奇怪）
	QString cstr = CHJGlobalFun::str2qstr(aMsg.GetStringEx(1));

	QVector<QString> vec;
	CHJGlobalFun::SplitStrToVector(cstr, ",", vec);

	//m_combidtype.ResetContent();
	ui.comboBox_credit->clear();
	for (size_t i = 0; i < vec.size(); i++)
	{
		ui.comboBox_credit->insertItem(i, g_TraderCpMgr.GetIdTypeName(vec.at(i)));
		//m_combidtype.InsertString(i, g_TraderCpMgr.GetIdTypeName(CHJGlobalFun::CStringToString(vec.at(i))).c_str());
	}

	// 设置第一个被选中
	ui.comboBox_credit->setCurrentIndex(0);
	//m_combidtype.SetCurSel(0);
	if (ui.comboBox_credit->count() < 2)
	{
		//m_combidtype.EnableWindow(false);
		ui.comboBox_credit->setEnabled(false);
	}
}

ArrayListMsg* frmGetGoodsDlg::GetTakeShhetDate()
{
	return &m_alm_take_sheet_date;

}



void frmGetGoodsDlg::GetTakeSheetDateFromServer()
{
	Req5041 req; //请求报文体
	req.oper_flag = 1;
	req.branch_id = g_HeaderReq.branch_id;
	req.is_search_usefull = CONSTANT_YES_NO_YES;
	req.search_num = "10";

	Rsp5041 rsp; //应答报文体
	if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "5041"))
	{
		//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS));
		return;
	}

	if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
	{
		//g_Global.m_alm_take_sheet_date = rsp.alm_take_sheet_date;
		*(ArrayListMsg*)GetTakeShhetDate() = rsp.alm_take_sheet_date;
	}
}


void frmGetGoodsDlg::LoadTakeSheetDateToComBox()
{
	ArrayListMsg* pALM = GetTakeShhetDate();

	// 获取提货日的个数
	size_t iSize = pALM->size();

	// 获取提货的开始日期
	//QTime cTakeBegin = CHJGlobalFun::TranslateDateStrToCTime(g_Global.m_strExchDate);
	QDateTime cTakeBegin =  QDateTime::fromString(g_Global.m_strExchDate, "yyyy-MM-dd");
	if (!cTakeBegin.isNull())//cTakeBegin != NULL)
	{
		// 获取结束日期
		QDateTime cTakeEnd = cTakeBegin;
		cTakeEnd.addDays(m_nTakeDayDis);// += QDateSpan(m_nTakeDayDis, 0, 0, 0);

		// 遍历
		for (size_t i = 0; i < iSize; i++) //遍历返回结果
		{
			//ArrayListMsg aMsg = pALM->GetValue(i); //获取结果的第一个组数据
			string sDate = pALM->GetStringEx(i);

			// 判断日期是否在开始日期和结束日期之间
			//CTime cCmp = CHJGlobalFun::TranslateDateStrToCTime(sDate);
			QDateTime cCmp = QDateTime::fromString(g_Global.m_strExchDate, "yyyy-MM-dd");
			if (!cCmp.isNull() && cCmp >= cTakeBegin && cCmp <= cTakeEnd)
			{
				//m_cmbTakeDate.InsertString(i, sDate.c_str());
				ui.comboBox_last_getgoods_day->insertItem(i, CHJGlobalFun::str2qstr(sDate));
			}
		}
	}
}


// 从配置文件加载仓库信息到内存
bool frmGetGoodsDlg::loadStore()
{
	// 数据配置文件路径
	QString strIniPath = g_Global.GetSystemPath() + CONSTANT_DATA_INI_NAME;

	// 读取配置文件中的仓库信息
	//char buf[10000] = { 0 };
	//::GetPrivateProfileString("Info", "store_code", "", buf, sizeof(buf), strIniPath);
	QString buf;
	App::GetPriProfileString(strIniPath, QStringLiteral("Info"), QStringLiteral("store_code"), buf);
	int iLen = buf.toInt();//strlen(buf);

	if (buf.count() != 0)
	{
		string str = CHJGlobalFun::qstr2str(buf);
		// 解析字符型到hashtable
		HashtableMsg htmStore;
		if (htmStore.Parse(str) == 0)
		{
			IniStoreInfo(htmStore);
			return true;
		}
		else
		{
			//AfxMessageBox("解析配置文件中仓库信息错误！");
			return false;
		}
	}
	else // 如果配置文件中仓库信息为空，则向服务器发送报文，获取仓库信息
	{
		Req1006 req;
		req.oper_flag = 1;

		Rsp1006 rsp;
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "1006"))
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS));

			//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
			return false;
		}

		if (!g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
		{
			return false;
		}
		else
		{
			//记录仓库信息
			IniStoreInfo(rsp.htm_stor_info);

			//写入仓库信息到文件
			//::WritePrivateProfileString("Info", "store_code", rsp.htm_stor_info.ToString().c_str(), strIniPath);

			App::WritePriProfileString(strIniPath, "Info", "store_code", CHJGlobalFun::str2qstr(rsp.htm_stor_info.ToString()));
			return true;
		}
	}
}


// 加载所有的仓库
void frmGetGoodsDlg::IniStoreInfo(HashtableMsg &htm)
{
	for (size_t i = 0; i < htm.size(); i++)
	{
		StoreInfoEx storeinfo;
		storeinfo.stor_id = htm.GetString(i, "stor_id");
		storeinfo.stor_name = htm.GetString(i, "stor_name");
		storeinfo.addr = htm.GetString(i, "addr");
		storeinfo.tel = htm.GetString(i, "tel");
		storeinfo.fax = htm.GetString(i, "fax");
		storeinfo.link_man = htm.GetString(i, "link_man");
		storeinfo.zip_code = htm.GetString(i, "zip_code");
		storeinfo.use_variety_type = htm.GetString(i, "use_variety_type");
		storeinfo.is_take = htm.GetString(i, "is_take");
		storeinfo.city_code = htm.GetString(i, "city_code");

		m_vStore.push_back(storeinfo);
	}
}


// 从配置文件加载城市信息到内存
bool frmGetGoodsDlg::loadCityFromFile()
{
	// 数据配置文件路径
	QString strIniPath = g_Global.GetSystemPath() + CONSTANT_DATA_INI_NAME;

	// 如果配置文件中的城市信息不为空，则读取
	//char buf[10000] = { 0 };
	//::GetPrivateProfileString("Info", "city_code", "", buf, sizeof(buf), strIniPath);

	QString buf;
	App::GetPriProfileString(strIniPath, QStringLiteral("Info"), QStringLiteral("city_code"), buf);


	if (buf.count() != 0)
	{
		//解析字符型到hashtable
		string str = CHJGlobalFun::qstr2str(buf);
		HashtableMsg htmCity;
		if (htmCity.Parse(str) == 0)
		{
			// 如果解析成功，则加载城市信息到内存
			IniCityInfo(htmCity);
			return true;
		}
		else
		{
			//AfxMessageBox("解析配置文件中城市信息错误！");
			return false;
		}
	}
	else // 如果配置文件中仓库信息为空，则向服务器发送报文，获取仓库信息
	{
		Req1007 req;
		req.oper_flag = 1;

		Rsp1007 rsp;
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "1007"))
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS));

			//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
			return false;
		}

		if (!g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
		{
			return false;
		}
		else
		{
			// 加载城市信息到内存
			IniCityInfo(rsp.htm_code_list);

			// 写入城市信息到文件
			//::WritePrivateProfileString("Info", "city_code", rsp.htm_code_list.ToString().c_str(), strIniPath);

			QString buf;
			App::WritePriProfileString(strIniPath, QStringLiteral("Info"), QStringLiteral("city_code"), CHJGlobalFun::str2qstr(rsp.htm_code_list.ToString()));




			return true;
		}
	}
}

// 加载城市信息到内存
void frmGetGoodsDlg::IniCityInfo(HashtableMsg &htm)
{
	for (size_t i = 0; i < htm.size(); i++)
	{
		CityInfo cityinfo;
		cityinfo.all_stor_id = htm.GetString(i, "all_stor_id");
		cityinfo.city_code = htm.GetString(i, "city_code");
		cityinfo.city_name = htm.GetString(i, "city_name");
		cityinfo.stor_id_default = htm.GetString(i, "stor_id_default");

		// added by Jerry Lee, 2013-6-27, 过滤掉没有提货仓库的城市
		for (int j = 0; j < m_vStore.size(); j++)
		{
			if (m_vStore[j].city_code.find(cityinfo.city_code) != string::npos)
			{
				m_vCity.push_back(cityinfo);
				break;
			}
		}
	}
}


// 初始化提货城市下拉菜单
void frmGetGoodsDlg::IniCityComboBox()
{
	ui.comboBox_city->clear();
	//m_combcity.ResetContent();
	for (size_t i = 0; i < m_vCity.size(); i++)
	{
		//m_combcity.InsertString(i, m_vCity.at(i).city_name.c_str());
		ui.comboBox_city->insertItem(i, CHJGlobalFun::str2qstr( m_vCity.at(i).city_name));
	}

}


bool frmGetGoodsDlg::CheckInput()
{
	//判断combo控件是否选择

	int index;

	//是否选择城市
	index = ui.comboBox_city->currentIndex();
	if (index == -1)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请选择提货城市"));

		//AfxMessageBox("请选择提货城市");
		//GetDlgItem(IDC_COMBO_CITY)->SetFocus();
		ui.comboBox_city->setFocus();
		return false;
	}

	//是否选择提货仓库
	//index = m_combbarn.GetCurSel();
	index = ui.comboBox_db->currentIndex();


	if (index == -1)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请选择提货仓库"));
		//AfxMessageBox("请选择提货仓库");
		//GetDlgItem(IDC_COMBO_BARN)->SetFocus();
		ui.comboBox_db->setFocus();
		return false;
	}

	//是否选择提货人
	//index = m_combperson.GetCurSel();
	index = ui.comboBox_person->currentIndex();

	if (index == -1)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请选择提货人"));
		//AfxMessageBox("请选择提货人");
		//GetDlgItem(IDC_COMBO_PERSON)->SetFocus();
		ui.comboBox_person->setFocus();
		return false;
	}

	///是否选择提货类型
	//index = m_combidtype.GetCurSel();
	index = ui.comboBox_credit->currentIndex();

	if (index == -1)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请选择证件类型"));

		//AfxMessageBox("请选择证件类型");
		///GetDlgItem(IDC_COMBO_IDTYPE)->SetFocus();
		ui.comboBox_credit->setFocus();
		return false;
	}

	//是否选择品种
	//index = m_combgoodstype.GetCurSel();
	index = ui.comboBox_type->currentIndex();

	if (index == -1)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请选择交割品种"));

		//AfxMessageBox("请选择交割品种");
		//GetDlgItem(IDC_COMBO_GOODSTYPE)->SetFocus();
		ui.comboBox_type->setFocus();
		return false;
	}

	//判断提货重量是否正确
	if (!IsWeightRight())
	{
		//GetDlgItem(IDC_EDIT_WEIGHT)->SetFocus();

		ui.lineEdit_weight->setFocus();
		return false;
	}

	//证件编号校验
	if (!CheckIDCard())
	{
		//GetDlgItem(IDC_EDIT_IDCODE)->SetFocus();
		ui.lineEdit_seq->setFocus();
		return false;
	}

	//密码验证
	if (!CheckPassword())
	{
		//GetDlgItem(IDC_EDIT_PSW)->SetFocus();
		ui.lineEdit_pswd->setFocus();
		return false;
	}

	//

	// 
	if (m_bShowTakeSheetDate)
	{
		if (ui.comboBox_last_getgoods_day->currentIndex() == -1)
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请选择提货日"));

			//AfxMessageBox("请选择提货日");
			//GetDlgItem(IDC_COMBO_TAKE_DATE)->SetFocus();
			ui.comboBox_last_getgoods_day->setFocus();

			return false;
		}
	}

	return true;
}


void frmGetGoodsDlg::OnApplyGoods()
{
	if (CheckInput())
	{
		if (QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("确实要申请提货吗?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) //弹出确认窗口
		{
			Req5101 req; //请求报文体

			//业务设置
			req.oper_flag = 2;
			req.app_record = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
			req.acct_no = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
			req.cust_id = CHJGlobalFun::qstr2str(g_Global.m_strUserID);

			//绑定控件信息

			int index;
			QString text;

			// 提货城市代码
			index = ui.comboBox_city->currentIndex();
			req.city_code = m_vCity.at(index).city_code;

			// 提货仓库代码 
			index = ui.comboBox_db->currentIndex();
			text = ui.comboBox_db->currentText();

			req.stor_id = GetStoreId(CHJGlobalFun::qstr2str(text));

			//提货品种id
			index = ui.comboBox_db->currentIndex();

			int nIndex = ui.comboBox_type->itemData(index).toInt();//(int)m_combgoodstype.GetItemData(index);
			req.variety_id = CHJGlobalFun::qstr2str(g_TraderCpMgr.m_vVariety.at(nIndex).variety_id);
			//申请提货重量
			req.app_amount = CHJGlobalFun::qstr2str(ui.lineEdit_weight->text());

			//提货人
			index = ui.comboBox_person->currentIndex();
			//index = m_combperson.GetCurSel();
			//m_combperson.GetLBText(index, text);
			req.take_man = CHJGlobalFun::qstr2str(ui.comboBox_person->currentText());

			//证件类型
			index = ui.comboBox_credit->currentIndex();
			//m_combidtype.GetLBText(index, text);
			req.cert_type_id = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetIdTypeId(ui.comboBox_credit->currentText()));

			//证件编号
			req.cert_num = CHJGlobalFun::qstr2str(ui.lineEdit_seq->text());
		
			//密码  
			req.app_pwd = CHJGlobalFun::qstr2str(ui.lineEdit_pswd->text());

			if (m_bShowTakeSheetDate)
			{

				index = ui.comboBox_last_getgoods_day->currentIndex();
				//m_cmbTakeDate.GetLBText(index, text);
				req.take_date = CHJGlobalFun::qstr2str(ui.comboBox_last_getgoods_day->currentText());
			}

			Rsp5101 rsp; //应答报文体
			if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "5101"))
			{
				QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral(CONSTANT_CONNECT_SERVER_FAIL_TIPS));
				//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
				return;
			}

			if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
			{
				QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("已向交易所发送提货申请，您可以点击查询按钮重新查询提货申请列表！"));
				//AfxMessageBox("已向交易所发送提货申请，您可以点击查询按钮重新查询提货申请列表！");
			}
		}
	}
}




// 根据仓库名称获取仓库id
string frmGetGoodsDlg::GetStoreId(const string &sName)
{
	for (size_t i = 0; i < m_vStore.size(); i++)
	{
		if (m_vStore.at(i).stor_name == sName)
			return m_vStore.at(i).stor_id;
	}

	return sName;
}








// 判断用户输入的提货重量是否正确
bool frmGetGoodsDlg::IsWeightRight()
{
	int min_pickup=0;
	int pickup_base=0;
	int input=0;
	QString cstr;

	//根据选择品种获取该品种最小提货量和提货步长
	//m_combgoodstype.GetLBText(m_combgoodstype.GetCurSel(), cstr);
	cstr = ui.comboBox_type->currentText();
	//string str = CHJGlobalFun::qstr2str(cstr);
	for (size_t i = 0; i < g_TraderCpMgr.m_vVariety.size(); i++)
	{
		if (cstr == g_TraderCpMgr.m_vVariety.at(i).name)
		{
			min_pickup = (int)g_TraderCpMgr.m_vVariety.at(i).min_pickup;
			pickup_base = (int)g_TraderCpMgr.m_vVariety.at(i).pickup_base;
			break;
		}
	}

	input = ui.lineEdit_weight->text().toInt();//atoi(m_editweight);
	if (input < min_pickup)//是否满足最小提货量
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("输入提货重量小于最小提货重量"));

		//AfxMessageBox("");
		return false;
	}
	else
	{
		if ((input - min_pickup) % pickup_base != 0)//是否满足提货步长
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("输入提货重量不满足步长要求"));

			//AfxMessageBox("");
			return false;
		}
	}
	return true;
}
//检查密码输入的正确性
bool frmGetGoodsDlg::CheckPassword()
{
	//判断密码是否输入正确（空提示，是否是数字）
	if (ui.lineEdit_pswd->text() == "")
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("密码不能为空"));

		//AfxMessageBox("");
		//GetDlgItem(IDC_EDIT_PSW)->SetFocus();
		ui.lineEdit_pswd->setFocus();
		return false;
	}
	if (!CHJGlobalFun::IsNumber(ui.lineEdit_pswd->text()))
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("密码必须为数字"));

		//AfxMessageBox("");
		//GetDlgItem(IDC_EDIT_PSW)->SetFocus();
		ui.lineEdit_pswd->setFocus();
		return false;
	}

	//判断密码是否输入正确（空提示，是否是数字）
	if (ui.lineEdit_ensure->text() == "")
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("确认密码不能为空"));

		//AfxMessageBox("");
		//GetDlgItem(IDC_EDIT_CONPWD)->SetFocus();
		ui.lineEdit_ensure->setFocus();
		return false;
	}
	if (!CHJGlobalFun::IsNumber(ui.lineEdit_ensure->text()))
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("确认密码必须为数字"));

		//AfxMessageBox("确认密码必须为数字");
		//GetDlgItem(IDC_EDIT_CONPWD)->SetFocus();

		ui.lineEdit_ensure->setFocus();
		return false;
	}

	//判断确认密码是否正确（与密码比较）
	if (ui.lineEdit_pswd->text() != ui.lineEdit_ensure->text())
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("两次输入密码不一致"));

		//AfxMessageBox("两次输入密码不一致!");
		//GetDlgItem(IDC_EDIT_CONPWD)->SetFocus();
		ui.lineEdit_ensure->setFocus();
		return false;
	}

	return true;
}

//检查用户id输入的正确性
bool frmGetGoodsDlg::CheckIDCard()
{
	//判断证件编号是否正确（是身份证号码的时候检查身份证号码是否正确，空提示）
	//int len = m_editidcode.GetLength();
	int len = ui.lineEdit_seq->text().count();
	if (len == 0)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请输入证件编号"));

		//AfxMessageBox("请输入证件编号");
		return false;
	}

	//判断输入的是否只是有数字和字母组成

	//判断当前是否选择了身份证，如果是，则判断是否是15位或者18位，如果不是则返回false，如果是，则校验身份证的合法性

	return true;
}
//判断15为身份证号码是否正确
bool frmGetGoodsDlg::CheckIDCard15(QString Id)
{
	//判断是否全部是数字
	if (!CHJGlobalFun::IsNumber(Id))
		return false;

	//判断省份是否正确
	QString address = "11x22x35x44x53x12x23x36x45x54x13x31x37x46x61x14x32x41x50x62x15x33x42x51x63x21x34x43x52x64x65x71x81x82x91";
	if (!address.contains(Id.left(2)))
		return false;

	//判断出生年月是否正确
	if (!IsRightDate("19" + Id.mid(6, 6)))
		return false;

	return true;
}
//判断18为身份证号码是否正确
bool frmGetGoodsDlg::CheckIDCard18(QString Id)
{
	//判断是否全部是数字
	if (!CHJGlobalFun::IsNumber(Id.left(17)))
		return false;

	//判断省份是否正确
	QString address = "11x22x35x44x53x12x23x36x45x54x13x31x37x46x61x14x32x41x50x62x15x33x42x51x63x21x34x43x52x64x65x71x81x82x91";
	if (!address.contains(Id.left(2)) )
		return false;

	//判断出生年月是否正确
	if (!IsRightDate(Id.mid(6, 8)))
		return false;

	//判断校验码
	int     Wi[17] = { 7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2 };//从左边到到第17位的对应位的乘数
	char fchar[11] = { '1','0','x','9','8','7','6','5','4','3','2' };//余数对应的参照码
	int sum = 0;
	for (int i = 0; i < 17; i++)
	{
		char mychar = Id.at(i).toLatin1();
		sum += (mychar - char('0')) * Wi[i];
	}
	sum = sum % 11;
	if (fchar[sum] != Id.at(17))
		return false;

	return true;
}
/*
判断一个日期是否正确，用于身份证中日期的校验
input:datestr（格式：19850101)
*/
bool frmGetGoodsDlg::IsRightDate(QString datestr)
{
	int year, month, day;
	bool isloop;
	int loopyear[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };//闰年的月份天数
	int normalyear[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	//获得int类型的年月日
	year = datestr.left(4).toInt();
	month = datestr.mid(4, 2).toInt();
	day = datestr.mid(6, 2).toInt();

	//判断月份和日期的基本正确性
	if (month < 1 || month > 12 || day < 1 || day > 31)
		return false;

	//判断是否是闰年
	isloop = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

	//根据闰年是否判断每月的日期是否正确，月份的必须不大于参照
	if (isloop && loopyear[month - 1] < month)
		return false;
	if (!isloop && normalyear[month - 1] < month)
		return false;

	return true;
}





void frmGetGoodsDlg::OnSearch()
{
	//清空list控件
	//m_1list1.DeleteAllItems();
	tableModel->removeRows(0, tableModel->rowCount());
	Rsp5101 rsp; //应答报文体


	//if (g_TiHuoMgr.Query(rsp,
	//	CHJGlobalFun::GetFormatTimeStr(m_timebegin),
	//	CHJGlobalFun::GetFormatTimeStr(m_timeend),
	//	""))

		if (g_TiHuoMgr.Query(rsp,
			CHJGlobalFun::qstr2str(ui.dateEdit_st->text()),
			CHJGlobalFun::qstr2str(ui.dateEdit_end->text()),
			""))
	{
		for (size_t i = 0; i < rsp.htm_take_sheet_detail.size(); i++)
		{
			string value[19];
			value[0] = rsp.htm_take_sheet_detail.GetString(i, "local_sheet_no");           // 提货流水号
			value[1] = rsp.htm_take_sheet_detail.GetString(i, "sheet_no");                 // 提货单编号
			value[2] = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetVarietyName(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "variety_id"))));   // 提货品种
			value[3] = rsp.htm_take_sheet_detail.GetString(i, "app_amount");               // 提货重量(克)
			value[4] = rsp.htm_take_sheet_detail.GetString(i, "city_code");                // 提货城市
			value[5] = GetStoreName(rsp.htm_take_sheet_detail.GetString(i, "stor_id"));    // 提货仓库
			value[6] = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetTake_statName(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "stat"))));   // 提货状态
			value[7] = CHJGlobalFun::qstr2str(CHJGlobalFun::FormatDate(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "app_date"))));     // 提货申请日期
			value[8] = CHJGlobalFun::qstr2str(CHJGlobalFun::FormatDate(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "app_end_date")))); // 提货单到期日
			value[9] = CHJGlobalFun::qstr2str(CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "take_margin")))); // 提货保证金
			value[10] = CHJGlobalFun::qstr2str(CHJGlobalFun::FormatDate(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "cancel_date"))));  // 撤销日期
			value[11] = CHJGlobalFun::qstr2str(CHJGlobalFun::FormatTime(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "cancel_time"))));  // 撤销时间
			value[12] = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetDraw_typeName(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "draw_type"))));  // 提货类型
			value[13] = rsp.htm_take_sheet_detail.GetString(i, "self_draw");          // 自提重量(克)
			value[14] = rsp.htm_take_sheet_detail.GetString(i, "trade_draw");         // 交提重量(克)
			value[15] = rsp.htm_take_sheet_detail.GetString(i, "take_man");           // 提货申请人
			value[16] = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetIdTypeName(CHJGlobalFun::str2qstr(rsp.htm_take_sheet_detail.GetString(i, "cert_type_id")))); // 证件类型
			value[17] = rsp.htm_take_sheet_detail.GetString(i, "cert_num");          // 证件号码
			value[18] = rsp.htm_take_sheet_detail.GetString(i, "rsp_msg");           // 操作信息

			tableModel->insertRow(i);
			for (int j = 0; j < 19; j++)
				tableModel->setItem(i, j, new QStandardItem(CHJGlobalFun::str2qstr(value[j])));
			/*m_1list1.InsertItem(i, value[0].c_str());
			for (int j = 1; j < 19; j++)
				m_1list1.SetItemText(i, j, value[j].c_str());*/
		}
	}
}
void frmGetGoodsDlg::OnPrint()
{
	int index = ui.tableView->currentIndex().row();//m_1list1.GetSelectionMark();//
	if (index == -1)
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请先选中项"));
		//AfxMessageBox("请先选中项");
	}
	else
	{
		QString strSheet_No = tableModel->item(index, 1)->text();//m_1list1.GetItemText(index, 1); //获取提货单编号
		if (strSheet_No == "")
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("该提货单没有申请成功！不能打印"));
			//AfxMessageBox("该提货单没有申请成功！不能打印");
			return;
		}

		// 则获取选中行的提货状态（名称），如果状态不为申请成功，则给出提示
		QString  strState = tableModel->item(index, 6)->text();//m_1list1.GetItemText(index, 6); // 获取提货单状态
		string strStateID =CHJGlobalFun::qstr2str( g_TraderCpMgr.GetTake_statID(strState));
		if (strStateID != CONSTANT_B_SHEET_STAT_SUCCESS)
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("该提货单没有申请成功！不能打印"));
			//AfxMessageBox("该提货单没有申请成功！不能打印");
			return;
		}

		//发送报文
		Req5103 req; //请求报文体
		req.oper_flag = 1;
		req.sheet_no = CHJGlobalFun::qstr2str(strSheet_No);  //提货单编号   测试数据"TH201205210042"    

		Rsp5103 rsp; //应答报文体
		if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "5103"))
		{
			QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("CONSTANT_CONNECT_SERVER_FAIL_TIPS"));
			//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
			return;
		}

		if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg)))
		{
			/*CPrintFrame *pFrame = new CPrintFrame((CDialog*)AfxGetMainWnd());

			pFrame->m_pPrintView->m_iType = E_TIHUO;
			//初始化打印数据
			pFrame->m_pPrintView->IniTiHuoData(rsp);

			pFrame->DoPrintView();*/
		}
	}
}
void frmGetGoodsDlg::OnCancel()
{
	//int index = m_1list1.GetSelectionMark();
	int index = ui.tableView->currentIndex().row();
	if (index != -1)
	{
		if(QMessageBox::information(nullptr,QStringLiteral("提示"),QStringLiteral("确实要撤销申请吗?"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes )
		//if (MessageBox("确实要撤销申请吗?", "提示", MB_YESNO | MB_ICONINFORMATION) == IDYES) //弹出确认窗口
		{
			Req5101 req; //请求报文体
			req.oper_flag = 3;
			req.local_sheet_no = CHJGlobalFun::qstr2str(tableModel->item(0, 1)->text());//提货流水号
			req.acct_no = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
			req.take_man = CHJGlobalFun::qstr2str(tableModel->item(0, 1)->text());//提货申请人

			Rsp5101 rsp; //应答报文体
			if (0 != g_TraderCpMgr.HandlePacket(rsp, req, "5101"))
			{
				QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("CONSTANT_CONNECT_SERVER_FAIL_TIPS"));
				//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
				return;
			}

			if (g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr( rsp.rsp_msg)))
			{
				QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("已向交易所发送撤单申请，您可点击查询按钮重新查询提货申请列表！"));
				
				//AfxMessageBox("已向交易所发送撤单申请，您可点击查询按钮重新查询提货申请列表！");
			}
		}
	}
	else
	{
		QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("请先选中项"));
		//AfxMessageBox("请先选中项");
	}
}



// 根据仓库ID获取仓库名称
string frmGetGoodsDlg::GetStoreName(const string &sID)
{
	for (size_t i = 0; i < m_vStore.size(); i++)
	{
		if (m_vStore.at(i).stor_id == sID)
			return m_vStore.at(i).stor_name;
	}

	return sID;
}