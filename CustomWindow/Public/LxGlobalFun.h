#pragma once
#include "TraderManager.h"
#include "ViewCommDataDef.h"
#include <string>
#include <vector>
using namespace std;

enum BussType;

class CLxGlobalFun
{
public:
	// 初始化合约代码
	static void IniProdCode();
	// 获取合约代码对应的id
	static string GetProdCode();
	static string GetProdCodeID(const string &strName);
	static string GetProdCodeName(const string &strID);
	// 初始化交易类型
	static void IniExchCode();
	// 获取交易类型id
	static string GetExchCode();
	static string GetCurrency(TRADE_MONEY_KIND type);
	static string GetCurrency(const string & type);
	static string GetHedgeFlag(POSI_HF_PLAG type);
	static string GetTodayOrYestoday(POSI_DATE m_nPosiDate);
	static wstring CLxGlobalFun::GetPageBussType(BussType  bussType);
	static string GetStatusString(ACCOUNT_STATUS status);
	// 初始化开平标志
	static void IniOffsetFlag();
	// 获取开平标志id
	static string GetOffsetFlag(TRADE_OPEN_CLOSE flag);
	// 初始化持仓方向
	static void IniLongShort();
	// 获取持仓方向id
	static string GetLongShort();
	// 初始化存取方向下拉菜单
	static void IniAccessWay( bool bAddEmpty = true);
	// 获取存取方向id
	static string GetAccessWay();
	static string GetAccessWayID();
	// 初始化交割品种代码
	static void IniVarietyId();
	// 获取交割品种代码ID
	static string GetVarietyId();

    // 功能：往vector(v)添加一个标题为strtitle值为strvalue的pare(p)
	static void PairAdd( vector< pair<string,string> > & v, string strtitle, string strvalue );
	// 根据交易市场ID获取交易市场名称
	static string GetMarketName(const string &strID);
	// 根据交易类型ID获取交易类型名称
	static string GetTradeTypeName(TRADE_TYPE type);
	// 获取报单状态类型名称
	static string GetOrderStatus(const int &status);
	// 获取报单提交状态
	static string GetSubmitOrderStatus(const int &status);
	static string FormatPrice(const double &price);
	//数值转字符
	static string StrFromIntVolume(const int &vol);
	static string StrFromDoubleVolume(const double &vol);
	
	// 根据开平标志ID获取开平标志名称
	static string GetOffset_flagName(const string &strID);
	// 根据交收标志ID获取交收标志名称
	static string GetDeli_flagName(const string &strID);
	// 根据买卖方向ID获取买卖方向名称
	static string GetBuySellName(TRADE_DIRECTION dirct);
	// 根据委托状态ID获取委托状态名称
	static string GetEntr_statName(const string &strID);
	// 根据撤单标志ID获取撤单标志名称
	static string GetCancel_flagName(const string &strID);
	// 根据操作方向ID获取操作方向名称
	static string GetAccessWayName(const string &strID);
	// 根据复核状态ID获取复核状态名称
	static string GetCheck_staName(const string &strID);
	// 根据转账是否成功ID获取转账是否成功名称
	static string GetIn_account_flagName(const string &strID);
	// 根据持仓方向ID获取持仓方向名称
	static string GetLongShortName(const string &strID);
	// 根据渠道ID获取渠道名称（包括委托渠道,撤销渠道,操作来源）
	static string GetTerm_typeName(const string &strID);
	// 刷新信息时根据代理机构id获取代理机构名称
	static string GetBranch(const string &strID); 
	// 根据品种ID获取品种名称
	static string GetGetVarietyName(const string &strID);
	// 根据提货状态ID获取提货状态名称
	static string GetTake_statName(const string &strID);
	// 根据提货类型ID获取提货类型名称
	static string GetDraw_typeName(const string &strID);
	// 根据证件类型ID获取证件类型名称
	static string GetCert_typeName(const string &strID);
   
	// 刷新信息时根据账户状态id获取账户状态名称
	static string GetAcct_stat(const string &strID);     
    
	// 判断一个字段ID是否应该格式化为浮点数
	static bool IsFloatTitle(string strID);
	static int StringXiaoAdd1(string &strFloat);
    // 格式化浮点数
	static string FormatFloat(string strFloat, bool bQuotation = false);
	// 检测要格式化的浮点数是否合法
	static bool CheckFloat(string &sFloat);

	//处理时间  int64变为串
	static  string StrFromInt64(__int64 ntime);

	static wstring Str2Wstr(const char * szData);
	static string t2sA(__int64 ntime);
	static std::string Get16HexRandNumber(const int &iCount);
	static UINT64 HexStrToUINT64(const string &sHexStr);

	//////////////////////////////////////////////////////////////////////////
	//交易逻辑处理
	//////////////////////////////////////////////////////////////////////////
	//滑点计算
	static void CalcSlipPointPrice(double & dPrice, double dSlipPoint, DIRECTION_TRADETYPE direction);
	//行情最新价是否优于下单价
	static bool IsSuperiorPrice(double dOrderPrice, double dQuotePrice, DIRECTION_TRADETYPE direction);
	//止损止盈滑点计算(bStopLoss:TRUE为止损,FALSE为止盈)
	static void CalcSlipPointPriceForStopLossandProfit(double & dPrice, double dSlipPoint, DIRECTION_TRADETYPE direction, BOOL bStopLoss/*=FALSE*/);
	//获取对手价
	static int GetOpponentPrice(DIRECTION_TRADETYPE direction, double & dPrice, double dSellPrice, double dBuyPrice);
	//优化调整报单价
	static void CalcMarketPrice(double & dPrice, TRADEPARSEUNIT &tpu);
	//止损止盈逻辑判断
	static int JudgeStopLossandProfit(TRADEPARSEUNIT &tpu, TRADEPRICE &tp, BOOL bSimulate);
	//移动止损止盈逻辑判断
	//参数:tpu 下单条件
	//	   tp 传出报单价和成交价(成交价模拟时需要使用)
	//	   bSimulate 是否是模拟交易(TRUE-是, FALSE-否)
	//返回值:
	//		0	成功,仅设置止盈止损传出报单价或成交价
	//		1	成功,设置止盈止损且本地预埋价,判定通过,传出报单价
	//		2	失败,指定价条件未达到
	static int JudgeMoveStopLossandProfit(TRADEPARSEUNIT &tpu, TRADEPRICE &tp, BOOL bSimulate);

    //////////////////////////////////////////////////////////////////////////
	//模拟交易类
	//////////////////////////////////////////////////////////////////////////
	//模拟平仓判定接口
	static int  simu_CloseParse(TRADEPARSEUNIT &tpu, TRADEPRICE &tp);
	//模拟开仓判定接口
	static int  simu_OpenParse(TRADEPARSEUNIT &tpu, TRADEPRICE & tp);
	//下单逻辑及撮合机制
	static int  logic_MatchEngine(TRADEPARSEUNIT &tpu, TRADEPRICE &tp);


private:
	//kenny  拆分的子函数  
	static int NTCheckPrice(TRADEPARSEUNIT &tpu, double *l_dReportPrice, double *l_dBidPrice);

};






