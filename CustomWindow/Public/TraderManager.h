#pragma once

#include <Windows.h>
#include "../lxTraderCTP/DllTradeInterface.h"
#include "../lxTraderCTP/base/ObjectPool.h"
#include <map>
#include <mutex>
#include <algorithm>
#include<ctime>

//只读数据接口 用于数据接收者
struct IEvData:public Countor
{
	virtual const eventData* ReadData() = 0;
	virtual int GetValInt(int index, const char* valname) = 0;
	virtual unsigned int GetValUInt(int index,const char* valname) = 0;
	virtual double GetValDouble(int index, const char* valname) = 0;
	virtual string GetValString(int index, const char* valname) = 0;
};
using EvDataHolder = AutoHolder<IEvData>;

//读写数据接口 用于数据维护者
struct IEvDataReadWrite : public IEvData
{
	virtual eventData* WriteData() = 0;
	virtual int SetValInt(int index, const char* valname, int val) = 0;
	virtual int SetValUInt(int index, const char* valname, unsigned int val) = 0;
	virtual int SetValDouble(int index, const char* valname, double val) = 0;
	virtual int SetValString(int index, const char* valname, const char* val) = 0;
};


using EvDataReadWriteHolder = AutoHolder<IEvDataReadWrite>;
//
struct ITraderHook
{
	//发送订单
	virtual int AfterSendOrder(const string& accName, string strategy, orderReq& req) = 0;
	//取消订单
	virtual int AfterCancelOrder(const string& accName, const string& orderNum) = 0;
	//修改订单
	virtual int AfterModifyOrder(const string& accName, orderModifyReq& req) = 0;

	virtual void OnSessionStatusChanged(const string& accName, __int64 cbUser, ACCOUNT_STATUS status) = 0;
	virtual void OnDataUpdate(const string& accName, EvDataReadWriteHolder data) = 0;
	virtual void OnMessage(const string& accName, MSG_TYPE mt, int num, const char* msg)=0;
};
using TraderHookMap = map<string, ITraderHook*>;

struct ITraderHandler
{
	virtual void OnSessionStatusChanged(const string& accName, __int64 cbUser, ACCOUNT_STATUS status) = 0;
	virtual void OnDataUpdate(const string& accName, EvDataHolder data) = 0;
	virtual void OnMessage(const string& accName, MSG_TYPE mt, int num, const char* msg) = 0;
};

//虚拟交易回调接口，根据状态去取数据
//暂时取AS_NORMAL和AS_LOGIN
struct IVTraderHandler
{
	//更新登录状态
	virtual void OnSessionStatusChanged(const string& accName,ACCOUNT_STATUS status) = 0;
	//更新交互状态
	virtual void OnMessage(const string& accName, MSG_TYPE mt, int num, const char* msg) = 0;
};

using TraderHandlerMap = map<string, ITraderHandler*>;
using TraderVHandlerMap = map<string, IVTraderHandler*>;
using AccountNameMap = map<string, int>;




////虚拟交易状态
//typedef void(*eventVSessionStatusChanged)(int sessId, __int64 cbUser, ACCOUNT_STATUS status);
//typedef struct __eventVHandler
//{
//	eventVSessionStatusChanged lpeventSessionStatusChanged;
//
//}eventVHandler;



struct SnapPrice
{

};
struct orderReqEx :public orderReq
{
	SnapPrice price;
};

class CTraderManager :public Singleton<CTraderManager>
{

	
public:
	CTraderManager();
	virtual ~CTraderManager();
	//注册与反注册Hook
	bool RegHook(string hookname,ITraderHook * hook);
	bool UnRegHook(string hookname);

	//注册与反注册Handler
	bool RegHandler(string handlername,ITraderHandler * hook);
	bool UnRegHandler(string handlername);

	//注册虚拟交易接口句柄
	bool RegVHandler(string handlername, IVTraderHandler * hook);
	bool UnRegVHandler(string handlername);

	int LoadTraderDll(string dllname);
	//创建账号连接会话
	int CreateSession(string dllname,string accName,AccountInfo& account, __int64 cbUser);
	int SessionIDList(vector<string>& sessidlist);
	int SessionStatus(const string& accName);
	int SessionOpen(const string& accName);
	int SessionClose(const string& accName);
	int SessionDel(const string& accName);
	int SessionAccountInfo(const string& accName, AccountInfo& account);
	
