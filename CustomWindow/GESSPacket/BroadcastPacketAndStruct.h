#pragma once
#include <string>
#include "BroadcastPacket.h"
#include "ArrayListMsg.h"
#include "HashtableMsg.h"

using namespace std;

//----------------------------------------------报单基类Ex 定义  --未用，待定
typedef struct tagOrderBaseEx
{
	bool bProcessed;                // 标志是否被处理
	string orderNo;					// 报单号 
	string localOrderNo;			// 本地报单号
	string instID;				    // 合约代码       
}OrderBaseEx,*POrderBaseEx;

//----------------------------------------------报单基类 定义
typedef struct tagOrderBase
{
	bool bProcessed;                // 标志是否被处理
	string orderNo;					// 报单号 
	string localOrderNo;			// 本地报单号
	string instID;				    // 合约代码       
	int amount;						// 数量        
	int remainAmount;				// 剩余数量  
	string status;					// 状态
	string RspMsg;					// 错误描述
	string  applyTime;				// 申请时间
}OrderBase,*POrderBase;

//----------------------------------------------报单定义
// 现货报单信息  
typedef struct tagSpotOrder : public OrderBase
{     
	string buyOrSell;				// 交易方向
	double price;					// 价格

	string clientID;			    // 客户代码        
	string memberID;				// 会员代码        
	string applyDate;				// 申报日期            
	string traderID;				// 交易员代码        
	string cancelTime;				// 撤销时间        
	string cancelID;				// 撤销员代码        
	string matchType;				// 类型        
	string endorseInstID;		    // 转让合约代码        

	//设置  TSpotOrder:现货报单信息 结构的值
	int Packet2Struct(CBroadcastPacket & pkt) 
	{
		pkt.GetParameterVal("orderNo",orderNo);              // 报单号
		pkt.GetParameterVal("instID",instID);                // 合约代码 (not null)
		pkt.GetParameterVal("clientID",clientID);            // 客户代码 (not null)
		pkt.GetParameterVal("memberID",memberID);            // 会员代码 (not null)
		pkt.GetParameterVal("buyOrSell",buyOrSell);          // 买卖方向 (not null)
		pkt.GetParameterVal("applyDate",applyDate);          // 申报日期
		pkt.GetParameterVal("applyTime",applyTime);          // 申报时间
		pkt.GetParameterVal("traderID",traderID);            // 交易员代码 (not null)
		pkt.GetParameterVal("price",price);                  // 价格  (not null)
		pkt.GetParameterVal("amount",amount);                // 数量  (not null)
		pkt.GetParameterVal("remainAmount",remainAmount);    // 剩余数量
		pkt.GetParameterVal("cancelTime",cancelTime);        // 撤消时间
		pkt.GetParameterVal("cancelID",cancelID);            // 撤消员代码
		pkt.GetParameterVal("status",status);                // 状态
		pkt.GetParameterVal("matchType",matchType);          // 类型  (not null)
		pkt.GetParameterVal("endorseInstID",endorseInstID);  // 转让合约代码 (not null)
		pkt.GetParameterVal("localOrderNo",localOrderNo);    // 本地报单号 (not null)
		pkt.GetParameterVal("RspMsg",RspMsg);                // 错误描述
		return 0;
	}
}SpotOrder,*PSpotOrder;

// 现货T+N报单（远期报单）  
typedef struct tagForwardOrder : public OrderBase
{
	double  price;				    // 价格 
	string  buyOrSell;				// 买卖方向

	string  clientID;				// 客户代码     
	string  memberID;				// 会员代码     
	string  applyDate;				// 申报日期
	string  traderID;				// 交易员代码   
	string  cancelTime;				// 撤消时间
	string  cancelID;				// 撤消员代码
	string  matchType;				// 类型		

	int Packet2Struct(CBroadcastPacket & pkt) 
	{
		pkt.GetParameterVal("orderNo",     orderNo);       // 报单号
		pkt.GetParameterVal("instID",      instID);        // 合约代码  (not null)
		pkt.GetParameterVal("clientID",    clientID);      // 客户代码  (not null)
		pkt.GetParameterVal("memberID",    memberID);      // 会员代码  (not null)
		pkt.GetParameterVal("buyOrSell",   buyOrSell);     // 买卖方向  (not null)
		pkt.GetParameterVal("applyDate",   applyDate);     // 申报日期
		pkt.GetParameterVal("applyTime",   applyTime);     // 申报时间
		pkt.GetParameterVal("traderID",    traderID);      // 交易员代码 (not null)
		pkt.GetParameterVal("price",       price);         // 价格   (not null)
		pkt.GetParameterVal("amount",      amount);        // 数量  (not null)
		pkt.GetParameterVal("remainAmount",remainAmount);  // 剩余数量
		pkt.GetParameterVal("cancelTime",  cancelTime);    // 撤消时间
		pkt.GetParameterVal("cancelID",    cancelID);      // 撤消员代码
		pkt.GetParameterVal("status",      status);        // 状态
		pkt.GetParameterVal("localOrderNo",localOrderNo);  // 本地报单号  (not null)
		pkt.GetParameterVal("matchType",   matchType);     // 类型  (not null)
		pkt.GetParameterVal("RspMsg",      RspMsg);        // 错误描述

		return 0;
	}
}ForwardOrder,*PForwardOrder;

// DeferOrder 现货延期交收报单  
typedef struct tagDeferOrder : public OrderBase
{        
	string buyOrSell; 				// 交易方向
	string offSetFlag;    		    // 开平仓标志
	double price;        	        // 价格

	string clientID;  				// 客户代码         
	string memberID;  				// 会员代码 
	string applyDate; 				// 申报日期 
	string cancelTime;    		    // 撤销时间
	double margin;      		    // 保证金率
	string marginType;    		    // 保证金类型
	string traderID;      		    // 交易员代码
	string cancelID;     		    // 撤销员代码
	string matchType;				// 类型

	int Packet2Struct(CBroadcastPacket & pkt) 
	{
		pkt.GetParameterVal("orderNo",     orderNo);      // 报单号 (not null)
		pkt.GetParameterVal("clientID",    clientID);     // 客户代码 (not null)
		pkt.GetParameterVal("instID",      instID);       // 合约代码 (not null)
		pkt.GetParameterVal("memberID",    memberID);     // 会员代码 (not null)
		pkt.GetParameterVal("buyOrSell",   buyOrSell);    // 买卖方向 (not null)
		pkt.GetParameterVal("applyDate",   applyDate);    // 申报日期
		pkt.GetParameterVal("applyTime",   applyTime);    // 申报时间
		pkt.GetParameterVal("offSetFlag",  offSetFlag);   // 开平仓标志 (not null)
		pkt.GetParameterVal("price",       price);        // 价格   (not null)
		pkt.GetParameterVal("amount",      amount);       // 数量  (not null)
		pkt.GetParameterVal("remainAmount",remainAmount); // 剩余数量
		pkt.GetParameterVal("cancelTime",  cancelTime);   // 撤消时间
		pkt.GetParameterVal("status",      status);       // 状态
		pkt.GetParameterVal("localOrderNo",localOrderNo); // 本地报单号  (not null)
		pkt.GetParameterVal("margin",      margin);       // 保证金率
		pkt.GetParameterVal("marginType"  ,marginType);   // 保证金类型  (not null)
		pkt.GetParameterVal("traderID",    traderID);     // 交易员代码  (not null)
		pkt.GetParameterVal("cancelID",    cancelID);     // 撤消员代码
		pkt.GetParameterVal("matchType",   matchType);    // 类型  (not null)
		pkt.GetParameterVal("RspMsg",      RspMsg);       // 错误描述

		return 0;
	}
}DeferOrder,*PDeferOrder;

//现货延期交收交割申报报单  
typedef struct tagDeferDeliveryAppOrder : public OrderBase
{
	string  buyOrSell;				// 买卖方向   

	string  applyDate;				// 申请日期
	string  cancelTime;				// 撤消时间
	string  memberID;				// 会员代码     
	string  traderID;				// 交易员代码   
	string  cancelID;				// 撤消员代码
	string  clientID;				// 客户代码

	int Packet2Struct(CBroadcastPacket & pkt) 
	{
		pkt.GetParameterVal("orderNo",orderNo);           // 报单编号
		pkt.GetParameterVal("clientID",clientID);         // 客户代码  (not null)
		pkt.GetParameterVal("instID",instID);             // 合约代码  (not null)
		pkt.GetParameterVal("buyOrSell",buyOrSell);       // 买卖方向  (not null)
		pkt.GetParameterVal("applyDate",applyDate);       // 申请日期
		pkt.GetParameterVal("applyTime",applyTime);       // 申请时间
		pkt.GetParameterVal("amount",amount);             // 数量  (not null)
		pkt.GetParameterVal("cancelTime",cancelTime);     // 撤消时间
		pkt.GetParameterVal("status",status);             // 报单状态
		pkt.GetParameterVal("localOrderNo",localOrderNo); // 本地报单编号 (not null)
		pkt.GetParameterVal("memberID",memberID);         // 会员代码  (not null)
		pkt.GetParameterVal("traderID",traderID);         // 交易员代码  (not null)
		pkt.GetParameterVal("cancelID",cancelID);         // 撤消员代码
		pkt.GetParameterVal("remainAmount",remainAmount); // 剩余数量
		pkt.GetParameterVal("RspMsg",      RspMsg);       // 错误描述

		return 0;
	}
}DeferDeliveryAppOrder,*PDeferDeliveryAppOrder;

//现货延期交收中立仓申报报单  
typedef struct tagMiddleAppOrder : public OrderBase
{
	string  buyOrSell;				// 买卖方向

	string  applyDate;				// 申请日期
	string  cancelTime;				// 撤消时间
	string  cancelID;				// 撤消员代码
	string  traderID;				// 交易员代码
	string  clientID;				// 客户代码     
	string  memberID;				// 会员代码

	int Packet2Struct(CBroadcastPacket & pkt) 
	{
		pkt.GetParameterVal("orderNo", orderNo);          // 报单编号
		pkt.GetParameterVal("clientID",clientID);         // 客户代码 (not null)
		pkt.GetParameterVal("instID",  instID);           // 合约代码 (not null)
		pkt.GetParameterVal("memberID",memberID);         // 会员代码  (not null)
		pkt.GetParameterVal("buyOrSell",buyOrSell);       // 买卖方向  (not null)
		pkt.GetParameterVal("applyDate",applyDate);       // 申请日期
		pkt.GetParameterVal("applyTime",applyTime);       // 申请时间
		pkt.GetParameterVal("amount",   amount);          // 数量  (not null)
		pkt.GetParameterVal("cancelTime",cancelTime);     // 撤消时间
		pkt.GetParameterVal("status",    status);         // 报单状态
		pkt.GetParameterVal("localOrderNo",localOrderNo); // 本地报单编号 (not null)
		pkt.GetParameterVal("traderID",    traderID);     // 交易员代码  (not null)
		pkt.GetParameterVal("cancelID",    cancelID);     // 撤消员代码
		pkt.GetParameterVal("remainAmount",remainAmount); // 剩余数量
		pkt.GetParameterVal("RspMsg",      RspMsg);       // 错误描述
		return 0;
	}
}MiddleAppOrder,*PMiddleAppOrder;



// 结构定义:onRecvRtnSpotInstStateUpdate
typedef struct tagInstState
{
	string       instID               ; //合约代码    
	string       tradeState           ; //合约交易状态  
	// 报文解析：报文 -> 结构    onRecvDeferDeliveryQuotation
	int Packet2Struct(CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"         , instID);
		pkt.GetParameterVal("tradeState"     , tradeState);	
		return 0;
	};
}InstState,*PInstState;


//----------------------------------------------报文接口 [onSysInit] 定义
// 结构定义:onSysInit
typedef struct tagSysInit
{
	string       old_exch_date        ; //原交易日期    
	string       new_exch_date        ; //新交易日期    
}SysInit,*PSysInit;


//----------------------------------------------报文接口 [onSysStatChange] 定义
// 结构定义:onSysStatChange
typedef struct tagSysStat
{
	string       sys_date             ; //系统日期    
	string       exch_date            ; //交易日期    
	string       m_sys_stat           ; //二级系统状态    m_sys_stat
	string       b_sys_stat           ; //交易所系统状态    b_sys_stat
}SysStat,*PSysStat;


//----------------------------------------------报文接口 [onBaseTableUpdate] 定义
// 结构定义:onBaseTableUpdate
typedef struct tagBaseTableUpdate
{
	string       table_name           ; //表名    已支持的表名：bank_infobank_mer_code_infobranch_infobroker_infocode_tablefare_model_defacct_fare_detailfare_model_detailinstID_defsystem_parateller_infovariety
}BaseTableUpdate,*PBaseTableUpdate;


//----------------------------------------------报文接口 [onForceLogout] 定义
// 结构定义:onForceLogout
typedef struct tagTForceLogout
{
	string       hint_msg             ; //提示信息    
	string		 session_id			  ;
}TForceLogout,*PTForceLogout;


//----------------------------------------------报文接口 [onNewBourseBulletin] 定义
// 结构定义:onNewBourseBulletin
typedef struct tagTNewBourseBulletin
{
	string       seq_no               ; //公告序号    
	string       title                ; //公告标题    
	string       content              ; //公告内容    
}TNewBourseBulletin,*PTNewBourseBulletin;


