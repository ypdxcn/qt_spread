#pragma once

#include "Def.h"
#include "TradePacketAndStruct.h"

class CTradeHandler
{
public:
	CTradeHandler(void);
public:
	~CTradeHandler(void);

	// 获取客户信息
	int GetCustomInfo( Rsp1020 &rsp1020, Req1020& req1020, ETipsShowType eType, bool bBackground = false );

	// 获取出入金流水
	int GetCushAccessFlow( Rsp3010 &rsp3010, Req3010& req3010, ETipsShowType eType, bool bBackground = false );

protected:
	// 判断一笔应答是否成功
	inline bool IsTradeSus( const string &sRspID );

	/*
	判断一个请求是否成功
	*/
	inline int IsSuccess( int nRet, const string &sRspCode );

	/*
	判断是否需要提示
	*/
	bool NeedShowTips( int nRet, ETipsShowType eType );
};

extern CTradeHandler g_TradeHandler;
