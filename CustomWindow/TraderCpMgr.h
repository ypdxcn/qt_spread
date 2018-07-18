#ifndef _CLIENT_CP_MGR_H
#define _CLIENT_CP_MGR_H

//#pragma warning( disable: 4819 ) // 去掉“该文件包含不能在当前代码页(936)中表示的字符”的warning

#include "Comm.h"
#include "CommHandler.h"
#include <QString>
#include <iostream>
#include <QMap>
#include <list>
#include <QString>
#include <QVector>
#include <QList>
#include <QMutex>

#include <thread>
#include <functional>
using namespace std;

#include "TradePacketAndStruct.h"
#include "BroadcastPacketAndStruct.h"
#include "..\StDef.h"
//
#include "PreOrder.h"
//#include "MyLogEx.h"
#include "HJDef.h"
#include "Const.h"
#include "threadPool.h"
#include "mainWindow.h"
#include "QParamEvent.h"


// added by Jerry Lee, 2013-6-14, 错误码定义
#define YLINK_NO_ERROR					0
#define YLINK_ERROR_BASECODE			-1
#define YLINK_ERROR_NETWORK				1000  // 网络异常
#define YLINK_ERROR_AUTHENTICATION		1001  // 认证失败
#define YLINK_ERROR_PASSWORD			1002  // 密码验证失败

struct ExchFare;
struct FareInfo;

struct OrderFrozeInfo
{
	double dEntrBuy;         // 委托金额，远期的话就是保证金
	double dEntrReserve;     // 委托溢短备付金
	double dEntrExchFare;    // 委托手续费
	int iTotal;              // 总手数

	int iRemainAmt;          // 剩余手数
							 // 	double dEntrBuyRem;      // 剩余委托金额 double dEntrReserveRem;  // double dEntrExchFareRem; //

	void Init(double dTempEntrBuy, double dTempEntrReserve, double dTempEntrExchFare, int iTempTotal)
	{
		iRemainAmt = iTotal = iTempTotal;
		dEntrReserve = dTempEntrReserve;
		dEntrExchFare = dTempEntrExchFare;

		dEntrBuy = dTempEntrBuy;
	}

	// 模拟解冻iCancelHand手
	//bool OrderFrozeInfo::CancelBal(int iCancelHand, double &dCancelBal);

	//bool OrderFrozeInfo::CancelBalForward(int iCancelHand, double &dCancelBal);
};

struct FundInfo
{
	double dSurplus;       // 浮动盈亏
	double dAllBalance;    // 总额
	double dUseFulBal;     // 可用金额
	double dExchFrozeBal;  // 交易冻结资金
	double dPosiMargin;    // 持仓盈亏

	FundInfo()
	{
		ClearData();
	}

	void ClearData()
	{
		dSurplus = 0.0;
		dAllBalance = 0.0;
		dUseFulBal = 0.0;
		dExchFrozeBal = 0.0;
		dPosiMargin = 0.0;
	}
};


extern HEADER_REQ    g_HeaderReq;
extern quint64        g_SeqNo;

typedef enum
{
	FIELDKEY_LASTSETTLE = 0x0000,
	FIELDKEY_LASTCLOSE = 0x0001,
	FIELDKEY_OPEN = 0x0002,
	FIELDKEY_HIGH = 0x0003,
	FIELDKEY_LOW = 0x0004,
	FIELDKEY_LAST = 0x0005,
	FIELDKEY_CLOSE = 0x0006,
	FIELDKEY_SETTLE = 0x0007,
	FIELDKEY_BID1 = 0x0008,
	FIELDKEY_BIDLOT1 = 0x0009,
	FIELDKEY_BID2 = 0x000A,
	FIELDKEY_BIDLOT2 = 0x000B,
	FIELDKEY_BID3 = 0x000C,
	FIELDKEY_BIDLOT3 = 0x000D,
	FIELDKEY_BID4 = 0x000E,
	FIELDKEY_BIDLOT4 = 0x000F,
	FIELDKEY_BID5 = 0x0010,
	FIELDKEY_BIDLOT5 = 0x0011,
	FIELDKEY_ASK1 = 0x0012,
	FIELDKEY_ASKLOT1 = 0x0013,
	FIELDKEY_ASK2 = 0x0014,
	FIELDKEY_ASKLOT2 = 0x0015,
	FIELDKEY_ASK3 = 0x0016,
	FIELDKEY_ASKLOT3 = 0x0017,
	FIELDKEY_ASK4 = 0x0018,
	FIELDKEY_ASKLOT4 = 0x0019,
	FIELDKEY_ASK5 = 0x001A,
	FIELDKEY_ASKLOT5 = 0x001B,
	FIELDKEY_VOLUME = 0x001C,
	FIELDKEY_WEIGHT = 0x001D,
	FIELDKEY_HIGHLIMIT = 0x001E,
	FIELDKEY_LOWLIMIT = 0x001F,
	FIELDKEY_POSI = 0x0020,
	FIELDKEY_UPDOWN = 0x0021,
	FIELDKEY_TURNOVER = 0x0022,
	FIELDKEY_AVERAGE = 0x0023,
	FIELDKEY_SEQUENCENO = 0x0024,
	FIELDKEY_QUOTETIME = 0x0025,
	FIELDKEY_UPDOWNRATE = 0x0026,
	//三代改造
	//FIELDKEY_QUOTEDATE = 0x0027,
	//FIELDKEY_UNIT = 0x0028,//40
	FIELDKEY_BID6 = 0x0027,
	FIELDKEY_BIDLOT6 = 0x0028,
	FIELDKEY_BID7 = 0x0029,
	FIELDKEY_BIDLOT7 = 0x002A,
	FIELDKEY_BID8 = 0x002B,
	FIELDKEY_BIDLOT8 = 0x002C,
	FIELDKEY_BID9 = 0x002D,
	FIELDKEY_BIDLOT9 = 0x002E,
	FIELDKEY_BID10 = 0x002F,
	FIELDKEY_BIDLOT10 = 0x0030,

	FIELDKEY_ASK6 = 0x0031,
	FIELDKEY_ASKLOT6 = 0x0032,
	FIELDKEY_ASK7 = 0x0033,
	FIELDKEY_ASKLOT7 = 0x0034,
	FIELDKEY_ASK8 = 0x0035,
	FIELDKEY_ASKLOT8 = 0x0036,
	FIELDKEY_ASK9 = 0x0037,
	FIELDKEY_ASKLOT9 = 0x0038,
	FIELDKEY_ASK10 = 0x0039,
	FIELDKEY_ASKLOT10 = 0x003A,

	FIELDKEY_UNKNOWN
} ENUM_FIELDKEY;

#define QUOTATION_REC_SIZE sizeof(QUOTATION) //128


