#include "StopPLMgr.h"

//#include "IniMgr.h"
#include "Global.h"
#include "HashtableMsg.h"
#include "TraderCpMgr.h"
#include "HJGlobalFun.h"
#include "HJConstant.h"
#include "Mgr/ProdCodeMgr.h"
#include "TranMsg.h"
#include "CustomInfoMgrEx.h"
#include "app.h"

#pragma execution_character_set("utf-8")


CStopPLMgr::CStopPLMgr(void)
{
	m_bMatchNoChange = false;
	m_bUseServer = false;
}

CStopPLMgr::~CStopPLMgr(void)
{
	// 程序退出时如果m_vecHandledMatchNo有变化，即重新写入到配置文件
	/*if( m_bMatchNoChange )
	{
		WriteMatchNo();
	}*/
}

CStopPLMgr& CStopPLMgr::GetInstance()
{
	static CStopPLMgr theSingleton;
	return theSingleton;
}

void CStopPLMgr::Initialize(  )
{
	LoadSetting();

	LoadHandledMatchNo();
}

// 获取一个品种的止损止盈配置信息
PLInfo* CStopPLMgr::GetPLInfo( const string &sInsID )
{
	map<string, PLInfo>::iterator it = m_mapInfo.find( sInsID );
	if( it != m_mapInfo.end() )
	{
		return &it->second;
	}
	return NULL;
}

