//#include "StdAfx.h"
//#include "HJBroadcast.h"
#include "TraderCpMgr.h"
#include "HJCommon.h"
#include "Global.h"
//#include "PreOrder/StopPLMgr.h"
#include "TranMessage.h"
#include "Mgr/TiHuoMgr.h"
#include "DelayReqHandler.h"
#include "Business.h"

#include "..\CTPSample\MyCTPQuote.h"
#include "..\CTPSample\MyCTPTrade.h"

#include<iostream>
#include<stack>
#include<map>
#include<set>
#include<functional>
#include<string>


using namespace std;


typedef function<float(float, float)> optionFun;
stack<float> snumber;
stack<string> soption;
map<string, optionFun> optFunMap;
map<string, int> optWeightMap;

#define DECLARE_OPT(opts,preority) \
     optWeightMap[#opts] = int(preority);\
     optFunMap[#opts] = bind([](float a, float b){return a opts b;}, placeholders::_1, placeholders::_2);


void init_option()
{
	DECLARE_OPT(+, 1);
	DECLARE_OPT(-, 1);
	DECLARE_OPT(*, 2);
	DECLARE_OPT(/ , 2);
}

bool isoption(string op)
{
	auto it = optWeightMap.find(op);
	return it != optWeightMap.end();
}
float recognize_float(const char* pstr, int& width)
{
	char value[24] = { 0 };
	int dotcount = 0;
	for (int i = 0; pstr[i] != '\0'; i++)
	{
		int x = pstr[i];
		if (isdigit(x) || (x == '.'&&dotcount != 0))// == 改为  !=
		{
			value[i] = x;
			dotcount++;
		}
		else
		{
			width = i;
			break;
		}
	}
	return atof(value);
}

//offset from '(' to ')'
int check_match(const char* str)
{
	int offset = 1;
	bool match = false;
	while (str[offset] != '\0')
	{
		if (str[offset++] == ')')
		{
			match = true;
			break;
		}
	}
	return match ? offset : -1;
}

void handle_stack()
{
	while (!soption.empty())
	{
		string opt = soption.top();
		if (opt == "(")
		{
			soption.pop();
			break;
		}
		auto it = optFunMap.find(opt);
		if (it != optFunMap.end())
		{
			auto fun = it->second;
			float a = snumber.top();
			snumber.pop();
			float b = snumber.top();
			snumber.pop();
			snumber.push(fun(b, a));
			soption.pop();
		}
	}
}
float calc(const char* str)
{
	if (str == nullptr)
	{
		cout << "invalid express" << endl;
		return 0;
	}

	if (*str == '+' || *str == '-')
	{
		snumber.push(0);
	}
	while (*str != '\0')
	{
		if (isdigit(*str))
		{
			int len;
			snumber.push(recognize_float(str, len));
			str += len;
			continue;
		}
		if (*str == '(')
		{
			int len = check_match(str);
			if (len == -1)
			{
				cout << "sync error : the express less a ')'" << endl;
				return -1;
			}
			soption.push("(");
			calc(str + 1);
			str += len;
			continue;
		}
		if (*str == ')')
		{
			handle_stack();
			return 0;
		}
		string ops(1, *str);
		if (isoption(ops))
		{
			if (!soption.empty())
			{
				if (optWeightMap[ops]<optWeightMap[soption.top()])
				{
					handle_stack();
				}
			}
			soption.push(ops);
			str++;
			continue;
		}
		cout << "invalid express" << endl;
		return 0;
	}
	handle_stack();
	return snumber.top();
}

//广播类报文对应的处理成员函数配置表 去掉初始化结构内最后一行后面的逗号
CTraderCpMgr::Cmd2Api CTraderCpMgr::m_Cmd2Api[] =
{
	//请求ApiName					请求处理函数指针
	{"onForceLogout", &CTraderCpMgr::onForceLogout},//强退
	{"onNewBourseBulletin", &CTraderCpMgr::onNewBourseBulletin},//公告
	{"onNewMemberBulletin", &CTraderCpMgr::onNewMemberBulletin},//公告
	{"onSessionKey", &CTraderCpMgr::onSessionKey},
	{"onRiskNotify", &CTraderCpMgr::onRiskNotify},//风险通知广播
	{"onSysInit", &CTraderCpMgr::onSysInit},//系统初始化
	{"onSysStatChange", &CTraderCpMgr::onSysStatChange},//系统状态变化
	{"onBaseTableUpdate", &CTraderCpMgr::onBaseTableUpdate},
	{"onAcctCapitalAccess", &CTraderCpMgr::onAcctCapitalAccess},
	{"onCustInfoChange", &CTraderCpMgr::onCustInfoChange},
	{"onCustRiskDegreeChange", &CTraderCpMgr::onCustRiskDegreeChange},
	{"onWindVane", &CTraderCpMgr::onRecvWindVaneQuotation},

	{"onRecvSpotQuotation", &CTraderCpMgr::onRecvSpotQuotation},//现货
	{"onRecvForwardQuotation", &CTraderCpMgr::onRecvForwardQuotation},//远期
	{"onRecvDeferQuotation", &CTraderCpMgr::onRecvDeferQuotation},//延期
	{"onRecvDeferDeliveryQuotation", &CTraderCpMgr::onRecvDeferDeliveryQuotation},

	{"onRecvRtnSpotInstStateUpdate", &CTraderCpMgr::onRecvRtnSpotInstStateUpdate},
	{"onRecvRtnForwardInstStateUpdate", &CTraderCpMgr::onRecvRtnForwardInstStateUpdate},
	{"onRecvRtnDeferInstStateUpdate", &CTraderCpMgr::onRecvRtnDeferInstStateUpdate},
	{"onRecvRtnSpotMarketStateUpdate", &CTraderCpMgr::onRecvRtnSpotMarketStateUpdate},
	{"onRecvRtnForwardMarketStateUpdate", &CTraderCpMgr::onRecvRtnForwardMarketStateUpdate},
	{"onRecvRtnDeferMarketStateUpdate", &CTraderCpMgr::onRecvRtnDeferMarketStateUpdate},

	{"ReqSpotOrder", &CTraderCpMgr::onRecvSpotOrder},
	{"onRecvRtnSpotOrder", &CTraderCpMgr::onRecvRtnSpotOrder},//现货报单流水
	{"onRecvRspSpotOrder", &CTraderCpMgr::onRecvRspSpotOrder},//现货报单流水

	{"ReqDeferOrder", &CTraderCpMgr::onRecvDeferOrder},
	{"onRecvRtnDeferOrder", &CTraderCpMgr::onRecvRtnDeferOrder},
	{"onRecvRspDeferOrder", &CTraderCpMgr::onRecvRspDeferOrder},

	{"ReqForwardOrder", &CTraderCpMgr::onRecvForwardOrder},
	{"onRecvRspForwardOrder", &CTraderCpMgr::onRecvRspForwardOrder},
	{"onRecvRtnForwardOrder", &CTraderCpMgr::onRecvRtnForwardOrder},

	{"ReqDeferDeliveryAppOrder", &CTraderCpMgr::onRecvDeferDeliveryAppOrder},
	{"onRecvRtnDeferDeliveryAppOrder", &CTraderCpMgr::onRecvRtnDeferDeliveryAppOrder},
	{"onRecvRspDeferDeliveryAppOrder", &CTraderCpMgr::onRecvRspDeferDeliveryAppOrder},

	{"ReqMiddleAppOrder", &CTraderCpMgr::onRecvMiddleAppOrder},
	{"onRecvRtnMiddleAppOrder", &CTraderCpMgr::onRecvRtnMiddleAppOrder},
	{"onRecvRspMiddleAppOrder", &CTraderCpMgr::onRecvRspMiddleAppOrder},

	{"onRecvRtnSpotOrderCancel", &CTraderCpMgr::onRecvRtnSpotOrderCancel},
	{"onRecvRspSpotOrderCancel", &CTraderCpMgr::onRecvRspSpotOrderCancel},
	{"onRecvRtnDeferOrderCancel", &CTraderCpMgr::onRecvRtnDeferOrderCancel},
	{"onRecvRspDeferOrderCancel", &CTraderCpMgr::onRecvRspDeferOrderCancel},
	{"onRecvRtnForwardOrderCancel", &CTraderCpMgr::onRecvRtnForwardOrderCancel},//撤单
	{"onRecvRspForwardOrderCancel", &CTraderCpMgr::onRecvRspForwardOrderCancel},//撤单
	{"onRecvRtnDeferDeliveryAppOrderCancel", &CTraderCpMgr::onRecvRtnDeferDeliveryAppOrderCancel},
	{"onRecvRspDeferDeliveryAppOrderCancel", &CTraderCpMgr::onRecvRspDeferDeliveryAppOrderCancel},
	{"onRecvRtnMiddleAppOrderCancel", &CTraderCpMgr::onRecvRtnMiddleAppOrderCancel},
	{"onRecvRspMiddleAppOrderCancel", &CTraderCpMgr::onRecvRspMiddleAppOrderCancel},

	{"onRecvRtnSpotMatch", &CTraderCpMgr::onRecvRtnSpotMatch},//现货成交单
	{"onRecvRtnForwardMatch", &CTraderCpMgr::onRecvRtnForwardMatch},//远期成交单
	{"onRecvRtnDeferMatch", &CTraderCpMgr::onRecvRtnDeferMatch},//延期成交单
	{"onRecvRtnDeferDeliveryAppMatch", &CTraderCpMgr::onRecvRtnDeferDeliveryAppMatch},//
	{"onRecvRtnFuture", &CTraderCpMgr::onRecvRtnFutureQuotation },//期货onRecvRtnFuture
};


void CTraderCpMgr::InitStrategy()
{
	framework = new Theron::Framework(2);
	gstrategyCond = new strategyCondition(*framework);
	gstrategyOrder = new strategyOrder(*framework);

	init_option();
}

void CTraderCpMgr::Receive(CPacket &pkt)
{
	m_criticalSectionBroadCast.lock();
	CBroadcastPacket pktBdr = dynamic_cast<CBroadcastPacket&>(pkt);

	std::string sCmdID = pktBdr.GetCmdID();
	int nSize = sizeof(m_Cmd2Api)/sizeof(Cmd2Api);
	for ( int i = 0 ; i < nSize ; i++ )
	{
		if ( m_Cmd2Api[i].sApiName == sCmdID )
		{
			if (m_Cmd2Api[i].pMemberFunc == 0)
				break;

			(this->*(m_Cmd2Api[i].pMemberFunc))(pktBdr);
		}
	}

	m_criticalSectionBroadCast.unlock();
}

void CTraderCpMgr::Receive(CBroadcastPacket &pkt)
{
	QMutexLocker locker(&m_BroadCast_qh);
	
	//adcastPacket pktBdr = dynamic_cast<CBroadcastPacket&>(pkt);

	//std::string sCmdID = "onRecvRtnFuture";//自定义期货行情
	//int nSize = sizeof(m_Cmd2Api) / sizeof(Cmd2Api);
	//for (int i = 0; i < nSize; i++)
	//{
	//	if (m_Cmd2Api[i].sApiName == sCmdID)
	//	{
	//		if (m_Cmd2Api[i].pMemberFunc == 0)
	//			break;

	//		(this->*(m_Cmd2Api[i].pMemberFunc))(pkt);
	//	}
	//}

	(this->*(m_Cmd2Api[51].pMemberFunc))(pkt);

}


//************************************************成交单**********************************************************************/
// 广播 接口 [onRecvRtnSpotMatch]SpotMatch 的业务实现
int CTraderCpMgr::onRecvRtnSpotMatch(CBroadcastPacket& pkt)
{
	SpotMatch body;
	SpotMatch::Packet2Struct(body, pkt);

	if(body.matchNo.empty() || body.orderNo.empty())
		return -1;

	string sKey = body.matchNo+body.orderNo;
	auto it     = m_QMapSpotMatch.find(sKey.c_str());
	if(it == m_QMapSpotMatch.end()) // 如果没有存在，才进行处理（解决之前相同的成交流水重复发送的问题)）
	{
		// 记录该成交流水
		m_QMapSpotMatch[sKey.c_str()] = body;

		// 添加延迟刷新【请求刷新客户信息】
		g_DelayReqHandler.HandleSpotMatch();

	    m_mapBdr[E_ONRECVRTNSPOTMATCH].Broadcast(MSG_RECV_RTN_SPOT_MATCH, &body, 0, FALSE);
	}

	return 0;
};

// 广播 接口 [onRecvRtnForwardMatch]ForwardMatch 的业务实现
int CTraderCpMgr::onRecvRtnForwardMatch(CBroadcastPacket& pkt)
{
	ForwardMatch body;
	ForwardMatch::Packet2Struct(body, pkt);

	if(body.matchNo.empty() || body.orderNo.empty())
		return -1;

	string sKey = body.matchNo+body.orderNo;
	auto it = m_QMapForwardMatch.find(sKey.c_str());
	if(it == m_QMapForwardMatch.end()) // 如果没有存在，才进行处理（解决之前相同的成交流水重复发送的问题)）
	{
		// 记录该成交流水
		m_QMapForwardMatch[sKey.c_str()] = body;

		// 交易代码
		auto sExchCode = CHJCommon::GetForwardId(CHJGlobalFun::str2qstr(body.buyOrSell));

        // 处理开仓的资金情况
		double dPosiMoney;
		HandleForwardMatchFee(CHJGlobalFun::str2qstr(body.instID), sExchCode, body.price, body.volume, dPosiMoney, CHJGlobalFun::str2qstr(body.localOrderNo));

		// ----------处理仓位 
		// 如果该品种在持仓信息中不存在，则添加该品种的持仓信息
		if(m_QMapDeferPosi.count(body.instID.c_str()) == 0)
		{
			DeferPosi stDeferPosi;
			stDeferPosi.sProdCode = body.instID.c_str();
			m_QMapDeferPosi[body.instID.c_str()] = stDeferPosi;
		}

		// 记录成交流水信息
		DeferMatchSimple structDMF;
		structDMF.dPrice = body.price;
		structDMF.iTotalHand = structDMF.iHand = body.volume;
		//structDMF.dPosiMoney = dPosiMoney;

		// 根据持仓方向获取对应的持仓信息
		auto it = m_QMapDeferPosi.find( body.instID.c_str() );
		bool bLong = (body.buyOrSell == CONSTANT_BUY_SELL_BUY) ? true : false;
		DeferPosiInfo &info = bLong ? it->infoLong : it->infoShort;

		// 处理仓位的变化
		// 总持仓
		info.iTodayAmt += body.volume;
		// 记录可用仓
		info.iUsefulAmt += body.volume;
		// 记录成交信息
		info.listMatch.push_back(structDMF);

		// 记录总持仓（昨仓+今仓）
		info.iCurrAllAmt = info.iYesAmt + info.iTodayAmt;

		// 重新计算统计信息
		//CalculateOtherValue(body.instID, info, bLong);
		// 计算持仓盈亏（浮动盈亏）
		//CalculatePosiMargin();

		// 成交的时候是否弹出提示对话框
		if( g_Global.m_bTipsOrderMatch && bIsOrderUnMatch(body.localOrderNo.c_str()) )
		{
			ShowMatchTips(body.instID.c_str(), body.price, body.volume, sExchCode, body.orderNo.c_str());
		}

		bitset<5> bitvec;
		SetBitSet(bitvec, true, false, true, bLong);

		m_mapBdr[E_ONRECVRTNFORWARDMATCH].Broadcast(MSG_RECV_RTN_FORWARD_MATCH, &body, &bitvec, FALSE);
	}

	return 0;
};

// 广播 接口 [onRecvRtnDeferMatch]DeferMatch 的业务实现
int CTraderCpMgr::onRecvRtnDeferMatch(CBroadcastPacket& pkt)
{
	DeferMatch body;
	DeferMatch::Packet2Struct(body, pkt);

	if(body.matchNo.empty() || body.orderNo.empty())
		return -1;

	// 本地报单号+报单号能唯一标识一个成交流水
	string sKey = body.matchNo+body.orderNo;
	auto it = m_QMapDeferMatch.find(CHJGlobalFun::str2qstr(sKey));
	if(it == m_QMapDeferMatch.end()) // 如果没有存在，才进行处理（解决之前相同的成交流水重复发送的问题)）
	{
		// 记录该成交流水
		m_QMapDeferMatch[sKey.c_str()] = body;

		// 获取该笔成交流水的交易类型
		auto sExchCode = CHJCommon::GetDeferId(CHJGlobalFun::str2qstr(body.offsetFlag),CHJGlobalFun::str2qstr( body.buyOrSell));
		body.sExchTypeCode = sExchCode.toStdString(); //记录交易类型ID
		// 仓位方向（多空）
		bool bLong = (sExchCode == CONSTANT_EXCH_CODE_DEFER_OPEN_LONG || sExchCode == CONSTANT_EXCH_CODE_DEFER_COV_LONG) ? true : false;
		// 开仓还是平仓（开平）
		bool bOpen = (sExchCode == CONSTANT_EXCH_CODE_DEFER_OPEN_LONG || sExchCode == CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT) ? true : false;


		// 成交的时候是否弹出提示对话框
		if( g_Global.m_bTipsOrderMatch && bIsOrderUnMatch(body.localOrderNo.c_str()) )
		{
			ShowMatchTips(body.instID.c_str(), body.price, body.volume, sExchCode, body.orderNo.c_str());
		}

		// 处理止盈止损
		if( bOpen )
		{
			//g_StopPLMgr.HandleOpenPosiMatch( body, bLong );
		}
		//加个判断，未加载主窗口
		if (this->m_hMain != NULL)	
		{
			g_DelayReqHandler.HandleDeferMatch();// 添加延迟刷新  20171219

			m_mapBdr[E_ONRECVRTNDEFERMATCH].Broadcast(MSG_RECV_RTN_DEFER_MATCH, &body, 0, FALSE);
		}
	}

	return 0;
};

// 广播 接口 [onRecvRtnDeferDeliveryAppMatch]DeferDeliveryAppMatch 的业务实现
int CTraderCpMgr::onRecvRtnDeferDeliveryAppMatch(CBroadcastPacket& pkt)
{
	DeferDeliveryAppMatch body;
	DeferDeliveryAppMatch::Packet2Struct(body, pkt);

	if(body.matchNo.empty() || body.orderNo.empty())
		return -1;

	// 本地报单号+报单号能唯一标识一个成交流水
	string sKey = body.matchNo+body.orderNo;
	auto it = m_QMapDDAMatch.find(sKey.c_str());
	if(it == m_QMapDDAMatch.end()) // 如果没有存在，才进行处理（解决之前相同的成交流水重复发送的问题)）
	{
		// 记录该成交流水
		m_QMapDDAMatch[sKey.c_str()] = body;

		// 成交的时候是否弹出提示对话框
		if( g_Global.m_bTipsOrderMatch && bIsOrderUnMatch(body.LocalOrderNo.c_str()) )
		{
			ShowMatchTips(body.instID.c_str(), 0.00, body.volume, CHJCommon::GetDDAOrderId(body.buyOrSell.c_str()), body.orderNo.c_str());
		}

		// 添加延迟刷新
		g_DelayReqHandler.HandleDDA_MAMatch();

		m_mapBdr[E_ONRECVRTNDEFERDELIVERYAPPMATCH].Broadcast(MSG_RECV_RTN_DDA_MATCH, &body, 0, FALSE);
	}

	return 0;
};


//************************************************报单**********************************************************************/

//广播 接口 [RtnSpotOrder]RtnSpotOrder 的业务实现
int CTraderCpMgr::onRecvRtnSpotOrder(CBroadcastPacket& pkt)
{
	return HandleSpotOrder(pkt, E_ONRECVRTNSPOTORDER);
// 	SpotOrder body;
// 	body.Packet2Struct(pkt);
// 	// 保存
// 	m_vSpotOrder.push_back(body);
// 	// 广播消息
// 	m_mapBdr[E_ONRECVRTNSPOTORDER].Broadcast(MSG_RECV_SPOT_ORDER, &body, 0, FALSE);
// 
//     return 0;
};

// 广播 接口 [RspSpotOrder]RspSpotOrder 的业务实现
int CTraderCpMgr::onRecvRspSpotOrder(CBroadcastPacket& pkt)
{
	return HandleSpotOrder(pkt, E_ONRECVRSPSPOTORDER);
};

int CTraderCpMgr::onRecvSpotOrder(CBroadcastPacket& pkt)
{
	return HandleSpotOrder(pkt, E_ONRECVSPOTORDER);
};

int CTraderCpMgr::HandleSpotOrder(CBroadcastPacket& pkt, int iBroadcastID)
{
	SpotOrder body;
	body.Packet2Struct(pkt);

	// 判断报单流水是否存在，存在则记录新的流水信息，不存在则插入到内存
	auto it = m_QMapSpotOrder.find(body.localOrderNo.c_str());
	if(it != m_QMapSpotOrder.end())
	{
		SpotOrder &stSpotOrder = *it;

		// 更新报单信息
		RefreshOrderInfo((OrderBase*)&stSpotOrder, (OrderBase*)&body);
	}
	else
	{
		// 插入
		auto Insert_Pair = m_QMapSpotOrder.insert(body.localOrderNo.c_str(), body);
		it = Insert_Pair;
	}

	// 处理报单信息-------------------------------------------------------------------------------------------
	SpotOrder &stOrder = *it;

	// 添加延迟刷新请求
	g_DelayReqHandler.HandleRecSpotOrder(Business::GetBS(stOrder.buyOrSell));

	m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_SPOT_ORDER, &stOrder, 0, FALSE);

	return 0;
};

// 广播 接口 [RtnForwardOrder]RtnForwardOrder 的业务实现
int CTraderCpMgr::onRecvRtnForwardOrder(CBroadcastPacket& pkt)
{
	return HandleForwardOrder(pkt, E_ONRECVRTNFORWARDORDER);
// 	ForwardOrder body;
// 	body.Packet2Struct(pkt);
// 	// 保存
// 	m_vForwardOrder.push_back(body);
// 	// 广播消息
// 	m_mapBdr[E_ONRECVRTNFORWARDORDER].Broadcast(MSG_RECV_FORWARD_ORDER, &body, 0, FALSE);
// 
//     return 0;
};

// 广播 接口 [RspForwardOrder]RspForwardOrder 的业务实现
int CTraderCpMgr::onRecvRspForwardOrder(CBroadcastPacket& pkt)
{
	return HandleForwardOrder(pkt, E_ONRECVRSPFORWARDORDER);
};

int CTraderCpMgr::onRecvForwardOrder(CBroadcastPacket& pkt)
{
	return HandleForwardOrder(pkt, E_ONRECVFORWARDORDER);
};

int CTraderCpMgr::HandleForwardOrder(CBroadcastPacket& pkt, int iBroadcastID)
{
	ForwardOrder body;
	body.Packet2Struct(pkt);
	body.bProcessed = false;

	// 判断报单流水是否存在，存在则记录新的流水信息，不存在则插入到内存
	auto it = m_QMapForwardOrder.find(body.localOrderNo.c_str());
	if(it != m_QMapForwardOrder.end())
	{
		ForwardOrder &stForwardOrder = (*it);

		// 更新报单信息
		RefreshOrderInfo((OrderBase*)&stForwardOrder, (OrderBase*)&body);
	}
	else
	{
		auto Insert_Pair = m_QMapForwardOrder.insert(body.localOrderNo.c_str(), body);
		it = Insert_Pair;
	}

	// 处理报单信息-------------------------------------------------------------------------------------------
	ForwardOrder &stOrder = *it;

	// 判断该报单没有做过处理
	if(!stOrder.bProcessed && IsOrderCanBeProcess(stOrder.status.c_str())) // && (stOrder.status == "o" || stOrder.status == "c" || stOrder.status == "p")
	{
		// 远期只有开仓，所以要冻结资金
		stOrder.bProcessed = HandleTradeFee(body.instID.c_str(), CHJCommon::GetForwardId(body.buyOrSell.c_str()), body.price, body.amount, true, stOrder.localOrderNo.c_str());
	}

	bitset<5> bitvec;
	SetBitSet(bitvec, true);

	m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_FORWARD_ORDER, &stOrder, &bitvec, FALSE);

	return 1;
}

