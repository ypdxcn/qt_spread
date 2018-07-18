//#include "stdafx.h"
#include "HJQueryBase.h"
//#include "HJSetColumnDlg.h"
//#include "excel.h"
#include "MyExcel.h"
//#include "Option.h"
#include "HJConstant.h"
#include "Global.h"
#include "HJGlobalFun.h"
#include "app.h"
#include "inifilesmanager.h"
#include <QMessageBox>


#pragma execution_character_set("utf-8")


struct PageNumST
{
    char Name[32];
    int Num;
};

const PageNumST PAGENUMS[] = {
    {"每页10条记录", 10},
    {"每页20条记录", 20},
    {"每页50条记录", 50},
    {"每页100条记录", 100}
};

// 对齐方式：1（左对齐） 3（右对齐）

CHJQueryBase::CHJQueryBase( void )
{
	//ui->tableView->setModel(m_tableModle);
}

CHJQueryBase::~CHJQueryBase( void )
{

}

void CHJQueryBase::InitPage()
{
	int num = m_cmbPage->count();
	for (int i = num-1; i >= 0; num--)
	{
		m_cmbPage->removeItem(num);
	}
	//m_cmbPage->clear();
	m_cmbPage->insertItem(0, ("第1页"));
	//m_cmbPage->setItemData();//.AddString("第1页");
	m_cmbPage->setCurrentIndex(0);
}

int CHJQueryBase::GetCurPage()
{
	int iPage = m_cmbPage->currentIndex();//.GetCurSel(); //获取当前页数
	iPage++;
	if (iPage <= 0)
		iPage = 1;
	return iPage;
}

void CHJQueryBase::InitPageNum()
{

    for(int i = 0; i < (sizeof(PAGENUMS)/sizeof(PAGENUMS[0])); i++)
    {

        m_cmbNum->insertItem(i, PAGENUMS[i].Name);

    }
	
	m_cmbNum->setCurrentIndex(0);
}

int CHJQueryBase::GetPageNum()
{
	int index = m_cmbNum->currentIndex();
	if(index == -1) index = 0;
    // mod by Jerry Lee, 修改为本地固定的记录条数, 2013-3-5
	//return atoi(g_TraderCpMgr.m_vPageInfo.at(index).code_id.c_str());
    return PAGENUMS[index].Num;
	/*
	switch(index)
	{
	case 0: return 10;
	case 1: return 20;
	case 2: return 50;
	}

	return 10;
	*/
}

void CHJQueryBase::SetComboxCurPage( EPageBtnType ePagebtnType )
{
	int cursel;
	if(ePagebtnType == E_FIRST) // 首页
	{
		m_cmbPage->setCurrentText(0);
		return;
	}
	else if(ePagebtnType == E_LAST)  // 最后一页
	{
		int count = m_cmbPage->count();//.GetCount();
		cursel = --count;
	}
	else // type为2或者3
	{
		cursel = m_cmbPage->currentIndex();//.GetCurSel();
		if(ePagebtnType == E_FORWARD) // 上一页
		{
			cursel--;
		}
		else // 下一页
		{
			cursel++;
		}
	}

	m_cmbPage->setCurrentIndex(cursel);
}

void CHJQueryBase::ResetComboxPage( const int &iTotal, int &iCurPage )
{
	int atemp = m_cmbPage->count();
	
    for (; atemp >= 0; atemp--)
	{
		m_cmbPage->removeItem(atemp);
		// cb->setItemText (atemp, "ddd");
	 }

	//m_cmbPage->removeItem   clear();// ResetContent();
	QString cstr;
	for(int i = 0 ; i < iTotal; i++)
	{
		cstr =  QString(("第%1页")).arg(i+1);
		//cstr.sprintf(QStringLiteral("第%d页"), i+1);
		m_cmbPage->insertItem(i, cstr);
	}
	
	m_cmbPage->setCurrentIndex(--iCurPage);

}

void CHJQueryBase::OnButtonFirst()
{
	if(m_tableModle->rowCount()/* .GetItemCount()*/ != 0)
	{
		if(m_cmbPage->currentIndex()/* GetCurSel()*/ == 0)
		{
			//AfxMessageBox("已经是首页");
			QMessageBox::information(nullptr, ("系统提示"), ("已经是首页!"));
		}
		else
		{
			// 设置为首页
			SetComboxCurPage(E_FIRST);
			OnButtonQuery();
		}
	}
}

