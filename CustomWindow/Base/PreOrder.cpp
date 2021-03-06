#include "PreOrder.h"
#include "TranMessage.h"

#include "TraderCpMgr.h"
#include "Global.h"
#include "HJConstant.h"
#include "Mgr/ProdCodeMgr.h"
#include "GlobalMgr.h"    //通过消息的形式想主窗口添加一个提示
#include "Struct.h"
#include "HJGlobalFun.h"




CPreOrder::CPreOrder(void)
{
	m_iPreOrderID = 0;
	//InitializeCriticalSection(&m_criticalSection);
	bRun = true;

}

CPreOrder::~CPreOrder(void)
{
	//DeleteCriticalSection(&m_criticalSection);
}

// 清除内存中已发送的预埋单，如果有删除的记录则返回true，如果没有删除则返回false
bool CPreOrder::ClearSended()
{
	bool bClear = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end();)
	{
		if(it->eOrderState == E_Sended)
		{
			it = EraseOrder(it);
			bClear = true;
		}
		else
		{
			it++;
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
	return bClear;
}

// 添加一笔预埋单，返回其ID
int CPreOrder::Add( PreOrderStruct &stPreOrder )
{
	stPreOrder.iID = m_iPreOrderID++;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	m_vPreOrders.push_back(stPreOrder);
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();

	// 如果需要弹出提示
	if( g_Global.IsTipsAddPreOrder() )
	{
		TipsOrderInfo stTipsInfo(E_Tips2_CreatePreOrder);
		stTipsInfo.sContent = CHJGlobalFun::qstr2str(PreOrderBusiness::GetPreOrderTips(stPreOrder));
		
		//kenny  2018-1-8
		//kenny  2018-4-2 屏蔽
		g_GlobalMgr.AddTips(stTipsInfo);
	}

	return stPreOrder.iID;
}

// 删除ID为iOrderID的预埋单，如果删除成功则返回true，否则返回false
bool CPreOrder::Erase( int iOrderID )
{
	bool bFind = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end();)
	{
		if(it->iID == iOrderID)
		{
			it = EraseOrder(it);
			bFind = true;
		}
		else
		{
			it++;
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
	return bFind;
}

// 获得触发条件列的显示文字，如果是预埋单的话则返回“自动”或者“手动”，如果是条件单则返回触发的条件文字
QString CPreOrder::GetTriConditionName( const PreOrderStruct &stPreOrder )
{
	if(stPreOrder.eOrderType == PreOrderType_Hand)
	{
		return QString::fromLocal8Bit("手动");
	}
	if(stPreOrder.eOrderType == PreOrderType_AUTO)
	{
		return QString::fromLocal8Bit("自动");
	}
	else // 条件单
	{
		QString sValue = QString::fromLocal8Bit("当");
		sValue.append(GetPriceTypeName(stPreOrder.eQTType));     // 价格类型
		sValue.append(GetCompareTypeName(stPreOrder.eCompType)); // 比较类型

		// 处理价格的格式化字符串
		stringstream ss; 
		ss << stPreOrder.dCmpPrice+0.001;
		string sPrice;
		sPrice = ss.str();
		int iPos = sPrice.find('.');
		if(iPos != -1)
		{
			// 如果小数点大于3位，则取两位小数
			if(sPrice.length()-iPos > 3)
			{
				sPrice =sPrice.substr(0, iPos+3);
			}
		}
		sValue.append(sPrice.c_str());
		sValue.append(QString::fromLocal8Bit("时发出"));

		return sValue;
	}
}

/*
解决之前采用GetTriConditionName当价格为182.9971时，没有四舍五入的问题
*/
QString CPreOrder::GetTriConditionNameEx( const PreOrderStruct &stPreOrder )
{
	if(stPreOrder.eOrderType == PreOrderType_Hand)
	{
		return QString::fromLocal8Bit("手动");
	}
	if(stPreOrder.eOrderType == PreOrderType_AUTO)
	{
		return QString::fromLocal8Bit("自动");
	}
	else // 条件单
	{
		QString csValue = QString::fromLocal8Bit("当%1%2%3时发出")
			.arg(GetPriceTypeName(stPreOrder.eQTType))
			.arg(GetCompareTypeName(stPreOrder.eCompType))
			.arg(QString::number(stPreOrder.dCmpPrice,'f',2));


		return csValue;
	}
}

// 根据预埋单类型ID获取类型名称
QString CPreOrder::GetOrderTypeName( EPreOrderType eOrderTypeID, bool bServer /*= false*/ )
{
	QString sValue;
	if( eOrderTypeID == PreOrderType_Loss )
		sValue = QString::fromLocal8Bit("止损单");
	else if( eOrderTypeID == PreOrderType_Profit )
		sValue = QString::fromLocal8Bit("止盈单");
	else if( eOrderTypeID == PreOrderType_Hand )
		sValue = QString::fromLocal8Bit("手动预埋单");
	else if( eOrderTypeID == PreOrderType_AUTO )
		sValue = QString::fromLocal8Bit("自动预埋单");
	else if(eOrderTypeID == PreOrderType_Condition )
		sValue = QString::fromLocal8Bit("条件单");
	
	if( bServer )
	{
		sValue = sValue + QString::fromLocal8Bit("(服务器)");
	}
	else
	{
		sValue = sValue + QString::fromLocal8Bit("(本地)");
	}

	return sValue;
}

// 根据预埋单状态ID获取状态名称
QString CPreOrder::GetOrderStateName( EPreOrderState eOrderStateID )
{
	switch(eOrderStateID)
	{
	case E_Sended :return QString::fromLocal8Bit("已发送");
	case E_SendFailed: return QString::fromLocal8Bit("发送失败");
	default: return QString::fromLocal8Bit("未发送");
	}
}

// 根据行情价格类型ID获取名称
QString CPreOrder::GetPriceTypeName( EQTType eQTType )
{
	switch (eQTType)
	{
	case E_Latest: return QString::fromLocal8Bit("最新价");
	case E_Sell1:  return QString::fromLocal8Bit("卖一价");
	case E_Buy1:   return QString::fromLocal8Bit("买一价");
	default: return "";
	}
}

// 根据比较类型ID获取比较类型名称
QString CPreOrder::GetCompareTypeName( ECompareType eCmpType )
{
	switch (eCmpType)
	{
	case E_Small:        return QString::fromLocal8Bit("小于");       // ＜ <
	case E_Small_Equal:  return QString::fromLocal8Bit("小于或等于"); // ≤
	case E_Big:          return QString::fromLocal8Bit("大于");       // ＞
	case E_Big_Equal:    return QString::fromLocal8Bit("大于或等于"); // ≥
	case E_Equal:        return QString::fromLocal8Bit("等于"); // ≥
	default: return "";
	}
}

// 发送一笔报单，由于涉及到两个线程都可能调用这个函数，可能在执行这个函数之前该笔报单已经被删除或者已经被移动了位置，故不能传递指针，所以只能通过预埋单ID
// 重新找到该笔报单的信息，（找不到则返回什么呢？待处理），
// 同时有一个问题，如果允许用户自动发送自动报单，则用户界面进入这个函数的时候，这笔报单的状态可能已经被线程修改为已发送，这样会导致重复发送，所以还是不建议用户手动发送自动报单
// 
int CPreOrder::CommitAsk( int iOrderID )
{
	// 根据id获取预埋单信息
	PreOrderStruct stPreOrder;
	if(GetOrderByID(iOrderID, stPreOrder))
	{
		Rsp4001 rsp;

		// 获取报单价格
		QString csPrice, csHand;
		if( stPreOrder.eOrderType == PreOrderType_Loss ||  stPreOrder.eOrderType == PreOrderType_Profit ) // 如果是止盈止损单
		{
			const ProdCodeInfo* pInfo = g_ProdCodeMgr.GetProdeCode( CHJGlobalFun::str2qstr(stPreOrder.sProdCode));
			if( pInfo != NULL )
			{
				PLParam *pPLParam = (PLParam*)stPreOrder.wParam;
                
                // added by Jerry Lee, 2013-3-26, 防止价格为零的情况发生
                if (*pPLParam->pPrice <= 0)
                {
                    return -2;
                }
				csPrice = QString::number(((*pPLParam->pPrice) / g_Global.m_dQTFactor + pInfo->tick*pPLParam->iAjust), 'f', 2);
				//csPrice.sprintf("%.2f", );
			}
			else
			{
				// 获取品种最小单位失败
				return -1;
			}
		}
		else
		{ 
			csPrice = QString::number(stPreOrder.dPrice  ,'f'  ,2);
		}
		
		csHand = QString::number( stPreOrder.iHand);
		// 发送该笔报单
		int iRspID = g_TraderCpMgr.CommitOrder(CHJGlobalFun::str2qstr(stPreOrder.sProdCode), csPrice, csHand, CHJGlobalFun::str2qstr(stPreOrder.sExchId), false);
        stPreOrder.eOrderState = (iRspID  == 0 ? E_Sended : E_SendFailed);

		// 修改该笔报单的状态为已发送，修改失败（即没找到怎么办）
		SetOrderState(iOrderID, stPreOrder.eOrderState);
		
		//kenny  2018-1-8 暂时屏蔽
		SendMessage(iOrderID, PreOrderMsgFlag_Mdf_State);

		//
		// 如果发送成功，而且是止盈止损单
		if( /*iRsp == 0 &&*/ IsStopPLOrder(stPreOrder.eOrderType) )
		{
			// 删除掉与其相关的其他止损单
			DelRelativeOrder(stPreOrder);
		}

		return iRspID;
	}
	else
	{
		return -1;
	}
}

// 根据位置获得预埋单信息，如果该位置上的数据没有被删除，则返回true，否则返回false
bool CPreOrder::GetOrderByPos( size_t iPos, PreOrderStruct &stOrder )
{
	bool bFind = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	if(m_vPreOrders.size() > iPos)
	{
		bFind = true;
		stOrder = m_vPreOrders.at(iPos);
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();

	return bFind;
}
// 根据预埋单ID获得该笔单的信息，没有找到则返回false，找到则返回true
bool CPreOrder::GetOrderByID( int iID, PreOrderStruct &stOrder )
{
	bool bFind = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	for (size_t i = 0; i < m_vPreOrders.size(); i++)
	{
		if(m_vPreOrders.at(i).iID == iID)
		{
			CopyOrder(stOrder, m_vPreOrders.at(i));
			bFind = true;
			break;
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
	return bFind;
}

// 设置ID为iOrderID的预埋单为已发送，找到则返回true，没找到则返回fale
bool CPreOrder::SetOrderState( int iOrderID , EPreOrderState eState)
{
	bool bFind = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	for (size_t i = 0; i < m_vPreOrders.size(); i++)
	{
		if(m_vPreOrders.at(i).iID == iOrderID)
		{
			m_vPreOrders.at(i).eOrderState = eState;
			bFind = true;
			break;
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
	return bFind;
}

// 获得当前预埋单的个数
int CPreOrder::GetOrderCount()
{
	int ret;
	m_criticalSection.lock();
	//EnterCriticalSection(&m_criticalSection);
	ret = m_vPreOrders.size();
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
	return ret;
}

/*
接受到一笔延期行情后调用的函数，处理止损单
sInsID：行情品种
uiLast：行情的最新价
处理逻辑：判断是否需要更新行情的触发价格，因为如果设置了动态止损的话，触发价格是可能动态变化的，这样的话要不断与
          静态触发价格比较（如果设置了静态止损的话），取他们两个中的最优值为触发价格
*/
void CPreOrder::HandleRecQuotation( const string &sInsID, unsigned int uiLast )
{
	m_criticalSection.lock();
	//EnterCriticalSection(&m_criticalSection);
	for (size_t i = 0; i < m_vPreOrders.size(); i++)
	{
		PreOrderStruct &stPreOrder = m_vPreOrders.at(i);
		// 如果当前的预埋单是止损单，而且没发送，而且和行情品种一致
		if( stPreOrder.eOrderType == PreOrderType_Loss && stPreOrder.eOrderState == E_NoSend && stPreOrder.sProdCode == sInsID )
		{
			bool bUpdateDys = false;
			LossParam *pLossParam = (LossParam*)stPreOrder.wParam;
			// 如果采用了动态止损
			if( pLossParam->uiDys != 0 )
			{
				if( pLossParam->bLong ) // 多仓
				{
					// 如果当前行情均价比极值大
					if( uiLast > pLossParam->uiExtreme )
					{
						// 新的触发条件的价格
						pLossParam->uiDys +=  uiLast - pLossParam->uiExtreme;

						bUpdateDys = true;
					}
				}
				else
				{
					// 如果当前行情均价比极值小
					if( uiLast < pLossParam->uiExtreme )
					{
						// 新的触发条件的价格
						pLossParam->uiDys +=  uiLast - pLossParam->uiExtreme;

						bUpdateDys = true;
					}
				}

				if( bUpdateDys )
				{
					// 更新极值
					pLossParam->uiExtreme = uiLast;

					// 更新比较价格
					bool bUpdatePrice = false;
					if( pLossParam->uiStatic != 0 )
					{
						if( pLossParam->bLong )
						{
							if( pLossParam->uiDys > pLossParam->uiStatic )
							{
								stPreOrder.dCmpPrice = pLossParam->uiDys / g_Global.m_dQTFactor;
								bUpdatePrice = true;
							}
						}
						else
						{
							if( pLossParam->uiDys < pLossParam->uiStatic )
							{
								stPreOrder.dCmpPrice = pLossParam->uiDys / g_Global.m_dQTFactor;
								bUpdatePrice = true;
							}
						}
					}
					else
					{
						// 如果没有配置静态止损，则直接更新
						stPreOrder.dCmpPrice = pLossParam->uiDys / g_Global.m_dQTFactor;
						bUpdatePrice = true;
					}
					
					if( bUpdatePrice )
					{
						// 更新预埋单界面的显示
						SendMessage(stPreOrder.iID, PreOrderMsgFlag_Mdf_Con);
					}
				}
			}
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
}

CPreOrder::IT CPreOrder::EraseOrder( const IT &it )
{
	/*if( it->eOrderType == PreOrderType_Profit || it->eOrderType == PreOrderType_Loss )
	{
		PLParam *pPLParam = (PLParam*)it->wParam;
		delete pPLParam;
	}*/

	return m_vPreOrders.erase(it);
}

void CPreOrder::DelRelativeOrder( const PreOrderStruct &stOrder )
{
	PLParam *pParam = (PLParam*)stOrder.wParam;
	m_criticalSection.lock();
	//EnterCriticalSection(&m_criticalSection);
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end(); it++)
	{
		// 如果当前报单不是发出的报单而且是止损单
		if(it->iID != stOrder.iID && IsStopPLOrder(it->eOrderType))
		{
			PLParam *pParamTemp = (PLParam*)it->wParam;
			// 如果当前报单指向的是同一成交单，则删除
            // mod by Jerry Lee, 2013-3-11, 修正误删相同成交单号的止损止盈单的问题
			if( pParam->sMatchNo == pParamTemp->sMatchNo && stOrder.sExchId == it->sExchId)
			{
				SendMessage(it->iID, PreOrderMsgFlag_Del);
				EraseOrder(it);
				break;
			}
		}
	}
	m_criticalSection.unlock();
	//LeaveCriticalSection(&m_criticalSection);
}

bool CPreOrder::IsStopPLOrder( EPreOrderType eOrderType )
{
	if( eOrderType == PreOrderType_Profit || eOrderType == PreOrderType_Loss )
		return true;
	else
		return false;
}

void CPreOrder::HandleReceiveCovPosi( const string &sInsID, const string &sExchID, int iHand )
{
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end();)
	{
		if( iHand > 0 && IsStopPLOrder(it->eOrderType) && it->sProdCode == sInsID && it->sExchId == sExchID )
		{
			// 如果该笔止盈止损单的手数不大于平仓手数
			if( it->iHand <= iHand )
			{
				// 获得还需要平掉的仓位
				iHand -= it->iHand;

				// 删除相关联的单（先删除后面的，避免迭代器失效）
				IT itRelative = GetRelativeOrder(it);
				if( itRelative != m_vPreOrders.end() )
				{
					//kenny  2018-1-8
					SendMessage( itRelative->iID, PreOrderMsgFlag_Del);
					EraseOrder(itRelative);
				}

				// 删除当前单
				//kenny  2018-1-8
				SendMessage( it->iID, PreOrderMsgFlag_Del);
				it = EraseOrder(it);
			}
			else
			{
				it->iHand -= iHand;
				// 更新手数
				//kenny  2018-1-8
				SendMessage( it->iID, PreOrderMsgFlag_Clear);

				// 更新相关联单的手数
				IT itRelative = GetRelativeOrder(it);
				if( itRelative != m_vPreOrders.end() )
				{
					itRelative->iHand -= iHand;
					//kenny  2018-1-8
					SendMessage( itRelative->iID, PreOrderMsgFlag_Clear);
				}

				return;
			}
		}
		else
		{
			it++;
		}
	}
}

CPreOrder::IT CPreOrder::GetRelativeOrder( IT itCompare )
{
	vector<PreOrderStruct>::iterator it = m_vPreOrders.begin();
	for(; it != m_vPreOrders.end(); it++)
	{
		// 如果当前报单不是比较的报单而且是止损单
		if(it->iID != itCompare->iID && IsStopPLOrder(it->eOrderType))
		{
			PLParam *pParamTemp = (PLParam*)it->wParam;
			PLParam *pParamCmp = (PLParam*)itCompare->wParam;
			/*
			modify 2013/03/11 解决误删止盈止损单的问题
			如果当前报单指向的是同一成交单，而且是同一笔成交单的止盈止损单
			*/
			if( pParamCmp->sMatchNo == pParamTemp->sMatchNo && itCompare->sExchId == it->sExchId )
			{
				break;
			}
		}
	}

	return it;
}

void CPreOrder::SendMessage( int iOrderID, PreOrderMsgFlag iType )
{

	g_TraderCpMgr.m_mapBdr[E_PREORDER_CHANGE].Broadcast(WM_PREORDER_CHANGE, (void *)&iOrderID, (void *)&iType);

}

void CPreOrder::GetSendOrders( vector<int> &vSendOrders )
{
	m_criticalSection.lock();
	//EnterCriticalSection(&m_criticalSection);
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end(); it++)
	{
		// 是自动报单而且未发送才做处理
		if( it->eOrderType != PreOrderType_Hand && (it->eOrderState == E_NoSend) )
		{
			bool bSend = true; // 默认发送该笔报单

			// 处理条件报单的情况
			if( it->eOrderType != PreOrderType_AUTO ) 
			{
				// 根据条件获取需要的行情价格
				double dValue;

				// 获得整形的价格
				const QUOTATION &qt = g_TraderCpMgr.m_QMapQuotation[it->sProdCode]; // 需确保该位置上的数据不会被删除，否则不安全（当前能确保）
				unsigned int uiValue;
				if( it->eQTType == E_Latest ) // 最新价
					uiValue = qt.m_uiLast;
				else if( it->eQTType == E_Buy1 ) // 买一价
					uiValue = qt.m_Bid[0].m_uiPrice; 
				else // 卖一价
					uiValue = qt.m_Ask[0].m_uiPrice;

				// 价格不合法则不处理
				if( !qt.IsValidate(uiValue) )
					continue;

				dValue = uiValue/g_Global.m_dQTFactor;

				// 比较行情价格与报价（小于、等于或大于这三种情况）
				ECompareType eCompResult = ComparePrice( it->dCmpPrice, dValue );
				// 根据行情与报价的情况与报价的要求对比来决定是否发送
				if(eCompResult == E_Small ) // 如果当前行情价格小于报价
				{
					// 如果要求行情价格不小于报价则不发送
					if( it->eCompType != E_Small && it->eCompType != E_Small_Equal)
					{
						bSend = false;
					}
				}
				else if(eCompResult == E_Equal) // 如果行情与报价相等
				{
					/*
					如果要求行情小于报价或者行情大于报价（即行情和报价不等）
					modify by xrs 20140319 解决用户设置了比较条件为等于，而比较结果为等于不触发的问题
					*/
					if( it->eCompType != E_Small_Equal && it->eCompType !=  E_Big_Equal && it->eCompType !=  E_Equal)
					{
						bSend = false;
					}
					/*if( it->eCompType != E_Small_Equal && it->eCompType !=  E_Big_Equal )
					{
						bSend = false;
					}*/
				}
				else // 行情大于报价
				{
					if( it->eCompType != E_Big && it->eCompType != E_Big_Equal )
					{
						bSend = false;
					}
				}
				#ifdef _WRITE_LOG
				if( bSend )
				{
					QString csLog;
					csLog.sprintf("条件单%d满足条件，行情价格：%.2f 比较价格：%.2f", it->iID, dValue, it->dCmpPrice);
					g_Global.WriteLog(csLog);
				}
				#endif
			}
			else // 自动预埋单
			{
				// 未作处理，当该品种的状态为活跃状态的时候发出 
				// 现货，远期（连续交易） 延期（普通报单则判断是否是连续交易，交收则判断是否是交割申报，中立仓则判断是否是中立仓

				// 获得品种当前的交易状态ID
				const string &sInsStateID = CHJGlobalFun::qstr2str(g_TraderCpMgr.GetInsStateID( CHJGlobalFun::str2qstr(it->sProdCode))); // 确保不会被删除，否则不安全

				// 获取该笔报单的交易类型
				const string &sExchID = it->sExchId;
				if(sExchID == CONSTANT_EXCH_CODE_SPOT_BUY || sExchID == CONSTANT_EXCH_CODE_SPOT_SELL || sExchID == CONSTANT_EXCH_CODE_FORWARD_LONG || sExchID == CONSTANT_EXCH_CODE_FORWARD_SHORT) // 现货报单
				{
					// 只有连续交易状态下才能报单
					if(sInsStateID != "5") 
					{
						bSend = false;
					}
				}
				else if(sExchID == CONSTANT_EXCH_CODE_DEFER_OPEN_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT
					|| sExchID == CONSTANT_EXCH_CODE_DEFER_COV_LONG || sExchID == CONSTANT_EXCH_CODE_DEFER_COV_SHORT) // 延期报单
				{ 
					// 只有在竞价报单 连续交易 交割申报下才能报单
					if(sInsStateID != "3" && sInsStateID != "5" && sInsStateID != "7")
					{
						bSend = false;
					}
				}
				else if(sExchID == CONSTANT_EXCH_CODE_DELIVERY_LONG || sExchID == CONSTANT_EXCH_CODE_DELIVERY_SHORT) // 交收
				{
					// 只有在交割申报状态下才能报单
					if(sInsStateID != "7")
					{
						bSend = false;
					}
				}
				else if(sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG || sExchID == CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT) // 中立仓
				{
					// 只有中立仓状态下才能报单
					if(sInsStateID != "9")
					{
						bSend = false;
					}
				}
				else // 未作处理
				{
					bSend = false;
				}
			}

			if(bSend)
			{
				vSendOrders.push_back(it->iID);
			}
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
}

// 将行情价格与报价对比：如果行情大于报价，则返回大于，如果行情小于报价，则返回小于，如果行情等于报价，则返回等于
ECompareType CPreOrder::ComparePrice( double dAsk, double dQT )
{
	if(dQT - dAsk > 0.005)
		return E_Big;
	else if(dAsk - dQT > 0.005)
		return E_Small;
	else
		return E_Equal;
}

void CPreOrder::CopyOrder( PreOrderStruct &stOrder, const PreOrderStruct &stOrgOrder )
{
	stOrder = stOrgOrder;
}

void CPreOrder::ClearPreOrder()
{
	m_criticalSection.lock();
	//EnterCriticalSection(&m_criticalSection);
	m_vPreOrders.clear();
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();

	SendMessage(0, PreOrderMsgFlag_Clear);
}

bool CPreOrder::HasUnSendOrder()
{
	bool bReturn = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end(); it++)
	{
		if( it->eOrderState == E_NoSend )
		{
			bReturn = true;
			break;
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();

	return bReturn;
}

//kenny  2018-1-8  添加的线程处理函数
void CPreOrder::CommitAskAll()
{
	while (bRun)
	{
		vector<int> vSendOrdes;
		GetSendOrders(vSendOrdes);

		// 根据预埋单号发送报单
		for (size_t i = 0; i < vSendOrdes.size(); i++)
		{
			CommitAsk(vSendOrdes[i]);
		}

		Sleep(50);
	}

}
/*
int CAutoSendThread::Run()
{
	// 获取需要发送的预埋单号
	vector<int> vSendOrdes;
	m_pPreOrder->GetSendOrders(vSendOrdes);

	// 根据预埋单号发送报单
	for( size_t i = 0; i < vSendOrdes.size(); i++ )
	{
		m_pPreOrder->CommitAsk(vSendOrdes[i]);
	}

	return 0;
}

// 初始化
void CAutoSendThread::Set( CPreOrder* pPreOrder )
{
	m_pPreOrder = pPreOrder;
}
*/
int CPreOrderMgr::Add( PreOrderStruct &stPreOrder )
{
	stPreOrder.iID = m_iPreOrderID++;
	m_criticalSection.lock();
	//EnterCriticalSection(&m_criticalSection);
	m_vPreOrders.push_back(stPreOrder);
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
	return stPreOrder.iID;
}

int CPreOrderMgr::AddEx( const PreOrderStruct &stPreOrder )
{
	PreOrderStruct stAddPreOrder = stPreOrder;
	stAddPreOrder.iID = m_iPreOrderID++;

	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	m_vPreOrders.push_back(stAddPreOrder);
	m_criticalSection.unlock();
	//LeaveCriticalSection(&m_criticalSection);

	return stAddPreOrder.iID;
}

bool CPreOrderMgr::Erase( int iOrderID )
{
	bool bFind = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end();it++)
	{
		if(it->iID == iOrderID)
		{
			m_vPreOrders.erase(it);
			bFind = true;
			break;
		}
	}
	m_criticalSection.unlock();
	//LeaveCriticalSection(&m_criticalSection);

	return bFind;
}

CPreOrderMgr::CPreOrderMgr( void )
{
	m_iPreOrderID = 0;
	//InitializeCriticalSection(&m_criticalSection);
}

bool CPreOrderMgr::CreatePreOrder( int iOrderID )
{
	bool bFind = false;
	//EnterCriticalSection(&m_criticalSection);
	m_criticalSection.lock();
	for(vector<PreOrderStruct>::iterator it = m_vPreOrders.begin(); it != m_vPreOrders.end();it++)
	{
		if(it->iID == iOrderID)
		{
			// 如果是服务器预埋单
			if( it->bServer )
			{

			}
			// 如果是客户端预埋单
			else
			{

			}

			bFind = true;
			break;
		}
	}
	//LeaveCriticalSection(&m_criticalSection);
	m_criticalSection.unlock();
	return bFind;
}

CPreOrderMgr::~CPreOrderMgr( void )
{
	//DeleteCriticalSection(&m_criticalSection);
}

QString PreOrderBusiness::GetOrderTypeName( const PreOrderStruct &stPreOrder )
{
	QString sValue;
	if( stPreOrder.eOrderType == PreOrderType_Loss )
		sValue = QString::fromLocal8Bit( "止损单");
	else if( stPreOrder.eOrderType == PreOrderType_Profit )
		sValue = QString::fromLocal8Bit("止盈单");
	else if( stPreOrder.eOrderType == PreOrderType_Hand )
		sValue = QString::fromLocal8Bit("手动预埋单");
	else if( stPreOrder.eOrderType == PreOrderType_AUTO )
		sValue = QString::fromLocal8Bit("自动预埋单");
	else if(stPreOrder.eOrderType == PreOrderType_Condition )
		sValue = QString::fromLocal8Bit("条件单");

	if( stPreOrder.bServer )
	{
		sValue = sValue + QString::fromLocal8Bit("（服务器）");
	}
	else
	{
		sValue = sValue + QString::fromLocal8Bit("（本地）");
	}

	return sValue;
}

QString PreOrderBusiness::GetAskPrice( const PreOrderStruct &stPreOrder )
{
	QString csValue;
	if( !IsDeferSpecial(CHJGlobalFun::str2qstr(stPreOrder.sExchId) ))
	{
		if( stPreOrder.eOrderType == PreOrderType_Loss || stPreOrder.eOrderType == PreOrderType_Profit )
		{
			csValue = GetPriceTypeName(stPreOrder.eAskPriceType);
			PLParam *pPLParam = (PLParam*)stPreOrder.wParam;
			if( pPLParam->iAjust > 0 )
			{
				QString csTemp = QString("+%1T").arg(pPLParam->iAjust);

				csValue += csTemp;
			}
			if( pPLParam->iAjust < 0 ) 
			{
				QString csTemp = QString("-%1T").arg(-pPLParam->iAjust);

				csValue += csTemp;
			}
		}
		else
		{
			csValue = QString::number(  stPreOrder.dPrice,'f',2);
		}
	}
	else
	{
		csValue = "";
	}

	return csValue;
}

QString PreOrderBusiness::GetTriConditionName( const PreOrderStruct &stPreOrder )
{
	if(stPreOrder.eOrderType == PreOrderType_Hand)
	{
		return QString::fromLocal8Bit("手动");
	}
	if(stPreOrder.eOrderType == PreOrderType_AUTO)
	{
		return QString::fromLocal8Bit("自动");
	}
	else // 条件单
	{
		QString csValue = QString::fromLocal8Bit("当%1%2%3时发出")
			.arg(GetPriceTypeName(stPreOrder.eQTType))
			.arg(GetCompareTypeName(stPreOrder.eCompType))
			.arg(QString::number(stPreOrder.dCmpPrice,'f',2));
		

		return csValue;
	}
}

QString PreOrderBusiness::GetCompareTypeName( ECompareType eCmpType )
{
	switch (eCmpType)
	{
	case E_Small:        return QString::fromLocal8Bit("小于");       // ＜ <
	case E_Small_Equal:  return QString::fromLocal8Bit("小于或等于"); // ≤
	case E_Big:          return QString::fromLocal8Bit("大于");       // ＞
	case E_Big_Equal:    return QString::fromLocal8Bit("大于或等于"); // ≥
	case E_Equal:        return QString::fromLocal8Bit("等于"); // ≥
	default: return "";
	}
}

QString PreOrderBusiness::GetPriceTypeName( EQTType eQTType )
{
	switch (eQTType)
	{
	case E_Latest: return QString::fromLocal8Bit("最新价");
	case E_Sell1:  return QString::fromLocal8Bit("卖一价");
	case E_Buy1:   return QString::fromLocal8Bit("买一价");
	default: return "";
	}
}

QString PreOrderBusiness::GetPreOrderTips( const PreOrderStruct &stPreOrder )
{
	QString csTips = QString::fromLocal8Bit("预埋单类型：%1\r\n合约代码：%2\r\n交易类型：%3\r\n  价格：%4\r\n  手数：%5\r\n触发条件：%6\r\n关联成交单号：%7\r\n")
		.arg(GetOrderTypeName(stPreOrder))
		.arg(CHJGlobalFun::str2qstr(stPreOrder.sProdCode))
		.arg(g_TraderCpMgr.GetExchName(CHJGlobalFun::str2qstr(stPreOrder.sExchId)))
		.arg(GetAskPrice(stPreOrder))
		.arg(CHJGlobalFun::IntToQString(stPreOrder.iHand))
		.arg(GetTriConditionName(stPreOrder))
		.arg(GetRelateMatchNo(stPreOrder));


	return csTips;
}

QString PreOrderBusiness::GetRelateMatchNo( const PreOrderStruct &stPreOrder )
{
	if( stPreOrder.wParam != 0 )
	{
		const PLParam* para = (const PLParam*)stPreOrder.wParam;
		return CHJGlobalFun::str2qstr(para->sMatchNo);
	}

	return "";
}

QString PreOrderBusiness::GetPreOrderLog( const PreOrderStruct &stPreOrder )
{
	QString csTips = QString::fromLocal8Bit("预埋单类型：%1 合约代码：%2 交易类型：%3 价格：%4 手数：%5 触发条件：%6 关联成交单号：%7")
		.arg(GetOrderTypeName(stPreOrder))
		.arg(CHJGlobalFun::str2qstr(stPreOrder.sProdCode))
		.arg(g_TraderCpMgr.GetExchName(CHJGlobalFun::str2qstr(stPreOrder.sExchId)))
		.arg(GetAskPrice(stPreOrder))
		.arg(CHJGlobalFun::IntToQString(stPreOrder.iHand))
		.arg(GetTriConditionName(stPreOrder))
		.arg(GetRelateMatchNo(stPreOrder));


	return csTips;
}
