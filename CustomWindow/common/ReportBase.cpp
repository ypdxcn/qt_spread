
#include "ReportBase.h"
#include "HJGlobalFun.h"
#include "Global.h"
#include <QPainter>
#include <QtWidgets/QApplication>
#include <QFontMetrics>

#pragma execution_character_set("utf-8")

QReportBase::QReportBase( void )
{
	m_xRate = m_yRate = 1;
	m_xRate = (float)g_Global.m_ndpiX/DEFAULT_DPI_X;
	m_yRate = (float)g_Global.m_ndpiY/DEFAULT_DPI_Y;
	m_iMaxPage = 1;

	m_iStorage = 0;
	m_iForward = 0;
	m_iDefer = 0;
	m_iSurplus = 0;
	m_iMatchFlow = 0;
	m_iFundInOutFlow = 0;
	m_nPageHeight  = 2339;		
	m_nPageWidth   = 1654;	
}

QReportBase::~QReportBase( void )
{

}

/* 数据初始化******************************************************************************/
/*初始化客户信息*/
void QReportBase::IniRisk( Rsp6007 &rsp )
{
	if(rsp.htm_AcctSettleRisk.size() != 0)
	{
		RptDataInfo stru;
		stru.isLeftAli = true;

		QString strTitle[6] = {"交易编码", "客户名称",  "代理机构",  "银行账号",   "风险度",     "追缴资金"};
		string  strConID[6] = {"acct_no",  "cust_abbr", "branch_id", "account_no", "risk_index", "need_bal"};

		for(int i = 0; i < 6; i++)
		{
			stru.strTitle = strTitle[i];

			if(i != 5)
			{
				stru.strContent = CHJGlobalFun::str2qstr(rsp.htm_AcctSettleRisk.GetString(0, strConID[i]));
			}
			else //追缴资金项特殊处理
			{
				stru.isLeftAli = false;
				stru.strContent = CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleRisk.GetString(0, strConID[i])));
			}

			m_vecRisk.push_back(stru);
		}

		//m_vecRisk.at(2).strContent = "内蒙古银行总行营业部测试好好";
		int iTry = 2;
		int iAdjust;
		if(m_vecRisk.at(2).strContent.length() > 24)
			iAdjust = (m_vecRisk.at(2).strContent.length()-24) * iTry;
		else
			iAdjust = 0;

		// add by xiao
		int    iColumnPer[3] = { 33, 33 - iAdjust, 34 + iAdjust}; // { 33, 27, 40};
		Table1Info info;
		for(int j = 0; j < 3; j++)
		{
			info.iColumnPercent = iColumnPer[j];
			m_vecRiskColumn.push_back(info);
		}
	}
}

/*初始化资金信息*/
void QReportBase::IniFund( Rsp6007 &rsp )
{
	if(rsp.htm_AcctSettleFund.size() != 0)
	{
		RptDataInfo stru;
		stru.isLeftAli = false;

		int iColIDPos;
		const int iCount = 14;
		QString sTitle[iCount];
		if(m_iType == E_DAY)
		{
			sTitle[0] = "上期余额"; sTitle[1] = "本期持仓保证金"; sTitle[2] = "交割保证金"; sTitle[3] = "本期余额"; sTitle[4] = "溢短备付金"; sTitle[5] = "基础保证金"; sTitle[6] = "本期可用";
			sTitle[7] = "交割准备金"; sTitle[8] = "法律冻结"; sTitle[9] = "提货保证金"; sTitle[10] = "仓储保证金"; sTitle[11] = "上期持仓保证金"; sTitle[12] = "上期提货保证金"; sTitle[13] = "上期仓储保证金";
		}
		else
		{
			sTitle[0] = "上期余额"; sTitle[1] = "本期余额"; sTitle[2] = "本期可用"; sTitle[3] = "交易保证金"; sTitle[4] = "溢短备付金"; sTitle[5] = "交割准备金"; sTitle[6] = "交割保证金";
			sTitle[7] = "基础保证金"; sTitle[8] = "法律冻结"; sTitle[9] = "上期持仓保证金"; sTitle[10] = "上期提货保证金"; sTitle[11] = "上期仓储保证金"; sTitle[12] = "提货保证金"; sTitle[13] = "仓储保证金";
		}

		string  strConID[14] = {"last_bal",         "margin",              "deli_margin", 
			"curr_bal",         "reserve",             "base_margin",
			"curr_can_use",     "deli_prepare",        "long_froz",
			"take_margin",      "stor_fare_froz",      "last_margin",
			"last_take_margin", "last_stor_fare_froz"};

		int iChange[14] = {0, 3, 6, 1, 4, 7, 2, 5, 8, 11, 12, 13, 9, 10};
		
        /*
		QString strTitle[14] = {"上期余额",       "本期持仓保证金",     "交割保证金",
			"本期余额",       "溢短备付金",     "基础保证金",
			"本期可用",       "交割准备金",     "法律冻结",
			"提货保证金",     "仓储保证金",     "上期持仓保证金",
			"上期提货保证金", "上期仓储保证金"};
		*/

		for(int i = 0; i < 14; i++)
		{
			stru.strTitle   = sTitle[i];
			//stru.strContent = CHJGlobalFun::FormatFloat(rsp.htm_AcctSettleFund.GetString(0, strConID[i])).c_str();
			iColIDPos = m_iType == E_DAY ? i : iChange[i] ;
			stru.strContent = CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleFund.GetString(0, strConID[iColIDPos])));

			m_vecFund.push_back(stru);
		}

		//添加空格
		stru.strTitle   = "";
		stru.strContent = "";
		m_vecFund.push_back(stru);
	}
	else
	{
		return;
	}
}
/*初始化资金发生明细*/
void QReportBase::IniFundDetail( Rsp6007 &rsp )
{
	if(rsp.htm_AcctSettleFundDetail.size() != 0)
	{
		double fTotalGaCha = 0.00;
		RptDataInfo stru;
		stru.isLeftAli = false;

		QString strTitle[21] = {"存入金额",   "转出金额",   "买入金额",
			"卖出金额",   "本期盈亏",   "交易手续费",
			"交割手续费", "质押登记费", "租借登记费",
			"溢短金额",   "品种升贴水", "条块升贴水",
			"品牌升贴水", "白银滞纳金", "白银罚款",
			"仓储费",     "运保费",     "超期费",
			"递延费",     "违约金",     "入账利息"};
		string strConID[21]  = {"in_bal",      "out_bal",      "buy_bal",
			"sell_bal",    "surplus",      "exch_fare",
			"deli_fare",   "impawn_fare",  "lend_fare",
			"diff_bal",    "variety_agio", "item_agio",
			"brand_agio",  "ag_late_fare", "ag_amerce",
			"storage_fee", "trans_fee",    "over_due_fee",
			"defer_fee",   "breach_fee",   "incr_inte"};
		for(int i = 0; i < 21; i++)
		{
			stru.strTitle   = strTitle[i];
			stru.strContent = CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleFundDetail.GetString(0, strConID[i])));
			m_vecFundDetail.push_back(stru);

			QString strContent = CHJGlobalFun::str2qstr(rsp.htm_AcctSettleFundDetail.GetString(0, strConID[i]));

			fTotalGaCha += strContent.toDouble();
		}

		QString strTemp;
		//strTemp.Format("%.2f", fTotalGaCha);
		strTemp.number(fTotalGaCha, 'f', 2);
		//string str(strTemp.GetBuffer()); //QString to string
		m_strGaCha = strTemp;
	}
	else
	{
		return;
	}
}