void CHJQueryBase::OnButtonPrev()
{
	if(m_tableModle->rowCount() != 0)
	{
		if(m_cmbPage->currentIndex() == 0)
		{
			QMessageBox::information(nullptr, ("系统提示"), ("已经是首页!"));
		}
		else
		{
			SetComboxCurPage(E_FORWARD); // 设置为上一页
			OnButtonQuery();
		}
	}
}

void CHJQueryBase::OnButtonNext()
{
	if(m_tableModle->rowCount() != 0)
	{
		int count = m_cmbPage->count();//.GetCount(); // 获取选项的总个数
		count--;
		if(count == m_cmbPage->currentIndex())
		{
			QMessageBox::information(nullptr, ("系统提示"), ("已经是末页!"));
			return;
		}
		else
		{
			SetComboxCurPage(E_NEXT);// 设置为下一页
			OnButtonQuery();
		}
	}
}

void CHJQueryBase::OnButtonLast()
{
	if(m_tableModle->rowCount() != 0)
	{
		if(m_cmbPage->count() == m_cmbPage->currentIndex() + 1)
		{
			QMessageBox::information(nullptr, ("系统提示"), ("已经是末页!"));
			return;
		}
		else
		{
			SetComboxCurPage(E_LAST);// 设置为最后一页
			OnButtonQuery();
		}
	}
}

void CHJQueryBase::OnButtonQuery()
{
	Req6002 stReq6002;
	Rsp6002 stRsp6002;

	/*
	if(m_bHistory)
	{
		CTime timeBegin,timeEnd;
		((CDateTimeCtrl*)GetDlgItem(DATETIME_BEGIN_CONTROL_ID))->GetTime(timeBegin);  // 获取起始日期和结束日期
		((CDateTimeCtrl*)GetDlgItem(DATETIME_END_CONTROL_ID))->GetTime(timeEnd);      // 获取结束日期

		if(timeBegin > timeEnd)
		{
			AfxMessageBox(CONSTANT_BEGIN_DATE_BIGGER);
			return;
		}
	}
	*/

	//m_tableModle->clear();//.DeleteAllItems(); //清空list控件

	m_tableModle->removeRows(0, m_tableModle->rowCount());

	// 获取子类的查询条件
	QVector< pair<QString, QString> >  vecPara;
	GetQueryPara(vecPara);

	// 获取当前选中的页数和每页量
	int iCurPage = GetCurPage();   
	int iPageNum = GetPageNum();

	// 向服务器提交查询请求
    stRsp6002.page_count = 1;
    stRsp6002.curr_page  = 1;

	if (Query(GetQueryID(), vecPara, iPageNum, iCurPage, stReq6002, stRsp6002))
	{
		ShowQueryResult(stReq6002, stRsp6002);
	}

	// 重新加载页码下拉菜单和当前选中页码
	//ResetComboxPage(stRsp6002.page_count, stRsp6002.curr_page);
}

void CHJQueryBase::OnButtonQuery( QDateTime &dtBegin, QDateTime &dtEnd )
{
	QDate timeBegin,timeEnd;
	dtBegin.setDate(timeBegin);
	dtEnd.setDate(timeEnd);

	if(timeBegin > timeEnd)
	{		
		QMessageBox::information(nullptr, ("系统提示"), (CONSTANT_BEGIN_DATE_BIGGER));
		return;
	}
	else
	{
		OnButtonQuery();
	}
}

