#pragma once

#include "HJConstant.h"
#include "HJGlobalFun.h"
#include <QString>


#define CONSTANT_OPEN_FARE_ID          "103"  // 开仓手续费ID
#define CONSTANT_COV_FARE_ID           "104"  // 平仓手续费ID

#define CONSTANT_FTD_T_MARGIN_LONG       "105"  // 开多仓保证金
#define CONSTANT_FTD_T_MARGIN_SHORT      "125"  // 开空仓保证金

#define CONSTANT_SPOT_BUY_FARE_ID      "101"  // 现货买入手续费ID
#define CONSTANT_SPOT_SELL_FARE_ID      "102"  // 现货卖出手续费ID

#define CONSTANT_EXCH_BAIL_FARE_ID     "105"  // 交易保证金IDID
#define CONSTANT_OVER_SPARE_BAIL_ID    "106"  // 溢短备付金率

#define CONSTANT_FTD_T_DELI_MARGIN    "108"  // 溢短备付金率

#define CONSTANT_CT_FARE_MODE_BAL      "1"  // 收费模式 金额  成交金额比率
#define CONSTANT_CT_FARE_MODE_WEIGHT   "2"  // 收费模式 重量  重量(元/克)
#define CONSTANT_CT_FARE_MODE_HAND     "3"  // 收费模式 手数  手数(元/手)

#define CONSTANT_AGTD                  "Ag(T+D)"  // 白银 Ag(T+D)
#define CONSTANT_PT9995                "Pt99.95"  // 铂金 Pt99.95

struct FareInfo
{
	// 收费模式
	QString fare_mode_id;

	// 收费模式名称
	QString fare_mode_name;

	/// 费率值
	QString fare_value;
};

struct ExchFare
{
	double dBourseFare;
	double dMemberFare;

	int iVolume;

	ExchFare()
	{
		dBourseFare = 0.00;
		dMemberFare = 0.00;
		iVolume = 0;
	}

	double getBourseFare45()
	{
		return CHJGlobalFun::DoubleFormat(dBourseFare);
	}

	double getMemberFare45()
	{
		return CHJGlobalFun::DoubleFormat(dMemberFare);
	}

	// 获得按每手为基础计算的交易所费用和会员费用的合，一般报单申报中
	double sumSingle()
	{
		double dBourseValue = CHJGlobalFun::DoubleFormat(dBourseFare);
		double dMemberValue = CHJGlobalFun::DoubleFormat(dMemberFare);
		double dTotal = dBourseValue + dMemberFare;

		if(iVolume > 0)
			dTotal = CHJGlobalFun::DoubleFormat(dTotal / iVolume, 2, ROUND_UP) * iVolume;

		return dTotal;
	}

	double sum()
	{
		return CHJGlobalFun::DoubleFormat(dBourseFare) + CHJGlobalFun::DoubleFormat(dMemberFare);
	}
};

class CScrollEditEx;
struct QUOTATION;

class CHJCommon
{
public:
	CHJCommon(void);
public:
	~CHJCommon(void);

	// 根据买卖方向获取现货交易类型id
	static QString GetSpotId(const QString &sBuySellFlag);

	// 根据买卖方向获取远期交易类型id
	static QString GetForwardId(const QString &sBuySellFlag );

	// 根据买卖方向和开平标志获取延期交易类型id
	static QString GetDeferId(const QString &sOffFlag, const QString &sBuySellFlag);

	// 根据买卖方向获取延期交收交易类型ID
	static QString GetDDAOrderId(const QString &sBuySellFlag);

	// 根据买卖方向获取中立仓交易类型ID
	static QString GetMAOrderId(const QString &sBuySellFlag);

	// 函数功能：根据费用模板,费用ID的费用计算模式计算相应的费用
	static double CalculateFareEx( const QString &sProdCode, double fUnite, double dExchBal, const FareInfo &fi, int iHand, bool bMatch = false );

	// 函数功能：根据费用ID的费用计算模式计算相应会员费用和交易所费用
	static ExchFare GetExchFareValueByExchBal(const QString &sProdCode, double dUnite, double dExchBal, int iHand, const FareInfo &fiBourse, const FareInfo &fiMember, bool bMatch = false);

	// 根据买卖标志判断是否是多仓（暂定用于延期交收和中立仓的判断）
	static bool IsLongPosition(const QString &sBuySellFlag);

	//
	static QString GetDDAMatchId(const QString &sMiddleFlag, const QString &sBuySellFlag);

	// 判断报单流水是否无效
	static bool CheckEntrFlowIsInvalidate( const QString &sState, int iRemainAmount );

	// 获取报单类型
	static QString GetOrderType(const QString &sLocalOrderNo, const QString &sStateId);

	// 计算某个品种每手收取的保证金 sProdCode（合约代码） fUnite（每手的重量） fPrice（该品种的价格） fi（该品种的费率信息）
	static double CalculateFare( const QString &sProdCode, double fUnite, double fPrice, const FareInfo &fi, int iHand = 1 );

	// 获取某个品种的一手的克数
	static double HandToGram( const QString &sProdCode, double dMeasureUnit );

	//
	static bool SetPriceControl( CScrollEditEx &editCtl, const QString &sInsID );

	static bool SetPriceControl( CScrollEditEx &editCtl, const QString &sInsID, const QUOTATION* pQT );

	// 判断一个品种是否是延期
	static bool IsDefer( const QString &sInst );
};
