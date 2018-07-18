/*
一般来说返回值有下面几种
0 表示连接服务器成功 返回-1则表示连接服务器失败  1表示连接服务器成功，但是获取应答报文有问题
*/
#pragma once
#include "TradePacketAndStruct.h"
#include <QMessageBox>
class CTranMessage
{
public:
	CTranMessage(void);
public:
	~CTranMessage(void);

	static bool m_bUseSystemTips;

	static void DealShowTips(int iReturn, const QString &sCode, QString &sMsg, bool bUseSystemTips = true);

	static int Handle2040( Rsp2040 &rsp2040 );

	static int Handle6002(Rsp6002 &stRsp6002, const QString &sQueryTag, const ArrayListMsg &almViewField, QVector< pair<QString,QString> > & vecPara, int iCountPerPage, int iCurPage = 1 );

	static int Handle4001(Rsp4001 &rsp4001, const QString &sProdCode, double dPrice, int iAmount, const QString &sExchID, int orderType,bool bUseSystemTips = true);

	static int Handle4071( Rsp4071 &rsp4071, const QString &sProdCode, const QString &sType, const QString &sPrice, int iAmount );

	static int Handle1005( Rsp1005 &rsp1005 );

	static int Handle9030( Rsp9030 &rsp9030, const QString &sParaID );

	static int Handle3099( Rsp3099 &rsp3099, bool bUseSystemTips = true);

	static int Handle3064( Rsp3064 &rsp3064, bool bUseSystemTips = true);

	static int Handle1020( Rsp1020 &rsp1020, char cCusInfo, char cFund, char cStorage, char cForward, char cDefer, char cSurplus, bool bUseSystemTips);

	// 出入金相关
	static int Handle3101( Rsp3101 &rsp3101, int iOper_flag, const QString &sAccess_way = "", double dExch_bal = 0, char *pcFund_pwd = NULL, bool bUseSystemTips = true);

	// 出入金相关
	static int Handle3021( Rsp3021 &rsp3021, int iOper_flag, const QString &sPsw, const QString &sAccess_way = "", double dExch_bal = 0,  bool bFundPsw = true, bool bUseSystemTips = true);

	static int Handle4061( Rsp4061 &rsp4061, const QString &sOrderNo );

	static int GetResult( const QString& sRspCode );
};