/*
//声明一个模板 
typedef std::function<void(const char * prod, const char * price, const char * volume)> Functional;

class  QStrategyRunning : public QRunnable
{

public:
	QStrategyRunning()
	{
		flgRunning = true;
		if (condition == nullptr)
		{
			condition = new strategy_conditionOrder();
		}
	};

	void stop()
	{
		//unique_lock<mutex> lock(queue_mutex);
		flgRunning = false;
	}

	void setStragtegyCondition(strategy_conditionOrder *sc)
	{
		*condition = *sc;
	}
	void run()
	{
		flgRunning = true;
		while (flgRunning)
		{
			unique_lock<mutex> lock(queue_mutex);

			qDebug() << " Strategy  Running.....";
			//判断行情
			//QMap<QString, vector<KLine> >mapProdData;
			//;
			if (g_TraderCpMgr.m_QHMapQuotation.size() < 1)
			{
				QThread::msleep(1500);
				continue;
			}

			QUOTATION  &quote = g_TraderCpMgr.m_QHMapQuotation[condition->id];

			//KLine &test = list[list.size() - 1];//拿出最新的数据出来比较
			if (condition->gainPrice > 0) //>= 符号
			{
				//价格差
				//double val = quote.m_Ask[0].m_uiPrice- condition->;
				//if (condition->compType  > 0)
				{
					//if (val >= condition->compVal)
					{
						//出现信号，提交买卖单
						//if (condition->orderDirection <= 0)//卖出
						{
							//报单
							//SendOrder(sellType,test.closeingPrice,condition->SellVolmune);
							/*funcSell(condition->strategyObj.prodID.toStdString().c_str(),
							QString::number(condition->SellPrice).toStdString().c_str(),
							QString::number(condition->SellVolmune).toStdString().c_str());

							condition->SellRunNum++;
						//}
					}
				}
			}
		}

		qDebug() << "Strategy Exited!";
	}

public:

	bool     flgRunning;
	//QMutex    *mutext;//一个策略一个线程，屏蔽
	strategy_conditionOrder *condition;

	mutex queue_mutex;

	Functional  funcSell;
	Functional  funcBuy;

	Functional  funcSell_QH;
	Functional  funcBuy_QH;
};

*/







// 所有的广播消息定义
typedef enum
{
	E_ONRECVRTNSPOTMATCH = 0,
	E_ONRECVRTNFORWARDMATCH,
	E_ONRECVRTNDEFERMATCH,
	E_ONRECVRTNDEFERDELIVERYAPPMATCH,

	E_ONRECVRTNSPOTORDER,
	E_ONRECVRSPSPOTORDER,
	E_ONRECVRSPFORWARDORDER,
	E_ONRECVRTNFORWARDORDER,
	E_ONRECVRTNDEFERORDER,
	E_ONRECVRSPDEFERORDER,
	E_ONRECVRTNDEFERDELIVERYAPPORDER,
	E_ONRECVRSPDEFERDELIVERYAPPORDER,
	E_ONRECVRTNMIDDLEAPPORDER,
	E_ONRECVRSPMIDDLEAPPORDER,

	E_ONRECVRTNSPOTORDERCANCEL,
	E_ONRECVRSPSPOTORDERCANCEL,
	E_ONRECVRTNFORWARDORDERCANCEL,
	E_ONRECVRSPFORWARDORDERCANCEL,
	E_ONRECVRTNDEFERORDERCANCEL,
	E_ONRECVRSPDEFERORDERCANCEL,
	E_ONRECVRTNDEFERDELIVERYAPPORDERCANCEL,
	E_ONRECVRSPDEFERDELIVERYAPPORDERCANCEL,
	E_ONRECVRTNMIDDLEAPPORDERCANCEL,
	E_ONRECVRSPMIDDLEAPPORDERCANCEL,

	E_ONNEWBOURSEBULLETIN,
	E_ONNEWMEMBERBULLETIN,
	E_ONRISKNOTIFY,
	E_ONSYSINIT,
	E_ONSYSSTATCHANGE,

	E_ONRECVSPOTQUOTATION,
	E_ONRECVFORWARDQUOTATION,
	E_ONRECVDEFERQUOTATION,
	E_ONRECVDEFERDELIVERYQUOTATION,
	E_ONRECVFUTUREQUOTATION,//期货行情
	E_ONRECVSPREADQUOTATION,//套利行情

	E_ONRECVRTNSPOTINSTSTATEUPDATE,
	E_ONRECVRTNFORWARDINSTSTATEUPDATE,
	E_ONRECVRTNDEFERINSTSTATEUPDATE,

	E_ONBASETABLEUPDATE,
	E_ONRECVRTNSPOTMARKETSTATEUPDATE,
	E_ONRECVRTNFORWARDMARKETSTATEUPDATE,
	E_ONRECVRTNDEFERMARKETSTATEUPDATE,
	E_ONCUSTINFOCHANGE,
	E_ONCUSTRISKDEGREECHANGE,

	E_ONFORCELOGOUT,
	E_ONSESSIONKEY,
	E_ONACCTCAPITALACCESS,

	E_ONRECVSPOTORDER,
	E_ONRECVFORWARDORDER,
	E_ONRECVDEFERORDER,
	E_ONRECVDEFERDELIVERYAPPORDER,
	E_ONRECVMIDDLEAPPORDER,

	// 登录成功
	E_ONLOGINSUCC,
	E_ONREFRESHCUSTOMDATA,
	E_COMMIT_POSI,
	E_COMMIT_STORE,
	E_PREORDER_CHANGE,
	E_REFRESHFUND,
	E_UPDATE_LIST,
	E_HQ_STATE,
	E_SER_PREORDER_REFRESH,

	E_WIND_VANE,
	E_SURPLUS_CHANGE, // 持仓盈亏发生变化
	E_TOTAL_SURPLUS_CHANGE, // 总持仓盈亏发生变化
	E_CUSTOM_INFO_CHANGE, // 客户信息变化
	E_ADD_PREORDER_CONFIRM, // 生成条件单
} EHJBROADCASTMSGTYPE;

typedef enum
{
	E_DlgOrder,
	E_DlgUpRight,
	E_DlgDownLeft,
	E_DlgDownRight,
	// 	E_DlgEnterFlow,
	// 	E_DlgPreOrder,
	// 	E_DlgPosi,
	// 	E_DlgStore,
	// 	E_DlgFund,
	// 	E_DlgMatchFlow,
} EDLGID;


// 广播器类
class CBroadcaster
{
public:
	CBroadcaster();

	~CBroadcaster();

	CBroadcaster(const CBroadcaster& bdr);

	CBroadcaster& operator=(const CBroadcaster& bdr);


	// 窗口句柄为int的窗口订阅消息
	void Subscribe(QWidget* val);

	// 窗口句柄为int的窗口取消订阅消息
	void Unsubscribe(QWidget* val);

	// 广播消息
	void Broadcast(uint uMsg, void* wParam, void * lParam, BOOL bAsyn = TRUE);
	

private:

