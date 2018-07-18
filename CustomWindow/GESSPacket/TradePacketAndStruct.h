#pragma once
#include <string>
#include "TradePacket.h"
#include "ArrayListMsg.h"
#include "HashtableMsg.h"

using namespace std;

//----------------------------------------------响应报文基类 定义
// 结构定义:响应报文基类
typedef struct tagRspBase
{
	string       rsp_msg              ;   
	string       rsp_code;
}RspBase,*PRspBase;

//----------------------------------------------Req报文接口 [1001] 定义
// 结构定义:码表获取的请求报文体
typedef struct tagReq1001
{
	int          oper_flag            ; //操作标志    1：查询
	string       code_type            ; //码表类型    
}Req1001,*PReq1001;


//----------------------------------------------Rsp报文接口 [1001] 定义
// 结构定义:码表获取的响应报文体
typedef struct tagRsp1001 : public RspBase
{
	int          oper_flag            ; //操作标志    1：查询
	string       code_type            ; //码表类型    
	string       code_id              ; //代码编号    
	string       code_desc            ; //代码描述    
	HashtableMsg htm_code_list        ; //码表列表    字段为：码表类型、代码编号、代码描述   
}Rsp1001,*PRsp1001;



//----------------------------------------------Req报文接口 [1002] 定义
// 结构定义:合约代码设定
typedef struct tagReq1002
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       prod_code            ; //合约代码    
	string       prod_name            ; //合约名称    
	string       variety_type         ; //品种类别    
	string       variety_id           ; //品种代码    
	double       tick                 ; //最小变动价位    
	double       upper_limit_value    ; //涨停板率    
	double       lower_limit_value    ; //跌停板率    
	string       active_flag          ; //活跃标志    
	string       exch_unit            ; //交易单位    
	double       measure_unit         ; //计量单位    
	string       market_id            ; //交易市场    
}Req1002,*PReq1002;


//----------------------------------------------Rsp报文接口 [1002] 定义
// 结构定义:合约代码设定
typedef struct tagRsp1002 : public RspBase
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       prod_code            ; //合约代码    
	string       prod_name            ; //合约名称    
	string       variety_type         ; //品种类别    
	string       variety_id           ; //品种代码    
	double       tick                 ; //最小变动价位    
	double       upper_limit_value    ; //涨停板率    
	double       lower_limit_value    ; //跌停板率    
	string       active_flag          ; //活跃标志    
	string       exch_unit            ; //交易单位    
	double       measure_unit         ; //计量单位    
	string       market_id            ; //交易市场    
	HashtableMsg htm_prod_code        ; //合约代码列表    字段：2-12
}Rsp1002,*PRsp1002;

//----------------------------------------------Req报文接口 [1004] 定义
// 结构定义:交割品种设定
typedef struct tagReq1004
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       variety_id           ; //交割品种代码    
	string       name                 ; //交割品种全称            
	string       abbr                 ; //简称                    
	string       variety_type         ; //品种类别    
	double       min_pickup           ; //最小提货数量        
	double       pickup_base          ; //提货步长        
	string       weight_unit          ; //重量单位        
	string       destroy_flag         ; //注销标志    
}Req1004,*PReq1004;


//----------------------------------------------Rsp报文接口 [1004] 定义
// 结构定义:交割品种设定
typedef struct tagRsp1004 : public RspBase
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       variety_id           ; //交割品种代码    
	string       name                 ; //交割品种全称            
	string       abbr                 ; //简称                    
	string       variety_type         ; //品种类别    
	double       min_pickup           ; //最小提货数量        
	double       pickup_base          ; //提货步长        
	string       weight_unit          ; //重量单位        
	string       destroy_flag         ; //注销标志    
	HashtableMsg htm_variety_list     ; //交割品种列表    字段为：2-9 
}Rsp1004,*PRsp1004;


//----------------------------------------------Req报文接口 [1005] 定义
// 结构定义:交割替代品种设定
typedef struct tagReq1005
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       prod_code            ; //合约代码    
	double       seq_no               ; //交割顺序    
	string       variety_id           ; //交割品种代码    
	double       diff_amt             ; //差价    
}Req1005,*PReq1005;


//----------------------------------------------Rsp报文接口 [1005] 定义
// 结构定义:交割替代品种设定
typedef struct tagRsp1005 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       prod_code            ; //合约代码    
	double       seq_no               ; //交割顺序    
	string       variety_id           ; //交割品种代码    
	double       diff_amt             ; //差价    
	HashtableMsg htm_variety_info     ; //交割替代品种列表    字段为：合约代码，交割顺序，交割品种代码，差价 
}Rsp1005,*PRsp1005;


//----------------------------------------------Req报文接口 [1006] 定义
// 结构定义:仓库信息维护
typedef struct tagReq1006
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       stor_id              ; //仓库代码    
	string       stor_name            ; //仓库名称    
	string       addr                 ; //仓库地址    
	string       tel                  ; //联系电话    
	string       fax                  ; //联系传真    
	string       link_man             ; //联系人    
	string       zip_code             ; //邮编    
	string       use_variety_type     ; //支持的提货品种    每个品种类型之间使用”,”分隔
	string       is_take              ; //是否允许提货    yes_no
	string       city_code            ; //所属城市    多个城市代码之间使用”,”分隔
}Req1006,*PReq1006;


//----------------------------------------------Rsp报文接口 [1006] 定义
// 结构定义:仓库信息维护
typedef struct tagRsp1006 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       stor_id              ; //仓库代码    
	string       stor_name            ; //仓库名称    
	string       addr                 ; //仓库地址    
	string       tel                  ; //联系电话    
	string       fax                  ; //联系传真    
	string       link_man             ; //联系人    
	string       zip_code             ; //邮编    
	string       use_variety_type     ; //支持的提货品种    每个品种类型之间使用”,”分隔
	string       is_take              ; //是否允许提货    yes_no
	string       city_code            ; //所属城市    多个城市代码之间使用”,”分隔
	HashtableMsg htm_stor_info        ; //仓库信息列表    字段为： 2-11
}Rsp1006,*PRsp1006;



//----------------------------------------------Req报文接口 [1007] 定义
// 结构定义:城市代码维护的请求报文体
typedef struct tagReq1007
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       city_code            ; //城市代码    2*,3*,4*
	string       city_name            ; //城市名称    2*,3*
	string       all_stor_id          ; //仓库代码    2*,3*
	string       stor_id_default      ; //默认提货仓库    2*,3*
}Req1007,*PReq1007;


//----------------------------------------------Rsp报文接口 [1007] 定义
// 结构定义:码表获取的响应报文体
typedef struct tagRsp1007 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询2：增加3：修改4：删除
	string       city_code            ; //城市代码    
	string       city_name            ; //城市名称    
	string       all_stor_id          ; //仓库代码    2*,3*
	string       stor_id_default      ; //默认提货仓库    2*,3*
	HashtableMsg htm_code_list        ; //码表列表    字段为：2-5 
}Rsp1007,*PRsp1007;

//----------------------------------------------Req报文接口 [1020] 定义
// 结构定义:客户综合信息查询
typedef struct tagReq1020
{
	int          oper_flag            ; //操作标志    1：查询
	string       acct_no              ; //客户号    
	string       qry_cust_info        ; //查询客户资料    yes_no
	string       qry_fund             ; //查询资金信息    yes_no
	string       qry_storage          ; //查询库存信息    yes_no
	string       qry_forward          ; //查询远期持仓信息    yes_no
	string       qry_defer            ; //查询延期持仓信息    yes_no
	string       qry_surplus          ; //查询浮动盈亏    yes_no
	string       is_check_stat        ; //是否检查状态    yes_no
}Req1020,*PReq1020;


//----------------------------------------------Rsp报文接口 [1020] 定义
// 结构定义:客户综合信息查询
typedef struct tagRsp1020 : public tagRspBase
{
	int          oper_flag            ; //操作标志    
	string       c_acct_no            ; //客户号    ――――――――――――――――客户资料
	string       c_cust_id            ; //客户代码    
	string       c_account_no         ; //开户行账号    
	string       c_open_bank_name     ; //开户行名称    
	string       c_cust_abbr          ; //客户简称    
	string       c_b_fare_model_id    ; //交易所费用模板ID    
	string       c_m_fare_model_id    ; //会员费用模板ID    
	string       c_acct_type          ; //账户类型    acct_type
	string       c_ocma_flag          ; //一卡多户标志    ocma_flag
	string       c_acct_stat          ; //账户状态    acct_stat
	string       c_broker_acct_no     ; //经纪人账户    
	string       c_cert_type          ; //证件类型    cert_type
	string       c_cert_num           ; //证件号码    
	string       c_branch_id          ; //所属代理机构    
	string       c_trans_check_info   ; //交易验证信息    
	string       f_currency_id        ; //币种    ――――――――――――――――资金情况
	string       f_curr_bal           ; //当前余额    
	string       f_can_use_bal        ; //可用金额    
	string       f_can_get_bal        ; //可提金额    
	string       f_in_bal             ; //当日入金    
	string       f_out_bal            ; //当日出金    
	string       f_buy_bal            ; //买入金额    
	string       f_sell_bal           ; //卖出金额    
	string       f_exch_froz_bal      ; //交易冻结资金    
	string       f_posi_margin        ; //持仓保证金    
	string       f_base_margin        ; //开户保证金    
	string       f_take_margin        ; //提货保证金    
	string       f_stor_margin        ; //仓储费保证金    
	string       f_pt_reserve         ; //铂金冻结资金    
	string       f_ag_margin          ; //白银货款资金    
	string       f_forward_froz       ; //远期浮亏冻结    
	string       f_exch_fare          ; //交易费用    当报单成交后，根据成交价重新计算的交易手续费
	string       s_variety_id         ; //品种代码    ――――――――――――――――库存情况
	string       s_curr_amt           ; //当前库存    
	string       s_can_use_amt        ; //可用库存    
	string       s_can_get_amt        ; //可提库存    
	string       s_day_deposit        ; //当日入库    
	string       s_day_draw           ; //当日出库    
	string       s_real_buy_amt       ; //成交买入    
	string       s_real_sell_amt      ; //成交卖出    
	string       s_entr_sell_amt      ; //报单冻结    
	string       s_app_froz_amt       ; //提货冻结    
	string       s_unit_cost          ; //库存成本    
	string       t5_prod_code         ; //合约代码    ――――――――――――――――远期仓位
	string       t5_long_amt          ; //当前多仓    
	string       t5_short_amt         ; //当前空仓    
	string       t5_can_use_long      ; //可用多仓    
	string       t5_can_use_short     ; //可用空仓    
	string       t5_day_open_long     ; //当日开多仓    
	string       t5_day_open_short    ; //当日开空仓    
	string       td_prod_code         ; //合约代码    ――――――――――――――――延期仓位
	string       td_long_amt          ; //当前多仓    
	string       td_short_amt         ; //当前空仓    
	string       td_can_use_long      ; //可用多仓    
	string       td_can_use_short     ; //可用空仓    
	string       td_day_open_long     ; //当日开多仓    
	string       td_day_open_short    ; //当日开空仓    
	string       td_day_cov_long      ; //当日平多仓    
	string       td_day_cov_short     ; //当日平空仓    
	string       td_day_deli_long     ; //当日交割多仓    
	string       td_day_deli_short    ; //当日交割空仓    
	string       td_day_cov_long_froz ; //当日平多仓冻结    
	string       td_day_cov_short_froz ; //当日平空仓冻结    
	string       td_day_deli_long_forz ; //当日交割多仓冻结    
	string       td_day_deli_short_forz ; //当日交割空仓冻结    
	string       td_long_open_avg_price ; //多头开仓均价    
	string       td_short_open_avg_price ; //空头开仓均价    
	string       td_long_posi_avg_price ; //多头持仓均价    
	string       td_short_posi_avg_price ; //空头持仓均价    
	string       td_long_margin       ; //多头持仓保证金    
	string       td_short_margin      ; //空头持仓保证金    
	string       td_last_settle_price ; //上日结算价    
	string       td_day_settle_price  ; //当日结算价    
	string       r_surplus            ; //浮动盈亏    ――――――――――――――――盈亏查询
	string       r_risk_index         ; //风险系数    
	string       r_risk_level         ; //风险级别    
	HashtableMsg hlm_stor_info        ; //库存信息列表    字段：29 － 38
	HashtableMsg htm_t5_info          ; //远期持仓信息列表    字段：39 － 45
	HashtableMsg htm_td_info          ; //延期持仓信息列表    字段：46 － 60

	HashtableMsg htm_prod_group_info;     //大边组合信息
	HashtableMsg htm_acct_large_side_sum; //大边保证金信息   

	void CopyCusInfo(const tagRsp1020& Other)
	{
		this->c_acct_no = Other.c_acct_no;
		this->c_account_no = Other.c_account_no;
		this->c_open_bank_name = Other.c_open_bank_name;
		this->c_cust_abbr = Other.c_cust_abbr;
		this->c_acct_stat = Other.c_acct_stat;
		this->c_branch_id = Other.c_branch_id;
	}

	void CopyFundInfo(const tagRsp1020& Other)
	{
		this->f_curr_bal = Other.f_curr_bal;
		this->f_can_use_bal = Other.f_can_use_bal;
		this->f_can_get_bal = Other.f_can_get_bal;
		this->f_exch_froz_bal = Other.f_exch_froz_bal;
		this->f_posi_margin = Other.f_posi_margin;
		this->f_base_margin = Other.f_base_margin;
		this->f_take_margin = Other.f_take_margin;
		this->f_stor_margin = Other.f_stor_margin;
		this->f_pt_reserve = Other.f_pt_reserve;
		this->f_ag_margin = Other.f_ag_margin;
	}

	void CopySurplus(const tagRsp1020& Other)
	{
		this->r_surplus = Other.r_surplus;
	}

}Rsp1020,*PRsp1020;

//----------------------------------------------Req报文接口 [2022] 定义
// 结构定义:账户密码修改的请求报文体
typedef struct tagReq2022
{
	int          oper_flag            ; //操作标志    1：密码修改2：交易密码重置3：资金密码重置
	string       acct_no              ; //客户号    
	string       acct_type            ; //账户类型    
	string       old_exch_pwd         ; //原交易密码    
	string       exch_pwd             ; //新交易密码    
	string       old_fund_pwd         ; //原资金密码    
	string       fund_pwd             ; //新资金密码    
}Req2022,*PReq2022;


//----------------------------------------------Rsp报文接口 [2022] 定义
// 结构定义:账户密码修改的响应报文体
typedef struct tagRsp2022 : public RspBase
{
	int          oper_flag            ; //操作标志    1：密码修改2：交易密码重置3：资金密码重置
}Rsp2022,*PRsp2022;

//----------------------------------------------Req报文接口 [2040] 定义
// 结构定义:客户实际费率查询的请求报文体
typedef struct tagReq2040
{
	int          oper_flag            ; //操作标志    1. 查询
	string       acct_no              ; //客户号    
}Req2040,*PReq2040;


//----------------------------------------------Rsp报文接口 [2040] 定义
// 结构定义:客户实际费率查询的响应报文体
typedef struct tagRsp2040 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1. 查询
	ArrayListMsg alm_b_fare_list      ; //交易所费用列表    字段为：合约代码，合约代码名称，费用类型，费用类型名称，收费模式，收费模式名称，费率值
	ArrayListMsg alm_m_fare_list      ; //会员费用列表    字段为：合约代码，合约代码名称，费用类型，费用类型名称，收费模式，收费模式名称，费率值,费率来源
}Rsp2040,*PRsp2040;

//----------------------------------------------Req报文接口 [2201] 定义
// 结构定义:日结单确认的请求报文体
typedef struct tagReq2201
{
	int          oper_flag            ; //操作标志    1：查询2：日结单确认
	string       acct_no              ; //客户号    1*,2*
	string       affirm_date          ; //确认日期    2*
}Req2201,*PReq2201;


//----------------------------------------------Rsp报文接口 [2201] 定义
// 结构定义:日结单确认的响应报文体
typedef struct tagRsp2201 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询2：日结单确认
	string       affirm_date          ; //确认日期    1*
}Rsp2201,*PRsp2201;

//----------------------------------------------Req报文接口 [3021] 定义
// 结构定义:自动出入金转账的请求报文体
typedef struct tagReq3010
{
	int          oper_flag            ; //操作标志    1：查询2：转账3：复核通过4：复核不通过
	string       serial_no            ; //交易流水号    3*,4*
	string       acct_no              ; //客户号    1,2*
	string       access_way           ; //存取标志    1*,2* access_way
	double       exch_bal             ; //发生金额    2*
	string       remark               ; //备注     2
	string       cust_id              ; //客户代码    1
	string       cust_abbr            ; //客户简称    1
	string       branch_id            ; //代理机构    1
	string       c_status             ; //复核状态    2*
	string       o_teller_id          ; //申请操作员    2*
	string       account_no           ; //银行账号
	string       open_bank_name       ; //银行名称    1
	string       fund_pwd             ; //资金密码    3*,4*
}Req3010,*PReq3010;

//----------------------------------------------Rsp报文接口 [3021] 定义
// 结构定义:自动出入金转账的响应报文体
typedef struct tagRsp3010 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询2：转账 3：复核通过4：复核不通过
	string       serial_no              ; //
	string       acct_no              ; //
	string       cust_id              ; //
	string       cust_abbr              ; //
	string       acct_stat              ; //
	string       exec_name              ; //
	string       exec_cert_num              ; //
	double       curr_bal              ; //
	double       can_get_cash              ; //
	double       can_use_cash              ; //        (废弃)字段为：交易流水号,客户号,客户简称,代理机构、银行帐号,银行名称，存取方向,发生金额,复核状态，复核人，复核日期,发送状态,是否已入账,备注, 
	ArrayListMsg alm_custtype_list    ; //当日资金流水列表   字段为：流水号，客户号，客户简称，账户状态，交易标志，发生金额，银行账号，开户行名称，复核状态，申请人，申请日期，复核人，复核日期，备注		 
}Rsp3010,*PRsp3010;

//----------------------------------------------Req报文接口 [3021] 定义
// 结构定义:自动出入金转账的请求报文体
typedef struct tagReq3021
{
	int          oper_flag            ; //操作标志    1：查询2：转账3：复核通过4：复核不通过
	string       acct_no              ; //客户号    1,2*
	string       access_way           ; //存取标志    1*,2* access_way
	double       exch_bal             ; //发生金额    2*
	string       fund_pwd             ; //资金密码    2*
	string       remark               ; //备注     2
	string       cust_id              ; //客户代码    1
	string       cust_abbr            ; //客户简称    1
	string       cust_type            ; //客户类别    1
	string       branch_id            ; //代理机构    1

	// 由于这个报文中有这个字段，所以修改了，但不知道对其他环境有没有影响，要观察
	string       card_pwd            ; //银行卡密码
	//string       serial_no            ; //交易流水号
}Req3021,*PReq3021;

//----------------------------------------------Rsp报文接口 [3021] 定义
// 结构定义:自动出入金转账的响应报文体
typedef struct tagRsp3021 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询2：转账 3：复核通过4：复核不通过
	ArrayListMsg alm_custtype_list    ; //当日资金流水列表    字段为：交易流水号,客户号,客户简称,代理机构、银行帐号,银行名称，存取方向,发生金额,复核状态，复核人，复核日期,发送状态,是否已入账,备注, 
}Rsp3021,*PRsp3021;

typedef struct tagRep3024
{
	///<Sammary>
	/// 操作标志  1：开通		 
	///</Sammary>
	int oper_flag;

	///<Sammary>
	/// 交易编码  1*		 
	///</Sammary>
	string acct_no;

	///<Sammary>
	/// 银行代码  1*		 
	///</Sammary>
	string bank_no;

	///<Sammary>
	/// 商户号  1*		 
	///</Sammary>
	string bk_mer_code;

	///<Sammary>
	/// 银行交易编码  		 
	///</Sammary>
	string bk_cust_no;

	///<Sammary>
	/// 银行客户名称  		 
	///</Sammary>
	string bk_acct_name;

	///<Sammary>
	/// 银行账号  1*		 
	///</Sammary>
	string bk_acct_no;

	///<Sammary>
	/// 银行证件类别  1*		 
	///</Sammary>
	string bk_cert_type;

	///<Sammary>
	/// 银行证件号码  1*		 
	///</Sammary>
	string bk_cert_no;

	///<Sammary>
	/// 是否短信通知  yes_no		 
	///</Sammary>
	string sms_flag;

	///<Sammary>
	/// 手机号码  可选(如果短信通知，手机号码必输)		 
	///</Sammary>
	string phone_num;

	///<Sammary>
	/// 备注  可选		 
	///</Sammary>
	string remark;
}Rep3024,*PRep3024;

