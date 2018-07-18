#include "TiHuoMgr.h"
#include "Global.h"
#include "TraderCpMgr.h"
#include "HJConstant.h"
#include "HJGlobalFun.h"
#include "TranMessage.h"

CTiHuoMgr::CTiHuoMgr(void)
{
}

CTiHuoMgr::~CTiHuoMgr(void)
{
}

CTiHuoMgr& CTiHuoMgr::GetInstance()
{
	static CTiHuoMgr theSingleton;
	return theSingleton;
}

void CTiHuoMgr::Initialize(  )
{
}

void CTiHuoMgr::QueryAll()
{
	// 查询当天的提货信息
	Rsp5101 rsp5101;
	if( Query(rsp5101, "", "", "") )
	{
		HandleQueryResult(rsp5101);
	}
}

/*
处理用户主动查询
rsp5101 查询结构
*/
void CTiHuoMgr::HandleUserQuery( Rsp5101 &rsp5101 )
{
	// 处理提货查询结果
	HandleQueryResult(rsp5101, false);

	// 刷新当日提货保证金冻结
	RefreshTakeFroze();
}

void CTiHuoMgr::HandleTodayTrade()
{
	// 查询当天的提货信息
	Rsp5101 rsp5101;
	if( Query(rsp5101, CHJGlobalFun::qstr2str(g_Global.m_strExchDate), CHJGlobalFun::qstr2str(g_Global.m_strExchDate), "") )
	{
		HandleQueryResult(rsp5101);
	}

	// 刷新当日提货保证金冻结
	RefreshTakeFroze();
}

void CTiHuoMgr::HandleCancel( const string &sLocalSheetNo )
{
	// 获取该提货单的信息
	Rsp5101 rsp5101;
	if( g_TiHuoMgr.Query(rsp5101, "", "", sLocalSheetNo) )
	{
		
	}
}

bool CTiHuoMgr::Query( Rsp5101 &rsp, const string &sBegin, const string &sEnd, const string &sLocalSheetNo )
{
	Req5101 req; //请求报文体

	req.oper_flag      = 1;
	req.local_sheet_no = sLocalSheetNo; //提货单单号，如果界面上有提货单输入，则填入该项，没有则取全部
	req.cust_id        = CHJGlobalFun::qstr2str(g_Global.m_strUserID); //用户id
	req.start_date     = sBegin; // 开始时间
	req.end_date       = sEnd;   // 结束时间

	//发送报文
	if(0 != g_TraderCpMgr.HandlePacket(rsp,req,"5101", 20))
	{
		//AfxMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
		return false;	
	}
	else
	{
		return g_TraderCpMgr.CheckRspCode(CHJGlobalFun::str2qstr(rsp.rsp_code), CHJGlobalFun::str2qstr(rsp.rsp_msg));
	}
}

bool CTiHuoMgr::HandleCalculate( const string &sOrderNo, const string sInsID, int nWeight, bool bFroze )
{
	FrozeInfo st;
	st.nWeight = nWeight;
	st.sInsID = sInsID;

	if( bFroze )
	{
		//m_mapFroze.insert(map<string, FrozeInfo>::value_type(sOrderNo, st));
		m_mapFroze.insert(sOrderNo, st);
	}
	else
	{
		//map<string, FrozeInfo>::iterator it = m_mapFroze.find(sOrderNo);
		auto it = m_mapFroze.find(sOrderNo);
		if( it != m_mapFroze.end() ) // 如果没有被处理过
		{
			m_mapFroze.erase(it);
		}
	}

	// 根据合约代码找到该笔库存
	auto itStore = g_TraderCpMgr.m_QMapStoreInfo.find(CHJGlobalFun::str2qstr(st.sInsID));
	if( itStore != g_TraderCpMgr.m_QMapStoreInfo.end() )
	{
		// 获得品种库存信息
		StoreInfo &stStore = itStore.value();
		if( bFroze )
			stStore.iUseFulAmt -= st.nWeight;
		else
			stStore.iUseFulAmt += st.nWeight;

		// 模拟发送一个现货报单
		bitset<5> bitvec;
		g_TraderCpMgr.SetBitSet( bitvec, false, true );
		SpotOrder stOrder;
		stOrder.instID = st.sInsID;
		g_TraderCpMgr.m_mapBdr[E_ONRECVRTNSPOTORDER].Broadcast(MSG_RECV_SPOT_ORDER, (void *)&stOrder, (void *)&bitvec, FALSE);
	} 

	return true;
}