	int Find(QWidget* val);
	QVector<QWidget *> m_arrint;


};


//QWaitCondition bufferUpdate;
//bool           newSignal;
//QMutex         sigMutex;
//QString        gstrCurT1ProdId;
//QString        gstrCurT2ProdId;
//
////套利策略
//using Functional = std::function<void(const char * prod, const char * price, const char * volume)>;
//class  QStrategyRunning : public QRunnable
//{
//
//public:
//	QStrategyRunning()
//	{
//		newSignal = false;
//		flgRunning = true;
//		//if (condition == nullptr)
//		//{
//		//	condition = new strategy_conditionOrder();
//		//}\
//
//		condition = std::make_shared<strategy_conditionOrder>();
//	};
//
//
//	void stop()
//	{
//		//unique_lock<mutex> lock(queue_mutex);
//		flgRunning = false;
//	}
//
//	void setStragtegyCondition(strategy_conditionOrder *sc)
//	{
//		*condition = *sc;
//	}
//	void run()
//	{
//		flgRunning = true;
//		while (flgRunning)
//		{
//			unique_lock<mutex> lock(queue_mutex);
//
//			sigMutex.lock();
//			//判断行情
//			if (!newSignal)
//				bufferUpdate.wait(&sigMutex);
//
//			//if (g_TraderCpMgr.m_QHMapQuotation.size() < 1)
//			//{
//			//	QThread::msleep(1500);
//			//	continue;
//			//}
//
//			QUOTATION  &QHquote = condition->obj[0].quote;// g_TraderCpMgr.m_QHMapQuotation[gstrCurT1ProdId];//condition->id
//			QUOTATION  &HJquote = condition->obj[1].quote;
//
//			//KLine &test = list[list.size() - 1];//拿出最新的数据出来比较
//			if (condition->gainPrice > 0) //>= 符号
//			{
//				//价格差
//				//double val = quote.m_Ask[0].m_uiPrice- condition->;
//				//if (condition->compType  > 0)
//				{
//					//if (val >= condition->compVal)
//					{
//						//出现信号，提交买卖单
//						//if (condition->orderDirection <= 0)//卖出
//						{
//							//报单
//							//SendOrder(sellType,test.closeingPrice,condition->SellVolmune);
//							/*funcSell(condition->strategyObj.prodID.toStdString().c_str(),
//							QString::number(condition->SellPrice).toStdString().c_str(),
//							QString::number(condition->SellVolmune).toStdString().c_str());
//
//							condition->SellRunNum++;*/
//						}
//					}
//				}
//			}
//
//
//
//			sigMutex.unlock();
//			newSignal = false;
//		}
//
//		//qDebug() << "Strategy Exited!";
//	}
//
//public:
//
//	bool     flgRunning;
//	//QMutex    *mutext;//一个策略一个线程，屏蔽
//	std::shared_ptr<strategy_conditionOrder >condition;
//
//	mutex queue_mutex;
//
//
//	Functional  funcSell;
//	Functional  funcBuy;
//
//	Functional  funcSell_QH;
//	Functional  funcBuy_QH;
//};



//class CPacketReceiver;

//class QStrategyRunning;

//struct   srategy_condition1
//{
//	//
//	int  test;
//	std::string t1;
//	std::string t2;
//};

class  strategyCondition : public Theron::Actor
{
public:
	inline strategyCondition(Theron::Framework & framework) :Theron::Actor(framework)
	{
		RegisterHandler(this, &strategyCondition::produce);
	}

private:
	inline void produce(const strategy_conditionOrder& item, const Theron::Address  from)
	{
		//
		strategy_conditionOrder  test;
		/*test.test = item.test + 1;
		test.t1 = item.t1;
		test.t2 = item.t2;*/
		if (Send(test, from))
		{
			//
		}

		return;
	}
};

class  strategyOrder : public Theron::Actor
{
	using Task = std::function<void(const char *prodId,double price,double volume,int direction，bool)>;//direction  1:
public:
	inline strategyOrder(Theron::Framework & framework) :Theron::Actor(framework)
	{
		RegisterHandler(this, &strategyOrder::consume);
	}
	void setTask(Task &task)
	{
		mDeque.push_back(task);
	}

	//void setSellTask(Task &task)
	//{
	//	mSTask = task;
	//}

private:
	inline void consume(const strategy_conditionOrder& item, const Theron::Address  from)
	{
		//
		//strategy_conditionOrder  test;
		/*test.test = item.test - 1;
		test.t1 = item.t1;
		test.t2 = item.t2;*/
		//编译，执行报单
		for (auto t : mDeque)
		{
			t(item.obj[0].prodID.toStdString().c_str(), item.price, item.volume, item.direction.toInt());//0对象同向
			t(item.obj[1].prodID.toStdString().c_str(), item.price, item.volume, item.direction == "1" ? 0 : 1);//反向
		}
		//通知完成
		if (Send(0, from))
		{
			//
		}

		return;
	}


	std::deque<Task> mDeque;
	//Task  mBTask;
	//Task  mSTask;
};





// 黄金业务处理类
class CTraderCpMgr : public CPacketReceiver, public CPacketReceiverQH
{
	Theron::Framework  *framework = nullptr;
	strategyCondition  *gstrategyCond = nullptr;
	strategyOrder      *gstrategyOrder = nullptr;
public:
	CTraderCpMgr();
	virtual ~CTraderCpMgr();

	void resetAccount(QString && str);
	void InitStrategy();
public:
	// 保存广播报文中成交流水数据
	QMap<QString, SpotMatch>                m_QMapSpotMatch;
	QMap<QString, ForwardMatch>             m_QMapForwardMatch;
	QMap<QString, DeferMatch>               m_QMapDeferMatch;
	QMap<QString, DeferDeliveryAppMatch>    m_QMapDDAMatch;

	// 保存广播报文中报单流水数据
	QMap<QString, SpotOrder>                 m_QMapSpotOrder;      //
	QMap<QString, ForwardOrder>              m_QMapForwardOrder;   //
	QMap<QString, DeferOrder>                m_QMapDeferOrder;     // 
	QMap<QString, DeferDeliveryAppOrder>     m_QMapDDAOrder;       // 
	QMap<QString, MiddleAppOrder>            m_QMapMiddleAppOrder; // 

																   // 撤单流水
	QMap<QString, OrderCancel>               m_QMapDeferOrderCancel; // 延期的撤单单独处理
	QMap<QString, OrderCancel>               m_QMapOrderCancel;      // 不包括延期的撤单

																	 // 记录客户的持仓信息
	QMap<QString, DeferPosi>                 m_QMapDeferPosi;

	// 库存信息
	QMap<QString, StoreInfo>               m_QMapStoreInfo; // 

															// 资金信息
	FundInfo m_stFundInfo;

	// 当日出入金信息
	QMap<QString, QString> m_QMapTransfer;