typedef struct tagRsp3024 : public tagRspBase
{
	///<Sammary>
	/// 操作标志  1：查询 2：开通 3：取消 4：修改		 
	///</Sammary>
	int oper_flag;

	///<Sammary>
	/// 客户号  		 
	///</Sammary>
	string acct_no;

	///<Sammary>
	/// 银行代码  		 
	///</Sammary>
	string bank_no;

	///<Sammary>
	/// 商户号  		 
	///</Sammary>
	string bk_mer_code;

	///<Sammary>
	/// 银行客户号  		 
	///</Sammary>
	string bk_cust_no;

	///<Sammary>
	/// 银行客户名称  		 
	///</Sammary>
	string bk_acct_name;

	///<Sammary>
	/// 银行账号  		 
	///</Sammary>
	string bk_acct_no;

	///<Sammary>
	/// 银行证件类别  		 
	///</Sammary>
	string bk_cert_type;

	///<Sammary>
	/// 银行证件号码  		 
	///</Sammary>
	string bk_cert_no;

	///<Sammary>
	/// 是否短信通知  		 
	///</Sammary>
	string sms_flag;

	///<Sammary>
	/// 手机号码  		 
	///</Sammary>
	string phone_num;

	///<Sammary>
	/// 备注  		 
	///</Sammary>
	string remark;
}Rsp3024,*PRsp3024;

//----------------------------------------------Req报文接口 [3064] 定义      风险提示查询请求的请求报文体 
typedef struct tagReq3064
{
	int oper_flag;                 // 操作标志    1：查询
	string branch_id;              // 代理机构编号
	string begin_time;             // 开始时间
	string end_time;               // 结束时间
	string acct_no;                // 交易编码
	string tip_type;               // 提示类型    tip_type1提示2追保通知3强平通知
}Req3064,*PReq3064;
//结构定义：风险提示查询请求的响应报文体 
typedef struct tagRsp3064 : public tagRspBase
{
	int oper_flag;                  // 操作标志    1：查询
	ArrayListMsg alm_result;        // 查询结果    字段：时间、源风险等级、目标风险等级、交易编码、客户简称、客户级别、账户类型、风险度1、风险度2、追保金额、通知标题、通知内容、代理商、代理商简称
}Rsp3064,*PRsp3064;

//结构定义：强平单查询的请求报文体 
typedef struct tagReq3069
{
	int oper_flag;                 // 操作标志    1：查询
	string acct_no;                // 交易编码
	string instid;                 // 合约代码    
	string gene_type;              // 生成方式
	string begin_date;             // 开始日期  指委托日期
	string end_date;               // 结束日期  指委托日期
	ArrayListMsg order_status_set; // 报单状态 

}Req3069,*PReq3069;
//结构定义：强平单查询的响应报文体 
typedef struct tagRsp3069 : public tagRspBase
{
	int oper_flag;                  // 操作标志    1：查询
	ArrayListMsg order_status_set;  //查询结果    字段顺序：交易编码,客户简称,客户级别,账户类型,合约代码,买卖方向,委托价格,委托数量,强平单号,报单状态,报单号,成交均价,成交数量,强平操作员,撤单操作员,委托时间,撤销时间,代理商编号,代理商简称,批次号,当时可用资金(风控),当时浮动盈亏,当时风险度1,当时风险度2,当时会员保证金,当时交易所保证金,当时总保证金,当时和约价格

}Rsp3069,*PRsp3069;

//----------------------------------------------Req报文接口 [3099] 定义      客户风险度查询的请求报文体
typedef struct tagReq3099
{
	int oper_flag;                 // 操作标志    1：查询
}Req3099,*PReq3099;
/*
结构定义：客户风险度查询的响应报文体 ，去掉了risk_degree1和risk_degree2
根据金联通测试要求，恢复risk_degree1以及risk_degree2，因为金联通采用的是risk_degree2，且暂时去掉risk_degree 
*/
typedef struct tagRsp3099 : public tagRspBase
{
	int oper_flag;                  // 操作标志    1：查询
	string risk_degree1;            // 风险度
	string risk_degree2;            // 风险度
	string risk_grade;              // 风险等级
}Rsp3099,*PRsp3099;

//----------------------------------------------Req报文接口 [3101] 定义
// 结构定义:保证金划拨的请求报文体
typedef struct tagReq3101
{
	int          oper_flag            ; //操作标志    1：银行转账流水查询 2：封闭保证金流水查询3：划转申请4：再次发送 5：划转查询6：封闭保证金补帐查询
	string       serial_no            ; //交易流水号    3*
	string       acct_no              ; //客户号    2*，1
	string       access_way           ; //存取标志    2*，1
	double       exch_bal             ; //发生金额    2*
	string       remark               ; //备注    
	string       fund_pwd             ; //资金密码    2*
	string       branch_id            ; //代理机构    1
	string       grade_id             ; //客户级别    1
	string       o_term_type          ; //申请来源    1 ，term_type 
	string       send_stat            ; //发送状态    1，b_send_stat 
}Req3101,*PReq3101;


//----------------------------------------------Rsp报文接口 [3101] 定义
// 结构定义:保证金划拨的响应报文体
typedef struct tagRsp3101 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：银行转账流水查询 2：封闭保证金流水查询3：划转申请4：再次发送 5：划转查询6：封闭保证金补帐查询
	string       Exch_date            ; //交易日期    
	string       Serial_no            ; //交易流水号    
	string       Acct_no              ; //客户号    
	string       bank_no              ; //银行代码    
	string       f_busi_type          ; //业务类型    
	string       access_way           ; //存取方向    
	string       out_account_no       ; //出金银行账号    
	string       in_account_no        ; //入金银行账号    
	string       Exch_bal             ; //发生金额    
	string       check_stat1          ; //复核状态1    
	string       check_stat2          ; //复核状态2    
	string       send_stat            ; //发送状态    
	string       in_account_flag      ; //是否已入账    
	string       remark               ; //备注    
	string       bk_plat_date         ; //银行日期    
	string       bk_seq_no            ; //银行流水号    
	string       bk_rsp_code          ; //银行响应代码    
	string       bk_rsp_msg           ; //银行响应消息    
	string       o_term_type          ; //创建来源    
	string       o_teller_id          ; //创建用户号    
	string       o_date               ; //创建日期    
	string       c_term_type1         ; //复核1来源    
	string       c_teller_id1         ; //复核1用户号    
	string       c_date1              ; //复核1日期    
	string       c_term_type2         ; //复核2来源    
	string       c_teller_id2         ; //复核2用户号    
	string       c_date2              ; //复核2日期 
	HashtableMsg htm_result           ; //查询结果    字段：2-29
}Rsp3101,*PRsp3101;

//----------------------------------------------Req报文接口 [3201] 定义
// 结构定义:查询客户卡余额请求报文
typedef struct tagReq3201
{
	int          oper_flag            ; // 操作标识 1:查询
	string       acct_no              ; // 客户号
}Req3201,*PReq3201;

typedef struct tagRsp3201 : public RspBase
{
	int    oper_flag;          // 操作标志    1：查询
	string card_bal;           // 卡余额
}Rsp3201,*PRsp3201;

//----------------------------------------------Req报文接口 [4001] 定义
// 结构定义:交易统一请求报文
typedef struct tagReq4001
{
	string       oper_flag            ;//add by xiao
	string       client_serial_no     ; //客户端流水号    
	string       acct_no              ; //客户号    
	string       cust_id              ; //客户代码    
	string       bank_no              ; //银行卡号    
	string       b_market_id          ; //交易市场    b_market_id
	string       prod_code            ; //合约代码    
	string       offset_flag          ; //开平标志    b_offset_flag
	string       deli_flag            ; //交收标志    b_deli_flag
	string       bs                   ; //交易方向    b_buyorsell
	double       entr_price           ; //委托价格    
	int          entr_amount          ; //委托数量    
	string       cov_type             ; //平仓方式    cov_type
	string       match_type           ; //成交类型    b_match_type
	string       src_match_no         ; //仓位编号    指定平仓的原成交编号
	string       order_send_type      ; //报单发送方式    order_send_type
	string       auto_send_time_s     ; //自动发送开始时间    格式：HH24:MI:SS
	string       auto_send_time_e     ; //自动发送结束时间    格式：HH24:MI:SS
	string       auto_send_stat_s     ; //源发送状态    B_INST_STAT
	string       auto_send_stat_t     ; //目标发送状态    B_INST_STAT
	string       auto_send_way        ; //自动发送方向    ORDER_SEND_WAY
	string       auto_send_comp       ; //自动发送关系    ORDER_SEND_COMP
	string       auto_send_price      ; //自动发送价格    
	string       order_type           ; //报单类型
}Req4001,*PReq4001;

//----------------------------------------------Rsp报文接口 [4001] 定义
// 结构定义:交易统一请求报文
typedef struct tagRsp4001 : public tagRspBase
{
	string       client_serial_no     ; //客户端流水号    
	string       local_order_no       ; //本地报单号    
}Rsp4001,*PRsp4001;


//----------------------------------------------Req报文接口 [4061] 定义
// 结构定义:交易撤单
typedef struct tagReq4061
{
	int          oper_flag            ; //操作标志    1：交易撤单 
	string       cancel_order_no      ; //被撤单报单号    
}Req4061,*PReq4061;

//----------------------------------------------Rsp报文接口 [4061] 定义
// 结构定义:交易撤单
typedef struct tagRsp4061 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：交易撤单 
}Rsp4061,*PRsp4061;


//----------------------------------------------Req报文接口 [4071] 定义
// 结构定义:平仓试算的请求报文
typedef struct tagReq4071
{
	int          oper_flag            ; //操作标志    1：平仓试算 
	string       acct_no              ; //客户号    
	string       prod_code            ; //合约代码    
	string       cov_bs               ; //平仓方向    b:平空仓s:平多仓
	string       cov_price            ; //平仓价格    
	int          cov_amount           ; //平仓数量    
}Req4071,*PReq4071;

//----------------------------------------------Rsp报文接口 [4071] 定义
// 结构定义:平仓试算的响应报文
typedef struct tagRsp4071 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：平仓试算 
	string       acct_no              ; //客户号    
	string       cust_abbr            ; //客户简称    
	string       prod_code            ; //合约代码    
	string       remain_long          ; //多仓剩余数量    
	string       remain_short         ; //空仓剩余数量    
	string       cov_exch_fare        ; //平仓手续费    
	string       cov_surplus          ; //平仓盈亏    
	string       can_use_bal          ; //可用金额   
}Rsp4071,*PRsp4071;


//----------------------------------------------Req报文接口 [5041] 定义
// 结构定义:机构提货日维护的请求报文体
typedef struct tagReq5041
{
	int          oper_flag            ; //操作标志    1：查询2：修改
	string       year                 ; //年份选择    
	string       branch_id            ; //代理机构    1*
	string       is_search_usefull    ; //是否查询当前交易日之后的设置    yes_no
	string       search_num           ; //查询最近多少条记录    
	ArrayListMsg alm_take_sheet_date  ; //提货日列表    字段为：交易日期
}Req5041,*PReq5041;

//----------------------------------------------Rsp报文接口 [5041] 定义
// 结构定义:机构提货日维护的响应报文体
typedef struct tagRsp5041 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询2：修改
	string       year                 ; //年份选择    
	string       branch_id            ; //代理机构   
	ArrayListMsg alm_take_sheet_date  ; //提货日列表    字段为：交易日期
}Rsp5041,*PRsp5041;


//----------------------------------------------Req报文接口 [5101] 定义
// 结构定义:带接口的提货申请流水维护的请求报文体
typedef struct tagReq5101
{
	int          oper_flag            ; //操作标志    1. 查询2. 提货申请3. 提货申请撤消4：异地提货确认
	string       local_sheet_no       ; //本地提货单编号    
	string       sheet_no             ; //提货单编号    
	string       acct_no              ; //客户号    
	string       cust_id              ; //客户代码    
	string       variety_id           ; //交割品种代码    
	string       app_amount           ; //提货标准重量    
	string       city_code            ; //提货城市    
	string       stor_id              ; //仓库代码    
	string       take_man             ; //提货人    
	string       cert_type_id         ; //提货人证件类型    
	string       cert_num             ; //提货人证件号码    
	string       app_pwd              ; //提货密码    
	string       app_record           ; //申请录入人    
	string       take_date            ; //预备提货日    
	string       start_date           ; //申请交易起始日期    8位日期表示，查询用
	string       end_date             ; //申请交易结束日期    8位日期表示，查询用
}Req5101,*PReq5101;


//----------------------------------------------Rsp报文接口 [5101] 定义
// 结构定义:提货申请流水查询的响应报文体
typedef struct tagRsp5101 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1. 查询2. 提货申请3. 提货申请撤消
	string       local_sheet_no       ; //本地提货单编号    
	string       sheet_no             ; //提货单编号    
	string       acct_no              ; //客户号    
	string       cust_id              ; //客户代码    
	string       variety_id           ; //交割品种代码    
	string       app_amount           ; //提货标准重量    
	string       draw_type            ; //提货类型    
	string       self_draw            ; //自提重量    
	string       trade_draw           ; //交提重量    
	string       city_code            ; //提货城市    
	string       stor_id              ; //仓库代码    
	string       take_man             ; //提货人    
	string       cert_type_id         ; //提货人证件类型    
	string       cert_num             ; //提货人证件号码    
	string       app_record           ; //申请录入人    
	string       app_date             ; //申请物理日期    
	string       app_time             ; //申请时间    
	string       app_exch_date        ; //申请交易日期    
	string       app_start_date       ; //申请提货开始日期    
	string       app_end_date         ; //申请提货截止日期    
	string       due_take_date        ; //预备提货日    
	string       stat                 ; //提货状态    b_sheet_stat
	string       cancel_date          ; //撤消物理日期    
	string       cancel_time          ; //撤消时间    
	string       cancel_exch_date     ; //撤消交易日期    
	string       cancel_record        ; //撤消录入人    
	string       send_stat            ; //发送状态    
	string       take_margin          ; //提货保证金    
	string       is_other_take        ; //是否异地提货    yes_no
	string       take_affirm          ; //异地提货确认    yes_no      
	HashtableMsg htm_take_sheet_detail ; //提货申请流水列表    字段：2--31
}Rsp5101,*PRsp5101;

//----------------------------------------------Req报文接口 [5103] 定义
// 结构定义:提货单打印请求报文
typedef struct tagReq5103
{
	int          oper_flag            ; //操作标志    1. 查询
	string       local_sheet_no       ; //本地提货单编号    
	string       sheet_no             ; //提货单编号    
	string       acct_no              ; //客户号    
	string       cust_id              ; //客户代码    
	string       start_date           ; //申请交易起始日期    8位日期表示，查询用
	string       end_date             ; //申请交易结束日期    8位日期表示，查询用
}Req5103,*PReq5103;


//----------------------------------------------Rsp报文接口 [5103] 定义
// 结构定义:提货单打印响应报文
typedef struct tagRsp5103 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1. 查询2.异地提货确认
	ArrayListMsg alm_take_sheet_detail ; //提货申请流水列表    本地提货单编号,提货单编号,客户号,客户代码,客户名称,会员代码,会员名称,代理会员代码,交割品种代码,提货标准重量,提货类型,自提重量,交提重量,提货城市,仓库代码,仓库名称,仓库地址,仓库联系人,仓库联系电话,提货人,提货人证件类型,提货人证件号码,申请交易日期,申请提货开始日期,申请提货截止日期,提货状态,发送状态,是否异地提货,异地提货确认,响应代码,响应信息
}Rsp5103,*PRsp5103;

//----------------------------------------------Req报文接口 [6001] 定义
// 结构定义:查询条件配置查询
typedef struct tagReq6001
{
	int          oper_flag            ; //操作标志    1：查询
	string       query_id             ; //查询标识    
}Req6001,*PReq6001;


//----------------------------------------------Rsp报文接口 [6001] 定义
// 结构定义:查询条件配置查询的响应报文体
typedef struct tagRsp6001 : public RspBase
{
	int          oper_flag            ; //操作标志    1：查询
	string       query_id             ; //查询标识    
	string       query_name           ; //查询名称    
	int          cell_num             ; //每行列数    
	int          cell_inv_width       ; //查询条件左右间距    
	int          cell_inv_height      ; //查询条件上下间距    
	ArrayListMsg alm_options          ; //查询条件    条件标识|默认值|控件类名|宽度比率|^
	ArrayListMsg alm_field_en_name    ; //查询结果英文字段名称    
	ArrayListMsg alm_field_cn_name    ; //查询结果中文字段名称    
	ArrayListMsg alm_field_width      ; //查询结果列宽度    
	ArrayListMsg alm_field_align      ; //查询结果列对齐方式    1：左对齐2：居中 3：右对齐
	ArrayListMsg alm_field_format     ; //查询结果格式化    
}Rsp6001,*PRsp6001;



//----------------------------------------------Req报文接口 [6002] 定义
// 结构定义:普通查询
typedef struct tagReq6002
{
	int          oper_flag            ; //操作标志    1：查询
	string       login_branch_id      ; //登录者代理机构号    
	string       login_teller_id      ; //登录者操作员号    
	int          paginal_num          ; //每页记录数    
	int          curr_page            ; //当前查询页    
	string       query_id             ; //查询标识    
	ArrayListMsg alm_view_field       ; //显示字段    当前查询需要显示的字段
}Req6002,*PReq6002;


//----------------------------------------------Rsp报文接口 [6002] 定义
// 结构定义:普通查询
typedef struct tagRsp6002 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询
	int          paginal_num          ; //每页记录数    
	int          page_count           ; //结果总页数    
	int          curr_page            ; //当前查询页  
	ArrayListMsg alm_result           ; //查询结果    
}Rsp6002,*PRsp6002;

//----------------------------------------------Req报文接口 [6005] 定义
// 结构定义:报表条件配置查询的请求报文体
typedef struct tagReq6005
{
	int          oper_flag            ; //操作标志    1：查询
	string       report_id            ; //报表标识    1*
	string       c_rdlc_ver           ; //客户端格式文件版本号    1*
}Req6005,*PReq6005;


//----------------------------------------------Rsp报文接口 [6005] 定义
// 结构定义:报表条件配置查询的响应报文体
typedef struct tagRsp6005 : public RspBase
{
	int          oper_flag            ; //操作标志    1：查询
	string       report_id            ; //报表标识    
	string       report_name          ; //报表名称    
	int          cell_num             ; //每行列数    
	int          cell_inv_width       ; //报表条件左右间距    
	int          cell_inv_height      ; //报表条件上下间距    
	string       s_rdlc_ver           ; //服务器格式文件版本号    
	//string       rsp_m;
	ArrayListMsg alm_data_source      ; //数据源列表    字段顺序：数据源名称
	ArrayListMsg alm_options          ; //报表条件    字段顺序：条件标识,默认值,控件类名,宽度比率
}Rsp6005,*PRsp6005;

//----------------------------------------------Req报文接口 [6007] 定义
// 结构定义:数据源下载的请求报文体
typedef struct tagReq6007
{
	int          oper_flag            ; //操作标志    1：数据源下载
	string       exch_date            ; //Add by xiaorensong
	ArrayListMsg alm_ds_list          ; //数据源列表    
}Req6007,*PReq6007;


//----------------------------------------------Rsp报文接口 [6007] 定义
// 结构定义:数据源下载的响应报文体
typedef struct tagRsp6007 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：数据源下载
	HashtableMsg htm_Title   ;//
	HashtableMsg htm_AcctSettleRisk   ;//
	HashtableMsg htm_AcctSettleFund   ;//
	HashtableMsg htm_AcctSettleFundDetail   ;//
	HashtableMsg htm_AcctSettleStorage   ;//
	HashtableMsg htm_AcctSettleForward   ;//
	HashtableMsg htm_AcctSettleDefer   ;//
	HashtableMsg htm_AcctSettleSurplus   ;//
	HashtableMsg htm_AcctSettleMatchFlow   ;//
	HashtableMsg htm_AcctSettleFundInOutFlow   ;//
}Rsp6007,*PRsp6007;