/*
stMatch 成交单流水结构
bLong 是否是多仓，false即为空仓
*/
void CStopPLMgr::HandleOpenPosiMatch( const DeferMatch &stMatch, bool bLong)
{
	// 如果不能修改文件，则不处理止盈止损，避免每次登陆都会重复处理止盈止损单
	if( !g_Global.m_bEnableIniFile )
		return;

	QString sRecordMatchNO(CHJGlobalFun::str2qstr(stMatch.matchNo));
	sRecordMatchNO += bLong ? "1" : "0";
	if( !IsHandled(sRecordMatchNO) )
	{
		// 添加成交单号
		AddMatchNo( sRecordMatchNO );

		// 获取该品种的止盈止损配置信息
		const PLInfo *pInfo = GetPLInfo( stMatch.instID );
		if( pInfo != NULL ) // 如果该品种有配置
		{
			// 获取该品种的信息（主要是为了获取tick，即最小价格单位）
			const ProdCodeInfo* pProCodeInfo = g_ProdCodeMgr.GetProdeCode( CHJGlobalFun::str2qstr(stMatch.instID ));
			if( pProCodeInfo != NULL )
			{
				// 获取品种的最小价格单位
				double fPriceUnite = pProCodeInfo->tick;

				// 获取基本比较价格
				double dCmpPrice;
				if( GetBaseCmpPrice(dCmpPrice, m_eCmpType, stMatch, bLong) )
				{
#ifdef _Write_StopPL_Log
					if( m_eCmpType == E_PLMatchPrice )
						g_Global.WriteLog("采取成交价格");
					else
						g_Global.WriteLog("采取持仓均价");
#endif
					if( m_bUseServer ) // 服务器止盈止损
					{
						bool bMatchNoChange = false;
						if( pInfo->iStopProfitAjust > 0 || pInfo->iStopLossAjust > 0 )
						{
							Req4074 req;
							req.oper_flag = 2; // 添加
							req.exch_type = bLong ? CONSTANT_EXCH_CODE_DEFER_COV_LONG : CONSTANT_EXCH_CODE_DEFER_COV_SHORT;; // 交易方式
							req.prod_code = stMatch.instID; // 品种

							char temp[3];
							sprintf(temp, "%d", GetPreOrderAskPriceType( m_eAskPriceType, bLong ));
							req.entr_price_type = temp; // 报单价格类型
							req.entr_amount = stMatch.volume;; // 手数
							req.entr_adjust_step = bLong ? -m_iAskPriceAjust : m_iAskPriceAjust; // 报单调整价位
							req.src_match_no = stMatch.matchNo;

							//req.trigger_value = CHJGlobalFun::DoubleToString(dCmpPrice,false); // 基础比较价格
							req.trigger_value = CHJGlobalFun::qstr2str(QString::number(dCmpPrice)); // 基础比较价格
						

							
							// 处理止盈
							if( pInfo->iStopProfitAjust > 0 )
							{
#ifdef _Write_StopPL_Log
								g_Global.WriteLog("处理止盈单");
#endif

								req.delegate_type = "2";  // 止盈单
								req.trigger_condition = bLong ? "0" : "1"; // 比较类型，多仓则大于或等于，空仓则为小于或等于

								//req.trigger_value_type = bLong ? "0" : "1"; // 多仓采用买1价，空仓采用卖1价

								req.base_adjust_step = bLong ? pInfo->iStopProfitAjust : -pInfo->iStopProfitAjust;

                                // mod by Jerry Lee, 2013-4-22, 按照金联通要求使用最新价触发
								//sprintf(temp, "%d", bLong ? E_Buy1 : E_Sell1);
                                sprintf(temp, "%d", E_Latest);
								req.trigger_value_type = temp;

								Rsp4074 rsp;
								if( CTranMsg::Handle4074(rsp, req) )
								{
									bMatchNoChange = true;
								}
							}

							// 处理止损
							if( pInfo->iStopLossAjust > 0 )
							{
#ifdef _Write_StopPL_Log
								g_Global.WriteLog("处理止损单");
#endif
								req.delegate_type = "1";  // 止损单
								req.trigger_condition = bLong ? "1" : "0"; // 比较类型，多仓则小于或等于，空仓则为大于
								//req.trigger_value_type = "2"; // 最新价

								req.base_adjust_step = bLong ? -pInfo->iStopLossAjust : pInfo->iStopLossAjust; // 触发调整价位

								sprintf(temp, "%d", E_Latest);
								req.trigger_value_type = temp;

								Rsp4074 rsp;
								if( CTranMsg::Handle4074(rsp, req) )
								{
									bMatchNoChange = true;
								}
							}
#ifdef _Write_StopPL_Log
							g_Global.WriteLog("开始服务器止盈止损刷新广播");
#endif

							if( bMatchNoChange )
							{
								g_TraderCpMgr.m_mapBdr[E_SER_PREORDER_REFRESH].Broadcast(WM_SER_PREORDER_REFRESH,0,0);
							}

#ifdef _Write_StopPL_Log
							g_Global.WriteLog("结束服务器止盈止损刷新广播");
#endif
						}
					}
					else
					{
						PreOrderStruct stPreOrder;
						stPreOrder.sProdCode = stMatch.instID;
						stPreOrder.sExchId = bLong ? CONSTANT_EXCH_CODE_DEFER_COV_LONG : CONSTANT_EXCH_CODE_DEFER_COV_SHORT;//stMatch.sExchTypeCode;
						stPreOrder.iHand = stMatch.volume;
						stPreOrder.sTime =CHJGlobalFun::qstr2str( CHJGlobalFun::GetCurTimeFormat());
						stPreOrder.eAskPriceType = GetPreOrderAskPriceType( m_eAskPriceType, bLong );

						// 根据报单时价格类型获取行情的指针
						const unsigned int *pHQPrice = GetHQPricePointer( stPreOrder.eAskPriceType, stMatch.instID);

						int iAskPriceAjust = bLong ? -m_iAskPriceAjust : m_iAskPriceAjust;

						// 处理止盈单
						if( pInfo->iStopProfitAjust > 0 )
						{
							stPreOrder.dCmpPrice =  bLong ? dCmpPrice + pInfo->iStopProfitAjust*fPriceUnite : dCmpPrice - pInfo->iStopProfitAjust*fPriceUnite;
							stPreOrder.eCompType = bLong ? E_Big_Equal : E_Small_Equal;
                            // mod by Jerry Lee, 2013-4-22, 按照金联通要求使用最新价触发
							//stPreOrder.eQTType = bLong ? E_Buy1 : E_Sell1;
                            stPreOrder.eQTType = E_Latest;

							stPreOrder.wParam = (void *)new PLParam( pHQPrice, iAskPriceAjust, bLong, stMatch.matchNo );
							stPreOrder.eOrderType = PreOrderType_Profit;

							// 添加一笔预埋单
							int iOrderID = AddPreOrder( stPreOrder );

							//WLog(E_LOG_NORMAL,"生成止盈止损单，%s", PreOrderBusiness::GetPreOrderLog(stPreOrder));
						}

						short nLossAjust = GetLossAjust(pInfo);
						if( nLossAjust != 0 )
						{
							stPreOrder.dCmpPrice =  bLong ? dCmpPrice - nLossAjust*fPriceUnite : dCmpPrice + nLossAjust*fPriceUnite;
							stPreOrder.eCompType = bLong ? E_Small_Equal : E_Big_Equal;
							stPreOrder.eQTType = E_Latest;

							LossParam *pLossParam = new LossParam( pHQPrice, iAskPriceAjust, bLong, stMatch.matchNo );

                            // added by Jerry Lee, 2013-3-31, 以成交价为基准价，修正追踪止损价格计算错误的问题
							pLossParam->uiExtreme = dCmpPrice*g_Global.m_dQTFactor+0.5;//g_TraderCpMgr.m_mapQuotation[stMatch.instID].m_uiLast;

							pLossParam->uiDys = GetLossPrice( pInfo->iDysAjust, dCmpPrice, bLong, fPriceUnite )*g_Global.m_dQTFactor+0.5;
							pLossParam->uiStatic = GetLossPrice( pInfo->iStopLossAjust, dCmpPrice, bLong, fPriceUnite )*g_Global.m_dQTFactor+0.5;

							stPreOrder.wParam = pLossParam;
							stPreOrder.eOrderType = PreOrderType_Loss;

							// 添加一笔预埋单
							int iOrderID = AddPreOrder( stPreOrder );

                            //WLog(E_LOG_NORMAL,"生成止盈止损单，%s", PreOrderBusiness::GetPreOrderLog(stPreOrder));
						}
					}
				}
			}
		}
	}
}