	// 当日报单冻结信息
	QMap<QString, OrderFrozeInfo> m_QMapOrderFroze; //

	// 处理成交流水时释放冻结资金的时候没找到的报单的本地报单号（发生情况？）
	QVector<QString> m_arrUnFindLocalOrderNo;

	QMap<QString, int> m_QMapMatchUnFindInfo;


	// 系统参数
	QMap<QString, QString> m_QMapSystemPara;

	// 品种状态
	QVector<InstState>                      m_vInstState;//

	// 预埋单
	CPreOrder                              m_cPreOrder;       // 预埋单管理器

	//CAutoSendThread                        m_cAutoSendThread;  //预埋单线程
	//kenny  2018-1-8 替换自动线程
	ThreadPool    pool;
	std::thread   *m_cAutoSendThread;


	 // 记录服务器客户信息（资金、个人信息、持仓以及库存）
	Rsp1020 m_rsp1020;

	// 记录客户的费率模板
	Rsp2040 m_rsp2040;

	quint64 m_uOrderSerial;

	QMap<EDLGID, int> m_QMapDlgHandle;

	list<QString> m_QMapUnMatchOrder; // 记录在报单中出现过的报单，主要是用来区分是登录是推送成交单或者是软件运行时成交单
	bool bIsOrderUnMatch(const QString &sLocalOrderNo, bool bErase = false);

	// added by Jerry Lee, 2013-6-19, 报单流水中是否存在开仓单
	bool OpenedBillExists();

public:
	// 基础数据相关-----------------------------------------------------------------------------------------------
	typedef bool (CTraderCpMgr::*PFunTypeAsk)(ArrayListMsg &);
	typedef void (CTraderCpMgr::*PFunTypLoad)(const ArrayListMsg &);

	// 初始化基础数据统一调用的函数
	bool IniBasicData(const QString &csTitle, PFunTypeAsk pFunAsk, PFunTypLoad pFunLoad);

	// 请求服务器基础数据统一调用的函数
	bool AskForData(const QString &sReqTitle, QVector< pair<QString, QString> > &vecPara, const ArrayListMsg &almViewField, ArrayListMsg &alm_result);

	// 请求资金基础数据
	bool AskForFundBaseData(ArrayListMsg &alm_result);

	// 加载资金基础数据
	void LoadFundBaseData(const ArrayListMsg &alm_result);

	// 请求延期基础数据
	bool AskForDeferPosiBaseData(ArrayListMsg &alm_result);

	// 加载延期基础数据
	void LoadDeferPosiBaseData(const ArrayListMsg &alm_result);

	// 请求远期基础数据
	bool AskForForwardPosiBaseData(ArrayListMsg &alm_result);

	// 加载远期基础数据
	void LoadForwardPosiBaseData(const ArrayListMsg &alm_result);

	// 请求库存基础数据
	bool AskForStoreBaseData(ArrayListMsg &alm_result);

	// 加载库存基础数据
	void LoadStoreBaseData(const ArrayListMsg &alm_result);

	// 读取配置文件，初始化当日的出入金流水
	void IniTodayTransfer();

	// 读取内存里面保存的当日出入金流水，构建成arraylist的格式
	QString FormatCurTransferStr();

	// 广播订阅相关---------------------------------------------------------------------------------
	void SubscribeOrder(QWidget* hHandle, bool bSubscribe = true, const QString &sMarketID = "");
	void SubscribeOrderCancel(QWidget* hHandle, bool bSubscribe = true, const QString &sMarketID = "");
	void SubscribeMatch(QWidget* hHandle, bool bSubscribe = true, const QString &sMarketID = "");
	void SubscribeAll(QWidget* hHandle, bool bSubscribe = true);
	// 订阅延期业务广播
	void SubscribeDefer(QWidget* hHandle, bool bSubscribe = true, bool bNormal = true, bool bDDA = true, bool bMA = true);


	// 出入金本地计算相关---------------------------------------------------------------------------------
	// 将内存中的当日出入金流水组建成字符串，写入文件
	void WriteTodayTransferToFile();

	// 处理出入金流水查询中一笔入账流水的信息，流水则内存中不存在则返回true，存在则返回false
	bool DealOneHandTransfer(const QString &sSerialNo, QString sExchBal, bool bIn);

	// 获取当前的出入金信息
	bool GetTodayTransfer();

	// 根据3101的响应报文加载当天出入金信息
	void LoadTodayTransfer(HashtableMsg &htm_result);
	void LoadTodayTransfer(ArrayListMsg &alm_result);

	// 资金本地计算相关---------------------------------------------------------------------------------

	void IniOrderSerial();

	QString GetOrderSerialStr();

#if (defined _VERSION_JSZX) || (defined _VERSION_ZHLHY)
	// 设置出入金采用的模式（自动出入金还是手动出入金，仅对JSZX版本有用）
	void SetTransferMode(const ArrayListMsg &alm_menu_info);
#endif

public:
	// 根据合约代码获取客户的可用仓位
	void CustomerUsefulPosi(const QString& prodCode, int& iUsefullong, int& iUsefulshort);

	// 获取延期品种sProdCode的交割替代品种的名称（sProdCode：延期报单品种  sSubVAriety：交割替代品种名称  sSeq：序号）
	bool GetCusSubVarietyName(const QString &sProdCode, QString &sSubVAriety, const QString &sSeq);

	// 获取中立仓报单价格
	double GetMAOrderPrice(const QString &sProdCode);

	// 处理品种状态相关
	int GetInstStatePos(const QString &sProdCode);
	// 根据行情品种获取当前品种的状态ID
	QString GetInsStateID(const QString &sInsID);

	// 处理现货报单时的库存冻结或解冻 bFreeze（是否是冻结，否则为解除冻结）
	bool HandleTradeStore(const QString &sProdCode, int iHand, bool bFreeze, const QString &sDDAProdCode = "");

