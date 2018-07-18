/*
止盈止损信息的管理类
管理止盈止损单从配置文件的加载以及成交时生成止盈止损单

止盈止损基本公式
行情价格 比较方式 比较价格 时发出
行情价格：最新价、买一价或者卖一价
比较方式：大于或等于、小于或等于
比较价格（CmpPrice）：基础比较价格（BaseCmpPrice）加上或者减去调整价位，比如在开多仓时设置了“每笔开仓价调整3个价位时发出”，则
          这时候基础比较价格为“每笔开仓价”，止损单的比较价格 = 每笔开仓价 - 3*价位

*/
#pragma once


#include "PreOrder/PreOrderDef.h"
#include "BroadcastPacketAndStruct.h"
#include <map>
#include <QVector>
#include <QString>

#define g_StopPLMgr CStopPLMgr::GetInstance()

class CStopPLMgr
{
private:
	CStopPLMgr(void);
	~CStopPLMgr(void);

	CStopPLMgr( const CStopPLMgr&) ;            // 不实现.
	CStopPLMgr& operator= ( const CStopPLMgr& ); // 不实现.

public:
	static CStopPLMgr& GetInstance();

	// 初始化止损止盈配置信息
	void Initialize( );

	// 加载用户止盈止损配置
	void LoadSetting();

	// 加载已处理成交单
	void LoadHandledMatchNo();

	// 处理接受到一笔延期成交单
	void HandleOpenPosiMatch( const DeferMatch &stMatch, bool bLong );

	EPLCmpType GetCmpType();
	EPLAskPriceType GetAskPriceType();
	short GetAskPriceAjust();
	bool IsServer();
	const map<string, PLInfo>* GetInfo();

private:
	// 获取一个品种的止损止盈配置信息
	PLInfo* GetPLInfo( const string &sInsID );

	// 获得基础比较价格
	bool GetBaseCmpPrice( double &dBaseCmpPrice, EPLCmpType eCmpType, const DeferMatch &stMatch, bool bLong );

	// 添加一笔预埋单
	int AddPreOrder( PreOrderStruct &stPreOrder );

	// 设置止损单公用函数
	void SetStopLoss( PreOrderStruct &stPreOrder, double dCmpPrice, double fPriceUnite, bool bLong, short nLossAjust );

	// 根据报单价格类型获取行情中对应字段的指针
	const unsigned int* GetHQPricePointer( EQTType eAskPriceType, const string &sInsID );

	// 获得报单时的报价类型，当前主要是预埋单对话框显示报价字段时要用到
	EQTType GetPreOrderAskPriceType( EPLAskPriceType eAskPriceType, bool bLong );

	//
	short GetLossAjust( const PLInfo *plInfo );

	// 根据止损调整价位获取止损价格
	double GetLossPrice( short Ajust, double dCmpBasePrice, bool bLong, double fPriceUnite );

	//
	bool IsHandled( const QString &sMatchNo );

	// 将已处理的成交流水写入到配置文件
	bool WriteMatchNo();

	void AddMatchNo( const QString &sMatchNo );

private:
	map<string, PLInfo> m_mapInfo; // 记录止盈止损配置信息
	EPLCmpType m_eCmpType;    // 比较（触发）价格类型，可能为每笔开仓价、持仓均价
	EPLAskPriceType m_eAskPriceType; // 平仓报单价格类型，可能为最新价、买吗价
	short m_iAskPriceAjust; // 报单价格调整价位
	bool m_bUseServer;

	QVector<QString> m_vecHandledMatchNo; // 记录已经发送过止盈止损单的成交单
	map<QString, QString> m_mapHandledMatchNo; // 记录已经发送过止盈止损单的成交单
	bool m_bMatchNoChange; // 标记m_vecHandledMatchNo从配置文件加载后是否变化
};
