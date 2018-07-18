#include "MyCTPQuote.h"
#include "MyCTPTrade.h"
#include <atomic>
#include "BroadcastPacket.h"

#include <filesystem>

MyCTPQuote::MyCTPQuote( )
{
	m_RecvSpi = new MyRecv();
	m_RecvSpi->Bind(this);

	requestID = 0;
	m_userLoginInfo = nullptr;

	m_bLoginSuccessed = false;
	//日志初始化
	//m_pConfig = new CConfigImpl();
}

MyCTPQuote::~MyCTPQuote()
{
	m_reqApi->Join();//2-180416

	if (m_userLoginInfo != nullptr)
		delete m_userLoginInfo;

	if (m_RecvSpi != nullptr)
		delete m_RecvSpi;

	//if (0 != m_pConfig)
	//{
	//	delete m_pConfig;
	//	m_pConfig = nullptr;
	//}
}

void MyCTPQuote::setLog(const string&  str)
{
	CRLog(E_DEBUG, str.c_str());
}

void    MyCTPQuote::setMainWidget(void *widget)
{
	mainHandle = widget;
}

  MyCTPQuote *MyCTPQuote::Instance()
{
	static MyCTPQuote  quote;
	return  &quote;
}
///1.回报-连接成功
void MyCTPQuote::OnFrontConnected()
{
	//ctpInit
	
	++requestID;
	m_reqApi->ReqUserLogin(m_userLoginInfo, requestID);

	setLog("Quote SDK: OnFrontConnected();");

}
///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void MyCTPQuote::OnFrontDisconnected(int nReason)
{
	setLog("Trade SDK: OnFrontDisconnected();");
	m_bLoginSuccessed = false;

}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void MyCTPQuote::OnHeartBeatWarning(int nTimeLapse)
{
	setLog("Trade SDK: OnHeartBeatWarning();");

}

///登录请求响应
void MyCTPQuote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	string str  = ("Quote SDK: OnRspUserLogin();");


	if ( pRspInfo->ErrorID == 0)		//登录成功，则进行行情订阅
	{
		m_TradingDay = pRspUserLogin->TradingDay;
		m_session = pRspUserLogin->SessionID;
		m_frontId  = pRspUserLogin->FrontID;
		OrderRef = pRspUserLogin->MaxOrderRef;

		int Qry   = atomic_exchange(&MyCTPTrade::Instance()->m_bQryIns, false);
		int QryOK = atomic_exchange(&MyCTPTrade::Instance()->m_bQryInsOK, true);

		if(!Qry && QryOK)
		  FinishQryInstrument();

		str += " Login Successful! ";
		setLog(str);

		m_bLoginSuccessed = true;
	}
	else
	{
		string strTmp = str + " Error msg : ";
		strTmp += pRspInfo->ErrorMsg;
		setLog(pRspInfo->ErrorMsg);
	}

}

///登出请求响应
void MyCTPQuote::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///错误应答
void MyCTPQuote::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///订阅行情应答
void MyCTPQuote::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


}

///取消订阅行情应答
void MyCTPQuote::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///订阅询价应答
void MyCTPQuote::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///取消订阅询价应答
void MyCTPQuote::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}


CBroadcastPacket   pack;
///深度行情通知
///每秒两次行情
void MyCTPQuote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (pDepthMarketData->OpenPrice >= LDBL_MAX)
		pDepthMarketData->OpenPrice = 0;

	if (pDepthMarketData->HighestPrice >= LDBL_MAX)
		pDepthMarketData->HighestPrice = 0;

	if (pDepthMarketData->LowestPrice >= LDBL_MAX)
		pDepthMarketData->LowestPrice = 0;

	if (pDepthMarketData->ClosePrice >= LDBL_MAX)
		pDepthMarketData->ClosePrice = 0;

	if (pDepthMarketData->PreClosePrice >= LDBL_MAX)
		pDepthMarketData->PreClosePrice = 0;

	string str = "Quote SDK 接收行情信息:InstrumentID:";
	str  += pDepthMarketData->InstrumentID ;
	str += " TradingDay:";   str += pDepthMarketData->TradingDay;
	str += " LastPrice:"; str += to_string(pDepthMarketData->LastPrice);
	str += " Volume:"; str += to_string(pDepthMarketData->Volume);
	str += " OpenPrice:"; str += to_string(pDepthMarketData->OpenPrice);
	str += " HighestPrice:"; str += to_string(pDepthMarketData->HighestPrice);
	str += " LowestPrice:";  str += to_string(pDepthMarketData->LowestPrice);
	str += " ClosePrice:"; str += to_string(pDepthMarketData->ClosePrice);
	str += " PreClosePrice:"; str += to_string(pDepthMarketData->PreClosePrice);
	str += " UpdateTime:"; str += pDepthMarketData->UpdateTime;
	CRLog(E_APPINFO, str.c_str());
	//本地保存
	//转发到界面
	//CThostFtdcDepthMarketDataField  DepthMarketData;
	//memcpy_s(&DepthMarketData, sizeof(DepthMarketData) ,pDepthMarketData , sizeof(CThostFtdcDepthMarketDataField));
	//m_mapInstrumentQuote[pDepthMarketData->InstrumentID] = DepthMarketData;
	m_mapInstrumentQuote[pDepthMarketData->InstrumentID] = *pDepthMarketData;

	pack.SetParameter("instID", pDepthMarketData->InstrumentID);
	pack.SetParameter("quoteDate", pDepthMarketData->TradingDay);
	pack.SetParameter("open", to_string(pDepthMarketData->OpenPrice));
	pack.SetParameter("high", to_string( pDepthMarketData->HighestPrice));
	pack.SetParameter("low", to_string(pDepthMarketData->LowestPrice));
	pack.SetParameter("close", to_string(pDepthMarketData->ClosePrice));
	pack.SetParameter("lastClose", to_string(pDepthMarketData->PreClosePrice));
	pack.SetParameter("highLimit", to_string(pDepthMarketData->UpperLimitPrice));
	pack.SetParameter("lowLimit", to_string(pDepthMarketData->LowerLimitPrice));
	pack.SetParameter("ask1", to_string(pDepthMarketData->AskPrice1));
	pack.SetParameter("askLot1", to_string(pDepthMarketData->AskVolume1));
	pack.SetParameter("bid1", to_string(pDepthMarketData->BidPrice1));
	pack.SetParameter("bidLot1", to_string(pDepthMarketData->BidVolume1));
	pack.SetParameter("last", to_string(pDepthMarketData->LastPrice));
	pack.SetParameter("volume", to_string( pDepthMarketData->Volume));
	pack.SetParameter("quoteTime", pDepthMarketData->UpdateTime);


	//给上层转发
	// 广播报文
	for (VPKTRECEIVER::iterator it = m_vPketReceiver.begin(); it < m_vPketReceiver.end(); it++)
	{
		(*it)->Receive(pack);
	}

	//((TradeManager *)mainHandle)->setQuotation(DepthMarketData);

	//QParamEvent * msgQuote = new QParamEvent(WM_RECV_QUOTATION);
	//msgQuote->setQuotation(pDepthMarketData);
	//QApplication::postEvent(mainwiget, msgQuote);

}