	// 处理报单时的资金冻结或解冻 bFreeze（是否是冻结，否则为解除冻结）
	bool HandleTradeFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, bool bFreeze, const QString &localOrderNo = "");

	// 处理撤单时的资金解冻
	bool HandleCancelTradeFee(const QString &sProdCode, int iHand, const QString &localOrderNo);

	// 计算一笔交易的费用
	double CalculateTradeFee(const ProdCodeInfo &info, const QString &sExchID, double dPrice, int iHand, bool bMatch = false, const QString &localOrderNo = "");

	// 计算
	void CalSpotSellMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, double &dMatchBal, double &dExchFare);

	// 获取一个延期品种的持仓保证金
	double CalculateMagin(const QString &sProdCode, double dPrice, int iHand, bool bSumSingle = true);

	// 计算现货成交的资金信息
	bool HandleSpotMatchFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, bool bAdd, const QString &localOrderNo = "");

	// 处理现货成交的库存
	bool HandleMatchStore(const QString &sProdCode, int iHand, bool bAdd);

	// 处理延期成交的资金信息
	bool HandleDeferMatchFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, bool bBuy, double &dPosiMoney, const QString &localOrderNo = "");

	// 处理延期平仓成交的资金信息
	bool HandleDeferCovMatchFee(const QString &sProdCode, double dPrice, int iHand, bool bLong);

	// 计算平仓的时候释放的保证金和持仓盈亏
	void HandleDeferCovReleaseFee(const QString &sProdCode, double dPrice, int iHand, double dMeasure_unit, bool bLong, ExchFare &ef_m_release_margin, double &d_cov_surplus);

	// 处理延期交收成交的资金信息
	bool HandleDDAMatchFee(const QString &sProdCode, int iHand, bool bBuy, const QString &localOrderNo = "");

	// 处理中立仓成交的资金信息
	bool HandleMAMatchFee(const QString &sProdCode, int iHand, bool bBuy, const QString &localOrderNo = "");

	// 延期交货所释放的延期开仓保证金
	double GetOpenFareOrg(const QString &sProdCode, bool bLong, double fUnite, int iHand /*= 1*/);

	// 根据参数ID获取系统参数的值，如果获取失败，则用默认值，如果成功，则记录到内存
	QString GetParaValue(const QString &sParaID, const QString &sDefaultValue);

	// 计算持仓盈亏
	void CalculatePosiMargin();

	// 代替交割品种
	HashtableMsg m_htmSubVariety;
	QString GetSubVarietyProdCode(const QString &sProdCode);

	// // 获取延期交收对资金处理所采用的价格：配置的价格（当前使用默认的上日结算价）*（1+涨停板率）
	double GetDeferDeliveryPrice(const QString &sProdCode);


	// 更新报单信息
	void RefreshOrderInfo(OrderBase *OrgOrder, const OrderBase *NewOrder);
	// 暂时定为延期交货所释放的延期开仓保证金
	double GetOpenFare(const QString &sProdCode, bool bLong, double fUnite, const FareInfo &fi, int iHand /*= 1*/);

	// 设置用来传递参数的bit数组，从右边开始依次是：资金、库存、持仓、多空、买卖
	void SetBitSet(bitset<5> &bitvec, bool bFund = false, bool bStore = false, bool bPosi = false, bool bLong = false, bool bBuy = false);

	double GetRealeaseMoney(const QString &localOrderNo, int iHand);

	double GetMatchUnReleaseMoney(const QString &localOrderNo);

	//void RecordOrderFroze(const QString &localOrderNo);

	double HandleMatchReleaseMoney(const QString &localOrderNo, int iHand);

	void CalForwardMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, double &dPosiMoney, double &dExchFare);

	// 处理一笔远期成交后的资金变化
	bool HandleForwardMatchFee(const QString &sProdCode, const QString &sExchID, double dPrice, int iHand, double &dPosiMoney, const QString &localOrderNo);

	bool IsOrderCanBeProcess(const QString &sOrderState);

	void CalDeferMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, double &dPosiMoney, double &dExchFare, bool bOpen);

	void CalDeferCovMatchFee(const ProdCodeInfo &info, double dPrice, int iHand, bool bLong, ExchFare &efPosiMoney, ExchFare &efExchFare, double &d_cov_surplus);

	void GetDDATradeFee(const ProdCodeInfo &info, bool bLong, int iHand, ExchFare &ef_m_deli_prepare, ExchFare &ef_m_release_margin);

	//
	void GetMAMatchFee(const ProdCodeInfo &info, bool bLong, int iHand, ExchFare &ef_m_deli_prepare, ExchFare &ef_m_release_margin);



	// 持仓自动计算相关-----------------------------------------------------------------------------------------------

	// 模拟今天仓位的平仓
	void RemoveMatchFlow(LISTDeferMatch &listMatch, int iHand);

	/*
	计算某个品种的某一个方向的持仓均价、持仓盈亏、持仓保证金
	bRefresh：是否向服务器请求数据
	*/
	void CalculateOtherValue(const QString &sProdCode, DeferPosiInfo &stDeferPosiInfo, bool bLong, bool bRefresh = false);

	// 计算一个品种的持仓盈亏
	double GetPosi_PL(const QString &sProdCode, double dAvgPosiPrice, int iTotalHand, bool bLong);

	// 接受到延期平仓的报单流水后处理冻结仓位
	bool DealDeferCancelFreezePosi(const QString &instID, bool bLong, bool bExist, int iTotalAmount, int iRemainAmount);

	int CommitOrder(const QString &sProdCode, double dPrice, int iAmount, const QString &sExchID);

	// 增加一个参数，标记是否采用自动提示
	int CommitOrder(const QString &sProdCode, const QString &csPrice, const QString &csAmount, const QString &sExchID, int orderType,bool bAuto = true);

	void ShowMatchTips(const QString &sProdCode, const double &dPrice, const int &iAmount, const QString &sExchID, const QString &sOrderNo);

	int CancelOrder(QString &csOrderNo);


	// 库存相关-----------------------------------------------------------------------------------------------

	bool GetVarietyInfo(const QString &sVarietyID, VarietyInfo &info);

	// 系统处理相关-----------------------------------------------------------------------------------------------

	void GetRsp2040();

	void RefreshCusFundInfo();

	void ClearSession();
	QString GetSessionID();

	QString GetCode_desc(QString codetype, QString codeid); // 获取代码表中某一类型下的某一id下对应的值(codedesc)
	QString GetCode_id(QString codetype, QString codeid); // 获取代码表中某一类型下的某一id下对应的值(codedesc)

														  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int Init();

	int Authenticate(const char* pszUsr, const char* pszPwd, const char* pszAddr = 0, int nPort = 0, QString useridtype = "1", bool bSign = false);
	int UserLogin(const QString &useridtype = "1", bool bSign = false);
	int UnlockScreen(const QString &pwd, const QString &useridtype = "1");

	// 比较本地版本号与服务器版本号，如果小于服务器的版本号，不需要更新参数
	bool CompareVersion(QString strLocalVersion, QString strSerVersion, int iCompare = 2);

	// 清空所有保存码表的QVector
	void ClearCodeTableList();
	//
	void GetProdCodeFromServer();
	//
	void GetVarietyFromServer();
	//
	void GetCodeFromServer();

	// 获取交割替代品种设定
	bool Ini1005();
	// 
	QString GetSubVarietyBySeqNo(const QString &sProdCode, const QString &sSeqNo);

	int UserLogout();

	bool IsLoginSucceed() { return m_bLoginSucceed; }



	int GetDlgHandleFromID(const EDLGID &eDlgID);

	void AddDlgHandle(const EDLGID &eDlgID, const int &hDlg);

	////根据买卖方向获取现货交易类型id
	//QString GetSpotId(QString buysell);

	////根据买卖方向获取远期交易类型id
	//QString GetForwardId(QString buysell);

	////根据买卖方向和开平标志获取延期交易类型id
	//QString GetDeferId(QString offflag, QString buysell);

	//QString GetDDAOrderId(QString buysell);

	//QString GetDDAMatchId(QString middleflag, QString buysell);

	//QString GetMAOrderId(QString buysell);

	//kenny 临界区改为互斥量
	//CRITICAL_SECTION m_criticalSection;
	QMutex m_criticalSection;

	void Broadcast(UINT uType, UINT uMsg, void* wParam, void* lParam, BOOL bAsyn = TRUE)
	{
		m_criticalSection.lock();
		m_mapBdr[uType].Broadcast(uMsg, wParam, lParam, bAsyn);
		m_criticalSection.unlock();
	}

	// 域名到ip
	QString GetIPFromDomain(const QString &sDomain);

