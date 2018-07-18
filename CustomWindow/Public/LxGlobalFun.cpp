#include "LxGlobalFun.h"


// 根据市场类型（strMarketID）初始化合约代码
void CLxGlobalFun::IniProdCode()
{
	
}

/*
获取合约代码对应的id
*/
string CLxGlobalFun::GetProdCode()
{

	return "";
}
string CLxGlobalFun::GetProdCodeName(const string &strID)
{
	return "";
}


string CLxGlobalFun::GetCurrency(TRADE_MONEY_KIND type)
{
	string strTmp = "";
	switch (type)
	{
	case TD_CNY:
		strTmp = "人民币";
		break;
	case TD_USD:
		strTmp = "美元";
		break;
	case TD_GBP:
		strTmp = "英镑";
		break;
	case TD_JPY:
		strTmp = "日元";
		break;
	case TD_HKD:
		strTmp = "港币";
		break;
	case TD_CAD:
		strTmp = "加拿大元";
		break;
	case TD_EUR:
		strTmp = "欧元";
		break;
	case TD_AUD:
		strTmp = "澳元";
	case TD_CHF:
		strTmp = "瑞士法郎";
		break;
	case TD_RUB:
		strTmp = "俄罗斯卢布";
		break;
    default:	
		strTmp = "法国法郎";
		break;
	}

	return strTmp;
}

string CLxGlobalFun::GetCurrency(const string & type)
{
	string strTmp = type;
	if (type == "CNY")
	{
		return "人民币";
	}
	else if (type == "USD")
	{
		return  "美元";
	}
	else if (type == "GBP")
	{
		return "英镑";
	}
	else if (type == "JPY")
	{
		return "日元";
	}
	else if (type == "HKD")
	{
		return "港币";
	}
	else if (type == "CAD")
	{
		return "加拿大元";
	}
	else if (type == "EUR")
	{
		return "欧元";
	}
	else if (type == "AUD")
	{
		return "澳元";
	}
	else if (type == "CHF")
	{
		return "瑞士法郎";
	}
	else if (type == "RUB")
	{
		return "俄罗斯卢布";
	}else
	{
		return "人民币";//法国法郎
	}

	return "";
}

/*
获取交易类型
*/
string CLxGlobalFun::GetExchCode()
{
	string strTmp = "";
	/*switch (type)
	{
	case FTDC_TRDT_deault:
		strTmp = "未知";
		break;
	case RDT_SplitCombination:
		strTmp = "组合持仓拆分为单一持仓,初始化不应包含该类型的持仓";
		break;
	case FTDC_TRDT_Common:
		strTmp = "普通成交";
		break;
	case FTDC_TRDT_OptionsExecution:
		strTmp = "期权执行";
		break;
	case FTDC_TRDT_OTC:
		strTmp = "OTC成交";
		break;
	case FTDC_TRDT_EFPDerived:
		strTmp = "期转现衍生成交";
		break;
	case FTDC_TRDT_CombinationDerived:
		strTmp = "组合衍生成交";
		break;
	default:
		strTmp = "未知";
		break;
	}*/

	return strTmp;
}
// 初始化开平标志
void CLxGlobalFun::IniOffsetFlag()
{
}


string CLxGlobalFun::FormatPrice(const double &price)
{
	char   szTmp[32] = {0};
	sprintf(szTmp, "%.2f", price);
	return szTmp;
}

string CLxGlobalFun::StrFromIntVolume(const int &vol)
{
	char   szTmp[32] = { 0 };
	sprintf(szTmp, "%d", vol);
	return szTmp;

}

string CLxGlobalFun::StrFromDoubleVolume(const double &vol)
{
	char   szTmp[32] = { 0 };
	sprintf(szTmp, "%.2f", vol);
	return szTmp;
}


/*
获取开平标志
*/
string CLxGlobalFun::GetOffsetFlag(TRADE_OPEN_CLOSE flag)
{
	if (flag == TOC_OPEN)
	{
		return "开";
	}
	else if (flag == TOC_CLOSE)
	{
		return "平";
	}
	else if (flag == TOC_EXTENT)
	{
		return "其它";
	}
	else if (flag == TOC_ForceClose)
	{
		return "强平";
	}
	else if (flag == TOC_CLOSE_Today)
	{
		return "平今";
	}
	else if (flag == TOC_Close_Yesterday)
	{
		return "平";// "平昨";需求展示为"平"
	}
	else if (flag == TOC_ForceOff)
	{
		return "强减";
	}
	else if (flag == TOC_LocalForceClose)
	{
		return "本地强平";
	}

	return "";
}
// 初始化持仓方向下拉菜单
void CLxGlobalFun::IniLongShort()
{
	
}
// 获取持仓方向id
string CLxGlobalFun::GetLongShort()
{
	return "";
	
}

// 获取存取方向id
std::string CLxGlobalFun::GetAccessWay()
{
	return "";
	
}

// 初始化交割品种代码
void CLxGlobalFun::IniVarietyId()
{
}

// 获取交割品种代码ID
std::string CLxGlobalFun::GetVarietyId()
{
	return "";
}



/*
功能：往vector(v)添加一个标题为strtitle值为strvalue的pare(p)
v vector
p 参数对
strtitile 标题
strvalue 标题的值
*/
void CLxGlobalFun::PairAdd(vector< pair<string, string> > & v, string strtitle, string strvalue)
{
	pair<string,string> p;
	p.first  = strtitle;
	p.second = strvalue;
	v.push_back(p);
}