// 获取触发条件的基本价格
bool CStopPLMgr::GetBaseCmpPrice( double &dBaseCmpPrice, EPLCmpType eCmpType, const DeferMatch &stMatch, bool bLong )
{
	if( eCmpType == E_PLMatchPrice ) // 成交价格
	{
		dBaseCmpPrice = stMatch.price;
		return true;
	}
	else // 持仓均价
	{
		DeferPosi stPosi;
		if(  g_CusInfo.GetPosiInfo(stPosi,stMatch.instID) )
		{
			dBaseCmpPrice = bLong ? stPosi.infoLong.dAvgPosPrice : stPosi.infoShort.dAvgPosPrice;
			return true;
		}
	}

	return false;
}

int CStopPLMgr::AddPreOrder( PreOrderStruct &stPreOrder )
{
	// 插入一笔预埋单
	int iOrderID = g_TraderCpMgr.m_cPreOrder.Add(stPreOrder);

	// 广播消息
	g_TraderCpMgr.m_cPreOrder.SendMessage(iOrderID, PreOrderMsgFlag_Add);

	return iOrderID;
}

/*
设置止损单公用函数
止损单都是最新价与触发价格（dCmpPrice）来比较
*/
void CStopPLMgr::SetStopLoss( PreOrderStruct &stPreOrder, double dCmpPrice, double fPriceUnite, bool bLong, short nLossAjust )
{
	// 获取止损价格：如果是开多仓，则止损价格为比较价-偏移价；如果开空仓，则止损价+偏移价
	stPreOrder.dCmpPrice =  bLong ? dCmpPrice - nLossAjust*fPriceUnite : dCmpPrice + nLossAjust*fPriceUnite;
	stPreOrder.eCompType = bLong ? E_Small_Equal : E_Big_Equal;
	stPreOrder.eQTType = E_Latest;
}

/*
根据报单价格类型获取行情中对应字段的指针
modify by xrs 20130129
*/
const unsigned int* CStopPLMgr::GetHQPricePointer( EQTType eAskPriceType, const string &sInsID )
{
	const QUOTATION& qt = g_TraderCpMgr.m_QMapQuotation[sInsID];
	if( eAskPriceType == E_Buy1 ) // 买一价
		return &qt.m_Bid[0].m_uiPrice;
	else if( eAskPriceType == E_Sell1 ) // 卖一价
		return &qt.m_Ask[0].m_uiPrice;
	else
		return &qt.m_uiLast; // 最新价
}