	int GetSessionID(const string& accName);
	string GetSessionName(int sessId);
	int SessionNameList(vector<string>& SessionNameList);

	int Recyle(eventData* data);

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////增加虚拟交易所的/////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	bool  IsVirtualExchange(const string& accName);
	int   VirtualSessionNameList(vector<string>& SessionNameList);
	int   VitualSessionIDList(vector<string>& sessidlist);
	int   VitualSessionID(const string &accName);
	int   VirtualSessionStatus(const string& accName);
	string VirtualSessionName(int& sessId);

	int   VirtualSessionOpen(const string& accName);//建立连接
	int   VirtualSessionClose(const string& accName);//断开连接
	int   VirtualSessionDel(const string& accName);

	int   VirtualSessionAccountInfo(const string& accName, AccountInfo& account);
	int   VirtualSendOrder(string accName, string strategy, orderReq& req);

	//生成模拟交易信息
	int   GenarateOrder(const string &accName, orderReq& req);
	int   GenarateTrade(const string &accName, orderReq& req);
	int   GenaratePosiAndUpdate(const string &accName, orderReq& req);
	int   UpdateCapital(const string &accName, TRADEPARSEUNIT &tpu);
	
	int   UpdatePosition(const string &accName, TRADEPARSEUNIT &tpu);
	int   UpdateAccountInfo(const string &accName, TRADEPARSEUNIT &tpu);

	char* get_num_str(char* lp_char);
	static char* GetGUID(char*lp_char, size_t size);

	//ObjBufferPool<string, orderReqVirtual> m_ReqOrderBuffMap;//保留模拟交易的请求下单信息 orderRef为唯一标识

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	/************************************订单操作*************************************************/
	//发送订单
	int SendOrder(const string& accName, string strategy, orderReq& req);
	//取消订单
	int CancelOrder(const string& accName, orderCancelReq& req);
	//修改订单
	int ModifyOrder(const string& accName, orderModifyReq& req);
	//异步获取数据
	int QueryData(const string& accName, eventData::EVENT_DATA_TYPE type, __int64 param, cbDataUpdate cbFunc);
	/********************************************************************************************/
	//修改持仓预报单
	int ModifyPosiPreOrder(const string& accName, orderModifyReq& req);





private:
	//虚拟交易所专用，用于向界面发送状态
	void FireVSessionStatusChanged(int sessId,  ACCOUNT_STATUS status);
	void FireVMessage(int sessId, MSG_TYPE mt, int num, const char* msg);


	void FireSessionStatusChanged(int sessId, __int64 cbUser, ACCOUNT_STATUS status);
	void FireDataUpdate(int sessId, eventData* data, __int64 param);
	void FireMessage(int sessId, MSG_TYPE mt, int num, const char* msg);
private:
	static bool SliptAccName(string& src, string& accNum, string& brokerNum);
	
	//用于传递给CTP组件的回调接口
	static void OnSessionStatusChanged(int sessId, __int64 cbUser, ACCOUNT_STATUS status);
	static void OnDataUpdate(int sessId, eventData* data, __int64 param);
	static void OnMessage(int sessId, MSG_TYPE mt, int num, const char* msg);

	static eventHandler s_handler;

	///获取交易模块名称
	fnTrade_Name m_lpTradeName=nullptr;
	//初始化
	fnTrade_Init m_lpTradeInit = nullptr;
	//反初始化
	fnTrade_Uninit m_lpTradeUninit = nullptr;
	//回收数据
	fnTrade_Recyle m_lpTradeRecyle = nullptr;
	//创建账号连接会话
	fnTrade_CreateSession m_lpTradeCreateSession = nullptr;
	fnTrade_SessionIDList m_lpTradeSessionIDList = nullptr;
	fnTrade_SessionStatus m_lpTradeSessionStatus = nullptr;
	fnTrade_SessionOpen m_lpTradeSessionOpen = nullptr;
	fnTrade_SessionClose m_lpTradeSessionClose = nullptr;
	fnTrade_SessionDel m_lpTradSessionDel = nullptr;
	fnTrade_SessionAccountInfo m_lpTradeSessionAccountInfo = nullptr;
	//发送订单
	fnTrade_SendOrder m_lpTradeSendOrder = nullptr;
	//取消订单
	fnTrade_CancelOrder m_lpTradeCancelOrder = nullptr;
	//修改订单
	fnTrade_ModifyOrder m_lpTradeModifyOrder = nullptr;
	//得到账户汇总信息	
	//异步获取数据
	fnTrade_QueryData m_lpTradeQueryData = nullptr;

	