void CHJQueryBase::LoadList()
{
	m_vecColumnIndexs.clear();

	// 获取配置文件的路径
	QString strPath;
	string strQueryID = GetQueryID(); // ???????ID,????????????

#ifdef _WIN32
	strPath = QString("%1QueryConfig\\%2.ini").arg(g_Global.GetSystemPath()).arg(CHJGlobalFun::str2qstr(strQueryID));

#else
    strPath = QString("%1QueryConfig/%2.ini").arg(g_Global.GetSystemPath()).arg(CHJGlobalFun::str2qstr(strQueryID));
#endif

	QString strValue;
	QString strAligns;
	size_t i;
	QVector<QString> vecAllNames;
	QVector<int>     vecAllWidths;
	QVector<int>     vecAllAligns;
	QVector<QString> vecUserName;
	QVector<int>     vecUserWidths;
	QVector<int>     vecUserAligns;

	//char buf[2000] = {0};

	// 读取用户选择列的ID，并且分割到vector
	//::GetPrivateProfileString("Info", "UserColumnIndexs", "", buf, sizeof(buf), strPath);
	//strValue = buf;

	App::GetPriProfileString(strPath, ("Info"), ("UserColumnIndexs"), strValue);
	CHJGlobalFun::SplitStrToVector(strValue, (QUERY_INI_SPLITER), m_vecColumnIndexs);

	// 读取全部字段名称，并分割到vector
	//::GetPrivateProfileString("Info", "AllColumnNames", "", buf, sizeof(buf), strPath);
	App::GetPriProfileString(strPath, ("Info"), ("AllColumnNames"), strValue);

	//strValue = buf;
	CHJGlobalFun::SplitStrToVector(strValue, (QUERY_INI_SPLITER), vecAllNames);

	// 读取全部字段宽度，并分割到vector
	//::GetPrivateProfileString("Info", "AllColumnWidths", "", buf, sizeof(buf), strPath);
	//strValue = buf;
	App::GetPriProfileString(strPath, ("Info"), ("AllColumnWidths"), strValue);
	CHJGlobalFun::SplitStrToVector(strValue, (QUERY_INI_SPLITER), vecAllWidths);

	// 读取全部字段对其方式，并分割到vector
	//::GetPrivateProfileString("Info", "AllColumnAligns", "", buf, sizeof(buf), strPath);
	//strValue = buf;
	App::GetPriProfileString(strPath, ("Info"), ("AllColumnAligns"), strValue);

	CHJGlobalFun::SplitStrToVector(strValue, (QUERY_INI_SPLITER), vecAllAligns);

	// 遍历位置字段，获取用户选择的每一列的对应的信息（列宽、对齐方式）
	for(i = 0; i < m_vecColumnIndexs.size(); i++)
	{
		// 加载标题名称
		int iPos = m_vecColumnIndexs.at(i);
		vecUserName.push_back(vecAllNames.at(iPos));

		// 加载对其方式
		int iAlign;
		if(vecAllAligns.at(iPos) == 1)
		{
			iAlign = Qt::AlignLeft;//LVCFMT_LEFT;
		}
		else if(vecAllAligns.at(iPos) == 3)
		{
			iAlign = Qt::AlignRight;
		}
		else
		{
			iAlign = Qt::AlignCenter;
		}
		vecUserAligns.push_back(iAlign);

		// 加载列宽
		vecUserWidths.push_back(vecAllWidths.at(iPos));
	}


	// 加载标题到list
	m_tableModle->setColumnCount(vecUserName.size());
	for(i = 0; i < vecUserName.size(); i++)
	{
		m_tableModle->setHeaderData(i, Qt::Horizontal, vecUserName.at(i));

		//m_list->column(i, vecUserName.at(i), vecUserAligns.at(i), vecUserWidths.at(i));

	}

	// 获取用户选择的字段ID，记录到ArraylistMsg
	GetUserColumnIDs(strPath);
}

void CHJQueryBase::GetUserColumnIDs( const QString &strPath )
{
	QVector<QString> vecAllColumnIDs;
	QString strValue;
	//char buf[2000] = {0};
	// 读取全部字段名称，并分割到vector
	//::GetPrivateProfileString("Info", "AllColumnIDs", "", buf, sizeof(buf), strPath);
	//strValue = buf;

	App::GetPriProfileString(strPath, ("Info"), ("AllColumnIDs"), strValue);

	CHJGlobalFun::SplitStrToVector(strValue, (QUERY_INI_SPLITER), vecAllColumnIDs);

	QString strUserColumnIDs  = "";

	// 遍历位置字段
	for(size_t i = 0; i < m_vecColumnIndexs.size(); i++)
	{
		int iPos = m_vecColumnIndexs.at(i);

		// 添加一个字段ID
		strUserColumnIDs += vecAllColumnIDs.at(iPos) + (QUERY_INI_SPLITER);
	}

	// 解析
	m_almUserColumnIDs.clear();
	m_almUserColumnIDs.Parse(CHJGlobalFun::qstr2str(strUserColumnIDs));
}