EPLCmpType CStopPLMgr::GetCmpType()
{
	return m_eCmpType;
}

EPLAskPriceType CStopPLMgr::GetAskPriceType()
{
	return m_eAskPriceType;
}

short CStopPLMgr::GetAskPriceAjust()
{
	return m_iAskPriceAjust;
}

const map<string, PLInfo>* CStopPLMgr::GetInfo()
{
	return &m_mapInfo;
}

/*
获取止盈止损单的报单价格类型
*/
EQTType CStopPLMgr::GetPreOrderAskPriceType( EPLAskPriceType eAskPriceType, bool bLong )
{
	if( eAskPriceType == e_PLLatestPrice ) // 最新价
		return E_Latest;
	else // 买卖价
		return  bLong ? E_Buy1 : E_Sell1; // 如果是开多仓，则采用买1价，如果是开空仓，则采用卖1价
}

short CStopPLMgr::GetLossAjust( const PLInfo *plInfo )
{
	short nLossAjust = 0;
	if( plInfo->iStopLossAjust > 0 && plInfo->iDysAjust == 0 )
		nLossAjust = plInfo->iStopLossAjust;
	else if( plInfo->iDysAjust > 0 && plInfo->iStopLossAjust == 0 )
		nLossAjust = plInfo->iDysAjust;
	else if( plInfo->iDysAjust > 0 && plInfo->iStopLossAjust > 0 )
		nLossAjust = (plInfo->iStopLossAjust > plInfo->iDysAjust) ? plInfo->iDysAjust : plInfo->iStopLossAjust;

	return nLossAjust;
}

double CStopPLMgr::GetLossPrice( short Ajust, double dCmpBasePrice, bool bLong, double fPriceUnite )
{
	if( Ajust != 0 )
		return  bLong ? dCmpBasePrice - Ajust*fPriceUnite : dCmpBasePrice + Ajust*fPriceUnite;
	else
		return 0.00;
}

bool CStopPLMgr::IsHandled( const QString &sMatchNo )
{
	auto it = m_mapHandledMatchNo.find(sMatchNo);
	if (it != m_mapHandledMatchNo.end())
	{
		return true;
	}
	return false;
	for(size_t i = 0; i< m_vecHandledMatchNo.size(); i++)
	{
		if( sMatchNo == m_vecHandledMatchNo[i] )
			return true;
	}

	return false;
}

bool CStopPLMgr::IsServer()
{
	return m_bUseServer;
}

bool CStopPLMgr::WriteMatchNo()
{
	QString csValue(g_Global.m_strExchDate);
	csValue += "∧";
	for(size_t i = 0; i< m_vecHandledMatchNo.size(); i++ )
	{
		csValue += m_vecHandledMatchNo[i];
		csValue += "^";
	}

#ifdef _Write_Test_ReBuild_PL_Log
	CString csLog("写入已处理成交单号：");
	csLog += csValue;
	g_Global.WriteLog(csLog);
#endif

#ifdef _Write_Test_ReBuild_PL_Log
	if( ::WritePrivateProfileString("Info", "HandledMatchNo", csValue, g_Global.GetUserDataIniPath()) )
	{
		g_Global.WriteLog("写入已处理成交单号成功");
	}
	else
	{
		g_Global.WriteLog("写入已处理成交单号失败");
	}
#else
	//::WritePrivateProfileString("Info", "HandledMatchNo", csValue, g_Global.GetUserDataIniPath());
	App::WritePriProfileString(g_Global.GetUserDataIniPath(),"Info", "HandledMatchNo", csValue);
#endif

	return true;
}

void CStopPLMgr::AddMatchNo( const QString &sMatchNo )
{
	// 记录没处理过的成交单
	m_vecHandledMatchNo.push_back(sMatchNo);
	m_mapHandledMatchNo.insert(make_pair(sMatchNo, sMatchNo));
	WriteMatchNo();
}