/*初始化库存信息*/
void QReportBase::IniStorage( Rsp6007 &rsp )
{
	QString cstrTitle[11] = {"交割品种",  "上期库存","可用库存",    "待提库存",    "质押库存",  "冻结库存","本期买入","本期卖出","本期存入","本期提出","库存总量"};
	string   strConID[11] = {"variety_id","last_amt","curr_can_use","app_froz_amt","impawn_amt","froz_amt","buy_amt", "sell_amt","in_amt",  "out_amt", "curr_amt"};

	//添加标题
	int i;
	for(i = 0; i < 11; i++)
	{
		m_arrayStorage.append(std::move(cstrTitle[i]));
	}
	//添加内容
	for(i = 0; i < rsp.htm_AcctSettleStorage.size(); i++)
	{
		for(int j = 0; j < 11; j++)
		{
			m_arrayStorage.append(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleStorage.GetString(i, strConID[j])));
		}
	}
	//设置行数（不包括标题）
	m_iStorage = rsp.htm_AcctSettleStorage.size();

	RptColumnInfo stru;
	for(i = 0; i < 11; i++)
	{
		stru.bReturn = false;
		stru.iReturnBegin = 0;
		stru.iColumnPercent = 0;

		m_vStorage.push_back(stru);
	}
}  

/*空*/
void QReportBase::IniForward( Rsp6007 &rsp )
{

}
/*延期持仓信息*/
void QReportBase::IniDefer( Rsp6007 &rsp )
{
	QString cstrTitle[10] = {"合约代码", "持仓方向",  "上期持仓","本期开仓","本期平仓","本期交收","中立仓建仓",  "本期持仓量","开仓均价",  "持仓均价"};
	string   strConID[10] = {"prod_code","long_short","last_amt","open_amt","cov_amt", "deli_amt","mid_deli_amt","curr_amt",  "open_price","hold_price"};
	int    iColumnPer[10] = { 12,         9,          9,        9,        9,        9,        9,            9,          13,          13};

	//添加标题
	int i;
	for(i = 0; i < 10; i++)
	{
		m_arrayDefer.append(std::move(cstrTitle[i]));
	}
	//添加内容
	for(i = 0; i < rsp.htm_AcctSettleDefer.size(); i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(j < 8)
			{
				m_arrayDefer.append(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleDefer.GetString(i, strConID[j])));
			}
			else
			{
				m_arrayDefer.append(CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleDefer.GetString(i, strConID[j]))));
			}
		}
	}
	//设置行数（不包括标题）
	m_iDefer = rsp.htm_AcctSettleDefer.size();

	RptColumnInfo stru;
	for(i = 0; i < 10; i++)
	{
		if(i == 7 || i == 6)
		{
			stru.bReturn = true;
			stru.iReturnBegin = 8;
		}
		else
		{
			stru.bReturn = false;
			stru.iReturnBegin = 0;
		}
		stru.iColumnPercent = iColumnPer[i];
		m_vDefer.push_back(stru);
	}
}

/*本期盈亏信息*/
void QReportBase::IniSurplus( Rsp6007 &rsp )
{
	const int iCount = 3;
	QString cstrTitle[iCount] = {"合约代码", "本期结算价",  "本期盈亏"};
	string   strConID[iCount] = {"prod_code","settle_price","market_surplus"};
	if(m_iType == E_MONTH)
	{
		cstrTitle[1] = "期末结算价";
	}
	
	//添加标题
	int i;
	for(i = 0; i < iCount; i++)
	{
		m_arraySurplus.append(cstrTitle[i]);
	}
	//添加内容
	for(i = 0; i < rsp.htm_AcctSettleSurplus.size(); i++)
	{
		for(int j = 0; j < iCount; j++)
		{
			if( j == 0) //第一列（合约代码）不用格式化
			{
				m_arraySurplus.append(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleSurplus.GetString(i, strConID[j])));
			}
			else
			{
				m_arraySurplus.append(CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleSurplus.GetString(i, strConID[j]))));
			}
		}
	}
	//设置行数（不包括标题）
	m_iSurplus = rsp.htm_AcctSettleSurplus.size();

	RptColumnInfo stru;
	for(i = 0; i < iCount; i++)
	{
		stru.bReturn = false;
		stru.iReturnBegin = 0;
		stru.iColumnPercent = 0;

		m_vSurplus.push_back(stru);
	}
}

/*成交流水*/
void QReportBase::IniMatchFlow( Rsp6007 &rsp )
{
	QString cstrTitle[8] = {"成交编号","报单号",  "合约代码", "交易方向", "成交价格",   "成交数量", "成交金额", "手续费"};
	string   strConID[8] = {"match_no","order_no","prod_code","exch_code","match_price","match_amt","match_bal","exch_fare"};
	int    iColumnPer[8] = { 19,        12,        10,         14,         10,           7,         16,         12};
	//添加标题
	int i;
	for(i = 0; i < 8; i++)
	{
		m_arrayMatchFlow.append(cstrTitle[i]);
	}
	//添加内容
	for(i = 0; i < rsp.htm_AcctSettleMatchFlow.size(); i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(j == 6 || j == 7 || j == 4)
			{
				m_arrayMatchFlow.append(CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleMatchFlow.GetString(i, strConID[j]))));
			}
			else
			{
				m_arrayMatchFlow.append(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleMatchFlow.GetString(i, strConID[j])));
			}
		}
	}
	//设置行数（不包括标题）
	m_iMatchFlow = rsp.htm_AcctSettleMatchFlow.size();
    
	RptColumnInfo stru;
	for(i = 0; i < 8; i++)
	{
		if(i == 5)
		{
			stru.bReturn = true;
			stru.iReturnBegin = 4;
		}
		else
		{
			stru.bReturn = false;
			stru.iReturnBegin = 0;
		}
		stru.iColumnPercent = iColumnPer[i];
		m_vMatchFlow.push_back(stru);
	}
}
/*出入金流水*/
void QReportBase::IniFundInOutFlow( Rsp6007 &rsp )
{
	QString cstrTitle[6] = {"交易日期", "交易流水号","存取方向",  "发生金额","银行日期","银行流水号"};
	string   strConID[6] = {"exch_date","serial_no", "access_way","exch_bal","bk_date", "bk_seq_no"};
	int    iColumnPer[6] = { 15,         15,          15,          15,        15,        25};
	//添加标题
	int i;
	for(i = 0; i < 6; i++)
	{
		m_arrayFundInOutFlow.append(cstrTitle[i]);
	}
	//添加内容
	for(i = 0; i < rsp.htm_AcctSettleFundInOutFlow.size(); i++)
	{
		for(int j = 0; j < 6; j++)
		{
			if(j == 3)
			{
				m_arrayFundInOutFlow.append(CHJGlobalFun::FormatFloat(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleFundInOutFlow.GetString(i, strConID[j]))));
			}
			else
			{
				m_arrayFundInOutFlow.append(CHJGlobalFun::str2qstr(rsp.htm_AcctSettleFundInOutFlow.GetString(i, strConID[j])));
			}
		}
	}
	//设置行数（不包括标题）
	m_iFundInOutFlow = rsp.htm_AcctSettleFundInOutFlow.size();

	RptColumnInfo stru;
	for(i = 0; i < 6; i++)
	{
		stru.bReturn = false;
		stru.iReturnBegin = 0;
		
		stru.iColumnPercent = iColumnPer[i];
		m_vFundInOutFlow.push_back(stru);
	}
}

