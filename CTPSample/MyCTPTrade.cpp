#include "MyCTPTrade.h"
#include "MyCTPQuote.h"


#include "strutils.h"
#include <filesystem>
//#include <string>

MyCTPTrade::MyCTPTrade()
{
	m_RecvSpi = new MyRecv();
	m_RecvSpi->Bind(this);

	m_userLoginInfo = nullptr;
	m_bQryIns       = false;// 是否正在查询合约，如果正在查询，行情服务就不自己主动去订阅行情，因为查询合约完成后，会有订阅行情的动作
	m_bQryInsOK     = false;// 是否查询合约成功，成功订阅行情的动作

	m_reqApi        = nullptr;
	m_bLoginSuccessed = false;

	//日志初始化
	m_pConfig = new CConfigImpl();

}

MyCTPTrade::~MyCTPTrade()
{
	m_reqApi->Join();

	if (m_userLoginInfo != nullptr)
		delete m_userLoginInfo;

	if (m_RecvSpi != nullptr)
		delete m_RecvSpi;

	if (0 != m_pConfig)
	{
		delete m_pConfig;
		m_pConfig = nullptr;
	}

}

void    MyCTPTrade::setMainWidget(void *widget)
{
	mainHandle = widget;
}

void MyCTPTrade::setLog(const string&  str)
{
	CRLog(E_DEBUG, str.c_str());
}
///1.回报-连接成功
void MyCTPTrade::OnFrontConnected()
{
	m_reqApi->ReqUserLogin(m_userLoginInfo,m_RecvSpi->GetRequestId());


	setLog("Trade SDK: OnFrontConnected();");
}
///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void MyCTPTrade::OnFrontDisconnected(int nReason)
{

	setLog("Trade SDK: OnFrontDisconnected();");

}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void MyCTPTrade::OnHeartBeatWarning(int nTimeLapse)
{

	setLog("Trade SDK: OnHeartBeatWarning();");

}

///登录请求响应
void MyCTPTrade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	string  str = ("Trade SDK: OnRspUserLogin();");

	if ( pRspInfo->ErrorID == 0)		//登录成功，则进行行情订阅
	{
		m_TradingDay = pRspUserLogin->TradingDay;
		m_session = pRspUserLogin->SessionID;
		m_frontId  = pRspUserLogin->FrontID;
		m_serverOrderRef = pRspUserLogin->MaxOrderRef;

		///请求查询所有合约
		CThostFtdcQryInstrumentField req;
		memset(&req, 0, sizeof(req));
		strcpy_s(req.InstrumentID, "");//为空表示查询所有合约


		// 查询合约，第一个参数为null，表示查询所有的合约
		if(m_reqApi != nullptr)
			m_reqApi->ReqQryInstrument(&req, m_RecvSpi->GetRequestId());

		m_bLoginSuccessed = true;
		setLog(str + " Login Successful ");
	}
	else
	{
		string strTmp = str + " Error msg : ";
		strTmp  +=  pRspInfo->ErrorMsg;
		setLog(strTmp);
	}

}

