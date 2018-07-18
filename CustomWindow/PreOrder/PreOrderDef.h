#ifndef PREORDERDEF_H
#define PREORDERDEF_H

#include <string>
#include <QString>
#include "osdepend.h"

using namespace std;

typedef enum
{
	PreOrderMsgFlag_Add = 0,   // 添加   
	PreOrderMsgFlag_Mdf_State, // 修改状态
	PreOrderMsgFlag_Mdf_Con,   // 修改触发条件
	PreOrderMsgFlag_Del,       // 删除
	PreOrderMsgFlag_Clear,       // 清空
	PreOrderMsgFlag_Mdf_Hand,  // 修改手数
}PreOrderMsgFlag;

typedef enum
{
	E_NoSend = 0, // 未发送    
	E_Sended,     // 已发送
	E_SendFailed, // 发送失败 
} EPreOrderState;

typedef enum
{
	PreOrderType_Condition = 0, // 条件单，满足条件时发送
	PreOrderType_Loss,      // 止损单
	PreOrderType_Profit,    // 止盈单
	PreOrderType_Hand,  // 手动发送
	PreOrderType_AUTO,      // 自动报单，即进入交易时段后发送
} EPreOrderType;

/*
行情价格与报价的比较类型，例如E_Small，即当行情价格小于报价时触发

*/
typedef enum
{
	E_Big_Equal=0,       // 大于或等于
	E_Small_Equal,     // 小于或等于
	E_Equal,           // 等于
	E_Small,      // 小于
	E_Big,             // 大于
} ECompareType; 

/*
与服务端保持一致
*/
typedef enum
{
	E_Buy1 = 0, // 买1
	E_Sell1, // 卖1
	E_Latest, // 最新价
}EQTType;

// 止盈止损单参数基类
class PLParam
{
public:
	const unsigned int *pPrice; // 止盈止损单报单价格指向行情的字段（可能为最新价、买1价或者卖1价）
	int iAjust;                 // 偏移行情价格的尺度
	bool bLong;                 // 是否多仓
	string sMatchNo;            // 当前止盈止损单对应的成交单号

	PLParam( const unsigned int *pPrice2, int iAjust2, bool bLong2, const string &sMatchNo2 ) : pPrice(pPrice2), iAjust(iAjust2), bLong(bLong2), sMatchNo(sMatchNo2) {};

	PLParam()
	{
		pPrice = NULL;
	}
	virtual ~PLParam() {};
};

// 止损单的参数
class LossParam : public PLParam
{
public:
	unsigned int uiExtreme; // 极值，初始化为发出条件单时的最新价（自该笔动态止损单开始的最新价的极值，多仓即为最大，空仓则为最小）
	bool bUseDys;           // 当前极值采用的是否是动态触发价
	uint uiStatic;          // 静态止损触发价
	uint uiDys;             // 动态止损触发价

	LossParam( const unsigned int *pPrice2, int iAjust2, bool bLong2, const string &sMatchNo2 ) : PLParam(pPrice2, iAjust2, bLong2, sMatchNo2){};
	LossParam()
	{
		pPrice = NULL;
	}
	~LossParam() {};
};

// 预埋单结构
struct PreOrderStruct
{
	int iID; // 预埋单ID
	EPreOrderType  eOrderType;  // 预埋单类型
	EPreOrderState eOrderState; // 预埋单状态
	string sProdCode; // 品种
	string sExchId;   // 交易类型
	int iHand;        // 手数
	string sTime;     // 录入时间

	double dPrice;    // 报单价格，止盈止损单无效（因为止盈止损单报单价格采用的是行情价加调整价位），其他有效

	/*
	下面3个字段对条件单以及止盈止损单有效
	*/
	ECompareType eCompType; // 比较的原则是：行情价格 VS 比较价格
	EQTType eQTType;        // 与行情哪个数据比较（最新价、买一、卖一）
	double dCmpPrice;       // 触发价格

	/*
	采用的行情字段类型（最新价、买卖1价）（用于报单）
	本来是止盈止损单才要用的字段，不过可能以后要用到其他类型单里面，所以在这里声明了
	*/
	EQTType eAskPriceType;

	bool bServer;

	void* wParam;

	PreOrderStruct()
	{
		eOrderState = E_NoSend;
		wParam = NULL;
		bServer = false;
		iID = -1;
	}

	bool IsUserDefPrice()
	{
		if( eOrderType == PreOrderType_Loss || eOrderType == PreOrderType_Profit )
			return true;
		else
			return false;
	}

	PreOrderStruct& operator = (const PreOrderStruct& Other)
	{
		Release();

		Copy(Other);
		
		return *this;
	}

	PreOrderStruct(const PreOrderStruct& Other)
	{
		Copy(Other);
	}

	void Copy( const PreOrderStruct& Other )
	{
		this->iID = Other.iID;
		this->eOrderType = Other.eOrderType;
		this->eOrderState = Other.eOrderState;
		this->sProdCode = Other.sProdCode;
		this->sExchId = Other.sExchId;
		this->dPrice = Other.dPrice;
		this->eAskPriceType = Other.eAskPriceType;
		this->iHand = Other.iHand;
		this->sTime = Other.sTime;
		this->eCompType = Other.eCompType;
		this->eQTType = Other.eQTType;
		this->dCmpPrice = Other.dCmpPrice;
		
		if( eOrderType == PreOrderType_Profit )
		{
			PLParam *pParam = new PLParam();
			*pParam = *((LossParam*)Other.wParam);
			wParam = pParam;
		}
		else if( eOrderType == PreOrderType_Loss )
		{
			LossParam *pParam = new LossParam();
			*pParam = *((LossParam*)Other.wParam);
			wParam = pParam;
		}
		else
		{
			this->wParam = Other.wParam;
		}
	}

	void Release()
	{
		if( wParam != NULL )
		{
			if( eOrderType == PreOrderType_Profit )
			{
				PLParam *pPLParam = (PLParam*)wParam;
				if( pPLParam != NULL )
					delete pPLParam;
			}
			else if( eOrderType == PreOrderType_Loss )
			{
				LossParam *pPLParam = (LossParam*)wParam;
				if( pPLParam != NULL )
					delete pPLParam;
			}

			wParam = NULL;
		}
	}

	~PreOrderStruct()
	{
		Release();
	}
};

// 止盈止损单比较价格类型
typedef enum
{
	E_PLMatchPrice, // 每笔开仓价
	E_PLPosiAvg,    // 持仓均价	
}EPLCmpType;

// 平仓报单价类型
typedef enum
{
	e_PLLatestPrice, // 最新价
	e_BuySellPrice, // 买卖价
}EPLAskPriceType;

// 一个品种的止盈止损的结构
struct PLInfo
{
	//string sInsID;     // 合约代码
	short iStopProfitAjust; // 止盈调整价位
	short iStopLossAjust;   // 止损调整价位
	short iDysAjust;        // 追踪止盈止损调整价位
	//EPLCmpType eCmpType;    // 比较（触发）价格类型
	//EPLAskPriceType eAskPriceType; // 平仓报单价格类型
	//short iAskPriceAjust;
};

struct InstPLInfo
{
	QString csInsID;
	QString csStopProfitAjust;
	QString csStopLossAjust;
	QString csDysAjust;

	InstPLInfo(){};

	InstPLInfo( const QString&csInsID2, const QString&csStopProfitAjust2, 
		const QString&csStopLossAjust2,const QString&csDysAjust2) :
	    csInsID(csInsID2), csStopProfitAjust(csStopProfitAjust2), 
		csStopLossAjust(csStopLossAjust2),csDysAjust(csDysAjust2){};;
};

#endif