void QReportBase::IniAllData( Rsp6007 &rsp )
{
	IniRisk(rsp);/*初始化客户信息*/
	IniFund(rsp);/*初始化资金信息*/
	IniFundDetail(rsp);/*初始化资金发生明细*/
	IniStorage(rsp);/*初始化库存信息*/
	IniForward(rsp);/**/
	IniDefer(rsp);/*延期持仓信息*/
	IniSurplus(rsp);/*本期盈亏信息*/
	IniMatchFlow(rsp);/*成交流水*/
	IniFundInOutFlow(rsp);/*出入金流水*/
}
/* 数据初始化******************************************************************************/
//
void QReportBase::Draw( QPainter* pDC , PrintInfo* pInfo)
{
	// 创建字体
	QFont oldfont  = qApp->font();
	QFont *newfont = nullptr;
	newfont = new QFont("宋体", 10);

	pDC->setFont(*newfont);
	if(m_iType == E_DAY || m_iType == E_MONTH) // 如果是日结单打印或者月结单打印
	{
		int iLeftTopy=0;  // 表格左上角的y值，两种表格公用

		// 如果当前页是第一页
		if(pInfo->m_nCurPage == 1)
		{
			// 如果总页数为1，则打印全部
			//if(m_printInfo.GetMaxPage() == 1 )
			if(pInfo->m_MaxPage == 1) // modify by xiao，解决m_printInfo.GetMaxPage()返回错误的问题
			{
				DrawPart1(iLeftTopy, pDC);
				DrawPart2(iLeftTopy, pDC);
			}
			else // 如果总页数是2，则打印第一部分
			{
				DrawPart1(iLeftTopy, pDC);
			}
		}
		else // 如果当前是第二页，则打印第二部分
		{
			iLeftTopy = GetY(TABLE_PAGE2_BEGIN_Y);
			DrawPart2(iLeftTopy, pDC);
		}
	}
	else if(m_iType == E_TIHUO) // 如果是提货单打印
	{
		DrawTiHuo(pDC);
	}
	else
	{
	}

	// 写入页码信息
	WritePage(pDC, pInfo);

	pDC->setFont(oldfont);// > SelectObject(pOldFont);
}
//设置总的页码数
void QReportBase::SetPage(PrintInfo* pInfo, int& nHeight)
{
	int iTemp = TABLE_LINE_HEIGHT;
	nHeight  = GetY(TABLE_BEGIN_Y + (CUS_INFO_LINE_COUNT + FUN_INFO_LINE_COUNT + FUN_DETAIL_LINE_COUNT + 1)*TABLE_LINE_HEIGHT);
	nHeight += GetY((TABLE_TITLE_HEIGHT + TABLE_TO_TITLE_DIS)*2); //资金发生明细表格的底部的y值

	//如果库存信息不为空
	if(m_iStorage != 0)
	{
		//获得库存信息表格的底部的y值
		nHeight += GetY(TABLE_TO_TITLE_DIS + TABLE_TITLE_HEIGHT + TABLE2_TITLE_HEIGHT + TABLE_LINE_HEIGHT*m_iStorage);
	}

	//如果
	if(m_iForward != 0)
	{

	}

	//如果延期持仓信息不为空
	if(m_iDefer != 0)
	{
		nHeight += GetY(TABLE_TO_TITLE_DIS + TABLE_TITLE_HEIGHT + TABLE2_TITLE_HEIGHT + TABLE_LINE_HEIGHT*m_iDefer);
	}

	//如果本期盈亏信息不为空
	if(m_iSurplus != 0)
	{
		nHeight += GetY(TABLE_TO_TITLE_DIS + TABLE_TITLE_HEIGHT + TABLE2_TITLE_HEIGHT + TABLE_LINE_HEIGHT*m_iSurplus);
	}

	//如果成交流水不为空
	if(m_iMatchFlow != 0)
	{
		nHeight += GetY(TABLE_TO_TITLE_DIS + TABLE_TITLE_HEIGHT + TABLE2_TITLE_HEIGHT + TABLE_LINE_HEIGHT*m_iMatchFlow);
	}

	//如果出入金流水不为空
	if(m_iFundInOutFlow != 0)
	{
		nHeight += GetY(TABLE_TO_TITLE_DIS + TABLE_TITLE_HEIGHT + TABLE2_TITLE_HEIGHT + TABLE_LINE_HEIGHT*m_iFundInOutFlow);
	}

	// 如果是打印模式，而且需要显示的高度溢出，则分两页
	if(m_eShowMode == E_PRINT && nHeight > (GetY(PAGE_BEGIN_Y-100)))
	{
		// 如果要绘制的表格的最底部不在安全线内，则换页
		pInfo->m_MaxPage = 2;//(2);
		m_iMaxPage = 2;
	}
	else
	{
		pInfo->m_MaxPage = 1;
		m_iMaxPage = 1;
	}
}
//
void QReportBase::DrawPart1(int &iLeftTopy, QPainter* pDC)
{
	//画标题
	DrawTitle(pDC);

	/*写入客户信息*/
	//iLeftTopy = DrawTable(pDC, GetY(TABLE_BEGIN_Y), CUS_INFO_LINE_COUNT,   TABLE1_LINE_TITLE_COUNT, GetX(CUS_INFO_COL_TITLE_WIDTH),   m_vecRisk, "客户信息", "金额:元");
	iLeftTopy = DrawTableEx(pDC, GetY(TABLE_BEGIN_Y), CUS_INFO_LINE_COUNT,   TABLE1_LINE_TITLE_COUNT, GetX(CUS_INFO_COL_TITLE_WIDTH),   m_vecRisk, "客户信息", "金额:元", m_vecRiskColumn);
	 
	/*写入资金信息*/
	if(m_vecFund.size() != 0)
	{
		iLeftTopy = DrawTable(pDC, iLeftTopy,     FUN_INFO_LINE_COUNT,   TABLE1_LINE_TITLE_COUNT, GetX(FUN_INFO_COL_TITLE_WIDTH),   m_vecFund, "资金信息", "单位:元");
	}

	/*写入资金发生明细*/
	if(m_vecFundDetail.size() != 0)
	{
		iLeftTopy = DrawTable(pDC, iLeftTopy,     FUN_DETAIL_LINE_COUNT, TABLE1_LINE_TITLE_COUNT, GetX(FUN_DETAIL_COL_TITLE_WIDTH), m_vecFundDetail, "资金发生明细", "单位:元");
		iLeftTopy = DrawFunDetail(pDC, iLeftTopy, "轧差", m_strGaCha);
	}

	/*写入库存信息*/
	if(m_iStorage != 0)
	{
		iLeftTopy = DrawTable2(pDC, iLeftTopy, m_iStorage, STORE_INFO_COLUME_COUNT,   m_arrayStorage, "库存信息",     "单位:克",          m_vStorage, 1);
	}

	/*写入延期持仓信息*/
	if(m_iDefer != 0)
	{
		iLeftTopy = DrawTable2(pDC, iLeftTopy, m_iDefer,   CHICANG_INFO_COLUME_COUNT, m_arrayDefer,   "延期持仓信息", "数量:手  金额:元", m_vDefer, 2, false);
	}
}

