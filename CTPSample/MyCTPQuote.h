#pragma once

#include "ctpbase.h"
#include <vector>
#include <thread>
#include <map>
#include "CTP\ThostFtdcMdApi.h"
#include <mutex>

using namespace std;

//#include "PairPacket.h"
#include "BroadcastPacket.h"
#include "ConfigImpl.h"

class  CPacketReceiverQH
{
public:
	virtual void Receive(CBroadcastPacket &pkt) = 0;
};


//定义一个管理器，管理行情接受
class   CTPSAMPLE_EXPORT MyCTPQuote
{
	//嵌入行情回报类
	class  MyRecv : public  CThostFtdcMdSpi
	{
	public:
		MyRecv():m_Parent(nullptr) {};
		~MyRecv() {};
		void Bind(MyCTPQuote  *Parent) { m_Parent = Parent; }
	public:
		///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
		virtual void OnFrontConnected() { m_Parent->OnFrontConnected(); }

		///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
		///@param nReason 错误原因
		///        0x1001 网络读失败
		///        0x1002 网络写失败
		///        0x2001 接收心跳超时
		///        0x2002 发送心跳失败
		///        0x2003 收到错误报文
		virtual void OnFrontDisconnected(int nReason) { m_Parent->OnFrontDisconnected(nReason); }

		///心跳超时警告。当长时间未收到报文时，该方法被调用。
		///@param nTimeLapse 距离上次接收报文的时间
		virtual void OnHeartBeatWarning(int nTimeLapse) { m_Parent->OnHeartBeatWarning(nTimeLapse); }

		///登录请求响应
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUserLogin(pRspUserLogin, pRspInfo, nRequestID, bIsLast); }

		///登出请求响应
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUserLogout(pUserLogout, pRspInfo,  nRequestID,  bIsLast); }

		///错误应答
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspError(pRspInfo, nRequestID,  bIsLast); }

		///订阅行情应答
		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspSubMarketData(pSpecificInstrument, pRspInfo, nRequestID, bIsLast); }

		///取消订阅行情应答
		virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUnSubMarketData(pSpecificInstrument, pRspInfo, nRequestID,  bIsLast); }

		///订阅询价应答
		virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspSubForQuoteRsp(pSpecificInstrument, pRspInfo,  nRequestID,  bIsLast); }

		///取消订阅询价应答
		virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) { m_Parent->OnRspUnSubForQuoteRsp(pSpecificInstrument, pRspInfo,  nRequestID,  bIsLast); }

		///深度行情通知
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) { m_Parent->OnRtnDepthMarketData(pDepthMarketData); }

		///询价通知
		virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) { m_Parent->OnRtnForQuoteRsp(pForQuoteRsp); }

	public:

		MyCTPQuote  *m_Parent;
	};

	//thread
public:
	MyCTPQuote( );
	~MyCTPQuote();

	void    setMainWidget(void *widget);
	// 订阅广播报文
	void Subscribe(CPacketReceiverQH *pPacketReceiver);

	// 取消广播报文订阅
	void Unsubscribe(CPacketReceiverQH *pPacketReceiver);


	static  MyCTPQuote *Instance();

	void  Init(const char  *User, const  char  *pswd, const  char  *broker, const char *pszAddress);
	void  FinishQryInstrument();

	void  setLog(const string&  str);
	bool      m_bLoginSuccessed;
public:
	///////////////////////////////////////请求类函数,提供主要逻辑，供外部使用////////////////////////////////////////////////////////////////////////////
	///用户登录请求
	int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) 
	{ 
		return m_reqApi->ReqUserLogin(pReqUserLoginField, nRequestID	);
	}

	///登出请求
	int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) { return m_reqApi->ReqUserLogout(pUserLogout, nRequestID); }

	///获取当前交易日
	///@retrun 获取到的交易日
	///@remark 只有登录成功后,才能得到正确的交易日
	const char *GetTradingDay() { return m_reqApi->GetTradingDay(); }
	///订阅行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	int SubscribeMarketData(char *ppInstrumentID[], int nCount) { return m_reqApi->SubscribeMarketData(ppInstrumentID, nCount); }

	///退订行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	int UnSubscribeMarketData(char *ppInstrumentID[], int nCount) { return m_reqApi->UnSubscribeMarketData(ppInstrumentID, nCount); }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


public:
	///////////////////////////////////////回报类函数，收到数据，更新本地行情信息////////////////////////////////////////////////////////////////////////////


	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	void OnHeartBeatWarning(int nTimeLapse);

	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅询价应答
	void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅询价应答
	void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///询价通知
	void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	CThostFtdcMdApi* m_reqApi;
	MyRecv         *m_RecvSpi;
	map<string /*InstrumentID*/, CThostFtdcInstrumentField>					   m_mapInstrument;//合约ID，合约信息
	map<string /*InstrumentID*/,CThostFtdcDepthMarketDataField /*Quotation*/>  m_mapInstrumentQuote;//保存  合约ID-最新行情  对,合约ID唯一。
	vector<string /*InstrumentID*/> m_vecInstrumentId;//合约ID
	int         requestID;
	string   m_TradingDay;

	int    m_session ;
	int    m_frontId ;
	string  OrderRef ;


	mutex m_mutex;
	///用户登录信息
	CThostFtdcReqUserLoginField  *m_userLoginInfo;
	bool m_bQryInstrumentOK;

	void  *mainHandle;

	//用于给上层订阅
	typedef vector<CPacketReceiverQH*> VPKTRECEIVER;
	VPKTRECEIVER            m_vPketReceiver;


	//CConfigImpl*	 m_pConfig;
	
};