// 广播 接口 [RtnDeferOrder]RtnDeferOrder 的业务实现
int CTraderCpMgr::onRecvRtnDeferOrder(CBroadcastPacket& pkt)
{
	return HandleDeferOrder(pkt, E_ONRECVRTNDEFERORDER);
};

// 广播 接口 [RspDeferOrder]RspDeferOrder 的业务实现
int CTraderCpMgr::onRecvRspDeferOrder(CBroadcastPacket& pkt)
{
	return HandleDeferOrder(pkt, E_ONRECVRSPDEFERORDER);
	/*
	DeferOrder body;
	body.Packet2Struct(pkt);
	// 保存
	m_vDeferOrder.push_back(body);
	// 广播消息
	m_mapBdr[E_ONRECVRSPDEFERORDER].Broadcast(MSG_RECV_DEFER_ORDER, &body, 0, FALSE);

    return 0;
	*/
};

int CTraderCpMgr::onRecvDeferOrder(CBroadcastPacket& pkt)
{
	return HandleDeferOrder(pkt, E_ONRECVDEFERORDER);
};

int CTraderCpMgr::HandleDeferOrder(CBroadcastPacket& pkt, int iBroadcastID)
{
	DeferOrder body;
	body.Packet2Struct(pkt);
	body.bProcessed = false;

	// 判断报单流水是否存在，存在则记录新的流水信息，不存在则插入到内存
	auto it = m_QMapDeferOrder.find(body.localOrderNo.c_str());
	if(it != m_QMapDeferOrder.end())
	{
		DeferOrder &stDeferOrder = (*it);

		// 更新报单信息
		RefreshOrderInfo((OrderBase*)&stDeferOrder, (OrderBase*)&body);
	}
	else
	{
		m_QMapDeferOrder[body.localOrderNo.c_str()] = body;
	}

	// 添加延迟刷新   kenny   20171219   [打开就报错[??????????????
	//加个判断，未加载主窗口
	if (this->m_hMain != NULL)
	{
		g_DelayReqHandler.HandleRecDeferOrder(Business::GetKP(body.offSetFlag));

		DeferOrder &stOrder = m_QMapDeferOrder[body.localOrderNo.c_str()];
		m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_DEFER_ORDER, &stOrder, 0, FALSE);
	}

	return 0;
};