void CStopPLMgr::LoadSetting()
{
	m_mapInfo.clear();

	/*
	读取品种基本配置，参照设置里面的止盈止损上面部分的配置
	*/
	QString csValue;
	if( App::GetPriProfileString(g_Global.GetUserIniPath(),"PreOrder", "StopPLSetting",csValue))
		//CIniMgr::GetStringValue(csValue, "PreOrder", "StopPLSetting", g_Global.GetUserIniPath()) )
	{
		QVector<int> vSetting;
		CHJGlobalFun::SplitStrToVector(csValue, "^", vSetting);
		if( vSetting.size() >= 4 )
		{
			// 根据金联通版本V1.1修改
#ifdef _VERSION_GF
			m_eCmpType = E_PLMatchPrice;
			m_eAskPriceType = e_PLLatestPrice;
#else
			m_eCmpType = (EPLCmpType)vSetting[0];
			m_eAskPriceType = (EPLAskPriceType)vSetting[1];
#endif
			
			m_iAskPriceAjust = vSetting[2];
			m_bUseServer = vSetting[3];

			// 从配置文件读取每个品种的止盈止损配置
			QString sValue;
			if(App::GetPriProfileString( g_Global.GetUserIniPath(), "PreOrder", "InstSetting",sValue) )
			{
				HashtableMsg htm;
				if( htm.Parse(CHJGlobalFun::qstr2str(sValue)) == 0 )
				{
					PLInfo info;
					for( size_t i = 0; i < htm.size(); i++ )
					{
						info.iStopProfitAjust = atoi(htm.GetString(i, "StopProfitAjust").c_str()); // 止盈调整价位
						info.iStopLossAjust = atoi(htm.GetString(i, "StopLossAjust").c_str()); // 止损调整价位
						info.iDysAjust = atoi(htm.GetString(i, "DysAjust").c_str()); // 自动追踪止损调整价位

						m_mapInfo[htm.GetString(i, "InsID")] = info;
					}
				}
			}
		}
	}
}

void CStopPLMgr::LoadHandledMatchNo()
{
	m_vecHandledMatchNo.clear();
	m_mapHandledMatchNo.clear();

	string sValue = "5000";
	QString buf;
	if(App::GetPriProfileString(g_Global.GetUserDataIniPath(), "Info", "HandledMatchNo",buf) )
	{
		sValue = CHJGlobalFun::qstr2str(buf);
		//sValue = "20121022∧2012101902000002^2012101902000011^2012101902000005^2012101902000016^2012101902000017^2012101902000018^2012101902000019^2012101902000020^2012101902000022^2012101902000023^2012101902000025^2012101902000026^2012101902000027^2012101902000028^2012101902000030^2012101902000031^2012101902000032^2012101902000033^2012101902000034^2012101902000036^2012101902000037^2012101902000039^2012101902000041^2012101902000042^2012101902000043^2012101902000045^2012101902000048^2012101902000049^2012101902000050^2012101902000051^2012101902000076^2012101902000078^2012101902000079^2012101902000080^2012101902000081^";
		int iPos = sValue.find("∧");
		if( iPos != -1 )
		{
			if(sValue.substr(0, iPos) == CHJGlobalFun::qstr2str(g_Global.m_strExchDate))
			{
#ifdef _Write_Test_ReBuild_PL_Log
				CString csLog("初始化已处理成交单号：");
				csLog += sValue.c_str();
				g_Global.WriteLog(csLog);
#endif

				sValue = sValue.substr(iPos+2);
				CHJGlobalFun::SplitStrToVector(CHJGlobalFun::str2qstr(sValue), "^", m_vecHandledMatchNo);
				for (size_t i = 0; i < m_vecHandledMatchNo.size(); i++)
				{
					m_mapHandledMatchNo.insert(make_pair(m_vecHandledMatchNo[i], m_vecHandledMatchNo[i]));
				}
			}
			else
			{
				//::WritePrivateProfileString("Info", "HandledMatchNo", "", g_Global.GetUserDataIniPath()); 
				App::WritePriProfileString(g_Global.GetUserDataIniPath(),"Info", "HandledMatchNo", "");

#ifdef _Write_Test_ReBuild_PL_Log
				g_Global.WriteLog("交易日期不符合，清除该字段");
#endif
			}
		}
	}
}
