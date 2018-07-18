#include "ProdCodeMgr.h"

#include "TranMsg.h"
#include "HJGlobalFun.h"

CProdCodeMgr& CProdCodeMgr::GetInstance()
{
	static CProdCodeMgr theSingleton;
	return theSingleton;
}

CProdCodeMgr::CProdCodeMgr(void)
{
	m_vProdCode = NULL;
	m_csTitle = "prod_code";
	m_csTitleType = "ProdCodeDataType";
	m_iBufSize = 2000;
}

CProdCodeMgr::~CProdCodeMgr(void)
{
}

bool CProdCodeMgr::Initialize( QVector<ProdCodeInfo> &vProdCode, const ArrayListMsg *pVariety_list /*= NULL */ )
{
	m_vProdCode = &vProdCode;
	m_vProdCode->clear();

	return CAlmHtmBase::Initialize( pVariety_list );
}

// 从服务器加载
bool CProdCodeMgr::LoadDataFromServer( void )
{
	Rsp1002 rsp;
	if( CTranMsg::Handle1002( rsp ) )
	{
		if( rsp.htm_prod_code.size() > 0 )
		{
			// 写入数据
//			WriteDataToFile( CHJGlobalFun::str2qstr(rsp.htm_prod_code.ToQString()), true );

			// 加载到内存
			LoadFromHtm( rsp.htm_prod_code );

			return true;
		}
	}

	return false;
}

// 从ArrayListMsg加载
void CProdCodeMgr::LoadFromAlm( const ArrayListMsg &alm )
{
	for( size_t i = 0; i < alm.size(); i++ ) //遍历返回结果
	{  
		ArrayListMsg aMsg = alm.GetValue(i); //获取结果的第一个组数据

		ProdCodeInfo prodecode;
		prodecode.prod_code         = aMsg.GetStringEx(0).c_str();
		prodecode.prod_name         = aMsg.GetStringEx(1).c_str();
		prodecode.variety_type      = aMsg.GetStringEx(2).c_str();
		prodecode.variety_id        = aMsg.GetStringEx(3).c_str();
		prodecode.tick              = aMsg.GetValue<double>(4);
		prodecode.upper_limit_value = aMsg.GetValue<double>(5);
		prodecode.lower_limit_value = aMsg.GetValue<double>(6);
		prodecode.active_flag       = aMsg.GetStringEx(7).c_str();
		prodecode.exch_unit         = aMsg.GetStringEx(8).c_str();
		prodecode.measure_unit      = aMsg.GetValue<double>(9);
		prodecode.market_id         = aMsg.GetStringEx(10).c_str();

		/*if( prodecode.active_flag == "1" )*/
		{
			m_vProdCode->push_back(prodecode);
		}
	}
}

// 从HashtableMsg加载
void CProdCodeMgr::LoadFromHtm( HashtableMsg &htm )
{
	for( size_t i = 0; i < htm.size(); i++ )
	{
		ProdCodeInfo prodecode;
		prodecode.prod_code         = CHJGlobalFun::str2qstr(htm.GetString(i, "prod_code"));
		prodecode.prod_name         = CHJGlobalFun::str2qstr(htm.GetString(i, "prod_name"));
		prodecode.variety_type      = CHJGlobalFun::str2qstr(htm.GetString(i, "variety_type"));
		prodecode.variety_id        = CHJGlobalFun::str2qstr(htm.GetString(i, "variety_id"));
		prodecode.tick              = htm.GetDouble(i, "tick");
		prodecode.upper_limit_value = htm.GetDouble(i, "upper_limit_value");
		prodecode.lower_limit_value = htm.GetDouble(i, "lower_limit_value");
		prodecode.active_flag       = CHJGlobalFun::str2qstr(htm.GetString(i, "active_flag"));
		prodecode.exch_unit         = CHJGlobalFun::str2qstr(htm.GetString(i, "exch_unit"));
		prodecode.measure_unit      = htm.GetDouble(i, "measure_unit");
		prodecode.market_id         = CHJGlobalFun::str2qstr(htm.GetString(i, "market_id"));

		/*if( prodecode.active_flag == "1" )*/
		{
			m_vProdCode->push_back(prodecode);
		}
	}
}

const ProdCodeInfo* CProdCodeMgr::GetProdeCode( const QString &sInsID )
{
	for( size_t i = 0; i < m_vProdCode->size(); i++ )
	{
		if(m_vProdCode->at(i).prod_code == sInsID)
		{
			return &m_vProdCode->at(i);
		}	
	}

	return NULL;
}

MarketType CProdCodeMgr::GetMarketType( const ProdCodeInfo* pInfo )
{
	if( pInfo != NULL )
	{
		return ::GetMarketType(pInfo->market_id);
	}
	else
		return e_mk_Others;
}