///询价通知
void MyCTPQuote::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{

}


/////////////////////////////////////////////////////////////////////////
void MyCTPQuote::Init(const char  *User,const char  *pswd ,const char  *broker, const char *pszAddress)
{
	if(m_userLoginInfo == nullptr)
		m_userLoginInfo = new CThostFtdcReqUserLoginField();

	strcpy(m_userLoginInfo->BrokerID, broker);
	strcpy(m_userLoginInfo->UserID, User);
	strcpy(m_userLoginInfo->Password, pswd);

	//当前路径
	std::tr2::sys::path path = std::tr2::sys::current_path();
	path.append("/CTP/MdCon");
	//实例化
	m_reqApi = CThostFtdcMdApi::CreateFtdcMdApi(path.string().c_str());
	//注册事件
	m_reqApi->RegisterSpi(m_RecvSpi);
	//注册前置机
	m_reqApi->RegisterFront((char *)pszAddress);// (char *)"tcp://27.17.62.149:40205");	//模拟

	//注册前置机
	m_reqApi->RegisterFront((char *)pszAddress);//再次运行，切换地址

	m_reqApi->Init();
	//m_reqApi->Join();

	CRLog(E_DEBUG, "CTP行情模块初始化成功");

}

#define MAX_INSTRUMENT_NUM 1024
void  MyCTPQuote::FinishQryInstrument()
{
	// 开始订阅行情
	// 查询行情,有限制数量
	char *ppInstrumentID[MAX_INSTRUMENT_NUM] = { 0 };

	lock_guard<mutex> lock(m_mutex);

	m_bQryInstrumentOK = true;	//查询全部合约成功标识
	//调用
	m_mapInstrument = MyCTPTrade::Instance()->GetMapInstrument();

	// 清除多余的请求数据
	while (m_mapInstrument.size() > MAX_INSTRUMENT_NUM)
	{
		m_mapInstrument.erase(m_mapInstrument.end());
	}

	int nInstrumentCout = 0;
	//auto mapInstrument2 = MyCTPTrader::GetInstance()->GetMapInstrument2();
	//CSampleManager::GetInstance()->CalcDYXY(mapInstrument2);


	for (auto it = m_mapInstrument.begin(); it != m_mapInstrument.end(); it++)
	{
		ppInstrumentID[nInstrumentCout++] = (char*)(it->first.c_str());
	}

	//wstring wstrInfo = CStringUtils::Format(L"收到CTP码表信息：市场数：%d，合约数：%d", (int)mapInstrument2.size(), nInstrumentCout);
	//统计，计算
	{
		//收到行情时间重置为0
		//COwner lock(m_mutex);
		//m_tmLastQuote = 0;
	}

	//ReadData();

	// 订阅行情
	SubscribeMarketData(ppInstrumentID, nInstrumentCout);

	setLog("Quote SDK: FinishQryInstrument()  and SubscribeMarketData();");
	
}


void MyCTPQuote::Subscribe(CPacketReceiverQH *pPacketReceiver)
{
	VPKTRECEIVER::iterator it = find(m_vPketReceiver.begin(), m_vPketReceiver.end(), pPacketReceiver);
	if (it == m_vPketReceiver.end())
	{
		m_vPketReceiver.push_back(pPacketReceiver);
	}
}

void MyCTPQuote::Unsubscribe(CPacketReceiverQH *pPacketReceiver)
{
	VPKTRECEIVER::iterator it = find(m_vPketReceiver.begin(), m_vPketReceiver.end(), pPacketReceiver);
	if (it != m_vPketReceiver.end())
	{
		m_vPketReceiver.erase(it);
	}
}