//
void QReportBase::DrawPart2(int &iLeftTopy, QPainter* pDC)
{
	/*写入本期盈亏信息*/
	if(m_iSurplus != 0)
	{
		iLeftTopy = DrawTable2(pDC, iLeftTopy, m_iSurplus, YINGKUI_INFO_COLUME_COUNT, m_arraySurplus, "本期盈亏信息", "单位:元",          m_vSurplus, 1);
	}

	/*写入成交流水信息*/
	if(m_iMatchFlow != 0)
	{
		iLeftTopy = DrawTable2(pDC, iLeftTopy, m_iMatchFlow, MATCHFLOW_COLUME_COUNT, m_arrayMatchFlow, "成交流水", "数量:手  金额:元",          m_vMatchFlow, 1, false);
	}

	/*写入出入金流水信息*/
	if(m_iFundInOutFlow != 0)
	{
		iLeftTopy = DrawTable2(pDC, iLeftTopy, m_iFundInOutFlow, FUNINOUTFLOW_COLUME_COUNT, m_arrayFundInOutFlow, "出入金流水", "",          m_vFundInOutFlow, 1, false);
	}
}

//
//画日结单标题
void QReportBase::DrawTitle(QPainter* pDC )
{
	QFont oldfont = qApp->font();
	QFont *font = new QFont("宋体", 20);
	//CFont *pOldFont;
	//CFont DataFont;
	//DataFont.CreatePointFont(200, "宋体", pDC); 
	//pOldFont = pDC->SelectObject(&DataFont);

	pDC->setFont(*font);
	pDC->drawText(GetX(TITLE_BEGIN_X), GetY(TITLE_BEGIN_Y), m_strTitle);
	//pDC->TextOut(GetX(TITLE_BEGIN_X), GetY(TITLE_BEGIN_Y), m_strTitle);

	//pDC->MoveTo();
	//pDC->LineTo();

	//pDC->SelectObject (pOldFont);
	pDC->setFont(oldfont);
	pDC->drawLine(GetX(LEFT_X), GetY(LINE_Y), GetX(RIGHT_X), GetY(LINE_Y));
}
/*
画日结单的第一种表格
iLeftTopY 绘画表格的起点（即左上角的y坐标）
iLineCount 表格的行数（除了标题栏）
iTitleCount 表格的列数
iTitleWid 表格的宽度
vec 装载表格的内容
strLTitle 表格左上角的标题
strRTitle 表格右上角的单位
*/
int QReportBase::DrawTable(QPainter* pDC, int iLeftTopY, int iLineCount, int iTitleCount, int iTitleWid, const vector<RptDataInfo> &vec, QString strLTitle, QString strRTitle)
{
	int x1,y1;
	int x2,y2;
	int i;
	int iContentWid;

	//画标题
	//QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
	//option.setWrapMode(QTextOption::WordWrap);

	y1 = iLeftTopY - GetY(TABLE_TITLE_HEIGHT);

	pDC->drawText(LEFT_X + TEXT_DISTANCE_X, y1, pDC->fontMetrics().width(strLTitle), pDC->fontMetrics().height(), Qt::AlignLeft, strLTitle);

	pDC->drawText(GetX(RIGHT_X - TEXT_DISTANCE_X)-pDC->fontMetrics().width(strRTitle), y1, pDC->fontMetrics().width(strRTitle), pDC->fontMetrics().height(), Qt::AlignLeft, strRTitle);


	//画整个table的矩形
	x1 = GetX(LEFT_X);
	y1 = iLeftTopY;
	x2 = GetX(RIGHT_X);
	y2 = y1 + GetY(TABLE_LINE_HEIGHT)*iLineCount;
	//pDC->Rectangle(x1, y1, x2, y2); 
	pDC->drawRect(x1, y1,x2-x1, GetY(TABLE_LINE_HEIGHT)*iLineCount);
	//画行
	for(i = 1; i < iLineCount; i++)
	{
		y1 += GetY(TABLE_LINE_HEIGHT);
		//pDC->MoveTo(GetX(LEFT_X), y1);
		//pDC->LineTo(GetX(RIGHT_X), y1);
		pDC->drawLine(GetX(LEFT_X), y1, GetX(RIGHT_X), y1);
	}

	//画列
	x1 = GetX(LEFT_X);
	y1 = iLeftTopY; //每一列线上面的y值
	y2 = iLeftTopY + GetY(TABLE_LINE_HEIGHT)*iLineCount;//每一列线的下面的y值
	iContentWid = GetX(TABLE_WIDTH)/iTitleCount - iTitleWid; //内容列的宽度
	for(i = 1; i < iTitleCount*2; i++)
	{
		if(i%2 == 1)
		{
			x1 += iTitleWid;
		}
		else
		{
			x1 += iContentWid;
		}

		//pDC->MoveTo(x1, y1);
		//pDC->LineTo(x1, y2);

		pDC->drawLine(x1, y1, x1, y2);
	}


	//写入内容（标题和内容）
	for(i = 0; i < iLineCount*iTitleCount; i++)
	{
		if(i == 0)
		{
			x1 = GetX(LEFT_X);
			y1 = iLeftTopY;
		}
		else if(i%iTitleCount == 0) //如果是下一行
		{
			x1  = GetX(LEFT_X);
			y1 += GetY(TABLE_LINE_HEIGHT);
		}
		else
		{
			x1 += GetX(TABLE_WIDTH)/iTitleCount;
		}
		QString str1 = vec.at(i).strTitle;
		//pDC->TextOut(x1 + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), vec.at(i).strTitle);
		pDC->drawText(x1 + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(vec.at(i).strTitle), pDC->fontMetrics().height(), Qt::AlignLeft, vec.at(i).strTitle);

		if(vec.at(i).isLeftAli)
		{
			//pDC->TextOut(x1 + iTitleWid + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), vec.at(i).strContent);	

			pDC->drawText(x1 + iTitleWid + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(vec.at(i).strContent), pDC->fontMetrics().height(), Qt::AlignLeft, vec.at(i).strContent);
		}
		else
		{
			//pDC->SetTextAlign(TA_RIGHT);
			//pDC->TextOut(x1 + GetX(TABLE_WIDTH)/iTitleCount - GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), vec.at(i).strContent);
			pDC->drawText(x1 + GetX(TABLE_WIDTH) / iTitleCount - GetX(TEXT_DISTANCE_X)-pDC->fontMetrics().width(vec.at(i).strContent), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(vec.at(i).strContent), pDC->fontMetrics().height(), Qt::AlignRight, vec.at(i).strContent);
		}


	}

	return iLeftTopY + GetY(iLineCount*TABLE_LINE_HEIGHT + TABLE_TITLE_HEIGHT + TABLE_TO_TITLE_DIS);
}