public: // 内存变量
	std::map<string, QUOTATION> m_QMapQuotation;  //用来记录广播过来的行情实时报文	
	std::map<QString, QUOTATION> m_QHMapQuotation;  //用来记录广播过来的期货行情实时报文	
	std::map<QString, QUOTATION> m_SpreadMapQuotation;  //套利行情实时报文	

	QVector<DeferDeliveryQuotation> m_vecDDQuotation;

	QVector<MyCodeInfo> m_vCode;                  // 对应于1001,所有的查询条件代码（暂时不用）

												  //查询条件
	QVector<ProdCodeInfo> m_vProdCode; //交易类型
	ProdCodeInfo GetProdeCode(const QString &sProdCode);
	bool GetProdeCode(ProdCodeInfo& stProdCode, const QString &sProdCode);

	QVector<VarietyInfo> m_vVariety;   //交割品种代码 

	QVector<MyCodeInfo> m_vExchCode;   //交易类型
	QVector<MyCodeInfo> m_vOffsetFlag; //开平标志
	QVector<MyCodeInfo> m_vFeeCode;    //费用代码
	QVector<MyCodeInfo> m_vSubFeeCode; //费用代码
	QVector<MyCodeInfo> m_vLongShort;  //持仓方向

									   //list字段转换
	QVector<MyCodeInfo> m_vMarketId;   //交易市场
	QVector<MyCodeInfo> m_vEntrState;  //委托状态
	QVector<MyCodeInfo> m_vCancelFlag; //撤单标志
	QVector<MyCodeInfo> m_vTermType;   //撤销渠道
	QVector<MyCodeInfo> m_vAccessWay;  //操作方向
	QVector<MyCodeInfo> m_vCheckState; //复核状态
	QVector<MyCodeInfo> m_vTakeState;  //提货状态
	QVector<MyCodeInfo> m_vDrawType;   //提货类型
	QVector<MyCodeInfo> m_vCertType;   //证件类型
	QVector<MyCodeInfo> m_vAcctState;  //账户状态
	QVector<MyCodeInfo> m_vInstState2;  //交易所合约状态
	QVector<MyCodeInfo> m_vFCOrderState; // 用于强平单类型的转换
	QVector<MyCodeInfo> m_vDeliFlag; // 交收标志
	QVector<MyCodeInfo> m_vPageInfo; // 查询界面的页码信息

									 // 根据合约代码以及交易代码获取客户的交易所费率模板
	FareInfo GetBFareValue(const QString &sProdCode, const QString &sExchCode);
	// 根据合约代码以及交易代码获取客户的会员费率模板
	FareInfo GetMFareValue(const QString &sProdCode, const QString &sExchCode);

	unsigned int GetLastPrice(const QUOTATION &qt);

	double GetLastPriceEx(const QUOTATION &qt);

	// 根据合约代码获取该合约的相关信息
	bool GetProdCodeInfo(const QString &sProdCode, ProdCodeInfo &info);

	// 初始化系统基本参数
	void IniSystemSetting();

	//
	bool IsUnFindLocalOrderNo(const QString &sLocalOrderNo);

	//bool IsMatchOrderLegal();

public:
	// 根据交割品种ID获取交割品种名称
	QString GetVarietyName(const QString &sID);

	// 根据交易类型ID获取交易类型名称
	QString GetExchName(const QString &sID);

	QString GetExchIDFromName(const QString &sName);

	// 根据委托状态ID获取委托状态名称
	QString GetEntr_statName(const QString &sID);

	// 根据复核状态ID获取复核状态名称
	QString GetCheck_staName(const QString &sID);

	// 根据账户状态ID获取账户状态名称
	QString GetAcct_statName(const QString &sID);

	// 根据证件ID获取证件名称
	QString GetIdTypeName(const QString &sID);

	// 根据证件名称获得证件id
	QString GetIdTypeId(const QString &sName);

	// 根据提货状态ID获取提货状态名称
	QString GetTake_statName(const QString &sID);

	// 根据提货状态名称获取提货状态ID
	QString GetTake_statID(const QString &sName);

	// 根据提货类型ID获取提货类型名称
	QString GetDraw_typeName(const QString &sID);

	// 
	QString GetFCOrderStateName(const QString &sID);

	// 根据交易市场ID获取交易市场名称
	QString GetMarketName(const QString &sID);

	// 根据开平标志ID获取开平标志名称
	QString GetOffset_flagName(const QString &sID);

	// 根据交收标志ID获取交收标志名称
	QString GetDeli_flagName(const QString &sID);

	// 根据买卖方向ID获取买卖方向名称
	QString GetBuySellName(const QString &sID);

	// 根据撤单标志ID获取撤单标志名称
	QString GetCancel_flagName(const QString &sID);

	// 根据操作方向ID获取操作方向名称
	QString GetAccessWayName(const QString &sID);

	// 根据转账是否成功ID获取转账是否成功名称
	QString GetIn_account_flagName(const QString &sID);

	// 根据持仓方向ID获取持仓方向名称
	QString GetLongShortName(const QString &sID);

	// 根据渠道ID获取渠道名称（包括委托渠道,撤销渠道,操作来源）
	QString GetTerm_typeName(const QString &sID);

	QString GetProdCodeName(const QString &sID);

	QString GetInsStateNameFromID(const QString &sInsID);