//----------------------------------------------Req报文接口 [8001] 定义
// 结构定义:用户登录
typedef struct tagReq8001
{
	int          oper_flag            ; //操作标志    1：用户登录
	string       user_type            ; //用户类型    user_type
	string       user_id_type         ; //用户ID类型    当登录用户为客户时，此值有效。1：客户号2：银行账号
	string       user_id              ; //用户ID    
	string       user_pwd             ; //用户密码    
	string       login_ip             ; //登录IP    
	string       ver_num              ; //客户端版本号    
	string       is_down_para         ; //是否下载参数信息    如果客户端的版本号与服务器端的最新版本号不一致，强制下载。响应报文的字段20开始
	string       bank_no              ; //银行代码    当客户使用“银行账号”登录时，此值必填
	string       login_server_code    ; //服务器代码    客户当前处理业务使用的服务器代码

	string       isSign               ; // 是否签名
	string       CN                   ; // 签名文件CN码 issign为是时,必填
	string		 session_id			  ;//add lqh 2014.3.10
}Req8001,*PReq8001;


//----------------------------------------------Rsp报文接口 [8001] 定义
// 结构定义:用户登录响应报文体
typedef struct tagRsp8001 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：用户登录
	string       check_code           ; //校验码    
	string       member_id            ; //会员编号    客户登录时有值
	string       member_name          ; //会员名称    客户登录时有值
	string       branch_id            ; //所属代理机构编号    
	string       branch_name          ; //所属代理机构名称    
	string       acct_type            ; //账户类型    客户登录时有值
	string       user_id              ; //用户ID    
	string       user_name            ; //用户名称    
	string       sys_date             ; //系统日期    
	string       exch_date            ; //交易日期    
	string       exch_time            ; //交易时间    
	string       last_exch_date       ; //上个交易日期    
	string       version              ; //服务器版本号    
	string       m_sys_stat           ; //二级系统状态    M_SYS_STAT
	string       b_sys_stat           ; //交易所系统状态    B_SYS_STAT
	string       first_login          ; //是否首次登录    yes_no
	string       need_change_pwd      ; //是否必须修改密码    yes_no
	string       last_login_date      ; //上次登录日期    
	string       last_login_time      ; //上次登录时间    
	string       last_lan_ip          ; //上次登录本地IP    
	string       last_www_ip          ; //上次登录公网IP    暂时无值
	string       exist_login          ; //是否已登录    
	int          no_affirm_rpt_num    ; //日结单未确认天数    客户登录时有值
	string       comp_stat            ; //客户端版本兼容情况    1：登录客户端为最新版，不必更新；2：系统有最新的客户端版本，兼容目前登录客户端，可暂缓更新，3：系统有最新的客户端版本，不兼营目前登录客户端，必须更新。
	string auto_entry_menu_id         ;
	string		more_login_msg		;		//用于显示多点登录历史登录记录
	ArrayListMsg alm_menu_info        ; //有权操作的菜单    
	ArrayListMsg alm_cust_type_id     ; //所属客户分组    客户登录时有值字段：客户分组ID
	ArrayListMsg alm_take_man_list    ; //提货人列表    字段：提货人姓名，证件类型，证件号码
	ArrayListMsg alm_role_list        ; //操所员所属角色列表    字段：角色ID
	ArrayListMsg alm_code_table_list  ; //码表列表    字段：码表名称，码表ID，码表描述
	ArrayListMsg alm_prod_code_list   ; //合约代码列表    字段：合约代码，合约名称，品种类别，交割品种代码，最小变动价位，涨停板率，跌停板率，活跃标志，交易单位，计量单位，交易市场
	ArrayListMsg alm_variety_list     ; //交割品种列表    字段：交割品种代码，交割品种全称，简称，品种类别，最小提货数量，提货步长，重量单位，注销标志
	ArrayListMsg alm_city_code_list   ; //城市代码列表    字段：城市代码，城市名称，拥有提货仓库，默认提货仓库
	ArrayListMsg alm_fare_type_list   ; //费用类型列表    字段：费用类型ID，费用类型分类，费用类型描述，适用的合约代码，
	ArrayListMsg alm_query_list       ; //查询条件列表    字段见表可下说明
	ArrayListMsg alm_report_list      ; //报表条件列表    字段见表格下说明
	ArrayListMsg alm_noread_bulletin_list ; //未读公告列表    字段：公告序号、公告标题
}Rsp8001,*PRsp8001;



//----------------------------------------------Req报文接口 [8002] 定义
// 结构定义:用户退出
typedef struct tagReq8002
{
	int          oper_flag            ; //操作标志    1：用户退出
	string       user_type            ; //用户类型    user_type
	string       user_id              ; //用户ID    
	string       session_id           ; //会话ID    
}Req8002,*PReq8002;


//----------------------------------------------Rsp报文接口 [8002] 定义
// 结构定义:用户登录响应报文体
typedef struct tagRsp8002 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：用户退出
}Rsp8002,*PRsp8002;

//----------------------------------------------Req报文接口 [8006] 定义
// 结构定义:登录认证
typedef struct tagReq8006
{
	int          oper_flag            ; //操作标志    1：登录认证
	string       user_type            ; //用户类型    user_type
	string       user_id_type         ; //用户ID类型    当登录用户为客户时，此值有效。1：客户号2：银行账号
	string       user_id              ; //用户ID    
	string       user_pwd             ; //用户密码    
	string       login_ip             ; //登录IP    
	string       bank_no              ; //银行代码    当客户使用“银行账号”登录时，此值必填
	string       net_envionment       ; //网络环境    net_envionment
	string       net_agent            ; //网络运营商    net_agent
	string       isSign               ; // add for 金联通
}Req8006,*PReq8006;


//----------------------------------------------Rsp报文接口 [8006] 定义
// 结构定义:登录认证
typedef struct tagRsp8006 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：登录认证
	string       session_id           ; //会话ID    
	string       session_key          ; //会话密钥    
	string       server_code          ; //服务器编码    
	string       server_name          ; //服务器名称    
	string       trans_ip             ; //交易服务地址    
	int          trans_port           ; //交易服务端口    
	string       query_ip             ; //查询服务地址    
	int          query_port           ; //查询服务端口    
	string       broadcast_ip         ; //资讯服务地址    
	int          broadcast_port       ; //资讯服务端口    
	string       risk_trans_ip        ; //风控交易服务地址    
	int          risk_trans_port      ; //风控交易服务端口    
	string       risk_broadcast_ip    ; //风控广播服务地址    
	int          risk_broadcast_port  ; //风控广播服务端口    
	HashtableMsg htm_server_list      ; //服务器组列表    字段：4-15
	ArrayListMsg htm_more_login_list;	//多点登录内容 ,lqh 2014-3-6 10:45:36 for GF
}Rsp8006,*PRsp8006;

//----------------------------------------------Req报文接口[8007] 定义
// 结构定义:屏幕解锁
typedef struct tagReq8007
{
	int          oper_flag            ; //操作标志    1：登录认证
	string       user_type            ; //用户类型    user_type
	string       user_id_type         ; //用户ID类型    当登录用户为客户时，此值有效。1：客户号2：银行账号
	string       user_id              ; //用户ID    
	string       user_pwd             ; //用户密码    
	string       login_ip             ; //登录IP    
	string       bank_no              ; //银行代码    当客户使用“银行账号”登录时，此值必填
	string       isSign               ; // add for 金联通
}Req8007,*PReq8007;

//----------------------------------------------Rsp报文接口 [8007] 定义
// 结构定义:屏幕解锁
typedef struct tagRsp8007 : public RspBase
{
	int          oper_flag            ; //操作标志    1：解锁验密
	string       succ                 ; //验密成功    yes_no
}Rsp8007,*PRsp8007;


//----------------------------------------------Req报文接口 [8030] 定义
// 结构定义:交易所公告查询的请求报文体
typedef struct tagReq8030
{
	int          oper_flag            ; //操作标志    1：查询 
	string       seq_no               ; //公告序号    
	string       title                ; //公告标题    可模糊匹配
	string       s_exch_date          ; //起始公告日期    
	string       e_exch_date          ; //结束公告日期    
}Req8030,*PReq8030;

//----------------------------------------------Rsp报文接口 [8030] 定义
// 结构定义:交易所公告查询的响应报文体
typedef struct tagRsp8030 : public RspBase
{
	int          oper_flag            ; //操作标志    1：查询 
	ArrayListMsg alm_result           ; //查询结果    字段顺序：序号、标题、内容、发布人、公告日期、公告时间
}Rsp8030,*PRsp8030;


//----------------------------------------------Req报文接口 [8031] 定义
// 结构定义:会员公告查询的请求报文体
typedef struct tagReq8031
{
	int          oper_flag            ; //操作标志    1：查询 
	string       bulletin_id          ; //公告序号    
	string       title                ; //公告标题    可模糊匹配
	string       s_exch_date          ; //起始公告日期    
	string       e_exch_date          ; //结束公告日期    
}Req8031,*PReq8031;


//----------------------------------------------Rsp报文接口 [8031] 定义
// 结构定义:会员公告查询的响应报文体
typedef struct tagRsp8031 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询 
	ArrayListMsg alm_result           ; //查询结果    字段顺序：序号、标题、内容、发布人、发布时间
}Rsp8031,*PRsp8031;

//----------------------------------------------Req报文接口 [9010] 定义
// 结构定义:代理机构查询的请求报文体
typedef struct tagReq9010
{
	int          oper_flag            ; //操作标志    1：查询下级2：查询上级
	string       branch_id            ; //代理机构编号    
	string       is_contain_self      ; //是否包括本身    
	string       bank_no              ; //银行代码    
}Req9010,*PReq9010;


//----------------------------------------------Rsp报文接口 [9010] 定义
// 结构定义:代理机构查询的响应报文体
typedef struct tagRsp9010 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询下级2：查询上级
	string       branch_id            ; //代理机构编号    
	string       branch_name          ; //代理机构名称    
	string       branch_level         ; //代理机构级别    
	string       parent_branch_id     ; //上级代理机构    
	string       bank_no              ; //银行代码    
	string       city_code            ; //所属城市    
	string       is_self_bank         ; //是否本行机构   
	HashtableMsg htm_branch_info      ; //代理机构列表    字段： 2-8
}Rsp9010,*PRsp9010;


//----------------------------------------------Req报文接口 [9030] 定义
// 结构定义:系统参数设置请求报文体
typedef struct tagReq4074
{
	int          oper_flag            ; //操作标志    1：查询 2：增加 3：修改 4：删除 
	string       start_date              ; //开始日期  
	string       end_date            ; //结束日期    
	string       acct_no            ; //客户号    
	string       delegate_type           ; //委托类型   
	string       src_match_no           ; // 原成交流水号
	string       trigger_status           ; // 触发状态
	string       serial_no           ; // 托管流水号
	string       exch_type           ; // 交易代码
	string       prod_code           ; // 合约代码
	double       entr_price           ; // 委托价格
	int       entr_amount           ; // 委托数量
	string       entr_price_type           ; // 报单价类型
	string       trigger_condition           ; // 触发条件
	string       trigger_value_type           ; // 触发值类型
	string       trigger_value           ; // 触发值
	int       base_adjust_step           ; // 基准价调整价位
	int       entr_adjust_step           ; // 报单价调整价位

	tagReq4074()
	{
		base_adjust_step = 0;
		entr_adjust_step = 0;
		entr_price = 0.00;
	}
}Req4074,*PReq4074;

typedef struct tagRsp4074 : public RspBase
{
	int          oper_flag            ; //操作标志    1：查询 2：增加 3：修改 4：删除
	ArrayListMsg       alm_result              ; /*字段：
												 托管流水号(0),交易日期(1),客户号(2),委托类型(3),交易代码(4),合约代码(5),
												 委托价格(6),委托数量(7),触发类型(8),触发条件(9),触发值(10),
												 基准价调整价位(11),报单价调整价位(12),触发状态(13),触发时间(14),委托来源(15),
												 委托操作员(16),委托时间(17),修改来源(18),修改操作员(19),最后修改时间(20),
												 撤消来源(21),撤消操作员(22),撤消时间(23)   
												 */
}Rsp4074,*PRsp4074;


//----------------------------------------------Req报文接口 [9030] 定义
// 结构定义:系统参数设置请求报文体
typedef struct tagReq9030
{
	int          oper_flag            ; //操作标志    1：查询 2：增加 3：修改 4：删除 
	string       para_id              ; //参数ID    
	string       para_type            ; //参数类型    
	string       para_desc            ; //参数描述    
	string       para_value           ; //参数值    
}Req9030,*PReq9030;


//----------------------------------------------Rsp报文接口 [9030] 定义
// 结构定义:系统参数设置响应报文体
typedef struct tagRsp9030 : public tagRspBase
{
	int          oper_flag            ; //操作标志    1：查询 2：增加 3：修改 4：删除
	string       para_id              ; //参数ID    
	string       para_type            ; //参数类型    
	string       para_desc            ; //参数描述    
	string       para_value           ; //参数值    
	HashtableMsg htm_para_info        ; //交易参数列表    字段为：参数ID，参数类型，参数描述，参数值
}Rsp9030,*PRsp9030;

#ifdef RUN_EMULATOR
// 
typedef struct tagReq3999
{
	int          oper_flag            ; // 操作标志  1：查询余额 2：查询流水 
	string       acct_no              ; // 客户号   
	string       trans_type           ; // 业务类型  0：出金 1：入金 4：折现    
	string       in_flag              ; // 入帐标志  0：未入帐 1：已入帐    
	string       start_date           ; // 开始日期    
	string       end_date             ; // 结束日期   
}Req3999,*PReq3999;
typedef struct tagRsp3999 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：查询余额 2：查询流水
	string       balance              ; // 余额 
	ArrayListMsg alm_flow             ; // 流水信息  字段为：银行日期，银行流水号，客户编码，银行账号，业务类型，发生金额，是否已入帐，备注，交易日期，交易流水号
}Rsp3999,*PRsp3999;

// 
typedef struct tagReq5999
{
	int          oper_flag            ; // 操作标志  1：查询 2：折现  
	string       acct_no              ; // 客户号   
	string       variety_id           ; // 交割品种    
	string       amount               ; // 重量   
}Req5999,*PReq5999;
typedef struct tagRsp5999 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：查询 2：折现
	ArrayListMsg alm_flow             ; // 流水信息  字段为：客户号，交割品种，重量
}Rsp5999,*PRsp5999;

// 资料修改的请求报文体
typedef struct tagReq2997
{
	int          oper_flag            ; // 操作标志  1：资料修改 2：查询客户信息  
	string       acct_no              ; // 客户号   
	string       cust_name            ; // 客户名称    
	string       fax                  ; // 传真号码 
	string       zipcode              ; // 邮政编码 
	string       address              ; // 联系地址 
	string       email                ; // 邮箱 
	string       qq                   ; // QQ 
	string       msn                  ; // MSN 
}Req2997,*PReq2997;
typedef struct tagRsp2997 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：资料修改 2：查询客户信息
	string       cust_name            ; // 客户名称
	string       fax                  ; // 传真号码
	string       zipcode              ; // 邮政编码
	string       address              ; // 联系地址
	string       email                ; // 邮箱
	string       qq                   ; // QQ
	string       msn                  ; // MSN
}Rsp2997,*PRsp2997;

// 手机号变更的请求报文体
typedef struct tagReq2996
{
	int          oper_flag            ; // 操作标志  1：查询客户问题 2：手机号变更
	string       acct_no              ; // 客户号   
	string       question1            ; // 问题1    
	string       answer1              ; // 答案1 
	string       question2            ; // 问题2 
	string       answer2              ; // 答案2 
	string       new_mobile           ; // 新手机号 
}Req2996,*PReq2996;
// 手机号变更的响应报文体
typedef struct tagRsp2996 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：查询客户问题 2：手机号变更
	string       question_id1         ; // 问题1编号
	string       question_content1    ; // 问题1内容
	string       question_id2         ; // 问题2编号
	string       question_content2    ; // 问题2内容
}Rsp2996,*PRsp2996;

// 所属机构变更的请求报文体
typedef struct tagReq2995
{
	int          oper_flag            ; // 操作标志  1：查询 2：所属机构变更
	string       acct_no              ; // 客户号   
	string       question1            ; // 问题1    
	string       answer1              ; // 答案1 
	string       question2            ; // 问题2 
	string       answer2              ; // 答案2 
	string       new_branch_id        ; // 新所属机构 
}Req2995,*PReq2995;
// 手机号变更的响应报文体
typedef struct tagRsp2995 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：查询客户问题 2：手机号变更
	string       question_id1         ; // 问题1编号
	string       question_content1    ; // 问题1内容
	string       question_id2         ; // 问题2编号
	string       question_content2    ; // 问题2内容
	ArrayListMsg alm_branch_info      ; // 代理机构列表  字段为：代理机构编号，代理机构名称，代理机构类型
}Rsp2995,*PRsp2995;

// 恢复初始状态的请求报文体
typedef struct tagReq2994
{
	int          oper_flag            ; // 操作标志  1：恢复初始状态
	string       acct_no              ; // 客户号   
}Req2994,*PReq2994;
typedef struct tagRsp2994 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：恢复初始状态
}Rsp2994,*PRsp2994;

// 身份认证的请求报文体
typedef struct tagReq8999
{
	int          oper_flag            ; // 操作标志  1：查询客户问题 2：查询客户信息 3：确认
	string       acct_no              ; // 客户号   
	string       question1            ; // 问题1    
	string       answer1              ; // 答案1 
	string       question2            ; // 问题2 
	string       answer2              ; // 答案2 
	string       cert_type_id         ; // 证件类型
	string       cert_num             ; // 证件号码
	string       cust_name            ; // 客户名称
	string       addr                 ; // 通讯地址
	string       zip_code             ; // 邮编
}Req8999,*PReq8999;
typedef struct tagRsp8999 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：查询客户问题 2：查询客户信息 3：确认
 
	string       question_id1         ; // 问题1编号
	string       question_content1    ; // 问题1内容
	string       question_id2         ; // 问题2编号
	string       question_content2    ; // 问题2内容

	string       cert_type_id         ; // 证件类型
	string       cert_num             ; // 证件号码
	string       cust_name            ; // 客户名称
	string       addr                 ; // 通讯地址
	string       zipcode             ; // 邮编
}Rsp8999,*PRsp8999;

// 大赛报名的请求报文体
typedef struct tagReq8998
{
	int          oper_flag            ; // 操作标志  1：恢复初始状态
	string       acct_no              ; // 客户号   
	string       match_no             ; // 大赛编号
}Req8998,*PReq8998;
typedef struct tagRsp8998 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：恢复初始状态
	ArrayListMsg alm_match_info       ; // 大赛信息列表  字段为：大赛编号，大赛简称，大赛全称，举办代理机构，初始资金，获奖人数，获奖说明，报名开始日期，报名结束日期，大赛开始日期，大赛结束日期，最小交易金额，最小交易次数，大赛状态，备注
}Rsp8998,*PRsp8998;

// 大赛退出的请求报文体
typedef struct tagReq8997
{
	int          oper_flag            ; // 操作标志  1：恢复初始状态
	string       acct_no              ; // 客户号   
	string       match_no             ; // 大赛编号
}Req8997,*PReq8997;
typedef struct tagRsp8997 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：恢复初始状态
	string       match_no             ; // 大赛编号
	string       match_abbr           ; // 大赛简称
	string       match_name           ; // 大赛全称
	string       branch_id            ; // 举办代理机构
	string       init_fund            ; // 初始资金
	string       win_peoples          ; // 获奖人数
	string       win_show             ; // 获奖说明
	string       start_join_date      ; // 报名开始日期
	string       end_join_date        ; // 报名结束日期
	string       start_match_date     ; // 大赛开始日期
	string       end_match_date       ; // 大赛结束日期
	string       min_exch_bal         ; // 最小交易金额
	string       min_exch_count       ; // 最小交易次数
	string       stat                 ; // 大赛状态
	string       memo                 ; // 备注
}Rsp8997,*PRsp8997;

// 代理机构大赛查询的请求报文体
typedef struct tagReq8897
{
	int          oper_flag            ; // 操作标志  1：查询
	string       branch_id            ; // 代理机构   
}Req8897,*PReq8897;
typedef struct tagRsp8897 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：查询
	ArrayListMsg alm_match_info       ; // 大赛信息列表  字段为：大赛编号，大赛简称
}Rsp8897,*PRsp8897;

// 密码重置的请求报文体
typedef struct tagReq2998
{
	int          oper_flag            ; // 操作标志  1：查询客户问题 2：交易密码重置 3：资金密码重置		
	string       question1            ; // 问题1    
	string       answer1              ; // 答案1 
	string       question2            ; // 问题2 
	string       answer2              ; // 答案2 
	string       mobile_phone         ; // 手机号 
	string       new_password         ; // 新密码   
}Req2998,*PReq2998;
typedef struct tagRsp2998 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：查询客户问题 2：交易密码重置 3：资金密码重置	
	string       question_id1         ; // 问题1编号
	string       question_content1    ; // 问题1内容
	string       question_id2         ; // 问题2编号
	string       question_content2    ; // 问题2内容
}Rsp2998,*PRsp2998;