int QReportBase::DrawFunDetail(QPainter* pDC, int iLeftTopY, QString strTitle, QString strContetn)//画资金发生明细的最后一行
{
	int iReturn = iLeftTopY + GetY(TABLE_LINE_HEIGHT); //补上漏画的一行的高度
	iLeftTopY -= GetY(TABLE_TITLE_HEIGHT + TABLE_TO_TITLE_DIS); //获取资金明细的表格的最下面的y值（没有画漏掉的行的时候）

	//画第一条竖线
	//pDC->MoveTo(GetX(LEFT_X), iLeftTopY);
	//pDC->LineTo(GetX(LEFT_X), iLeftTopY  + GetY(TABLE_LINE_HEIGHT));
	pDC->drawLine(GetX(LEFT_X), iLeftTopY, GetX(LEFT_X), iLeftTopY + GetY(TABLE_LINE_HEIGHT));
	//画第二条竖线
	//pDC->MoveTo(GetX(LEFT_X + FUN_DETAIL_COL_TITLE_WIDTH), iLeftTopY);
	//pDC->LineTo(GetX(LEFT_X + FUN_DETAIL_COL_TITLE_WIDTH), iLeftTopY + GetY(TABLE_LINE_HEIGHT));
	pDC->drawLine(GetX(LEFT_X + FUN_DETAIL_COL_TITLE_WIDTH), iLeftTopY, GetX(LEFT_X + FUN_DETAIL_COL_TITLE_WIDTH), iLeftTopY + GetY(TABLE_LINE_HEIGHT));


	//画第三条竖线,-1是用来校正偏移量，否则线条不对其
	//pDC->MoveTo(GetX(RIGHT_X-1), iLeftTopY); //不减掉5无法对齐，可能是由于这一行少画了几条线
	//pDC->LineTo(GetX(RIGHT_X-1), iLeftTopY  +  GetY(TABLE_LINE_HEIGHT));
	pDC->drawLine(GetX(RIGHT_X - 1), iLeftTopY, GetX(RIGHT_X - 1), iLeftTopY + GetY(TABLE_LINE_HEIGHT));

	//画底部的横线
	//pDC->MoveTo(GetX(LEFT_X), iLeftTopY + GetY(TABLE_LINE_HEIGHT));
	//pDC->LineTo(GetX(RIGHT_X), iLeftTopY + GetY(TABLE_LINE_HEIGHT));
	pDC->drawLine(GetX(LEFT_X), iLeftTopY + GetY(TABLE_LINE_HEIGHT), GetX(RIGHT_X), iLeftTopY + GetY(TABLE_LINE_HEIGHT));

	//写标题字
	pDC->drawText(GetX(LEFT_X + TEXT_DISTANCE_X), iLeftTopY + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(strTitle), pDC->fontMetrics().height(), Qt::AlignLeft, strTitle);

	/*pDC->TextOut(GetX(LEFT_X + TEXT_DISTANCE_X), iLeftTopY + GetY(TEXT_DISTANCE_Y), strTitle);
	pDC->SetTextAlign(TA_RIGHT);
	pDC->TextOut(GetX(RIGHT_X - TEXT_DISTANCE_X), iLeftTopY + GetY(TEXT_DISTANCE_Y), strContetn);*/
	pDC->drawText(GetX(RIGHT_X - TEXT_DISTANCE_X) - pDC->fontMetrics().width(strContetn), iLeftTopY + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(strContetn), pDC->fontMetrics().height(), Qt::AlignRight, strContetn);

	return iReturn;
}


int QReportBase::DrawTable2( QPainter* pDC, int iLeftTopY, int iLineCount, int iColCount, const QStringList &strArray, QString strLTitle, QString strRTitle, const vector<RptColumnInfo> &vColumnInfo , int iLAliBegin /*= 0*/,  bool bColumnEven /*= true*/)
{
	int x1,y1;
	int x2,y2;
	int i;
	int iColWid;

	//画标题
	//pDC->SetTextAlign(TA_LEFT);
	y1 = iLeftTopY - GetY(TABLE_TITLE_HEIGHT);
	//pDC->TextOut(GetX(LEFT_X + TEXT_DISTANCE_X), y1, strLTitle);

	pDC->drawText(GetX(LEFT_X + TEXT_DISTANCE_X), y1, pDC->fontMetrics().width(strLTitle), pDC->fontMetrics().height(), Qt::AlignLeft, strLTitle);


	//pDC->SetTextAlign(TA_RIGHT);
	//pDC->TextOut(GetX(RIGHT_X - TEXT_DISTANCE_X), y1, strRTitle);
	//pDC->SetTextAlign(TA_LEFT);
	pDC->drawText(GetX(RIGHT_X - TEXT_DISTANCE_X)- pDC->fontMetrics().width(strRTitle), y1, pDC->fontMetrics().width(strRTitle), pDC->fontMetrics().height(), Qt::AlignRight, strRTitle);

	//画整个table的矩形
	x1 = GetX(LEFT_X);
	y1 = iLeftTopY;
	x2 = GetX(RIGHT_X);
	y2 = y1 + GetY(TABLE_LINE_HEIGHT)*iLineCount + GetY(TABLE2_TITLE_HEIGHT);
	//pDC->Rectangle(x1, y1, x2, y2); 
	pDC->drawRect(x1, y1, x2-x1, y2-y1);
	//画行
	for(i = 0; i < iLineCount; i++)
	{
		if(i == 0) //第一行为标题行，高度不同
		{
			y1 += GetY(TABLE2_TITLE_HEIGHT);
		}
		else
		{
			y1 += GetY(TABLE_LINE_HEIGHT);
		}

		//pDC->MoveTo(GetX(LEFT_X), y1);
		//pDC->LineTo(GetX(RIGHT_X), y1);
		pDC->drawLine(GetX(LEFT_X), y1, GetX(RIGHT_X), y1);
	}

	//画列
	x1 = GetX(LEFT_X);
	y1 = iLeftTopY; //每一列线上面的y值
	iColWid = GetX(TABLE_WIDTH) / iColCount; //内容列的宽度
	for(i = 1; i < iColCount; i++)
	{
		if(bColumnEven)
		{
			x1 += iColWid;
		}
		else
		{
			int iTemp = (int)(((float)vColumnInfo.at(i-1).iColumnPercent)/100.0*GetX(TABLE_WIDTH));
			x1 += iTemp;
		}

		//
		//pDC->MoveTo(x1, y1);
		//pDC->LineTo(x1, y2); //y2沿用上面的值

		pDC->drawLine(x1,y1,x1,y2);
	}

	//写入内容（标题和内容）
	for(i = 0; i < (iLineCount+1)*iColCount; i++)
	{
		// x1是当前列的左边的开始位置
		// 获得当前列的宽度
		int iCurColumnWid;
		if(bColumnEven)
		{
			iCurColumnWid = iColWid;
		}
		else
		{
			iCurColumnWid = (int)(((float)vColumnInfo.at(i%iColCount).iColumnPercent)/100.0*GetX(TABLE_WIDTH));
		}
		 
		if(i == 0)
		{
			x1 = GetX(LEFT_X);
			y1 = iLeftTopY;
		}
		else if(i%iColCount == 0) //如果是下一行，则刚好换到下一行的第一列
		{
			x1  = GetX(LEFT_X);
			if(i == iColCount) // 如果刚好是第二行的第一列
			{
				y1 += GetY(TABLE2_TITLE_HEIGHT);
			}
			else //
			{
				y1 += GetY(TABLE_LINE_HEIGHT);
			}
		}
		else
		{
			// 如果不是第一列，则加上一列的宽度
			if(bColumnEven)
			{
				x1 += iColWid;
			}
			else
			{
				x1 += (int)(((float)vColumnInfo.at(i%iColCount-1).iColumnPercent)/100.0*GetX(TABLE_WIDTH));
			}
		}

		// 如果是第一行，或者是左对齐的列
		if(i < iColCount || i%iColCount < iLAliBegin)
		{
			//pDC->SetTextAlign(TA_LEFT);
			QString str = strArray[i];
			if(i < iColCount) // 如果是第一行
			{
				int iDisX,iDisY;

				int len = str.length(); //中文字等于2个
                int iReturnBegin = vColumnInfo.at(i%iColCount).iReturnBegin;
				if(vColumnInfo.at(i%iColCount).bReturn && len > iReturnBegin) // 如果该列的标题需要换行，而且个数已经大于换行的个数
				{
					// 第一行x方向开始的位置
					iDisX = (iCurColumnWid - iReturnBegin/2*GetY(MY_FONT_SIZE))/2;
					//pDC->TextOut(x1 + iDisX, y1 + GetY(TEXT_DISTANCE_Y), str.left(iReturnBegin));
					pDC->drawText(x1 + iDisX, y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(str.left(iReturnBegin)), pDC->fontMetrics().height(), Qt::AlignLeft, str.left(iReturnBegin));


					QString str2 = str.mid(iReturnBegin);
					iDisX = (iCurColumnWid - str2.length()/2*GetY(MY_FONT_SIZE))/2;
					//pDC->TextOut(x1 + iDisX, y1 + GetY(TEXT_DISTANCE_Y + MY_FONT_SIZE), str2);
					pDC->drawText(x1 + iDisX, y1 + GetY(TEXT_DISTANCE_Y + MY_FONT_SIZE), pDC->fontMetrics().width(str2), pDC->fontMetrics().height(), Qt::AlignLeft, str2);

				}
				else
				{
					// 一行居中显示
					iDisY = GetY(TABLE2_TITLE_HEIGHT - MY_FONT_SIZE)/2;
					iDisX = (iCurColumnWid - (len/2)*GetY(MY_FONT_SIZE))/2;
					//pDC->TextOut(x1 + iDisX, y1 + iDisY, str);
					//2018-1-5  15矫正
					pDC->drawText(x1 + iDisX-15, y1 + iDisY, pDC->fontMetrics().width(str), pDC->fontMetrics().height(), Qt::AlignLeft, str);
				}
			}
			else // 如果是左对齐的列
			{
				//pDC->TextOut(x1 + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), str);	
				pDC->drawText(x1 + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(str), pDC->fontMetrics().height(), Qt::AlignLeft, str);
			}	
		}
		else
		{

			// 如果是打印模式的最后一列，解决打印时的最后一列的数据偏出表格的问题
			if(m_eShowMode == E_PRINT && i%iColCount == iColCount -1) 
			{
				// 20是校准值
				//pDC->TextOut(GetX(RIGHT_X)-20, y1 + GetY(TEXT_DISTANCE_Y), strArray.GetAt(i));
				pDC->drawText(GetX(RIGHT_X) - 20- pDC->fontMetrics().width(strArray.at(i)), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(strArray.at(i)), pDC->fontMetrics().height(), Qt::AlignRight, strArray.at(i));
			
			}
			else
			{
				//pDC->TextOut(x1 + iCurColumnWid - GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), strArray.GetAt(i));
				pDC->drawText(x1 + iCurColumnWid - GetX(TEXT_DISTANCE_X)- pDC->fontMetrics().width(strArray.at(i)), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(strArray.at(i)), pDC->fontMetrics().height(), Qt::AlignRight, strArray.at(i));

			}
		}	
	}

	return iLeftTopY + GetY(TABLE2_TITLE_HEIGHT) + GetY(TABLE_LINE_HEIGHT)*iLineCount 
		+ GetY(TABLE_TO_TITLE_DIS) + GetY(TABLE_TITLE_HEIGHT);
}