//----------------------------------------------报文接口 [onNewMemberBulletin] 定义
// 结构定义:onNewMemberBulletin
typedef struct tagTNewMemberBulletin
{
	string       bulletin_id          ; //公告序号    
	string       title                ; //公告标题    
}TNewMemberBulletin,*PTNewMemberBulletin;


//----------------------------------------------报文接口 [onSessionKey] 定义
// 结构定义:onSessionKey
typedef struct tagTSessionKey
{
	int          oper_flag            ; //操作标志    1：生效2：注销
	string       session_id           ; //会话ID    
	string       session_key          ; //会话密钥    
}TSessionKey,*PTSessionKey;


//----------------------------------------------报文接口 [onRiskNotify] 定义
// 结构定义:onRiskNotify
typedef struct tagTRiskNotity
{
	string       acct_no              ; //客户号    
	string       risk_idx1            ; //风险度1    
	string       risk_idx2            ; //风险度2    
	string       call_margin          ; //追保金额    
	string       exch_date            ; //交易日期    
	string       notify_title         ; //通知标题    
	string       notify_content       ; //风险通知内容    

	tagTRiskNotity& operator = (const tagTRiskNotity& Other)
	{
		this->acct_no        = Other.acct_no;
		this->risk_idx1      = Other.risk_idx1;
		this->risk_idx2      = Other.risk_idx2;
		this->call_margin    = Other.call_margin;
		this->exch_date      = Other.exch_date;
		this->notify_title   = Other.notify_title;
		this->notify_content = Other.notify_content;
		return *this;
	}
}TRiskNotity,*PTRiskNotity;


//----------------------------------------------报文接口 [onRecvRtnSpotInstStateUpdate] 定义
// 结构定义:onRecvRtnSpotInstStateUpdate
typedef struct tagSpotInstState
{
	string       instID               ; //合约代码    
	char         tradeState           ; //合约交易状态    
}SpotInstState,*PSpotInstState;


//----------------------------------------------报文接口 [onRecvRtnForwardInstStateUpdate] 定义
// 结构定义:onRecvRtnForwardInstStateUpdate
typedef struct tagForwardInstState
{
	string       instID               ; //合约代码    
	char         tradeState           ; //合约交易状态    
}ForwardInstState,*PForwardInstState;


//----------------------------------------------报文接口 [onRecvRtnDeferInstStateUpdate] 定义
// 结构定义:onRecvRtnDeferInstStateUpdate
typedef struct tagDeferInstState
{
	string       instID               ; //合约代码    
	char         tradeState           ; //合约交易状态    
}DeferInstState,*PDeferInstState;


//----------------------------------------------报文接口 [onRecvRtnSpotMarketStateUpdate] 定义
// 结构定义:onRecvRtnSpotMarketStateUpdate
typedef struct tagSpotMarketState
{
	string       marketID             ; //市场代码    
	char         marketState          ; //市场交易状态    
}SpotMarketState,*PSpotMarketState;


//----------------------------------------------报文接口 [onRecvRtnForwardMarketStateUpdate] 定义
// 结构定义:onRecvRtnForwardMarketStateUpdate
typedef struct tagForwardMarketState
{
	string       marketID             ; //市场代码    
	char         marketState          ; //市场交易状态    
}ForwardMarketState,*PForwardMarketState;


//----------------------------------------------报文接口 [onRecvRtnDeferMarketStateUpdate] 定义
// 结构定义:onRecvRtnDeferMarketStateUpdate
typedef struct tagDeferMarketState
{
	string       marketID             ; //市场代码    
	char         marketState          ; //市场交易状态    
}DeferMarketState,*PDeferMarketState;


//----------------------------------------------报文接口 [onAcctCapitalAccess] 定义
// 结构定义:onAcctCapitalAccess
typedef struct tagAcctCapitalAccess
{
	string       fund_src_type        ; //资金来源类型    fund_src_type
	string       serial_no            ; //操作流水号    
	string       acct_no              ; //客户号    
	int          access_way           ; //存取方向    出入方向access_way
	double       amount               ; //转账金额    
}AcctCapitalAccess,*PAcctCapitalAccess;


//----------------------------------------------报文接口 [onCustInfoChange] 定义
// 结构定义:onCustInfoChange
typedef struct tagCustInfoChange
{
	int          update_flag          ; //更改标志    1：增加2：修改3：删除
	string       acct_no              ; //客户号    
	string       member_id            ; //会员编号    
	string       cust_abbr            ; //客户简称    
	string       m_Fare_model_id      ; //会员费用模版ID    
	string       b_Fare_model_id      ; //交易所费用模版ID    
	string       acct_type            ; //账户类型    acct_type
	string       acct_stat            ; //账户状态    
	string       branch_ID            ; //所属代理机构    
}CustInfoChange,*PCustInfoChange;


//----------------------------------------------报文接口 [onCustRiskDegreeChange] 定义
// 结构定义:onCustRiskDegreeChange
typedef struct tagCustRiskDegreeChange
{
	int          oper_flag            ; //操作标志    1：广播
	string       risk_degree1         ; //风险度1    
	string       risk_degree2         ; //风险度2    
	string       risk_grade           ; //风险等级    
}CustRiskDegreeChange,*PCustRiskDegreeChange;


typedef struct tagRspMsg
{        
	string RspCode;					///错误码        
	string RspMsg;					///错误描述
}RspMsg,*PRspMsg;
		


//现货撤单请求  
typedef struct tagSpotOrderCancel
{
	string  orderNo;				//报单号         (not null)
	string  instID;					//合约代码        (not null)
	string  clientID;				//客户代码       (not null)
	string  memberID;				//会员代码         (not null)
	string  traderID;				//交易员代码      (not null)
}SpotOrderCancel,*PSpotOrderCancel;

// 撤单统一结构 add by xiaorensong
typedef struct tagOrderCancel
{
	string  orderNo;				// 报单号
	string  localOrderNo;
	string  instID;				    // 合约代码
	int     amount;                 // 总报入量
	int     remainAmount;           // 未成交量
	double  price;					// 价格
	string  buyOrSell;
	string  offSetFlag;

	int Packet2Struct(CBroadcastPacket & pkt) 
	{
		pkt.GetParameterVal("orderNo",orderNo);
		pkt.GetParameterVal("localOrderNo",localOrderNo);
		if(localOrderNo.empty()) // 用于解决交收撤单的时候发送过来的key为localOrderID的问题
		{
			pkt.GetParameterVal("localOrderID",localOrderNo);
		}
		pkt.GetParameterVal("instID",instID);
		pkt.GetParameterVal("amount",amount);
		pkt.GetParameterVal("remainAmount",remainAmount);
		pkt.GetParameterVal("price",price);
		pkt.GetParameterVal("buyOrSell",buyOrSell);
		pkt.GetParameterVal("offSetFlag",offSetFlag);

		return 0;
	}
}OrderCancel,*POrderCancel;

//现货成交单信息  
typedef struct tagSpotMatch
{
	string  matchNo;				// 成交编号
	string  buyOrSell;				// 买卖方向
	string  clientID;				// 客户代码
	string  clientType;				// 客户类型
	string  memberID;				// 会员代码
	string  instID;				    // 合约代码
	string  matchDate;				// 成交日期
	string  matchTime;				// 成交时间
	double  price;				    // 价格
	int     volume;				    // 数量
	string  orderNo;				// 系统报单号
	string  localOrderNo;		    // 报单本地编号

	string  matchType;				// 类型

	static int Packet2Struct(tagSpotMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.matchNo );
		pkt.GetParameterVal("buyOrSell"         , stBody.buyOrSell);
		pkt.GetParameterVal("clientID"          , stBody.clientID);
		pkt.GetParameterVal("clientType"        , stBody.clientType);
		pkt.GetParameterVal("memberID"          , stBody.memberID);
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("matchDate"         , stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.matchTime);
		pkt.GetParameterVal("price"             , stBody.price);
		pkt.GetParameterVal("volume"            , stBody.volume);
		pkt.GetParameterVal("orderNo"           , stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.localOrderNo);
		pkt.GetParameterVal("matchType"         , stBody.matchType);

		return 0;
	};
}SpotMatch,*PSpotMatch;




//现货延期交收撤单请求  
typedef struct tagDeferOrderCancel
{
	string  orderNo;				//报单号             (not null)
	string  instID;					//合约代码             (not null)
	string  clientID;				//客户代码           (not null)
	string  memberID;				//会员代码            (not null)
	string  traderID;				//交易员代码           (not null)
}DeferOrderCancel,*PDeferOrderCancel;

//现货延期交收成交单  
typedef struct tagDeferMatch
{
	string  localOrderNo;		    // 报单本地编号
	string  matchNo;				// 成交编号
	string  orderNo;				// 系统报单号
	string  instID;				    // 合约代码
	double  price;				    // 价格
	int     volume;				    // 数量
	string  buyOrSell;				// 买卖方向
	string  offsetFlag;				// 开平标志
	
	string  matchDate;				// 成交日期
	string  matchTime;				// 成交时间
	string  clientID;				// 客户代码
	string  clientType;				// 客户类型
	string  memberID;				// 会员代码

	string sExchTypeCode;

	static int Packet2Struct(tagDeferMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.matchNo);
		pkt.GetParameterVal("buyOrSell"         , stBody.buyOrSell);
		pkt.GetParameterVal("clientID"          , stBody.clientID);
		pkt.GetParameterVal("clientType"        , stBody.clientType);
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("memberID"          , stBody.memberID);
		pkt.GetParameterVal("matchDate"         , stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.matchTime);
		pkt.GetParameterVal("price"             , stBody.price);
		pkt.GetParameterVal("volume"            , stBody.volume);
		pkt.GetParameterVal("offSetFlag"        , stBody.offsetFlag);
		pkt.GetParameterVal("orderNo"           , stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.localOrderNo);

		return 0;
	};
}DeferMatch,*PDeferMatch;



//现货延期交收交割申报撤单请求  
typedef struct tagDeferDeliveryAppOrderCancel
{
	string  orderNo;				//报单号               (not null)
	string  instID;				//合约代码              (not null)
	string  clientID;				//客户代码             (not null)
	string  memberID;				//会员代码		(not null)
	string  traderID;				//交易员代码       (not null)
}DeferDeliveryAppOrderCancel,*PDeferDeliveryAppOrderCancel;

//递延交割申报成交单  
typedef struct tagDeferDeliveryAppMatch
{
	string  orderNo;				// 报单号
	string  LocalOrderNo;			// 报单本地编号
	string  matchNo;				// 成交序号
	string  instID;				    // 合约代码
	int     volume;				    // 数量
	string  buyOrSell;				// 买卖方向
	string  middleFlag;				// 是否为中立仓

	string  matchDate;				// 成交日期
	string  matchTime;				// 成交时间
	string  clientID;				// 客户代码
	string  memberID;				// 会员代码

	static int Packet2Struct(tagDeferDeliveryAppMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.matchNo);
		pkt.GetParameterVal("clientID"          , stBody.clientID);
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("memberID"          , stBody.memberID);
		pkt.GetParameterVal("buyOrSell"         , stBody.buyOrSell);
		pkt.GetParameterVal("volume"            , stBody.volume);
		pkt.GetParameterVal("matchDate"         , stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.matchTime);
		pkt.GetParameterVal("orderNo"           , stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.LocalOrderNo);
		pkt.GetParameterVal("middleFlag"        , stBody.middleFlag);

		return 0;
	};
}DeferDeliveryAppMatch,*PDeferDeliveryAppMatch;



//现货延期交收中立仓申报撤单请求  
typedef struct tagMiddleAppOrderCancel
{
	string  orderNo;				//报单号          (not null)
	string  instID;				//合约代码         (not null)
	string  clientID;				//客户代码        (not null)
	string  memberID;				//会员代码        (not null)
	string  traderID;				//交易员代码      (not null)
}MiddleAppOrderCancel,*PMiddleAppOrderCancel;



//现货T+N撤单请求  
typedef struct tagForwardOrderCancel
{                                                                           
	string  orderNo;				//报单号            (not null)
	string  instID;				//合约代码           (not null)
	string  clientID;				//客户代码          (not null)
	string  memberID;				//会员代码          (not null)
	string  traderID;				//交易员代码        (not null)
}ForwardOrderCancel,*PForwardOrderCancel;

//现货T+N成交单  
typedef struct tagForwardMatch
{
	string  matchNo;				// 成交编号
	string  buyOrSell;				// 买卖方向
	string  clientID;				// 客户代码
	string  clientType;				// 客户类型
	string  memberID;				// 会员代码
	string  instID;				    // 合约代码
	string  matchDate;				// 成交日期
	string  matchTime;				// 成交时间
	double  price;				    // 价格
	int     volume;				    // 数量
	string  orderNo;				// 系统报单号
	string  localOrderNo;			// 报单本地编号
	string  marginRate;				// 保证金率
	string  rateType;				// 保证金类型

	static int Packet2Struct(tagForwardMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.matchNo);
		pkt.GetParameterVal("buyOrSell"         , stBody.buyOrSell);
		pkt.GetParameterVal("clientID"          , stBody.clientID);
		pkt.GetParameterVal("clientType"        , stBody.clientType);
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("memberID"          , stBody.memberID);
		pkt.GetParameterVal("matchDate"         , stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.matchTime);
		pkt.GetParameterVal("price"             , stBody.price);
		pkt.GetParameterVal("volume"            , stBody.volume);
		pkt.GetParameterVal("marginRate"        , stBody.marginRate);
		pkt.GetParameterVal("rateType"          , stBody.rateType);
		pkt.GetParameterVal("orderNo"           , stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.localOrderNo);

		return 0;
	};
}ForwardMatch,*PForwardMatch;