// 新客户注册的请求报文体
typedef struct tagReq2999
{
	int          oper_flag            ; // 操作标志  1：注册前查询 2：新客户注册 3：获取手机验证码		
	string       acct_no              ; // 客户编码
	string       cust_name            ; // 客户名称
	string       sex                  ; // 客户性别  1：男 2：女
	string       mobile_phone         ; // 手机号
	string       verify_code          ; // 手机验证码
	string       account_no           ; // 银行卡号
	string       cert_type_id         ; // 证件类型
	string       cert_num             ; // 证件号码
	string       branch_id            ; // 代理机构
	string       zipcode              ; // 邮政编码
	string       addr                 ; // 联系地址
	string       email                ; // 邮箱
	string       qq                   ; // QQ
	string       msn                  ; // MSN
	string       fax                  ; // 传真
	string       exch_pwd             ; // 交易密码
	string       fund_pwd             ; // 资金密码
	string       question1            ; // 问题1
	string       answer1              ; // 答案1
	string       question2            ; // 问题2
	string       answer2              ; // 答案2
	string       question3            ; // 问题3
	string       answer3              ; // 答案3
	string       question4            ; // 问题4
	string       answer4              ; // 答案4
	string       question5            ; // 问题5
	string       answer5              ; // 答案5
	string       bank_no              ; // 银行代码
}Req2999,*PReq2999;
typedef struct tagRsp2999 : public tagRspBase
{
	int          oper_flag            ; // 操作标志  1：注册前查询 2：新客户注册 3：获取手机验证码		
	string       acct_no              ; // 客户编码
	string       is_verify            ; // 是否需要手机验证
	ArrayListMsg    alm_branch_info   ; // 代理机构列表  字段为：代理机构编号，代理机构名称，代理机构类型
	ArrayListMsg    alm_question      ; // 问题列表  字段为：问题编号，问题内容
}Rsp2999,*PRsp2999;

#endif

class CTradePacketAndStruct
{
public:

#ifdef RUN_EMULATOR
	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req3999& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("trans_type"       , stBody.trans_type);
		pkt.AddParameter("in_flag"          , stBody.in_flag);
		pkt.AddParameter("start_date"       , stBody.start_date);
		pkt.AddParameter("end_date"         , stBody.end_date);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp3999& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"       , stBody.oper_flag);
		pkt.GetParameterVal("balance"         , stBody.balance);
		pkt.GetParameterVal("alm_flow"        , stBody.alm_flow);

		return 0;
	};


	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req5999& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("variety_id"       , stBody.variety_id);
		pkt.AddParameter("amount"           , stBody.amount);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp5999& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"       , stBody.oper_flag);
		pkt.GetParameterVal("alm_flow"         , stBody.alm_flow);
		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req2997& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("cust_name"        , stBody.cust_name);
		pkt.AddParameter("fax"              , stBody.fax);
		pkt.AddParameter("zipcode"          , stBody.zipcode);
		pkt.AddParameter("address"          , stBody.address);
		pkt.AddParameter("email"            , stBody.email);
		pkt.AddParameter("qq"               , stBody.qq);
		pkt.AddParameter("msn"              , stBody.msn);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp2997& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"       , stBody.oper_flag);
		pkt.GetParameterVal("cust_name"       , stBody.cust_name);
		pkt.GetParameterVal("fax"             , stBody.fax);
		pkt.GetParameterVal("zipcode"         , stBody.zipcode);
		pkt.GetParameterVal("address"         , stBody.address);
		pkt.GetParameterVal("email"           , stBody.email);
		pkt.GetParameterVal("qq"              , stBody.qq);
		pkt.GetParameterVal("msn"             , stBody.msn);

		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req2996& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("question1"        , stBody.question1);
		pkt.AddParameter("answer1"          , stBody.answer1);
		pkt.AddParameter("question2"        , stBody.question2);
		pkt.AddParameter("answer2"          , stBody.answer2);
		pkt.AddParameter("new_mobile"       , stBody.new_mobile);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp2996& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		pkt.GetParameterVal("question_id1"       , stBody.question_id1);
		pkt.GetParameterVal("question_content1"  , stBody.question_content1);
		pkt.GetParameterVal("question_id2"       , stBody.question_id2);
		pkt.GetParameterVal("question_content2"  , stBody.question_content2);

		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req2995& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("question1"        , stBody.question1);
		pkt.AddParameter("answer1"          , stBody.answer1);
		pkt.AddParameter("question2"        , stBody.question2);
		pkt.AddParameter("answer2"          , stBody.answer2);
		pkt.AddParameter("new_branch_id"    , stBody.new_branch_id);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp2995& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		pkt.GetParameterVal("question_id1"       , stBody.question_id1);
		pkt.GetParameterVal("question_content1"  , stBody.question_content1);
		pkt.GetParameterVal("question_id2"       , stBody.question_id2);
		pkt.GetParameterVal("question_content2"  , stBody.question_content2);
		pkt.GetParameterVal("alm_branch_info"    , stBody.alm_branch_info);

		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req2994& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp2994& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req8999& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("question1"        , stBody.question1);
		pkt.AddParameter("answer1"          , stBody.answer1);
		pkt.AddParameter("question2"        , stBody.question2);
		pkt.AddParameter("answer2"          , stBody.answer2);
		pkt.AddParameter("cert_type_id"     , stBody.cert_type_id);
		pkt.AddParameter("cert_num"         , stBody.cert_num);
		pkt.AddParameter("cust_name"        , stBody.cust_name);
		pkt.AddParameter("addr"             , stBody.addr);
		pkt.AddParameter("zip_code"         , stBody.zip_code);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp8999& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"           , stBody.oper_flag);
		pkt.GetParameterVal("question_id1"        , stBody.question_id1);
		pkt.GetParameterVal("question_content1"   , stBody.question_content1);
		pkt.GetParameterVal("question_id2"        , stBody.question_id2);
		pkt.GetParameterVal("question_content2"   , stBody.question_content2);
		pkt.GetParameterVal("cert_type_id"        , stBody.cert_type_id);
		pkt.GetParameterVal("cert_num"            , stBody.cert_num);
		pkt.GetParameterVal("cust_name"           , stBody.cust_name);
		pkt.GetParameterVal("addr"                , stBody.addr);
		pkt.GetParameterVal("zipcode"             , stBody.zipcode);

		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req8998& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("match_no"          , stBody.match_no);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp8998& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		pkt.GetParameterVal("alm_match_info"     , stBody.alm_match_info);
		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req8997& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("match_no"          , stBody.match_no);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp8997& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		pkt.GetParameterVal("match_no"           , stBody.match_no);
		pkt.GetParameterVal("match_abbr"         , stBody.match_abbr);
		pkt.GetParameterVal("match_name"         , stBody.match_name);
		pkt.GetParameterVal("branch_id"          , stBody.branch_id);
		pkt.GetParameterVal("init_fund"          , stBody.init_fund);
		pkt.GetParameterVal("win_peoples"        , stBody.win_peoples);
		pkt.GetParameterVal("win_show"           , stBody.win_show);
		pkt.GetParameterVal("start_join_date"    , stBody.start_join_date);
		pkt.GetParameterVal("end_join_date"      , stBody.end_join_date);
		pkt.GetParameterVal("start_match_date"   , stBody.start_match_date);
		pkt.GetParameterVal("end_match_date"     , stBody.end_match_date);
		pkt.GetParameterVal("min_exch_bal"       , stBody.min_exch_bal);
		pkt.GetParameterVal("min_exch_count"     , stBody.min_exch_count);
		pkt.GetParameterVal("stat"               , stBody.stat);
		pkt.GetParameterVal("memo"               , stBody.memo);

		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req8897& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("branch_id"          , stBody.branch_id);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp8897& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		pkt.GetParameterVal("alm_match_info"     , stBody.alm_match_info);
		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req2998& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("question1"        , stBody.question1);
		pkt.AddParameter("answer1"          , stBody.answer1);
		pkt.AddParameter("question2"        , stBody.question2);
		pkt.AddParameter("answer2"          , stBody.answer2);
		pkt.AddParameter("mobile_phone"       , stBody.mobile_phone);
		pkt.AddParameter("new_password"          , stBody.new_password);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp2998& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		pkt.GetParameterVal("question_id1"       , stBody.question_id1);
		pkt.GetParameterVal("question_content1"  , stBody.question_content1);
		pkt.GetParameterVal("question_id2"       , stBody.question_id2);
		pkt.GetParameterVal("question_content2"  , stBody.question_content2);

		Packet2StructBase(stBody, pkt);

		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req2999& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"        , stBody.oper_flag);
		pkt.AddParameter("acct_no"          , stBody.acct_no);
		pkt.AddParameter("cust_name"        , stBody.cust_name);
		pkt.AddParameter("sex"              , stBody.sex);
		pkt.AddParameter("mobile_phone"     , stBody.mobile_phone);
		pkt.AddParameter("verify_code"      , stBody.verify_code);
		pkt.AddParameter("account_no"       , stBody.account_no);
		pkt.AddParameter("cert_type_id"     , stBody.cert_type_id);
		pkt.AddParameter("cert_num"         , stBody.cert_num);
		pkt.AddParameter("branch_id"        , stBody.branch_id);
		pkt.AddParameter("zipcode"          , stBody.zipcode);
		pkt.AddParameter("addr"             , stBody.addr);
		pkt.AddParameter("email"            , stBody.email);
		pkt.AddParameter("qq"               , stBody.qq);
		pkt.AddParameter("msn"              , stBody.msn);
		pkt.AddParameter("fax"              , stBody.fax);
		pkt.AddParameter("exch_pwd"         , stBody.exch_pwd);
		pkt.AddParameter("fund_pwd"         , stBody.fund_pwd);
		pkt.AddParameter("question1"        , stBody.question1);
		pkt.AddParameter("answer1"          , stBody.answer1);
		pkt.AddParameter("question2"        , stBody.question2);
		pkt.AddParameter("answer2"          , stBody.answer2);
		pkt.AddParameter("question3"        , stBody.question3);
		pkt.AddParameter("answer3"          , stBody.answer3);
		pkt.AddParameter("question4"        , stBody.question4);
		pkt.AddParameter("answer4"          , stBody.answer4);
		pkt.AddParameter("question5"        , stBody.question5);
		pkt.AddParameter("answer5"          , stBody.answer5);
		pkt.AddParameter("bank_no"          , stBody.bank_no);

		return 0;
	};
	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp2999& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"          , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"       , stBody.acct_no);
		pkt.GetParameterVal("is_verify"  , stBody.is_verify);
		pkt.GetParameterVal("alm_branch_info"       , stBody.alm_branch_info);
		pkt.GetParameterVal("alm_question"  , stBody.alm_question);

		Packet2StructBase(stBody, pkt);

		return 0;
	};


