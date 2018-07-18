#pragma once

class CModifySetting
{
public:
	CModifySetting(void);
public:
	~CModifySetting(void);

	// 报单前确认
    static void ModifyTradeConfirm(bool bConfirm);

	// 撤单前确认
	static void ModifyCancelConfirm(bool bConfirm);

	// 平仓前确认
	static void ModifyOppCovConfirm(bool bConfirm);

	// 反手前确认
	static void ModifyCovPosiConfirm(bool bConfirm);

	// temp
	static void ModifyTipsAskSus(bool bTips);

	// temp
	static void ModifyTipsAskFail(bool bTips);

	// temp
	static void ModifyTipsOrderMatch(bool bTips);

	// temp
	static void ModifyTipsCancelSus(bool bTips);

	// temp
	static void ModifyTipsCancelFail(bool bTips);

	static void ModifyTipsPreOrder(bool bTips);

	static void ModifyTipsAskOverFlow(bool bTips);

	static void ModifyTipsAskPreOrder(bool bTips);
};