//现货延期交收补偿费率信息  
typedef struct tagDeferFeeRate
{
	string  effectDate;				//日期
	string  instID;				//合约代码
	string  payDirection;				//支付方向
	double  feeRate;				//递延费率
}DeferFeeRate,*PDeferFeeRate;

//现货行情  
typedef struct tagSpotQuotation
{
	string  instID;				//合约代码
	string  name;				//合约名称
	double  lastClose;		    //昨收盘
	double  open;				//开盘价
	double  high;				//最高价
	double  low;				//最低价
	double  last;				//最新价
	double  close;				//收盘价
	double  bid1;				//买价1
	int  bidLot1;				//买量1
	double  bid2;				//买价2
	int  bidLot2;				//买量2
	double  bid3;				//买价3
	int  bidLot3;				//买量3
	double  bid4;				//买价4
	int  bidLot4;				//买量4
	double  bid5;				//买价5
	int  bidLot5;				//买量5
	double  ask1;				//卖价1
	int  askLot1;				//卖量1
	double  ask2;				//卖价2
	int  askLot2;				//卖量2
	double  ask3;				//卖价3
	int  askLot3;				//卖量3
	double  ask4;				//卖价4
	int  askLot4;				//卖量4
	double  ask5;				//卖价5
	int  askLot5;				//卖量5
	int  volume;				//成交量（双边）
	double  weight;				//成交（双边）重量
	double  highLimit;				//涨停板
	double  lowLimit;				//跌停板
	double  upDown;				//涨跌
	double  upDownRate;				//涨跌幅度
	double  turnOver;				//成交额
	double  average;				//均价
	string  quoteDate;				//行情日期
	string  quoteTime;				//行情时间
	int  sequenceNo;				//行情序号,每天从1开始递增
	string sZipBuff;				//压缩
    
	tagSpotQuotation& operator = (const tagSpotQuotation& Other)
	{
		this->instID    = Other.instID;
		this->name      = Other.name;
		this->lastClose = Other.lastClose;
		this->open = Other.open;
		this->high = Other.high;
		this->low = Other.low;
		this->last = Other.last;
		this->close = Other.close;
		this->bid1 = Other.bid1;
		this->bidLot1 = Other.bidLot1;
		this->bid2 = Other.bid2;
		this->bidLot2 = Other.bidLot2;
		this->bid3 = Other.bid3;
		this->bidLot3 = Other.bidLot3;
		this->bid4 = Other.bid4;
		this->bidLot4 = Other.bidLot4;
		this->bid5 = Other.bid5;
		this->bidLot5 = Other.bidLot5;
		this->ask1 = Other.ask1;
		this->askLot1 = Other.askLot1;
		this->ask2 = Other.ask2;
		this->askLot2 = Other.askLot2;
		this->ask3 = Other.ask3;
		this->askLot3 = Other.askLot3;
		this->ask4 = Other.ask4;
		this->askLot4 = Other.askLot4;
		this->ask5 = Other.ask5;
		this->askLot5 = Other.askLot5;
		this->volume = Other.volume;
		this->weight = Other.weight;
		this->highLimit = Other.highLimit;
		this->lowLimit = Other.lowLimit;
		this->upDown = Other.upDown;
		this->upDownRate = Other.upDownRate;
		this->turnOver = Other.turnOver;
		this->average = Other.average;
		this->quoteDate = Other.quoteDate;
		this->quoteTime = Other.quoteTime;
		this->sequenceNo = Other.sequenceNo;
		this->sZipBuff = Other.sZipBuff;

		return *this;
	}
}SpotQuotation,*PSpotQuotation;
    
//现货延期交收行情  
typedef struct tagDeferQuotation
{
	string  instID;				//合约代码
	string  name;				//合约名称
	double  lastSettle;				//昨结算
	double  lastClose;				//昨收盘
	double  open;				//开盘价
	double  high;				//最高价
	double  low;				//最低价
	double  last;				//最新价
	double  close;				//收盘价
	double  settle;				//结算价
	double  bid1;				//买价1
	int  bidLot1;				//买量1
	double  bid2;				//买价2
	int  bidLot2;				//买量2
	double  bid3;				//买价3
	int  bidLot3;				//买量3
	double  bid4;				//买价4
	int  bidLot4;				//买量4
	double  bid5;				//买价5
	int  bidLot5;				//买量5
	double  ask1;				//卖价1
	int  askLot1;				//卖量1
	double  ask2;				//卖价2
	int  askLot2;				//卖量2
	double  ask3;				//卖价>3
	int  askLot3;				//卖量3
	double  ask4;				//卖价4
	int  askLot4;				//卖量4
	double  ask5;				//卖价5
	int  askLot5;				//卖量5
	int  volume;				//成交量
	double  weight;				//成交（双边）重量
	double  highLimit;				//涨停板
	double  lowLimit;				//跌停板
	int  Posi;				//持仓量
	double  upDown;				//涨跌
	double  upDownRate;				//涨跌幅度
	double  turnOver;				//成交额
	double  average;				//均价
	string  quoteDate;				//行情日期
	string  quoteTime;				//行情时间
	int  sequenceNo;				//行情序号
	string sZipBuff;				//压缩

	tagDeferQuotation& operator = (const tagDeferQuotation& Other)
	{
		this->instID    = Other.instID;
		this->name      = Other.name;
		this->lastClose = Other.lastClose;
		this->open = Other.open;
		this->high = Other.high;
		this->low = Other.low;
		this->last = Other.last;
		this->close = Other.close;
		this->bid1 = Other.bid1;
		this->bidLot1 = Other.bidLot1;
		this->bid2 = Other.bid2;
		this->bidLot2 = Other.bidLot2;
		this->bid3 = Other.bid3;
		this->bidLot3 = Other.bidLot3;
		this->bid4 = Other.bid4;
		this->bidLot4 = Other.bidLot4;
		this->bid5 = Other.bid5;
		this->bidLot5 = Other.bidLot5;
		this->ask1 = Other.ask1;
		this->askLot1 = Other.askLot1;
		this->ask2 = Other.ask2;
		this->askLot2 = Other.askLot2;
		this->ask3 = Other.ask3;
		this->askLot3 = Other.askLot3;
		this->ask4 = Other.ask4;
		this->askLot4 = Other.askLot4;
		this->ask5 = Other.ask5;
		this->askLot5 = Other.askLot5;
		this->volume = Other.volume;
		this->weight = Other.weight;
		this->highLimit = Other.highLimit;
		this->lowLimit = Other.lowLimit;
		this->upDown = Other.upDown;
		this->upDownRate = Other.upDownRate;
		this->turnOver = Other.turnOver;
		this->average = Other.average;
		this->quoteDate = Other.quoteDate;
		this->quoteTime = Other.quoteTime;
		this->sequenceNo = Other.sequenceNo;
		this->sZipBuff = Other.sZipBuff;

		return *this;
	}
}DeferQuotation,*PDeferQuotation;


// 行情统一结构
typedef struct	tagBidAskUnit
{ 
	tagBidAskUnit()
	{
		m_uiVol = 0;
		m_uiPrice = 0;
	}
	unsigned int m_uiVol;		//委量
	unsigned int m_uiPrice;	    //委价
} BID_ASK_UNIT,*PBID_ASK_UNIT;

struct QUOTATION
{
	string  instID;				//合约代码
	string  name;				//合约名称

	QUOTATION()
	{
		m_uilastClose = 0;
		m_uiLastSettle = 0;
		m_uiSettle = 0;
		m_uiOpenPrice = 0;
		m_uiHigh = 0;
		m_uiLow = 0;
		m_uiClose = 0;
		m_uiHighLimit = 0;
		m_uiLowLimit = 0;
		m_uiLast = 0;
		m_uiAverage = 0;
		m_uiVolume = 0;
		m_uiWeight = 0;
		m_uiTurnOver = 0;
		m_uiChiCangLiang = 0;
		m_uiLastChiCangLiang = 0;
        m_uiUpDown = 0;
        m_dUpDownRate = 0;

		m_Bid[0].m_uiPrice = 0;

		memset(m_Bid, 0, sizeof(BID_ASK_UNIT));
		memset(m_Ask, 0, sizeof(BID_ASK_UNIT));
	}

	unsigned int m_uiSeqNo;     //包号
	unsigned int m_uiDate;
	unsigned int m_uiTime; 
	unsigned int m_uilastClose; //昨收
	unsigned int m_uiLastSettle;//昨结
	unsigned int m_uiSettle;    //结算
	unsigned int m_uiOpenPrice; //今开盘
	unsigned int m_uiHigh;      //最高价
	unsigned int m_uiLow;       //最低价
	unsigned int m_uiClose;     //收盘价
	unsigned int m_uiHighLimit;
	unsigned int m_uiLowLimit;	
	unsigned int m_uiLast;      //最新价
	unsigned int m_uiAverage;   //均价
	unsigned int m_uiVolume;    //成交量
	unsigned int m_uiWeight;    //成交重量
	//unsigned int m_uiTurnOver;  //成交金额
	UINT64 m_uiTurnOver;  //成交金额
	unsigned int m_uiChiCangLiang; //持仓量
	unsigned int m_uiLastChiCangLiang;//昨日持仓量
	int m_uiUpDown;       // 涨跌
	double m_dUpDownRate;   // 涨跌幅度
	BID_ASK_UNIT     m_Bid[10];//买入价格
	BID_ASK_UNIT     m_Ask[10];//卖出价格

	string Key() const
	{
		return instID;
	}
	void Decode(const char* pBuf, unsigned int uiSize)
	{
		*this = *(QUOTATION*)(pBuf);
	}
	const char* Encode(unsigned int & uiLen)
	{
		uiLen = sizeof(QUOTATION);
		return (const char*)(this);
	}

	bool IsValidate( unsigned int uiPrice ) const
	{
		if( m_uiLowLimit > 0 && uiPrice >= m_uiLowLimit && m_uiHighLimit >= uiPrice )
			return true;
		else
			return false;
	}

	// 获取计算浮动盈亏的价格
	UINT GetCalSurplusPrice() const
	{
		// 如果最新价没有则采用昨结
		return m_uiLast > 0 ? m_uiLast : m_uiLastSettle;
	}

} /*QUOTATION,*PQUOTATION*/;

    
//递延交收行情  
typedef struct tagDeferDeliveryQuotation
{
	string  instID;				//合约代码
	int  bidLot;				//买量
	int  askLot;				//卖量
	int  midBidLot;				//中立仓买量
	int  midAskLot;				//中立仓卖量

	int Packet2Struct(CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"         , instID);
		pkt.GetParameterVal("bidLot"         , bidLot);
		pkt.GetParameterVal("askLot"         , askLot);
		pkt.GetParameterVal("midBidLot"      , midBidLot);
		pkt.GetParameterVal("midAskLot"      , midAskLot);		
		return 0;
	};
}DeferDeliveryQuotation,*PDeferDeliveryQuotation;


//现货T+N行情  
typedef struct tagForwardQuotation
{
	string  instID;				//合约代码
	string  name;				//合约名称
	double  lastSettle;				//昨结算
	double  lastClose;				//昨收盘
	double  open;				//开盘价
	double  high;				//最高价
	double  low;				//最低价
	double  last;				//最新价
	double  close;				//收盘价
	double  settle;				//结算价
	double  bid1;				//买价1
	int  bidLot1;				//买量1
	double  bid2;				//买价2
	int  bidLot2;				//买量2
	double  bid3;				//买价3
	int  bidLot3;				//买量3
	double  bid4;				//买价4
	int  bidLot4;				//买量4
	double  bid5;				//买价5
	int  bidLot5;				//买量5
	double  ask1;				//卖价1
	int  askLot1;				//卖量1
	double  ask2;				//卖价2
	int  askLot2;				//卖量2
	double  ask3;				//卖价>3
	int  askLot3;				//卖量3
	double  ask4;				//卖价4
	int  askLot4;				//卖量4
	double  ask5;				//卖价5
	int  askLot5;				//卖量5
	int  volume;				//成交量
	double  weight;				//成交（双边）重量
	double  highLimit;				//涨停板
	double  lowLimit;				//跌停板
	int  Posi;				//持仓量
	double  upDown;				//涨跌
	double  upDownRate;				//涨跌幅度
	double  turnOver;				//成交额
	double  average;				//均价
	string  quoteDate;				//行情日期
	string  quoteTime;				//行情时间
	int  sequenceNo;				//行情序号
	string sZipBuff;				//压缩

	tagForwardQuotation& operator = (const tagForwardQuotation& Other)
	{
		this->instID    = Other.instID;
		this->name      = Other.name;
		this->lastClose = Other.lastClose;
		this->open = Other.open;
		this->high = Other.high;
		this->low = Other.low;
		this->last = Other.last;
		this->close = Other.close;
		this->bid1 = Other.bid1;
		this->bidLot1 = Other.bidLot1;
		this->bid2 = Other.bid2;
		this->bidLot2 = Other.bidLot2;
		this->bid3 = Other.bid3;
		this->bidLot3 = Other.bidLot3;
		this->bid4 = Other.bid4;
		this->bidLot4 = Other.bidLot4;
		this->bid5 = Other.bid5;
		this->bidLot5 = Other.bidLot5;
		this->ask1 = Other.ask1;
		this->askLot1 = Other.askLot1;
		this->ask2 = Other.ask2;
		this->askLot2 = Other.askLot2;
		this->ask3 = Other.ask3;
		this->askLot3 = Other.askLot3;
		this->ask4 = Other.ask4;
		this->askLot4 = Other.askLot4;
		this->ask5 = Other.ask5;
		this->askLot5 = Other.askLot5;
		this->volume = Other.volume;
		this->weight = Other.weight;
		this->highLimit = Other.highLimit;
		this->lowLimit = Other.lowLimit;
		this->upDown = Other.upDown;
		this->upDownRate = Other.upDownRate;
		this->turnOver = Other.turnOver;
		this->average = Other.average;
		this->quoteDate = Other.quoteDate;
		this->quoteTime = Other.quoteTime;
		this->sequenceNo = Other.sequenceNo;
		this->sZipBuff = Other.sZipBuff;

		return *this;
	}
}ForwardQuotation,*PForwardQuotation;