// 根据交易市场ID获取交易市场名称
std::string CLxGlobalFun::GetMarketName(const string &strID)
{
	//遍历交易市场

	return strID;
}

std::string CLxGlobalFun::GetHedgeFlag(POSI_HF_PLAG type)
{
	string strTmp;
	switch (type)
	{
	case THOST_HF_Speculation:
		strTmp = "投机";
		break;
	case THOST_HF_Arbitrage:
		strTmp = "套利";
		break;
	case THOST_HF_Hedge:
		strTmp = "套保";
		break;
	case THOST_HF_MarketMaker:
		strTmp = "做市商";
		break;
	default:
		strTmp = "";
		break;
	}

	return  strTmp;
}

string CLxGlobalFun::GetTodayOrYestoday(POSI_DATE m_nPosiDate)
{
	string strTmp;
	switch (m_nPosiDate)
	{
	case THOST_FTDC_POSI_TODAY:
		strTmp = "今仓";
		break;
	case THOST_FTDC_POSI_HISTORY:
		strTmp = "昨仓";
		break;
	default:
		strTmp = "";
		break;
	}

	return  strTmp;
}

string CLxGlobalFun::GetStatusString(ACCOUNT_STATUS status)
{
	string str;
	switch (status)
	{
	case AS_NULL:
		str = "无账号";
		break;
	case AS_NORMAL:
		str = "正常（未登录）";
		break;
	case AS_LOGINING:
		str = "登录中...";
		break;
	case AS_LOGIN:
		str = "已登录";
		break;
	case AS_CONNECTED:
		str = "已连接";
		break;
	case AS_DISCONNECTED:
		str = "连接断开";
		break;
	case AS_GET_INVESTOR:
		str = "取得投资者名称";
		break;
	case AS_GET_INSTRUMENTED:
		str = "取得交易所合约信息";
		break;
	case AS_ACCOUNT_CAPITAL_UPDATE:
		str = "账户资金更新";
		break;
	case AS_POSITIONS_UPDATE:
		str = "持仓更新";
		break;
	case AS_POSITIONS_DETAIL_UPDATE:
		str = "持仓明细更新";
		break;
	case AS_TRANS_UPDATE:
		str = "成交明细更新";
		break;
	case AS_CONNIMTS_UPDATE:
		str = "委托报单更新";
		break;
	default:
		str = "未知状态";
		break;
	}

	return str;
}



string CLxGlobalFun::GetOrderStatus(const int &status)
{
	string  strTmp = "";
	switch (status)
	{
	case tos_default:
		strTmp = "未知";
		break;
	case tos_ordering:
		strTmp = "委托中";
		break;
	case tos_ordered:
		strTmp = "委托成功";
		break;
	case tos_orderfailed:
		strTmp = "委托失败";
		break;
	case tos_tradeing:
		strTmp = "部分成交";
		break;
	case tos_traded:
		strTmp = "成交完成";
		break;
	case tos_cancling:
		strTmp = "撤单中";
		break;
	case tos_cancled:
		strTmp = "撤单成功";
		break;
	case tos_canclfailed:
		strTmp = "撤单失败";
		break;

	default:
		break;
	}

	return  strTmp;
}

// 获取报单提交状态
string CLxGlobalFun::GetSubmitOrderStatus(const int &status)
{
	string  strTmp = "";
	switch (status)
	{
	case tos_submit_default:
		strTmp = "提交中";
		break;
	case tos_Inserted:
		strTmp = "订单已经提交";
		break;
	case tos_Cancel:
		strTmp = "撤单已经提交";
		break;
	case tos_Modify:
		strTmp = "修改已经提交";
		break;
	case tos_Accepted:
		strTmp = "已经接受";
		break;
	case tos_InsertRejected:
		strTmp = "报单已经被拒绝";
		break;
	case tos_CancelRejected:
		strTmp = "撤单已经被拒绝";
		break;
	case tos_ModifyRejected:
		strTmp = "改单已经被拒绝";
		break;

	default:
		break;
	}

	return  strTmp;
}

// 根据交易类型ID获取交易类型名称
std::wstring CLxGlobalFun::GetPageBussType(BussType  bussType)
{
	wstring m_strType = L"";
	switch (bussType)
	{
	case EProgramRuntime:
		m_strType = L"程序化实盘";
		break;
	case EManualRuntime:
		m_strType = L"手动实盘";
		break;
	
	case EManualSimu:
		m_strType = L"手动实盘";
		break;
	case EManualRuntimeSimu:
		m_strType = L"手动实时模拟";
		break;
	case EProgramRuntimeSimu:
		m_strType = L"程序化实时模拟";
		break;
	case EManulaHis:
		m_strType = L"手动历史模拟";
		break;
	case EStaticTest:
		m_strType = L"静态评测";
	case EDYNTest:
		m_strType = L"动态评测";
		break;
	default:
		m_strType = L"未知";
		break;
	}

	return m_strType;
}

// 根据交易类型ID获取交易类型名称
std::string CLxGlobalFun::GetTradeTypeName(TRADE_TYPE type)
{
	string strTmp = "";
	switch (type)
	{
	case FTDC_TRDT_deault:
		strTmp = "未知";
		break;
	case RDT_SplitCombination:
		strTmp = "组合持仓拆分为单一持仓,初始化不应包含该类型的持仓";
		break;
	case FTDC_TRDT_Common:
		strTmp = "普通成交";
		break;
	case FTDC_TRDT_OptionsExecution:
		strTmp = "期权执行";
		break;
	case FTDC_TRDT_OTC:
		strTmp = "OTC成交";
		break;
	case FTDC_TRDT_EFPDerived:
		strTmp = "期转现衍生成交";
		break;
	case FTDC_TRDT_CombinationDerived:
		strTmp = "组合衍生成交";
		break;
	default:
		strTmp = "未知";
		break;
	}

	return strTmp;
}
// 根据开平标志ID获取开平标志名称
std::string CLxGlobalFun::GetOffset_flagName(const string &strID)
{
	return strID;
}

