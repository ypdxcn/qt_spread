#include "QuotationMgr.h"
#include "HJGlobalFun.h"

CQuotationMgr::CQuotationMgr(void)
{
	m_fFactor = 100.00;
}

CQuotationMgr::~CQuotationMgr(void)
{
}

CQuotationMgr& CQuotationMgr::GetInstance()
{
	static CQuotationMgr theSingleton;
	return theSingleton;
}

bool CQuotationMgr::Initialize(map<string, QUOTATION> &mapQuotation )
{
	m_mapQuotation = &mapQuotation;

	return true;
}

int CQuotationMgr::GetFormatPrice( QString &csPrice, const QString &sInsID, EPriceType eType )
{
	map<string, QUOTATION>::iterator it = m_mapQuotation->find( sInsID.toStdString() );
	if( it != m_mapQuotation->end() )
	{
		uint uiPrice = GetPrice(it->second, eType);
		//csPrice.Format("%.2f", uiPrice/m_fFactor);
		csPrice = QString::number(uiPrice / m_fFactor, 'f', 2);
		return uiPrice == 0 ? 1 : 0;
	}
	else
	{
		return -1;
	}
}

// 如何处理else的异常呢？
uint CQuotationMgr::GetPrice( const QUOTATION &qt, EPriceType eType ) const
{
	if( eType == PriceType_Last )
		return qt.m_uiLast;
	else if( eType == PriceType_Buy1 )
		return qt.m_Bid[0].m_uiPrice;
	else if( eType == PriceType_Sell1 )
		return qt.m_Ask[0].m_uiPrice;

	return 0;
}

const QUOTATION* CQuotationMgr::GetQuotation( const QString &sInsID ) const
{
//#ifdef _DEBUG
//	if( m_mapQuotation != NULL )
//	{
//#endif
	auto it = m_mapQuotation->find( CHJGlobalFun::qstr2str(sInsID ));

	return it != m_mapQuotation->end() ? &(it->second) : NULL;
//#ifdef _DEBUG
//	}
//#endif

	return nullptr;
}

uint CQuotationMgr::GetBSPrice( const QUOTATION &qt, EPriceType eType ) const
{
	return eType == PriceType_Buy1 ? qt.m_Bid[0].m_uiPrice : qt.m_Ask[0].m_uiPrice;
}

uint CQuotationMgr::GetValidePrice( const QUOTATION &qt ) const
{
	unsigned int uiOrg = qt.m_uiLast > 0 ? qt.m_uiLast : qt.m_uiLastSettle;

	if(uiOrg > 0) 
		return uiOrg;
	else
		return qt.m_uilastClose;
}

double CQuotationMgr::GetBSPriceEx( const QUOTATION &qt, EPriceType eType ) const
{
	uint uiValue = GetBSPrice(qt, eType);
	if( uiValue == 0 )
		uiValue = GetValidePrice(qt);

	return uiValue/m_fFactor;
}

