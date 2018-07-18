
#include "DelayReqHandler.h"
#include "TradeHandler.h"
#include "Const.h"
#include "hjglobalfun.h"

CDelayReqHandler g_DelayReqHandler;

CDelayReqHandler::CDelayReqHandler(void)
{
	m_bRspToBroadcast = true;
}

CDelayReqHandler::~CDelayReqHandler(void)
{
}

bool CDelayReqHandler::Initlize( const string& sDelayTime )
{
	int nDelayTime = atoi(sDelayTime.c_str());
	if( nDelayTime >= 0 )
	{
		// 限制不能太频繁
		if( nDelayTime < 700 )
		{
            //kenny  20180424  window???linux??,????
            #ifdef  _WIN32
                nDelayTime = 700;
            #else
                nDelayTime = 2;
            #endif
		}
	}
	else
	{
		// 不处理广播请求
		m_bRspToBroadcast = false;

#ifdef  _WIN32
		nDelayTime = 700;
#else
		nDelayTime = 2;
#endif

	}

	// 记录延迟时间
	SetDelayTime(nDelayTime);

	// 启动线程
	BeginThread();

	return true;
}

void CDelayReqHandler::SendReq( const deque<ReqPara> &deqReq )
{
	for( std::deque<ReqPara>::const_iterator it = deqReq.begin(); it != deqReq.end(); it++ )
	{
        if( e_ReqType_CustomInfo == it->eReqType )
		{
			if( it->vPara.size() > 5 )
			{
				Req1020 stReq;
				stReq.qry_fund      = it->vPara[1];
				stReq.qry_defer     = it->vPara[2];
				stReq.qry_storage   = it->vPara[3];
				stReq.qry_cust_info = it->vPara[4];
				stReq.qry_surplus   = it->vPara[5];
				stReq.qry_forward   = CHJGlobalFun::qstr2str(gc_YesNo_No);

				Rsp1020 stRsp;
				//kenny  20171207 不知道这个true是什么意思
				g_TradeHandler.GetCustomInfo(stRsp, stReq, e_TipsShowType_Fail, true);
			}
		}
	}
}

void CDelayReqHandler::AddReqCommon()
{
	ReqPara stReqPara;
	stReqPara.eReqType = e_ReqType_CustomInfo;
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_EmptyStr));
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_Yes)); // 资金
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_Yes)); // 持仓
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_Yes)); // 库存
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_No));  // 客户信息
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_No));  // 浮动盈亏
	AddReq(stReqPara);
}

void CDelayReqHandler::HandleRecSpotOrder( BSFLAG eBSFlag  )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		// 如果是买入，则只是资金变动
		if( eBSFlag == BS_BUY )
		{
			AddCustomInfoReq(true,false,false);
		}
		// 如果是卖出，则只是库存变动
		else
		{
			AddCustomInfoReq(false,false,true);
		}
	}
}

void CDelayReqHandler::HandleRecDeferOrder( KPFLAG eKPFlag )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		// 如果是开仓，则刷新资金
		if( eKPFlag == KPF_KAI )
		{
			//AddCustomInfoReq(true,false,false);
			AddCustomInfoReq(true, true, false);
		}
		// 如果是平仓，则刷新仓位
		else
		{
			AddCustomInfoReq(false,true,false);
		}
	}
}

void CDelayReqHandler::HandleRecDDAOrder( void )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		AddReqCommon();
	}
}

void CDelayReqHandler::HandleRecMAOrder( void )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		AddReqCommon();
	}
}

void CDelayReqHandler::HandleRevSpotOrderCancel( BSFLAG eBSFlag )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		HandleRecSpotOrder(eBSFlag);
	}
}

void CDelayReqHandler::HandleRevDeferOrderCancel( KPFLAG eKPFlag )
{
	// 和报单的逻辑保持一致
	HandleRecDeferOrder(eKPFlag);
}

void CDelayReqHandler::HandleRevDDAOrderCancel( void )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		AddReqCommon();
	}
}

