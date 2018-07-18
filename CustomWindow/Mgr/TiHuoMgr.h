#pragma once

#include "TradePacketAndStruct.h"
#include <map>
#include <string>
#include <QString>
#include <QMap>

#define g_TiHuoMgr CTiHuoMgr::GetInstance()

class CTiHuoMgr
{
private:
	CTiHuoMgr(void);
	~CTiHuoMgr(void);

	CTiHuoMgr( const CTiHuoMgr&) ;            // 不实现.
	CTiHuoMgr& operator= ( const CTiHuoMgr& ); // 不实现.

public:
	// 提货冻结的结构
	struct FrozeInfo
	{
		string    sInsID;
		quint32   nWeight;
	};

public:
	static CTiHuoMgr& GetInstance();

	// 初始化函数，软件初始化时调用
	void Initialize( );

	// 处理用户主动查询
	void HandleUserQuery( Rsp5101 &rsp5101 );

	// 处理当前的提货交易
	void HandleTodayTrade();

	// 处理用户撤单
	void HandleCancel( const string &sLocalSheetNo );

	// 查询提货信息，提供给外部公用
	bool Query( Rsp5101 &rsp, const string &sBegin, const string &sEnd, const string &sLocalSheetNo );

	// 记录当日提货保证金冻结
	void SetDay_take_margin( const string &sDay_take_margin );

protected:
	// 查询所有，现在只设计为初始化时调用
	void QueryAll();

	// 处理提货查询结果, mod by Jerry Lee, 2013-3-25
	void HandleQueryResult( Rsp5101 &rsp5101, bool bCancel = true/*是否处理撤销*/ );

	// 刷新提货保证金冻结
	void RefreshTakeFroze();

	/*
	处理库存的冻结或者解冻，由HandleQueryResult调用
	sOrderNo 提货单号
	sInsID 提货品种
	nWeight 处理重量
	bFroze 是否冻结，否则为解冻
	*/
	bool HandleCalculate( const string &sOrderNo, const string sInsID, int nWeight, bool bFroze );

private:
	QMap<string, FrozeInfo> m_mapFroze; // 记录提货的冻结信息

	string m_sDay_take_margin; // 当日提货保证金冻结
};
