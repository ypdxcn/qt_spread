#ifndef _STDEF_H
#define _STDEF_H


#include <QString>
#include <list>
#include <QRunnable>
#include "BroadcastPacketAndStruct.h"
#include <QMutex>
#include <QWaitCondition>

#include <thread>
#include <functional>
#include <mutex>

//#include<iostream>
//#include<stack>
//#include<map>
//#include<set>
//#include<functional>
//#include<string>


using namespace std;

struct VarietyInfo
{
	QString       variety_id           ; // 交割品种代码    
	QString       name                 ; // 交割品种全称            
	QString       abbr                 ; // 简称                    
	QString       variety_type         ; // 品种类别    
	double        min_pickup           ; // 最小提货数量        
	double        pickup_base          ; // 提货步长
	QString       weight_unit          ; // 重量单位        
	QString       destroy_flag         ; // 注销标志 
};

struct MyCodeInfo
{
	QString       code_type            ; //码表类型    
	QString       code_id              ; //代码编号    
	QString       code_desc            ; //代码描述  

	MyCodeInfo(){}

	MyCodeInfo( const QString &sCode_type, const QString &sCode_id,  const QString &sCode_desc) : code_type(sCode_type),code_id(sCode_id),code_desc(sCode_desc){}
};

//
struct ProdCodeInfo
{
	QString       prod_code            ; //合约代码    
	QString       prod_name            ; //合约名称    
	QString       variety_type         ; //品种类别    
	QString       variety_id           ; //品种代码    
	double       tick                 ; //最小变动价位    
	double       upper_limit_value    ; //涨停板率    
	double       lower_limit_value    ; //跌停板率    
	QString       active_flag          ; //活跃标志    
	QString       exch_unit            ; //交易单位    
	double       measure_unit         ; //计量单位    
	QString       market_id            ; //交易市场   
};

struct StoreInfo
{
	int iAllAmt;
	int iUseFulAmt;
	int iCanGetAmt;
	int iTodayIn;
	int iTodayOut;
	int iExchFreeze;
	int iTihuoFreeze;
	int iUnitCost;

	StoreInfo()
	{
		iAllAmt = 0;
		iUseFulAmt = 0;
		iCanGetAmt = 0;
		iTodayIn = 0;
		iTodayOut = 0;
		iExchFreeze = 0;
		iTihuoFreeze = 0;
		iUnitCost = 0;
	}
};

struct DeferMatchSimple
{
	QString sLocalOrderNo;
	double dPosiMoney;
	double dPrice;
	int iHand; // 剩余的手数
	int iTotalHand; // 总的手数，即成交的手数

	DeferMatchSimple()
	{
		dPosiMoney = 0.00;
		iTotalHand = 1;
	}
};

typedef list<DeferMatchSimple> LISTDeferMatch;

struct DeferPosiInfo
{
	int iCurrAllAmt;     // 总持仓
	int iYesAmt;         // 昨仓
	int iTodayAmt;       // 今仓
	int iUsefulAmt;      // 可用仓

	double fYesAvgPosPrice; // 上日持仓均价

	double dAvgPosPrice;     // 总的持仓均价
	double dAvgOpenPosPrice; // 总的开仓均价
	double dPosi_PL;         // 持仓盈亏
	double dTotalFare;       // 持仓保证金

    double dSvrAvgPosPrice;  // 服务端持仓均价
    double dSvrAvgOpenPosPrice; // 服务端开仓均价

	LISTDeferMatch listMatch; // 成交列表

	double dYesPosiMoney;
	int iYesOrgAmt;
	double dYesAvgOpenPosPrice; // 上日开仓均价

	DeferPosiInfo()
	{
		iCurrAllAmt = 0;
		iYesAmt = 0;
		iTodayAmt = 0;
		iUsefulAmt = 0;

		fYesAvgPosPrice = 0.00;
		dAvgOpenPosPrice = 0.00;
		dAvgPosPrice = 0.00;
		dPosi_PL = 0.00;
		dTotalFare = 0.00;

		iYesOrgAmt = 0;
		dYesPosiMoney = 0.00;
	}
};

struct DeferPosi
{
	QString sProdCode;
	unsigned int uiLastPrice; // 上一次用来处理盈亏的价格
	double dUnite; // 

	DeferPosiInfo infoLong;//多
	DeferPosiInfo infoShort;//空

	DeferPosi()
	{	
		uiLastPrice = 0;
		dUnite = 0.00;
	}
};

// 请求平仓试算参数
struct PCSSPara 
{
	QString csProdCode;
	QString sBS;
	QString csPrice;
	QString csHand;
};

namespace AutoMode
{

}
struct  strategy_obj
{
	QString prodID;
	QString market;//“QH”,"HJ"
	int     direction;//0：买空，1：买多
	int     flag;//0: -  ;  1:  +
	int     rate;//配比
	QUOTATION quote;
};
//套利-条件单
struct  strategy_conditionOrder
{
	QString  id;
	QString  flag;//"0":单次；"1":多次
	QString  name;//套利合约名称
	QString  status;//运行状态
	QString  direction;//买卖方向
	double   price;
	double   volume;
	double   positionPrice;
	double   positionColume;
	double   gainPrice;
	double   lossPrice;
	double   avgPrice;
	double   profitloss;
	QString  stop;   //停止方式"0"：手工；“1”：手动
	QString  proInfo;//组合信息
	int      period; //组合阶段
	QString  express;//价格公式，计算表达式  如:T1-T2
	strategy_obj  obj[2];//0:T1合约   ；  1：t2合约
	strategy_conditionOrder operator = (const strategy_conditionOrder& sCondOrder)
	{
		this->id = sCondOrder.id;
		this->flag = sCondOrder.flag;//"0":单次；"1":多次
		this->name = sCondOrder.name;//套利合约名称
		this->status = sCondOrder.status;//运行状态
		this->direction = sCondOrder.direction;//买卖方向
		this->price = sCondOrder.price;
		this->volume = sCondOrder.volume;
		this->positionPrice = sCondOrder.positionPrice;
		this->positionColume = sCondOrder.positionColume;
		this->gainPrice = sCondOrder.gainPrice;
		this->lossPrice = sCondOrder.lossPrice;
		this->avgPrice = sCondOrder.avgPrice;
		this->profitloss = sCondOrder.profitloss;
		this->stop = sCondOrder.stop;   //停止方式"0"：手工；“1”：手动
		this->proInfo = sCondOrder.proInfo;//组合信息
		this->period = sCondOrder.period; //组合阶段

		for (int i = 0; i < 2; i++)
		{
			obj[i].direction = sCondOrder.obj[i].direction;
			obj[i].flag = sCondOrder.obj[i].flag;
			obj[i].prodID = sCondOrder.obj[i].prodID;
			obj[i].rate = sCondOrder.obj[i].rate;
			obj[i].quote.m_uiLast = sCondOrder.obj[i].quote.m_uiLast;
			obj[i].quote.m_Bid[0].m_uiPrice = sCondOrder.obj[i].quote.m_Bid[0].m_uiPrice;
			obj[i].quote.m_Ask[0].m_uiPrice = sCondOrder.obj[i].quote.m_Ask[0].m_uiPrice;
		}
		

		return  *this;
	}
};









#endif