// 根据买卖方向ID获取买卖方向名称
//快期是买，卖，我们是多，空
std::string CLxGlobalFun::GetBuySellName(TRADE_DIRECTION dirct)
{
	if (dirct == TD_BUY)
	{
		return "多";//改为多，和其它保持一致
	}
	else if (dirct == TD_SELL)
	{
		return "空";//
	}
	return "";
}
// 根据委托状态ID获取委托状态名称
std::string CLxGlobalFun::GetEntr_statName(const string &strID)
{
	return strID;
}
// 根据撤单标志ID获取撤单标志名称
std::string CLxGlobalFun::GetCancel_flagName(const string &strID)
{
	
	return strID;
}
// 根据操作方向ID获取操作方向名称
std::string CLxGlobalFun::GetAccessWayName(const string &strID)
{
	
	return strID;
}

// 根据转账是否成功ID获取转账是否成功名称
std::string CLxGlobalFun::GetIn_account_flagName(const string &strID)
{
	if(strID == "1")
	{
		return "是";
	}
	else
	{
		return strID;
	}
}
// 根据持仓方向ID获取持仓方向名称
std::string CLxGlobalFun::GetLongShortName(const string &strID)
{

	return strID;
}
// 根据渠道ID获取渠道名称（包括委托渠道,撤销渠道,操作来源）
std::string CLxGlobalFun::GetTerm_typeName(const string &strID)
{

	return strID;
}
// 刷新信息时根据账户状态id获取账户状态名称
std::string CLxGlobalFun::GetAcct_stat(const string &strID)
{

	return strID;
}
// 根据品种ID获取品种名称
std::string CLxGlobalFun::GetGetVarietyName(const string &strID)
{
	return strID;
}