///fanhui 合约信息
void MyCTPTrade::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	string strLog("Trade SDK: OnRspQryInstrument()");

	lock_guard<mutex> lock(m_getInstru);
	if (pRspInfo && pRspInfo->ErrorID)
	{
		atomic_exchange(&m_bQryIns, false);
		//接收失败
	}
	else if (pInstrument)
	{
		// 保存合约
		CThostFtdcInstrumentField instrument;
		memcpy_s(&instrument, sizeof(CThostFtdcInstrumentField), pInstrument, sizeof(CThostFtdcInstrumentField));

		// 过滤掉无效行情efp、sp*、IF等
		string strTmp(instrument.InstrumentID);

		
		int emitFlag = 0;
		string str = instrument.InstrumentID;

		if (strutils::trim(strTmp).size() >= 9) //暂时先去掉一些奇怪品种 如 SR811P6300
			emitFlag = 1;

		if (strTmp.compare("EFP") == 0 || strTmp.compare("SP") == 0 || strTmp.compare("IPS") == 0 || emitFlag == 1)
		{
			//日志记录
			setLog(strLog + "收到无效合约");
		}
		else
		{
			if (strlen(pInstrument->InstrumentID) > 0)
			{
				m_mapInstrument[pInstrument->InstrumentID] = instrument;

				if (strlen(pInstrument->ExchangeID) > 0)
				{
					string	strSecurityId = pInstrument->InstrumentID;
					//wstring	wstrBlockId = GetContrPrefix(strSecurityId);
					//m_mapInstrument2[CA2T(pInstrument->ExchangeID).m_psz][wstrBlockId][CA2T(pInstrument->InstrumentID).m_psz] = instrument;
				}


			}
			
			//日志记录
			setLog(strLog + "Instrument ID :"+strTmp);

		}

		// 查询完成通知行情服务
		if (bIsLast)
		{
			atomic_exchange(&m_bQryInsOK, true);
			atomic_exchange(&m_bQryIns, false);

			//wstring strLog = CStringUtils::Format(_T("%s合约数：(%d)"), LOG_CTP_RECV_INSTRUMENT_OK, m_mapInstrument.size());
			string strNum = strutils::intstr(m_mapInstrument.size());
			setLog(strLog + "Instrument num Totols :" + strNum);

			MyCTPQuote::Instance()->FinishQryInstrument();
		}
	}
	else
	{
		//_WRITELOG(_T("CTP交易：OnRspQryInvestor收到错误数据\r\n"));
	}

}

///登出请求响应
void MyCTPTrade::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void MyCTPTrade::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//
}

void  MyCTPTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	setLog("Trade SDK: OnRspError();");
}



MyCTPTrade * MyCTPTrade::Instance()
{
	static  MyCTPTrade  trade;
	return  &trade;
}
/////////////////////////////////////////////////////////////////////////
void MyCTPTrade::Init(const char  *User,const char  *pswd ,const char  *broker, const char *pszAddress)
{
	m_bQryIns   = false;// 是否正在查询合约，如果正在查询，行情服务就不自己主动去订阅行情，因为查询合约完成后，会有订阅行情的动作
	m_bQryInsOK = false;// 是否查询合约成功，成功订阅行情的动作


	if(m_userLoginInfo == nullptr)
		m_userLoginInfo = new CThostFtdcReqUserLoginField();

	strcpy(m_userLoginInfo->BrokerID, broker);
	strcpy(m_userLoginInfo->UserID, User);
	strcpy(m_userLoginInfo->Password, pswd);

	//当前路径
	std::tr2::sys::path path1  = std::tr2::sys::current_path();
	path1.append("/CTP/TradeCon");
	//实例化
	m_reqApi  = CThostFtdcTraderApi::CreateFtdcTraderApi(path1.string().c_str());
	//注册事件
	m_reqApi->RegisterSpi(m_RecvSpi);
	//注册前置机
	m_reqApi->RegisterFront((char *)pszAddress);// (char *)"tcp://27.17.62.149:40205");	//模拟
	m_reqApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	m_reqApi->SubscribePublicTopic(THOST_TERT_QUICK);
	m_reqApi->Init();
	//m_reqApi->Join();

	//初始化配置，读出日志地址
	auto   path = std::tr2::sys::current_path();
	if (m_pConfig->Load(path.string() + "\\config.cfg") != 0)
		return;

	// 初始化日志，对应的动作有，如果关于日志的配置不存在，则要初始化
	if (CLogger::Instance()->Initial(m_pConfig->GetProperties("logger")) != 0)
		return;

	// 启动日志
	if (CLogger::Instance()->Start() != 0)
		return;

	CRLog(E_DEBUG, "CTP交易模块初始化成功");
}


map<string, CThostFtdcInstrumentField>&	 MyCTPTrade::GetMapInstrument()
{
	//lock_guard<mutex> lock(m_getInstru);
	return m_mapInstrument;
}