void CHJQueryBase::ClearList()
{
	// 清空list数据
	m_tableModle->clear();

	m_tableModle->columnCount();
	// 获取列的个数
	int nColumnCount = m_tableModle->columnCount();//m_list->verticalHeader()->count();//.GetHeaderCtrl()->GetItemCount();
	// 清楚列
	m_tableModle->removeColumns(0, nColumnCount);

}

bool CHJQueryBase::Query( const string &strQueryTag, QVector< pair<QString,QString> > & vecPara, const int &iPageNum/*每页的数量*/, const int &iCurPage/*当前请求的页码*/, Req6002 &stReq6002, Rsp6002 &stRsp6002 )
{
	HEADER_RSP stHeaderRsp;

	stReq6002.login_branch_id = g_HeaderReq.branch_id;
	stReq6002.login_teller_id = CHJGlobalFun::qstr2str(g_Global.m_strUserID);
	stReq6002.oper_flag       = 1;  //不变
	stReq6002.paginal_num     = iPageNum;  //总页数
	stReq6002.curr_page       = iCurPage;   //当前页数
	stReq6002.query_id        = strQueryTag; //查询类型
	stReq6002.alm_view_field  = m_almUserColumnIDs;

	g_HeaderReq.SetExchCode("6002");
	g_HeaderReq.SetSeqNo(g_SeqNo ++);

	if ( 0 != g_TraderCpMgr.Tran6002Handle(g_HeaderReq, stReq6002, stHeaderRsp, stRsp6002, vecPara, 20)) //连接6002，返回查询信息
	{
		//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
		QMessageBox::information(nullptr, ("系统提示"), (CONSTANT_CONNECT_SERVER_FAIL_TIPS));

		return false;
	}

	return true;
}

void CHJQueryBase::ShowQueryResult( const Req6002 &stReq6002, const Rsp6002 &stRsp6002 )
{
	if(stRsp6002.alm_result.size() == 0)
	{
		//AfxMessageBox(CONSTANT_QUERY_NO_RECORD);
		QMessageBox::information(nullptr, ("系统提示"), (CONSTANT_QUERY_NO_RECORD));

		return;
	}
	size_t i;

	//根据查询返回来的数据填充list控件
	for(i = 0; i < stRsp6002.alm_result.size(); i++) //遍历返回结果
	{  
		const ArrayListMsg &aMsg = stRsp6002.alm_result.GetValue(i); //获取结果的第一个组数据
		size_t count = m_almUserColumnIDs.size();  //aMsg.size();//获取每组数据的字段数
		for(size_t j = 0; j < count; j++) //插入该行的其他条
		{
			string titleid  =  m_almUserColumnIDs.GetStringEx(j);//获得字段id
			QString value = CHJGlobalFun::str2qstr( aMsg.GetStringEx(j) );//获得字段值
			if(titleid == "exch_date" || titleid == "bk_plat_date" || titleid == "stor_date")//交易时间
				value = CHJGlobalFun::FormatDate(value);
			else if(titleid == "market_id")//交易市场
				value = g_TraderCpMgr.GetMarketName(value);
			else if(titleid == "exch_code")//交易类型
				value = g_TraderCpMgr.GetExchName(value);
			else if(titleid == "offset_flag")//开平标志
				value = g_TraderCpMgr.GetOffset_flagName(value);
			else if(titleid == "deli_flag")//交收标志，待处理
				value = g_TraderCpMgr.GetDeli_flagName(value);
			else if(titleid == "bs")//买卖方向
				value = g_TraderCpMgr.GetBuySellName(value);
			else if(titleid == "entr_stat")//委托状态
				value = g_TraderCpMgr.GetEntr_statName(value);
			else if(titleid == "cancel_flag")//撤单标志
				value = g_TraderCpMgr.GetCancel_flagName(value);
			else if(titleid == "access_way")//操作方向
				value = g_TraderCpMgr.GetAccessWayName(value);
			else if(titleid == "check_stat1" || titleid == "check_stat2")//复核状态
				value = g_TraderCpMgr.GetCheck_staName(value);
			else if(titleid == "in_account_flag")//是否成功
				value = g_TraderCpMgr.GetIn_account_flagName(value);
			else if(titleid == "long_short")//持仓方向
				value = g_TraderCpMgr.GetLongShortName(value);
			else if(titleid == "e_term_type" || titleid == "c_term_type" || titleid == "term_type" || titleid == "o_term_type")//委托渠道,撤销渠道,操作来源
				value = g_TraderCpMgr.GetTerm_typeName(value);
			else if(/*titleid == "market_id" || */titleid == "accept_time" || titleid == "e_exch_time" || titleid == "c_exch_time" ||titleid == "exch_time" ||titleid == "stor_time")//撤销时间
				value = CHJGlobalFun::FormatTime(value);
			else if(IsFloatTitle(titleid,stReq6002.query_id))//委托价格
				value = CHJGlobalFun::FormatFloat(value);

			if(j == 0)
			{
				//m_list.InsertItem(i, value.c_str()); //插入每一行的第一条

				m_tableModle->insertRow(i);
				m_tableModle->setItem(i, 0, new QStandardItem(value));
			}
			else
			{
				//m_list.SetItemText(i, j, value.c_str()); //向list控件插入该数据	
				m_tableModle->setItem(i, j, new QStandardItem(value));
			}
		}
	}

	QueryReserver(stReq6002, stRsp6002);
}

