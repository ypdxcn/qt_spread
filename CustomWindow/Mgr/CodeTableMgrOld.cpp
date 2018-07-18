#include "CodeTableMgrOld.h"

#include "..\TranMsg.h"
#include "TraderCPMgr.h"
#include "HJGlobalFun.h"

#pragma execution_character_set("utf-8")

CCodeTableMgrOld::CCodeTableMgrOld(void)
{
	m_iBufSize = 1024*1024; // mod by Jerry Lee, 修改serverdata.ini中码表读入不正确的问题, 2013-3-5
	m_csTitle = "code_table";
	m_csTitleType = "CodeTableDataType";
}

CCodeTableMgrOld::~CCodeTableMgrOld(void)
{
}

CCodeTableMgrOld& CCodeTableMgrOld::GetInstance()
{
	static CCodeTableMgrOld theSingleton;
	return theSingleton;
}

// 初始化类变量
bool CCodeTableMgrOld::Initialize( const ArrayListMsg *pCodeTable /*= NULL */ )
{
	SetDelegate();

	return CAlmHtmBase::Initialize(pCodeTable);
}

// 从服务器加载码表
bool CCodeTableMgrOld::LoadDataFromServer( void )
{
	Rsp1001 rsp;
	if( CTranMsg::Handle1001( rsp ) )
	{
		if( rsp.htm_code_list.size() > 0 )
		{
			// 写入数据
			WriteDataToFile(CHJGlobalFun::str2qstr( rsp.htm_code_list.ToString()), true );

			// 加载到内存
			LoadFromHtm( rsp.htm_code_list );

			return true;
		}
	}

	return false;
}

// 从ArrayListMsg加载代码表
void CCodeTableMgrOld::LoadFromAlm( const ArrayListMsg &alm )
{
	// 加载数据到内存
	for( size_t i = 0; i < alm.size(); i++ ) //遍历返回结果
	{ 
		ArrayListMsg aMsg = alm.GetValue(i); //获取结果的第一个组数据

		MyCodeInfo stCodeTable( aMsg.GetStringEx(0).c_str(), aMsg.GetStringEx(1).c_str(), aMsg.GetStringEx(2).c_str());
		AddValue( stCodeTable );
	}
}

void CCodeTableMgrOld::LoadFromHtm( HashtableMsg &htm )
{
	for( size_t i = 0; i < htm.size(); i++ )
	{

		MyCodeInfo stCodeTable( htm.GetString(i, "code_type").c_str()\
			, htm.GetString(i, "code_id").c_str(), \
			CHJGlobalFun::str2qstr(htm.GetString(i, "code_desc")));
		AddValue( stCodeTable );
	}
}

bool CCodeTableMgrOld::IsNeed( const string &sValue )
{
	if( sValue == "exch_code" || sValue == "b_offset_flag" || sValue == "fee_code" || sValue == "sub_fee_code" || sValue == "b_market_id" 
		|| sValue == "b_entr_stat"  || sValue == "cancel_flag" || sValue == "term_type" || sValue == "access_way" || sValue == "check_stat"
		|| sValue == "b_longorshort" || sValue == "b_sheet_stat" || sValue == "b_pickup_type" || sValue == "cert_type" || sValue == "acct_stat"
		|| sValue == "b_inst_stat" || sValue == "fc_order_state" || sValue == "b_deli_flag" || sValue == "paginal_num" 
		|| sValue == "match_stat" || sValue == "match_acct_stat" || sValue == "simu_question")
		return true;
	else
		return false;
}

