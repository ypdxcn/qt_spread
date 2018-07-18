
//#include "stdafx.h"
#include "TraderManager.h"
#include "EvDataImp.h"
#include "LxGlobalFun.h"


#define CHECK_MODULE if(m_hModule == NULL){ return -10000;}

eventHandler CTraderManager::s_handler={
	CTraderManager::OnSessionStatusChanged,
	CTraderManager::OnDataUpdate,
	CTraderManager::OnMessage
};

CTraderManager   *g_LxTranManager = CTraderManager::instance();


void CTraderManager::OnSessionStatusChanged(int sessId, __int64 cbUser, ACCOUNT_STATUS status)
{
	CTraderManager::instance()->FireSessionStatusChanged(sessId, cbUser, status);
}
void CTraderManager::OnDataUpdate(int sessId, eventData* data, __int64 param)
{
	CTraderManager::instance()->FireDataUpdate(sessId, data, param);
}
void CTraderManager::OnMessage(int sessId, MSG_TYPE mt, int num, const char* msg)
{
	CTraderManager::instance()->FireMessage(sessId, mt, num, msg);
}

CTraderManager::CTraderManager()
{

}


CTraderManager::~CTraderManager()
{
	if (m_hModule != NULL)
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}

}

int CTraderManager::LoadTraderDll(string dllname)
{
	int ret = 1;
	m_hModule = ::LoadLibraryA(dllname.c_str());
	if (m_hModule != NULL)
	{
		m_lpTradeName = (fnTrade_Name)::GetProcAddress(m_hModule, "Trade_Name");
		m_lpTradeInit = (fnTrade_Init)::GetProcAddress(m_hModule, "Trade_Init");
		m_lpTradeUninit = (fnTrade_Uninit)::GetProcAddress(m_hModule, "Trade_Uninit");
		m_lpTradeRecyle = (fnTrade_Recyle)::GetProcAddress(m_hModule, "Trade_Recyle");
		m_lpTradeCreateSession = (fnTrade_CreateSession)::GetProcAddress(m_hModule, "Trade_CreateSession");
		m_lpTradeSessionIDList = (fnTrade_SessionIDList)::GetProcAddress(m_hModule, "Trade_SessionIDList");
		m_lpTradeSessionStatus = (fnTrade_SessionStatus)::GetProcAddress(m_hModule, "Trade_SessionStatus");
		m_lpTradeSessionOpen = (fnTrade_SessionOpen)::GetProcAddress(m_hModule, "Trade_SessionOpen");
		m_lpTradeSessionClose = (fnTrade_SessionClose)::GetProcAddress(m_hModule, "Trade_SessionClose");
		m_lpTradSessionDel = (fnTrade_SessionDel)::GetProcAddress(m_hModule, "Trade_SessionDel");
		m_lpTradeSessionAccountInfo = (fnTrade_SessionAccountInfo)::GetProcAddress(m_hModule, "Trade_SessionAccountInfo");
		m_lpTradeSendOrder = (fnTrade_SendOrder)::GetProcAddress(m_hModule, "Trade_SendOrder");
		m_lpTradeCancelOrder = (fnTrade_CancelOrder)::GetProcAddress(m_hModule, "Trade_CancelOrder");
		m_lpTradeModifyOrder = (fnTrade_ModifyOrder)::GetProcAddress(m_hModule, "Trade_ModifyOrder");
		m_lpTradeQueryData = (fnTrade_QueryData)::GetProcAddress(m_hModule, "Trade_QueryData");
		

		if ( m_lpTradeName== 0 ||
			 m_lpTradeInit== 0 ||
			 m_lpTradeUninit== 0 ||
			 m_lpTradeRecyle== 0 ||
			 m_lpTradeCreateSession== 0 ||
			 m_lpTradeSessionIDList== 0 ||
			 m_lpTradeSessionStatus== 0 ||
			 m_lpTradeSessionOpen== 0 ||
			 m_lpTradeSessionClose== 0 ||
			 m_lpTradSessionDel== 0 ||
			 m_lpTradeSessionAccountInfo== 0 ||
			 m_lpTradeSendOrder== 0 ||
			 m_lpTradeCancelOrder== 0 ||
			 m_lpTradeModifyOrder== 0 ||
			 m_lpTradeQueryData== 0)
		{
			
			ret= -1;
		}else
			ret = m_lpTradeInit(&s_handler);
		
	}
	else
	{
		
		ret= -2;
	}

	if (ret<0 && m_hModule != NULL)
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
	
	return ret;
}

bool CTraderManager::RegHook(string hookname,ITraderHook * hook)
{
	std::lock_guard<std::mutex> lck(m_mtxHook);
	TraderHookMap::iterator it = m_mapHook.find(hookname);
	if (it!=m_mapHook.end())
	{
		return false;
	}
	m_mapHook[hookname] = hook;
	return true;
}
bool CTraderManager::UnRegHook(string hookname)
{
	std::lock_guard<std::mutex> lck(m_mtxHook);
	TraderHookMap::iterator it = m_mapHook.find(hookname);
	if (it != m_mapHook.end())
	{
		m_mapHook.erase(it);
		return true;
	}
	
	return false;
}

bool CTraderManager::RegHandler(string handlername,ITraderHandler * hanlder)
{
	std::lock_guard<std::mutex> lck(m_mtxHandler);

	TraderHandlerMap::iterator it = m_mapHandler.find(handlername);
	if (it != m_mapHandler.end())
	{
		return false;
	}

	m_mapHandler[handlername] = hanlder;
	return true;
}
bool CTraderManager::UnRegHandler(string handlername)
{
	std::lock_guard<std::mutex> lck(m_mtxHandler);
	TraderHandlerMap::iterator it = m_mapHandler.find(handlername);
	if (it!=m_mapHandler.end())
	{
		m_mapHandler.erase(it);
		return true;
	}
	return false;
}

bool CTraderManager::RegVHandler(string handlername, IVTraderHandler * hanlder)
{
	std::lock_guard<std::mutex> lck(m_mtxHandler);

	TraderVHandlerMap::iterator it = m_mapVHandler.find(handlername);
	if (it != m_mapVHandler.end())
	{
		return false;
	}

	m_mapVHandler[handlername] = hanlder;
	return true;
}
bool CTraderManager::UnRegVHandler(string handlername)
{
	std::lock_guard<std::mutex> lck(m_mtxHandler);
	TraderVHandlerMap::iterator it = m_mapVHandler.find(handlername);
	if (it != m_mapVHandler.end())
	{
		m_mapVHandler.erase(it);
		return true;
	}
	return false;
}

bool  CTraderManager::IsVirtualExchange(const string& accName)
{
	for (auto a : m_mapInvestor)
	{
		if (a.first == accName)
		{
			return  true;
		}
	}

	return  false;

}