bool CHJQueryBase::IsFloatTitle( const string &strID, const string &sQueryID /*= ""*/ )
{
	if( sQueryID == "AcctFundHis" )
	{
		if( strID == "curr_can_use" || strID == "last_can_use" || strID == "curr_can_get")
		{
			return true;
		}
	}

	const int iCount = 53;
	/*
	去掉last_long_froz,last_can_use,curr_can_use,curr_can_get
    modify by xrs 20130204 添加diff_storage（入库溢短总量）、unclea r_diff（未清算入库溢短）两个字段，在金联通测试环境展示为-.9的形式
	*/
	string title[iCount] = {
		"entr_price", "match_price", "exch_bal", "exch_fare", "margin", "long_open_avg_price", "short_open_avg_price", "long_posi_avg_price", "short_posi_avg_price", "last_settle_price", 
		"day_settle_price", "storage_cost", "last_bal", "curr_bal", "last_margin", "last_reserve", "out_bal","in_bal","real_buy","real_sell",
		"real_reserve","real_margin","base_margin","last_base_margin","deli_prepare","last_deli_prepare","deli_margin","last_deli_margin","real_exch_fare","other_fare",
		"pay_breach","take_breach","cov_surplus","mark_surplus","float_surplus","day_long_froz ","last_forward_froz","day_forward_froz","inte_integral","puni_integral",
		"wait_incr_inte","wait_incr_inte_tax","day_incr_inte","day_incr_inte_tax","last_take_margin","day_take_margin","last_stor_fare_froz", "day_stor_fare_froz", "stor_price", "price", 
		"fee_value",    "diff_storage", "unclear_diff" };//
	// "last_can_use","curr_can_use","curr_can_get",

	for(int i = 0; i < iCount; i++)
	{
		if(title[i] == strID)
		{
			return true;
		}
	}
	return false;
}