// 广播 接口 [RtnDeferDeliveryAppOrder]RtnDeferDeliveryAppOrder 的业务实现
int CTraderCpMgr::onRecvRtnDeferDeliveryAppOrder(CBroadcastPacket& pkt)
{
	return HandleDeferDeliveryAppOrder(pkt, E_ONRECVRTNDEFERDELIVERYAPPORDER);
};

// 广播 接口 [RspDeferDeliveryAppOrder]RspDeferDeliveryAppOrder 的业务实现
int CTraderCpMgr::onRecvRspDeferDeliveryAppOrder(CBroadcastPacket& pkt)
{
	return HandleDeferDeliveryAppOrder(pkt, E_ONRECVRSPDEFERDELIVERYAPPORDER);
};
// 广播 接口 [RspDeferDeliveryAppOrder]RspDeferDeliveryAppOrder 的业务实现
int CTraderCpMgr::onRecvDeferDeliveryAppOrder(CBroadcastPacket& pkt)
{
	return HandleDeferDeliveryAppOrder(pkt, E_ONRECVDEFERDELIVERYAPPORDER);
};
// 处理交收广播
int CTraderCpMgr::HandleDeferDeliveryAppOrder( CBroadcastPacket& pkt, int iBroadcastID )
{
	DeferDeliveryAppOrder body;
	body.Packet2Struct(pkt);

	// 判断报单流水是否存在，存在则记录新的流水信息，不存在则插入到内存
	auto it = m_QMapDDAOrder.find(body.localOrderNo.c_str());
	if(it != m_QMapDDAOrder.end())
	{
		// 存在则更新报单信息
		DeferDeliveryAppOrder &stDDAOrder = (*it);

		RefreshOrderInfo((OrderBase*)&stDDAOrder, (OrderBase*)&body);
	}
	else
	{
		// 如果不存在，则插入该笔报单流水
		m_QMapDDAOrder[body.localOrderNo.c_str()] = body;
	}

	// 添加延迟刷新
	g_DelayReqHandler.HandleRecDDAOrder();

	// 获取该笔报单的信息
	DeferDeliveryAppOrder &stOrder = m_QMapDDAOrder[body.localOrderNo.c_str()];
	m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_DDA_ORDER, &stOrder, 0, FALSE);

	return 1;
}