/*打印提货相关**************************************************************************************************************/
void QReportBase::DrawTiHuo(QPainter* pDC)
{
	//画标题
	int i;
	//CFont *pOldFont;
	//CFont DataFont;

	QFont oldFont  = qApp->font();
	QFont *newFont = new QFont("宋体",20);
	newFont->setBold(true);
	int nFontHeight = GetY(130);
 	//DataFont.CreateFont(
 	//	nFontHeight,
 	//	0,
 	//	0,
 	//	900,
 	//	FW_HEAVY, // 粗体
 	//	0,
 	//	0,
 	//	0,
 	//	DEFAULT_CHARSET,
 	//	OUT_DEFAULT_PRECIS,
 	//	CLIP_DEFAULT_PRECIS,
 	//	DEFAULT_QUALITY,
 	//	FF_DONTCARE,
 	//	_T("宋体") 
 	//	);;
	//pOldFont = pDC->SelectObject(&DataFont);
	pDC->setFont(*newFont);

	QString strCnTitle = "上海黄金交易所提货单";

	QFontMetrics fm(*newFont);
	QRect rec = fm.boundingRect(strCnTitle);
	//字符串所占的像素宽度,高度  
	int textWidth = rec.width();
	int textHeight = rec.height();
	//CSize sizeCnTitle = pDC->GetTextExtent(strCnTitle);
	

	TIHUO_TITLE_BEGIN_X = (m_nPageWidth - textWidth)/2;
	//pDC->TextOut(TIHUO_TITLE_BEGIN_X, GetY(TIHUO_TITLE_BEGIN_Y), strCnTitle);
	pDC->drawText(TIHUO_TITLE_BEGIN_X, GetY(TIHUO_TITLE_BEGIN_Y), pDC->fontMetrics().width(strCnTitle), pDC->fontMetrics().height(), Qt::AlignLeft, strCnTitle);


	QString strEnTitle = "Shanghai Gold Exchange Lading Application Form";
	QRect rec2      = fm.boundingRect(strEnTitle);
	//textWidth = rec.width();
	//textHeight = rec.height();
	//CSize sizeTitle = pDC->GetTextExtent(strEnTitle);
	//pDC->TextOut((m_nPageWidth - textWidth)/2,GetY(TIHUO_TITLE_BEGIN_Y )+ sizeTitle.cy, strEnTitle);
	pDC->drawText((m_nPageWidth - textWidth) / 2, GetY(TIHUO_TITLE_BEGIN_Y) + rec2.height(),  pDC->fontMetrics().width(strEnTitle),pDC->fontMetrics().height(), Qt::AlignLeft, strEnTitle);

	
	//pDC->SelectObject (pOldFont);
	pDC->setFont(oldFont);
	//画上面的部分（title下面的）
	QString strTitle[17] = {"提货单编号(Bill of lading No.)", "申请日期(Application date)", 
		"会员编号(Member account No.)",     "会员名称(Member name)",     "客户代码(Client account No.)", 
		"客户名称(Client name)", 
		"仓库代码(Vault Code)",   "仓库名称(Vault name)", "仓库地址(Vault address)",  "联系人(Vault contacts)",
		"联系电话(Vault Tel No.)", "提货人姓名(Lading person)",
		"证件类型(Lading person's certificate type)",   "证件号(Lading person's certificate No.)",   
		"提货开始日期(Starting date of lading)", "提货截止日期(Ending date of lading)", "申请状态(Application Status)"};

	QString strWords;
	int x,y;
	y = GetY(TIHUO_TITLE_BEGIN_Y + TITLE_TOP_DISTANCE) + rec2.height();
	//CSize sizeLeft = pDC->GetTextExtent(strTitle[12]);
	QFontMetrics fm1(oldFont);
	QRect sizeLeft = fm1.boundingRect(strTitle[12]);
	//CSize sizeRight(0,0);
	QRect sizeRight(0, 0, 0, 0);
	//m_strTiHuoTop[8] = "提货开始日期(Starting date of lading)提货开始日期(Starting date of lading)";
	for(i = 0; i < 17; i++)
	{
		//CSize sizeValue = pDC->GetTextExtent(m_strTiHuoTop[i]);

		QRect rec4 = fm1.boundingRect(m_strTiHuoTop[i]);
		if (sizeLeft.width() > sizeRight.width())
		{
			sizeRight = rec4;
		}
	}

	if (sizeLeft.width() + sizeRight.width() < m_nPageWidth)
	{
		TOP_BEGIN_X = (m_nPageWidth - sizeLeft.width() - sizeRight.width())/3;//起始位置,也是名称和值的间距
	}

	TOP_TITLE_WIDTH = sizeLeft.width() + TOP_BEGIN_X;
	int nValueWidth = m_nPageWidth - TOP_BEGIN_X*3 - sizeLeft.width();
	for(i = 0; i < 17; i++)
	{
// 		if(i % 2 == 0)
// 		{
			//如果是第一列，则初始化x值，并增加y值
			x = TOP_BEGIN_X;
			y += GetY(TOP_LINE_HEIGHT);
// 		}
// 		else
// 		{
// 			//如果是第二列，则增加y值
// 			x += GetX(TOP_CONTENT_EIDTH);
// 		}
		//写标题
		//pDC->TextOut(x, y, strTitle[i]);
		pDC->drawText(x,y, pDC->fontMetrics().width(strTitle[i]), pDC->fontMetrics().height(), Qt::AlignLeft, strTitle[i]);

		//写内容
		x = x + TOP_TITLE_WIDTH;
		//CSize sizeValue = pDC->GetTextExtent(m_strTiHuoTop[i]);

		QRect sizeValue = fm1.boundingRect(m_strTiHuoTop[i]);
		if (sizeValue.width() > nValueWidth)
		{
			int nLine = sizeValue.width() / nValueWidth + 1;
			QRect rcValue(x, y, x + (nValueWidth), y + (nFontHeight * nLine));
			
			//pDC->DrawText(m_strTiHuoTop[i],rcValue, DT_LEFT|DT_WORDBREAK);
			pDC->drawText(rcValue, m_strTiHuoTop[i]);
			//(x, y, pDC->fontMetrics().width(), pDC->fontMetrics().height(), Qt::AlignLeft, strTitle[i]);

			y += (nFontHeight* (nLine-1));//GetY(rcValue.Height()) - GetY(TOP_LINE_HEIGHT);
		}
		else
			//pDC->TextOut(x, y, m_strTiHuoTop[i]);
		    pDC->drawText(x, y, pDC->fontMetrics().width(strTitle[i]), pDC->fontMetrics().height(), Qt::AlignLeft, strTitle[i]);
	}

	y += GetY(TOP_MIDDLE_DISTANCE + TOP_LINE_HEIGHT);

	int x1,y1;
	//画表格的横线
	MIDDLE_COLUMN_WIDTH = (m_nPageWidth - TOP_BEGIN_X*2)/2;
	MIDDLE_BEGIN_X = TOP_BEGIN_X;
	x1 = MIDDLE_BEGIN_X + 2*(MIDDLE_COLUMN_WIDTH); //终点的x值
	y1 = y;
	for(i = 0; i < 4; i++)
	{
		//pDC->MoveTo((MIDDLE_BEGIN_X), y1); //起点
		//pDC->LineTo(x1, y1);             //终点
		pDC->drawLine((MIDDLE_BEGIN_X), y1,x1, y1);
		y1 += GetY(MIDDLE_LINE_HEIGTH);
		if (i == 0)
		{
			y1 +=  nFontHeight+GetY(10);//第一格增加一行文字的高度
		}
	}
	//画表格的竖线
	x1 = (MIDDLE_BEGIN_X);
	y1 = y + 3*GetY(MIDDLE_LINE_HEIGTH) + nFontHeight+GetY(10); //终点的y值
	for(i = 0; i< 3; i ++)
	{
		//pDC->MoveTo(x1, y);
		//pDC->LineTo(x1, y1);
		pDC->drawLine(x1, y, x1, y1);

		x1 += (MIDDLE_COLUMN_WIDTH); //x每次增减一个单元格的宽度
	}

	//TEXTMETRIC textmetric;
	//pDC->GetTextMetrics(&textmetric);
	//写表格字
	y1 = y - GetY(MIDDLE_LINE_HEIGTH);
	QString strTable[8] = {"交割品种", "标准提货重量(kg)", "Delivery variety",
		"Standard weight(Kg)",
		m_strVarietyId, m_strAmount, "合计  In Total", m_strAmount};
	for(i = 0; i < 8; i++)
	{
		if( i %2 == 0)
		{
			x1  = (MIDDLE_BEGIN_X);
			if (i == 2)
			{
				y1 += nFontHeight+GetY(10);//第一格2行文字 
			} 
			else
			{
				y1 += GetY(MIDDLE_LINE_HEIGTH);
			}
			
		}
		else
		{
			x1 += (MIDDLE_COLUMN_WIDTH);
		}
		//获得文字的个数，用于文字居中
		int len = strTable[i].length(); //中文字等于2个
		//获得写入文字的开始点
		int iDisX = ((MIDDLE_COLUMN_WIDTH) - (len/2)*GetY(MY_FONT_SIZE))/2;
		//pDC->TextOut(x1+iDisX, y1 + GetY(MIDDLE_TEXT_DISTANCE_Y),  strTable[i]);

		pDC->drawText(x1 + iDisX, y1 + GetY(MIDDLE_TEXT_DISTANCE_Y), pDC->fontMetrics().width(strTable[i]), pDC->fontMetrics().height(), Qt::AlignLeft, strTable[i]);

	}

	y += 3*GetY(MIDDLE_LINE_HEIGTH) + sizeLeft.height();
	y += GetY(MIDDLE_LINE_HEIGTH);

	//写底端文字
	QString strBottom[4] = {"制单(Tabulator):", m_strTradeName, "审批(Approver):", ""};
	y1 = y;
	x1 = TOP_BEGIN_X;
	//pDC->TextOut(x1, y1, strBottom[0]);

	QRect si = fm1.boundingRect(strTitle[12]);
	//CSize si = pDC->GetTextExtent(strBottom[0]);


	//pDC->TextOut(x1 + (si.cx), y1, strBottom[1]);	
	pDC->drawText(x1 + (si.width()), y1 , pDC->fontMetrics().width(strBottom[1]), pDC->fontMetrics().height(), Qt::AlignLeft, strBottom[1]);


	x1 = (TOP_BEGIN_X*2 + sizeLeft.width() + MIDDLE_BEGIN_X + MIDDLE_COLUMN_WIDTH)/2;//value起始位置与表格中线的中间位置
	//pDC->TextOut(x1, y1, strBottom[2]);
	pDC->drawText(x1 , y1 , pDC->fontMetrics().width(strBottom[2]), pDC->fontMetrics().height(), Qt::AlignLeft, strBottom[2]);

	x1 += BOTTOM_TITLE_WIDTH;
	//pDC->TextOut(x1 , y1, strBottom[3]);
	pDC->drawText(x1 , y1 , pDC->fontMetrics().width(strBottom[3]), pDC->fontMetrics().height(), Qt::AlignLeft, strBottom[3]);

}