void CHJQueryBase::OnBtnSetshowcolumn(QString strNodeDate /*= ""*/)
{
// 	CHJSetColumnDlg dlg;
// 	dlg.SetFileName(GetQueryID().c_str());
	//COption dlg(strNodeDate);
	//if(dlg.DoModal() == IDOK)
	{
// 		ClearList();
// 
// 		LoadList();
	}
}
/*
初始化合约代码
marketid默认为空，为空的时候取全部，否则取相应marketid的品种
marketid 00为  01为 02为
*/
void CHJQueryBase::IniProdCode(QComboBox& comb,string marketid )
{
	string str;
	QString cstr;

	comb.clear();
	comb.addItem( (""));
	for( size_t i = 0; i < g_TraderCpMgr.m_vProdCode.size(); i++)
	{
		const ProdCodeInfo &prodecode = g_TraderCpMgr.m_vProdCode.at(i);
		bool add = true;
		if(marketid != "")//如果不为空，则根据条件获取
		{
			if (prodecode.market_id == CHJGlobalFun::str2qstr(marketid))
				add = true;
			else
				add = false;
		}

		if(add)//如果需要添加
		{
			if( prodecode.active_flag == "1" )
			{
				//str  = CHJGlobalFun::qstr2str(prodecode.prod_code);
				cstr = prodecode.prod_code;
				comb.addItem(cstr);
			}
		}
	}
}
// 获取合约代码对应的id，由于列表的值就是需要获取的，故直接取值即可
string CHJQueryBase::GetProdCode(QComboBox& comb)
{
	int index;
	QString cstr;

	index = comb.currentIndex();
	if(index < 0)//如果选择为空，则返回空值
		return "";


	return CHJGlobalFun::qstr2str(comb.itemText(index));
}

// 初始化交易类型
void CHJQueryBase::IniExchCode(QComboBox& comb)
{
	comb.clear();

	comb.insertItem( 0, "");
    // mod by Jerry Lee, 屏蔽远期开多仓和远期开空仓
    int n = 1;
	for(size_t i = 0; i < g_TraderCpMgr.m_vExchCode.size(); i++)
	{
        if(g_TraderCpMgr.m_vExchCode.at(i).code_id != CONSTANT_EXCH_CODE_FORWARD_LONG
            && g_TraderCpMgr.m_vExchCode.at(i).code_id != CONSTANT_EXCH_CODE_FORWARD_SHORT)
        {
            //comb.InsertString(n, g_TraderCpMgr.m_vExchCode.at(i).code_desc.c_str());

			comb.insertItem(0, g_TraderCpMgr.m_vExchCode.at(i).code_desc);

            n++;        
        }


	}
}

// 获取交易类型
QString CHJQueryBase::GetExchCode(QComboBox& comb)
{
	int iPos;
	QString cstr;
	//获得当前选择项
	iPos = comb.currentIndex();//.GetCurSel();
	if(iPos < 1)
		return "";
	
	QString str = comb.itemText(iPos);
	//遍历vec，获得该项对应的idf
	for(size_t i = 0; i < g_TraderCpMgr.m_vExchCode.size(); i++)
	{
		if(g_TraderCpMgr.m_vExchCode.at(i).code_desc == str)
			return g_TraderCpMgr.m_vExchCode.at(i).code_id;
	}

	return "";
}

// 初始化开平标志
void CHJQueryBase::IniOffsetFlag(QComboBox& comb)
{
	comb.clear();

	comb.insertItem(0, "");
	for(size_t i = 0; i < g_TraderCpMgr.m_vOffsetFlag.size(); i++)
	{
		comb.insertItem(i+1, g_TraderCpMgr.m_vOffsetFlag.at(i).code_desc);
	}
}
// 获取开平标志
string CHJQueryBase::GetOffsetFlag(QComboBox& comb)
{
	QString text = "";
	int index = comb.currentIndex();
	if(index > 0)
		return CHJGlobalFun::qstr2str(g_TraderCpMgr.m_vOffsetFlag.at(index-1).code_id);
	else//如果没选或者选中第一项
		return "";

}
// 初始化持仓方向
void CHJQueryBase::IniLongShort(QComboBox& comb)
{
	comb.clear();

	comb.insertItem(0, "");
	for(size_t i = 0; i < g_TraderCpMgr.m_vLongShort.size(); i++)
	{
		//QString cstr = g_TraderCpMgr.m_vLongShort.at(i).code_desc.c_str();
		comb.insertItem(i+1, g_TraderCpMgr.m_vLongShort.at(i).code_desc);
	}
}

