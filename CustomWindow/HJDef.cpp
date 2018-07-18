#include "HJDef.h"
#include "hjconstant.h"

QString GetYesNoName(const QString &sID)
{
	if(sID == CONSTANT_YES)
		return "ÊÇ";
	else
		return "·ñ";
}

MarketType GetMarketFromExchType( const QString &sExchTypeID )
{
	if( sExchTypeID == CONSTANT_EXCH_CODE_DEFER_OPEN_LONG || 
		sExchTypeID == CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT ||
		sExchTypeID == CONSTANT_EXCH_CODE_DEFER_COV_LONG ||
		sExchTypeID == CONSTANT_EXCH_CODE_DEFER_COV_SHORT ||
		sExchTypeID == CONSTANT_EXCH_CODE_DELIVERY_LONG ||
		sExchTypeID == CONSTANT_EXCH_CODE_DELIVERY_SHORT ||
		sExchTypeID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG ||
		sExchTypeID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT)
		return e_mk_Defer;
	else if( sExchTypeID == CONSTANT_EXCH_CODE_SPOT_BUY ||
		sExchTypeID == CONSTANT_EXCH_CODE_SPOT_SELL )
		return e_mk_Spot;
	else if( sExchTypeID == CONSTANT_EXCH_CODE_FORWARD_LONG ||
		sExchTypeID == CONSTANT_EXCH_CODE_FORWARD_SHORT )
		return e_mk_Forward;
	else
		return e_mk_Others;
}

extern MarketType GetMarketType( const QString &sMarketID )
{
	if( sMarketID == CONSTANT_B_MARKET_ID_DEFER )
		return e_mk_Defer;
	else if( sMarketID == CONSTANT_B_MARKET_ID_SPOT )
		return e_mk_Spot;
	else if( sMarketID == CONSTANT_B_MARKET_ID_DEFER )
		return e_mk_Forward;
	else
		return e_mk_Others;
}

extern bool IsDeferSpecial( const QString &sExchTypeID )
{
	if( sExchTypeID == CONSTANT_EXCH_CODE_DELIVERY_LONG 
		|| sExchTypeID == CONSTANT_EXCH_CODE_DELIVERY_SHORT  
		|| sExchTypeID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG
		|| sExchTypeID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT )
	{
		return true;
	}
	else
	{
		return false;
	}
}

extern bool IsDeferInst( const QString &sInst )
{
	if( sInst == "Au(T+D)" || sInst == "Ag(T+D)" || sInst == "Au(T+N1)" || sInst == "Au(T+N2)" )
		return true;
	else
		return false;
}