//初始化提货单打印相关数据
void QReportBase::IniTiHuoData( Rsp5103 &rsp )
{
	#ifdef _VERSION_GF
	int iValuePoiter[17] = {1, 24, 7, 8, 2, 4, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27};
	#else
	int iValuePoiter[17] = {1, 22, 5, 6, 2, 4, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25};
	#endif
	ArrayListMsg aMsg = rsp.alm_take_sheet_detail.GetValue(0);
	for(int i = 0; i < 17; i++)
	{
		string value = aMsg.GetStringEx(iValuePoiter[i]);
		m_strTiHuoTop[i] = CHJGlobalFun::str2qstr(value);
	}

	//交割品种和提货重量
	#ifdef _VERSION_GF
	m_strVarietyId = CHJGlobalFun::str2qstr(aMsg.GetStringEx(10));
	m_strAmount = CHJGlobalFun::str2qstr(aMsg.GetStringEx(11));
	#else
	m_strVarietyId = CHJGlobalFun::str2qstr(aMsg.GetStringEx(8));
	m_strAmount = CHJGlobalFun::str2qstr(aMsg.GetStringEx(9));
	#endif
	double fAmout = m_strAmount.toDouble()/1000.0;
	//m_strAmount.Format("%.2f", fAmout);
	m_strAmount.number(fAmout,'f',2);
	//制单
	m_strTradeName = CHJGlobalFun::str2qstr(aMsg.GetStringEx(3));
}