// 获取持仓方向
string CHJQueryBase::GetLongShort(QComboBox& comb)
{
	int index = comb.currentIndex();
	if(index < 1)//如果没选或者选中第一个
		return "";
	else
		return CHJGlobalFun::qstr2str(g_TraderCpMgr.m_vLongShort.at(index-1).code_id);
}
// 初始化存取方向
void  CHJQueryBase::IniAccessWay(QComboBox& comb)
{
	comb.clear();

	comb.insertItem(0, "");
	for(size_t i = 0; i < g_TraderCpMgr.m_vAccessWay.size(); i++)
	{
		comb.insertItem(i+1, g_TraderCpMgr.m_vAccessWay.at(i).code_desc);
	}
}
// 获取存取方向
string  CHJQueryBase::GetAccessWay(QComboBox& comb)
{
	int index = comb.currentIndex();
	if(index < 1)//如果没选或者选中第一个
		return "";
	else
		return CHJGlobalFun::qstr2str(g_TraderCpMgr.m_vAccessWay.at(index-1).code_id);
}
// 初始化交割品种代码
void  CHJQueryBase::IniVarietyId(QComboBox& comb)
{
	comb.clear();

	comb.insertItem(0, "");
    int n = 1;
	for(size_t i = 0; i < g_TraderCpMgr.m_vVariety.size(); i++)
	{
		// 根据金联通测试的要求，去掉该品种
		if( g_TraderCpMgr.m_vVariety.at(i).name != "Ag99.9" )
		{
			comb.insertItem(n, g_TraderCpMgr.m_vVariety.at(i).name);
            n++;
		}
	}
}
// 获取交割品种代码
string  CHJQueryBase::GetVarietyId(QComboBox& comb)
{
	int iPos;
	QString cstr;

	//获得当前选择项
	iPos=comb.currentIndex();

	//如果没选中或者选择空
	if(iPos<1)
		return "";


	QString str = comb.itemText(iPos);
	//遍历vec，获得该项对应的id
	for(size_t i=0; i < g_TraderCpMgr.m_vVariety.size(); i++)
	{
		if(g_TraderCpMgr.m_vVariety.at(i).name == str)
			return CHJGlobalFun::qstr2str(g_TraderCpMgr.m_vVariety.at(i).variety_id);
	}

	return "";
}
// 初始化费用代码 注意在前面添加了一个空行，故取数据的时候要向后移动一位了
void  CHJQueryBase::IniFeeCode(QComboBox& comb)
{
	comb.clear();

	comb.insertItem(0, "");
	for(size_t i = 0; i < g_TraderCpMgr.m_vFeeCode.size(); i++)
	{
		//QString cstr = g_TraderCpMgr.m_vFeeCode.at(i).code_desc.c_str();
		comb.insertItem(i+1, g_TraderCpMgr.m_vFeeCode.at(i).code_desc);
	}
}

// 获取费用代码
string  CHJQueryBase::GetFeeCode(QComboBox& comb)
{
	string str = "";
	int index = comb.currentIndex();
	if(index > 0)
	{
		str = CHJGlobalFun::qstr2str(g_TraderCpMgr.m_vFeeCode.at(index-1).code_id);
	}
	return str;
}

// 获取子费用代码
string  CHJQueryBase::GetSubFeeCode(QComboBox& comb)
{
	QString str2;
	QString cstr;

	int index=comb.currentIndex();

	if(index>0)
	{
		QString str1 = comb.itemText(index);
		for(size_t i=0;i<g_TraderCpMgr.m_vSubFeeCode.size();i++)
		{
			str2=g_TraderCpMgr.m_vSubFeeCode.at((i)).code_desc;
			if(str1==str2)
				return CHJGlobalFun::qstr2str(g_TraderCpMgr.m_vSubFeeCode.at((i)).code_id);	
		}
	}

	return "";
}

//费用代码变化时触发的函数：获取当前费用代码下的子费用代码
void CHJQueryBase::FeeCodeSelectChange( QComboBox& comb1,QComboBox& comb2 )
{
	int index;
	QString str1;
	QString str2;

	//清空子费用代码列表
	comb2.clear();
	//获得费用代码当前选项的对应id
	index=comb1.currentIndex();
	if(index<1)//如果选中为第一项或者没选中
		return;
	str1=g_TraderCpMgr.m_vFeeCode.at(index-1).code_id;
	// 根据金联通的要求，去掉“费用代码”为“手续费”时，不显示“会员”以及“交易所”的子费用代码
	if( str1 != "06" )
	{
		//给子费用代码列表添加一个空行
		comb2.addItem("");
		//遍历子费用代码，如果
		for(size_t i=0;i<g_TraderCpMgr.m_vSubFeeCode.size();i++)
		{
			str2=g_TraderCpMgr.m_vSubFeeCode.at((i)).code_id;
			str2=str2.mid(0,2);
			if(str2==str1)//添加字串
				comb2.addItem(g_TraderCpMgr.m_vSubFeeCode.at((i)).code_desc);
		}
	}
}

