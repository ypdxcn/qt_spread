#ifndef _HJDEF_H
#define _HJDEF_H

#include  <QString>

#define LOG_CONNECT_SERVER_FAIL_TIPS   "连接服务器失败！"

#define CONSTANT_YES								 "1"
#define CONSTANT_NO									 "0"
#define CONSTANT_TIHUO_SUS							"1"
#define CONSTANT_TIHUO_CANCEL_TODAY					"3"
#define CONSTANT_TIHUO_CANCEL_ANOTHER_DAY           "4"

#define CONSTANT_STOP_PL_SER_TIPS "注意：止盈止损单将发往服务器，由服务器触发！\r\n 【追踪止损调整价位】字段将无效！"
#define CONSTANT_STOP_PL_CLIENT_TIPS "注意：止盈止损单将保存在本机，由本机触发！"



enum MarketType
{
	e_mk_Spot,
	e_mk_Forward,
	e_mk_Defer,
	e_mk_Others,
};

typedef enum
{
	E_LOgin_Type_NUll = 0,
	E_Login_Type_Trade  = 1, // 交易
	E_TYPE3,
	E_TYPE4,
	E_Login_Type_HQ_Trade = 4, // 行情加交易
} ELoginType;


const QString g_sEmptyStr("");

extern QString GetYesNoName(const QString &sID);

/*
是否是延期交收或者中立仓的交易方式
*/
extern bool IsDeferSpecial(const QString &sExchTypeID);

extern MarketType GetMarketFromExchType( const QString &sExchTypeID );

extern MarketType GetMarketType( const QString &sMarketID );

extern bool IsDeferInst( const QString &sInst );

#endif