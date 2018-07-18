#include "PosiMgr.h"

#include "HJConstant.h"

CPosiMgr::CPosiMgr(void)
{
	m_QMapDeferPosi = NULL;
	m_QMapStoreInfo = NULL;
}

CPosiMgr::~CPosiMgr(void)
{ 
}

CPosiMgr& CPosiMgr::GetInstance()
{
	static CPosiMgr theSingleton;
	return theSingleton;
}

void CPosiMgr::Initialize( QMap<QString, DeferPosi> &QMapDeferPosi, QMap<QString, StoreInfo> &QMapStoreInfo )
{
	m_QMapDeferPosi = &QMapDeferPosi;
	m_QMapStoreInfo = &QMapStoreInfo;
}

const DeferPosiInfo* CPosiMgr::GetPosiInfo( const QString &sInsID, const QString &sExchID )
{
	bool bLong;
	if( IsLong(bLong, sExchID) )
	{
		return GetPosiInfo(sInsID, bLong);
	}

	return NULL;
}

const DeferPosiInfo* CPosiMgr::GetPosiInfo( const QString &sInsID, bool bLong )
{
	if( !sInsID.isEmpty() )
	{
		auto it = m_QMapDeferPosi->find( sInsID );
		if(it != m_QMapDeferPosi->end())
		{
			return bLong ? &it->infoLong : &it->infoShort;
		}
	}

	return NULL;
}

const DeferPosi* CPosiMgr::GetPosiInfo( const QString &sInsID )
{
	if( !sInsID.isEmpty() )
	{
		auto it = m_QMapDeferPosi->find( sInsID );
		if(it != m_QMapDeferPosi->end())
		{
			return &it.value();
		}
	}

	return NULL;
}

bool CPosiMgr::IsLong( bool &bLong, const QString &sExchID )
{
	if( sExchID == CONSTANT_EXCH_CODE_DEFER_OPEN_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_COV_LONG )
		bLong = true;
	else if( sExchID == CONSTANT_EXCH_CODE_DEFER_COV_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_COV_SHORT )
		bLong = false;
	else
		return false;

	return true;
}

const StoreInfo* CPosiMgr::GetStoreInfo( const QString &sInsID )
{
	if( !sInsID.isEmpty() )
	{
		auto it = m_QMapStoreInfo->find( sInsID );
		if(it != m_QMapStoreInfo->end())
		{
			return &it.value();
		}
	}

	return NULL;
}