// 广播 接口 [RtnMiddleAppOrder]RtnMiddleAppOrder 的业务实现
int CTraderCpMgr::onRecvRtnMiddleAppOrder(CBroadcastPacket& pkt)
{
	return HandleMiddleAppOrder(pkt, E_ONRECVRSPMIDDLEAPPORDER);
};

// 广播 接口 [RspMiddleAppOrder]RspMiddleAppOrder 的业务实现
int CTraderCpMgr::onRecvRspMiddleAppOrder(CBroadcastPacket& pkt)
{
	return HandleMiddleAppOrder(pkt, E_ONRECVRSPMIDDLEAPPORDER);
};

int CTraderCpMgr::onRecvMiddleAppOrder(CBroadcastPacket& pkt)
{
	return HandleMiddleAppOrder(pkt, E_ONRECVMIDDLEAPPORDER);
};
int CTraderCpMgr::HandleMiddleAppOrder( CBroadcastPacket& pkt, int iBroadcastID )
{
	MiddleAppOrder body;
	body.Packet2Struct(pkt);

	if(body.localOrderNo.empty())
		return -1;

	// 判断报单流水是否存在，存在则记录新的流水信息，不存在则插入到内存
	auto it = m_QMapMiddleAppOrder.find(body.localOrderNo.c_str());
	if(it != m_QMapMiddleAppOrder.end())
	{
		MiddleAppOrder &stMAOrder = (*it);

		RefreshOrderInfo((OrderBase*)&stMAOrder, (OrderBase*)&body);
	}
	else
	{
		m_QMapMiddleAppOrder[body.localOrderNo.c_str()] = body;
	}

	// 添加延迟刷新
	g_DelayReqHandler.HandleRecMAOrder();

	MiddleAppOrder &stOrder = m_QMapMiddleAppOrder[body.localOrderNo.c_str()];
	m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_MA_ORDER, &stOrder, 0, FALSE);

	return 1;
}


