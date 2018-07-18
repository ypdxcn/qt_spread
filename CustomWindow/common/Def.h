#ifndef _DEF_H
#define _DEF_H

enum EReqType
{
	e_ReqType_FundPosiStore,
	e_ReqType_CustomInfo, //
};

// 提示的类型
enum ETipsShowType
{
	e_TipsShowType_always,
	e_TipsShowType_Suc,
	e_TipsShowType_Fail,
	e_TipsShowType_no,
};

// 买卖类型
enum BSFLAG
{
	BS_UKNOWN = 0,
	BS_BUY ,
	BS_SELL,
};

// 开平
enum KPFLAG
{
	KPF_UKNOWN = 0,  // 未知
	KPF_USELESS,     // 开平没有启用，即现货
	KPF_KAI,         // 开仓
	KPF_PING,        // 平仓
	KPF_PT,          // 平今
	KPF_PY,          // 平昨
	KPF_FP,          // 强平
	KPF_DDA,         // 交收
	KPF_MA,          // 中立仓
};

typedef enum
{
	E_Tips2_Asksus,
	E_Tips2_AskFail,
	E_Tips2_OrderMatch,
	E_Tips2_CancelSus,
	E_Tips2_CancelFail,
	E_Tips2_CreatePreOrder,
} TipsType;


#endif