int CTraderManager::CreateSession(string dllname, string accName, AccountInfo& account, __int64 cbUser)
{
	CHECK_MODULE;

	int sessid = -1;
	if (account.IsVirtualExchange >= 1)//虚拟交易登录
	{
		std::lock_guard<std::mutex> lck(m_mtxAcc);
		/*if (account.IsVerifyAccount <= 0)//非法模拟账户
		{
		return -1;
		}*/

		//生成虚拟会话Id
		sessid = CLxGlobalFun::HexStrToUINT64(CLxGlobalFun::Get16HexRandNumber(4));

		//成功建立session
		if (sessid >= 0)
		{
			//生成投资者信息
			m_mapVInvestor[accName] = account;
			strncpy_s(m_mapVInvestor[accName].Uid, account.Uid, sizeof(account.Uid));
			strncpy_s(m_mapVInvestor[accName].Psw, account.Psw, sizeof(account.Psw));
			strncpy_s(m_mapVInvestor[accName].broker.strBrokerNum, account.broker.strBrokerNum, sizeof(account.broker.strBrokerNum));
			strncpy_s(m_mapVInvestor[accName].InvestorName, account.InvestorName, sizeof(account.InvestorName));
			if (strlen(m_mapVInvestor[accName].InvestorName) <= 0)
			{
				//临时拷贝
				strcpy_s(m_mapVInvestor[accName].InvestorName, "模拟账户");

			}


			m_mapVAcc[accName] = sessid;

			//正式需要读取服务器的资金信息
			m_mapVAccCapital[accName].m_Available = 1000000;
			m_mapVAccCapital[accName].m_RealEquity = 1000000;
			m_mapVAccCapital[accName].m_StaticEquity = 1000000;
			m_mapVAccCapital[accName].m_FrozenCash = 0;
			m_mapVAccCapital[accName].m_PreBalance = 1000000;
			m_mapVAccCapital[accName].m_PreMargin = 1000000;


			strncpy_s(m_mapVAccCapital[accName].m_AcctInfo.Uid, account.Uid, sizeof(account.Uid));
			strncpy_s(m_mapVAccCapital[accName].m_AcctInfo.Psw, account.Psw, sizeof(account.Psw));
			strncpy_s(m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum, account.broker.strBrokerNum, sizeof(account.broker.strBrokerNum));
			strncpy_s(m_mapVAccCapital[accName].m_AcctInfo.InvestorName, m_mapVInvestor[accName].InvestorName, sizeof(m_mapVInvestor[accName].InvestorName));

			strncpy_s(m_mapVAccCapital[accName].m_MoneyKind, "RMB", sizeof("RMB"));
			strncpy_s(m_mapVAccCapital[accName].m_AcctInfo.Psw, account.Psw, sizeof(account.Psw));

		}
	}
	else
	{
		sessid = m_lpTradeCreateSession(account, cbUser);
		if (sessid >= 0)
		{
			std::lock_guard<std::mutex> lck(m_mtxAcc);
			m_mapAcc[accName] = sessid;
		}
	}


	return sessid;

}
int CTraderManager::SessionNameList(vector<string>& SessionNameList)
{
	CHECK_MODULE;

	vector<int> sessIDList;
	int ret = m_lpTradeSessionIDList(sessIDList);
	for (int i = 0; i < sessIDList.size(); i++)
	{
		SessionNameList.push_back(GetSessionName(sessIDList.at(i)));
	}
	return ret;
}
int CTraderManager::SessionStatus(const string& accName)
{
	CHECK_MODULE;

	return m_lpTradeSessionStatus(GetSessionID(accName));
}
int CTraderManager::SessionOpen(const string& accName)
{
	CHECK_MODULE;

	return m_lpTradeSessionOpen(GetSessionID(accName));
}
int CTraderManager::SessionClose(const string& accName)
{
	CHECK_MODULE;

	//return m_lpTradeSessionClose(GetSessionID(accName));

	int sessid = m_lpTradeSessionClose(GetSessionID(accName));
	if (sessid == 0)//0表示成功注销
	{
		std::lock_guard<std::mutex> lck(m_mtxAcc);
		//清除数据
		m_mapAccCapital.erase(m_mapAccCapital.begin(), m_mapAccCapital.end());
		vector<commissionOrder>().swap(m_mapComOrder[accName]);
		vector<positionOrder>().swap(m_mapPosiMatch[accName]);
		vector<positionOrder>().swap(m_mapPosiDetailMatch[accName]);
		vector<transDetail>().swap(m_mapTransMatch[accName]);
		
	}

	return sessid;

}
int CTraderManager::SessionDel(const string& accName)
{
	CHECK_MODULE;

	return m_lpTradSessionDel(GetSessionID(accName));
}
int CTraderManager::SessionAccountInfo(const string& accName, AccountInfo& account)
{
	CHECK_MODULE;

	return m_lpTradeSessionAccountInfo(GetSessionID(accName),account);
}

int CTraderManager::Recyle(eventData* data)
{
	CHECK_MODULE;
	m_lpTradeRecyle(data);
	return 1;
}

int CTraderManager::SendOrder(const string& accName, string strategy, orderReq& req)
{
	CHECK_MODULE;

	return m_lpTradeSendOrder(GetSessionID(accName), req);
}

int CTraderManager::CancelOrder(const string& accName, orderCancelReq& req)
{
	CHECK_MODULE;

	return m_lpTradeCancelOrder(GetSessionID(accName), req);
}

int CTraderManager::ModifyOrder(const string& accName, orderModifyReq& req)
{
	CHECK_MODULE;

	return m_lpTradeModifyOrder(GetSessionID(accName), req);
}

int CTraderManager::ModifyPosiPreOrder(const string& accName, orderModifyReq& req)
{
	std::lock_guard<std::mutex> lck(m_mtxAcc);
    //查找到该用户的持仓
	//同方向，同合约
	for (auto &it = m_mapPosiMatch[accName].begin(); it != m_mapPosiMatch[accName].end();)
	{
		if (strstr(it->m_InstrumentId, req.szInstrument) != NULL &&
			it->m_nBuySell == req.nBuySell)
		{
			//更新数据
			it->m_LastTradeDealPrice = req.lPrice;
			it->m_WincutPrice        = req.dTakeProfit;
			it->m_LosecutPrice       = req.dStoploss;
			it->m_TrigNumWin         = req.dTakeProfitVolume;
			it->m_TrigNumLose        = req.dStoplossVolume;
			return  1;
		}
		else
		{
			++it;
		}
	}

	return 0;
}



int CTraderManager::QueryData(const string& accName, eventData::EVENT_DATA_TYPE type, __int64 param, cbDataUpdate cbFunc)
{
	CHECK_MODULE;

	return m_lpTradeQueryData(GetSessionID(accName), type, param,cbFunc);
}

int CTraderManager::GetSessionID(const string& accName)
{
	std::lock_guard<std::mutex> lck(m_mtxAcc);
	AccountNameMap::iterator it = m_mapAcc.find(accName);
	if (it!=m_mapAcc.end())
	{
		return it->second;
	}
	return 0;
}
string CTraderManager::GetSessionName(int sessId)
{
	string strname;
	std::lock_guard<std::mutex> lck(m_mtxAcc);
	for each (auto  it in m_mapAcc)
	{
		if (it.second==sessId)
		{
			return it.first;
		}

	}
	return "";
}

void CTraderManager::FireSessionStatusChanged(int sessId, __int64 cbUser, ACCOUNT_STATUS status)
{
	string accName = GetSessionName(sessId);


	//std::lock_guard<std::mutex> lck(m_mtxAcc);
	std::unique_lock<std::mutex> my_lock(m_mtxAcc);
	//更新所有信息的账户名
		if (status == AS_DISCONNECTED)//如果断网
	{
		m_mapAccCapital.erase(accName);

		vector<commissionOrder>().swap(m_mapComOrder[accName]);
		vector<positionOrder>().swap(m_mapPosiMatch[accName]);
		vector<positionOrder>().swap(m_mapPosiDetailMatch[accName]);
		vector<transDetail>().swap(m_mapTransMatch[accName]);
		
		m_mapComOrder.erase(accName);
		m_mapPosiMatch.erase(accName);
		m_mapPosiDetailMatch.erase(accName);
		m_mapTransMatch.erase(accName);
	}
		if (accName != "" && (status >= AS_LOGIN) && (status != AS_CONNIMTS_UPDATE) && (status != AS_TRANS_UPDATE))//&& (status >= AS_POSITIONS_DETAIL_UPDATE))

		{
			int len = strlen(m_mapInvestor[accName].InvestorName);
			if (len >0)
			{
				//遍历账户
				for (auto & a : m_mapAccCapital)
				{
					if (a.first == accName)
					{
						a.second.m_AcctInfo.As = status;
						strncpy_s(a.second.m_InvestorName, m_mapInvestor[accName].InvestorName, len);
					}
				}


				//遍历委托
				for (auto& a : m_mapComOrder)
				{
					if (a.first == accName)
						for (auto &b : a.second)
							strncpy_s(b.m_InvestorName, m_mapInvestor[accName].InvestorName, len);

				}

				//遍历持仓
				for (auto& a : m_mapPosiMatch)
				{
					if (a.first == accName)
						for (auto &b : a.second)
							strncpy_s(b.m_InvestorName, m_mapInvestor[accName].InvestorName, len);
					
				}

				//遍历持仓明细
				for (auto& a : m_mapPosiDetailMatch)
				{
					if (a.first == accName)
						for (auto &b : a.second)
							strncpy_s(b.m_InvestorName, m_mapInvestor[accName].InvestorName, len);
				}

				//遍历成交明细
				for (auto& a : m_mapTransMatch)
				{
					if (a.first == accName)
						for (auto &b : a.second)
							strncpy_s(b.m_InvestorName, m_mapInvestor[accName].InvestorName, len);
				}
			}

				
		}

	
	my_lock.unlock();


	{
		std::lock_guard<std::mutex> lck(m_mtxHook);
		for_each(m_mapHook.begin(), m_mapHook.end(), [&](TraderHookMap::const_reference it){
			it.second->OnSessionStatusChanged(accName, cbUser, status);
		});
	}
	{
		std::lock_guard<std::mutex> lck(m_mtxHandler);
		for_each(m_mapHandler.begin(), m_mapHandler.end(), [&](TraderHandlerMap::const_reference it){
			it.second->OnSessionStatusChanged(accName, cbUser, status);
		});
	}

}
	

