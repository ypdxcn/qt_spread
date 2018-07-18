#pragma once

#include "DelayReqBase.h"

class CDelayReqHandler : public CReqHandlerBase
{
public:
	CDelayReqHandler(void);
public:
	~CDelayReqHandler(void);

	// 外部调用该函数来启动线程
	bool Initlize( const string& sDelayTime );

	// 请求客户基础信息
	void AddCustomInfoReq();

	
public: // 广播处理---------------------------------------------------

	// 处理接收现货报单
	void HandleRecSpotOrder( BSFLAG eBSFlag );

	// 处理接收延期报单
	void HandleRecDeferOrder( KPFLAG eKPFlag );

	// 处理接收交收报单
	void HandleRecDDAOrder( void );

	// 处理接收中立仓报单
	void HandleRecMAOrder( void );


	// 处理接收现货撤单
	void HandleRevSpotOrderCancel( BSFLAG eBSFlag );

	// 处理接收延期撤单
	void HandleRevDeferOrderCancel( KPFLAG eKPFlag );

	// 处理接收交收撤单
	void HandleRevDDAOrderCancel( void );

	// 处理接收中立仓撤单
	void HandleRevMAOrderCancel( void );


	// 处理现货成交
	void HandleSpotMatch( void );

	// 处理延期成交
	void HandleDeferMatch( void );

	// 处理交收中立仓成交
	void HandleDDA_MAMatch( void );

	//重新请求刷新资金信息
	void RequestFundInfo();
protected:
	/*
	重写父类函数，发送所有的请求
	*/
	void SendReq( const deque<ReqPara> &deqReq );

	/*
	重写父类函数，判断一个请求是否添加
	*/
	bool IsUseless( ReqPara &stReqInQue, const ReqPara &stReqAdd );

	/*
	*/
	void AddReqCommon();

	/*
	*/
	void AddCustomInfoReq( bool bFund, bool bPosi, bool bStore, bool bCusInfo = false, bool bSurplus = false );

	const string GetReqPara( bool bReq );

private:
	bool m_bRspToBroadcast; // 接收交易类广播是否刷新
};

extern CDelayReqHandler g_DelayReqHandler;