	HMODULE  m_hModule=NULL;

	mutex m_mtxHandler;
	mutex m_mtxHook;
	mutex m_mtxAcc;

	//虚拟交易数据的同步
	mutex m_mtxVHandler;
	mutex m_mtxVAcc;
	TraderHandlerMap m_mapHandler;
	TraderVHandlerMap m_mapVHandler;

	TraderHookMap	 m_mapHook;

	AccountNameMap  m_mapAcc;
	AccountNameMap  m_mapVAcc;


public:
	// 查询合约信息
	vector<InstrumentInfo> m_vProdCode; //交易类型

	// 20170509	
	//基础数据
	using AccInvestorMap   = map<string, AccountInfo>;
	using AccBullMap       = map<string, ExchangeBulletin>;
	using AccInstrumentMap = map<string, InstrumentInfo>;

	//业务数据
	using AccCapitalMap    = map<string, accountDetail>;
	using AccOrderMap      = map<string, vector<commissionOrder>>;
	using AccPosiMap       = map<string, vector<positionOrder>>;
	using AccPosiDetailMap = map<string, vector<positionOrder>>;
	using AccTransMap      = map<string, vector<transDetail>>;


	/////////虚拟交易一套数据/////////////////////////////////////////////////////
	using CapitalInfoMap = map<string, vector<CaptalInfo>>;

	//投资者信息
	AccInvestorMap      m_mapVInvestor;
	//投资者资金流水信息
	CapitalInfoMap      m_mapVCapitalInfo;

	//合约信息
	AccInstrumentMap    m_mapVInstrument;
	//账户汇总信息
	AccCapitalMap       m_mapVAccCapital;
	//保存报单流水
	AccOrderMap         m_mapVComOrder;
	//保存持仓数据
	AccPosiMap          m_mapVPosiMatch;
	//保存持仓明细数据
	AccPosiDetailMap    m_mapVPosiDetailMatch;
	//保存成交流水数据
	AccTransMap         m_mapVTransMatch;
	//////////////////////////////////////////////////////////////////////////

	//投资者信息
	AccInvestorMap      m_mapInvestor;
	//交易所公告
	AccBullMap          m_mapBull;
	//合约信息
	AccInstrumentMap    m_mapInstrument;
	//账户汇总信息
	AccCapitalMap       m_mapAccCapital;
	//保存报单流水
	AccOrderMap         m_mapComOrder;
	//保存持仓数据
	AccPosiMap          m_mapPosiMatch;
	//保存持仓明细数据
	AccPosiDetailMap    m_mapPosiDetailMatch;
	//保存成交流水数据
	AccTransMap         m_mapTransMatch;
	


private:
	//dataUpdate  kenny   用于数据更新的子处理函数
	void AddExchangeBull(const string& accName, const eventData *data);
	void AddInvestorInfo(const string& accName, const eventData *data);
	void AddAccountDetailInfo(const string& accName,const eventData *data);
	void AddInstrumentInfo(const string& accName, const eventData *data);
	void AddCommissionOrder(const string& accName, const eventData *data);
	void AddPositionOrder(const string& accName, const eventData *data);
	void AddPositionOrderDetail(const string& accName, const eventData *data);
	void AddTransDetail(const string& accName, const eventData *data);
	
	//end


	//根据增量成交，更新持仓交易的持仓数据
	void UpdatePosiByTransaction(const string &accname, const transDetail *trans);
	//统计盈亏次数，胜率等
	void UpdateAccByPosiDetail(const string &accname);

	//通过持仓明细来更新持仓
	void UpdatePosiByPosiDetail(const string &accname);

	//通过委托查找持仓
	void UpdatePosiByCommiss(const string &accname);

	//统计手续费，盈亏总额等
	void UpdateAllByPosi();
	//统计全部
	//void UpdateAll();


};


extern  CTraderManager   *g_LxTranManager;