void CTraderManager::FireDataUpdate(int sessId, eventData* data, __int64 param)
{
	string accName = GetSessionName(sessId);

	//本地保留一份数据
	//std::lock_guard<std::mutex> lck(m_mtxAcc);
	std::unique_lock<std::mutex> my_lock(m_mtxAcc);
	switch (data->datatype)
	{
	case eventData::EDT_INSTRUMEN_INFO:
	{
		AddInstrumentInfo(accName,data);
	}
	break;
	case eventData::EDT_EXCHANGE_BULL:
	{
		AddExchangeBull(accName,data);	
	}
	break;
	case eventData::EDT_INVESTOR_INFO:
	{
		AddInvestorInfo(accName,data);
	}
	break;
	case eventData::EDT_ACC_DETAIL:
	case eventData::EDT_ACC_DETAIL_ARRAY:
	{
		AddAccountDetailInfo(accName, data);
	}
	break;
	case eventData::EDT_COMMISION:
	case eventData::EDT_COMMISION_ARRAY:
	{
		AddCommissionOrder(accName, data);
	}
	break;
	case eventData::EDT_POSITIONS:
	case eventData::EDT_POSITIONS_ARRAY:
	{
		AddPositionOrder(accName, data);

		//更新持仓，计算LX扩展字段值(根据合约和买卖方向)
		
		//统计【开仓价格】
		//UpdatePosiByCommiss(accName);
       //统计【持仓盈亏】
		UpdateAllByPosi();

	}
	break;
	case eventData::EDT_POSITIONS_DETAIL:
	case eventData::EDT_POSITIONS_DETAIL_ARRAY:
	{
		AddPositionOrderDetail(accName, data);
		//统计盈亏，胜率
		UpdateAccByPosiDetail(accName);
		//统计【开仓价格】
		UpdatePosiByPosiDetail(accName);
	}
	break;
	case eventData::EDT_TRANS_DETAIL:
	case eventData::EDT_TRANS_DETAIL_ARRAY:
	{

		AddTransDetail(accName, data);

		//持仓变动,更新持仓
		//1.找对应合约
		////统计【平仓盈亏】
		//UpdateAll();
		//统计盈亏，胜率
		UpdateAccByPosiDetail(accName);

	}
	break;

	}


	my_lock.unlock();

	EvDataReadWriteHolder evDataHolder = new CEvDataImp(data);
	{
		std::lock_guard<std::mutex> lck(m_mtxHook);
		for_each(m_mapHook.begin(), m_mapHook.end(), [&](TraderHookMap::const_reference it){
			it.second->OnDataUpdate(accName, evDataHolder);
		});
	}
	{
		std::lock_guard<std::mutex> lck(m_mtxHandler);
		for_each(m_mapHandler.begin(), m_mapHandler.end(), [&](TraderHandlerMap::const_reference it){
			it.second->OnDataUpdate(accName, (IEvData*)evDataHolder);
		});
	}
}

//添加一套
void CTraderManager::AddExchangeBull(const string& accName , const eventData *data)
{
	ExchangeBulletin  *pBull = data->data.exchangeBulletin[0];
	if (NULL == pBull)
		return;
	for (int i = 0; i < data->dataCount; i++)
	{
		pBull = data->data.exchangeBulletin[i];
		m_mapBull[accName] = *pBull;
	}

}

void CTraderManager::AddInvestorInfo(const string& accName, const eventData *data)
{
	AccountInfo  *pInvest = data->data.investor[0];
	if (NULL == pInvest)
		return;
	for (int i = 0; i < data->dataCount; i++)
	{
		pInvest = data->data.investor[i];
		m_mapInvestor[accName] = *pInvest;
	}
}

void CTraderManager::AddAccountDetailInfo(const string& accName, const eventData *data)
{
	accountDetail *pAccountDetail = data->data.details[0];
	if (NULL == pAccountDetail)
		return;
	for (int i = 0; i < data->dataCount; i++)
	{

		m_mapAccCapital[accName] = *(data->data.details[i]);
	}

}


void CTraderManager::AddInstrumentInfo(const string& accName, const eventData *data)
{
	InstrumentInfo  *pInstru = data->data.instrumentInfo[0];
	if (NULL == pInstru)
		return;
	for (int i = 0; i < data->dataCount; i++)
	{
		pInstru = data->data.instrumentInfo[i];
		m_mapInstrument[pInstru->InstrumentID] = *pInstru;
	}
}

void CTraderManager::AddCommissionOrder(const string& accName, const eventData *data)
{
	commissionOrder *pCommission = data->data.commissions[0];
	if (NULL == pCommission)
		return;
	for (int i = 0; i < data->dataCount; i++)
	{

		//更新状态
		pCommission = data->data.commissions[i];
		auto bFind = false;
		for (auto &a : m_mapComOrder[accName])
		{

			if ((a.m_state == pCommission->m_state) &&
				(a.m_SubmitStatus == pCommission->m_SubmitStatus) &&
				strcmp(a.m_InstrumentId, pCommission->m_InstrumentId) == 0
				&& strcmp(a.m_OrderLocalId, pCommission->m_OrderLocalId) == 0
				&& a.m_OrderDateTime == pCommission->m_OrderDateTime)
			{
				bFind = true;
				continue;
			}
		}

		if (bFind)
			continue;

		if (strlen(pCommission->m_OrderSystemId) > 0)
		{
			if (pCommission->m_state == tos_traded//【全部成交】
				|| pCommission->m_state == tos_ordered//已报单，【未成交】
				|| pCommission->m_state == tos_cancled)//【已撤单】
			{
				m_mapComOrder[accName].push_back(*pCommission);
			}
		}
		else
		{
			m_mapComOrder[accName].push_back(*pCommission);
		}

	}

}

void CTraderManager::AddPositionOrder(const string& accName, const eventData *data)
{
	positionOrder *pPositionOrder = data->data.posis[0];
	if (NULL == pPositionOrder)
		return;

	for (int i = 0; i < data->dataCount; i++)
	{
		pPositionOrder = data->data.posis[i];

		bool find = false;
		//删掉重复的
		for (auto it = m_mapPosiMatch[accName].begin(); it != m_mapPosiMatch[accName].end();)
		{
			if (strstr(it->m_InstrumentId, pPositionOrder->m_InstrumentId) != NULL &&
				it->m_nBuySell == pPositionOrder->m_nBuySell)
				//	it->m_nBuySell == pPositionOrder->m_nBuySell && it->m_nPosiDate == THOST_FTDC_POSI_HISTORY)
			{
				m_mapPosiMatch[accName].erase(it);

				break;
			}
			else
			{
				++it;
			}
		}

		m_mapPosiMatch[accName].push_back(*(data->data.posis[i]));
	}

}