//----------------------------------------------报文接口 [onRecvRtnSpotMatch] 定义
// 结构定义:onRecvRtnSpotMatch
typedef struct tagPktSpotMatch
{
	SpotMatch    stBody   						; //现货成交单信息
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}PktSpotMatch,*PPktSpotMatch;



//----------------------------------------------报文接口 [onRecvRtnForwardMatch] 定义
// 结构定义:onRecvRtnForwardMatch
typedef struct tagPktForwardMatch
{
	ForwardMatch	stBody							; //现货T+N成交单    
	string       acctNo               ; //客户号 
	RspMsg			 stRsp								; //错误信息   
}PktForwardMatch,*PPktForwardMatch;



//----------------------------------------------报文接口 [onRecvRtnDeferMatch] 定义
// 结构定义:onRecvRtnDeferMatch
typedef struct tagPktDeferMatch
{
	DeferMatch	 stBody 							; //现货延期交收成交单   
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息   
}PktDeferMatch,*PPktDeferMatch;



//----------------------------------------------报文接口 [onRecvRtnDeferDeliveryAppMatch] 定义
// 结构定义:onRecvRtnDeferDeliveryAppMatch
typedef struct tagPktDeferDeliveryAppMatch
{
	DeferDeliveryAppMatch	stBody			; //递延交割申报成交单   
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}PktDeferDeliveryAppMatch,*PPktDeferDeliveryAppMatch;