void CDelayReqHandler::HandleRevMAOrderCancel( void )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		AddReqCommon();
	}
}

void CDelayReqHandler::HandleSpotMatch( void )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		AddCustomInfoReq(true,false,true);
	}
}

void CDelayReqHandler::HandleDeferMatch( void )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		AddCustomInfoReq(true,true,false);
	}
}

void CDelayReqHandler::HandleDDA_MAMatch( void )
{
	if( IsWorking() && m_bRspToBroadcast)
	{
		AddReqCommon();
	}
}

bool CDelayReqHandler::IsUseless( ReqPara &stReqInQue, const ReqPara &stReqAdd )
{
	// 如果是请求客户信息，只需要将原来的参数强化即可
	if( stReqInQue.eReqType == e_ReqType_CustomInfo )
	{
		if( stReqInQue.vPara.size() > 5 )
		{
			// 资金
			if( stReqInQue.vPara[1] != stReqAdd.vPara[1] )
			{
				stReqInQue.vPara[1] = CHJGlobalFun::qstr2str( gc_YesNo_Yes);
			}

			// 持仓
			if( stReqInQue.vPara[2] != stReqAdd.vPara[2] )
			{
				stReqInQue.vPara[2] = CHJGlobalFun::qstr2str(gc_YesNo_Yes);
			}

			// 库存
			if( stReqInQue.vPara[3] !=stReqAdd.vPara[3] )
			{
				stReqInQue.vPara[3] = CHJGlobalFun::qstr2str(gc_YesNo_Yes);
			}

			// 客户信息
			if( stReqInQue.vPara[4] != stReqAdd.vPara[4] )
			{
				stReqInQue.vPara[4] = CHJGlobalFun::qstr2str(gc_YesNo_Yes);
			}

			// 浮动盈亏
			if( stReqInQue.vPara[5] != stReqAdd.vPara[5] )
			{
				stReqInQue.vPara[5] = CHJGlobalFun::qstr2str(gc_YesNo_Yes);
			}
		}

		return true;
	}
	else
		return false;
}

void CDelayReqHandler::AddCustomInfoReq()
{
	ReqPara stReqPara;
	stReqPara.eReqType = e_ReqType_CustomInfo;
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_EmptyStr));
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_Yes)); // 资金
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_Yes)); // 持仓
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_Yes)); // 库存
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_Yes)); // 客户信息
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_YesNo_No));  // 浮动盈亏
	AddReq(stReqPara);
}

void CDelayReqHandler::AddCustomInfoReq( bool bFund, bool bPosi, bool bStore, bool bCusInfo /*= false*/, bool bSurplus /*= false */ )
{
	m_deqCondMutex.Lock();

	ReqPara stReqPara;
	stReqPara.eReqType = e_ReqType_CustomInfo;
	stReqPara.vPara.push_back(CHJGlobalFun::qstr2str(gc_EmptyStr));
	stReqPara.vPara.push_back(GetReqPara(bFund)); // 资金
	stReqPara.vPara.push_back(GetReqPara(bPosi)); // 持仓
	stReqPara.vPara.push_back(GetReqPara(bStore)); // 库存
	stReqPara.vPara.push_back(GetReqPara(bCusInfo)); // 客户信息
	stReqPara.vPara.push_back(GetReqPara(bSurplus));  // 浮动盈亏
	AddReq(stReqPara);

	m_deqCondMutex.Unlock();
}

const string CDelayReqHandler::GetReqPara( bool bReq )
{
	//return bReq ? CHJGlobalFun::qstr2str(gc_YesNo_Yes) : CHJGlobalFun::qstr2str(gc_YesNo_No);
	if (bReq)
		return CHJGlobalFun::qstr2str(gc_YesNo_Yes);
	else
		return CHJGlobalFun::qstr2str(gc_YesNo_No);
}

void CDelayReqHandler::RequestFundInfo()
{
	AddCustomInfoReq(true,false,false);
}