/*公共**************************************************************************************************************/
// 设置显示的模式，是报表还是打印预览
void QReportBase::SetShowMode( EShowMode eShowMode )
{
	m_eShowMode = eShowMode;
}
// 画页码信息，子类可以重写
void QReportBase::WritePage( QPainter* pDC, PrintInfo* pInfo )
{

}

int QReportBase::DrawTableEx( QPainter* pDC, int iLeftTopY, int iLineCount, int iTitleCount, int iTitleWid, const vector<RptDataInfo> &vec, QString strLTitle, QString strRTitle, const vector<Table1Info> &vColumnInfo )
{
	int x1,y1;
	int x2,y2;
	int i;

	QFont* font = new QFont("宋体", 10);
	pDC->setFont(*font);
	//画标题
	y1 = GetY(iLeftTopY - TABLE_TITLE_HEIGHT);
	//pDC->TextOut(GetX(LEFT_X + TEXT_DISTANCE_X), y1, strLTitle);
	pDC->drawText(GetX(LEFT_X + TEXT_DISTANCE_X), y1, pDC->fontMetrics().width(strLTitle), pDC->fontMetrics().height(), Qt::AlignLeft, strLTitle);


	//pDC->SetTextAlign(TA_RIGHT);
	//pDC->TextOut(GetX(RIGHT_X - TEXT_DISTANCE_X), y1, strRTitle);
	pDC->drawText(GetX(RIGHT_X - TEXT_DISTANCE_X)- pDC->fontMetrics().width(strRTitle), y1, pDC->fontMetrics().width(strRTitle), pDC->fontMetrics().height(), Qt::AlignRight, strRTitle);

	//pDC->SetTextAlign(TA_LEFT);


	//画整个table的矩形
	x1 = GetX(LEFT_X);
	y1 = GetY(iLeftTopY);
	x2 = GetX(RIGHT_X);
	y2 = y1 + GetY(TABLE_LINE_HEIGHT)*iLineCount;
	//pDC->Rectangle(x1, y1, x2, y2); 
	pDC->drawRect(x1, y1, x2-x1, y2-y1);
	//画行
	for(i = 1; i < iLineCount; i++)
	{
		y1 += GetY(TABLE_LINE_HEIGHT);
		//pDC->MoveTo(GetX(LEFT_X), y1);
		//pDC->LineTo(GetX(RIGHT_X), y1);
		pDC->drawLine(GetX(LEFT_X), y1, GetX(RIGHT_X), y1);
	}

	//画列
	x1 = GetX(LEFT_X);
	y1 = GetY(iLeftTopY); //每一列线上面的y值
	y2 = GetY(iLeftTopY) + GetY(TABLE_LINE_HEIGHT)*iLineCount;//每一列线的下面的y值
	//iContentWid = GetX(TABLE_WIDTH)/iTitleCount - iTitleWid; //内容列的宽度
	int iContentAllWid = GetX(TABLE_WIDTH) - iTitleCount*iTitleWid;
	for(i = 1; i < iTitleCount*2; i++)
	{
		if(i%2 == 1)
		{
			// 标题右边的列
			x1 += iTitleWid;
		}
		else
		{
			// 内容右边的列
			x1 += (int)(((float)vColumnInfo.at(i/2-1).iColumnPercent)/100.0*iContentAllWid);
		}

		//pDC->MoveTo(x1, y1);
		//pDC->LineTo(x1, y2);

		pDC->drawLine(x1, y1, x1, y2);
	}

	 
	//写入内容（标题和内容）
	for(i = 0; i < iLineCount*iTitleCount; i++)
	{ 
		if(i == 0)
		{
			x1 = GetX(LEFT_X);
			y1 = GetY(iLeftTopY);
		}
		else if(i%iTitleCount == 0) //如果是下一行
		{
			x1  = GetX(LEFT_X);
			y1 += GetY(TABLE_LINE_HEIGHT);
		}
		else
		{
			//x1 += GetX(TABLE_WIDTH)/iTitleCount;
			x1 += iTitleWid + (int)(((float)vColumnInfo.at((i-1)%iTitleCount).iColumnPercent)/100.0*iContentAllWid);
		}
		QString str1 = vec.at(i).strTitle;
		//pDC->TextOut(x1 + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), vec.at(i).strTitle);
		pDC->drawText(x1 + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(vec.at(i).strTitle), pDC->fontMetrics().height(), Qt::AlignLeft, vec.at(i).strTitle);



		if(vec.at(i).isLeftAli)
		{
			//pDC->TextOut(x1 + iTitleWid + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), vec.at(i).strContent);	
		pDC->drawText(x1 + iTitleWid + GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(vec.at(i).strContent), pDC->fontMetrics().height(), Qt::AlignLeft, vec.at(i).strContent);


		}
		else
		{
			//pDC->SetTextAlign(TA_RIGHT);
			int iNextBColumn = (int)(((float)vColumnInfo.at((i)%iTitleCount).iColumnPercent)/100.0*iContentAllWid);
			//pDC->TextOut(x1  + iTitleWid + iNextBColumn - GetX(TEXT_DISTANCE_X), y1 + GetY(TEXT_DISTANCE_Y), vec.at(i).strContent);
		pDC->drawText(x1 + iTitleWid + iNextBColumn - GetX(TEXT_DISTANCE_X)- pDC->fontMetrics().width(vec.at(i).strContent), y1 + GetY(TEXT_DISTANCE_Y), pDC->fontMetrics().width(vec.at(i).strContent), pDC->fontMetrics().height(), Qt::AlignRight, vec.at(i).strContent);


		}

		//pDC->SetTextAlign(TA_LEFT);
	}

	return GetY(iLeftTopY) + GetY(iLineCount*TABLE_LINE_HEIGHT + TABLE_TITLE_HEIGHT + TABLE_TO_TITLE_DIS);
}

/*
int QReportBase::GetX( int x )
{
	return x*m_xRate;
}

int QReportBase::GetY( int y )
{
	return y*m_yRate;
}
*/