// 判断一个字段ID是否应该格式化为浮点数
bool CLxGlobalFun::IsFloatTitle(string strID)
{
	return false;
}
bool CLxGlobalFun::CheckFloat(string &sFloat)
{
	int iLen = sFloat.length();
	if(iLen != 0)
	{
		char ch;
		bool bHasPoint = false;
		for(int i = 0; i < iLen; i++)
		{
			ch = sFloat[i];
			if(ch < '0' || ch > '9') // 如果不是数字
			{
				if(ch == '-' && i == 0 && iLen > 1) // 如果是负号，而且是第一个，并且字符串长度大于1，则通过
				{
					continue;
				}
				else if(ch == '.' && !bHasPoint) // 如果是小数点，而且还没出现过
				{
					bHasPoint = true;
					if(i == 0 || i == iLen -1)
					{
						return false;
					}
					else
					{
						continue;
					}
				}
				else
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}


/*
将小数部分加1，如果有进位到整数的话，则返回1，否则返回0
比如：strFloat = "4445"，执行后strFloat为"4446"
*/
int CLxGlobalFun::StringXiaoAdd1(string &strFloat)
{
	int  iLen    = strFloat.length();
	int  iCount  = iLen -1;
	int  iReturn = 0;

	//找到从右边开始第一个不是9的数，并且将这之前的全部置为0，比如"8999"，则变为"9000"
	char cTemp  = strFloat.at(iCount);
	while(cTemp == '9' && iCount > 0)
	{
		// 将为9的数字置0，因为加1了
		strFloat.at(iCount) = '0'; //将该位置0
		iCount--;
		cTemp = strFloat.at(iCount);
	}

	//如果是第0个，且cTemp是9，则返回值为1，否则为0
	if(iCount == 0 && cTemp == '9')
	{
		strFloat.at(iCount) = '0';
		iReturn++;	
	}
	else//字符串加1
	{
		cTemp++;
		strFloat.at(iCount) = cTemp;
	}

	return iReturn;
}


// 根据提货状态ID获取提货状态名称
std::string CLxGlobalFun::GetTake_statName(const string &strID)
{
	
	return strID;
}

// 根据证件类型ID获取证件类型名称
std::string CLxGlobalFun::GetCert_typeName(const string &strID)
{
	
	return strID;
}


string CLxGlobalFun::StrFromInt64(__int64 ntime)
{
	string ret = "";
	char sz[128] = { 0 };
	if (ntime != 0)
	{
		tm t;
		localtime_s(&t, &ntime);
		//gmtime_s(&t, &ntime);
		strftime(sz, 128, "%Y-%m-%d %H:%M:%S", &t);
		ret = sz;
	}
	return ret;
}

std::string CLxGlobalFun::Get16HexRandNumber(const int &iCount)
{
	srand((unsigned)time(NULL)); // 用时间做种子

	const char *pHexUnit = "0123456789abcdef"; // 定义数组，记录16进制的所有字符
	const int iUnitNum = 16; // 单位的个数，这里是16进制，故有16个单位

	string sNum;
	for (int i = 0; i < iCount; i++)
	{
		if (i == 0)
		{
			// 第一位只能最大只能是e，避免序列号增加后溢出
			sNum.append(1, *(pHexUnit + rand() % (iUnitNum - 1))); // 在sNum后面添加一个字符
		}
		else
		{
			sNum.append(1, *(pHexUnit + rand() % iUnitNum)); // 在sNum后面添加一个字符
		}
	}

	return sNum;

}

// 思路，比如"A1A2A3A4"，则表示为(((A1)*16+A2)*16+A3)*16+A4 相当于 A1*16*6*16+A2*16*16+A3*16+A4
UINT64 CLxGlobalFun::HexStrToUINT64(const string &sHexStr)
{
	int iLen = sHexStr.length();
	UINT64 u64 = 0;

	char ch;
	for (int i = 0; i < iLen; i++)
	{
		ch = sHexStr[i];
		if ((ch <= 'F'&& ch >= 'A') || (ch >= '0' && ch <= '9') || (ch <= 'f' && ch >= 'a'))
		{
			if (ch <= '9' && ch >= '0') // 如果是0-9的数字
				u64 = u64 * 16 + (ch - 0x30);
			else if (ch <= 'F' && ch >= 'A') // 如果是大写的两位数
				u64 = u64 * 16 + (10 + ch - 'A');//(10 + ch - 'A') 获得该字符实际表示的整形格式，比如 ch = 'A'，结果则为10
			else // 如果是小写的两位数
				u64 = u64 * 16 + (10 + ch - 'a');
		}
		else
		{
			// 如果不符号要求，则返回-1
			return -1;
		}
	}

	return u64;

}



//滑点计算
void CLxGlobalFun::CalcSlipPointPrice(double & dPrice, double dSlipPoint, DIRECTION_TRADETYPE direction)
{
	switch (direction)//做空
	{
	case DTT_SHORT_BUY://空(开空\平多)
	case DTT_LONG_SELL:
	{
		dPrice -= dSlipPoint;
	}
	break;
	case DTT_LONG_BUY://多(开多\平空)
	case DTT_SHORT_SELL:
	{
		dPrice += dSlipPoint;
	}
	break;
	default://默认为多
	{
		//no op
	}
	break;
	}
}


//止损止盈滑点计算(bStopLoss:TRUE为止损,FALSE为止盈)
void CLxGlobalFun::CalcSlipPointPriceForStopLossandProfit(double & dPrice, double dSlipPoint, DIRECTION_TRADETYPE direction, BOOL bStopLoss/*=FALSE*/)
{
	switch (direction)//做空
	{
	case DTT_SHORT_BUY://空(开空\平多)
	case DTT_LONG_SELL:
	{
		if (!bStopLoss)
		{
			//止损
			dPrice += dSlipPoint;
		}
		else
		{
			//止盈
			dPrice -= dSlipPoint;
		}
	}
	break;
	case DTT_LONG_BUY://多(开多\平空)
	case DTT_SHORT_SELL:
	{
		if (bStopLoss)
		{
			//止损
			dPrice += dSlipPoint;
		}
		else
		{
			//止盈
			dPrice -= dSlipPoint;
		}
	}
	break;
	default://默认为多
	{
		//no op
	}
	break;
	}
}
//行情最新价是否优于下单价(做多,行情最新价>=下单价,成交;做空,行情价<=下单价,成交;否则不成交)
//成交则返回最新价为成交价
//返回值:true 符合条件,返回成交价
//		 false 不符合条件,不返回成交价
bool CLxGlobalFun::IsSuperiorPrice(double dOrderPrice, double dQuotePrice, DIRECTION_TRADETYPE direction)
{
	bool result = false;
	switch (direction)//做空
	{
	case DTT_SHORT_BUY://空(开空\平多)
	case DTT_LONG_SELL:
	{
		if (dOrderPrice <= dQuotePrice)
		{
			result = true;
		}
	}
	break;
	case DTT_LONG_BUY://多(开多\平空)
	case DTT_SHORT_SELL:
	{
		if (dOrderPrice >= dQuotePrice)
		{
			result = true;
		}
	}
	break;
	default://默认为多
	{
		//no op
	}
	break;
	}

	return result;
}

//获取对手价
int CLxGlobalFun::GetOpponentPrice(DIRECTION_TRADETYPE direction, double & dPrice, double dSellPrice, double dBuyPrice)
{
	switch (direction)//做空
	{
	case DTT_SHORT_BUY://空(开空\平多)
	case DTT_LONG_SELL:
	{
		//对手价为买一价
		dPrice = dBuyPrice;
	}
	break;
	case DTT_LONG_BUY://多(开多\平空)
	case DTT_SHORT_SELL:
	{
		//对手价为买一价
		dPrice = dSellPrice;
	}
	break;
	default://默认为多
	{
		//对手价为买一价
		dPrice = dBuyPrice;
	}
	break;
	}
	return 0;
}

//优化调整报单价
void CLxGlobalFun::CalcMarketPrice(double & dPrice, TRADEPARSEUNIT &tpu)
{
	if (tpu.bInternal)
	{
		if (tpu.dLowLimitPrice && dPrice < tpu.dLowLimitPrice)
		{
			//如果价格低于跌停价
			dPrice = tpu.dLowLimitPrice;
		}
		if (tpu.dTopLimitPrice && dPrice > tpu.dTopLimitPrice)
		{
			//如果价格高于涨停价
			dPrice = tpu.dTopLimitPrice;
		}
	}
}




//止损止盈逻辑判断
//参数:tpu 下单条件
//	   tp 传出报单价和成交价(成交价模拟时需要使用)
//	   bSimulate 是否是模拟交易(TRUE-是, FALSE-否)
//返回值:
//		0	成功,仅设置止盈止损传出报单价或成交价
//		1	成功,设置止盈止损且本地预埋价,判定通过,传出报单价
//		2	失败,指定价条件未达到
int CLxGlobalFun::JudgeStopLossandProfit(TRADEPARSEUNIT &tpu, TRADEPRICE &tp, BOOL bSimulate)
{
	int result = 0;
	int nStopType = 0;//0--空, 1--达到止损, 2--达到止盈
	double dStopLoss = 0.0f;//止损点
	double dStopProfit = 0.0f;//止盈点
	double l_dReportPrice = 0.0f;//报单价
	double l_dBidPrice = 0.0f;//成交价
	if (tpu.lc.b_stop_loss_flg || tpu.lc.b_stop_profit_flg)
	{
		//如果设置止损
		if (tpu.lc.b_stop_loss_flg)
		{
			dStopLoss = tpu.dStopBasePrice;
			CalcSlipPointPriceForStopLossandProfit(dStopLoss, tpu.lc.f_stop_loss_leap, tpu.nDirection, TRUE);
		}
		//如果设置止盈
		if (tpu.lc.b_stop_profit_flg)
		{
			dStopProfit = tpu.dStopBasePrice;
			CalcSlipPointPriceForStopLossandProfit(dStopProfit, tpu.lc.f_stop_profit_leap, tpu.nDirection, FALSE);
		}

		switch (tpu.nDirection)
		{
		case DTT_SHORT_BUY://空(开空\平多)
		case DTT_LONG_SELL:
		{
			if (tpu.lc.b_stop_loss_flg && dStopLoss >= tpu.dLatestPrice[0])
			{
				//达到止损点
				nStopType = 1;
				result = 0;
			}
			else if (tpu.lc.b_stop_profit_flg && dStopProfit <= tpu.dLatestPrice[0])
			{
				//达到止盈点
				nStopType = 2;
				result = 0;
			}
			else
			{
				result = 2;//未触发条件
			}
		}
		break;
		case DTT_LONG_BUY://多(开多\平空)
		case DTT_SHORT_SELL:
		{
			if (tpu.lc.b_stop_loss_flg && dStopLoss <= tpu.dLatestPrice[0])
			{
				//达到止损点
				nStopType = 1;
				result = 0;
			}
			else if (tpu.lc.b_stop_profit_flg && dStopProfit >= tpu.dLatestPrice[0])
			{
				//达到止盈点
				nStopType = 2;
				result = 0;
			}
			else
			{
				result = 2;//未触发条件
			}
		}
		break;
		default:
		{
			result = 2;//未触发条件
		}
		break;
		}

		if (result == 0)
		{
			//如果是本地预埋价, 则达到止损止盈以市价为报单价发出
			//Modify by ppshuai 2015-12-10，以止损止盈价发出
			if (tpu.lc.e_nt == NT_TRIGGER_PRICE)
			{
				//达到止盈点
				if (nStopType == 2)
				{
					l_dReportPrice = dStopProfit;//tpu.dLatestPrice[0];
				}
				//达到止损点
				else if (nStopType == 1)
				{
					l_dReportPrice = dStopLoss;//tpu.dLatestPrice[0];
				}
				else
				{
					//no op
				}
				//计算得出50跳滑点的市价
				//CalcSlipPointPrice(l_dReportPrice, tpu.dLimitDots, tpu.nDirection);
				CalcMarketPrice(l_dReportPrice, tpu);
				tp.dReportPrice = l_dReportPrice;
				result = 1;//触发条件,直接报单
			}
			else
			{
				result = 0;//触发条件,进入下个接口根据实际的价格性质报单(市价,对手价,最新价,排队价)
			}

			//如果是模拟交易
			if (bSimulate)
			{
				//////////////////////////////////////////////////////////////////////////
				//市价报单
				//报单价赋值为最新价
				//Modify by ppshuai 2015-12-10，以止损止盈价发出

				//达到止盈点
				if (nStopType == 2)
				{
					l_dReportPrice = dStopProfit;//tpu.dLatestPrice[0];
				}
				//达到止损点
				else if (nStopType == 1)
				{
					l_dReportPrice = dStopLoss;//tpu.dLatestPrice[0];
				}
				else
				{
					//no op
				}
				//计算得出50跳滑点的市价
				//CalcSlipPointPrice(l_dReportPrice, tpu.dLimitDots, tpu.nDirection);
				CalcMarketPrice(l_dReportPrice, tpu);
				tp.dReportPrice = l_dReportPrice;

				//以最新价为成交价
				l_dBidPrice = tpu.dLatestPrice[0];
				tp.dBidPrice = l_dBidPrice;
			}
		}
	}
	else
	{
		//未设置止损和止盈,返回由引擎处理
		result = 0;
	}

	return result;
}

//移动止损止盈逻辑判断
//参数:tpu 下单条件
//	   tp 传出报单价和成交价(成交价模拟时需要使用)
//	   bSimulate 是否是模拟交易(TRUE-是, FALSE-否)
//返回值:
//		0	成功,仅设置止盈止损传出报单价或成交价
//		1	成功,设置止盈止损且本地预埋价,判定通过,传出报单价
//		2	失败,指定价条件未达到
int CLxGlobalFun::JudgeMoveStopLossandProfit(TRADEPARSEUNIT &tpu, TRADEPRICE &tp, BOOL bSimulate)
{
	int result = 0;
	int nStopType = 0;//0--空, 1--达到止损, 2--达到止盈
	double dStopLoss = 0.0f;//止损点
	double dStopProfit = 0.0f;//止盈点
	double l_dReportPrice = 0.0f;//报单价
	double l_dBidPrice = 0.0f;//成交价
	if (tpu.lc.b_stop_loss_flg || tpu.lc.b_stop_profit_flg)
	{
		//如果设置止损
		if (tpu.lc.b_stop_loss_flg)
		{
			dStopLoss = tpu.dStopBasePrice;
			CalcSlipPointPriceForStopLossandProfit(dStopLoss, tpu.lc.f_stop_loss_leap, tpu.nDirection, TRUE);
		}
		//如果设置止盈
		if (tpu.lc.b_stop_profit_flg)
		{
			dStopProfit = tpu.dStopBasePrice;
			CalcSlipPointPriceForStopLossandProfit(dStopProfit, tpu.lc.f_stop_profit_leap, tpu.nDirection, FALSE);
		}

		switch (tpu.nDirection)
		{
		case DTT_SHORT_BUY://空(开空\平多)
		case DTT_LONG_SELL:
		{
			if (tpu.lc.b_stop_loss_flg && dStopLoss >= tpu.dLatestPrice[0])
			{
				//达到止损点
				nStopType = 1;
				result = 0;
			}
			else if (tpu.lc.b_stop_profit_flg && dStopProfit >= tpu.dLatestPrice[0])
			{
				//////////////////////////////////////////////////////////////////////////
				//最新价回撤到止盈价时触发，与需求讨论约定
				//达到止盈点
				nStopType = 2;
				result = 0;
			}
			else
			{
				result = 2;//未触发条件
			}
		}
		break;
		case DTT_LONG_BUY://多(开多\平空)
		case DTT_SHORT_SELL:
		{
			if (tpu.lc.b_stop_loss_flg && dStopLoss <= tpu.dLatestPrice[0])
			{
				//达到止损点
				nStopType = 1;
				result = 0;
			}
			else if (tpu.lc.b_stop_profit_flg && dStopProfit <= tpu.dLatestPrice[0])
			{
				//////////////////////////////////////////////////////////////////////////
				//最新价回撤到止盈价时触发，与需求讨论约定
				//达到止盈点
				nStopType = 2;
				result = 0;
			}
			else
			{
				result = 2;//未触发条件
			}
		}
		break;
		default:
		{
			result = 2;//未触发条件
		}
		break;
		}

		if (result == 0)
		{
			//如果是本地预埋价, 则达到止损止盈以市价为报单价发出
			//Modify by ppshuai 2015-12-10，以止损止盈价发出
			if (tpu.lc.e_nt == NT_TRIGGER_PRICE)
			{
				//达到止盈点
				if (nStopType == 2)
				{
					l_dReportPrice = dStopProfit;//tpu.dLatestPrice[0];
				}
				//达到止损点
				else if (nStopType == 1)
				{
					l_dReportPrice = dStopLoss;//tpu.dLatestPrice[0];
				}
				else
				{
					//no op
				}
				//计算得出50跳滑点的市价
				//CalcSlipPointPrice(l_dReportPrice, tpu.dLimitDots, tpu.nDirection);
				CalcMarketPrice(l_dReportPrice, tpu);
				tp.dReportPrice = l_dReportPrice;
				result = 1;//触发条件,直接报单
			}
			else
			{
				result = 0;//触发条件,进入下个接口根据实际的价格性质报单(市价,对手价,最新价,排队价)
			}

			//如果是模拟交易
			if (bSimulate)
			{
				//////////////////////////////////////////////////////////////////////////
				//市价报单
				//报单价赋值为最新价
				//Modify by ppshuai 2015-12-10，以止损止盈价发出

				//达到止盈点
				if (nStopType == 2)
				{
					l_dReportPrice = dStopProfit;//tpu.dLatestPrice[0];
				}
				//达到止损点
				else if (nStopType == 1)
				{
					l_dReportPrice = dStopLoss;//tpu.dLatestPrice[0];
				}
				else
				{
					//no op
				}
				//计算得出50跳滑点的市价
				//CalcSlipPointPrice(l_dReportPrice, tpu.dLimitDots, tpu.nDirection);
				CalcMarketPrice(l_dReportPrice, tpu);
				tp.dReportPrice = l_dReportPrice;

				//以最新价为成交价
				l_dBidPrice = tpu.dLatestPrice[0];
				tp.dBidPrice = l_dBidPrice;
			}
		}
	}
	else
	{
		//未设置止损和止盈,返回由引擎处理
		result = 0;
	}

	return result;
}

//kenny  20170609
int CLxGlobalFun::NTCheckPrice(TRADEPARSEUNIT &tpu, double *l_dReportPrice, double *l_dBidPrice)
{
	int result = 0;
	double dSpecifiedPrice = tpu.lc.f_check_price;

	//如果启用滑点
	if (tpu.lc.b_slip_price_flg)
	{
		CalcSlipPointPrice(dSpecifiedPrice, tpu.lc.f_slip_price_leap, tpu.nDirection);
	}

	//模拟交易
	switch (tpu.nDirection)
	{
	case DTT_SHORT_BUY:	//开空
	case DTT_LONG_SELL:	//平多
	{
		//指定价小于等于当前TICK最新价
		if ((dSpecifiedPrice <= tpu.dLatestPrice[0]))
		{
			*l_dReportPrice = dSpecifiedPrice;
			*l_dBidPrice = tpu.dLatestPrice[0];//最新价成交
			result = 0;
		}
		else
		{
			result = 1;//指定价未达条件
		}
	}
	break;
	case DTT_SHORT_SELL://平空
	case DTT_LONG_BUY:	//开多
	{
		//指定价大于等于当前TICK最新价
		if ((dSpecifiedPrice >= tpu.dLatestPrice[0]))
		{
			*l_dReportPrice = dSpecifiedPrice;
			*l_dBidPrice = tpu.dLatestPrice[0];//最新价成交
			result = 0;
		}
		else
		{
			result = 1;//指定价未达条件
		}
	}
	break;
	default:
	{
		result = 1;//指定价未达条件
	}
	break;
	}

	return result;
}


//下单逻辑及撮合机制
//参数:tpu 下单条件
//	   tp 传出报单价和成交价(成交价模拟时需要使用)
//	   bSimulate 是否是模拟交易(TRUE-是, FALSE-否)
//返回值:0 成功,传出报单价或成交价
//		 1 失败,指定价条件未达到
int CLxGlobalFun::logic_MatchEngine(TRADEPARSEUNIT &tpu, TRADEPRICE &tp)
{
	int result = 1;//默认不能成交
	double l_dBidPrice = 0.0f;//成交价
	double l_dReportPrice = 0.0f;//报单价

	//根据报单性质撮合成交
	switch (tpu.lc.e_nt)
	{
	case NT_MARKET_PRICE://市价
	{
		//市价下单
		l_dReportPrice = tpu.dReportPrice;//tpu.dLatestPrice[0];
		//最新价+/-50跳作为市价
		CalcSlipPointPrice(l_dReportPrice, tpu.dLimitDots, tpu.nDirection);

		//模拟交易
		if (IsSuperiorPrice(l_dReportPrice, tpu.dLatestPrice[0], tpu.nDirection))
		{
			l_dBidPrice = tpu.dLatestPrice[0];//最新价成交
			result = 0;
		}
	}
	break;
	case NT_CHECK_PRICE://指定价
	{

		result = NTCheckPrice(tpu, &l_dReportPrice,&l_dBidPrice);
		
	}
	break;
	case NT_BIDDING_PRICE://对手价
	{
		double dOpponentPrice = 0.0f;
		//获取对手价
		GetOpponentPrice(tpu.nDirection, dOpponentPrice, tpu.dSellPrice[0], tpu.dBuyPrice[0]);

		l_dReportPrice = tpu.dReportPrice;//dOpponentPrice;(已经加了滑点的价格)
		//如果启用滑点
		if (tpu.lc.b_slip_price_flg)
		{
			//CalcSlipPointPrice(l_dReportPrice, tpu.lc.f_slip_price_leap, tpu.nDirection);
		}

		//模拟交易
		if (IsSuperiorPrice(l_dReportPrice, tpu.dLatestPrice[0], tpu.nDirection))
		{
			l_dBidPrice = tpu.dLatestPrice[0];//最新价成交
			result = 0;
		}
	}
	break;
	case NT_TRIGGER_PRICE://本地预埋价
	{
		//本地预埋价价位于两笔Tick最新价之间
		double dEmbeddedPrice = tpu.lc.f_trigger_price;
		//如果启用滑点
		if (tpu.lc.b_slip_price_flg)
		{
			CalcSlipPointPrice(dEmbeddedPrice, tpu.lc.f_slip_price_leap, tpu.nDirection);
		}
		//本地预埋价位于两笔Tick最新价之间
		if (((dEmbeddedPrice <= tpu.dLatestPrice[0]) && (dEmbeddedPrice >= tpu.dLatestPrice[1])) //本次最新价 >= 本地预埋价 >= 上次最新价
			|| ((dEmbeddedPrice >= tpu.dLatestPrice[0]) && (dEmbeddedPrice <= tpu.dLatestPrice[1])))//本次最新价 <= 本地预埋价 <= 上次最新价
		{
			l_dReportPrice = dEmbeddedPrice;
			//模拟交易
			l_dBidPrice = tpu.dLatestPrice[0];//最新价成交
			result = 0;
		}
		else
		{
			result = 1;//本地预埋价未达条件
		}
	}
	break;
	case NT_LATEST://最新价
	{
		l_dReportPrice = tpu.dReportPrice;//tpu.dLatestPrice[0];(已经加了滑点的价格)
		//如果启用滑点
		if (tpu.lc.b_slip_price_flg)
		{
			//CalcSlipPointPrice(l_dReportPrice, tpu.lc.f_slip_price_leap, tpu.nDirection);
		}

		//模拟交易
		if (IsSuperiorPrice(l_dReportPrice, tpu.dLatestPrice[0], tpu.nDirection))
		{
			l_dBidPrice = tpu.dLatestPrice[0];//最新价成交
			result = 0;
		}
	}
	break;
	case NT_QUEUE://排队价
	{
		double dQueuePrice = 0.0f;
		//获取排队价
		GetOpponentPrice(tpu.nDirection, dQueuePrice, tpu.dBuyPrice[0], tpu.dSellPrice[0]);
		l_dReportPrice = tpu.dReportPrice;//dQueuePrice;(已经加了滑点的价格)
		//如果启用滑点
		if (tpu.lc.b_slip_price_flg)
		{
			//CalcSlipPointPrice(l_dReportPrice, tpu.lc.f_slip_price_leap, tpu.nDirection);
		}
		//模拟交易
		if (IsSuperiorPrice(l_dReportPrice, tpu.dLatestPrice[0], tpu.nDirection))
		{
			l_dBidPrice = tpu.dLatestPrice[0];//最新价成交
			result = 0;
		}
	}
	break;
	default://其它价
	{
		result = 1;//未触发条件
	}
	break;
	}

	if (result == 0)
	{
		//判断是否超过涨停或跌停板,如果超过则以涨停或跌停价计算
		CalcMarketPrice(l_dReportPrice, tpu);
		tp.dReportPrice = l_dReportPrice;

		//模拟交易
		tp.dBidPrice = l_dBidPrice;
	}

	return result;
}
//模拟开仓判定接口
//参数:tpu 下单条件
//	   tp	传出报单价和成交价(模拟时需要使用)
//返回值:0 成功
//		 1 失败,指定价条件未达到
//		 2 失败,无效的交易信号
int CLxGlobalFun::simu_OpenParse(TRADEPARSEUNIT &tpu, TRADEPRICE & tp)
{
	int result = 0;//默认成功
	//TRADEPRICE tp = {0.0f, 0.0f};

	if (tpu.nDirection <= DTT_NULL || tpu.nDirection >= DTT_MAX)
	{
		//PrintfOutputWindow(("==>模拟开仓-无效的方向值:%d(有效值为:[1, 4])\r\n"), tpu.nDirection);
		return 2;
	}

	//市价默认使用最新价成交
	result = logic_MatchEngine(tpu, tp);
	if (result == 0)
	{
		//dBidPrice = tp.dBidPrice;
		//////////////////////////////////////////////////////////////////////////
		//以下为测试打印
		if (tpu.nDirection > DTT_NULL && tpu.nDirection < DTT_MAX)
		{
			//PrintfOutputWindow(TEXT("==>模拟开仓-%s 买一价:%lf 卖一价:%lf 最新价:%lf 报单价:%lf 成交价:%lf\r\n"), 
			//	g_ptOperatorList[tpu.nDirection], tpu.dBuyPrice[0], tpu.dSellPrice[0], tpu.dLatestPrice[0], tp.dReportPrice, tp.dBidPrice);
		}
		//////////////////////////////////////////////////////////////////////////
	}

	return result;
}

//模拟平仓判定接口
//参数:tpu 下单条件
//		tp	传出报单价和成交价(模拟时需要使用)
//返回值:0 成功
//		 1 失败,指定价条件未达到
//		 2 失败,无效的交易信号
int CLxGlobalFun::simu_CloseParse(TRADEPARSEUNIT &tpu, TRADEPRICE &tp)
{
	int result = 0;//默认成功
	//TRADEPRICE tp = {0.0f, 0.0f};

	if (tpu.nDirection <= DTT_NULL || tpu.nDirection >= DTT_MAX)
	{
		//PrintfOutputWindow("<==模拟平仓-无效的方向值:%d(有效值为:[1, 4])\r\n"), tpu.nDirection);
		return 2;
	}

	switch (tpu.lc.e_nt)
	{
	case NT_CHECK_PRICE:
	{
		//默认使用最新价成交
		result = logic_MatchEngine(tpu, tp);
	}
	break;
	case NT_TRIGGER_PRICE:
	{
		//止损止盈点判断
		result = JudgeStopLossandProfit(tpu, tp, TRUE);
		if (result == 0)
		{
			//本地预埋价,根据实际的价格性质进行计算成交价
			result = logic_MatchEngine(tpu, tp);
		}
		else if (result == 1)
		{
			result = 0;//达到止损止盈点,且是本地预埋价
		}
		else
		{
			// no op
		}
	}
	break;
	case NT_MARKET_PRICE:	//市价
	case NT_BIDDING_PRICE:	//对手价
	case NT_LATEST:			//最新价
	case NT_QUEUE:			//排队价
	{
		//止损止盈点判断
		result = JudgeStopLossandProfit(tpu, tp, TRUE);
		if (result == 0)
		{
			//市价默认使用最新价成交
			result = logic_MatchEngine(tpu, tp);
		}
	}
	break;
	default:
	{
		result = 1;//无效价格性质类型
	}
	break;
	}

	if (result == 0)
	{
		//dBidPrice = tp.dBidPrice;
		//////////////////////////////////////////////////////////////////////////
		//以下为测试打印
		if (tpu.nDirection > DTT_NULL && tpu.nDirection < DTT_MAX)
		{
			//PrintfOutputWindow(TEXT("<==模拟平仓-%s 买一价:%lf 卖一价:%lf 最新价:%lf 报单价:%lf 成交价:%lf\r\n"), 
			//	g_ptOperatorList[tpu.nDirection], tpu.dBuyPrice[0], tpu.dSellPrice[0], tpu.dLatestPrice[0], tp.dReportPrice, tp.dBidPrice);
		}
		//////////////////////////////////////////////////////////////////////////
	}

	return result;
}


wstring CLxGlobalFun::Str2Wstr(const char * szData)
{
	string str(szData);
	if (str.length() == 0)
		return L"";
	std::wstring wstr;
	wstr.assign(str.begin(), str.end());
	return wstr;
}

string CLxGlobalFun::t2sA(__int64 ntime)
{
	string ret = "";
	char sz[128] = { 0 };
	if (ntime != 0)
	{
		tm t;
		localtime_s(&t, &ntime);
		//gmtime_s(&t, &ntime);
		strftime(sz, 128, "%Y-%m-%d %H:%M:%S", &t);
		ret = sz;
	}
	return ret;
}