//************************************************撤单**********************************************************************/

// 广播 接口 [RtnSpotOrderCancel]RtnSpotOrderCancel 的业务实现
int CTraderCpMgr::onRecvRtnSpotOrderCancel(CBroadcastPacket& pkt)
{
	return HandleSpotOrderCancel(pkt, E_ONRECVRTNSPOTORDERCANCEL);
// 	OrderCancel body;
// 	body.Packet2Struct(pkt);
// 	// 保存
// 	m_vOrderCancel.push_back(body);
// 	// 广播消息
// 	m_mapBdr[E_ONRECVRTNSPOTORDERCANCEL].Broadcast(MSG_RECV_ORDER_CANCEL, &body, 0, FALSE);
// 
//     return 0;
};

// 广播 接口 [RspSpotOrderCancel]RspSpotOrderCancel 的业务实现
int CTraderCpMgr::onRecvRspSpotOrderCancel(CBroadcastPacket& pkt)
{
	return HandleSpotOrderCancel(pkt, E_ONRECVRSPSPOTORDERCANCEL);
};

int CTraderCpMgr::HandleSpotOrderCancel(CBroadcastPacket& pkt, int iBroadcastID)
{
	OrderCancel body;
	body.Packet2Struct(pkt);

	// 如果不存在，则做处理
	auto it = m_QMapOrderCancel.find(body.localOrderNo.c_str());
	if(it == m_QMapOrderCancel.end())
	{
		// 记录该笔报单信息
		m_QMapOrderCancel[body.localOrderNo.c_str()] = body;

		// 找到流水中与撤单广播的报单号相同的流水，修改该笔报单的状态为"已撤销"
		auto it = m_QMapSpotOrder.find(body.localOrderNo.c_str());
		if( it != m_QMapSpotOrder.end() )
		{
			it->status = "d";

			// 添加延迟刷新
			g_DelayReqHandler.HandleRevSpotOrderCancel(Business::GetBS(it->buyOrSell.c_str()));

		}
		
		// 发送消息--------------------------------------------------------------------------------------------------
		m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_ORDER_CANCEL, &body, 0, FALSE);
	}

	return 1;
}

// 广播 接口 [RtnForwardOrderCancel]RtnForwardOrderCancel 的业务实现
int CTraderCpMgr::onRecvRtnForwardOrderCancel(CBroadcastPacket& pkt)
{
	return HandleForwardOrderCancel(pkt, E_ONRECVRTNFORWARDORDERCANCEL);
// 	OrderCancel body;
// 	body.Packet2Struct(pkt);
// 	// 保存
// 	m_vOrderCancel.push_back(body);
// 	// 广播消息
// 	m_mapBdr[E_ONRECVRTNFORWARDORDERCANCEL].Broadcast(MSG_RECV_ORDER_CANCEL, &body, 0, FALSE);
};

// 广播 接口 [RspForwardOrderCancel]RspForwardOrderCancel 的业务实现
int CTraderCpMgr::onRecvRspForwardOrderCancel(CBroadcastPacket& pkt)
{
	return HandleForwardOrderCancel(pkt, E_ONRECVRSPFORWARDORDERCANCEL);
};

int CTraderCpMgr::HandleForwardOrderCancel(CBroadcastPacket& pkt, int iBroadcastID)
{
	OrderCancel body;
	body.Packet2Struct(pkt);

	// 如果不存在，则做处理
	auto  it = m_QMapOrderCancel.find(body.localOrderNo.c_str());
	if(it == m_QMapOrderCancel.end())
	{
		// 记录该笔报单信息
		m_QMapOrderCancel[body.localOrderNo.c_str()] = body;

		// 找到流水中与撤单广播的报单号相同的流水，修改该笔报单的状态为"已撤销"
		auto it = m_QMapForwardOrder.find(body.localOrderNo.c_str());
		bool bExist = (it != m_QMapForwardOrder.end() ? true : false);
		if(bExist)
		{
			it->status = "d";
		}

		bool bFundChange = false;
		// 对撤单信息进行处理------------------------------------------------------------------------------
		if(bExist) // 存在才处理，不处理重新登录后部分成交的撤单报文（不存在的话，暂时发现就这种情况）
		{
			// 处理资金的解冻，释放未成交部分的资金
			bFundChange = HandleCancelTradeFee(body.instID.c_str(), body.remainAmount, body.localOrderNo.c_str());
		}
// 		else // 如果不存在，证明是重新登录后
// 		{
// 			// 		int iMatchHand = body.amount - body.remainAmount;
// 			// 		if( iMatchHand > 0) // 对部分撤单做处理
// 			// 		{
// 			// 			// 远期交易只有（开多仓和开空仓），报单只涉及到资金的变化，与库存和持仓没有关系
// 			// 			bFundChange = HandleTradeFee(body.instID, CHJCommon::GetForwardId(body.buyOrSell), body.price, iMatchHand, true);
// 			// 		}
// 		}

		bitset<5> bitvec;
		SetBitSet(bitvec, bFundChange);

		m_mapBdr[iBroadcastID].Broadcast(MSG_RECV_ORDER_CANCEL, &body, &bitvec, FALSE);
	}

	return 1;
}

// 广播 接口 [RtnDeferOrderCancel]RtnDeferOrderCancel 的业务实现
int CTraderCpMgr::onRecvRtnDeferOrderCancel(CBroadcastPacket& pkt)
{
	return HandleDeferOrderCancel(pkt, E_ONRECVRTNDEFERORDERCANCEL);
};

// 广播 接口 [RspDeferOrderCancel]RspDeferOrderCancel 的业务实现
int CTraderCpMgr::onRecvRspDeferOrderCancel(CBroadcastPacket& pkt)
{
	return HandleDeferOrderCancel(pkt, E_ONRECVRSPDEFERORDERCANCEL);
	/*
	OrderCancel body;
	body.Packet2Struct(pkt);
	// 保存
	m_vOrderCancel.push_back(body);
	// 广播消息
	m_mapBdr[E_ONRECVRSPDEFERORDERCANCEL].Broadcast(MSG_RECV_ORDER_CANCEL, &body, 0, FALSE);

    return 0;
	*/
};

// 广播 接口 [RspDeferOrderCancel]RspDeferOrderCancel 的业务实现
int CTraderCpMgr::HandleDeferOrderCancel(CBroadcastPacket& pkt, int iBroadcastID)
{
	OrderCancel body;
	body.Packet2Struct(pkt);

	if(body.localOrderNo.empty())
		return -1;

	// 如果不存在，则做处理
	auto it = m_QMapDeferOrderCancel.find(body.localOrderNo.c_str());
	if(it == m_QMapDeferOrderCancel.end())
	{
		// 记录该笔报单信息
		m_QMapDeferOrderCancel[body.localOrderNo.c_str()] = body;

		// 找到流水中与广播的报单号相同的流水，修改该笔报单的状态为"已撤销"
		auto it = m_QMapDeferOrder.find(body.localOrderNo.c_str());
		if( it != m_QMapDeferOrder.end() )
		{
			it->status = "d";

			// 添加延迟刷新
			g_DelayReqHandler.HandleRevDeferOrderCancel(Business::GetKP(it->offSetFlag));
		}

		m_mapBdr[iBroadcastID].Broadcast(MSG_DEFER_ORDER_CANCEL, &body, 0, FALSE);
	}

	return 0;
};

