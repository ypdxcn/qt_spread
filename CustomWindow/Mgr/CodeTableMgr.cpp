#include "CodeTableMgr.h"

#include "..\TranMsg.h"
#include "HJGlobalFun.h"

CCodeTableMgr::CCodeTableMgr(void)
{
	m_iBufSize = 100000;
	m_csTitle = "code_table";
	m_csTitleType = "CodeTableDataType";
}

CCodeTableMgr::~CCodeTableMgr(void)
{
}

CCodeTableMgr& CCodeTableMgr::GetInstance()
{
	static CCodeTableMgr theSingleton;
	return theSingleton;
}

// 获取一组码表
QVector<const CodeValue*> CCodeTableMgr::GetGroupCode( const QString &sCodeType )
{
	QVector<const CodeValue*> vecGroupCode;

	auto range = m_mapCode.equal_range( sCodeType );
	for(CIT i= range.first; i != range.second; ++i)
	{
		vecGroupCode.append( &(i.value() ));
	}

	return vecGroupCode;
}

// 根据码表字段ID或者码表字段的值
QString CCodeTableMgr::GetCodeNameFromID( const QString &sCodeType, const QString &sCodeID )
{
	auto range = m_mapCode.equal_range( sCodeType );

	for( CIT i= range.first; i != range.second; ++i )
	{
		if( i->code_id == sCodeID )
			return i->code_desc;
	}

	return "";
}

// 根据码表字段值来获取码表字段ID
QString CCodeTableMgr::GetCodeIDFromName( const QString &sCodeType, const QString &sCodeName )
{
	auto range = m_mapCode.equal_range( sCodeType );

	for( CIT i= range.first; i != range.second; ++i )
	{
		if( i->code_desc == sCodeName )
			return i->code_id;
	}

	return "";
}

// 初始化类变量
bool CCodeTableMgr::Initialize( const ArrayListMsg *pCodeTable /*= NULL */ )
{
	m_mapCode.clear();

	return CAlmHtmBase::Initialize(pCodeTable);
}


// 从服务器加载码表
bool CCodeTableMgr::LoadDataFromServer( void )
{
	Rsp1001 rsp;
	if( CTranMsg::Handle1001( rsp ) )
	{
		if( rsp.htm_code_list.size() > 0 )
		{
			// 写入数据
			WriteDataToFile( CHJGlobalFun::str2qstr(rsp.htm_code_list.ToString()), true );

			// 加载到内存
			LoadFromHtm( rsp.htm_code_list );

			return true;
		}
	}

	return false;
}

// 从ArrayListMsg加载代码表
void CCodeTableMgr::LoadFromAlm( const ArrayListMsg &alm )
{
	// 加载数据到内存
	for( size_t i = 0; i < alm.size(); i++ ) //遍历返回结果
	{ 
		if( IsNeed(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 0)) ))
		{
			CodeValue stCodeValue(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 1)), CHJGlobalFun::str2qstr(alm.GetStringEx(i, 2))) ;
			//m_mapCode.insert( make_pair( alm.GetQStringEx(i, 0), stCodeValue ) );
			m_mapCode.insert(CHJGlobalFun::str2qstr(alm.GetStringEx(i, 0)), stCodeValue);
		}
	}
}

void CCodeTableMgr::LoadFromHtm( HashtableMsg &htm )
{
	for( size_t i = 0; i < htm.size(); i++ )
	{
		if( IsNeed(CHJGlobalFun::str2qstr(htm.GetString(i, "code_type"))) )
		{
			CodeValue stCodeValue(CHJGlobalFun::str2qstr(htm.GetString(i, "code_id")), CHJGlobalFun::str2qstr(htm.GetString(i, "code_desc")));
			//m_mapCode.insert( make_pair( htm.GetQString(i, "code_type"), stCodeValue ) );
			m_mapCode.insert(CHJGlobalFun::str2qstr(htm.GetString(i, "code_type")), stCodeValue);
		}
	}
}

bool CCodeTableMgr::IsNeed( const QString &sValue )
{
	if( sValue == "exch_code" || sValue == "b_offset_flag" || sValue == "fee_code" || sValue == "sub_fee_code" || sValue == "b_market_id" 
		|| sValue == "b_entr_stat"  || sValue == "cancel_flag" || sValue == "term_type" || sValue == "access_way" || sValue == "check_stat"
		|| sValue == "b_longorshort" || sValue == "b_sheet_stat" || sValue == "b_pickup_type" || sValue == "cert_type" || sValue == "acct_stat"
		|| sValue == "b_inst_stat" || sValue == "fc_order_state" || sValue == "b_deli_flag" || sValue == "paginal_num" 
		|| sValue == "match_stat" || sValue == "match_acct_stat" || sValue == "simu_question" || sValue == "m_sys_stat")
		return true;
	else
		return false;

}

void CCodeTableMgr::GetExchTypeName( QVector<const CodeValue*> &vecCode, MarketType eMarket )
{
	// 获取所有的交易方式
	QVector<const CodeValue*> vecGroupCode = GetGroupCode("exch_code");
	if( eMarket == e_mk_Others )
	{
		vecCode = vecGroupCode;
		EraseUselessExchType(vecCode);
	}
	else
	{
		for( auto it = vecGroupCode.begin(); it != vecGroupCode.end(); it++ )
		{
			if( GetMarketFromExchType((*it)->code_id) == eMarket )
				vecCode.push_back(*it);
		}
	}
}

void CCodeTableMgr::EraseUselessExchType( QVector<const CodeValue*> &vecCode )
{
	for( auto it = vecCode.begin(); it != vecCode.end(); it++ )
	{
		if( (*it)->code_id == "4061" )
		{
			vecCode.erase(it);
			break;
		}
	}
}