public:
	//内存路由表
	//QMap<QString, CConnector3**>  m_tblIfRouter;
	QMap<QString, CConnectPointSync**>  m_tblIfRouter;
	int InitRouterTbl();

	int Tran6002Handle(HEADER_REQ& stHeaderReq, Req6002& stBodyReq, HEADER_RSP& stHeaderRsp,
		Rsp6002& stBodyRsp, QVector< pair<QString, QString> > & v, unsigned int uiTimeout);


	template<class REQ, class RSP>
	int HandlePacketCommon(RSP& stBodyRsp, REQ& stBodyReq, const QString &sExchCode, CTradePacket &oPktReq, unsigned int uiTimeout = 0, bool bBackground = false)
	{
		if (uiTimeout == 0)
			uiTimeout = m_nTimeOut;

		//kenny  2017-11-17   修改网络接口部分代码
		// 初始化请求头
		m_csSendPkt.lock();
		g_HeaderReq.IniRequst(sExchCode.toStdString(), g_SeqNo);
		oPktReq.SetHeader(g_HeaderReq);
		m_csSendPkt.unlock();

		CTradePacketAndStruct::Struct2Packet(stBodyReq, oPktReq);

		// 根据交易编码找到对应的接口
		CConnectPointSync* pCp = nullptr;
		if (bBackground)
		{
			pCp = CCommHandler::Instance()->GetInterfaceVip();
		}
		else
		{
			pCp = FindRoute(sExchCode);
		}

		if (nullptr == pCp) // 没有对应的接口
		{
			return -2;
		}

		qDebug( sExchCode.toLatin1().data(), "发送报文");

		// 发送报文
		CTradePacket oPktRsp;
		int nRtn = 0;
		nRtn = pCp->SendPacket(*(CPacket *)&oPktReq, *(CPacket *)&oPktRsp, uiTimeout);
		if (0 != nRtn)
		{
			stBodyRsp.rsp_code = gc_ErrCode_ConSverFail.toStdString();
			stBodyRsp.rsp_msg = "连接服务器失败";
			qDebug(sExchCode.toLatin1().data(), stBodyRsp.rsp_code);

			/*if( nRtn == -100 )*/
			// 如果是风控相关，则不切换服务器，测试环境下请求风险度几乎都是失败的
			/*			if( sExchCode != "3099" )
			{
			SwitchServer();
			}
			*/
			return nRtn;
		}

		oPktRsp.GetRspCode(stBodyRsp.rsp_code);
		CTradePacketAndStruct::Packet2Struct(stBodyRsp, oPktRsp);

		qDebug(sExchCode.toLatin1().data(), stBodyRsp.rsp_code);

		return 0;
	}

	template<class REQ, class RSP>
	int HandlePacket(RSP& stBodyRsp, REQ& stBodyReq, const QString &sExchCode, unsigned int uiTimeout = 0, bool bBackground = false)
	{
		CTradePacket oPktReq;
		return HandlePacketCommon(stBodyRsp, stBodyReq, sExchCode, oPktReq, uiTimeout, bBackground);
	}

	template<class REQ, class RSP>
	int HandlePacket6002(RSP& stBodyRsp, REQ& stBodyReq, const QString &sExchCode, const QVector< pair<QString, QString> > & v, unsigned int uiTimeout = 0)
	{
		CTradePacket oPktReq;

		for (size_t i = 0;i<v.size();i++)
		{
			//oPktReq.AddParameter(v.at(i).first, v.at(i).second);
		}

		return HandlePacketCommon(stBodyRsp, stBodyReq, sExchCode, oPktReq, uiTimeout);
	}

	// 接收数据
	virtual void Receive(CPacket &pkt);

	//接收期货数据
	virtual void Receive(CBroadcastPacket &pkt) override;

	// 查找消息广播器
	CBroadcaster* GetBroadcaster(int nBdrType);

	// 订阅广播消息
	void Subscribe(int nBdrMsgType, QWidget * val);

	// 取消订阅广播消息
	void Unsubscribe(int nBdrMsgType, QWidget * val);

	// 根据id获得品种的库存
	// QString GetUsefulStore(QString variety_id);

	// 根据id获得品种的远期仓位
	//   void GetCangWeiT5(const QString& prodCode, QString& usefullong, QString& usefulshort);

	QString GetEncryptPSW(const char* pszPwd, bool bShort = false);

private:
	//定义成员函数指针
	typedef int (CTraderCpMgr::*MFP_PacketHandleApi)(CBroadcastPacket& pkt);

	//报文命令字与报文处理成员函数映射结构
	typedef struct tagCmd2Api
	{
		string sApiName;						//报文ApiName或交易代码
		MFP_PacketHandleApi pMemberFunc;		//报文处理函数指针

	} Cmd2Api;

	//报文命令字与报文处理成员函数映射表
	static Cmd2Api m_Cmd2Api[];

	//成交单流水*****************************************************************************************
	int onRecvRtnSpotMatch(CBroadcastPacket& pkt);             //Spot
	int onRecvRtnForwardMatch(CBroadcastPacket& pkt);          //Forward
	int onRecvRtnDeferMatch(CBroadcastPacket& pkt);            //Defer
	int onRecvRtnDeferDeliveryAppMatch(CBroadcastPacket& pkt); //DeferDeliveryApp（延期交收和中立仓都在这里处理？）
															   //报单流水****************************************************************************************
	int onRecvRtnSpotOrder(CBroadcastPacket& pkt);             //Spot
	int onRecvRspSpotOrder(CBroadcastPacket& pkt);
	int onRecvSpotOrder(CBroadcastPacket& pkt);                //
	int HandleSpotOrder(CBroadcastPacket& pkt, int iBroadcastID);

	int onRecvRtnForwardOrder(CBroadcastPacket& pkt);          //Forward
	int onRecvRspForwardOrder(CBroadcastPacket& pkt);
	int onRecvForwardOrder(CBroadcastPacket& pkt);             //
	int HandleForwardOrder(CBroadcastPacket& pkt, int iBroadcastID);

	int onRecvRtnDeferOrder(CBroadcastPacket& pkt);            //Defer
	int onRecvRspDeferOrder(CBroadcastPacket& pkt);
	int onRecvDeferOrder(CBroadcastPacket& pkt);            //
	int HandleDeferOrder(CBroadcastPacket& pkt, int iBroadcastID);

	// DeferDeliveryApp
	int onRecvRtnDeferDeliveryAppOrder(CBroadcastPacket& pkt);
	int onRecvRspDeferDeliveryAppOrder(CBroadcastPacket& pkt);
	int onRecvDeferDeliveryAppOrder(CBroadcastPacket& pkt);
	int HandleDeferDeliveryAppOrder(CBroadcastPacket& pkt, int iBroadcastID);

	int onRecvRtnMiddleAppOrder(CBroadcastPacket& pkt);        //MiddleApp
	int onRecvRspMiddleAppOrder(CBroadcastPacket& pkt);
	int onRecvMiddleAppOrder(CBroadcastPacket& pkt);           //
	int HandleMiddleAppOrder(CBroadcastPacket& pkt, int iBroadcastID);
	//撤单********************************************************************************************
	int onRecvRtnSpotOrderCancel(CBroadcastPacket& pkt);             //Spot
	int onRecvRspSpotOrderCancel(CBroadcastPacket& pkt);
	int HandleSpotOrderCancel(CBroadcastPacket& pkt, int iBroadcastID);

	int onRecvRtnForwardOrderCancel(CBroadcastPacket& pkt);          //Forward
	int onRecvRspForwardOrderCancel(CBroadcastPacket& pkt);
	int HandleForwardOrderCancel(CBroadcastPacket& pkt, int iBroadcastID);

	int onRecvRtnDeferOrderCancel(CBroadcastPacket& pkt);            //Defer
	int onRecvRspDeferOrderCancel(CBroadcastPacket& pkt);
	int HandleDeferOrderCancel(CBroadcastPacket& pkt, int iBroadcastID);

	int onRecvRtnDeferDeliveryAppOrderCancel(CBroadcastPacket& pkt); //DeferDeliveryApp
	int onRecvRspDeferDeliveryAppOrderCancel(CBroadcastPacket& pkt);
	int HandleDeferDeliveryAppOrderCancel(CBroadcastPacket& pkt, int iBroadcastID);

	int onRecvRtnMiddleAppOrderCancel(CBroadcastPacket& pkt);        //MiddleApp
	int onRecvRspMiddleAppOrderCancel(CBroadcastPacket& pkt);
	int HandleMiddleAppOrderCancel(CBroadcastPacket& pkt, int iBroadcastID);
	//************************************************************************************************
	int onNewBourseBulletin(CBroadcastPacket& pkt); //弹出公告，制作公告窗口
	int onNewMemberBulletin(CBroadcastPacket& pkt); //弹出公告
	int onRiskNotify(CBroadcastPacket& pkt);        //弹出风险信息
	int onSysInit(CBroadcastPacket& pkt);           //获取最后一个交易日，清除成交和报单流水，弹出日结单结算信息
	int onSysStatChange(CBroadcastPacket& pkt);     //
	int onCustRiskDegreeChange(CBroadcastPacket& pkt); //

	int onSessionKey(CBroadcastPacket& pkt);        //在二级系统交易端没找到，暂时不管
	int onForceLogout(CBroadcastPacket& pkt);       //弹出警示信息，关闭当前窗口
	int onBaseTableUpdate(CBroadcastPacket& pkt);   //没找到
	int onAcctCapitalAccess(CBroadcastPacket& pkt); //????????????????????????????
	int onCustInfoChange(CBroadcastPacket& pkt);    //?????????????????????????????
													//StateUpdate*****************************************************************************************
	int onRecvRtnSpotInstStateUpdate(CBroadcastPacket& pkt);
	int onRecvRtnForwardInstStateUpdate(CBroadcastPacket& pkt);
	int onRecvRtnDeferInstStateUpdate(CBroadcastPacket& pkt);

	// 行情状态改变的时候统一调用的函数
	int DealRecvInstStateUpdate(CBroadcastPacket& pkt, const int &iBroadcastID);

	int onRecvRtnSpotMarketStateUpdate(CBroadcastPacket& pkt);
	int onRecvRtnForwardMarketStateUpdate(CBroadcastPacket& pkt);
	int onRecvRtnDeferMarketStateUpdate(CBroadcastPacket& pkt);

	//行情************************************************************************************************
	int onRecvSpotQuotation(CBroadcastPacket& pkt);
	int onRecvForwardQuotation(CBroadcastPacket& pkt);
	int onRecvDeferQuotation(CBroadcastPacket& pkt);
	int onRecvDeferDeliveryQuotation(CBroadcastPacket& pkt);

	int onRecvWindVaneQuotation(CBroadcastPacket& pkt);

	int onRecvRtnFutureQuotation(CBroadcastPacket& pkt);