// 广播 接口 [RtnDeferDeliveryAppOrderCancel]RtnDeferDeliveryAppOrderCancel 的业务实现
int CTraderCpMgr::onRecvRtnDeferDeliveryAppOrderCancel(CBroadcastPacket& pkt)
{
	return HandleDeferDeliveryAppOrderCancel(pkt, E_ONRECVRTNDEFERDELIVERYAPPORDERCANCEL);
};

// 广播 接口 [RspDeferDeliveryAppOrderCancel]RspDeferDeliveryAppOrderCancel 的业务实现
int CTraderCpMgr::onRecvRspDeferDeliveryAppOrderCancel(CBroadcastPacket& pkt)
{
	return HandleDeferDeliveryAppOrderCancel(pkt, E_ONRECVRSPDEFERDELIVERYAPPORDERCANCEL);
};

int CTraderCpMgr::HandleDeferDeliveryAppOrderCancel( CBroadcastPacket& pkt, int iBroadcastID )
{
	OrderCancel body;
	body.Packet2Struct(pkt);

	if(body.localOrderNo.empty())
		return -1;

	// 如果不存在，则做处理
	auto it = m_QMapDeferOrderCancel.find(body.localOrderNo.c_str());
	if(it == m_QMapDeferOrderCancel.end())
	{
		// 记录该笔报单信息
		m_QMapDeferOrderCancel[body.localOrderNo.c_str()] = body;

		// 找到报单流水中与广播的本地报单号相同的流水，修改该笔报单的状态为"已撤销"
		auto it = m_QMapDDAOrder.find(body.localOrderNo.c_str());
		bool bExist = (it != m_QMapDDAOrder.end() ? true : false);
		if(bExist)
		{
			it->status = "d";
		}

		// 添加延迟刷新
		g_DelayReqHandler.HandleRevDDAOrderCancel();

		m_mapBdr[iBroadcastID].Broadcast(MSG_DDA_ORDER_CANCEL,&body,  0, FALSE);
	}

	return 1;
}


// 广播 接口 [RtnMiddleAppOrderCancel]RtnMiddleAppOrderCancel 的业务实现
int CTraderCpMgr::onRecvRtnMiddleAppOrderCancel(CBroadcastPacket& pkt)
{
	return HandleMiddleAppOrderCancel(pkt, E_ONRECVRTNMIDDLEAPPORDERCANCEL);
};

// 广播 接口 [RspMiddleAppOrderCancel]RspMiddleAppOrderCancel 的业务实现
int CTraderCpMgr::onRecvRspMiddleAppOrderCancel(CBroadcastPacket& pkt)
{
	return HandleMiddleAppOrderCancel(pkt, E_ONRECVRSPMIDDLEAPPORDERCANCEL);
};
int CTraderCpMgr::HandleMiddleAppOrderCancel( CBroadcastPacket& pkt, int iBroadcastID )
{
	OrderCancel body;
	body.Packet2Struct(pkt);

	if(body.localOrderNo.empty())
		return -1;

	// 如果不存在，则做处理
	auto it = m_QMapDeferOrderCancel.find(body.localOrderNo.c_str());
	if(it == m_QMapDeferOrderCancel.end())
	{
		// 记录该笔中立仓撤单流水
		m_QMapDeferOrderCancel[body.localOrderNo.c_str()] = body;

		// 找到报单流水中与广播的本地报单号相同的流水，修改该笔报单的状态为"已撤销"
		auto it = m_QMapMiddleAppOrder.find(body.localOrderNo.c_str());
		bool bExist = (it != m_QMapMiddleAppOrder.end() ? true : false);
		if(bExist)
		{
			it->status = "d";
		}

		// 添加延迟刷新
		g_DelayReqHandler.HandleRevMAOrderCancel();

		m_mapBdr[iBroadcastID].Broadcast(MSG_MA_ORDER_CANCEL, &body, 0, FALSE);
	}

	return 1;
}

//************************************************行情**********************************************************************/

// 广播 接口 [onRecvSpotQuotation]SpotQuotation 的业务实现
int CTraderCpMgr::onRecvSpotQuotation(CBroadcastPacket& pkt)
{
    return DealRecvQuotation(pkt, E_ONRECVSPOTQUOTATION);
};

// 广播 接口 [onRecvForwardQuotation]ForwardQuotation 的业务实现
int CTraderCpMgr::onRecvForwardQuotation(CBroadcastPacket& pkt)
{
    return DealRecvQuotation(pkt, E_ONRECVFORWARDQUOTATION);
};

// 广播 接口 [onRecvDeferQuotation]DeferQuotation 的业务实现
int CTraderCpMgr::onRecvDeferQuotation(CBroadcastPacket& pkt)
{
    // 如果需要解码，则解码，并将二级系统的形式转换为行情系统的统一格式。
    // 如果该笔行情已经存在，则用最新的覆盖旧的，如果不存在，则添加行情到vector
    return DealRecvQuotation(pkt, E_ONRECVDEFERQUOTATION, true);

};

// 广播 接口 [DeferDeliveryQuotation]DeferDeliveryQuotation 的业务实现
int CTraderCpMgr::onRecvDeferDeliveryQuotation(CBroadcastPacket& pkt)
{
	DeferDeliveryQuotation body;
	body.Packet2Struct(pkt);

	// 可能需要解码

	// 保存
	bool bExist = false;
	size_t i;
	for(i = 0; i < m_vecDDQuotation.size(); i++)
	{
		if(m_vecDDQuotation.at(i).instID == body.instID)
		{
			bExist = true;
			break;;
		}
	}
	if(bExist)
	{
		m_vecDDQuotation.insert(i, body);
	}
	else
	{
		m_vecDDQuotation.push_back(body);
	}

	// 向递延交收行情发送行情消息
	m_mapBdr[E_ONRECVDEFERDELIVERYQUOTATION].Broadcast(MSG_RECV_DEFERDELIVERY_QUOTATION, &i, 0, FALSE);

	return 0;
};

// 广播 接口 [onRecvRtnSpotInstStateUpdate]SpotInstState 的业务实现
int CTraderCpMgr::onRecvRtnSpotInstStateUpdate(CBroadcastPacket& pkt)
{
    return DealRecvInstStateUpdate(pkt, E_ONRECVRTNSPOTINSTSTATEUPDATE);
};

// 广播 接口 [onRecvRtnForwardInstStateUpdate]ForwardInstState 的业务实现
int CTraderCpMgr::onRecvRtnForwardInstStateUpdate(CBroadcastPacket& pkt)
{
    return DealRecvInstStateUpdate(pkt, E_ONRECVRTNFORWARDINSTSTATEUPDATE);
};

// 广播 接口 [onRecvRtnDeferInstStateUpdate]DeferInstState 的业务实现
int CTraderCpMgr::onRecvRtnDeferInstStateUpdate(CBroadcastPacket& pkt)
{
    return DealRecvInstStateUpdate(pkt, E_ONRECVRTNDEFERINSTSTATEUPDATE);
};

//#include "Mgr/WindVaneMgr.h"
int CTraderCpMgr::onRecvWindVaneQuotation( CBroadcastPacket& pkt )
{
	WindVane body;
	body.Packet2Struct(pkt);

	// 刷新内存
	//CWindVaneMgr::GetInstance().RefreshWindVane(body);

	// 广播
	m_mapBdr[E_WIND_VANE].Broadcast(WM_WIND_VANE, &body, 0, FALSE);

	return 0;
}