void CTraderManager::AddPositionOrderDetail(const string& accName, const eventData *data)
{
	positionOrder *pPositionOrder = data->data.posisDetail[0];
	if (NULL == pPositionOrder)
		return;
	for (int i = 0; i < data->dataCount; i++)
	{
		//过滤重复
		pPositionOrder = data->data.posis[i];
		auto bFind = false;
		for (auto a : m_mapPosiDetailMatch[accName])
		{
			if (strcmp(a.m_TradeID, pPositionOrder->m_TradeID) == 0)
			{
				bFind = true;
				continue;
			}
		}

		if (bFind)
			continue;

		m_mapPosiDetailMatch[accName].push_back(*pPositionOrder);
	}

}


void CTraderManager::AddTransDetail(const string& accName, const eventData *data)
{
	transDetail *pTransDetail = data->data.transs[0];
	if (NULL == pTransDetail)
		return;

	//20170518
	//交易回报后，会查一次持仓，本地先清除
	//if (!m_mapPosiMatch[accName].empty())
	//{
	//	//vector<positionOrder>().swap(m_mapPosiMatch[accName]);
	//	m_mapPosiMatch[accName].clear();
	//}


	for (int i = 0; i < data->dataCount; i++)
	{
		pTransDetail = data->data.transs[i];
		auto bFind = false;
		for (auto a : m_mapTransMatch[accName])
		{
			if (strcmp(a.m_TradeCode, pTransDetail->m_TradeCode) == 0)
			{
				bFind = true;
				continue;
			}
		}

		if (bFind)
			continue;

		m_mapTransMatch[accName].push_back(*pTransDetail);
		//统计交易费用
		m_mapAccCapital[accName].m_Commissiontotal += pTransDetail->m_trade_fee;


		UpdatePosiByTransaction(accName, pTransDetail);
	}
}


void CTraderManager::UpdatePosiByTransaction(const string &accname, const transDetail *trans)
{
	//if (m_mapAccCapital[accname].m_AcctInfo.As >= AS_LOGIN)//登陆之后的增量单
	{

		double  nTotalPositionProfit = 0;//持仓盈亏  与行情有关

		for (auto &it = m_mapPosiMatch[accname].begin(); it != m_mapPosiMatch[accname].end(); ++it)
		{	
      //      if (it->m_nPosiDate == THOST_FTDC_POSI_HISTORY && it->m_YdPosition!= 0)
      //         it->m_PostionNum = it->m_YdPosition + it->m_TodayPosition;
			//相同合约,相同方向
			if (strstr(it->m_InstrumentId, trans->m_InstrumentId) != NULL 
				//&& it->m_PostionNum != 0
				&& it->m_nBuySell       == trans->m_nBuySell 
				&& trans->m_nOpenClose  == TOC_OPEN)
			{
				
				//不合并持仓，保持原始性，为了统计特色字段   kenny  20170518
				//更新持仓数据
				/*if (it->m_nBuySell == trans->m_nBuySell)
				{
					it->m_PostionNum += trans->m_tradeNum;
					if (trans->m_nIstoday)
						it->m_TodayPosition += trans->m_tradeNum;
					else
						it->m_YdPosition += trans->m_tradeNum;

				}
				else
				{
					it->m_PostionNum -= trans->m_tradeNum;
					if (trans->m_nIstoday)
						it->m_TodayPosition -= trans->m_tradeNum;
					else
						it->m_YdPosition -= trans->m_tradeNum;


				}*/

				//若开仓，更新[持仓]  开仓价格；
				//若平仓，更新[持仓]  止盈，止损价格；
				if (trans->m_nOpenClose == TOC_OPEN)//开仓
				{
					it->m_avgOpenPirce = trans->m_tradePrice;

				    //平仓，设置好止盈止损
					it->m_LastTradeDealPrice = trans->m_tradePrice;//持仓的止盈止损基准价
					it->m_WincutPrice  = trans->m_WincutPrice;
		     		it->m_TrigNumWin   = trans->m_TrigNumWin;
				    it->m_LosecutPrice = trans->m_LosecutPrice;
				    it->m_TrigNumLose  = trans->m_TrigNumLose;
                }

			}
			//nTotalPositionProfit += it->m_PositionProfit;//累加持仓盈亏

		}

	}

}

//根据持仓明细来更新持仓
void CTraderManager::UpdatePosiByPosiDetail(const string &accname)
{
	for (auto & posi : m_mapPosiMatch[accname])
	{

		//去委托里面找开仓价格
		for (auto posiDetail : m_mapPosiDetailMatch[accname])
		{
           // if (posi.m_nPosiDate == THOST_FTDC_POSI_HISTORY && posi.m_YdPosition!= 0)
			//    posi.m_PostionNum = posi.m_TodayPosition + posi.m_YdPosition;

			//计算盈亏次数
			if (strcmp(posi.m_InstrumentId, posiDetail.m_InstrumentId) == 0
								&& (posi.m_nBuySell == posiDetail.m_nBuySell)
								/*&& (posi.m_nPosiDate == posiDetail.m_nPosiDate)*/)
			{
				if (posiDetail.m_avgPirce  > 0)
					posi.m_avgPirce = posiDetail.m_avgPirce;
				if (posiDetail.m_avgOpenPirce > 0)
					posi.m_avgOpenPirce = posiDetail.m_avgOpenPirce;

				//if (posi.m_YdPosition >0)
				//{
				//	posi.m_avgPirce = 
				//}
				if (posi.m_avgPirce < 1)
				{
					if (posi.m_nPosiDate == THOST_FTDC_POSI_HISTORY)
						posi.m_avgPirce = posi.m_PreSettlementPrice;
					else
						posi.m_avgPirce = posi.m_SettlementPrice;
				}
			}


		}

	}

}


//根据历史成交的报单，获得持仓的开盘价格
void CTraderManager::UpdatePosiByCommiss(const string &accname)
{
	for (auto & posi : m_mapPosiMatch[accname])
	{
		//委托里面找开仓价格
		for (auto order : m_mapComOrder[accname])
		{
			if (strcmp(posi.m_InstrumentId, order.m_InstrumentId) == 0
				&& (posi.m_nBuySell == order.m_nBuySell)
				/*&& (posi.m_nPosiDate == posiDetail.m_nPosiDate)*/ && order.m_state == 5)
			{

				posi.m_avgOpenPirce = order.m_OrderPrice;//.m_avgOpenPirce;
				continue;
			}


		}

	}
}