void CHJQueryBase::OnSelchangeComboNum()
{
	m_cmbPage->setCurrentIndex(0);
	OnButtonQuery();
}

void CHJQueryBase::OnSelchangeComboPage()
{
	OnButtonQuery();
}
/*
功能：往vector(v)添加一个标题为strtitle值为strvalue的pare(p)
v vector
p 参数对
strtitile 标题
strvalue 标题的值
*/
void CHJQueryBase::PairAdd(QVector< pair<QString,QString> > & v,QString strtitle,QString strvalue)
{
	pair<QString,QString> p;
	p.first=strtitle;
	p.second=strvalue;
	v.push_back(p);
}

#include "Mgr/CodeTableMgr.h"
#include "Mgr/ProdCodeMgr.h"
void CHJQueryBase::ProdCodeSelectChange( QComboBox& comb1,QComboBox& comb2 )
{
	// 清空交易类型列表
	comb2.clear();

	// 获取当前选择品种的市场
	MarketType eMarket = e_mk_Others;
	int iSel = comb1.currentIndex();
	if( iSel > 0 )
	{
		//QString cstr;
		//comb1.GetLBText(iSel,cstr);
		QString str = comb1.itemText(iSel);

		const ProdCodeInfo* pInfo = g_ProdCodeMgr.GetProdeCode(str);
		eMarket = pInfo != NULL ? ::GetMarketType(pInfo->market_id) : e_mk_Others;
	}

	// 获取当前市场下的所有交易类型 
	QVector<const CodeValue*> vecCode;
	g_CodeTableMgr.GetExchTypeName( vecCode, eMarket );

	// 加载交易方式列表
	comb2.insertItem( 0, "" ); // 给交易类型列表添加一个空行
	for( size_t j = 0; j < vecCode.size(); j++ )
		comb2.insertItem( j+1, vecCode.at(j)->code_desc );
}

//
void CHJQueryBase::OnOutputToExce(QString csSheetName)
{
	// 获取数据行的对齐方式
	vector<int>  vecAlign;
	GetUserAlign(vecAlign);

	// 获取模板路径
	QString sPath = QString("%1Trade\\report.xlsx").arg(g_Global.GetSystemPath());

	CMyExcel myExcel;
	myExcel.WriteListDataToExcel(*m_list, csSheetName, csSheetName, vecAlign, sPath);
}

void CHJQueryBase::GetUserAlign( vector<int> & vecAlign )
{
	// 获取配置文件的路径
	string strQueryID = GetQueryID(); // 从父类获取查询ID，从而获取配置文件的文件名
	QString   strPath = QString("%1QueryConfig\\%2.ini").arg(g_Global.GetSystemPath()).arg(CHJGlobalFun::str2qstr(strQueryID));

	QString strValue;
	size_t i;
	QVector<int>     vecAllAligns;

	//char buf[500] = {0};

	// 读取全部字段对其方式，并分割到vector
	//::GetPrivateProfileString("Info", "AllColumnAligns", "", buf, sizeof(buf), strPath);
	//strValue = buf;

	App::GetPriProfileString(strPath, ("Info"), ("AllColumnAligns"), strValue);
	CHJGlobalFun::SplitStrToVector(strValue, (QUERY_INI_SPLITER), vecAllAligns);

	// 遍历位置字段，获取用户选择的每一列的对应的信息（列宽、对齐方式）
	for(i = 0; i < m_vecColumnIndexs.size(); i++)
	{
		// 加载对其方式
		vecAlign.push_back(vecAllAligns.at(m_vecColumnIndexs.at(i)));
	}
}

void CHJQueryBase::QueryReserver( const Req6002 &stReq6002, const Rsp6002 &stRsp6002 )
{
}