//
int CTraderCpMgr::DealRecvInstStateUpdate( CBroadcastPacket& pkt, const int &iBroadcastID )
{
	InstState body;
	body.Packet2Struct(pkt);

	/*vector<InstState>::iterator it = m_vInstState.begin();
	for(; it != m_vInstState.end(); it++ )
	{
		if( it->instID == body.instID )
		{
			it->tradeState = body.tradeState;
			break;
		}
	}
	if( it == m_vInstState.end() )
	{
		m_vInstState.push_back(body);
		it = m_vInstState.end();
		it--;
	}
	m_mapBdr[iBroadcastID].Broadcast(WM_INST_STATE_UPDATE, (WPARAM)&it, 0, false);*/
	

	// 修改品种的状态ID
	int iPos = GetInstStatePos(body.instID.c_str());
	if(iPos != -1)
	{
		//m_vInstState.at(iPos).tradeState = body.tradeState;
	}
	else
	{
		m_vInstState.push_back(body);
		iPos = m_vInstState.size()-1;
	}

	uint *pos = new uint(iPos);
	m_mapBdr[iBroadcastID].Broadcast(WM_INST_STATE_UPDATE, &pos, 0, false);

//#ifdef _WRITE_LOG
//	CString csLog;
//	csLog.Format("广播品种%s的状态变化，变化为%s", body.instID.c_str(), body.tradeState.c_str());
//	g_Global.WriteLog(csLog);
//#endif

	return 1;
}

//************************************************其他**********************************************************************/
// 广播 接口 [onNewBourseBulletin]TNewBourseBulletin 的业务实现
int CTraderCpMgr::onNewBourseBulletin(CBroadcastPacket& pkt)
{
	TNewBourseBulletin body;
    CBroadcastPacketAndStruct::Packet2Struct(body, pkt);
	m_mapBdr[E_ONNEWBOURSEBULLETIN].Broadcast(WM_ON_NEW_BOURSE_BULLETIN, &body, 0, FALSE);

    return 0;
};

// 广播 接口 [onNewMemberBulletin]TNewMemberBulletin 的业务实现
int CTraderCpMgr::onNewMemberBulletin(CBroadcastPacket& pkt)
{
	TNewMemberBulletin  body;
	CBroadcastPacketAndStruct::Packet2Struct(body, pkt);

	// 广播消息
	m_mapBdr[E_ONNEWMEMBERBULLETIN].Broadcast(WM_ON_NEW_MEMBER_BULLETIN, &body, 0, FALSE);

    return 0;
};

// 广播 接口 [onRiskNotify]TRiskNotity 的业务实现
int CTraderCpMgr::onRiskNotify(CBroadcastPacket& pkt)
{
	TRiskNotity body;
	CBroadcastPacketAndStruct::Packet2Struct(body, pkt);

	// 广播消息
	m_mapBdr[E_ONRISKNOTIFY].Broadcast(WM_ON_RISK_NOTIFY, &body, 0, FALSE);

    return 0;
};

// 广播 接口 [onRiskNotify]TRiskNotity 的业务实现
int CTraderCpMgr::onSysInit(CBroadcastPacket& pkt)
{
	SysInit body;
	CBroadcastPacketAndStruct::Packet2Struct(body, pkt);

	// 修改全局变量
	g_Global.m_strLast_Exch_Date = body.old_exch_date.c_str();

	// 清空内存中的成交流水
	m_QMapSpotMatch.clear();
	m_QMapForwardMatch.clear();
	m_QMapDeferMatch.clear();
	m_QMapDDAMatch.clear();

    // 清除内存中的报单流水
	m_QMapSpotOrder.clear();
	m_QMapForwardOrder.clear();
	m_QMapDeferOrder.clear();
	m_QMapDDAOrder.clear();
	m_QMapMiddleAppOrder.clear();
	
    // 清除撤单流水
	m_QMapDeferOrderCancel.clear();
	m_QMapOrderCancel.clear();

	// 清除预埋单
	//m_cPreOrder.ClearPreOrder();

	// 重新请求客户数据
	g_DelayReqHandler.AddCustomInfoReq();

	// 广播消息（清空报单和成交流水窗口的界面数据；弹出日结单确认）
	m_mapBdr[E_ONSYSINIT].Broadcast(WM_ON_SYS_INIT, 0, 0, TRUE);

    return 0;
};

// 广播 接口 [onSysStatChange]SysStat 的业务实现
int CTraderCpMgr::onSysStatChange(CBroadcastPacket& pkt)
{
	SysStat body;
	CBroadcastPacketAndStruct::Packet2Struct(body, pkt);

	// 获取系统状态
	g_Global.m_strSysState = body.m_sys_stat.c_str();
	
	//获取交易日期
	g_Global.m_strExchDate = body.exch_date.c_str();

	//获取系统日期
	g_Global.m_strSysDate  = body.sys_date.c_str();

	// 根据系统状态id，获取系统状态名称，显示到状态栏，没实现
	m_mapBdr[E_ONSYSSTATCHANGE].Broadcast(WM_SYS_STAT_CHANGE, &body, 0, FALSE);

    return 0;
};

// 广播 接口 [onCustRiskDegreeChange]CustRiskDegreeChange 的业务实现
int CTraderCpMgr::onCustRiskDegreeChange(CBroadcastPacket& pkt)
{
    CustRiskDegreeChange body;
	CBroadcastPacketAndStruct::Packet2Struct(body, pkt);

    m_mapBdr[E_ONCUSTRISKDEGREECHANGE].Broadcast(WM_RISK_DEGREE_CHANGE, &body, 0, FALSE);

    return 0;
};

//************************************************未实现**********************************************************************/

// 广播 接口 [onAcctCapitalAccess]AcctCapitalAccess 的业务实现
int CTraderCpMgr::onAcctCapitalAccess(CBroadcastPacket& pkt)
{
    return 0;
};

// 广播 接口 [onBaseTableUpdate]BaseTableUpdate 的业务实现
int CTraderCpMgr::onBaseTableUpdate(CBroadcastPacket& pkt)
{
    return 0;
};

// 广播 接口 [onCustInfoChange]CustInfoChange 的业务实现
int CTraderCpMgr::onCustInfoChange(CBroadcastPacket& pkt)
{
    return 0;
};

// 广播 接口 [onForceLogout]TForceLogout 的业务实现
int CTraderCpMgr::onForceLogout(CBroadcastPacket& pkt)
{
	// debug 版本不强退
  	//#ifdef _DEBUG
  	//return 0;
  	//#endif

	TForceLogout stLogout;
	CBroadcastPacketAndStruct::Packet2Struct(stLogout, pkt);

	// 记录强退报文
	//WLog(E_LOG_BROADCAST,"接收强退广播：%s %s", stLogout.session_id.c_str(), stLogout.hint_msg.c_str());

#ifdef _VERSION_GF
	// 如果SessionID与本地不匹配则不签退
	if (!stLogout.session_id.empty() && stLogout.session_id != g_Global.m_strSessionID)
	{
		return 0;
	}
	//remove 2014.4.29
// 	int nRet = stLogout.hint_msg.find("SESSION_ID.");
// 	if (nRet >= 0)
// 	{
// 		int nBegin = stLogout.hint_msg.find("[");
// 		int nEnd = stLogout.hint_msg.find("]");
// 		if (nBegin < 0 || nEnd < 0 || nBegin >= nEnd)
// 		{
// 			LOG("接收到多点签退广播，未找到中括号匹配的sessionId");
// 			return 0;
// 		}
// 		string sSession = stLogout.hint_msg.substr(nBegin+1, nEnd - nBegin - 1);
// 
// 		if (stLogout.hint_msg.length() > 0 && sSession != g_Global.m_strSessionID )
// 		{
// 			string sLog = "接收到多点签退广播，未找到需要签退的";
// 			sLog += sSession;
// 			LOG(sLog);
// 			return 0;
// 		}
// 	}
#endif	

	//LOG("接收到签退广播，执行签退");


	QParamEvent *msg = new QParamEvent(WM_FORCE_LOGOUT);
	msg->setWParam(&stLogout);

	QApplication::postEvent(m_hMain, msg);



    return 0;
};

