#include "HJCommon.h"
#include "Global.h"
#include "TraderCpMgr.h"
#pragma execution_character_set("utf-8")

CHJCommon::CHJCommon(void)
{
}

CHJCommon::~CHJCommon(void)
{
}
// 根据买卖标志获取现货的交易类型ID
QString CHJCommon::GetSpotId( const QString &sBuySellFlag )
{
	return sBuySellFlag == "b" ? CONSTANT_EXCH_CODE_SPOT_BUY : CONSTANT_EXCH_CODE_SPOT_SELL;
}
// 根据买卖标志获取远期的交易类型ID
QString CHJCommon::GetForwardId( const QString &sBuySellFlag  )
{
	return sBuySellFlag == "b" ? CONSTANT_EXCH_CODE_FORWARD_LONG : CONSTANT_EXCH_CODE_FORWARD_SHORT;
}
// 根据开平和买卖标志获取延期的交易类型ID "0"表示开仓 "b"表示多仓
QString CHJCommon::GetDeferId( const QString &sOffFlag, const QString &sBuySellFlag )
{
	return sOffFlag == "0" ? (sBuySellFlag == "b" ? CONSTANT_EXCH_CODE_DEFER_OPEN_LONG : CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT) : (sBuySellFlag == "b" ? CONSTANT_EXCH_CODE_DEFER_COV_SHORT : CONSTANT_EXCH_CODE_DEFER_COV_LONG);
}
// 根据买卖标志获取延期交收的交易类型ID
QString CHJCommon::GetDDAOrderId( const QString &sBuySellFlag )
{
	return sBuySellFlag == "b" ? CONSTANT_EXCH_CODE_DELIVERY_LONG : CONSTANT_EXCH_CODE_DELIVERY_SHORT;
}
// 根据买卖标志获取延期中立仓的交易类型ID
QString CHJCommon::GetMAOrderId( const QString &sBuySellFlag )
{
	return sBuySellFlag == "b" ? CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG : CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT;
}

bool CHJCommon::IsLongPosition( const QString &sBuySellFlag )
{
	return sBuySellFlag == "b" ? true : false;
}


QString CHJCommon::GetDDAMatchId( const QString &sMiddleFlag, const QString &sBuySellFlag )
{
	return sMiddleFlag == "0" ? (sBuySellFlag == "b" ? "4045" : "4046") : (sBuySellFlag == "b" ? "4047" : "4048");
}

// 根据报单状态和未成交手数判断一笔报单是否有效
bool CHJCommon::CheckEntrFlowIsInvalidate( const QString &sState, int iRemainAmount )
{
	// 如果状态不是"正在申报","已报入","部分成交"、"无效"的一种，则为无效报单
	if(sState != "1" && sState != "o" && sState != "p" && sState != "2")
	{
		return true;
	}

	// 如果状态有效，但是未成交手数不大于0，则为无效报单
	if(iRemainAmount <= 0)
	{
		return true;
	}

	return false;
}

QString CHJCommon::GetOrderType( const QString &sLocalOrderNo, const QString &sStateId )
{
	if(sStateId == "2")
	{
		return QString("失败报单");
	}
	else
	{
		//char  ch  = sLocalOrderNo.at[sLocalOrderNo.length()-2];
		//return ch == '8'? QString("强平报单"): QString("正常报单");
		QString str1 = "强平报单";
		QString str2 = "正常报单";
		return sLocalOrderNo.at(sLocalOrderNo.length() - 2) == '8'? str1:str2 ;

	}
}
// 计算某一个品种买n手所需要的手续费
double CHJCommon::CalculateFare( const QString &sProdCode, double fUnite, double fPrice, const FareInfo &fi, int iHand /*= 1*/ )
{
	if (fi.fare_value.isEmpty())
	{
		return 0;
	}

	double fFare_Value = fi.fare_value.toFloat()*iHand;

	if (fi.fare_mode_id == CONSTANT_CT_FARE_MODE_BAL) // 按金额模式计算保证金
	{
		if (sProdCode == "Ag99.9" || sProdCode == CONSTANT_AGTD) // 如果是白银
		{
			return fFare_Value * fPrice * fUnite / 1000.0;
		}
		else
		{
			return fFare_Value * fPrice * fUnite;
		}
	}
	else if (fi.fare_mode_id == CONSTANT_CT_FARE_MODE_WEIGHT) // 重量(元/克)
	{
		return fFare_Value * fUnite;
	}
	else if (fi.fare_mode_id == CONSTANT_CT_FARE_MODE_HAND) // 手数(元/手)
	{
		return fFare_Value;
	}
	else
	{
		return 0;
	}
}