public:

	int Reigister(mainWindow* pkt);


	// 消息发布器table
	typedef QMap<int, CBroadcaster> QMapBDR;
	QMapBDR m_mapBdr;

	void addStrategyConditionOrder(QString && id, strategy_conditionOrder &cond);
	void runStrategyConditionOrder(QString && id);
	void stopStrategyConditionOrder(QString && id);
	void deleteStrategyConditionOrder(QString && id);

	//QString getT1ProdID(QString && id);
	//QString getT2ProdID(QString && id);

	void runAllStrategyConditionOrder();

	//套利-条件单
	QMap<QString, strategy_conditionOrder >mapStrategyConditionOrder;

	QMap<QString, strategy_conditionOrder >mapStrategyRunning;//运行状态的
	QMap<QString, strategy_conditionOrder >mapStrategyStopping;//停止状态的

	void LoginCTP(const char *user, const char*pswd, const char *broker);
private:
	CConfigImpl*		    m_pConfig;
	bool                    m_bLoginSucceed;

private:
	CConnectPointSync* FindRoute(const QString &sCmdID);

	const QUOTATION* Translate(CBroadcastPacket& oPktSrc);
	const QUOTATION* TranslateQH(CBroadcastPacket& oPktSrc);
	int TranslateUnzipPacket(CBroadcastPacket& oPktSrc, QUOTATION& stQuotation);
	int TranslateZipPacket(CBroadcastPacket& oPktSrc, QUOTATION& stQuotation);
	int TranslateQHPacket(CBroadcastPacket& oPktSrc, QUOTATION& stQuotation);
	int DealRecvQuotation(CBroadcastPacket& pkt, const int& iBroadcastID, bool bPosi_PL = false);
	bool CheckQt(const QUOTATION& stQuotation);
	inline bool AddPrice(QVector<unsigned int> &vPrice, unsigned int dPrice);
	//报文命令字与对应路由接口映射结构
	typedef struct tagIfRouterCfg
	{
		QString sCmdID;
		unsigned long ulIfTo;
		CConnectPointSync* pCp;
	}IfRouterCfg;

	//报文命令字与对应路由接口映射关系配置表
	static IfRouterCfg m_tblIfRouterCfg[];

	//
	QVector<QString> m_vInvalidLocalOrderNo;

	void SwitchServer();

public:

	//int  setMainwnd(mainWindow* wnd);

	//
	bool CheckRspCode(const QString &strCode, QString &strMsg, bool bShowMsg = true);

	QString TranslateIDToValue(const QString &sTitleID, const QString &sValue);

	bool IsFloatTitle(const QString &sTitleID);

	// 
	bool InitEx();

	// 获取认证需要的ip和端口
	bool GetAuthenticateInfo(QString &sIP, QString &sPort);

	int TranslateZipPacketTest();

	void ClearHistoryInfo(void);

	// 判断客户今天是否有过有效的交易
	bool HasValidTrade(void);

	void ClearCusInfo(void);

	void RecordVersionInfo(QString csServerVersion);

	void IniQuotationInsID();


	mainWindow *   m_hMain;


	/*int LoginEx();*/

	// 临界区
	//	CRITICAL_SECTION m_criticalSectionBroadCast;
	QMutex m_criticalSectionBroadCast;//kenny  2017/11/16

	QMutex m_BroadCast_qh;//kenny  2017/11/16
private:
	// 唤起升级程序
	bool CallUpdateExe(void) const;

	QMutex	    m_csSendPkt;
	int           m_nTimeOut; // 短连接默认超时时间

							  // 记录服务器信息
	void SetServerList(HashtableMsg &htm_server_list);
};


extern CTraderCpMgr  g_TraderCpMgr;
#endif