#endif

	// 报文解析：报文 -> 结构 
	static int Packet2Struct(RspBase& stBody, CTradePacket& pkt)
	{
		return 0;
	};

	// 报文解析：报文 -> 结构 
	static int Packet2StructBase(RspBase& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("rsp_msg"         , stBody.rsp_msg);

		return 0;
	};


	// 报文解析：报文 -> 结构    码表获取的请求报文体
	static int Packet2Struct(Req1001& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("code_type"         , stBody.code_type);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req1001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("code_type"         , stBody.code_type);
		return 0;
	};

	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp1001& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("code_type"         , stBody.code_type);
		pkt.GetParameterVal("code_id"           , stBody.code_id);
		pkt.GetParameterVal("code_desc"         , stBody.code_desc);
		pkt.GetParameterVal("htm_code_list"     , stBody.htm_code_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的响应报文体
	static int Struct2Packet(Rsp1001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("code_type"         , stBody.code_type);
		pkt.AddParameter("code_id"           , stBody.code_id);
		pkt.AddParameter("code_desc"         , stBody.code_desc);
		pkt.AddParameter("htm_code_list"     , stBody.htm_code_list);
		return 0;
	};

	// 报文解析：报文 -> 结构    合约代码设定
	static int Packet2Struct(Req1002& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("prod_code"         , stBody.prod_code);
		pkt.GetParameterVal("prod_name"         , stBody.prod_name);
		pkt.GetParameterVal("variety_type"      , stBody.variety_type);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("tick"              , stBody.tick);
		pkt.GetParameterVal("upper_limit_value" , stBody.upper_limit_value);
		pkt.GetParameterVal("lower_limit_value" , stBody.lower_limit_value);
		pkt.GetParameterVal("active_flag"       , stBody.active_flag);
		pkt.GetParameterVal("exch_unit"         , stBody.exch_unit);
		pkt.GetParameterVal("measure_unit"      , stBody.measure_unit);
		pkt.GetParameterVal("market_id"         , stBody.market_id);
		return 0;
	};

	// 报文解析： 结构 -> 报文    合约代码设定
	static int Struct2Packet(Req1002& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("prod_name"         , stBody.prod_name);
		pkt.AddParameter("variety_type"      , stBody.variety_type);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("tick"              , stBody.tick);
		pkt.AddParameter("upper_limit_value" , stBody.upper_limit_value);
		pkt.AddParameter("lower_limit_value" , stBody.lower_limit_value);
		pkt.AddParameter("active_flag"       , stBody.active_flag);
		pkt.AddParameter("exch_unit"         , stBody.exch_unit);
		pkt.AddParameter("measure_unit"      , stBody.measure_unit);
		pkt.AddParameter("market_id"         , stBody.market_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    合约代码设定
	static int Packet2Struct(Rsp1002& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("prod_code"         , stBody.prod_code);
		pkt.GetParameterVal("prod_name"         , stBody.prod_name);
		pkt.GetParameterVal("variety_type"      , stBody.variety_type);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("tick"              , stBody.tick);
		pkt.GetParameterVal("upper_limit_value" , stBody.upper_limit_value);
		pkt.GetParameterVal("lower_limit_value" , stBody.lower_limit_value);
		pkt.GetParameterVal("active_flag"       , stBody.active_flag);
		pkt.GetParameterVal("exch_unit"         , stBody.exch_unit);
		pkt.GetParameterVal("measure_unit"      , stBody.measure_unit);
		pkt.GetParameterVal("market_id"         , stBody.market_id);
		pkt.GetParameterVal("htm_prod_code"     , stBody.htm_prod_code);
		return 0;
	};

	// 报文解析： 结构 -> 报文    合约代码设定
	static int Struct2Packet(Rsp1002& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("prod_name"         , stBody.prod_name);
		pkt.AddParameter("variety_type"      , stBody.variety_type);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("tick"              , stBody.tick);
		pkt.AddParameter("upper_limit_value" , stBody.upper_limit_value);
		pkt.AddParameter("lower_limit_value" , stBody.lower_limit_value);
		pkt.AddParameter("active_flag"       , stBody.active_flag);
		pkt.AddParameter("exch_unit"         , stBody.exch_unit);
		pkt.AddParameter("measure_unit"      , stBody.measure_unit);
		pkt.AddParameter("market_id"         , stBody.market_id);
		pkt.AddParameter("htm_prod_code"     , stBody.htm_prod_code);
		return 0;
	};



	// 报文解析：报文 -> 结构    交割品种设定
	static int Packet2Struct(Req1004& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("name"              , stBody.name);
		pkt.GetParameterVal("abbr"              , stBody.abbr);
		pkt.GetParameterVal("variety_type"      , stBody.variety_type);
		pkt.GetParameterVal("min_pickup"        , stBody.min_pickup);
		pkt.GetParameterVal("pickup_base"       , stBody.pickup_base);
		pkt.GetParameterVal("weight_unit"       , stBody.weight_unit);
		pkt.GetParameterVal("destroy_flag"      , stBody.destroy_flag);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交割品种设定
	static int Struct2Packet(Req1004& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("name"              , stBody.name);
		pkt.AddParameter("abbr"              , stBody.abbr);
		pkt.AddParameter("variety_type"      , stBody.variety_type);
		pkt.AddParameter("min_pickup"        , stBody.min_pickup);
		pkt.AddParameter("pickup_base"       , stBody.pickup_base);
		pkt.AddParameter("weight_unit"       , stBody.weight_unit);
		pkt.AddParameter("destroy_flag"      , stBody.destroy_flag);
		return 0;
	};

	// 报文解析：报文 -> 结构    交割品种设定
	static int Packet2Struct(Rsp1004& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("name"              , stBody.name);
		pkt.GetParameterVal("abbr"              , stBody.abbr);
		pkt.GetParameterVal("variety_type"      , stBody.variety_type);
		pkt.GetParameterVal("min_pickup"        , stBody.min_pickup);
		pkt.GetParameterVal("pickup_base"       , stBody.pickup_base);
		pkt.GetParameterVal("weight_unit"       , stBody.weight_unit);
		pkt.GetParameterVal("destroy_flag"      , stBody.destroy_flag);
		pkt.GetParameterVal("htm_variety_list"  , stBody.htm_variety_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交割品种设定
	static int Struct2Packet(Rsp1004& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("name"              , stBody.name);
		pkt.AddParameter("abbr"              , stBody.abbr);
		pkt.AddParameter("variety_type"      , stBody.variety_type);
		pkt.AddParameter("min_pickup"        , stBody.min_pickup);
		pkt.AddParameter("pickup_base"       , stBody.pickup_base);
		pkt.AddParameter("weight_unit"       , stBody.weight_unit);
		pkt.AddParameter("destroy_flag"      , stBody.destroy_flag);
		pkt.AddParameter("htm_variety_list"  , stBody.htm_variety_list);
		return 0;
	};



	// 报文解析：报文 -> 结构    交割替代品种设定
	static int Packet2Struct(Req1005& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("prod_code"         , stBody.prod_code);
		pkt.GetParameterVal("seq_no"            , stBody.seq_no);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("diff_amt"          , stBody.diff_amt);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交割替代品种设定
	static int Struct2Packet(Req1005& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("seq_no"            , stBody.seq_no);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("diff_amt"          , stBody.diff_amt);
		return 0;
	};

	// 报文解析：报文 -> 结构    交割替代品种设定
	static int Packet2Struct(Rsp1005& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("prod_code"         , stBody.prod_code);
		pkt.GetParameterVal("seq_no"            , stBody.seq_no);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("diff_amt"          , stBody.diff_amt);
		pkt.GetParameterVal("htm_variety_info"  , stBody.htm_variety_info);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交割替代品种设定
	static int Struct2Packet(Rsp1005& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("seq_no"            , stBody.seq_no);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("diff_amt"          , stBody.diff_amt);
		pkt.AddParameter("htm_variety_info"  , stBody.htm_variety_info);
		return 0;
	};



	// 报文解析：报文 -> 结构    仓库信息维护
	static int Packet2Struct(Req1006& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("stor_id  "         , stBody.stor_id  );
		pkt.GetParameterVal("stor_name"         , stBody.stor_name);
		pkt.GetParameterVal("addr     "         , stBody.addr     );
		pkt.GetParameterVal("tel      "         , stBody.tel      );
		pkt.GetParameterVal("fax      "         , stBody.fax      );
		pkt.GetParameterVal("link_man "         , stBody.link_man );
		pkt.GetParameterVal("zip_code "         , stBody.zip_code );
		pkt.GetParameterVal("use_variety_type"  , stBody.use_variety_type);
		pkt.GetParameterVal("is_take"           , stBody.is_take);
		pkt.GetParameterVal("city_code"         , stBody.city_code);
		return 0;
	};

	// 报文解析： 结构 -> 报文    仓库信息维护
	static int Struct2Packet(Req1006& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("stor_id  "         , stBody.stor_id  );
		pkt.AddParameter("stor_name"         , stBody.stor_name);
		pkt.AddParameter("addr     "         , stBody.addr     );
		pkt.AddParameter("tel      "         , stBody.tel      );
		pkt.AddParameter("fax      "         , stBody.fax      );
		pkt.AddParameter("link_man "         , stBody.link_man );
		pkt.AddParameter("zip_code "         , stBody.zip_code );
		pkt.AddParameter("use_variety_type"  , stBody.use_variety_type);
		pkt.AddParameter("is_take"           , stBody.is_take);
		pkt.AddParameter("city_code"         , stBody.city_code);
		return 0;
	};

	// 报文解析：报文 -> 结构    仓库信息维护
	static int Packet2Struct(Rsp1006& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("stor_id  "         , stBody.stor_id  );
		pkt.GetParameterVal("stor_name"         , stBody.stor_name);
		pkt.GetParameterVal("addr     "         , stBody.addr     );
		pkt.GetParameterVal("tel      "         , stBody.tel      );
		pkt.GetParameterVal("fax      "         , stBody.fax      );
		pkt.GetParameterVal("link_man "         , stBody.link_man );
		pkt.GetParameterVal("zip_code "         , stBody.zip_code );
		pkt.GetParameterVal("use_variety_type"  , stBody.use_variety_type);
		pkt.GetParameterVal("is_take"           , stBody.is_take);
		pkt.GetParameterVal("city_code"         , stBody.city_code);
		pkt.GetParameterVal("htm_stor_info"     , stBody.htm_stor_info);
		return 0;
	};

	// 报文解析： 结构 -> 报文    仓库信息维护
	static int Struct2Packet(Rsp1006& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("stor_id  "         , stBody.stor_id  );
		pkt.AddParameter("stor_name"         , stBody.stor_name);
		pkt.AddParameter("addr     "         , stBody.addr     );
		pkt.AddParameter("tel      "         , stBody.tel      );
		pkt.AddParameter("fax      "         , stBody.fax      );
		pkt.AddParameter("link_man "         , stBody.link_man );
		pkt.AddParameter("zip_code "         , stBody.zip_code );
		pkt.AddParameter("use_variety_type"  , stBody.use_variety_type);
		pkt.AddParameter("is_take"           , stBody.is_take);
		pkt.AddParameter("city_code"         , stBody.city_code);
		pkt.AddParameter("htm_stor_info"     , stBody.htm_stor_info);
		return 0;
	};



	// 报文解析：报文 -> 结构    城市代码维护的请求报文体
	static int Packet2Struct(Req1007& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("city_code"         , stBody.city_code);
		pkt.GetParameterVal("city_name"         , stBody.city_name);
		pkt.GetParameterVal("all_stor_id"       , stBody.all_stor_id);
		pkt.GetParameterVal("stor_id_default"   , stBody.stor_id_default);
		return 0;
	};

	// 报文解析： 结构 -> 报文    城市代码维护的请求报文体
	static int Struct2Packet(Req1007& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("city_code"         , stBody.city_code);
		pkt.AddParameter("city_name"         , stBody.city_name);
		pkt.AddParameter("all_stor_id"       , stBody.all_stor_id);
		pkt.AddParameter("stor_id_default"   , stBody.stor_id_default);
		return 0;
	};

	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp1007& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("city_code"         , stBody.city_code);
		pkt.GetParameterVal("city_name"         , stBody.city_name);
		pkt.GetParameterVal("all_stor_id"       , stBody.all_stor_id);
		pkt.GetParameterVal("stor_id_default"   , stBody.stor_id_default);
		pkt.GetParameterVal("htm_code_list"     , stBody.htm_code_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的响应报文体
	static int Struct2Packet(Rsp1007& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("city_code"         , stBody.city_code);
		pkt.AddParameter("city_name"         , stBody.city_name);
		pkt.AddParameter("all_stor_id"       , stBody.all_stor_id);
		pkt.AddParameter("stor_id_default"   , stBody.stor_id_default);
		pkt.AddParameter("htm_code_list"     , stBody.htm_code_list);
		return 0;
	};



	// 报文解析：报文 -> 结构    客户综合信息查询
	static int Packet2Struct(Req1020& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("qry_cust_info"     , stBody.qry_cust_info);
		pkt.GetParameterVal("qry_fund"          , stBody.qry_fund);
		pkt.GetParameterVal("qry_storage"       , stBody.qry_storage);
		pkt.GetParameterVal("qry_forward"       , stBody.qry_forward);
		pkt.GetParameterVal("qry_defer"         , stBody.qry_defer);
		pkt.GetParameterVal("qry_surplus"       , stBody.qry_surplus);
		pkt.GetParameterVal("is_check_stat"     , stBody.is_check_stat);
		return 0;
	};

	// 报文解析： 结构 -> 报文    客户综合信息查询
	static int Struct2Packet(Req1020& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("qry_cust_info"     , stBody.qry_cust_info);
		pkt.AddParameter("qry_fund"          , stBody.qry_fund);
		pkt.AddParameter("qry_storage"       , stBody.qry_storage);
		pkt.AddParameter("qry_forward"       , stBody.qry_forward);
		pkt.AddParameter("qry_defer"         , stBody.qry_defer);
		pkt.AddParameter("qry_surplus"       , stBody.qry_surplus);
		pkt.AddParameter("is_check_stat"     , stBody.is_check_stat);
		return 0;
	};

	// 报文解析：报文 -> 结构    客户综合信息查询
	static int Packet2Struct(Rsp1020& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("c_acct_no"         , stBody.c_acct_no);
		pkt.GetParameterVal("c_cust_id"         , stBody.c_cust_id);
		pkt.GetParameterVal("c_account_no"      , stBody.c_account_no);
		pkt.GetParameterVal("c_open_bank_name"  , stBody.c_open_bank_name);
		pkt.GetParameterVal("c_cust_abbr"       , stBody.c_cust_abbr);
		pkt.GetParameterVal("c_b_fare_model_id" , stBody.c_b_fare_model_id);
		pkt.GetParameterVal("c_m_fare_model_id" , stBody.c_m_fare_model_id);
		pkt.GetParameterVal("c_acct_type"       , stBody.c_acct_type);
		pkt.GetParameterVal("c_ocma_flag"       , stBody.c_ocma_flag);
		pkt.GetParameterVal("c_acct_stat"       , stBody.c_acct_stat);
		pkt.GetParameterVal("c_broker_acct_no"  , stBody.c_broker_acct_no);
		pkt.GetParameterVal("c_cert_type"       , stBody.c_cert_type);
		pkt.GetParameterVal("c_cert_num"        , stBody.c_cert_num);
		pkt.GetParameterVal("c_branch_id"       , stBody.c_branch_id);
		pkt.GetParameterVal("c_trans_check_info", stBody.c_trans_check_info);
		pkt.GetParameterVal("f_currency_id"     , stBody.f_currency_id);
		pkt.GetParameterVal("f_curr_bal"        , stBody.f_curr_bal);
		pkt.GetParameterVal("f_can_use_bal"     , stBody.f_can_use_bal);
		pkt.GetParameterVal("f_can_get_bal"     , stBody.f_can_get_bal);
		pkt.GetParameterVal("f_in_bal"          , stBody.f_in_bal);
		pkt.GetParameterVal("f_out_bal"         , stBody.f_out_bal);
		pkt.GetParameterVal("f_buy_bal"         , stBody.f_buy_bal);
		pkt.GetParameterVal("f_sell_bal"        , stBody.f_sell_bal);
		pkt.GetParameterVal("f_exch_froz_bal"   , stBody.f_exch_froz_bal);
		pkt.GetParameterVal("f_posi_margin"     , stBody.f_posi_margin);
		pkt.GetParameterVal("f_base_margin"     , stBody.f_base_margin);
		pkt.GetParameterVal("f_take_margin"     , stBody.f_take_margin);
		pkt.GetParameterVal("f_stor_margin"     , stBody.f_stor_margin);
		pkt.GetParameterVal("f_pt_reserve"      , stBody.f_pt_reserve);
		pkt.GetParameterVal("f_ag_margin"       , stBody.f_ag_margin);
		pkt.GetParameterVal("f_forward_froz"    , stBody.f_forward_froz);
		pkt.GetParameterVal("f_exch_fare"       , stBody.f_exch_fare);
		pkt.GetParameterVal("s_variety_id"      , stBody.s_variety_id);
		pkt.GetParameterVal("s_curr_amt"        , stBody.s_curr_amt);
		pkt.GetParameterVal("s_can_use_amt"     , stBody.s_can_use_amt);
		pkt.GetParameterVal("s_can_get_amt"     , stBody.s_can_get_amt);
		pkt.GetParameterVal("s_day_deposit"     , stBody.s_day_deposit);
		pkt.GetParameterVal("s_day_draw"        , stBody.s_day_draw);
		pkt.GetParameterVal("s_real_buy_amt"    , stBody.s_real_buy_amt);
		pkt.GetParameterVal("s_real_sell_amt"   , stBody.s_real_sell_amt);
		pkt.GetParameterVal("s_entr_sell_amt"   , stBody.s_entr_sell_amt);
		pkt.GetParameterVal("s_app_froz_amt"    , stBody.s_app_froz_amt);
		pkt.GetParameterVal("s_unit_cost"       , stBody.s_unit_cost);
		pkt.GetParameterVal("t5_prod_code"      , stBody.t5_prod_code);
		pkt.GetParameterVal("t5_long_amt"       , stBody.t5_long_amt);
		pkt.GetParameterVal("t5_short_amt"      , stBody.t5_short_amt);
		pkt.GetParameterVal("t5_can_use_long"   , stBody.t5_can_use_long);
		pkt.GetParameterVal("t5_can_use_short"  , stBody.t5_can_use_short);
		pkt.GetParameterVal("t5_day_open_long"  , stBody.t5_day_open_long);
		pkt.GetParameterVal("t5_day_open_short" , stBody.t5_day_open_short);
		pkt.GetParameterVal("td_prod_code"      , stBody.td_prod_code);
		pkt.GetParameterVal("td_long_amt"       , stBody.td_long_amt);
		pkt.GetParameterVal("td_short_amt"      , stBody.td_short_amt);
		pkt.GetParameterVal("td_can_use_long"   , stBody.td_can_use_long);
		pkt.GetParameterVal("td_can_use_short"  , stBody.td_can_use_short);
		pkt.GetParameterVal("td_day_open_long"  , stBody.td_day_open_long);
		pkt.GetParameterVal("td_day_open_short" , stBody.td_day_open_short);
		pkt.GetParameterVal("td_day_cov_long"   , stBody.td_day_cov_long);
		pkt.GetParameterVal("td_day_cov_short"  , stBody.td_day_cov_short);
		pkt.GetParameterVal("td_day_deli_long"  , stBody.td_day_deli_long);
		pkt.GetParameterVal("td_day_deli_short" , stBody.td_day_deli_short);
		pkt.GetParameterVal("td_day_cov_long_froz", stBody.td_day_cov_long_froz);
		pkt.GetParameterVal("td_day_cov_short_froz", stBody.td_day_cov_short_froz);
		pkt.GetParameterVal("td_day_deli_long_forz", stBody.td_day_deli_long_forz);
		pkt.GetParameterVal("td_day_deli_short_forz", stBody.td_day_deli_short_forz);
		pkt.GetParameterVal("td_long_open_avg_price ", stBody.td_long_open_avg_price );
		pkt.GetParameterVal("td_short_open_avg_price", stBody.td_short_open_avg_price);
		pkt.GetParameterVal("td_long_posi_avg_price ", stBody.td_long_posi_avg_price );
		pkt.GetParameterVal("td_short_posi_avg_price", stBody.td_short_posi_avg_price);
		pkt.GetParameterVal("td_long_margin         ", stBody.td_long_margin         );
		pkt.GetParameterVal("td_short_margin        ", stBody.td_short_margin        );
		pkt.GetParameterVal("td_last_settle_price   ", stBody.td_last_settle_price   );
		pkt.GetParameterVal("td_day_settle_price    ", stBody.td_day_settle_price    );
		pkt.GetParameterVal("r_surplus"         , stBody.r_surplus);
		pkt.GetParameterVal("r_risk_index"      , stBody.r_risk_index);
		pkt.GetParameterVal("r_risk_level"      , stBody.r_risk_level);
		pkt.GetParameterVal("hlm_stor_info"     , stBody.hlm_stor_info);
		pkt.GetParameterVal("htm_t5_info"       , stBody.htm_t5_info);
		pkt.GetParameterVal("htm_td_info"       , stBody.htm_td_info);
		pkt.GetParameterVal("htm_prod_group_info"       , stBody.htm_prod_group_info);
		pkt.GetParameterVal("htm_acct_large_side_sum"       , stBody.htm_acct_large_side_sum);

		return 0;
	};

	// 报文解析： 结构 -> 报文    客户综合信息查询
	static int Struct2Packet(Rsp1020& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("c_acct_no"         , stBody.c_acct_no);
		pkt.AddParameter("c_cust_id"         , stBody.c_cust_id);
		pkt.AddParameter("c_account_no"      , stBody.c_account_no);
		pkt.AddParameter("c_open_bank_name"  , stBody.c_open_bank_name);
		pkt.AddParameter("c_cust_abbr"       , stBody.c_cust_abbr);
		pkt.AddParameter("c_b_fare_model_id" , stBody.c_b_fare_model_id);
		pkt.AddParameter("c_m_fare_model_id" , stBody.c_m_fare_model_id);
		pkt.AddParameter("c_acct_type"       , stBody.c_acct_type);
		pkt.AddParameter("c_ocma_flag"       , stBody.c_ocma_flag);
		pkt.AddParameter("c_acct_stat"       , stBody.c_acct_stat);
		pkt.AddParameter("c_broker_acct_no"  , stBody.c_broker_acct_no);
		pkt.AddParameter("c_cert_type"       , stBody.c_cert_type);
		pkt.AddParameter("c_cert_num"        , stBody.c_cert_num);
		pkt.AddParameter("c_branch_id"       , stBody.c_branch_id);
		pkt.AddParameter("c_trans_check_info", stBody.c_trans_check_info);
		pkt.AddParameter("f_currency_id"     , stBody.f_currency_id);
		pkt.AddParameter("f_curr_bal"        , stBody.f_curr_bal);
		pkt.AddParameter("f_can_use_bal"     , stBody.f_can_use_bal);
		pkt.AddParameter("f_can_get_bal"     , stBody.f_can_get_bal);
		pkt.AddParameter("f_in_bal"          , stBody.f_in_bal);
		pkt.AddParameter("f_out_bal"         , stBody.f_out_bal);
		pkt.AddParameter("f_buy_bal"         , stBody.f_buy_bal);
		pkt.AddParameter("f_sell_bal"        , stBody.f_sell_bal);
		pkt.AddParameter("f_exch_froz_bal"   , stBody.f_exch_froz_bal);
		pkt.AddParameter("f_posi_margin"     , stBody.f_posi_margin);
		pkt.AddParameter("f_base_margin"     , stBody.f_base_margin);
		pkt.AddParameter("f_take_margin"     , stBody.f_take_margin);
		pkt.AddParameter("f_stor_margin"     , stBody.f_stor_margin);
		pkt.AddParameter("f_pt_reserve"      , stBody.f_pt_reserve);
		pkt.AddParameter("f_ag_margin"       , stBody.f_ag_margin);
		pkt.AddParameter("f_forward_froz"    , stBody.f_forward_froz);
		pkt.AddParameter("f_exch_fare"       , stBody.f_exch_fare);
		pkt.AddParameter("s_variety_id"      , stBody.s_variety_id);
		pkt.AddParameter("s_curr_amt"        , stBody.s_curr_amt);
		pkt.AddParameter("s_can_use_amt"     , stBody.s_can_use_amt);
		pkt.AddParameter("s_can_get_amt"     , stBody.s_can_get_amt);
		pkt.AddParameter("s_day_deposit"     , stBody.s_day_deposit);
		pkt.AddParameter("s_day_draw"        , stBody.s_day_draw);
		pkt.AddParameter("s_real_buy_amt"    , stBody.s_real_buy_amt);
		pkt.AddParameter("s_real_sell_amt"   , stBody.s_real_sell_amt);
		pkt.AddParameter("s_entr_sell_amt"   , stBody.s_entr_sell_amt);
		pkt.AddParameter("s_app_froz_amt"    , stBody.s_app_froz_amt);
		pkt.AddParameter("s_unit_cost"       , stBody.s_unit_cost);
		pkt.AddParameter("t5_prod_code"      , stBody.t5_prod_code);
		pkt.AddParameter("t5_long_amt"       , stBody.t5_long_amt);
		pkt.AddParameter("t5_short_amt"      , stBody.t5_short_amt);
		pkt.AddParameter("t5_can_use_long"   , stBody.t5_can_use_long);
		pkt.AddParameter("t5_can_use_short"  , stBody.t5_can_use_short);
		pkt.AddParameter("t5_day_open_long"  , stBody.t5_day_open_long);
		pkt.AddParameter("t5_day_open_short" , stBody.t5_day_open_short);
		pkt.AddParameter("td_prod_code"      , stBody.td_prod_code);
		pkt.AddParameter("td_long_amt"       , stBody.td_long_amt);
		pkt.AddParameter("td_short_amt"      , stBody.td_short_amt);
		pkt.AddParameter("td_can_use_long"   , stBody.td_can_use_long);
		pkt.AddParameter("td_can_use_short"  , stBody.td_can_use_short);
		pkt.AddParameter("td_day_open_long"  , stBody.td_day_open_long);
		pkt.AddParameter("td_day_open_short" , stBody.td_day_open_short);
		pkt.AddParameter("td_day_cov_long"   , stBody.td_day_cov_long);
		pkt.AddParameter("td_day_cov_short"  , stBody.td_day_cov_short);
		pkt.AddParameter("td_day_deli_long"  , stBody.td_day_deli_long);
		pkt.AddParameter("td_day_deli_short" , stBody.td_day_deli_short);
		pkt.AddParameter("td_day_cov_long_froz", stBody.td_day_cov_long_froz);
		pkt.AddParameter("td_day_cov_short_froz", stBody.td_day_cov_short_froz);
		pkt.AddParameter("td_day_deli_long_forz", stBody.td_day_deli_long_forz);
		pkt.AddParameter("td_day_deli_short_forz", stBody.td_day_deli_short_forz);
		pkt.AddParameter("td_long_open_avg_price ", stBody.td_long_open_avg_price );
		pkt.AddParameter("td_short_open_avg_price", stBody.td_short_open_avg_price);
		pkt.AddParameter("td_long_posi_avg_price ", stBody.td_long_posi_avg_price );
		pkt.AddParameter("td_short_posi_avg_price", stBody.td_short_posi_avg_price);
		pkt.AddParameter("td_long_margin         ", stBody.td_long_margin         );
		pkt.AddParameter("td_short_margin        ", stBody.td_short_margin        );
		pkt.AddParameter("td_last_settle_price   ", stBody.td_last_settle_price   );
		pkt.AddParameter("td_day_settle_price    ", stBody.td_day_settle_price    );
		pkt.AddParameter("r_surplus"         , stBody.r_surplus);
		pkt.AddParameter("r_risk_index"      , stBody.r_risk_index);
		pkt.AddParameter("r_risk_level"      , stBody.r_risk_level);
		pkt.AddParameter("hlm_stor_info"     , stBody.hlm_stor_info);
		pkt.AddParameter("htm_t5_info"       , stBody.htm_t5_info);
		pkt.AddParameter("htm_td_info"       , stBody.htm_td_info);
		pkt.AddParameter("htm_prod_group_info"       , stBody.htm_prod_group_info);
		pkt.AddParameter("htm_acct_large_side_sum"       , stBody.htm_acct_large_side_sum);

		return 0;
	};



	// 报文解析：报文 -> 结构    账户密码修改的请求报文体
	static int Packet2Struct(Req2022& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("acct_type"         , stBody.acct_type);
		pkt.GetParameterVal("old_exch_pwd"      , stBody.old_exch_pwd);
		pkt.GetParameterVal("exch_pwd"          , stBody.exch_pwd);
		pkt.GetParameterVal("old_fund_pwd"      , stBody.old_fund_pwd);
		pkt.GetParameterVal("fund_pwd"          , stBody.fund_pwd);
		return 0;
	};

	// 报文解析： 结构 -> 报文    账户密码修改的请求报文体
	static int Struct2Packet(Req2022& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("acct_type"         , stBody.acct_type);
		pkt.AddParameter("old_exch_pwd"      , stBody.old_exch_pwd);
		pkt.AddParameter("exch_pwd"          , stBody.exch_pwd);
		pkt.AddParameter("old_fund_pwd"      , stBody.old_fund_pwd);
		pkt.AddParameter("fund_pwd"          , stBody.fund_pwd);
		return 0;
	};

	// 报文解析：报文 -> 结构    账户密码修改的响应报文体
	static int Packet2Struct(Rsp2022& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		return 0;
	};

	// 报文解析： 结构 -> 报文    账户密码修改的响应报文体
	static int Struct2Packet(Rsp2022& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		return 0;
	};



	// 报文解析：报文 -> 结构    客户实际费率查询的请求报文体
	static int Packet2Struct(Req2040& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		return 0;
	};

	// 报文解析： 结构 -> 报文    客户实际费率查询的请求报文体
	static int Struct2Packet(Req2040& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		return 0;
	};

	// 报文解析：报文 -> 结构    客户实际费率查询的响应报文体
	static int Packet2Struct(Rsp2040& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("alm_b_fare_list"   , stBody.alm_b_fare_list);
		pkt.GetParameterVal("alm_m_fare_list"   , stBody.alm_m_fare_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    客户实际费率查询的响应报文体
	static int Struct2Packet(Rsp2040& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("alm_b_fare_list"   , stBody.alm_b_fare_list);
		pkt.AddParameter("alm_m_fare_list"   , stBody.alm_m_fare_list);
		return 0;
	};



	// 报文解析：报文 -> 结构    日结单确认的请求报文体
	static int Packet2Struct(Req2201& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("affirm_date"       , stBody.affirm_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    日结单确认的请求报文体
	static int Struct2Packet(Req2201& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("affirm_date"       , stBody.affirm_date);
		return 0;
	};

	// 报文解析：报文 -> 结构    日结单确认的响应报文体
	static int Packet2Struct(Rsp2201& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("affirm_date"       , stBody.affirm_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    日结单确认的响应报文体
	static int Struct2Packet(Rsp2201& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("affirm_date"       , stBody.affirm_date);
		return 0;
	};


	// 报文解析： 结构 -> 报文    自动出入金转账的请求报文体
	static int Struct2Packet(Req3010& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("serial_no"         , stBody.serial_no);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("access_way"        , stBody.access_way);
		pkt.AddParameter("exch_bal"          , stBody.exch_bal);
		pkt.AddParameter("remark"            , stBody.remark);
		pkt.AddParameter("cust_id"           , stBody.cust_id);
		pkt.AddParameter("cust_abbr"         , stBody.cust_abbr);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("c_status"          , stBody.c_status);
		pkt.AddParameter("o_teller_id"       , stBody.o_teller_id);
		pkt.AddParameter("account_no"        , stBody.account_no);
		pkt.AddParameter("open_bank_name"    , stBody.open_bank_name);
		pkt.AddParameter("fund_pwd"          , stBody.fund_pwd);
		return 0;
	};

	// 报文解析：报文 -> 结构    自动出入金转账的响应报文体
	static int Packet2Struct(Rsp3010& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("serial_no"         , stBody.serial_no);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("cust_id"           , stBody.cust_id);
		pkt.GetParameterVal("cust_abbr"         , stBody.cust_abbr);
		pkt.GetParameterVal("acct_stat"         , stBody.acct_stat);
		pkt.GetParameterVal("exec_name"         , stBody.exec_name);
		pkt.GetParameterVal("exec_cert_num"     , stBody.exec_cert_num);
		pkt.GetParameterVal("curr_bal"          , stBody.curr_bal);
		pkt.GetParameterVal("can_get_cash"      , stBody.can_get_cash);
		pkt.GetParameterVal("can_use_cash"      , stBody.can_use_cash);
		pkt.GetParameterVal("alm_custtype_list" , stBody.alm_custtype_list);
		return 0;
	};



	// 报文解析：报文 -> 结构    自动出入金转账的请求报文体
	static int Packet2Struct(Req3021& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("access_way"        , stBody.access_way);
		pkt.GetParameterVal("exch_bal"          , stBody.exch_bal);
		pkt.GetParameterVal("fund_pwd"          , stBody.fund_pwd);
		pkt.GetParameterVal("remark"            , stBody.remark);
		pkt.GetParameterVal("cust_id"           , stBody.cust_id);
		pkt.GetParameterVal("cust_abbr"         , stBody.cust_abbr);
		pkt.GetParameterVal("cust_type"         , stBody.cust_type);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("card_pwd"         , stBody.card_pwd);
		return 0;
	};

	// 报文解析： 结构 -> 报文    自动出入金转账的请求报文体
	static int Struct2Packet(Req3021& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("access_way"        , stBody.access_way);
		pkt.AddParameter("exch_bal"          , stBody.exch_bal);
		pkt.AddParameter("fund_pwd"          , stBody.fund_pwd);
		pkt.AddParameter("remark"            , stBody.remark);
		pkt.AddParameter("cust_id"           , stBody.cust_id);
		pkt.AddParameter("cust_abbr"         , stBody.cust_abbr);
		pkt.AddParameter("cust_type"         , stBody.cust_type);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("card_pwd"         , stBody.card_pwd);
		return 0;
	};

	// 报文解析：报文 -> 结构    自动出入金转账的响应报文体
	static int Packet2Struct(Rsp3021& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("alm_custtype_list" , stBody.alm_custtype_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    自动出入金转账的响应报文体
	static int Struct2Packet(Rsp3021& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("alm_custtype_list" , stBody.alm_custtype_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    自动出入金转账的请求报文体
	static int Struct2Packet(Rep3024& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"       , stBody.oper_flag);
		pkt.AddParameter("acct_no"         , stBody.acct_no);
		pkt.AddParameter("bank_no"         , stBody.bank_no);
		pkt.AddParameter("bk_mer_code"     , stBody.bk_mer_code);
		pkt.AddParameter("bk_cust_no"      , stBody.bk_cust_no);
		pkt.AddParameter("bk_acct_name"    , stBody.bk_acct_name);
		pkt.AddParameter("bk_acct_no"      , stBody.bk_acct_no);
		pkt.AddParameter("bk_cert_type"    , stBody.bk_cert_type);
		pkt.AddParameter("bk_cert_no"      , stBody.bk_cert_no);
		pkt.AddParameter("sms_flag"        , stBody.sms_flag);
		pkt.AddParameter("phone_num"       , stBody.phone_num);
		pkt.AddParameter("remark"          , stBody.remark);

		return 0;
	};

	// 报文解析：报文 -> 结构    自动出入金转账的响应报文体
	static int Packet2Struct(Rsp3024& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"       , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"         , stBody.acct_no);
		pkt.GetParameterVal("bank_no"         , stBody.bank_no);
		pkt.GetParameterVal("bk_mer_code"     , stBody.bk_mer_code);
		pkt.GetParameterVal("bk_cust_no"      , stBody.bk_cust_no);
		pkt.GetParameterVal("bk_acct_name"    , stBody.bk_acct_name);
		pkt.GetParameterVal("bk_acct_no"      , stBody.bk_acct_no);
		pkt.GetParameterVal("bk_cert_type"    , stBody.bk_cert_type);
		pkt.GetParameterVal("bk_cert_no"      , stBody.bk_cert_no);
		pkt.GetParameterVal("sms_flag"        , stBody.sms_flag);
		pkt.GetParameterVal("phone_num"       , stBody.phone_num);
		pkt.GetParameterVal("remark"          , stBody.remark);

		return 0;
	};


	// 报文解析：报文 -> 结构    码表获取的请求报文体
	static int Packet2Struct(Req3064& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("begin_time"        , stBody.begin_time);
		pkt.GetParameterVal("end_time"          , stBody.end_time);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("tip_type"          , stBody.tip_type);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req3064& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("begin_time"         , stBody.begin_time);
		pkt.AddParameter("end_time"         , stBody.end_time);
		pkt.AddParameter("acct_no"         , stBody.acct_no);
		pkt.AddParameter("tip_type"         , stBody.tip_type);
		return 0;
	};

	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp3064& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("alm_result"       , stBody.alm_result);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的响应报文体
	static int Struct2Packet(Rsp3064& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("alm_result"       , stBody.alm_result);
		return 0;
	};



	// 报文解析：报文 -> 结构    码表获取的请求报文体
	static int Packet2Struct(Req3069& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"                , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"                  , stBody.acct_no);
		pkt.GetParameterVal("instid"                   , stBody.instid);
		pkt.GetParameterVal("gene_type"                , stBody.gene_type);
		pkt.GetParameterVal("begin_date"               , stBody.begin_date);
		pkt.GetParameterVal("end_date"                 , stBody.end_date);
		pkt.GetParameterVal("order_status_set"         , stBody.order_status_set);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req3069& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"                , stBody.oper_flag);
		pkt.AddParameter("acct_no"                  , stBody.acct_no);
		pkt.AddParameter("instid"                   , stBody.instid);
		pkt.AddParameter("gene_type"                , stBody.gene_type);
		pkt.AddParameter("begin_date"               , stBody.begin_date);
		pkt.AddParameter("end_date"                 , stBody.end_date);
		pkt.AddParameter("order_status_set"         , stBody.order_status_set);
		return 0;
	};

	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp3069& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"                , stBody.oper_flag);
		pkt.GetParameterVal("alm_result"         , stBody.order_status_set);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的响应报文体
	static int Struct2Packet(Rsp3069& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"                , stBody.oper_flag);
		pkt.AddParameter("order_status_set"         , stBody.order_status_set);
		return 0;
	};



	// 报文解析：报文 -> 结构    码表获取的请求报文体
	static int Packet2Struct(Req3099& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req3099& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		return 0;
	};

	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp3099& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("risk_degree1"       , stBody.risk_degree1);
		pkt.GetParameterVal("risk_degree2"       , stBody.risk_degree2);
		pkt.GetParameterVal("risk_grade"        , stBody.risk_grade);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的响应报文体
	static int Struct2Packet(Rsp3099& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("risk_degree1"       , stBody.risk_degree1);
		pkt.AddParameter("risk_degree2"       , stBody.risk_degree2);
		pkt.AddParameter("risk_grade"        , stBody.risk_grade);
		return 0;
	};



	// 报文解析：报文 -> 结构    保证金划拨的请求报文体
	static int Packet2Struct(Req3101& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("serial_no"         , stBody.serial_no);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("access_way"        , stBody.access_way);
		pkt.GetParameterVal("exch_bal"          , stBody.exch_bal);
		pkt.GetParameterVal("remark"            , stBody.remark);
		pkt.GetParameterVal("fund_pwd"          , stBody.fund_pwd);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("grade_id"          , stBody.grade_id);
		pkt.GetParameterVal("o_term_type"       , stBody.o_term_type);
		pkt.GetParameterVal("send_stat"         , stBody.send_stat);
		return 0;
	};

	// 报文解析： 结构 -> 报文    保证金划拨的请求报文体
	static int Struct2Packet(Req3101& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("serial_no"         , stBody.serial_no);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("access_way"        , stBody.access_way);
		pkt.AddParameter("exch_bal"          , stBody.exch_bal);
		pkt.AddParameter("remark"            , stBody.remark);
		pkt.AddParameter("fund_pwd"          , stBody.fund_pwd);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("grade_id"          , stBody.grade_id);
		pkt.AddParameter("o_term_type"       , stBody.o_term_type);
		pkt.AddParameter("send_stat"         , stBody.send_stat);
		return 0;
	};

	// 报文解析：报文 -> 结构    保证金划拨的响应报文体
	static int Packet2Struct(Rsp3101& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("Exch_date"         , stBody.Exch_date);
		pkt.GetParameterVal("Serial_no"         , stBody.Serial_no);
		pkt.GetParameterVal("Acct_no"           , stBody.Acct_no);
		pkt.GetParameterVal("bank_no"           , stBody.bank_no);
		pkt.GetParameterVal("f_busi_type"       , stBody.f_busi_type);
		pkt.GetParameterVal("access_way"        , stBody.access_way);
		pkt.GetParameterVal("out_account_no"    , stBody.out_account_no);
		pkt.GetParameterVal("in_account_no"     , stBody.in_account_no);
		pkt.GetParameterVal("Exch_bal"          , stBody.Exch_bal);
		pkt.GetParameterVal("check_stat1"       , stBody.check_stat1);
		pkt.GetParameterVal("check_stat2"       , stBody.check_stat2);
		pkt.GetParameterVal("send_stat"         , stBody.send_stat);
		pkt.GetParameterVal("in_account_flag"   , stBody.in_account_flag);
		pkt.GetParameterVal("remark"            , stBody.remark);
		pkt.GetParameterVal("bk_plat_date"      , stBody.bk_plat_date);
		pkt.GetParameterVal("bk_seq_no"         , stBody.bk_seq_no);
		pkt.GetParameterVal("bk_rsp_code"       , stBody.bk_rsp_code);
		pkt.GetParameterVal("bk_rsp_msg"        , stBody.bk_rsp_msg);
		pkt.GetParameterVal("o_term_type"       , stBody.o_term_type);
		pkt.GetParameterVal("o_teller_id"       , stBody.o_teller_id);
		pkt.GetParameterVal("o_date"            , stBody.o_date);
		pkt.GetParameterVal("c_term_type1"      , stBody.c_term_type1);
		pkt.GetParameterVal("c_teller_id1"      , stBody.c_teller_id1);
		pkt.GetParameterVal("c_date1"           , stBody.c_date1);
		pkt.GetParameterVal("c_term_type2"      , stBody.c_term_type2);
		pkt.GetParameterVal("c_teller_id2"      , stBody.c_teller_id2);
		pkt.GetParameterVal("c_date2"           , stBody.c_date2);
		pkt.GetParameterVal("htm_result"        , stBody.htm_result);
		return 0;
	};

	// 报文解析： 结构 -> 报文    保证金划拨的响应报文体
	static int Struct2Packet(Rsp3101& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("Exch_date"         , stBody.Exch_date);
		pkt.AddParameter("Serial_no"         , stBody.Serial_no);
		pkt.AddParameter("Acct_no"           , stBody.Acct_no);
		pkt.AddParameter("bank_no"           , stBody.bank_no);
		pkt.AddParameter("f_busi_type"       , stBody.f_busi_type);
		pkt.AddParameter("access_way"        , stBody.access_way);
		pkt.AddParameter("out_account_no"    , stBody.out_account_no);
		pkt.AddParameter("in_account_no"     , stBody.in_account_no);
		pkt.AddParameter("Exch_bal"          , stBody.Exch_bal);
		pkt.AddParameter("check_stat1"       , stBody.check_stat1);
		pkt.AddParameter("check_stat2"       , stBody.check_stat2);
		pkt.AddParameter("send_stat"         , stBody.send_stat);
		pkt.AddParameter("in_account_flag"   , stBody.in_account_flag);
		pkt.AddParameter("remark"            , stBody.remark);
		pkt.AddParameter("bk_plat_date"      , stBody.bk_plat_date);
		pkt.AddParameter("bk_seq_no"         , stBody.bk_seq_no);
		pkt.AddParameter("bk_rsp_code"       , stBody.bk_rsp_code);
		pkt.AddParameter("bk_rsp_msg"        , stBody.bk_rsp_msg);
		pkt.AddParameter("o_term_type"       , stBody.o_term_type);
		pkt.AddParameter("o_teller_id"       , stBody.o_teller_id);
		pkt.AddParameter("o_date"            , stBody.o_date);
		pkt.AddParameter("c_term_type1"      , stBody.c_term_type1);
		pkt.AddParameter("c_teller_id1"      , stBody.c_teller_id1);
		pkt.AddParameter("c_date1"           , stBody.c_date1);
		pkt.AddParameter("c_term_type2"      , stBody.c_term_type2);
		pkt.AddParameter("c_teller_id2"      , stBody.c_teller_id2);
		pkt.AddParameter("c_date2"           , stBody.c_date2);
		pkt.AddParameter("htm_result"        , stBody.htm_result);
		return 0;
	};

	// 报文解析： 结构 -> 报文    码表获取的响应报文体
	static int Struct2Packet(Req3201& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"       , stBody.oper_flag);
		pkt.AddParameter("acct_no"         , stBody.acct_no);
		return 0;
	};

	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp3201& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"      , stBody.oper_flag);
		pkt.GetParameterVal("card_bal"       , stBody.card_bal);
		return 0;
	};



	// 报文解析：报文 -> 结构    交易统一请求报文
	static int Packet2Struct(Req4001& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);//add by xiao
		pkt.GetParameterVal("client_serial_no"  , stBody.client_serial_no);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("cust_id"           , stBody.cust_id);
		pkt.GetParameterVal("bank_no"           , stBody.bank_no);
		pkt.GetParameterVal("b_market_id"       , stBody.b_market_id);
		pkt.GetParameterVal("prod_code"         , stBody.prod_code);
		pkt.GetParameterVal("offset_flag"       , stBody.offset_flag);
		pkt.GetParameterVal("deli_flag"         , stBody.deli_flag);
		pkt.GetParameterVal("bs"                , stBody.bs);
		pkt.GetParameterVal("entr_price"        , stBody.entr_price);
		pkt.GetParameterVal("entr_amount"       , stBody.entr_amount);
		pkt.GetParameterVal("cov_type"          , stBody.cov_type);
		pkt.GetParameterVal("match_type"        , stBody.match_type);
		pkt.GetParameterVal("src_match_no"      , stBody.src_match_no);
		pkt.GetParameterVal("order_send_type"   , stBody.order_send_type);
		pkt.GetParameterVal("auto_send_time_s"  , stBody.auto_send_time_s);
		pkt.GetParameterVal("auto_send_time_e"  , stBody.auto_send_time_e);
		pkt.GetParameterVal("auto_send_stat_s"  , stBody.auto_send_stat_s);
		pkt.GetParameterVal("auto_send_stat_t"  , stBody.auto_send_stat_t);
		pkt.GetParameterVal("auto_send_way"     , stBody.auto_send_way);
		pkt.GetParameterVal("auto_send_comp"    , stBody.auto_send_comp);
		pkt.GetParameterVal("auto_send_price"   , stBody.auto_send_price);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交易统一请求报文
	static int Struct2Packet(Req4001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);//add by xiao
		pkt.AddParameter("client_serial_no"  , stBody.client_serial_no);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("cust_id"           , stBody.cust_id);
		pkt.AddParameter("bank_no"           , stBody.bank_no);
		pkt.AddParameter("b_market_id"       , stBody.b_market_id);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("offset_flag"       , stBody.offset_flag);
		pkt.AddParameter("deli_flag"         , stBody.deli_flag);
		pkt.AddParameter("bs"                , stBody.bs);
		pkt.AddParameter("entr_price"        , stBody.entr_price);
		pkt.AddParameter("entr_amount"       , stBody.entr_amount);
		pkt.AddParameter("cov_type"          , stBody.cov_type);
		pkt.AddParameter("match_type"        , stBody.match_type);
		pkt.AddParameter("src_match_no"      , stBody.src_match_no);
		pkt.AddParameter("order_send_type"   , stBody.order_send_type);
		pkt.AddParameter("auto_send_time_s"  , stBody.auto_send_time_s);
		pkt.AddParameter("auto_send_time_e"  , stBody.auto_send_time_e);
		pkt.AddParameter("auto_send_stat_s"  , stBody.auto_send_stat_s);
		pkt.AddParameter("auto_send_stat_t"  , stBody.auto_send_stat_t);
		pkt.AddParameter("auto_send_way"     , stBody.auto_send_way);
		pkt.AddParameter("auto_send_comp"    , stBody.auto_send_comp);
		pkt.AddParameter("auto_send_price"   , stBody.auto_send_price);
		return 0;
	};

	// 报文解析：报文 -> 结构    交易统一请求报文
	static int Packet2Struct(Rsp4001& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("client_serial_no"  , stBody.client_serial_no);
		pkt.GetParameterVal("local_order_no"    , stBody.local_order_no);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交易统一请求报文
	static int Struct2Packet(Rsp4001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("client_serial_no"  , stBody.client_serial_no);
		pkt.AddParameter("local_order_no"    , stBody.local_order_no);
		return 0;
	};



	// 报文解析：报文 -> 结构    交易撤单
	static int Packet2Struct(Req4061& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("cancel_order_no"   , stBody.cancel_order_no);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交易撤单
	static int Struct2Packet(Req4061& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("cancel_order_no"   , stBody.cancel_order_no);
		return 0;
	};

	// 报文解析：报文 -> 结构    交易撤单
	static int Packet2Struct(Rsp4061& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交易撤单
	static int Struct2Packet(Rsp4061& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		return 0;
	};



	// 报文解析：报文 -> 结构    平仓试算的请求报文
	static int Packet2Struct(Req4071& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("prod_code"         , stBody.prod_code);
		pkt.GetParameterVal("cov_bs"            , stBody.cov_bs);
		pkt.GetParameterVal("cov_price"         , stBody.cov_price);
		pkt.GetParameterVal("cov_amount"        , stBody.cov_amount);
		return 0;
	};

	// 报文解析： 结构 -> 报文    平仓试算的请求报文
	static int Struct2Packet(Req4071& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("cov_bs"            , stBody.cov_bs);
		pkt.AddParameter("cov_price"         , stBody.cov_price);
		pkt.AddParameter("cov_amount"        , stBody.cov_amount);
		return 0;
	};

	// 报文解析：报文 -> 结构    平仓试算的响应报文
	static int Packet2Struct(Rsp4071& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("cust_abbr"         , stBody.cust_abbr);
		pkt.GetParameterVal("prod_code"         , stBody.prod_code);
		pkt.GetParameterVal("remain_long"       , stBody.remain_long);
		pkt.GetParameterVal("remain_short"      , stBody.remain_short);
		pkt.GetParameterVal("cov_exch_fare"     , stBody.cov_exch_fare);
		pkt.GetParameterVal("cov_surplus"       , stBody.cov_surplus);
		pkt.GetParameterVal("can_use_bal"       , stBody.can_use_bal);
		return 0;
	};

	// 报文解析： 结构 -> 报文    平仓试算的响应报文
	static int Struct2Packet(Rsp4071& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("cust_abbr"         , stBody.cust_abbr);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("remain_long"       , stBody.remain_long);
		pkt.AddParameter("remain_short"      , stBody.remain_short);
		pkt.AddParameter("cov_exch_fare"     , stBody.cov_exch_fare);
		pkt.AddParameter("cov_surplus"       , stBody.cov_surplus);
		pkt.AddParameter("can_use_bal"       , stBody.can_use_bal);
		return 0;
	};



	// 报文解析：报文 -> 结构    机构提货日维护的请求报文体
	static int Packet2Struct(Req5041& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("year"              , stBody.year);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("is_search_usefull" , stBody.is_search_usefull);
		pkt.GetParameterVal("search_num"        , stBody.search_num);
		pkt.GetParameterVal("alm_take_sheet_date", stBody.alm_take_sheet_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    机构提货日维护的请求报文体
	static int Struct2Packet(Req5041& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("year"              , stBody.year);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("is_search_usefull" , stBody.is_search_usefull);
		pkt.AddParameter("search_num"        , stBody.search_num);
		pkt.AddParameter("alm_take_sheet_date", stBody.alm_take_sheet_date);
		return 0;
	};

	// 报文解析：报文 -> 结构    机构提货日维护的响应报文体
	static int Packet2Struct(Rsp5041& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("year"              , stBody.year);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("alm_take_sheet_date", stBody.alm_take_sheet_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    机构提货日维护的响应报文体
	static int Struct2Packet(Rsp5041& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("year"              , stBody.year);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("alm_take_sheet_date", stBody.alm_take_sheet_date);
		return 0;
	};



	// 报文解析：报文 -> 结构    带接口的提货申请流水维护的请求报文体
	static int Packet2Struct(Req5101& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("local_sheet_no"    , stBody.local_sheet_no);
		pkt.GetParameterVal("sheet_no"          , stBody.sheet_no);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("cust_id"           , stBody.cust_id);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("app_amount"        , stBody.app_amount);
		pkt.GetParameterVal("city_code"         , stBody.city_code);
		pkt.GetParameterVal("stor_id"           , stBody.stor_id);
		pkt.GetParameterVal("take_man"          , stBody.take_man);
		pkt.GetParameterVal("cert_type_id"      , stBody.cert_type_id);
		pkt.GetParameterVal("cert_num"          , stBody.cert_num);
		pkt.GetParameterVal("app_pwd"           , stBody.app_pwd);
		pkt.GetParameterVal("app_record"        , stBody.app_record);
		pkt.GetParameterVal("take_date"         , stBody.take_date);
		pkt.GetParameterVal("start_date"        , stBody.start_date);
		pkt.GetParameterVal("end_date"          , stBody.end_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    带接口的提货申请流水维护的请求报文体
	static int Struct2Packet(Req5101& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("local_sheet_no"    , stBody.local_sheet_no);
		pkt.AddParameter("sheet_no"          , stBody.sheet_no);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("cust_id"           , stBody.cust_id);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("app_amount"        , stBody.app_amount);
		pkt.AddParameter("city_code"         , stBody.city_code);
		pkt.AddParameter("stor_id"           , stBody.stor_id);
		pkt.AddParameter("take_man"          , stBody.take_man);
		pkt.AddParameter("cert_type_id"      , stBody.cert_type_id);
		pkt.AddParameter("cert_num"          , stBody.cert_num);
		pkt.AddParameter("app_pwd"           , stBody.app_pwd);
		pkt.AddParameter("app_record"        , stBody.app_record);
		pkt.AddParameter("take_date"         , stBody.take_date);
		pkt.AddParameter("start_date"        , stBody.start_date);
		pkt.AddParameter("end_date"          , stBody.end_date);
		return 0;
	};

	// 报文解析：报文 -> 结构    提货申请流水查询的响应报文体
	static int Packet2Struct(Rsp5101& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("local_sheet_no"    , stBody.local_sheet_no);
		pkt.GetParameterVal("sheet_no"          , stBody.sheet_no);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("cust_id"           , stBody.cust_id);
		pkt.GetParameterVal("variety_id"        , stBody.variety_id);
		pkt.GetParameterVal("app_amount"        , stBody.app_amount);
		pkt.GetParameterVal("draw_type"         , stBody.draw_type);
		pkt.GetParameterVal("self_draw"         , stBody.self_draw);
		pkt.GetParameterVal("trade_draw"        , stBody.trade_draw);
		pkt.GetParameterVal("city_code"         , stBody.city_code);
		pkt.GetParameterVal("stor_id"           , stBody.stor_id);
		pkt.GetParameterVal("take_man"          , stBody.take_man);
		pkt.GetParameterVal("cert_type_id"      , stBody.cert_type_id);
		pkt.GetParameterVal("cert_num"          , stBody.cert_num);
		pkt.GetParameterVal("app_record"        , stBody.app_record);
		pkt.GetParameterVal("app_date"          , stBody.app_date);
		pkt.GetParameterVal("app_time"          , stBody.app_time);
		pkt.GetParameterVal("app_exch_date"     , stBody.app_exch_date);
		pkt.GetParameterVal("app_start_date"    , stBody.app_start_date);
		pkt.GetParameterVal("app_end_date"      , stBody.app_end_date);
		pkt.GetParameterVal("due_take_date"     , stBody.due_take_date);
		pkt.GetParameterVal("stat"              , stBody.stat);
		pkt.GetParameterVal("cancel_date"       , stBody.cancel_date);
		pkt.GetParameterVal("cancel_time"       , stBody.cancel_time);
		pkt.GetParameterVal("cancel_exch_date"  , stBody.cancel_exch_date);
		pkt.GetParameterVal("cancel_record"     , stBody.cancel_record);
		pkt.GetParameterVal("send_stat"         , stBody.send_stat);
		pkt.GetParameterVal("take_margin"       , stBody.take_margin);
		pkt.GetParameterVal("is_other_take"     , stBody.is_other_take);
		pkt.GetParameterVal("take_affirm"       , stBody.take_affirm);
		pkt.GetParameterVal("htm_take_sheet_detail", stBody.htm_take_sheet_detail);
		return 0;
	};

	// 报文解析： 结构 -> 报文    提货申请流水查询的响应报文体
	static int Struct2Packet(Rsp5101& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("local_sheet_no"    , stBody.local_sheet_no);
		pkt.AddParameter("sheet_no"          , stBody.sheet_no);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("cust_id"           , stBody.cust_id);
		pkt.AddParameter("variety_id"        , stBody.variety_id);
		pkt.AddParameter("app_amount"        , stBody.app_amount);
		pkt.AddParameter("draw_type"         , stBody.draw_type);
		pkt.AddParameter("self_draw"         , stBody.self_draw);
		pkt.AddParameter("trade_draw"        , stBody.trade_draw);
		pkt.AddParameter("city_code"         , stBody.city_code);
		pkt.AddParameter("stor_id"           , stBody.stor_id);
		pkt.AddParameter("take_man"          , stBody.take_man);
		pkt.AddParameter("cert_type_id"      , stBody.cert_type_id);
		pkt.AddParameter("cert_num"          , stBody.cert_num);
		pkt.AddParameter("app_record"        , stBody.app_record);
		pkt.AddParameter("app_date"          , stBody.app_date);
		pkt.AddParameter("app_time"          , stBody.app_time);
		pkt.AddParameter("app_exch_date"     , stBody.app_exch_date);
		pkt.AddParameter("app_start_date"    , stBody.app_start_date);
		pkt.AddParameter("app_end_date"      , stBody.app_end_date);
		pkt.AddParameter("due_take_date"     , stBody.due_take_date);
		pkt.AddParameter("stat"              , stBody.stat);
		pkt.AddParameter("cancel_date"       , stBody.cancel_date);
		pkt.AddParameter("cancel_time"       , stBody.cancel_time);
		pkt.AddParameter("cancel_exch_date"  , stBody.cancel_exch_date);
		pkt.AddParameter("cancel_record"     , stBody.cancel_record);
		pkt.AddParameter("send_stat"         , stBody.send_stat);
		pkt.AddParameter("take_margin"       , stBody.take_margin);
		pkt.AddParameter("is_other_take"     , stBody.is_other_take);
		pkt.AddParameter("take_affirm"       , stBody.take_affirm);
		pkt.AddParameter("htm_take_sheet_detail", stBody.htm_take_sheet_detail);
		return 0;
	};



	// 报文解析：报文 -> 结构    提货单打印请求报文
	static int Packet2Struct(Req5103& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("local_sheet_no"    , stBody.local_sheet_no);
		pkt.GetParameterVal("sheet_no"          , stBody.sheet_no);
		pkt.GetParameterVal("acct_no"           , stBody.acct_no);
		pkt.GetParameterVal("cust_id      "     , stBody.cust_id      );
		pkt.GetParameterVal("start_date"        , stBody.start_date);
		pkt.GetParameterVal("end_date"          , stBody.end_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    提货单打印请求报文
	static int Struct2Packet(Req5103& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("local_sheet_no"    , stBody.local_sheet_no);
		pkt.AddParameter("sheet_no"          , stBody.sheet_no);
		pkt.AddParameter("acct_no"           , stBody.acct_no);
		pkt.AddParameter("cust_id      "     , stBody.cust_id      );
		pkt.AddParameter("start_date"        , stBody.start_date);
		pkt.AddParameter("end_date"          , stBody.end_date);
		return 0;
	};

	// 报文解析：报文 -> 结构    提货单打印响应报文
	static int Packet2Struct(Rsp5103& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("alm_take_sheet_detail", stBody.alm_take_sheet_detail);
		return 0;
	};

	// 报文解析： 结构 -> 报文    提货单打印响应报文
	static int Struct2Packet(Rsp5103& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("alm_take_sheet_detail", stBody.alm_take_sheet_detail);
		return 0;
	};



	// 报文解析：报文 -> 结构    查询条件配置查询
	static int Packet2Struct(Req6001& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("query_id"          , stBody.query_id);
		return 0;
	};

	// 报文解析： 结构 -> 报文    查询条件配置查询
	static int Struct2Packet(Req6001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("query_id"          , stBody.query_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    查询条件配置查询的响应报文体
	static int Packet2Struct(Rsp6001& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("query_id"          , stBody.query_id);
		pkt.GetParameterVal("query_name"        , stBody.query_name);
		pkt.GetParameterVal("cell_num"          , stBody.cell_num);
		pkt.GetParameterVal("cell_inv_width"    , stBody.cell_inv_width);
		pkt.GetParameterVal("cell_inv_height"   , stBody.cell_inv_height);
		pkt.GetParameterVal("alm_options"       , stBody.alm_options);
		pkt.GetParameterVal("alm_field_en_name" , stBody.alm_field_en_name);
		pkt.GetParameterVal("alm_field_cn_name" , stBody.alm_field_cn_name);
		pkt.GetParameterVal("alm_field_width"   , stBody.alm_field_width);
		pkt.GetParameterVal("alm_field_align"   , stBody.alm_field_align);
		pkt.GetParameterVal("alm_field_format"  , stBody.alm_field_format);
		return 0;
	};

	// 报文解析： 结构 -> 报文    查询条件配置查询的响应报文体
	static int Struct2Packet(Rsp6001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("query_id"          , stBody.query_id);
		pkt.AddParameter("query_name"        , stBody.query_name);
		pkt.AddParameter("cell_num"          , stBody.cell_num);
		pkt.AddParameter("cell_inv_width"    , stBody.cell_inv_width);
		pkt.AddParameter("cell_inv_height"   , stBody.cell_inv_height);
		pkt.AddParameter("alm_options"       , stBody.alm_options);
		pkt.AddParameter("alm_field_en_name" , stBody.alm_field_en_name);
		pkt.AddParameter("alm_field_cn_name" , stBody.alm_field_cn_name);
		pkt.AddParameter("alm_field_width"   , stBody.alm_field_width);
		pkt.AddParameter("alm_field_align"   , stBody.alm_field_align);
		pkt.AddParameter("alm_field_format"  , stBody.alm_field_format);
		return 0;
	};



	// 报文解析：报文 -> 结构    普通查询
	static int Packet2Struct(Req6002& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("login_branch_id"   , stBody.login_branch_id);
		pkt.GetParameterVal("login_teller_id"   , stBody.login_teller_id);
		pkt.GetParameterVal("paginal_num"       , stBody.paginal_num);
		pkt.GetParameterVal("curr_page"         , stBody.curr_page);
		pkt.GetParameterVal("query_id"          , stBody.query_id);
		pkt.GetParameterVal("alm_view_field"    , stBody.alm_view_field);
		return 0;
	};

	// 报文解析： 结构 -> 报文    普通查询
	static int Struct2Packet(Req6002& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("login_branch_id"   , stBody.login_branch_id);
		pkt.AddParameter("login_teller_id"   , stBody.login_teller_id);
		pkt.AddParameter("paginal_num"       , stBody.paginal_num);
		pkt.AddParameter("curr_page"         , stBody.curr_page);
		pkt.AddParameter("query_id"          , stBody.query_id);
		pkt.AddParameter("alm_view_field"    , stBody.alm_view_field);
		return 0;
	};

	// 报文解析：报文 -> 结构    普通查询
	static int Packet2Struct(Rsp6002& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("paginal_num"       , stBody.paginal_num);
		pkt.GetParameterVal("page_count"        , stBody.page_count);
		pkt.GetParameterVal("curr_page"         , stBody.curr_page);
		pkt.GetParameterVal("alm_result"        , stBody.alm_result);
		return 0;
	};

	// 报文解析： 结构 -> 报文    普通查询
	static int Struct2Packet(Rsp6002& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("paginal_num"       , stBody.paginal_num);
		pkt.AddParameter("page_count"        , stBody.page_count);
		pkt.AddParameter("curr_page"         , stBody.curr_page);
		pkt.AddParameter("alm_result"        , stBody.alm_result);
		return 0;
	};



	// 报文解析：报文 -> 结构    报表条件配置查询的请求报文体
	static int Packet2Struct(Req6005& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("report_id"         , stBody.report_id);
		pkt.GetParameterVal("c_rdlc_ver"        , stBody.c_rdlc_ver);
		return 0;
	};

	// 报文解析： 结构 -> 报文    报表条件配置查询的请求报文体
	static int Struct2Packet(Req6005& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("report_id"         , stBody.report_id);
		pkt.AddParameter("c_rdlc_ver"        , stBody.c_rdlc_ver);
		return 0;
	};

	// 报文解析：报文 -> 结构    报表条件配置查询的响应报文体
	static int Packet2Struct(Rsp6005& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("report_id"         , stBody.report_id);
		pkt.GetParameterVal("report_name"       , stBody.report_name);
		pkt.GetParameterVal("cell_num"          , stBody.cell_num);
		pkt.GetParameterVal("cell_inv_width"    , stBody.cell_inv_width);
		pkt.GetParameterVal("cell_inv_height"   , stBody.cell_inv_height);
		pkt.GetParameterVal("s_rdlc_ver"        , stBody.s_rdlc_ver);
		pkt.GetParameterVal("alm_data_source"   , stBody.alm_data_source);
		pkt.GetParameterVal("alm_options"       , stBody.alm_options);
		return 0;
	};

	// 报文解析： 结构 -> 报文    报表条件配置查询的响应报文体
	static int Struct2Packet(Rsp6005& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("report_id"         , stBody.report_id);
		pkt.AddParameter("report_name"       , stBody.report_name);
		pkt.AddParameter("cell_num"          , stBody.cell_num);
		pkt.AddParameter("cell_inv_width"    , stBody.cell_inv_width);
		pkt.AddParameter("cell_inv_height"   , stBody.cell_inv_height);
		pkt.AddParameter("s_rdlc_ver"        , stBody.s_rdlc_ver);
		pkt.AddParameter("alm_data_source"   , stBody.alm_data_source);
		pkt.AddParameter("alm_options"       , stBody.alm_options);
		return 0;
	};



	// 报文解析：报文 -> 结构    数据源下载的请求报文体
	static int Packet2Struct(Req6007& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("exch_date"         , stBody.exch_date);
		pkt.GetParameterVal("alm_ds_list"       , stBody.alm_ds_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    数据源下载的请求报文体
	static int Struct2Packet(Req6007& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("exch_date"         , stBody.exch_date);
		pkt.AddParameter("alm_ds_list"       , stBody.alm_ds_list);
		return 0;
	};

	// 报文解析：报文 -> 结构    数据源下载的响应报文体
	static int Packet2Struct(Rsp6007& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"       , stBody.oper_flag);
		pkt.GetParameterVal("Title"                    , stBody.htm_Title);
		pkt.GetParameterVal("AcctSettleRisk"           , stBody.htm_AcctSettleRisk);
		pkt.GetParameterVal("AcctSettleFund"           , stBody.htm_AcctSettleFund);
		pkt.GetParameterVal("AcctSettleFundDetail"     , stBody.htm_AcctSettleFundDetail);
		pkt.GetParameterVal("AcctSettleStorage"        , stBody.htm_AcctSettleStorage);
		pkt.GetParameterVal("AcctSettleForward"        , stBody.htm_AcctSettleForward);
		pkt.GetParameterVal("AcctSettleDefer"          , stBody.htm_AcctSettleDefer);
		pkt.GetParameterVal("AcctSettleSurplus"        , stBody.htm_AcctSettleSurplus);
		pkt.GetParameterVal("AcctSettleMatchFlow"      , stBody.htm_AcctSettleMatchFlow);
		pkt.GetParameterVal("AcctSettleFundInOutFlow"  , stBody.htm_AcctSettleFundInOutFlow);

		pkt.GetParameterVal("TitleMonth"                    , stBody.htm_Title);
		pkt.GetParameterVal("AcctSettleRiskMonth"           , stBody.htm_AcctSettleRisk);
		pkt.GetParameterVal("AcctSettleFundMonth"           , stBody.htm_AcctSettleFund);
		pkt.GetParameterVal("AcctSettleFundDetailMonth"     , stBody.htm_AcctSettleFundDetail);
		pkt.GetParameterVal("AcctSettleStorageMonth"        , stBody.htm_AcctSettleStorage);
		pkt.GetParameterVal("AcctSettleDeferMonth"          , stBody.htm_AcctSettleDefer);
		pkt.GetParameterVal("AcctSettleSurplusMonth"        , stBody.htm_AcctSettleSurplus);
		return 0;
	};

	// 报文解析： 结构 -> 报文    数据源下载的响应报文体
	static int Struct2Packet(Rsp6007& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"           , stBody.oper_flag);
		pkt.AddParameter("AcctSettleRisk"  , stBody.htm_AcctSettleRisk);
		return 0;
	};



	// 报文解析：报文 -> 结构    用户登录
	static int Packet2Struct(Req8001& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("user_type"         , stBody.user_type);
		pkt.GetParameterVal("user_id_type"      , stBody.user_id_type);
		pkt.GetParameterVal("user_id"           , stBody.user_id);
		pkt.GetParameterVal("user_pwd"          , stBody.user_pwd);
		pkt.GetParameterVal("login_ip"          , stBody.login_ip);
		pkt.GetParameterVal("ver_num"           , stBody.ver_num);
		pkt.GetParameterVal("is_down_para"      , stBody.is_down_para);
		pkt.GetParameterVal("bank_no"           , stBody.bank_no);
		pkt.GetParameterVal("login_server_code" , stBody.login_server_code);
		pkt.GetParameterVal("isSign"            , stBody.isSign);
		return 0;
	};

	// 报文解析： 结构 -> 报文    用户登录
	static int Struct2Packet(Req8001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("user_type"         , stBody.user_type);
		pkt.AddParameter("user_id_type"      , stBody.user_id_type);
		pkt.AddParameter("user_id"           , stBody.user_id);
		pkt.AddParameter("user_pwd"          , stBody.user_pwd);
		pkt.AddParameter("login_ip"          , stBody.login_ip);
		pkt.AddParameter("ver_num"           , stBody.ver_num);
		pkt.AddParameter("is_down_para"      , stBody.is_down_para);
		pkt.AddParameter("bank_no"           , stBody.bank_no);
		pkt.AddParameter("login_server_code" , stBody.login_server_code);
		pkt.AddParameter("isSign"            , stBody.isSign);
		pkt.AddParameter("CN"            , stBody.CN);
		pkt.AddParameter("session_id"		, stBody.session_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    用户登录响应报文体
	static int Packet2Struct(Rsp8001& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("check_code"        , stBody.check_code);
		pkt.GetParameterVal("member_id"         , stBody.member_id);
		pkt.GetParameterVal("member_name"       , stBody.member_name);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("branch_name"       , stBody.branch_name);
		pkt.GetParameterVal("acct_type"         , stBody.acct_type);
		pkt.GetParameterVal("user_id"           , stBody.user_id);
		pkt.GetParameterVal("user_name"         , stBody.user_name);
		pkt.GetParameterVal("sys_date"          , stBody.sys_date);
		pkt.GetParameterVal("exch_date"         , stBody.exch_date);
		pkt.GetParameterVal("exch_time"         , stBody.exch_time);
		pkt.GetParameterVal("last_exch_date"    , stBody.last_exch_date);
		pkt.GetParameterVal("version"           , stBody.version);
		pkt.GetParameterVal("m_sys_stat"        , stBody.m_sys_stat);
		pkt.GetParameterVal("b_sys_stat"        , stBody.b_sys_stat);
		pkt.GetParameterVal("first_login"       , stBody.first_login);
		pkt.GetParameterVal("need_change_pwd"   , stBody.need_change_pwd);
		pkt.GetParameterVal("last_login_date"   , stBody.last_login_date);
		pkt.GetParameterVal("last_login_time"   , stBody.last_login_time);
		pkt.GetParameterVal("last_lan_ip"       , stBody.last_lan_ip);
		pkt.GetParameterVal("last_www_ip"       , stBody.last_www_ip);
		pkt.GetParameterVal("exist_login"       , stBody.exist_login);
		pkt.GetParameterVal("no_affirm_rpt_num" , stBody.no_affirm_rpt_num);
		pkt.GetParameterVal("comp_stat"         , stBody.comp_stat);
		pkt.GetParameterVal("auto_entry_menu_id"         , stBody.auto_entry_menu_id);
		pkt.GetParameterVal("alm_menu_info"     , stBody.alm_menu_info);
		pkt.GetParameterVal("alm_cust_type_id"  , stBody.alm_cust_type_id);
		pkt.GetParameterVal("more_login_msg"  , stBody.more_login_msg);
		pkt.GetParameterVal("alm_take_man_list" , stBody.alm_take_man_list);
		pkt.GetParameterVal("alm_role_list"     , stBody.alm_role_list);
		pkt.GetParameterVal("alm_code_table_list", stBody.alm_code_table_list);
		pkt.GetParameterVal("alm_prod_code_list", stBody.alm_prod_code_list);
		pkt.GetParameterVal("alm_variety_list"  , stBody.alm_variety_list);
		pkt.GetParameterVal("alm_city_code_list", stBody.alm_city_code_list);
		pkt.GetParameterVal("alm_fare_type_list", stBody.alm_fare_type_list);
		pkt.GetParameterVal("alm_query_list"    , stBody.alm_query_list);
		pkt.GetParameterVal("alm_report_list"   , stBody.alm_report_list);
		pkt.GetParameterVal("alm_noread_bulletin_list", stBody.alm_noread_bulletin_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    用户登录响应报文体
	static int Struct2Packet(Rsp8001& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("check_code"        , stBody.check_code);
		pkt.AddParameter("member_id"         , stBody.member_id);
		pkt.AddParameter("member_name"       , stBody.member_name);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("branch_name"       , stBody.branch_name);
		pkt.AddParameter("acct_type"         , stBody.acct_type);
		pkt.AddParameter("user_id"           , stBody.user_id);
		pkt.AddParameter("user_name"         , stBody.user_name);
		pkt.AddParameter("sys_date"          , stBody.sys_date);
		pkt.AddParameter("exch_date"         , stBody.exch_date);
		pkt.AddParameter("exch_time"         , stBody.exch_time);
		pkt.AddParameter("last_exch_date"    , stBody.last_exch_date);
		pkt.AddParameter("version"           , stBody.version);
		pkt.AddParameter("m_sys_stat"        , stBody.m_sys_stat);
		pkt.AddParameter("b_sys_stat"        , stBody.b_sys_stat);
		pkt.AddParameter("first_login"       , stBody.first_login);
		pkt.AddParameter("need_change_pwd"   , stBody.need_change_pwd);
		pkt.AddParameter("last_login_date"   , stBody.last_login_date);
		pkt.AddParameter("last_login_time"   , stBody.last_login_time);
		pkt.AddParameter("last_lan_ip"       , stBody.last_lan_ip);
		pkt.AddParameter("last_www_ip"       , stBody.last_www_ip);
		pkt.AddParameter("exist_login"       , stBody.exist_login);
		pkt.AddParameter("no_affirm_rpt_num" , stBody.no_affirm_rpt_num);
		pkt.AddParameter("comp_stat"         , stBody.comp_stat);
		pkt.AddParameter("alm_menu_info"     , stBody.alm_menu_info);
		pkt.AddParameter("alm_cust_type_id"  , stBody.alm_cust_type_id);
		pkt.AddParameter("alm_take_man_list" , stBody.alm_take_man_list);
		pkt.AddParameter("alm_role_list"     , stBody.alm_role_list);
		pkt.AddParameter("alm_code_table_list", stBody.alm_code_table_list);
		pkt.AddParameter("alm_prod_code_list", stBody.alm_prod_code_list);
		pkt.AddParameter("alm_variety_list"  , stBody.alm_variety_list);
		pkt.AddParameter("alm_city_code_list", stBody.alm_city_code_list);
		pkt.AddParameter("alm_fare_type_list", stBody.alm_fare_type_list);
		pkt.AddParameter("alm_query_list"    , stBody.alm_query_list);
		pkt.AddParameter("alm_report_list"   , stBody.alm_report_list);
		pkt.AddParameter("alm_noread_bulletin_list", stBody.alm_noread_bulletin_list);
		return 0;
	};



	// 报文解析：报文 -> 结构    用户退出
	static int Packet2Struct(Req8002& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("user_type"         , stBody.user_type);
		pkt.GetParameterVal("user_id"           , stBody.user_id);
		pkt.GetParameterVal("session_id"        , stBody.session_id);
		return 0;
	};

	// 报文解析： 结构 -> 报文    用户退出
	static int Struct2Packet(Req8002& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("user_type"         , stBody.user_type);
		pkt.AddParameter("user_id"           , stBody.user_id);
		pkt.AddParameter("session_id"        , stBody.session_id);
		return 0;
	};

	// 报文解析：报文 -> 结构    用户登录响应报文体
	static int Packet2Struct(Rsp8002& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		return 0;
	};

	// 报文解析： 结构 -> 报文    用户登录响应报文体
	static int Struct2Packet(Rsp8002& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		return 0;
	};



	// 报文解析：报文 -> 结构    登录认证
	static int Packet2Struct(Req8006& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("user_type"         , stBody.user_type);
		pkt.GetParameterVal("user_id_type"      , stBody.user_id_type);
		pkt.GetParameterVal("user_id"           , stBody.user_id);
		pkt.GetParameterVal("user_pwd"          , stBody.user_pwd);
		pkt.GetParameterVal("login_ip"          , stBody.login_ip);
		pkt.GetParameterVal("bank_no"           , stBody.bank_no);
		pkt.GetParameterVal("net_envionment"    , stBody.net_envionment);
		pkt.GetParameterVal("net_agent"         , stBody.net_agent);
		pkt.GetParameterVal("isSign"            , stBody.isSign);
		return 0;
	};

	// 报文解析： 结构 -> 报文    登录认证
	static int Struct2Packet(Req8006& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("user_type"         , stBody.user_type.c_str());
		pkt.AddParameter("user_id_type"      , stBody.user_id_type.c_str());
		pkt.AddParameter("user_id"           , stBody.user_id.c_str());
		pkt.AddParameter("user_pwd"          , stBody.user_pwd.c_str());
		pkt.AddParameter("login_ip"          , stBody.login_ip.c_str());
		pkt.AddParameter("bank_no"           , stBody.bank_no.c_str());
		pkt.AddParameter("net_envionment"    , stBody.net_envionment.c_str());
		pkt.AddParameter("net_agent"         , stBody.net_agent.c_str());
		pkt.AddParameter("isSign"            , stBody.isSign.c_str());
		return 0;
	};

	// 报文解析：报文 -> 结构    登录认证
	static int Packet2Struct(Rsp8006& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("session_id"        , stBody.session_id);
		pkt.GetParameterVal("session_key"       , stBody.session_key);
		pkt.GetParameterVal("server_code"       , stBody.server_code);
		pkt.GetParameterVal("server_name"       , stBody.server_name);
		pkt.GetParameterVal("trans_ip"          , stBody.trans_ip);
		pkt.GetParameterVal("trans_port"        , stBody.trans_port);
		pkt.GetParameterVal("query_ip"          , stBody.query_ip);
		pkt.GetParameterVal("query_port"        , stBody.query_port);
		pkt.GetParameterVal("broadcast_ip"      , stBody.broadcast_ip);
		pkt.GetParameterVal("broadcast_port"    , stBody.broadcast_port);
		pkt.GetParameterVal("risk_trans_ip"     , stBody.risk_trans_ip);
		pkt.GetParameterVal("risk_trans_port"   , stBody.risk_trans_port);
		pkt.GetParameterVal("risk_broadcast_ip" , stBody.risk_broadcast_ip);
		pkt.GetParameterVal("risk_broadcast_port", stBody.risk_broadcast_port);
		pkt.GetParameterVal("htm_server_list"   , stBody.htm_server_list);
		pkt.GetParameterVal("htm_more_login_list"   , stBody.htm_more_login_list);
		return 0;
	};

	// 报文解析： 结构 -> 报文    登录认证
	static int Struct2Packet(Rsp8006& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("session_id"        , stBody.session_id);
		pkt.AddParameter("session_key"       , stBody.session_key);
		pkt.AddParameter("server_code"       , stBody.server_code);
		pkt.AddParameter("server_name"       , stBody.server_name);
		pkt.AddParameter("trans_ip"          , stBody.trans_ip);
		pkt.AddParameter("trans_port"        , stBody.trans_port);
		pkt.AddParameter("query_ip"          , stBody.query_ip);
		pkt.AddParameter("query_port"        , stBody.query_port);
		pkt.AddParameter("broadcast_ip"      , stBody.broadcast_ip);
		pkt.AddParameter("broadcast_port"    , stBody.broadcast_port);
		pkt.AddParameter("risk_trans_ip"     , stBody.risk_trans_ip);
		pkt.AddParameter("risk_trans_port"   , stBody.risk_trans_port);
		pkt.AddParameter("risk_broadcast_ip" , stBody.risk_broadcast_ip);
		pkt.AddParameter("risk_broadcast_port", stBody.risk_broadcast_port);
		pkt.AddParameter("htm_server_list"   , stBody.htm_server_list);
		return 0;
	};



	// 报文解析：报文 -> 结构    登录认证
	static int Packet2Struct(Req8007& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("user_type"         , stBody.user_type);
		pkt.GetParameterVal("user_id_type"      , stBody.user_id_type);
		pkt.GetParameterVal("user_id"           , stBody.user_id);
		pkt.GetParameterVal("user_pwd"          , stBody.user_pwd);
		pkt.GetParameterVal("login_ip"          , stBody.login_ip);
		pkt.GetParameterVal("bank_no"           , stBody.bank_no);
		pkt.GetParameterVal("isSign"            , stBody.isSign);
		return 0;
	};

	// 报文解析： 结构 -> 报文    登录认证
	static int Struct2Packet(Req8007& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("user_type"         , stBody.user_type);
		pkt.AddParameter("user_id_type"      , stBody.user_id_type);
		pkt.AddParameter("user_id"           , stBody.user_id);
		pkt.AddParameter("user_pwd"          , stBody.user_pwd);
		pkt.AddParameter("login_ip"          , stBody.login_ip);
		pkt.AddParameter("bank_no"           , stBody.bank_no);
		pkt.AddParameter("isSign"            , stBody.isSign);
		return 0;
	};

	// 报文解析：报文 -> 结构    登录认证
	static int Packet2Struct(Rsp8007& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("succ"         , stBody.succ);
		return 0;
	};

	// 报文解析： 结构 -> 报文    登录认证
	static int Struct2Packet(Rsp8007& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("succ"        , stBody.succ);
		return 0;
	};



	// 报文解析：报文 -> 结构    交易所公告查询的请求报文体
	static int Packet2Struct(Req8030& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("seq_no"            , stBody.seq_no);
		pkt.GetParameterVal("title"             , stBody.title);
		pkt.GetParameterVal("s_exch_date"       , stBody.s_exch_date);
		pkt.GetParameterVal("e_exch_date"       , stBody.e_exch_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交易所公告查询的请求报文体
	static int Struct2Packet(Req8030& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("seq_no"            , stBody.seq_no);
		pkt.AddParameter("title"             , stBody.title);
		pkt.AddParameter("s_exch_date"       , stBody.s_exch_date);
		pkt.AddParameter("e_exch_date"       , stBody.e_exch_date);
		return 0;
	};

	// 报文解析：报文 -> 结构    交易所公告查询的响应报文体
	static int Packet2Struct(Rsp8030& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("alm_result"        , stBody.alm_result);
		return 0;
	};

	// 报文解析： 结构 -> 报文    交易所公告查询的响应报文体
	static int Struct2Packet(Rsp8030& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("alm_result"        , stBody.alm_result);
		return 0;
	};



	// 报文解析：报文 -> 结构    会员公告查询的请求报文体
	static int Packet2Struct(Req8031& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("bulletin_id"       , stBody.bulletin_id);
		pkt.GetParameterVal("title"             , stBody.title);
		pkt.GetParameterVal("s_exch_date"       , stBody.s_exch_date);
		pkt.GetParameterVal("e_exch_date"       , stBody.e_exch_date);
		return 0;
	};

	// 报文解析： 结构 -> 报文    会员公告查询的请求报文体
	static int Struct2Packet(Req8031& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("bulletin_id"       , stBody.bulletin_id);
		pkt.AddParameter("title"             , stBody.title);
		pkt.AddParameter("s_exch_date"       , stBody.s_exch_date);
		pkt.AddParameter("e_exch_date"       , stBody.e_exch_date);
		return 0;
	};

	// 报文解析：报文 -> 结构    会员公告查询的响应报文体
	static int Packet2Struct(Rsp8031& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("alm_result"        , stBody.alm_result);
		return 0;
	};

	// 报文解析： 结构 -> 报文    会员公告查询的响应报文体
	static int Struct2Packet(Rsp8031& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("alm_result"        , stBody.alm_result);
		return 0;
	};



	// 报文解析：报文 -> 结构    代理机构查询的请求报文体
	static int Packet2Struct(Req9010& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("is_contain_self"   , stBody.is_contain_self);
		pkt.GetParameterVal("bank_no"           , stBody.bank_no);
		return 0;
	};

	// 报文解析： 结构 -> 报文    代理机构查询的请求报文体
	static int Struct2Packet(Req9010& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("is_contain_self"   , stBody.is_contain_self);
		pkt.AddParameter("bank_no"           , stBody.bank_no);
		return 0;
	};

	// 报文解析：报文 -> 结构    代理机构查询的响应报文体
	static int Packet2Struct(Rsp9010& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("branch_id"         , stBody.branch_id);
		pkt.GetParameterVal("branch_name"       , stBody.branch_name);
		pkt.GetParameterVal("branch_level"      , stBody.branch_level);
		pkt.GetParameterVal("parent_branch_id"  , stBody.parent_branch_id);
		pkt.GetParameterVal("bank_no"           , stBody.bank_no);
		pkt.GetParameterVal("city_code"         , stBody.city_code);
		pkt.GetParameterVal("is_self_bank"      , stBody.is_self_bank);
		pkt.GetParameterVal("htm_branch_info"   , stBody.htm_branch_info);
		return 0;
	};

	// 报文解析： 结构 -> 报文    代理机构查询的响应报文体
	static int Struct2Packet(Rsp9010& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("branch_id"         , stBody.branch_id);
		pkt.AddParameter("branch_name"       , stBody.branch_name);
		pkt.AddParameter("branch_level"      , stBody.branch_level);
		pkt.AddParameter("parent_branch_id"  , stBody.parent_branch_id);
		pkt.AddParameter("bank_no"           , stBody.bank_no);
		pkt.AddParameter("city_code"         , stBody.city_code);
		pkt.AddParameter("is_self_bank"      , stBody.is_self_bank);
		pkt.AddParameter("htm_branch_info"   , stBody.htm_branch_info);
		return 0;
	};



	// 报文解析：报文 -> 结构    系统参数设置请求报文体
	static int Packet2Struct(Req9030& stBody, CTradePacket& pkt)
	{
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("para_id"           , stBody.para_id);
		pkt.GetParameterVal("para_type"         , stBody.para_type);
		pkt.GetParameterVal("para_desc"         , stBody.para_desc);
		pkt.GetParameterVal("para_value"        , stBody.para_value);
		return 0;
	};


	// 报文解析： 结构 -> 报文    系统参数设置请求报文体
	static int Struct2Packet(Req9030& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("para_id"           , stBody.para_id);
		pkt.AddParameter("para_type"         , stBody.para_type);
		pkt.AddParameter("para_desc"         , stBody.para_desc);
		pkt.AddParameter("para_value"        , stBody.para_value);
		return 0;
	};

	// 报文解析：报文 -> 结构    系统参数设置响应报文体
	static int Packet2Struct(Rsp9030& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("para_id"           , stBody.para_id);
		pkt.GetParameterVal("para_type"         , stBody.para_type);
		pkt.GetParameterVal("para_desc"         , stBody.para_desc);
		pkt.GetParameterVal("para_value"        , stBody.para_value);
		pkt.GetParameterVal("htm_para_info"     , stBody.htm_para_info);
		return 0;
	};

	// 报文解析： 结构 -> 报文    系统参数设置响应报文体
	static int Struct2Packet(Rsp9030& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("para_id"           , stBody.para_id);
		pkt.AddParameter("para_type"         , stBody.para_type);
		pkt.AddParameter("para_desc"         , stBody.para_desc);
		pkt.AddParameter("para_value"        , stBody.para_value);
		pkt.AddParameter("htm_para_info"     , stBody.htm_para_info);
		return 0;
	};


	// 报文解析： 结构 -> 报文    码表获取的请求报文体
	static int Struct2Packet(Req4074& stBody, CTradePacket& pkt)
	{
		pkt.AddParameter("oper_flag"         , stBody.oper_flag);
		pkt.AddParameter("start_date"         , stBody.start_date);
		pkt.AddParameter("end_date"         , stBody.end_date);
		pkt.AddParameter("acct_no"         , stBody.acct_no);
		pkt.AddParameter("delegate_type"         , stBody.delegate_type);
		pkt.AddParameter("src_match_no"         , stBody.src_match_no);
		pkt.AddParameter("trigger_status"         , stBody.trigger_status);
		pkt.AddParameter("serial_no"         , stBody.serial_no);
		pkt.AddParameter("exch_type"         , stBody.exch_type);
		pkt.AddParameter("prod_code"         , stBody.prod_code);
		pkt.AddParameter("entr_price"         , stBody.entr_price);
		pkt.AddParameter("entr_amount"         , stBody.entr_amount);
		pkt.AddParameter("entr_price_type"         , stBody.entr_price_type);
		pkt.AddParameter("trigger_condition"         , stBody.trigger_condition);
		pkt.AddParameter("trigger_value_type"         , stBody.trigger_value_type);
		pkt.AddParameter("trigger_value"         , stBody.trigger_value);
		pkt.AddParameter("base_adjust_step"         , stBody.base_adjust_step);
		pkt.AddParameter("entr_adjust_step"         , stBody.entr_adjust_step);

		return 0;
	};

	// 报文解析：报文 -> 结构    码表获取的响应报文体
	static int Packet2Struct(Rsp4074& stBody, CTradePacket& pkt)
	{
		Packet2StructBase(stBody, pkt);
		pkt.GetParameterVal("oper_flag"         , stBody.oper_flag);
		pkt.GetParameterVal("alm_result"         , stBody.alm_result);

		return 0;
	};
};