void CCodeTableMgrOld::SetDelegate( void )
{
	m_vExchCode     = &g_TraderCpMgr.m_vExchCode;
	m_vOffsetFlag   = &g_TraderCpMgr.m_vOffsetFlag;
	m_vFeeCode      = &g_TraderCpMgr.m_vFeeCode;
	m_vSubFeeCode   = &g_TraderCpMgr.m_vSubFeeCode;
	m_vLongShort    = &g_TraderCpMgr.m_vLongShort;
	m_vMarketId     = &g_TraderCpMgr.m_vMarketId;
	m_vEntrState    = &g_TraderCpMgr.m_vEntrState;
	m_vCancelFlag   = &g_TraderCpMgr.m_vCancelFlag;
	m_vTermType     = &g_TraderCpMgr.m_vTermType;
	m_vAccessWay    = &g_TraderCpMgr.m_vAccessWay;
	m_vCheckState   = &g_TraderCpMgr.m_vCheckState;
	m_vTakeState    = &g_TraderCpMgr.m_vTakeState;
	m_vDrawType     = &g_TraderCpMgr.m_vDrawType;
	m_vCertType     = &g_TraderCpMgr.m_vCertType;
	m_vAcctState    = &g_TraderCpMgr.m_vAcctState;
	m_vInstState2   = &g_TraderCpMgr.m_vInstState2;
	m_vFCOrderState = &g_TraderCpMgr.m_vFCOrderState;
	m_vDeliFlag     = &g_TraderCpMgr.m_vDeliFlag;
	m_vPageInfo     = &g_TraderCpMgr.m_vPageInfo;
}

void CCodeTableMgrOld::AddValue( const MyCodeInfo &stCodeTable )
{
	// 获取码表类型
	const QString &strCodeType = stCodeTable.code_type;

	if(strCodeType == "exch_code" && stCodeTable.code_id != "4061")
	{
		//添加交易类型，去掉撤单的选项(4061)
		m_vExchCode->push_back(stCodeTable);
	}
	else if(strCodeType == "b_offset_flag") 
	{
		// 添加开平标志
		m_vOffsetFlag->push_back(stCodeTable);
	}
	else if(strCodeType == "fee_code")     
	{
		// 初始化费用代码
		m_vFeeCode->push_back(stCodeTable);
	}
	else if(strCodeType=="sub_fee_code")    
	{
		// 子费用代码
		m_vSubFeeCode->push_back(stCodeTable);
	}
	else if(strCodeType == "b_market_id")
	{
		// 交易市场
		m_vMarketId->push_back(stCodeTable);
	}
	else if(strCodeType == "b_entr_stat")   
	{
		// 委托状态
		m_vEntrState->push_back(stCodeTable);
	}
	else if(strCodeType == "cancel_flag")   
	{
		// 撤单标志
		m_vCancelFlag->push_back(stCodeTable);
	}
	else if(strCodeType == "term_type")
	{
		//
		m_vTermType->push_back(stCodeTable);
	}
	else if(strCodeType == "access_way")    
	{
		// 存取方向
		m_vAccessWay->push_back(stCodeTable);
	}
	else if(strCodeType == "check_stat")    
	{
		// 操作方向
		m_vCheckState->push_back(stCodeTable);
	}
	else if(strCodeType == "b_longorshort") 
	{
		// 持仓方向
		m_vLongShort->push_back(stCodeTable);
	}
	else if(strCodeType == "b_sheet_stat")  
	{
		// 提货状态
		m_vTakeState->push_back(stCodeTable);
	}
	else if(strCodeType == "b_pickup_type") 
	{
		// 提货类型
		m_vDrawType->push_back(stCodeTable);
	}
	else if(strCodeType == "cert_type")     
	{
		// 证件类型
		m_vCertType->push_back(stCodeTable);
	}
	else if(strCodeType == "acct_stat")     
	{
		// 账户类型
		m_vAcctState->push_back(stCodeTable);
	}
	else if(strCodeType == "b_inst_stat")
	{
		// 交易所合约状态
		m_vInstState2->push_back(stCodeTable);
	}
	else if(strCodeType == "fc_order_state")
	{
		// 交易所合约状态
		m_vFCOrderState->push_back(stCodeTable);
	}
	else if(strCodeType == "b_deli_flag")
	{
		// 交易所合约状态
		m_vDeliFlag->push_back(stCodeTable);
	}
	else if(strCodeType == "paginal_num")
	{
		// 交易所合约状态
		m_vPageInfo->push_back(stCodeTable);
	}
}