void CTraderManager::UpdateAccByPosiDetail(const string& accName)
{
		int  nWinCount = 0;//盈利次数
		int  nLoseCount = 0;//亏损次数
		int  nBuyCount = 0; //做多次数
		int  nSellCount = 0; //做空次数
		double  nWinAmount = 0; //赚的钱
		double  nLoseAmount = 0; //亏的钱

		double  nTotalCloseProfit = 0;//平仓盈亏
		double  nTotalPositionProfit = 0;//累计持仓盈亏
		double  nTodayFee = 0;//当天手续费
		double  nTotalFee = 0;//手续费


		map<string, int>tmpMp;
		for (auto trans : m_mapTransMatch[accName])
		{
			//每条进行统计
			//MergePosiByTransaction(a.first, &trans);

			//统计交易品种数目
			tmpMp.insert(make_pair(trans.m_InstrumentId, 0));


			//去委托里面找开仓价格
			for (auto & posi : m_mapPosiDetailMatch[accName])
			{

				//计算字段
				if (posi.m_avgPirce < 1)
				{
					if (posi.m_nPosiDate == THOST_FTDC_POSI_HISTORY)
						posi.m_avgPirce = posi.m_PreSettlementPrice;
					else
						posi.m_avgPirce = posi.m_SettlementPrice;
				}
				posi.m_PostionNum = posi.m_YdPosition + posi.m_TodayPosition;

				//计算盈亏次数
				if (strcmp(trans.m_InstrumentId, posi.m_InstrumentId) == 0
					&& 
					((trans.m_nOpenClose == TOC_CLOSE_Today) 
					|| (trans.m_nOpenClose == TOC_Close_Yesterday)
					|| (trans.m_nOpenClose == TOC_CLOSE) ))

				{
					//价格相减，得出盈亏
					if (trans.m_nBuySell == TD_SELL /*&& posi.m_nBuySell == TD_BUY */)//做多
					{
						nBuyCount++;
						if ((trans.m_tradePrice - posi.m_avgOpenPirce) >0 )
							nWinCount++;
						else
							nLoseCount++;

					}
					else if (trans.m_nBuySell == TD_BUY /*&& posi.m_nBuySell == TD_SELL*/)//做空
					{
						nSellCount++;

						if ((trans.m_tradePrice - posi.m_avgOpenPirce) >0 )
							nWinCount++;
						else
							nLoseCount++;

					}
					break;
				}


				//nTotalPositionProfit += posi.m_PositionProfit;//累加持仓盈亏

			}



		}



		//更新账户总汇
		m_mapAccCapital[accName].m_nTradeInstruCount = tmpMp.size();//交易品种数

		//统计Lx扩展字段
		m_mapAccCapital[accName].m_WinProfitCount = nWinCount;
		m_mapAccCapital[accName].m_LoseProfitCount = nLoseCount;
		m_mapAccCapital[accName].m_BuyCount = nBuyCount;
		m_mapAccCapital[accName].m_SellCount = nSellCount;
		
		//计算胜率
		double  total = m_mapAccCapital[accName].m_WinProfitCount + m_mapAccCapital[accName].m_LoseProfitCount;
		if (total > 0)
			m_mapAccCapital[accName].m_WinRate = m_mapAccCapital[accName].m_WinProfitCount / total;

		//计算盈亏比
		if (m_mapAccCapital[accName].m_LoseProfitCount > 0)
			m_mapAccCapital[accName].m_ProfitLossRatio = m_mapAccCapital[accName].m_WinProfitCount / m_mapAccCapital[accName].m_LoseProfitCount;



	

}

void CTraderManager::UpdateAllByPosi()
{
	for (auto a : m_mapAccCapital)
	{
		//int  nWinCount = 0;//盈利次数
		//int  nLoseCount = 0;//亏损次数
		//int  nBuyCount = 0; //做多次数
		//int  nSellCount = 0; //做空次数
		//double  nWinAmount = 0; //赚的钱
		//double  nLoseAmount = 0; //亏的钱

		double  nTotalCloseProfit = 0;//平仓盈亏
		double  nTotalPositionProfit = 0;//累计持仓盈亏
		double  nTodayFee = 0;//当天手续费
		double  nTotalFee = 0;//手续费
		double  nTodayCloseWin = 0;//今日平仓盈亏

		map<string, int>tmpMp;
		for (auto trans : m_mapTransMatch[a.first])
		{
			//每条进行统计
			//MergePosiByTransaction(a.first, &trans);

			//统计交易品种数目
			tmpMp.insert(make_pair(trans.m_InstrumentId, 0));


			//去委托里面找开仓价格
			for (auto &posi : m_mapPosiMatch[a.first])//加& ，可修改
			{
             ///   if (posi.m_nPosiDate == THOST_FTDC_POSI_HISTORY && posi.m_YdPosition!= 0)
              //      posi.m_PostionNum = posi.m_YdPosition + posi.m_TodayPosition;
                if (strcmp(trans.m_InstrumentId, posi.m_InstrumentId) == 0
                    && ((trans.m_nOpenClose == TOC_OPEN) && (posi.m_nPosiDate == THOST_FTDC_POSI_TODAY)))
                {
                //  posi.m_PostionNum = posi.m_YdPosition + posi.m_TodayPosition;
                    posi.m_WincutPrice = trans.m_WincutPrice;
                    posi.m_TrigNumWin = trans.m_TrigNumWin;
                    posi.m_LosecutPrice = trans.m_LosecutPrice;
                    posi.m_TrigNumLose = trans.m_TrigNumLose;
                    posi.m_LastTradeDealPrice = trans.m_tradePrice;
                 }

                //计算盈亏次数
				if (strcmp(trans.m_InstrumentId, posi.m_InstrumentId) == 0
					&& (((trans.m_nOpenClose == TOC_CLOSE_Today) && (posi.m_nPosiDate == THOST_FTDC_POSI_TODAY))
					|| ((trans.m_nOpenClose == TOC_Close_Yesterday) && (posi.m_nPosiDate == THOST_FTDC_POSI_HISTORY)))
					//&& (posi.m_PostionNum == 0)
					/*&& (((posi.m_nPosiDirection == THOST_FTDC_Long) && (trans.m_nBuySell == TD_SELL))
					|| ((posi.m_nPosiDirection == THOST_FTDC_Short) && (trans.m_nBuySell == TD_BUY)))*/)
				{
				
					if (posi.m_PostionNum == 0)
					{
						nTotalCloseProfit += posi.m_CloseProfit;//平仓盈亏
					}
					
					if (posi.m_nPosiDate == THOST_FTDC_POSI_TODAY)
					{
	      				nTodayCloseWin = posi.m_CloseProfit;
					}


					//kenny  20170519
					if (trans.m_nOpenClose == TOC_OPEN)
					{
						posi.m_avgOpenPirce = trans.m_tradePrice;
					}
					//else if (trans.m_nOpenClose >= TOC_CLOSE )
					{
						posi.m_WincutPrice = trans.m_WincutPrice;
						posi.m_TrigNumWin = trans.m_TrigNumWin;
						posi.m_LosecutPrice = trans.m_LosecutPrice;
						posi.m_TrigNumLose = trans.m_TrigNumLose;
						posi.m_LastTradeDealPrice = trans.m_tradePrice;
					}
				}
				


				//nTotalPositionProfit += posi.m_PositionProfit;//累加持仓盈亏

				//计算字段
				if (posi.m_nPosiDate == THOST_FTDC_POSI_HISTORY)
					posi.m_avgPirce = posi.m_PreSettlementPrice;
				else
					posi.m_avgPirce = posi.m_SettlementPrice;

			}



		}


		a.second.m_CloseProfitTotal = nTotalPositionProfit;
		a.second.m_TodayCloseWin    = nTodayCloseWin;
		
	}

}





void CTraderManager::FireMessage(int sessId, MSG_TYPE mt, int num, const char* msg)
{
	string accName = GetSessionName(sessId);
	{
		std::lock_guard<std::mutex> lck(m_mtxHook);
		for_each(m_mapHook.begin(), m_mapHook.end(), [&](TraderHookMap::const_reference it){
			it.second->OnMessage(accName, mt,num,msg);
		});
	}
	{
		std::lock_guard<std::mutex> lck(m_mtxHandler);
		for_each(m_mapHandler.begin(), m_mapHandler.end(), [&](TraderHandlerMap::const_reference it){
			it.second->OnMessage(accName, mt, num, msg);
		});
	}
}



string CTraderManager::VirtualSessionName(int& sessId)
{
	string strname;
	std::lock_guard<std::mutex> lck(m_mtxAcc);
	for each (auto  it in m_mapVAcc)
	{
		if (it.second == sessId)
		{
			return it.first;
		}

	}
	return "";
}

int CTraderManager::VitualSessionID(const string& accName)
{
	std::lock_guard<std::mutex> lck(m_mtxAcc);
	AccountNameMap::iterator it = m_mapVAcc.find(accName);
	if (it != m_mapVAcc.end())
	{
		return it->second;
	}
	return 0;
}