void CTiHuoMgr::SetDay_take_margin( const string &sDay_take_margin )
{
	m_sDay_take_margin = sDay_take_margin;
}

void CTiHuoMgr::HandleQueryResult( Rsp5101 &rsp5101, bool bCancel )
{
	// 遍历
	QString sToday = CHJGlobalFun::GetCurDateFormat();
	for(size_t i = 0; i < rsp5101.htm_take_sheet_detail.size(); i++)
	{
		// 获取提货状态
		string sState = rsp5101.htm_take_sheet_detail.GetString(i, "stat");
		// 获取提货单号
		string sOrderNo = rsp5101.htm_take_sheet_detail.GetString(i, "local_sheet_no");

		// 判断是否当前
		QString sApp_exch_date = CHJGlobalFun::str2qstr(rsp5101.htm_take_sheet_detail.GetString(i, "app_exch_date"));
		if( sApp_exch_date == g_Global.m_strExchDate )
		{
			// 判断该单号是否处理
			auto it = m_mapFroze.find(sOrderNo);
			if( it == m_mapFroze.end() ) // 如果没有被处理过
			{
				// 如果是提货成功，则处理冻结
				if( CONSTANT_TIHUO_SUS == sState )
				{
					HandleCalculate( 
						sOrderNo,
						CHJGlobalFun::qstr2str(g_TraderCpMgr.GetVarietyName(CHJGlobalFun::str2qstr(rsp5101.htm_take_sheet_detail.GetString(i, "variety_id")))),
						rsp5101.htm_take_sheet_detail.GetInt(i, "app_amount"),
						true);
				}
			}
			else
			{
				// 如果是撤销提货成功，则处理解冻
				if( CONSTANT_TIHUO_CANCEL_TODAY == sState )
				{
					HandleCalculate( 
						sOrderNo,
						CHJGlobalFun::qstr2str(g_TraderCpMgr.GetVarietyName(CHJGlobalFun::str2qstr(rsp5101.htm_take_sheet_detail.GetString(i, "variety_id")))),
						rsp5101.htm_take_sheet_detail.GetInt(i, "app_amount"),
						false);
				}
			}
		}
		else
		{
			// 如果是会员次日撤销且撤销日期是今天
			if( sState == "4" && rsp5101.htm_take_sheet_detail.GetString(i, "cancel_exch_date") == CHJGlobalFun::qstr2str(g_Global.m_strExchDate ))
			{
                // mod by Jerry Lee, 2013-3-24, 解决可用库存一直累加的问题
                if (bCancel)
                {
                    HandleCalculate( 
                        sOrderNo,
						CHJGlobalFun::qstr2str(g_TraderCpMgr.GetVarietyName(CHJGlobalFun::str2qstr(rsp5101.htm_take_sheet_detail.GetString(i, "variety_id")))),
                        rsp5101.htm_take_sheet_detail.GetInt(i, "app_amount"),
                        false);
                }
			}
		}
	}
}

void CTiHuoMgr::RefreshTakeFroze()
{
	// 获取最新的提货冻结信息，如果
	Rsp1020 rsp1020;
	if(CTranMessage::Handle1020(rsp1020, '0', '1', '0', '0', '0', '0', true) == 0)
	{
		if( rsp1020.f_take_margin != m_sDay_take_margin )
		{
			g_TraderCpMgr.m_stFundInfo.dUseFulBal += atof(rsp1020.f_take_margin.c_str())-atof(m_sDay_take_margin.c_str());
			m_sDay_take_margin = rsp1020.f_take_margin;
			g_TraderCpMgr.m_mapBdr[E_REFRESHFUND].Broadcast(WM_REFRESH_FUND, 0, 0);
		}
	}
}


