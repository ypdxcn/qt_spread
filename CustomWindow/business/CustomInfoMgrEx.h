#pragma once

#include "StDef.h"
#include <map>
#include <unordered_map>
#include "TradePacketAndStruct.h"
struct QUOTATION;
struct FundInfoS;
struct FundInfo;

class CustomInfoMgrEx
{
public:
	CustomInfoMgrEx(void);
public:
	~CustomInfoMgrEx(void);

	// 根据应答报文刷新内存信息
	void UpdateCustomInfo( const Rsp1020 &stRsp, const Req1020& req1020 );

	// 处理接收一笔行情
	void HandleRecvQuotation( const QUOTATION& stQuotation );

	// 获取总的持仓盈亏
	double GetTotalPosiSurplus();

	// 获取持仓信息
	unordered_map<string, DeferPosi> GetPosi();

	// 获取库存信息
	unordered_map<string, StoreInfo> GetStore();

	// 获取资金信息
	FundInfoS GetFundInfoS();

	// 获取资金信息
	FundInfo GetFundInfo();

	// 获取客户名称
	string GetCustomName();

	// 获取可用余额
	double GetUsefullBal();

	// 获取可用库存
	int GetUsefulStore( const string& sInst );

	// 获取可用持仓
	int GetUsefulPosi( const string& sInst, bool bLong );

	// 获取一个品种的持仓信息
	bool GetPosiInfo( DeferPosi& stPosi, const string& sInst );

	Rsp1020 Get1020();

	//依托本地的大边信息计算冻结保证金
	double  CalculateFrozenCapital(const QString & ProdCode, const QString & ExchId  , double  orderCapital);
protected:
	// 从报文中获取持仓信息到内存
	void RefreshPosi( const HashtableMsg &hsmPosi );

	// 从报文中获取库存信息到内存
	void RefreshStore( const HashtableMsg &htmStore );

	// 刷新资金信息
	void RefreshFundInfo( const Rsp1020 &stRsp );

	// 获取持仓盈亏
	double GetPosiSurplus( UINT uiLast, double dPosiAve, int iTotalHand, double dUnite, bool bLong );

	// 计算持仓盈亏
	void CalculateSurplus( DeferPosi& stPosiInfo, UINT uiLast );

protected:
	Rsp1020 m_rsp1020; // 保存一份原始数据
	//map<string, DeferPosi>   m_mapDeferPosi; // 持仓信息
	//map<string, StoreInfo>   m_mapStoreInfo; // 库存信息

	unordered_map<string, DeferPosi>   m_mapDeferPosi; // 持仓信息
	unordered_map<string, StoreInfo>   m_mapStoreInfo; // 库存信息

	CGessMutex	 m_csMutex;
};

extern CustomInfoMgrEx g_CusInfo;