//生成唯一编号
char* CTraderManager::GetGUID(char*lp_char, size_t size)
{
	if (lp_char == NULL || size <= 32)
		return NULL;
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		sprintf_s(lp_char, size - 1
			, "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}
	else
	{
		return NULL;
	}
	return lp_char;
}
static int g_n_rand = 0;
//生成唯一编号
char* CTraderManager::get_num_str(char* lp_char)
{
	if (lp_char == NULL)
		return NULL;



	char sz_uid[128] = { 0 };
	if (CTraderManager::GetGUID(sz_uid, sizeof(sz_uid) - 1))
	{
		int i = 0;
		int n = 0;
		for (n = 0, i = 0; n < 32 && i < 8; n++)
		{
			if (sz_uid[n] >= '0' && sz_uid[n] <= '9')
			{
				lp_char[i] = sz_uid[n];
				lp_char[i + 1] = 0;
			}
		}
	}

	if (strlen(lp_char) < 3)
	{
		SYSTEMTIME sm = { 0 };
		GetSystemTime(&sm);

		DWORD dw_time = GetTickCount();

		srand(dw_time);
		int n = (int)(rand() / 9.9 * 123456);
		while (n == 0 && g_n_rand != n)
		{
			n = rand() % 1000000;
		}
		g_n_rand = n;
		n = n % 1000000;

		sprintf_s(&lp_char[0],3, "%06d", n);
	}

	return lp_char;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////虚拟交易处理逻辑//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CTraderManager::VirtualSendOrder(string accName, string strategy, orderReq& req )
{
	CHECK_MODULE;

	orderReqVirtual &VReq = *(orderReqVirtual *)&req;
	TRADEPRICE  tp;
	//tp.dBidPrice = VReq.m_Tpu.dPrice;
	tp.dReportPrice = VReq.dOrderPrice;
	int nOrderRef = 0;//本地报单号
	if (req.nOpenClose == TOC_OPEN)
	{
		/*    界面层处理
		//下单的时候考虑，这里不做考虑
		//当前买单需要多少钱
		//资金够不够
        */

	
		if (strlen(m_mapVAccCapital[accName].m_InvestorName) <= 0)
			strncpy_s(m_mapVAccCapital[accName].m_InvestorName, m_mapVInvestor[accName].InvestorName, strlen(m_mapVInvestor[accName].InvestorName));

		if (strlen(m_mapVAccCapital[accName].m_AcctInfo.Uid) <= 0)
			strncpy_s(m_mapVAccCapital[accName].m_AcctInfo.Uid, m_mapVInvestor[accName].Uid, strlen(m_mapVInvestor[accName].Uid));

		strncpy_s(m_mapVAccCapital[accName].m_MoneyKind, "RMB", strlen("RMB"));
		m_mapVAccCapital[accName].m_AcctInfo.IsVirtualExchange =1;
		m_mapVAccCapital[accName].m_AcctInfo.As = AS_TRANS_UPDATE;

	
		if (CLxGlobalFun::simu_OpenParse(VReq.m_Tpu, tp) == 0)//"开仓"下单成功
		{	
			//本地报单编号
			get_num_str(VReq.orderRef);

			VReq.m_Tpu.dPrice = tp.dBidPrice;
			//生成历史委托信息
			GenarateOrder(accName, VReq);
			//生成交易信息
			GenarateTrade(accName, VReq);
			//生成持仓信息
			GenaratePosiAndUpdate(accName, VReq);
			//资金变化
			UpdateCapital(accName, VReq.m_Tpu);
			//统计账户汇总信息【统计交易品种】
			UpdateAccountInfo(accName, VReq.m_Tpu);
			//传到界面
			FireVSessionStatusChanged(VitualSessionID(accName), AS_TRANS_UPDATE);
		}
		else//下单不成功
		{
			//修改状态
			m_mapVAccCapital[accName].m_AcctInfo.As = AS_NULL;
			FireVMessage(VitualSessionID(accName), MT_INFO, 0, "报单不成功");
		}
	}
	else if (VReq.nOpenClose == TOC_CLOSE)
	{
		if (CLxGlobalFun::simu_CloseParse(VReq.m_Tpu, tp) == 0)//下单成功
		{
			nOrderRef = atoi(VReq.orderRef) + INC_BASE_OF_OPEN_ORDERDEF;//平仓读取传递下来的本地报单号的匹配值
			sprintf_s(VReq.orderRef, "%d", nOrderRef);

			VReq.m_Tpu.dPrice = tp.dBidPrice;

			//生成历史委托信息[增加记录]
			GenarateOrder(accName, VReq);
			//生成交易信息[增加记录]
			GenarateTrade(accName, VReq);
			//生成持仓信息【持仓变少】
			GenaratePosiAndUpdate(accName, VReq);
			//资金变化[可用资金变多]
			UpdateCapital(accName, VReq.m_Tpu);
			//统计账户汇总信息
			UpdateAccountInfo(accName, VReq.m_Tpu);

			//传到界面
			FireVSessionStatusChanged(VitualSessionID(accName), AS_TRANS_UPDATE);
		}
		else//下单不成功
		{
			//修改状态
			m_mapVAccCapital[accName].m_AcctInfo.As = AS_NULL;
			FireVMessage(VitualSessionID(accName), MT_INFO, 0, "报单不成功");
		}
	}

	
	

	return  1;
}


int CTraderManager::VirtualSessionAccountInfo(const string& accName, AccountInfo& account)
{
	CHECK_MODULE;

	for (auto a : m_mapVInvestor)
	{
		if (a.first == accName)
		{
			account.As = a.second.As;

			strncpy_s(account.Uid, a.second.Uid, sizeof(account.Uid));
			strncpy_s(account.Psw, a.second.Psw, sizeof(account.Psw));
			strncpy_s(account.Telephone, a.second.Telephone, sizeof(account.Uid));
			strncpy_s(account.Address, a.second.Address, sizeof(account.Address));
			strncpy_s(account.broker.strBrokerName, a.second.broker.strBrokerName, sizeof(account.broker.strBrokerName));
			strncpy_s(account.OpenDate, a.second.OpenDate, sizeof(account.OpenDate));

			account.IsVirtualExchange = a.second.IsVirtualExchange;
			account.IsVerifyAccount   = a.second.IsVerifyAccount;


			return  1;
		}
	}
	

	return   0;
}


int CTraderManager::VirtualSessionDel(const string& accName)
{
	CHECK_MODULE;

	std::lock_guard<std::mutex> lck(m_mtxAcc);
	AccountNameMap::iterator it = m_mapVAcc.find(accName);
	if (it != m_mapVAcc.end())
	{
		m_mapVAcc.erase(it);
		return 1;
	}
	return 0;

}

int CTraderManager::VirtualSessionClose(const string& accName)
{
	CHECK_MODULE;


	std::lock_guard<std::mutex> lck(m_mtxAcc);
	//清除数据
	m_mapVAccCapital.erase(m_mapVAccCapital.begin(), m_mapVAccCapital.end());
	vector<commissionOrder>().swap(m_mapVComOrder[accName]);
	vector<positionOrder>().swap(m_mapVPosiMatch[accName]);
	vector<positionOrder>().swap(m_mapVPosiDetailMatch[accName]);
	vector<transDetail>().swap(m_mapVTransMatch[accName]);

	return 1;

}

int CTraderManager::VirtualSessionOpen(const string& accName)
{
	CHECK_MODULE;
	//置登录状态
	int sessionID = m_mapVAcc[accName];

	m_mapVAccCapital[accName].m_AcctInfo.As = AS_LOGIN;

	return 1;
}


int CTraderManager::VirtualSessionStatus(const string& accName)
{
	CHECK_MODULE;

	int ret = m_mapVAccCapital[accName].m_AcctInfo.As;

	return ret;
}

int CTraderManager::VirtualSessionNameList(vector<string>& SessionNameList)
{
	CHECK_MODULE;
	std::lock_guard<std::mutex> lck(m_mtxAcc);
	for_each(m_mapVAcc.begin(), m_mapVAcc.end(), [&](AccountNameMap::const_reference it){
		SessionNameList.push_back(it.first);
	});

	return 1;
}


//发送给界面状态
void CTraderManager::FireVSessionStatusChanged(int sessId,  ACCOUNT_STATUS status)
{
	string accName = VirtualSessionName(sessId);

	std::lock_guard<std::mutex> lck(m_mtxVHandler);
	for_each(m_mapVHandler.begin(), m_mapVHandler.end(), [&](TraderVHandlerMap::const_reference it){
		it.second->OnSessionStatusChanged(accName, status);
	});

}


//发送给界面状态
void CTraderManager::FireVMessage(int sessId, MSG_TYPE mt, int num, const char* msg)
{
	string accName = VirtualSessionName(sessId);

	std::lock_guard<std::mutex> lck(m_mtxVHandler);
	for_each(m_mapVHandler.begin(), m_mapVHandler.end(), [&](TraderVHandlerMap::const_reference it){
		it.second->OnMessage(accName, mt, num,  msg);
	});

}




//取tpu的报单价，成交价
int  CTraderManager::GenarateOrder(const string &accName, orderReq& req)
{
	commissionOrder    Order;
	TRADEPARSEUNIT &tpu = ((orderReqVirtual *)&req)->m_Tpu;
	Order.m_OrderPrice = tpu.dReportPrice;
	Order.m_state = 5;
	Order.m_trade_fee = 0;
	strncpy_s(Order.m_InvestorName, m_mapVInvestor[accName].InvestorName, sizeof(Order.m_InvestorName));
	strncpy_s(Order.m_UserId, m_mapVInvestor[accName].Uid, sizeof(Order.m_UserId));
	//strncpy_s(Order.m_OrderSystemId, m_mapVInvestor[accName].InvestorName, sizeof(Order.m_InvestorName));
	//生成本地报单号
	strncpy_s(Order.m_OrderLocalId, req.orderRef, strlen(req.orderRef));
	strncpy_s(Order.m_OrderRef, req.orderRef, strlen(req.orderRef));
	//生成系统报单号(此编号模拟排队报单)
	get_num_str(Order.m_OrderSystemId);

	Order.m_OrderPrice = tpu.dReportPrice;
	Order.m_OrderLots  = req.lLots;
	Order.m_TradeLots  = req.lLots;

	Order.m_OrderDateTime = time(NULL);

	strncpy_s(Order.m_statuemsg, "成功报单", sizeof(Order.m_statuemsg));


	if (tpu.nDirection == DTT_SHORT_BUY)//开空
	{
		Order.m_nBuySell = TD_SELL;
		Order.m_nOpenClose = TOC_OPEN;

		Order.m_WincutPrice = tpu.dStopBasePrice - tpu.dLimitDots;
	}
	else if (tpu.nDirection == DTT_SHORT_SELL)//平空
	{
		Order.m_nBuySell = TD_SELL;
		Order.m_nOpenClose = TOC_CLOSE;
	}
	else if (tpu.nDirection == DTT_LONG_BUY)//开多
	{
		Order.m_nBuySell = TD_BUY;
		Order.m_nOpenClose = TOC_OPEN;
	}
	else if (tpu.nDirection == DTT_LONG_SELL)//平多
	{
		Order.m_nBuySell = TD_BUY;
		Order.m_nOpenClose = TOC_CLOSE;

		Order.m_WincutPrice = tpu.dStopBasePrice + tpu.dLimitDots;
	}
	else
	{
	}



	strncpy_s(Order.m_InstrumentId, req.szInstrument, strlen(((orderReqVirtual *)&req)->szInstrument));
	strncpy_s(Order.m_InvestorName, m_mapVAccCapital[accName].m_InvestorName, strlen(m_mapVAccCapital[accName].m_InvestorName));
	strncpy_s(Order.m_BrokeId, m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum, strlen(m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum));
	strncpy_s(Order.m_CurrencyID, "RMB", strlen("RMB"));

	strncpy_s(Order.m_UserId, m_mapVAccCapital[accName].m_AcctInfo.Uid, strlen(m_mapVAccCapital[accName].m_AcctInfo.Uid));


	Order.m_SubmitStatus = 5;         //提交状态
	Order.m_state = 5;		   //状态

	m_mapVComOrder[accName].push_back(Order);

	return   1;
}

int CTraderManager::GenarateTrade(const string &accName, orderReq& req)
{
	transDetail   trans;
	TRADEPARSEUNIT &tpu = ((orderReqVirtual *)&req)->m_Tpu;
	trans.m_tradeNum = tpu.dVolume = req.lLots;//模拟全部成交

	if (tpu.nDirection == DTT_SHORT_BUY)//开空
	{
		trans.m_nOpenClose = TOC_OPEN;
		trans.m_nBuySell = TD_SELL;
	}
	else if (tpu.nDirection == DTT_SHORT_SELL)		//平空
	{
		trans.m_nOpenClose = TOC_CLOSE;
		trans.m_nBuySell = TD_SELL;
		trans.m_CloseNum = tpu.dVolume;
	}
	else if (tpu.nDirection == DTT_LONG_BUY)		//开多
	{
		trans.m_nOpenClose = TOC_OPEN;
		trans.m_nBuySell = TD_BUY;

	}
	else if (tpu.nDirection == DTT_LONG_SELL)	//平多
	{
		trans.m_nOpenClose = TOC_CLOSE;
		trans.m_nBuySell = TD_BUY;
		trans.m_CloseNum = tpu.dVolume;


	}

	//生成成交编码
	get_num_str(trans.m_OrderSystemId);
	strncpy_s(trans.m_TradeCode, trans.m_OrderSystemId, strlen(trans.m_OrderSystemId));//系统编码=成交编码
	strncpy_s(tpu.tradeCode, trans.m_OrderSystemId, strlen(trans.m_OrderSystemId));//

	//本地报单编号
	strncpy_s(trans.m_OrderClientID, req.orderRef, strlen(req.orderRef));
	//系统报单编号
	//成交编码
	strncpy_s(trans.m_OrderRef, req.orderRef, strlen(req.orderRef));

	trans.m_tradeType = FTDC_TRDT_Common;
	trans.m_tradeTime = time(NULL);
	trans.m_MoneyKind = TD_CNY;

	tpu.tradeDate = trans.m_tradeTime;

	strncpy_s(trans.m_InstrumentId, req.szInstrument, strlen(((orderReqVirtual *)&req)->szInstrument));
	strncpy_s(trans.m_UserId, m_mapVAccCapital[accName].m_AcctInfo.Uid, strlen(m_mapVAccCapital[accName].m_AcctInfo.Uid));//
	strncpy_s(trans.m_BrokeId, m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum, strlen(m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum));//
	//strncpy_s(((orderReqVirtual *)&req)->orderRef, m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum, strlen(m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum));//


	//trans.m_trade_fee = ;//成交费用
	trans.m_AvgOpenPrice = tpu.dReportPrice;
	trans.m_tradePrice = tpu.dPrice;



	m_mapVTransMatch[accName].push_back(trans);


	return  1;
}


int CTraderManager::GenaratePosiAndUpdate(const string &accName, orderReq& req)
{
	positionOrder   position;
	TRADEPARSEUNIT &tpu = ((orderReqVirtual *)&req)->m_Tpu;
	//平仓盈亏
	int  WinCost = 0;
	if (tpu.nDirection == DTT_SHORT_BUY || tpu.nDirection == DTT_LONG_BUY)//开
	{
		position.m_avgOpenPirce = tpu.dPrice;// tpu.dReportPrice;
		position.m_PostionNum = tpu.dVolume;
		position.m_avgPirce = tpu.dPrice;
		position.m_OpenDate = tpu.tradeDate;

		if (tpu.nDirection == DTT_LONG_BUY)
		{

			position.m_nBuySell = TD_BUY;

		}
		else
		{
			position.m_nBuySell = TD_SELL;
		}


	}
	else
	{
		if (tpu.nDirection == DTT_LONG_SELL)//平多
		{
			position.m_nBuySell = TD_SELL;
		}
		else//short_sell//平空
		{
			position.m_nBuySell = TD_BUY;
		}


		//position.m_WincutPrice = tpu.dPrice;
		//position.m_TrigNumWin = tpu.dVolume;


		//position.m_LosecutPrice = tpu.dPrice;
		//position.m_TrigNumLose = tpu.dVolume;


	}


	position.m_TradeType = FTDC_TRDT_Common;
	strncpy_s(position.m_Currency, "RMB", strlen("RMB"));
	strncpy_s(position.m_InvestorName, m_mapVAccCapital[accName].m_InvestorName, strlen(m_mapVAccCapital[accName].m_InvestorName));

	strncpy_s(position.m_InstrumentId, req.szInstrument, strlen(req.szInstrument));
	strncpy_s(position.m_UserId, m_mapVAccCapital[accName].m_AcctInfo.Uid, strlen(m_mapVAccCapital[accName].m_AcctInfo.Uid));//
	strncpy_s(position.m_BrokeId, m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum, strlen(m_mapVAccCapital[accName].m_AcctInfo.broker.strBrokerNum));//
	strncpy_s(position.m_TradeID, tpu.tradeCode, strlen(tpu.tradeCode));//
	strncpy_s(position.m_OrderRef, req.orderRef, strlen(req.orderRef));//本地报单号，用于平仓报单

	position.m_CloseProfit = WinCost;


	//查找旧的，存在则更具开平来更新，新合约则成为新的持仓
	//for (auto &a:m_mapVPosiMatch[accName])

	for (auto &it = m_mapVPosiMatch[accName].begin(); it != m_mapVPosiMatch[accName].end(); ++it)
	{
		if (strstr(it->m_InstrumentId , position.m_InstrumentId) != NULL)
		{
			if (tpu.nDirection == DTT_SHORT_BUY || tpu.nDirection == DTT_LONG_BUY)//开仓
			{
				if ((it->m_nBuySell == TD_BUY) && (tpu.nDirection == DTT_LONG_BUY)
					|| (it->m_nBuySell == TD_SELL) && (tpu.nDirection == DTT_SHORT_BUY))
				{
					it->m_PostionNum += tpu.dVolume;
					
					if (it->m_nBuySell == TD_BUY)
					{
						it->m_WincutPrice = tpu.dStopBasePrice+tpu.dLimitDots;
						it->m_TrigNumWin  = tpu.dVolume;
						it->m_LosecutPrice = tpu.dStopBasePrice - tpu.dLimitDots;;
						it->m_TrigNumLose = tpu.dVolume;
					}
					else
					{
						it->m_WincutPrice  = tpu.dStopBasePrice - tpu.dLimitDots;
						it->m_TrigNumWin   = tpu.dVolume;
						it->m_LosecutPrice = tpu.dStopBasePrice + tpu.dLimitDots;;
						it->m_TrigNumLose  = tpu.dVolume;
					}

					return  1;
				}
				


				
			}
			else//统计平仓
			{
				//同合约,同方向，找到开,平对，则统计盈亏
				if ((it->m_nBuySell ==  TD_BUY) && (tpu.nDirection == DTT_LONG_SELL)//平多
					|| (it->m_nBuySell == TD_SELL) && (tpu.nDirection == DTT_SHORT_SELL))//平空
				{
					if (it->m_PostionNum >= tpu.dVolume)
					{
						it->m_PostionNum -= tpu.dVolume;
					}
					//统计一次账户
					if (it->m_nBuySell ==  TD_BUY)
						WinCost = (tpu.dPrice - it->m_avgOpenPirce)* tpu.dVolume;
					else
						WinCost = (it->m_avgOpenPirce - tpu.dPrice )* tpu.dVolume;
					
				
					if (WinCost > 0)
						m_mapVAccCapital[accName].m_WinProfitCount++;
					else
						m_mapVAccCapital[accName].m_LoseProfitCount++;

					double total = m_mapVAccCapital[accName].m_WinProfitCount + m_mapVAccCapital[accName].m_LoseProfitCount;

					if (total > 0)
					{
						//胜率
						m_mapVAccCapital[accName].m_WinRate = m_mapVAccCapital[accName].m_WinProfitCount / total;
					}


					if (it->m_PostionNum  <= 0)
					{
						//全平，则删除记录，并返回
						m_mapVPosiMatch[accName].erase(it);
					}
					

					return  1;
				}


			}
		}

	}
	//新的成为一个新的记录
	m_mapVPosiMatch[accName].push_back(position);

	return  1;
}

//先不算是手续费【统计模拟交易的买多空，盈亏次数】
int CTraderManager::UpdateCapital(const string &accName, TRADEPARSEUNIT &tpu)
{
	//平仓盈亏
	int  WinCost = 0;
	//动态权益
	//开仓  
	if (tpu.nDirection == DTT_SHORT_BUY || tpu.nDirection == DTT_LONG_BUY)
	{
		//可用变少
		m_mapVAccCapital[accName].m_Available -= tpu.dPrice * tpu.dVolume;
		m_mapVAccCapital[accName].m_Credit -= tpu.dPrice * tpu.dVolume;
		m_mapVAccCapital[accName].m_FrozenMargin += tpu.dPrice * tpu.dVolume;
		m_mapVAccCapital[accName].m_FrozenCash += tpu.dPrice * tpu.dVolume;
	}
	else//平仓，总资产有变动
	{
		if (tpu.nDirection == DTT_LONG_SELL)
		{
			m_mapVAccCapital[accName].m_BuyCount++;
		}
		else//short_sell
		{
			m_mapVAccCapital[accName].m_SellCount++;
		}

		m_mapVAccCapital[accName].m_Available += tpu.dPrice * tpu.dVolume;
		m_mapVAccCapital[accName].m_Credit += tpu.dPrice * tpu.dVolume;

		m_mapVAccCapital[accName].m_FrozenMargin -= tpu.dPrice * tpu.dVolume;
		m_mapVAccCapital[accName].m_FrozenCash   -= tpu.dPrice * tpu.dVolume;


		//m_CloseProfit
	}


	//计算静态权益
	m_mapVAccCapital[accName].m_StaticEquity = m_mapVAccCapital[accName].m_PreBalance + m_mapVAccCapital[accName].m_PreCredit + m_mapVAccCapital[accName].m_Deposit - m_mapVAccCapital[accName].m_Withdraw;
	//计算动态权益
	m_mapVAccCapital[accName].m_RealEquity = m_mapVAccCapital[accName].m_StaticEquity + WinCost /*PositionProfit*/ + m_mapVAccCapital[accName].m_Credit - m_mapVAccCapital[accName].m_Commission;
	//当前风险度 = 100% * 持仓保证金/当前总资产
	if (m_mapVAccCapital[accName].m_CurrMargin < 0.01)
	{
		m_mapVAccCapital[accName].m_RiskDegree = 0.0;
	}
	else
	{
		if (m_mapVAccCapital[accName].m_RealEquity > 0)
			m_mapVAccCapital[accName].m_RiskDegree = m_mapVAccCapital[accName].m_CurrMargin / m_mapVAccCapital[accName].m_RealEquity;
	}


	return  1;
}



//没用到，待删除
int   CTraderManager::UpdatePosition(const string &accName, TRADEPARSEUNIT &tpu)
{
	//
	positionOrder  posiOrder;
	posiOrder.m_avgOpenPirce = tpu.dPrice;
	posiOrder.m_avgPirce = tpu.dPrice;
	posiOrder.m_PostionNum = tpu.dVolume;


	m_mapVPosiMatch[accName].push_back(posiOrder);
	return  1;
}

//主要统计【账户】【交易品种数】
int CTraderManager::UpdateAccountInfo(const string &accName, TRADEPARSEUNIT &tpu)
{
	map<string, int>tmpMp;
	for (auto trans : m_mapVTransMatch[accName])
	{
		//统计交易品种数目
		tmpMp.insert(make_pair(trans.m_InstrumentId, 0));
	}

	//更新账户总汇
	m_mapVAccCapital[accName].m_nTradeInstruCount = tmpMp.size();//交易品种数

	return  1;
}