//----------------------------------------------报文接口 [onRecvRspSpotOrder] 定义
// 结构定义:onRecvRspSpotOrder
typedef struct tagRspSpotOrder
{
	SpotOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspSpotOrder,*PRspSpotOrder;

//----------------------------------------------报文接口 [onRecvRtnSpotOrder] 定义
// 结构定义:onRecvRtnSpotOrder
typedef struct tagRtnSpotOrder
{
	SpotOrder				stBody						; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnSpotOrder,*PRtnSpotOrder;

//----------------------------------------------报文接口 [onRecvRspSpotOrderCancel] 定义
// 结构定义:onRecvRspSpotOrderCancel
typedef struct tagRspSpotOrderCancel
{
	SpotOrderCancel				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspSpotOrderCancel,*PRspSpotOrderCancel; 

//----------------------------------------------报文接口 [onRecvRtnSpotOrderCancel] 定义
// 结构定义:onRecvRtnSpotOrderCancel
typedef struct tagRtnSpotOrderCancel
{
	SpotOrder				stBody						; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnSpotOrderCancel,*PRtnSpotOrderCancel;

//----------------------------------------------报文接口 [onRecvRspDeferOrder] 定义
// 结构定义:onRecvRspDeferOrder
typedef struct tagRspDeferOrder
{
	DeferOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspDeferOrder,*PRspDeferOrder;     
 
//----------------------------------------------报文接口 [onRecvRtnDeferOrder] 定义
// 结构定义:onRecvRtnDeferOrder
typedef struct tagRtnDeferOrder
{
	DeferOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnDeferOrder,*PRtnDeferOrder;

//----------------------------------------------报文接口 [onRecvRspDeferOrderCancel] 定义
// 结构定义:onRecvRspDeferOrderCancel
typedef struct tagRspDeferOrderCancel
{
	DeferOrderCancel				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspDeferOrderCancel,*PRspDeferOrderCancel; 

//----------------------------------------------报文接口 [onRecvRtnDeferOrderCancel] 定义
// 结构定义:onRecvRtnDeferOrderCancel
typedef struct tagRtnDeferOrderCancel
{
	DeferOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnDeferOrderCancel,*PRtnDeferOrderCancel;

//----------------------------------------------报文接口 [onRecvRspDeferDeliveryAppOrder] 定义
// 结构定义:onRecvRspDeferDeliveryAppOrder
typedef struct tagRspDeferDeliveryAppOrder
{
	DeferDeliveryAppOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspDeferDeliveryAppOrder,*PRspDeferDeliveryAppOrder;  


//----------------------------------------------报文接口 [onRecvRtnDeferDeliveryAppOrder] 定义
// 结构定义:onRecvRtnDeferDeliveryAppOrder
typedef struct tagRtnDeferDeliveryAppOrder
{
	DeferDeliveryAppOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnDeferDeliveryAppOrder,*PRtnDeferDeliveryAppOrder;

//----------------------------------------------报文接口 [onRecvRspDeferDeliveryAppOrderCancel] 定义
// 结构定义:onRecvRspDeferDeliveryAppOrderCancel
typedef struct tagRspDeferDeliveryAppOrderCancel
{
	DeferDeliveryAppOrderCancel				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspDeferDeliveryAppOrderCancel,*PRspDeferDeliveryAppOrderCancel;   

//----------------------------------------------报文接口 [onRecvRtnDeferDeliveryAppOrderCancel] 定义
// 结构定义:onRecvRtnDeferDeliveryAppOrderCancel
typedef struct tagRtnDeferDeliveryAppOrderCancel
{
	DeferDeliveryAppOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnDeferDeliveryAppOrderCancel,*PRtnDeferDeliveryAppOrderCancel;  
 
//----------------------------------------------报文接口 [onRecvRspMiddleAppOrder] 定义
// 结构定义:onRecvRspMiddleAppOrder
typedef struct tagRspMiddleAppOrder
{
	MiddleAppOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspMiddleAppOrder,*PRspMiddleAppOrder; 

//----------------------------------------------报文接口 [onRecvRtnMiddleAppOrder] 定义
// 结构定义:onRecvRtnMiddleAppOrder
typedef struct tagRtnMiddleAppOrder
{
	MiddleAppOrder				stBody			; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnMiddleAppOrder,*PRtnMiddleAppOrder;

//----------------------------------------------报文接口 [onRecvRspMiddleAppOrderCancel] 定义
// 结构定义:onRecvRspMiddleAppOrderCancel
typedef struct tagRspMiddleAppOrderCancel
{
	MiddleAppOrderCancel				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspMiddleAppOrderCancel,*PRspMiddleAppOrderCancel;  
   
//----------------------------------------------报文接口 [onRecvRtnMiddleAppOrderCancel] 定义
// 结构定义:onRecvRtnMiddleAppOrderCancel
typedef struct tagRtnMiddleAppOrderCancel
{
	MiddleAppOrder	stBody						; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnMiddleAppOrderCancel,*PRtnMiddleAppOrderCancel;      
    

//----------------------------------------------报文接口 [onRecvRtnForwardOrder] 定义
// 结构定义:onRecvRtnForwardOrder
typedef struct tagRtnForwardOrder
{
	ForwardOrder				stBody				; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnForwardOrder,*PRtnForwardOrder;
  
//----------------------------------------------报文接口 [onRecvRspForwardOrder] 定义
// 结构定义:onRecvRspForwardOrder
typedef struct tagRspForwardOrder
{
	ForwardOrder				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspForwardOrder,*PRspForwardOrder; 

//----------------------------------------------报文接口 [onRecvRspForwardOrderCancel] 定义
// 结构定义:onRecvRspForwardOrderCancel
typedef struct tagRspForwardOrderCancel
{
	ForwardOrderCancel				stBody					; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RspForwardOrderCancel,*PRspForwardOrderCancel; 
       
//----------------------------------------------报文接口 [onRecvRtnForwardOrderCancel] 定义
// 结构定义:onRecvRtnForwardOrderCancel
typedef struct tagRtnForwardOrderCancel
{
	ForwardOrder				stBody				; //    
	string       acctNo               ; //客户号    
	RspMsg			 stRsp								; //错误信息
}RtnForwardOrderCancel,*PRtnForwardOrderCancel;

//----------------------------------------------报文接口 [onRecvRtnForwardOrderCancel] 定义
// 结构定义:onRecvRtnForwardOrderCancel
typedef struct tagWindVane
{
	//string        serial_no		   ; // 样本流水号  样本设置中对应各个样本的流水号 
	string        prod_code            ; // 合约代码 目前只做Au(T+D)和Ag(T+D)    
	string        memo                 ; // 备注栏 详细内容中涉及备注，提示客户责任备注
	string        moreStorPer          ; // 多仓占用率
	string        openMoreStorAvg      ; // 开多仓均价
	string        haveMoreStorAvg      ; // 持多仓均价
	string        haveMoreStorPer      ; // 持多仓比例
	string        nullStorPer          ; // 空仓占用率
	string        openNullStorAvg      ; // 开空仓均价
	string        haveNullStorAvg      ; // 持空仓均价
	string        haveNullStorPer      ; // 持空仓比例
	string        lookPer              ; // 观望占用率 
	//RspMsg		  stRsp				   ; // 错误信息

	// 报文解析：报文 -> 结构    onRecvDeferDeliveryQuotation
	int Packet2Struct(CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("prod_code"         , prod_code);
		pkt.GetParameterVal("memo"         , memo);
		pkt.GetParameterVal("moreStorPer"         , moreStorPer);
		pkt.GetParameterVal("openMoreStorAvg"         , openMoreStorAvg);
		pkt.GetParameterVal("haveMoreSotrAvg"         , haveMoreStorAvg);
		pkt.GetParameterVal("haveMoreStorPer"         , haveMoreStorPer);
		pkt.GetParameterVal("nullStorPer"         , nullStorPer);
		pkt.GetParameterVal("openNullStorAvg"         , openNullStorAvg);
		pkt.GetParameterVal("haveNullStorAvg"         , haveNullStorAvg);
		pkt.GetParameterVal("haveNullStorPer"         , haveNullStorPer);
		pkt.GetParameterVal("lookPer"         , lookPer);
		return 0;
	};
}WindVane,*PWindVane;


class CBroadcastPacketAndStruct
{
public:
	//----------------------------------------------报文接口 [onSysInit] 报文结构相关解析
	// 报文解析：报文 -> 结构    onSysInit
	static int Packet2Struct(SysInit& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("old_exch_date"     , stBody.old_exch_date);
		pkt.GetParameterVal("new_exch_date"     , stBody.new_exch_date);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onSysInit
	static int Struct2Packet(SysInit& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("old_exch_date"     , stBody.old_exch_date);
		pkt.AddParameter("new_exch_date"     , stBody.new_exch_date);
		return 0;
	};



	//----------------------------------------------报文接口 [onSysStatChange] 报文结构相关解析
	// 报文解析：报文 -> 结构    onSysStatChange
	static int Packet2Struct(SysStat& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("sys_date"          , stBody.sys_date);
		pkt.GetParameterVal("exch_date"         , stBody.exch_date);
		pkt.GetParameterVal("m_sys_stat"        , stBody.m_sys_stat);
		pkt.GetParameterVal("b_sys_stat"        , stBody.b_sys_stat);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onSysStatChange
	static int Struct2Packet(SysStat& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("sys_date"          , stBody.sys_date);
		pkt.AddParameter("exch_date"         , stBody.exch_date);
		pkt.AddParameter("m_sys_stat"        , stBody.m_sys_stat);
		pkt.AddParameter("b_sys_stat"        , stBody.b_sys_stat);
		return 0;
	};



	//----------------------------------------------报文接口 [onBaseTableUpdate] 报文结构相关解析
	// 报文解析：报文 -> 结构    onBaseTableUpdate
	static int Packet2Struct(BaseTableUpdate& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("table_name"        , stBody.table_name);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onBaseTableUpdate
	static int Struct2Packet(BaseTableUpdate& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("table_name"        , stBody.table_name);
		return 0;
	};
	//----------------------------------------------报文接口 [onForceLogout] 报文结构相关解析
	// 报文解析：报文 -> 结构    onForceLogout
	static int Packet2Struct(TForceLogout& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("hint_msg"          , stBody.hint_msg);
		pkt.GetParameterVal("session_id"          , stBody.session_id);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onForceLogout
	static int Struct2Packet(TForceLogout& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("hint_msg"          , stBody.hint_msg);
		return 0;
	};



	//----------------------------------------------报文接口 [onNewBourseBulletin] 报文结构相关解析
	// 报文解析：报文 -> 结构    onNewBourseBulletin
	static int Packet2Struct(TNewBourseBulletin& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("seq_no"            , stBody.seq_no);
		pkt.GetParameterVal("title"             , stBody.title);
		pkt.GetParameterVal("content"           , stBody.content);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onNewBourseBulletin
	static int Struct2Packet(TNewBourseBulletin& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("seq_no"            , stBody.seq_no);
		pkt.AddParameter("title"             , stBody.title);
		pkt.AddParameter("content"           , stBody.content);
		return 0;
	};



	//----------------------------------------------报文接口 [onNewMemberBulletin] 报文结构相关解析
	// 报文解析：报文 -> 结构    onNewMemberBulletin
	static int Packet2Struct(TNewMemberBulletin& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("bulletin_id"       , stBody.bulletin_id);
		pkt.GetParameterVal("title"             , stBody.title);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onNewMemberBulletin
	static int Struct2Packet(TNewMemberBulletin& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("bulletin_id"       , stBody.bulletin_id);
		pkt.AddParameter("title"             , stBody.title);
		return 0;
	};



	//----------------------------------------------报文接口 [onSessionKey] 报文结构相关解析
	// 报文解析：报文 -> 结构    onSessionKey
	static int Packet2Struct(TSessionKey& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("session_id"        , stBody.session_id);
		pkt.GetParameterVal("session_key"       , stBody.session_key);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onSessionKey
	static int Struct2Packet(TSessionKey& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("session_id"        , stBody.session_id);
		pkt.AddParameter("session_key"       , stBody.session_key);
		return 0;
	};



	//----------------------------------------------报文接口 [onRiskNotify] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRiskNotify
	static int Packet2Struct(TRiskNotity& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("risk_idx1"         , stBody.risk_idx1);
		pkt.GetParameterVal("risk_idx2"         , stBody.risk_idx2);
		pkt.GetParameterVal("call_margin"      , stBody.call_margin );
		pkt.GetParameterVal("exch_date"         , stBody.exch_date);
		pkt.GetParameterVal("notify_title"      , stBody.notify_title);
		pkt.GetParameterVal("notify_content"    , stBody.notify_content);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRiskNotify
	static int Struct2Packet(TRiskNotity& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("risk_idx1"         , stBody.risk_idx1);
		pkt.AddParameter("risk_idx2"         , stBody.risk_idx2);
		pkt.AddParameter("call_margin "      , stBody.call_margin );
		pkt.AddParameter("exch_date"         , stBody.exch_date);
		pkt.AddParameter("notify_title"      , stBody.notify_title);
		pkt.AddParameter("notify_content"    , stBody.notify_content);
		return 0;
	};

	//----------------------------------------------报文接口 [onRecvRtnSpotInstStateUpdate] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnSpotInstStateUpdate
	static int Packet2Struct(SpotInstState& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("tradeState"        , stBody.tradeState);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnSpotInstStateUpdate
	static int Struct2Packet(SpotInstState& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("instID"            , stBody.instID);
		pkt.AddParameter("tradeState"        , stBody.tradeState);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnForwardInstStateUpdate] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnForwardInstStateUpdate
	static int Packet2Struct(ForwardInstState& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("tradeState"        , stBody.tradeState);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnForwardInstStateUpdate
	static int Struct2Packet(ForwardInstState& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("instID"            , stBody.instID);
		pkt.AddParameter("tradeState"        , stBody.tradeState);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnDeferInstStateUpdate] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferInstStateUpdate
	static int Packet2Struct(DeferInstState& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("tradeState"        , stBody.tradeState);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferInstStateUpdate
	static int Struct2Packet(DeferInstState& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("instID"            , stBody.instID);
		pkt.AddParameter("tradeState"        , stBody.tradeState);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnSpotMarketStateUpdate] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnSpotMarketStateUpdate
	static int Packet2Struct(SpotMarketState& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("marketID"          , stBody.marketID);
		pkt.GetParameterVal("marketState"       , stBody.marketState);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnSpotMarketStateUpdate
	static int Struct2Packet(SpotMarketState& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("marketID"          , stBody.marketID);
		pkt.AddParameter("marketState"       , stBody.marketState);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnForwardMarketStateUpdate] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnForwardMarketStateUpdate
	static int Packet2Struct(ForwardMarketState& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("marketID"          , stBody.marketID);
		pkt.GetParameterVal("marketState"       , stBody.marketState);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnForwardMarketStateUpdate
	static int Struct2Packet(ForwardMarketState& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("marketID"          , stBody.marketID);
		pkt.AddParameter("marketState"       , stBody.marketState);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnDeferMarketStateUpdate] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferMarketStateUpdate
	static int Packet2Struct(DeferMarketState& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("marketID"          , stBody.marketID);
		pkt.GetParameterVal("marketState"       , stBody.marketState);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferMarketStateUpdate
	static int Struct2Packet(DeferMarketState& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("marketID"          , stBody.marketID);
		pkt.AddParameter("marketState"       , stBody.marketState);
		return 0;
	};



	//----------------------------------------------报文接口 [onAcctCapitalAccess] 报文结构相关解析
	// 报文解析：报文 -> 结构    onAcctCapitalAccess
	static int Packet2Struct(AcctCapitalAccess& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("fund_src_type"     , stBody.fund_src_type);
		pkt.GetParameterVal("serial_no"         , stBody.serial_no);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("access_way"        , stBody.access_way);
		pkt.GetParameterVal("amount"            , stBody.amount);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onAcctCapitalAccess
	static int Struct2Packet(AcctCapitalAccess& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("fund_src_type"     , stBody.fund_src_type);
		pkt.AddParameter("serial_no"         , stBody.serial_no);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("access_way"        , stBody.access_way);
		pkt.AddParameter("amount"            , stBody.amount);
		return 0;
	};



	//----------------------------------------------报文接口 [onCustInfoChange] 报文结构相关解析
	// 报文解析：报文 -> 结构    onCustInfoChange
	static int Packet2Struct(CustInfoChange& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("update_flag"       , stBody.update_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("member_id"         , stBody.member_id);
		pkt.GetParameterVal("cust_abbr"         , stBody.cust_abbr);
		pkt.GetParameterVal("m_Fare_model_id"   , stBody.m_Fare_model_id);
		pkt.GetParameterVal("b_Fare_model_id"   , stBody.b_Fare_model_id);
		pkt.GetParameterVal("acct_type"         , stBody.acct_type);
		pkt.GetParameterVal("acct_stat"         , stBody.acct_stat);
		pkt.GetParameterVal("branch_ID"         , stBody.branch_ID);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onCustInfoChange
	static int Struct2Packet(CustInfoChange& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("update_flag"       , stBody.update_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("member_id"         , stBody.member_id);
		pkt.AddParameter("cust_abbr"         , stBody.cust_abbr);
		pkt.AddParameter("m_Fare_model_id"   , stBody.m_Fare_model_id);
		pkt.AddParameter("b_Fare_model_id"   , stBody.b_Fare_model_id);
		pkt.AddParameter("acct_type"         , stBody.acct_type);
		pkt.AddParameter("acct_stat"         , stBody.acct_stat);
		pkt.AddParameter("branch_ID"         , stBody.branch_ID);
		return 0;
	};



	//----------------------------------------------报文接口 [onCustRiskDegreeChange] 报文结构相关解析
	// 报文解析：报文 -> 结构    onCustRiskDegreeChange
	static int Packet2Struct(CustRiskDegreeChange& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("risk_degree1"      , stBody.risk_degree1);
		pkt.GetParameterVal("risk_degree2"      , stBody.risk_degree2);
		pkt.GetParameterVal("risk_grade"        , stBody.risk_grade);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onCustRiskDegreeChange
	static int Struct2Packet(CustRiskDegreeChange& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("risk_degree1"      , stBody.risk_degree1);
		pkt.AddParameter("risk_degree2"      , stBody.risk_degree2);
		pkt.AddParameter("risk_grade"        , stBody.risk_grade);
		return 0;
	};

	//---------------------------------------------- [RspMsg] 结构相关解析
	// 报文解析：报文 -> 结构    RspMsg
	static int Packet2Struct(RspMsg &v, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("RspCode",v.RspCode);
		pkt.GetParameterVal("RspMsg",v.RspMsg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    RspMsg
	static int Struct2Packet(RspMsg &v, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("RspCode",v.RspCode);
		pkt.AddParameter("RspMsg",v.RspMsg);
		return 0;
	};  
		
		//设置  TSpotOrder:现货报单信息 结构的值
    static int Packet2Struct(SpotOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号
        pkt.GetParameterVal("instID",v.instID);    //合约代码 (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码 (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码 (not null)
        pkt.GetParameterVal("buyOrSell",v.buyOrSell);    //买卖方向 (not null)
        pkt.GetParameterVal("applyDate",v.applyDate);    //申报日期
        pkt.GetParameterVal("applyTime",v.applyTime);    //申报时间
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码 (not null)
        pkt.GetParameterVal("price",v.price);    //价格  (not null)
        pkt.GetParameterVal("amount",v.amount);    //数量  (not null)
        pkt.GetParameterVal("remainAmount",v.remainAmount);    //剩余数量
        pkt.GetParameterVal("cancelTime",v.cancelTime);    //撤消时间
        pkt.GetParameterVal("cancelID",v.cancelID);    //撤消员代码
        pkt.GetParameterVal("status",v.status);    //状态
        pkt.GetParameterVal("matchType",v.matchType);    //类型  (not null)
        pkt.GetParameterVal("endorseInstID",v.endorseInstID);    //转让合约代码 (not null)
        pkt.GetParameterVal("localOrderNo",v.localOrderNo);    //本地报单号 (not null)
        return 0;
    }

    //将  TSpotOrder:现货报单信息 结构的值组成字符串
    static int Struct2Packet(SpotOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("buyOrSell",v.buyOrSell);
        pkt.AddParameter("applyDate",v.applyDate);
        pkt.AddParameter("applyTime",v.applyTime);
        pkt.AddParameter("traderID",v.traderID);
        pkt.AddParameter("price",v.price);
        pkt.AddParameter("amount",v.amount);
        pkt.AddParameter("remainAmount",v.remainAmount);
        pkt.AddParameter("cancelTime",v.cancelTime);
        pkt.AddParameter("cancelID",v.cancelID);
        pkt.AddParameter("status",v.status);
        pkt.AddParameter("matchType",v.matchType);
        pkt.AddParameter("endorseInstID",v.endorseInstID);
        pkt.AddParameter("localOrderNo",v.localOrderNo);
        return 0;
    }
	
		//将  TSpotOrderCancel:现货撤单请求 结构的值组成字符串
    static int Struct2Packet(SpotOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("traderID",v.traderID);
    }
	
		//设置  TDeferOrderCancel:现货延期交收撤单请求 结构的值
    static int Packet2Struct(SpotOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号  (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码  (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码  (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码  (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码  (not null)
        return 0;
    }
    
    //设置  TDeferOrder:现货延期交收报单 结构的值
    static int Packet2Struct(DeferOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号 (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码 (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码 (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码 (not null)
        pkt.GetParameterVal("buyOrSell",v.buyOrSell);    //买卖方向 (not null)
        pkt.GetParameterVal("applyDate",v.applyDate);    //申报日期
        pkt.GetParameterVal("applyTime",v.applyTime);    //申报时间
        pkt.GetParameterVal("offSetFlag",v.offSetFlag);    //开平仓标志 (not null)
        pkt.GetParameterVal("price",v.price);    //价格   (not null)
        pkt.GetParameterVal("amount",v.amount);    //数量  (not null)
        pkt.GetParameterVal("remainAmount",v.remainAmount);    //剩余数量
        pkt.GetParameterVal("cancelTime",v.cancelTime);    //撤消时间
        pkt.GetParameterVal("status",v.status);    //状态
        pkt.GetParameterVal("localOrderNo",v.localOrderNo);    //本地报单号  (not null)
        pkt.GetParameterVal("margin",v.margin);    //保证金率
        pkt.GetParameterVal("marginType",v.marginType);    //保证金类型  (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码  (not null)
        pkt.GetParameterVal("cancelID",v.cancelID);    //撤消员代码
        pkt.GetParameterVal("matchType",v.matchType);    //类型  (not null)
        return 0;
    }

    //将  TDeferOrder:现货延期交收报单 结构的值组成字符串
    static int Struct2Packet(DeferOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("buyOrSell",v.buyOrSell);
        pkt.AddParameter("applyDate",v.applyDate);
        pkt.AddParameter("applyTime",v.applyTime);
        pkt.AddParameter("offSetFlag",v.offSetFlag);
        pkt.AddParameter("price",v.price);
        pkt.AddParameter("amount",v.amount);
        pkt.AddParameter("remainAmount",v.remainAmount);
        pkt.AddParameter("cancelTime",v.cancelTime);
        pkt.AddParameter("status",v.status);
        pkt.AddParameter("localOrderNo",v.localOrderNo);
        pkt.AddParameter("margin",v.margin);
        pkt.AddParameter("marginType",v.marginType);
        pkt.AddParameter("traderID",v.traderID);
        pkt.AddParameter("cancelID",v.cancelID);
        pkt.AddParameter("matchType",v.matchType);
        return 0;
    }

		//设置  TDeferOrderCancel:现货延期交收撤单请求 结构的值
    static int Packet2Struct(DeferOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号  (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码  (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码  (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码  (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码  (not null)
        return 0;
    }

    //将  TDeferOrderCancel:现货延期交收撤单请求 结构的值组成字符串
    static int Struct2Packet(DeferOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("traderID",v.traderID);
        return 0;
    }
    
    //设置  TDeferDeliveryAppOrder:现货延期交收交割申报报单 结构的值
    static int Packet2Struct(DeferDeliveryAppOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单编号
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码  (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码  (not null)
        pkt.GetParameterVal("buyOrSell",v.buyOrSell);    //买卖方向  (not null)
        pkt.GetParameterVal("applyDate",v.applyDate);    //申请日期
        pkt.GetParameterVal("applyTime",v.applyTime);    //申请时间
        pkt.GetParameterVal("amount",v.amount);    //数量  (not null)
        pkt.GetParameterVal("cancelTime",v.cancelTime);    //撤消时间
        pkt.GetParameterVal("status",v.status);    //报单状态
        pkt.GetParameterVal("localOrderNo",v.localOrderNo);    //本地报单编号 (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码  (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码  (not null)
        pkt.GetParameterVal("cancelID",v.cancelID);    //撤消员代码
        pkt.GetParameterVal("remainAmount",v.remainAmount);    //剩余数量
        return 0;
    }

    //将  TDeferDeliveryAppOrder:现货延期交收交割申报报单 结构的值组成字符串
    static int Struct2Packet(DeferDeliveryAppOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("buyOrSell",v.buyOrSell);
        pkt.AddParameter("applyDate",v.applyDate);
        pkt.AddParameter("applyTime",v.applyTime);
        pkt.AddParameter("amount",v.amount);
        pkt.AddParameter("cancelTime",v.cancelTime);
        pkt.AddParameter("status",v.status);
        pkt.AddParameter("localOrderNo",v.localOrderNo);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("traderID",v.traderID);
        pkt.AddParameter("cancelID",v.cancelID);
        pkt.AddParameter("remainAmount",v.remainAmount);
        return 0;
    }
    
    
    //设置  TDeferDeliveryAppOrderCancel:现货延期交收交割申报撤单请求 结构的值
    static int Packet2Struct(DeferDeliveryAppOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号  (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码  (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码  (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码 (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码 (not null)
        return 0;
    }

    //将  TDeferDeliveryAppOrderCancel:现货延期交收交割申报撤单请求 结构的值组成字符串
    static int Struct2Packet(DeferDeliveryAppOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("traderID",v.traderID);
        return 0;
    }
    
    //设置  TMiddleAppOrder:现货延期交收中立仓申报报单 结构的值
    static int Packet2Struct(MiddleAppOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单编号
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码 (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码 (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码  (not null)
        pkt.GetParameterVal("buyOrSell",v.buyOrSell);    //买卖方向  (not null)
        pkt.GetParameterVal("applyDate",v.applyDate);    //申请日期
        pkt.GetParameterVal("applyTime",v.applyTime);    //申请时间
        pkt.GetParameterVal("amount",v.amount);    //数量  (not null)
        pkt.GetParameterVal("cancelTime",v.cancelTime);    //撤消时间
        pkt.GetParameterVal("status",v.status);    //报单状态
        pkt.GetParameterVal("localOrderNo",v.localOrderNo);    //本地报单编号 (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码  (not null)
        pkt.GetParameterVal("cancelID",v.cancelID);    //撤消员代码
        pkt.GetParameterVal("remainAmount",v.remainAmount);    //剩余数量
        return 0;
    }

    //将  TMiddleAppOrder:现货延期交收中立仓申报报单 结构的值组成字符串/
    static int Struct2Packet(MiddleAppOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("buyOrSell",v.buyOrSell);
        pkt.AddParameter("applyDate",v.applyDate);
        pkt.AddParameter("applyTime",v.applyTime);
        pkt.AddParameter("amount",v.amount);
        pkt.AddParameter("cancelTime",v.cancelTime);
        pkt.AddParameter("status",v.status);
        pkt.AddParameter("localOrderNo",v.localOrderNo);
        pkt.AddParameter("traderID",v.traderID);
        pkt.AddParameter("cancelID",v.cancelID);
        pkt.AddParameter("remainAmount",v.remainAmount);
        return 0;
    }
    
    //设置  TMiddleAppOrderCancel:现货延期交收中立仓申报撤单请求 结构的值
    static int Packet2Struct(MiddleAppOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号  (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码  (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码 (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码 (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码 (not null)
        return 0;
    }

    //TMiddleAppOrderCancel:现货延期交收中立仓申报撤单请求 结构的值组成字符串
    static int Struct2Packet(MiddleAppOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("traderID",v.traderID);
        return 0;
    }
          
    //设置  TForwardOrder:现货T+N报单 结构的值/
    static int Packet2Struct(ForwardOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号
        pkt.GetParameterVal("instID",v.instID);    //合约代码  (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码  (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码  (not null)
        pkt.GetParameterVal("buyOrSell",v.buyOrSell);    //买卖方向  (not null)
        pkt.GetParameterVal("applyDate",v.applyDate);    //申报日期
        pkt.GetParameterVal("applyTime",v.applyTime);    //申报时间
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码 (not null)
        pkt.GetParameterVal("price",v.price);    //价格   (not null)
        pkt.GetParameterVal("amount",v.amount);    //数量  (not null)
        pkt.GetParameterVal("remainAmount",v.remainAmount);    //剩余数量
        pkt.GetParameterVal("cancelTime",v.cancelTime);    //撤消时间
        pkt.GetParameterVal("cancelID",v.cancelID);    //撤消员代码
        pkt.GetParameterVal("status",v.status);    //状态
        pkt.GetParameterVal("localOrderNo",v.localOrderNo);    //本地报单号  (not null)
        pkt.GetParameterVal("matchType",v.matchType);    //类型  (not null)
        return 0;
    }

    //将  TForwardOrder:现货T+N报单 结构的值组成字符串/
    static int Struct2Packet(ForwardOrder &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("buyOrSell",v.buyOrSell);
        pkt.AddParameter("applyDate",v.applyDate);
        pkt.AddParameter("applyTime",v.applyTime);
        pkt.AddParameter("traderID",v.traderID);
        pkt.AddParameter("price",v.price);
        pkt.AddParameter("amount",v.amount);
        pkt.AddParameter("remainAmount",v.remainAmount);
        pkt.AddParameter("cancelTime",v.cancelTime);
        pkt.AddParameter("cancelID",v.cancelID);
        pkt.AddParameter("status",v.status);
        pkt.AddParameter("localOrderNo",v.localOrderNo);
        pkt.AddParameter("matchType",v.matchType);
		return 0;
    }
    
    //设置  TForwardOrderCancel:现货T+N撤单请求 结构的值
    static int Packet2Struct(ForwardOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.GetParameterVal("orderNo",v.orderNo);    //报单号  (not null)
        pkt.GetParameterVal("instID",v.instID);    //合约代码  (not null)
        pkt.GetParameterVal("clientID",v.clientID);    //客户代码  (not null)
        pkt.GetParameterVal("memberID",v.memberID);    //会员代码  (not null)
        pkt.GetParameterVal("traderID",v.traderID);    //交易员代码 (not null)
        return 0;
    }

    //将  TForwardOrderCancel:现货T+N撤单请求 结构的值组成字符串/
    static int Struct2Packet(ForwardOrderCancel &v, CBroadcastPacket & pkt) 
    {
        pkt.AddParameter("orderNo",v.orderNo);
        pkt.AddParameter("instID",v.instID);
        pkt.AddParameter("clientID",v.clientID);
        pkt.AddParameter("memberID",v.memberID);
        pkt.AddParameter("traderID",v.traderID);
		return 0;
    }

	//----------------------------------------------报文接口 [onRecvSpotQuotation] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvSpotQuotation
	static int Packet2Struct(SpotQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("name"              , stBody.name);
		pkt.GetParameterVal("lastClose"         , stBody.lastClose);
		pkt.GetParameterVal("open"              , stBody.open);
		pkt.GetParameterVal("high"              , stBody.high);
		pkt.GetParameterVal("low"               , stBody.low);
		pkt.GetParameterVal("last"              , stBody.last);
		pkt.GetParameterVal("close"             , stBody.close);
		pkt.GetParameterVal("bid1"              , stBody.bid1);
		pkt.GetParameterVal("bidLot1"           , stBody.bidLot1);
		pkt.GetParameterVal("bid2"              , stBody.bid2);
		pkt.GetParameterVal("bidLot2"           , stBody.bidLot2);
		pkt.GetParameterVal("bid3"              , stBody.bid3);
		pkt.GetParameterVal("bidLot3"           , stBody.bidLot3);
		pkt.GetParameterVal("bid4"              , stBody.bid4);
		pkt.GetParameterVal("bidLot4"           , stBody.bidLot4);
		pkt.GetParameterVal("bid5"              , stBody.bid5);
		pkt.GetParameterVal("bidLot5"           , stBody.bidLot5);
		pkt.GetParameterVal("ask1"              , stBody.ask1);
		pkt.GetParameterVal("askLot1"           , stBody.askLot1);
		pkt.GetParameterVal("ask2"              , stBody.ask2);
		pkt.GetParameterVal("askLot2"           , stBody.askLot2);
		pkt.GetParameterVal("ask3"              , stBody.ask3);
		pkt.GetParameterVal("askLot3"           , stBody.askLot3);
		pkt.GetParameterVal("ask4"              , stBody.ask4);
		pkt.GetParameterVal("askLot4"           , stBody.askLot4);
		pkt.GetParameterVal("ask5"              , stBody.ask5);
		pkt.GetParameterVal("askLot5"           , stBody.askLot5);
		pkt.GetParameterVal("volume"            , stBody.volume);
		pkt.GetParameterVal("weight"            , stBody.weight);
		pkt.GetParameterVal("highLimit"         , stBody.highLimit);
		pkt.GetParameterVal("lowLimit"          , stBody.lowLimit);
		pkt.GetParameterVal("upDown"            , stBody.upDown);
		pkt.GetParameterVal("upDownRate"        , stBody.upDownRate);
		pkt.GetParameterVal("turnOver"          , stBody.turnOver);
		pkt.GetParameterVal("average"           , stBody.average);
		pkt.GetParameterVal("quoteDate"         , stBody.quoteDate);
		pkt.GetParameterVal("quoteTime"         , stBody.quoteTime);
		pkt.GetParameterVal("sequenceNo"        , stBody.sequenceNo);
		pkt.GetParameterVal("sZipBuff"          , stBody.sZipBuff);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvSpotQuotation
	static int Struct2Packet(SpotQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("instID"            , stBody.instID);
		pkt.AddParameter("name"              , stBody.name);
		pkt.AddParameter("lastClose"         , stBody.lastClose);
		pkt.AddParameter("open"              , stBody.open);
		pkt.AddParameter("high"              , stBody.high);
		pkt.AddParameter("low"               , stBody.low);
		pkt.AddParameter("last"              , stBody.last);
		pkt.AddParameter("close"             , stBody.close);
		pkt.AddParameter("bid1"              , stBody.bid1);
		pkt.AddParameter("bidLot1"           , stBody.bidLot1);
		pkt.AddParameter("bid2"              , stBody.bid2);
		pkt.AddParameter("bidLot2"           , stBody.bidLot2);
		pkt.AddParameter("bid3"              , stBody.bid3);
		pkt.AddParameter("bidLot3"           , stBody.bidLot3);
		pkt.AddParameter("bid4"              , stBody.bid4);
		pkt.AddParameter("bidLot4"           , stBody.bidLot4);
		pkt.AddParameter("bid5"              , stBody.bid5);
		pkt.AddParameter("bidLot5"           , stBody.bidLot5);
		pkt.AddParameter("ask1"              , stBody.ask1);
		pkt.AddParameter("askLot1"           , stBody.askLot1);
		pkt.AddParameter("ask2"              , stBody.ask2);
		pkt.AddParameter("askLot2"           , stBody.askLot2);
		pkt.AddParameter("ask3"              , stBody.ask3);
		pkt.AddParameter("askLot3"           , stBody.askLot3);
		pkt.AddParameter("ask4"              , stBody.ask4);
		pkt.AddParameter("askLot4"           , stBody.askLot4);
		pkt.AddParameter("ask5"              , stBody.ask5);
		pkt.AddParameter("askLot5"           , stBody.askLot5);
		pkt.AddParameter("volume"            , stBody.volume);
		pkt.AddParameter("weight"            , stBody.weight);
		pkt.AddParameter("highLimit"         , stBody.highLimit);
		pkt.AddParameter("lowLimit"          , stBody.lowLimit);
		pkt.AddParameter("upDown"            , stBody.upDown);
		pkt.AddParameter("upDownRate"        , stBody.upDownRate);
		pkt.AddParameter("turnOver"          , stBody.turnOver);
		pkt.AddParameter("average"           , stBody.average);
		pkt.AddParameter("quoteDate"         , stBody.quoteDate);
		pkt.AddParameter("quoteTime"         , stBody.quoteTime);
		pkt.AddParameter("sequenceNo"        , stBody.sequenceNo);
		pkt.AddParameter("sZipBuff"          , stBody.sZipBuff);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvForwardQuotation] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvForwardQuotation
	static int Packet2Struct(ForwardQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("name"              , stBody.name);
		pkt.GetParameterVal("lastSettle"        , stBody.lastSettle);
		pkt.GetParameterVal("lastClose"         , stBody.lastClose);
		pkt.GetParameterVal("open"              , stBody.open);
		pkt.GetParameterVal("high"              , stBody.high);
		pkt.GetParameterVal("low"               , stBody.low);
		pkt.GetParameterVal("last"              , stBody.last);
		pkt.GetParameterVal("close"             , stBody.close);
		pkt.GetParameterVal("settle"            , stBody.settle);
		pkt.GetParameterVal("bid1"              , stBody.bid1);
		pkt.GetParameterVal("bidLot1"           , stBody.bidLot1);
		pkt.GetParameterVal("bid2"              , stBody.bid2);
		pkt.GetParameterVal("bidLot2"           , stBody.bidLot2);
		pkt.GetParameterVal("bid3"              , stBody.bid3);
		pkt.GetParameterVal("bidLot3"           , stBody.bidLot3);
		pkt.GetParameterVal("bid4"              , stBody.bid4);
		pkt.GetParameterVal("bidLot4"           , stBody.bidLot4);
		pkt.GetParameterVal("bid5"              , stBody.bid5);
		pkt.GetParameterVal("bidLot5"           , stBody.bidLot5);
		pkt.GetParameterVal("ask1"              , stBody.ask1);
		pkt.GetParameterVal("askLot1"           , stBody.askLot1);
		pkt.GetParameterVal("ask2"              , stBody.ask2);
		pkt.GetParameterVal("askLot2"           , stBody.askLot2);
		pkt.GetParameterVal("ask3"              , stBody.ask3);
		pkt.GetParameterVal("askLot3"           , stBody.askLot3);
		pkt.GetParameterVal("ask4"              , stBody.ask4);
		pkt.GetParameterVal("askLot4"           , stBody.askLot4);
		pkt.GetParameterVal("ask5"              , stBody.ask5);
		pkt.GetParameterVal("askLot5"           , stBody.askLot5);
		pkt.GetParameterVal("volume"            , stBody.volume);
		pkt.GetParameterVal("weight"            , stBody.weight);
		pkt.GetParameterVal("highLimit"         , stBody.highLimit);
		pkt.GetParameterVal("lowLimit"          , stBody.lowLimit);
		pkt.GetParameterVal("Posi"              , stBody.Posi);
		pkt.GetParameterVal("upDown"            , stBody.upDown);
		pkt.GetParameterVal("upDownRate"        , stBody.upDownRate);
		pkt.GetParameterVal("turnOver"          , stBody.turnOver);
		pkt.GetParameterVal("average"           , stBody.average);
		pkt.GetParameterVal("quoteDate"         , stBody.quoteDate);
		pkt.GetParameterVal("quoteTime"         , stBody.quoteTime);
		pkt.GetParameterVal("sequenceNo"        , stBody.sequenceNo);
		pkt.GetParameterVal("sZipBuff"          , stBody.sZipBuff);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvForwardQuotation
	static int Struct2Packet(ForwardQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("instID"            , stBody.instID);
		pkt.AddParameter("name"              , stBody.name);
		pkt.AddParameter("lastSettle"        , stBody.lastSettle);
		pkt.AddParameter("lastClose"         , stBody.lastClose);
		pkt.AddParameter("open"              , stBody.open);
		pkt.AddParameter("high"              , stBody.high);
		pkt.AddParameter("low"               , stBody.low);
		pkt.AddParameter("last"              , stBody.last);
		pkt.AddParameter("close"             , stBody.close);
		pkt.AddParameter("settle"            , stBody.settle);
		pkt.AddParameter("bid1"              , stBody.bid1);
		pkt.AddParameter("bidLot1"           , stBody.bidLot1);
		pkt.AddParameter("bid2"              , stBody.bid2);
		pkt.AddParameter("bidLot2"           , stBody.bidLot2);
		pkt.AddParameter("bid3"              , stBody.bid3);
		pkt.AddParameter("bidLot3"           , stBody.bidLot3);
		pkt.AddParameter("bid4"              , stBody.bid4);
		pkt.AddParameter("bidLot4"           , stBody.bidLot4);
		pkt.AddParameter("bid5"              , stBody.bid5);
		pkt.AddParameter("bidLot5"           , stBody.bidLot5);
		pkt.AddParameter("ask1"              , stBody.ask1);
		pkt.AddParameter("askLot1"           , stBody.askLot1);
		pkt.AddParameter("ask2"              , stBody.ask2);
		pkt.AddParameter("askLot2"           , stBody.askLot2);
		pkt.AddParameter("ask3"              , stBody.ask3);
		pkt.AddParameter("askLot3"           , stBody.askLot3);
		pkt.AddParameter("ask4"              , stBody.ask4);
		pkt.AddParameter("askLot4"           , stBody.askLot4);
		pkt.AddParameter("ask5"              , stBody.ask5);
		pkt.AddParameter("askLot5"           , stBody.askLot5);
		pkt.AddParameter("volume"            , stBody.volume);
		pkt.AddParameter("weight"            , stBody.weight);
		pkt.AddParameter("highLimit"         , stBody.highLimit);
		pkt.AddParameter("lowLimit"          , stBody.lowLimit);
		pkt.AddParameter("Posi"              , stBody.Posi);
		pkt.AddParameter("upDown"            , stBody.upDown);
		pkt.AddParameter("upDownRate"        , stBody.upDownRate);
		pkt.AddParameter("turnOver"          , stBody.turnOver);
		pkt.AddParameter("average"           , stBody.average);
		pkt.AddParameter("quoteDate"         , stBody.quoteDate);
		pkt.AddParameter("quoteTime"         , stBody.quoteTime);
		pkt.AddParameter("sequenceNo"        , stBody.sequenceNo);
		pkt.AddParameter("sZipBuff"          , stBody.sZipBuff);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvDeferQuotation] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvDeferQuotation
	static int Packet2Struct(DeferQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"            , stBody.instID);
		pkt.GetParameterVal("name"              , stBody.name);
		pkt.GetParameterVal("lastSettle"        , stBody.lastSettle);
		pkt.GetParameterVal("lastClose"         , stBody.lastClose);
		pkt.GetParameterVal("open"              , stBody.open);
		pkt.GetParameterVal("high"              , stBody.high);
		pkt.GetParameterVal("low"               , stBody.low);
		pkt.GetParameterVal("last"              , stBody.last);
		pkt.GetParameterVal("close"             , stBody.close);
		pkt.GetParameterVal("settle"            , stBody.settle);
		pkt.GetParameterVal("bid1"              , stBody.bid1);
		pkt.GetParameterVal("bidLot1"           , stBody.bidLot1);
		pkt.GetParameterVal("bid2"              , stBody.bid2);
		pkt.GetParameterVal("bidLot2"           , stBody.bidLot2);
		pkt.GetParameterVal("bid3"              , stBody.bid3);
		pkt.GetParameterVal("bidLot3"           , stBody.bidLot3);
		pkt.GetParameterVal("bid4"              , stBody.bid4);
		pkt.GetParameterVal("bidLot4"           , stBody.bidLot4);
		pkt.GetParameterVal("bid5"              , stBody.bid5);
		pkt.GetParameterVal("bidLot5"           , stBody.bidLot5);
		pkt.GetParameterVal("ask1"              , stBody.ask1);
		pkt.GetParameterVal("askLot1"           , stBody.askLot1);
		pkt.GetParameterVal("ask2"              , stBody.ask2);
		pkt.GetParameterVal("askLot2"           , stBody.askLot2);
		pkt.GetParameterVal("ask3"              , stBody.ask3);
		pkt.GetParameterVal("askLot3"           , stBody.askLot3);
		pkt.GetParameterVal("ask4"              , stBody.ask4);
		pkt.GetParameterVal("askLot4"           , stBody.askLot4);
		pkt.GetParameterVal("ask5"              , stBody.ask5);
		pkt.GetParameterVal("askLot5"           , stBody.askLot5);
		pkt.GetParameterVal("volume"            , stBody.volume);
		pkt.GetParameterVal("weight"            , stBody.weight);
		pkt.GetParameterVal("highLimit"         , stBody.highLimit);
		pkt.GetParameterVal("lowLimit"          , stBody.lowLimit);
		pkt.GetParameterVal("Posi"              , stBody.Posi);
		pkt.GetParameterVal("upDown"            , stBody.upDown);
		pkt.GetParameterVal("upDownRate"        , stBody.upDownRate);
		pkt.GetParameterVal("turnOver"          , stBody.turnOver);
		pkt.GetParameterVal("average"           , stBody.average);
		pkt.GetParameterVal("quoteDate"         , stBody.quoteDate);
		pkt.GetParameterVal("quoteTime"         , stBody.quoteTime);
		pkt.GetParameterVal("sequenceNo"        , stBody.sequenceNo);
		pkt.GetParameterVal("sZipBuff"          , stBody.sZipBuff);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvDeferQuotation
	static int Struct2Packet(DeferQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("instID"            , stBody.instID);
		pkt.AddParameter("name"              , stBody.name);
		pkt.AddParameter("lastSettle"        , stBody.lastSettle);
		pkt.AddParameter("lastClose"         , stBody.lastClose);
		pkt.AddParameter("open"              , stBody.open);
		pkt.AddParameter("high"              , stBody.high);
		pkt.AddParameter("low"               , stBody.low);
		pkt.AddParameter("last"              , stBody.last);
		pkt.AddParameter("close"             , stBody.close);
		pkt.AddParameter("settle"            , stBody.settle);
		pkt.AddParameter("bid1"              , stBody.bid1);
		pkt.AddParameter("bidLot1"           , stBody.bidLot1);
		pkt.AddParameter("bid2"              , stBody.bid2);
		pkt.AddParameter("bidLot2"           , stBody.bidLot2);
		pkt.AddParameter("bid3"              , stBody.bid3);
		pkt.AddParameter("bidLot3"           , stBody.bidLot3);
		pkt.AddParameter("bid4"              , stBody.bid4);
		pkt.AddParameter("bidLot4"           , stBody.bidLot4);
		pkt.AddParameter("bid5"              , stBody.bid5);
		pkt.AddParameter("bidLot5"           , stBody.bidLot5);
		pkt.AddParameter("ask1"              , stBody.ask1);
		pkt.AddParameter("askLot1"           , stBody.askLot1);
		pkt.AddParameter("ask2"              , stBody.ask2);
		pkt.AddParameter("askLot2"           , stBody.askLot2);
		pkt.AddParameter("ask3"              , stBody.ask3);
		pkt.AddParameter("askLot3"           , stBody.askLot3);
		pkt.AddParameter("ask4"              , stBody.ask4);
		pkt.AddParameter("askLot4"           , stBody.askLot4);
		pkt.AddParameter("ask5"              , stBody.ask5);
		pkt.AddParameter("askLot5"           , stBody.askLot5);
		pkt.AddParameter("volume"            , stBody.volume);
		pkt.AddParameter("weight"            , stBody.weight);
		pkt.AddParameter("highLimit"         , stBody.highLimit);
		pkt.AddParameter("lowLimit"          , stBody.lowLimit);
		pkt.AddParameter("Posi"              , stBody.Posi);
		pkt.AddParameter("upDown"            , stBody.upDown);
		pkt.AddParameter("upDownRate"        , stBody.upDownRate);
		pkt.AddParameter("turnOver"          , stBody.turnOver);
		pkt.AddParameter("average"           , stBody.average);
		pkt.AddParameter("quoteDate"         , stBody.quoteDate);
		pkt.AddParameter("quoteTime"         , stBody.quoteTime);
		pkt.AddParameter("sequenceNo"        , stBody.sequenceNo);
		pkt.AddParameter("sZipBuff"          , stBody.sZipBuff);
		return 0;
	};
	
	//----------------------------------------------报文接口 [onRecvDeferDeliveryQuotation] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvDeferDeliveryQuotation
	static int Packet2Struct(DeferDeliveryQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("instID"         , stBody.instID);
		pkt.GetParameterVal("bidLot"         , stBody.bidLot);
		pkt.GetParameterVal("askLot"         , stBody.askLot);
		pkt.GetParameterVal("midBidLot"      , stBody.midBidLot);
		pkt.GetParameterVal("midAskLot"      , stBody.midAskLot);		
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvDeferDeliveryQuotation
	static int Struct2Packet(DeferDeliveryQuotation& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("instID"         , stBody.instID);
		pkt.AddParameter("bidLot"         , stBody.bidLot);
		pkt.AddParameter("askLot"         , stBody.askLot);
		pkt.AddParameter("midBidLot"      , stBody.midBidLot);
		pkt.AddParameter("midAskLot"      , stBody.midAskLot);		
		return 0;
	};


	//----------------------------------------------报文接口 [onRecvRtnSpotMatch] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnSpotMatch
	static int Packet2Struct(PktSpotMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.stBody.matchNo );
		pkt.GetParameterVal("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.GetParameterVal("clientID"          , stBody.stBody.clientID);
		pkt.GetParameterVal("clientType"        , stBody.stBody.clientType);
		pkt.GetParameterVal("memberID"          , stBody.stBody.memberID);
		pkt.GetParameterVal("instID"            , stBody.stBody.instID);
		pkt.GetParameterVal("matchDate"         , stBody.stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.stBody.matchTime);
		pkt.GetParameterVal("price"             , stBody.stBody.price);
		pkt.GetParameterVal("volume"            , stBody.stBody.volume);
		pkt.GetParameterVal("orderNo"           , stBody.stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.stBody.localOrderNo);
		pkt.GetParameterVal("matchType"         , stBody.stBody.matchType);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		pkt.GetParameterVal("RspCode"           , stBody.stRsp.RspCode);
		pkt.GetParameterVal("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnSpotMatch
	static int Struct2Packet(PktSpotMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("matchNo "          , stBody.stBody.matchNo );
		pkt.AddParameter("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.AddParameter("clientID"          , stBody.stBody.clientID);
		pkt.AddParameter("clientType"        , stBody.stBody.clientType);
		pkt.AddParameter("memberID"          , stBody.stBody.memberID);
		pkt.AddParameter("instID"            , stBody.stBody.instID);
		pkt.AddParameter("matchDate"         , stBody.stBody.matchDate);
		pkt.AddParameter("matchTime"         , stBody.stBody.matchTime);
		pkt.AddParameter("price"             , stBody.stBody.price);
		pkt.AddParameter("volume"            , stBody.stBody.volume);
		pkt.AddParameter("orderNo"           , stBody.stBody.orderNo);
		pkt.AddParameter("localOrderNo"      , stBody.stBody.localOrderNo);
		pkt.AddParameter("matchType"         , stBody.stBody.matchType);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		pkt.AddParameter("RspCode"           , stBody.stRsp.RspCode);
		pkt.AddParameter("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnForwardMatch] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnForwardMatch
	static int Packet2Struct(PktForwardMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.stBody.matchNo);
		pkt.GetParameterVal("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.GetParameterVal("clientID"          , stBody.stBody.clientID);
		pkt.GetParameterVal("clientType"        , stBody.stBody.clientType);
		pkt.GetParameterVal("instID"            , stBody.stBody.instID);
		pkt.GetParameterVal("memberID"          , stBody.stBody.memberID);
		pkt.GetParameterVal("matchDate"         , stBody.stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.stBody.matchTime);
		pkt.GetParameterVal("price"             , stBody.stBody.price);
		pkt.GetParameterVal("volume"            , stBody.stBody.volume);
		pkt.GetParameterVal("marginRate"        , stBody.stBody.marginRate);
		pkt.GetParameterVal("rateType"          , stBody.stBody.rateType);
		pkt.GetParameterVal("orderNo"           , stBody.stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.stBody.localOrderNo);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		pkt.GetParameterVal("RspCode"           , stBody.stRsp.RspCode);
		pkt.GetParameterVal("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnForwardMatch
	static int Struct2Packet(PktForwardMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("matchNo"           , stBody.stBody.matchNo);
		pkt.AddParameter("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.AddParameter("clientID"          , stBody.stBody.clientID);
		pkt.AddParameter("clientType"        , stBody.stBody.clientType);
		pkt.AddParameter("instID"            , stBody.stBody.instID);
		pkt.AddParameter("memberID"          , stBody.stBody.memberID);
		pkt.AddParameter("matchDate"         , stBody.stBody.matchDate);
		pkt.AddParameter("matchTime"         , stBody.stBody.matchTime);
		pkt.AddParameter("price"             , stBody.stBody.price);
		pkt.AddParameter("volume"            , stBody.stBody.volume);
		pkt.AddParameter("marginRate"        , stBody.stBody.marginRate);
		pkt.AddParameter("rateType"          , stBody.stBody.rateType);
		pkt.AddParameter("orderNo"           , stBody.stBody.orderNo);
		pkt.AddParameter("localOrderNo"      , stBody.stBody.localOrderNo);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		pkt.AddParameter("RspCode"           , stBody.stRsp.RspCode);
		pkt.AddParameter("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnDeferMatch] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferMatch
	static int Packet2Struct(PktDeferMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.stBody.matchNo);
		pkt.GetParameterVal("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.GetParameterVal("clientID"          , stBody.stBody.clientID);
		pkt.GetParameterVal("clientType"        , stBody.stBody.clientType);
		pkt.GetParameterVal("instID"            , stBody.stBody.instID);
		pkt.GetParameterVal("memberID"          , stBody.stBody.memberID);
		pkt.GetParameterVal("matchDate"         , stBody.stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.stBody.matchTime);
		pkt.GetParameterVal("price"             , stBody.stBody.price);
		pkt.GetParameterVal("volume"            , stBody.stBody.volume);
		pkt.GetParameterVal("offSetFlag"        , stBody.stBody.offsetFlag);
		pkt.GetParameterVal("orderNo"           , stBody.stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.stBody.localOrderNo);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		pkt.GetParameterVal("RspCode"           , stBody.stRsp.RspCode);
		pkt.GetParameterVal("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferMatch
	static int Struct2Packet(PktDeferMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("matchNo"           , stBody.stBody.matchNo);
		pkt.AddParameter("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.AddParameter("clientID"          , stBody.stBody.clientID);
		pkt.AddParameter("clientType"        , stBody.stBody.clientType);
		pkt.AddParameter("instID"            , stBody.stBody.instID);
		pkt.AddParameter("memberID"          , stBody.stBody.memberID);
		pkt.AddParameter("matchDate"         , stBody.stBody.matchDate);
		pkt.AddParameter("matchTime"         , stBody.stBody.matchTime);
		pkt.AddParameter("price"             , stBody.stBody.price);
		pkt.AddParameter("volume"            , stBody.stBody.volume);
		pkt.AddParameter("offSetFlag"        , stBody.stBody.offsetFlag);
		pkt.AddParameter("orderNo"           , stBody.stBody.orderNo);
		pkt.AddParameter("localOrderNo"      , stBody.stBody.localOrderNo);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		pkt.AddParameter("RspCode"           , stBody.stRsp.RspCode);
		pkt.AddParameter("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};



	//----------------------------------------------报文接口 [onRecvRtnDeferDeliveryAppMatch] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferDeliveryAppMatch
	static int Packet2Struct(PktDeferDeliveryAppMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("matchNo"           , stBody.stBody.matchNo);
		pkt.GetParameterVal("clientID"          , stBody.stBody.clientID);
		pkt.GetParameterVal("instID"            , stBody.stBody.instID);
		pkt.GetParameterVal("memberID"          , stBody.stBody.memberID);
		pkt.GetParameterVal("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.GetParameterVal("volume"            , stBody.stBody.volume);
		pkt.GetParameterVal("matchDate"         , stBody.stBody.matchDate);
		pkt.GetParameterVal("matchTime"         , stBody.stBody.matchTime);
		pkt.GetParameterVal("orderNo"           , stBody.stBody.orderNo);
		pkt.GetParameterVal("localOrderNo"      , stBody.stBody.LocalOrderNo);
		pkt.GetParameterVal("middleFlag"        , stBody.stBody.middleFlag);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		pkt.GetParameterVal("RspCode"           , stBody.stRsp.RspCode);
		pkt.GetParameterVal("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferDeliveryAppMatch
	static int Struct2Packet(PktDeferDeliveryAppMatch& stBody, CBroadcastPacket& pkt)
	{
		pkt.AddParameter("matchNo"           , stBody.stBody.matchNo);
		pkt.AddParameter("clientID"          , stBody.stBody.clientID);
		pkt.AddParameter("instID"            , stBody.stBody.instID);
		pkt.AddParameter("memberID"          , stBody.stBody.memberID);
		pkt.AddParameter("buyOrSell"         , stBody.stBody.buyOrSell);
		pkt.AddParameter("volume"            , stBody.stBody.volume);
		pkt.AddParameter("matchDate"         , stBody.stBody.matchDate);
		pkt.AddParameter("matchTime"         , stBody.stBody.matchTime);
		pkt.AddParameter("orderNo"           , stBody.stBody.orderNo);
		pkt.AddParameter("localOrderNo"      , stBody.stBody.LocalOrderNo);
		pkt.AddParameter("middleFlag"        , stBody.stBody.middleFlag);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		pkt.AddParameter("RspCode"           , stBody.stRsp.RspCode);
		pkt.AddParameter("RspMsg"            , stBody.stRsp.RspMsg);
		return 0;
	};


  //----------------------------------------------报文接口 [onRecvRspSpotOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspSpotOrder
	static int Packet2Struct(RspSpotOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspSpotOrder
	static int Struct2Packet(RspSpotOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  
    
    
  //----------------------------------------------报文接口 [onRecvRtnSpotOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnSpotOrder
	static int Packet2Struct(RtnSpotOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnSpotOrder
	static int Struct2Packet(RtnSpotOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  
	
	//----------------------------------------------报文接口 [onRecvRspSpotOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspSpotOrderCancel
	static int Packet2Struct(RspSpotOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspSpotOrderCancel
	static int Struct2Packet(RspSpotOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  
	
	//----------------------------------------------报文接口 [onRecvRtnSpotOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnSpotOrderCancel
	static int Packet2Struct(RtnSpotOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnSpotOrderCancel
	static int Struct2Packet(RtnSpotOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	
	

	//----------------------------------------------报文接口 [onRecvRspDeferOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspDeferOrder
	static int Packet2Struct(RspDeferOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspDeferOrder
	static int Struct2Packet(RspDeferOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	
		

	//----------------------------------------------报文接口 [onRecvRtnDeferOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferOrder
	static int Packet2Struct(RtnDeferOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferOrder
	static int Struct2Packet(RtnDeferOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRspDeferOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspDeferOrderCancel
	static int Packet2Struct(RspDeferOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspDeferOrderCancel
	static int Struct2Packet(RspDeferOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRtnDeferOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferOrderCancel
	static int Packet2Struct(RtnDeferOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferOrderCancel
	static int Struct2Packet(RtnDeferOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRspDeferDeliveryAppOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspDeferDeliveryAppOrder
	static int Packet2Struct(RspDeferDeliveryAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspDeferDeliveryAppOrder
	static int Struct2Packet(RspDeferDeliveryAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRtnDeferDeliveryAppOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferDeliveryAppOrder
	static int Packet2Struct(RtnDeferDeliveryAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferDeliveryAppOrder
	static int Struct2Packet(RtnDeferDeliveryAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRspDeferDeliveryAppOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspDeferDeliveryAppOrderCancel
	static int Packet2Struct(RspDeferDeliveryAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspDeferDeliveryAppOrderCancel
	static int Struct2Packet(RspDeferDeliveryAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRtnDeferDeliveryAppOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnDeferDeliveryAppOrderCancel
	static int Packet2Struct(RtnDeferDeliveryAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnDeferDeliveryAppOrderCancel
	static int Struct2Packet(RtnDeferDeliveryAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRspMiddleAppOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspMiddleAppOrder
	static int Packet2Struct(RspMiddleAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspMiddleAppOrder
	static int Struct2Packet(RspMiddleAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRtnMiddleAppOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnMiddleAppOrder
	static int Packet2Struct(RtnMiddleAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnMiddleAppOrder
	static int Struct2Packet(RtnMiddleAppOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	



	//----------------------------------------------报文接口 [onRecvRspMiddleAppOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspMiddleAppOrderCancel
	static int Packet2Struct(RspMiddleAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspMiddleAppOrderCancel
	static int Struct2Packet(RspMiddleAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	



	//----------------------------------------------报文接口 [onRecvRtnMiddleAppOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnMiddleAppOrderCancel
	static int Packet2Struct(RtnMiddleAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnMiddleAppOrderCancel
	static int Struct2Packet(RtnMiddleAppOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRtnForwardOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnForwardOrder
	static int Packet2Struct(RtnForwardOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnForwardOrder
	static int Struct2Packet(RtnForwardOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRspForwardOrder] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspForwardOrder
	static int Packet2Struct(RspForwardOrder& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspForwardOrder
	static int Struct2Packet(RspForwardOrder& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRspForwardOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRspForwardOrderCancel
	static int Packet2Struct(RspForwardOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRspForwardOrderCancel
	static int Struct2Packet(RspForwardOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	//----------------------------------------------报文接口 [onRecvRtnForwardOrderCancel] 报文结构相关解析
	// 报文解析：报文 -> 结构    onRecvRtnForwardOrderCancel
	static int Packet2Struct(RtnForwardOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Packet2Struct(stBody.stBody,pkt);
		pkt.GetParameterVal("acctNo"            , stBody.acctNo);
		Packet2Struct(stBody.stRsp,pkt);
		return 0;
	};


	// 报文解析： 结构 -> 报文    onRecvRtnForwardOrderCancel
	static int Struct2Packet(RtnForwardOrderCancel& stBody, CBroadcastPacket& pkt)
	{
		Struct2Packet(stBody.stBody,pkt);
		pkt.AddParameter("acctNo"            , stBody.acctNo);
		Struct2Packet(stBody.stRsp,pkt);		
		return 0;
	};  	


	// 报文解析： 结构 -> 报文    onRecvRtnForwardOrderCancel
	/*static int Packet2Struct(WindVane& stBody, CBroadcastPacket& pkt)
	{
		pkt.GetParameterVal("prod_code"            , stBody.prod_code);
		pkt.GetParameterVal("memo"                 , stBody.memo);
		pkt.GetParameterVal("moreStorPer"          , stBody.moreStorPer);
		pkt.GetParameterVal("openMoreStorAvg"      , stBody.openMoreStorAvg);
		pkt.GetParameterVal("haveMoreSotrAvg"      , stBody.haveMoreSotrAvg);
		pkt.GetParameterVal("haveMoreStorPer"      , stBody.haveMoreStorPer);
		pkt.GetParameterVal("nullStorPer"          , stBody.nullStorPer);
		pkt.GetParameterVal("openNullSotrAvg"      , stBody.openNullSotrAvg);
		pkt.GetParameterVal("haveNullSotrAvg"      , stBody.haveNullSotrAvg);
		pkt.GetParameterVal("haveNullStorPer"      , stBody.haveNullStorPer);
		pkt.GetParameterVal("lookPer"              , stBody.lookPer);

		return 0;
	};  	*/
};