double CHJCommon::HandToGram( const QString &sProdCode, double dMeasureUnit )
{
	if (sProdCode == "Ag99.9" || sProdCode == "Ag(T+D)")
	{
		// 如果是白银 以千克计算
		dMeasureUnit = dMeasureUnit / 1000.00;
	}
	
	return dMeasureUnit;
}

double CHJCommon::CalculateFareEx( const QString &sProdCode, double fUnite, double dExchBal, const FareInfo &fi, int iHand, bool bMatch /*= false*/ )
{
	if (fi.fare_value.isEmpty())
	{
		return 0;
	}

	// 初始化总额为手续费收取的比率
	double fFare_Value = fi.fare_value.toFloat();

	if (fi.fare_mode_id == CONSTANT_CT_FARE_MODE_BAL) // 按金额模式计算保证金
	{
		fFare_Value = dExchBal * fFare_Value; 
	}
	else if (fi.fare_mode_id == CONSTANT_CT_FARE_MODE_WEIGHT) // 重量(元/克)
	{
		fFare_Value = iHand * fUnite * fFare_Value;
	}
	else if (fi.fare_mode_id == CONSTANT_CT_FARE_MODE_HAND) // 手数(元/手)
	{
		fFare_Value = iHand * fFare_Value;
	}
	else
	{
		fFare_Value = 0.00;
	}

	// 解决浮点数精度的问题 876.82499999999993
	//if(bMatch) fFare_Value += 0.00000001;
	fFare_Value += 0.00000001;

	return CHJGlobalFun::DoubleFormat(fFare_Value);
}

ExchFare CHJCommon::GetExchFareValueByExchBal( const QString &sProdCode, double dUnite, double dExchBal, int iHand, const FareInfo &fiBourse, const FareInfo &fiMember, bool bMatch /*= false*/)
{
	ExchFare exch_fare;
	exch_fare.iVolume = iHand;

	// 计算交易所费用
	exch_fare.dBourseFare = CalculateFareEx(sProdCode, dUnite, dExchBal, fiBourse, iHand, bMatch);

	// 计算会员收的总费用费用
	exch_fare.dMemberFare = CalculateFareEx(sProdCode, dUnite, dExchBal, fiMember, iHand, bMatch);

	// 判断设置的会员费率是否包含交易所费率，暂不做处理

	// 返回
	return exch_fare;
}


//#include "ScrollEditEx.h"
#include "Mgr/QuotationMgr.h"
bool CHJCommon::SetPriceControl( CScrollEditEx &editCtl, const QString &sInsID )
{
	const QUOTATION* pQT  = g_QuoMgr.GetQuotation(sInsID);
	return SetPriceControl( editCtl, sInsID, pQT );
}

bool CHJCommon::SetPriceControl( CScrollEditEx &editCtl, const QString &sInsID, const QUOTATION* pQT )
{
	if( pQT != NULL )
	{
		//if( sInsID == "Ag99.9" || sInsID == "Ag(T+D)" )
			//editCtl.IniInt(CNumCommn::e_both, 2, pQT->m_uiLowLimit/g_Global.m_dQTFactor, pQT->m_uiHighLimit/g_Global.m_dQTFactor, false);
            //editCtl.IniInt(CNumCommn::e_both, 2, 0, 100000, false);
		//else
			//editCtl.IniFloat(CNumCommn::e_both, 2, 0.01, 2, pQT->m_uiLowLimit/g_Global.m_dQTFactor, pQT->m_uiHighLimit/g_Global.m_dQTFactor, false);
            //editCtl.IniFloat(CNumCommn::e_both, 2, 0.01, 2, 0, 100000, false);

		return true;
	}
	else
	{
		return false;
	}
}

bool CHJCommon::IsDefer( const QString &sInst )
{
	const ProdCodeInfo* pInfo = NULL;
	for(size_t i = 0; i < g_TraderCpMgr.m_vProdCode.size(); i++)
	{
		pInfo = &g_TraderCpMgr.m_vProdCode.at(i);
		if( pInfo->prod_code == sInst )
		{
			if( pInfo->market_id == CONSTANT_B_MARKET_ID_DEFER )
				return true;
		}	
	}

	return false;
}