// 广播 接口 [onSessionKey]TSessionKey 的业务实现
int CTraderCpMgr::onSessionKey(CBroadcastPacket& pkt)
{
    return 0;
};

// 广播 接口 [onRecvRtnSpotMarketStateUpdate]SpotMarketState 的业务实现
int CTraderCpMgr::onRecvRtnSpotMarketStateUpdate(CBroadcastPacket& pkt)
{
    return 0;
};

// 广播 接口 [onRecvRtnForwardMarketStateUpdate]ForwardMarketState 的业务实现
int CTraderCpMgr::onRecvRtnForwardMarketStateUpdate(CBroadcastPacket& pkt)
{

    return 0;
};

// 广播 接口 [onRecvRtnDeferMarketStateUpdate]DeferMarketState 的业务实现
int CTraderCpMgr::onRecvRtnDeferMarketStateUpdate(CBroadcastPacket& pkt)
{

    return 0;
};


int CTraderCpMgr::Reigister(mainWindow* pkt)
{
	m_hMain = pkt;
	return 0;
};


/***************************/
string UserID = "087614";
string Pswd = "ypdxcn";
string Broker = "9999";
string INSTRU = "a1805";
/***************************/

//收到的期货行情
void CTraderCpMgr::LoginCTP(const char *user, const char*pswd, const char *broker)
{
	/*MyCTPTrade::Instance()->setMainWidget(this);
	MyCTPQuote::Instance()->setMainWidget(this);*/

	int  loginNum = 0;
	while (true)
	{
		static int num = 1;//用于行情轮动
		string  strTradeFront;
		string  strQuoteFront;
		string strTradeFiled = "CTPFront.trade";
		string strQuoteFiled = "CTPFront.quote";

		strTradeFiled += strutils::intstr(num % 4);
		strQuoteFiled += strutils::intstr(num % 4);

		num = num == 0 ? 1 : num;

		m_pConfig->GetProperty(strTradeFiled, strTradeFront);
		m_pConfig->GetProperty(strQuoteFiled, strQuoteFront);
		if (!MyCTPTrade::Instance()->m_bLoginSuccessed)
			MyCTPTrade::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), strTradeFront.c_str());//初始化交易
		if (!MyCTPQuote::Instance()->m_bLoginSuccessed)
			MyCTPQuote::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), strQuoteFront.c_str());//初始化行情

		Sleep(7000);
		loginNum++;
		num++;

		if (MyCTPTrade::Instance()->m_bLoginSuccessed && MyCTPQuote::Instance()->m_bLoginSuccessed)
		{
			CRLog(E_APPINFO, "登录CTP成功，登陆次数[%d]，成功订阅行情", loginNum);

			MyCTPQuote::Instance()->Subscribe(this);

			m_hMain->SetLoginQHStatus(true);
			break;
		}

		if (loginNum > 20)
		{
			m_hMain->SetLoginQHStatus(false);
			break;
		}
	}

	//MyCTPTrade::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10001");//初始化交易
	//MyCTPQuote::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10011");//初始化行情

	//上期模拟   ----ok    交易日，16：00～次日09：00；非交易日，16：00～次日15：00。
	//MyCTPTrade::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10030");//初始化交易
	//MyCTPQuote::Instance()->Init(UserID.c_str(), Pswd.c_str(), Broker.c_str(), (char *)"tcp://180.168.146.187:10031");//初始化行情

}


//extern QWaitCondition bufferUpdate;
//extern bool           newSignal;
//extern QMutex         sigMutex;
//extern QString        gstrCurT1ProdId;
//extern QString        gstrCurT2ProdId;
//
//class QStrategyRunning;
//收到的期货行情
int CTraderCpMgr::onRecvRtnFutureQuotation(CBroadcastPacket& pkt)
{
	const QUOTATION* pQuotation = TranslateQH(pkt);
	if (pQuotation == NULL)
	{
		// 解码失败
		return -1;
	}
	else
	{
		const string &sInstID = pQuotation->instID;
		if (mapStrategyRunning.size() > 0)
		{
			// 计算持仓盈亏
		
			//计算套利-条件策略
			//找到套利合约的ID
			//QString strInst = sInstID.c_str();
			//gstrCurT1ProdId = "";
			//gstrCurT2ProdId = "";
			//mapStrategyConditionOrder[id]
			//便利查找到原子合约
			for (auto id : mapStrategyRunning.keys())
			{
				if(id.contains(sInstID.c_str()))
				{
					//QStrategyRunning *condorder = new QStrategyRunning();
					QString t1 = mapStrategyRunning[id].obj[0].prodID;
					QString t2 = mapStrategyRunning[id].obj[1].prodID;

					if (t1.contains(sInstID.c_str()))
					{
						//get  t2  为黄金的合约ID
						auto it = m_QMapQuotation.find(t2.toStdString());
						if (it != m_QMapQuotation.end())
						{
							const QUOTATION& pQuotation2 = m_QMapQuotation[t2.toStdString()];
							//计算生成套利行情

							strategy_conditionOrder & st_cond = mapStrategyRunning[id];
							st_cond.obj[0].quote.m_uiLast = pQuotation->m_uiLast;
							st_cond.obj[1].quote.m_uiLast = pQuotation2.m_uiLast;
							
							if (framework->Send(st_cond, gstrategyOrder->GetAddress(), gstrategyCond->GetAddress()))
							{
								//
							}

							//计算套利行情
							QUOTATION Quotation;
							Quotation.instID  =  st_cond.id.toStdString();
							Quotation.name    =  st_cond.name.toStdString();

							QString exp = st_cond.express;
							exp.replace("T1", QString::number(st_cond.obj[0].quote.m_uiLast));
							exp.replace("T2", QString::number(st_cond.obj[1].quote.m_uiLast));

							QString exp1 = st_cond.express;
							exp.replace("T1", QString::number(st_cond.obj[0].quote.m_Bid[0].m_uiPrice));
							exp.replace("T2", QString::number(st_cond.obj[1].quote.m_Bid[0].m_uiPrice));

							QString exp2 = st_cond.express;
							exp.replace("T1", QString::number(st_cond.obj[0].quote.m_Ask[0].m_uiPrice));
							exp.replace("T2", QString::number(st_cond.obj[1].quote.m_Ask[0].m_uiPrice));

							Quotation.m_uiLast           = calc(exp.toStdString().c_str());
							Quotation.m_Bid[0].m_uiPrice = calc(exp.toStdString().c_str());//买1价
							Quotation.m_Ask[0].m_uiPrice = calc(exp.toStdString().c_str());//卖1价

							//广播套利行情
							m_mapBdr[E_ONRECVFUTUREQUOTATION].Broadcast(MSG_RECV_SPREAD_QUOTATION, (void *)&Quotation, nullptr, false);//&sInstID
						}
					}
					else if(t2.contains(sInstID.c_str()))
					{
						//get  t1  为黄金的合约ID
						auto it = m_QMapQuotation.find(t1.toStdString());
						if (it != m_QMapQuotation.end())
						{
							const QUOTATION& pQuotation2 = m_QMapQuotation[t1.toStdString()];
							//计算生成套利行情

							//广播套利行情
							m_mapBdr[E_ONRECVFUTUREQUOTATION].Broadcast(MSG_RECV_SPREAD_QUOTATION, (void *)pQuotation, nullptr, false);//&sInstID
						}
					}
					else
					{
						continue;
					}
				}
			}


			
		}

		// 广播期货消息
		m_mapBdr[E_ONRECVFUTUREQUOTATION].Broadcast(MSG_RECV_FUTURE_QUOTATION, (void *)pQuotation, nullptr, false);//&sInstID
		
	}

	return 0;
}