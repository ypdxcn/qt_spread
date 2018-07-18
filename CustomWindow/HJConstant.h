#pragma once
#include <QEvent>


#define CONSTANT_B_SHEET_STAT_SUCCESS       "1" //提货状态id：对应的名称：申请成功

#define CONSTANT_BUY_SELL_BUY     "b"   //
#define CONSTANT_BUY_SELL_SELL    "s"   //

#define CONSTANT_B_MARKET_ID_SPOT      "00"   // 交易市场代码：现货市场
#define CONSTANT_B_MARKET_ID_FORWARD   "01"   // 交易市场代码：远期市场
#define CONSTANT_B_MARKET_ID_DEFER     "02"   // 交易市场代码：延期市场

#define CONSTANT_EXCH_CODE_SPOT_BUY                "4011"  // 现货买入
#define CONSTANT_EXCH_CODE_SPOT_SELL               "4012"  // 现货卖出

#define CONSTANT_EXCH_CODE_FORWARD_LONG            "4021"  // 远期开多仓
#define CONSTANT_EXCH_CODE_FORWARD_SHORT           "4022"  // 远期开空仓

#define CONSTANT_EXCH_CODE_DEFER_OPEN_LONG         "4041"  // 延期开多仓/开买
#define CONSTANT_EXCH_CODE_DEFER_OPEN_SHORT        "4042"  // 延期开空仓/开卖
#define CONSTANT_EXCH_CODE_DEFER_COV_LONG          "4043"  // 延期平多仓/平卖
#define CONSTANT_EXCH_CODE_DEFER_COV_SHORT         "4044"  // 延期平空仓/平买
#define CONSTANT_EXCH_CODE_DELIVERY_LONG           "4045"  // 延期收金
#define CONSTANT_EXCH_CODE_DELIVERY_SHORT          "4046"  // 延期交金
#define CONSTANT_EXCH_CODE_MIDD_DELIVERY_LONG      "4047"  // 延期中立仓收金
#define CONSTANT_EXCH_CODE_MIDD_DELIVERY_SHORT     "4048"  // 延期中立仓交金


const QEvent::Type MSG_CREATE_DLG0 = (QEvent::Type)(5000+300);
//报单流水广播
const QEvent::Type MSG_RECV_RTN_SPOT_ORDER = (QEvent::Type)(5000 + 1);
const QEvent::Type MSG_RECV_RSP_SPOT_ORDER = (QEvent::Type)(5000 + 2);
const QEvent::Type MSG_RECV_RTN_FORWARD_ORDER = (QEvent::Type)(5000 + 3);
const QEvent::Type MSG_RECV_RSP_FORWARD_ORDER = (QEvent::Type)(5000 + 4);
const QEvent::Type MSG_RECV_RTN_DEFER_ORDER = (QEvent::Type)(5000 + 5);
const QEvent::Type MSG_RECV_RSP_DEFER_ORDER = (QEvent::Type)(5000 + 6);
const QEvent::Type MSG_RECV_RTN_DDA_ORDER = (QEvent::Type)(5000 + 7);
const QEvent::Type MSG_RECV_RSP_DDA_ORDER = (QEvent::Type)(5000 + 8);
const QEvent::Type MSG_RECV_RTN_MA_ORDER = (QEvent::Type)(5000 + 9);
const QEvent::Type MSG_RECV_RSP_MA_ORDER = (QEvent::Type)(5000 + 10);

//撤单广播
const QEvent::Type MSG_RECV_RTN_SPOT_ORDER_CANCEL = (QEvent::Type)(5000 + 15);
const QEvent::Type MSG_RECV_RSP_SPOT_ORDER_CANCEL = (QEvent::Type)(5000 + 16);
const QEvent::Type MSG_RECV_RTN_FORWARD_ORDER_CANCEL = (QEvent::Type)(5000 + 17);
const QEvent::Type MSG_RECV_RSP_FORWARD_ORDER_CANCEL = (QEvent::Type)(5000 + 18);
const QEvent::Type MSG_DEFER_ORDER_CANCEL = (QEvent::Type)(5000 + 19);
const QEvent::Type MSG_DDA_ORDER_CANCEL = (QEvent::Type)(5000 + 21);
const QEvent::Type MSG_MA_ORDER_CANCEL = (QEvent::Type)(5000 + 23);

//行情广播
const QEvent::Type MSG_RECV_QUOTATION = (QEvent::Type)(5000 + 25);
const QEvent::Type MSG_RECV_SPOT_QUOTATION = (QEvent::Type)(5000 + 26);
const QEvent::Type MSG_RECV_FORWARD_QUOTATION = (QEvent::Type)(5000 + 27);
const QEvent::Type MSG_RECV_DEFER_QUOTATION = (QEvent::Type)(5000 + 28);
const QEvent::Type MSG_RECV_DEFERDELIVERY_QUOTATION = (QEvent::Type)(5000 + 20);
const QEvent::Type MSG_RECV_SYSINIT_CLEARLIST = (QEvent::Type)(5000 + 40);

//报单流水广播
const QEvent::Type MSG_RECV_SPOT_ORDER = (QEvent::Type)(5000 + 41);
const QEvent::Type MSG_RECV_FORWARD_ORDER = (QEvent::Type)(5000 + 42);
const QEvent::Type MSG_RECV_DEFER_ORDER = (QEvent::Type)(5000 + 43);
const QEvent::Type MSG_RECV_DDA_ORDER = (QEvent::Type)(5000 + 44);
const QEvent::Type MSG_RECV_MA_ORDER = (QEvent::Type)(5000 + 45);

//成交流水广播
const QEvent::Type MSG_RECV_RTN_SPOT_MATCH = (QEvent::Type)(5000 + 46);
const QEvent::Type MSG_RECV_RTN_FORWARD_MATCH = (QEvent::Type)(5000 + 47);
const QEvent::Type MSG_RECV_RTN_DEFER_MATCH = (QEvent::Type)(5000 + 48);
const QEvent::Type MSG_RECV_RTN_DDA_MATCH = (QEvent::Type)(5000 + 49);


const QEvent::Type MSG_RECV_ORDER_CANCEL = (QEvent::Type)(5000 + 50);
const QEvent::Type MSG_REFRESH_DATA = (QEvent::Type)(5000 + 51);
const QEvent::Type WM_SHOW_RJD_REPORT = (QEvent::Type)(5000 + 52);
const QEvent::Type WM_REFRESH_DATA = (QEvent::Type)(5000 + 53);
const QEvent::Type WM_SHOW_TD_TO_TRADE = (QEvent::Type)(5000 + 54);
const QEvent::Type WM_SHOW_STORE_TO_TRADE = (QEvent::Type)(5000 + 55);
const QEvent::Type WM_ON_NEW_BOURSE_BULLETIN = (QEvent::Type)(5000 + 56);
const QEvent::Type WM_ON_NEW_MEMBER_BULLETIN = (QEvent::Type)(5000 + 57);
const QEvent::Type WM_ON_RISK_NOTIFY = (QEvent::Type)(5000 + 58);
const QEvent::Type WM_ON_SYS_INIT = (QEvent::Type)(5000 + 59);
const QEvent::Type WM_SHOW_RJD = (QEvent::Type)(5000 + 60);
const QEvent::Type WM_SHOW_RISK_MODULE = (QEvent::Type)(5000 + 61);
const QEvent::Type WM_SHOW_UNREAD_BULLETINS = (QEvent::Type)(5000 + 62);
const QEvent::Type WM_RISK_DEGREE_CHANGE = (QEvent::Type)(5000 + 63);
const QEvent::Type WM_REFRESH_CUSTOM_INFO = (QEvent::Type)(5000 + 64);
const QEvent::Type WM_MENU_YQPCSHS = (QEvent::Type)(5000 + 65);
const QEvent::Type WM_MENU_TRANSFER = (QEvent::Type)(5000 + 66);
const QEvent::Type WM_MENU_TAKEGOOD = (QEvent::Type)(5000 + 67);
const QEvent::Type WM_MENU_MODIFY_LOGIN_PSW = (QEvent::Type)(5000 + 68);
const QEvent::Type WM_MENU_MODIFY_FUND_PSW = (QEvent::Type)(5000 + 69);
const QEvent::Type WM_MENU_MENBER_MSG = (QEvent::Type)(5000 + 70);


const QEvent::Type WM_MENU_ENTR_FLOW = (QEvent::Type)(5000 + 71);
const QEvent::Type WM_MENU_MATCH_FLOW = (QEvent::Type)(5000 + 72);
const QEvent::Type WM_MENU_DEFER_POSI_DETAIL = (QEvent::Type)(5000 + 73);
const QEvent::Type WM_MENU_FUND_AUTO_IN_OUT_FLOW = (QEvent::Type)(5000 + 74);
const QEvent::Type WM_MENU_ENTR_FLOW_HIS = (QEvent::Type)(5000 + 75);
const QEvent::Type WM_MENU_MATCH_FLOW_HIS = (QEvent::Type)(5000 + 76);
const QEvent::Type WM_MENU_DEFER_POSI_HIS = (QEvent::Type)(5000 + 77);
const QEvent::Type WM_MENU_STORAGE_HIS = (QEvent::Type)(5000 + 78);
const QEvent::Type WM_MENU_FUND_HIS = (QEvent::Type)(5000 + 79);
const QEvent::Type WM_MENU_FUND_FLOW = (QEvent::Type)(5000 + 80);
const QEvent::Type WM_MENU_FARE_DETAIL = (QEvent::Type)(5000 + 81);
const QEvent::Type WM_MENU_RJDCHX = (QEvent::Type)(5000 + 82);
const QEvent::Type WM_MENU_YJDCHX = (QEvent::Type)(5000 + 83);
const QEvent::Type WM_MENU_QPDCHX = (QEvent::Type)(5000 + 84);
const QEvent::Type WM_MENU_DROPOUT = (QEvent::Type)(5000 + 85);
const QEvent::Type WM_MENU_CLEAR_DATA = (QEvent::Type)(5000 + 86);
const QEvent::Type WM_MENU_SET_LIST = (QEvent::Type)(5000 + 87);
const QEvent::Type WM_MENU_DEFERDELIVERY = (QEvent::Type)(5000 + 88);
const QEvent::Type WM_MENU_PRODSTATE = (QEvent::Type)(5000 + 89);


const QEvent::Type WM_MENU_INVERT_COMMIT = (QEvent::Type)(5000 + 90);
const QEvent::Type WM_MENU_SET_SHOW_INSID = (QEvent::Type)(5000 + 91);
const QEvent::Type WM_MENU_INST_TRIGGER_INFO = (QEvent::Type)(5000 + 92);
const QEvent::Type WM_MENU_OPTION = (QEvent::Type)(5000 + 93);
const QEvent::Type WM_MENU_FLOAT_TRADE = (QEvent::Type)(5000 + 94);
const QEvent::Type WM_UPDATE_LIST = (QEvent::Type)(5000 + 95);
const QEvent::Type WM_UPDATE_DATA_STATUS = (QEvent::Type)(5000 + 96);
const QEvent::Type WM_MENU_CUST_CURR_DATA = (QEvent::Type)(5000 + 97);
const QEvent::Type WM_PREORDER_CHANGE = (QEvent::Type)(5000 + 100);
const QEvent::Type WM_INST_STATE_UPDATE = (QEvent::Type)(5000 + 101);
const QEvent::Type WM_TAB_SHOW = (QEvent::Type)(5000 + 102);

const QEvent::Type WM_UPDATE_ORDER = (QEvent::Type)(5000 + 103);
const QEvent::Type WM_UPDATE_ENTR_FLOW = (QEvent::Type)(5000 + 104);
const QEvent::Type WM_UPDATE_DEFER_POSI = (QEvent::Type)(5000 + 105);
const QEvent::Type WM_UPDATE_STORE = (QEvent::Type)(5000 + 106);
const QEvent::Type WM_UPDATE_FUN = (QEvent::Type)(5000 + 107);
const QEvent::Type WM_UPDATE_MATCH_FLOW = (QEvent::Type)(5000 + 108);
const QEvent::Type WM_UPDATE_DEFER_POSI_S = (QEvent::Type)(5000 + 109);
const QEvent::Type WM_UPDATE_STORE_S = (QEvent::Type)(5000 + 110);
const QEvent::Type WM_UPDATE_FUN_S = (QEvent::Type)(5000 + 111);
const QEvent::Type WM_UPDATE_QUOTATION_LIST = (QEvent::Type)(5000 + 112);
const QEvent::Type WM_MENU_OPPOSITE_ORDER = (QEvent::Type)(5000 + 113);
const QEvent::Type WM_MENU_MODIFY_ORDER = (QEvent::Type)(5000 + 114);
const QEvent::Type WM_MENU_RUN_INTRODUCTION = (QEvent::Type)(5000 + 115);
const QEvent::Type WM_CHANGE_TRADE_SHOW_MODE = (QEvent::Type)(5000 + 116);
const QEvent::Type WM_CHANGE_DEFER_SHOW_MODE = (QEvent::Type)(5000 + 117);
const QEvent::Type WM_CHANGE_TAB_SEL = (QEvent::Type)(5000 + 118);
const QEvent::Type WM_TAB_ITEM_CHANGE = (QEvent::Type)(5000 + 119);
const QEvent::Type WM_SHOW_ORDER_TIPS = (QEvent::Type)(5000 + 120);//报单成功与否

const QEvent::Type WM_PLAY_TIPS_MUSIC = (QEvent::Type)(5000 + 121);
const QEvent::Type WM_TEST = (QEvent::Type)(5000 + 122);
const QEvent::Type WM_SYS_STAT_CHANGE = (QEvent::Type)(5000 + 123);
const QEvent::Type WM_HQ_STATE_CHANGE = (QEvent::Type)(5000 + 124);
const QEvent::Type WM_MENU_REQ_PCSS = (QEvent::Type)(5000 + 125);
const QEvent::Type WM_MENU_LOCKSCREEN = (QEvent::Type)(5000 + 126);
const QEvent::Type WM_MENU_SVRPREORDER = (QEvent::Type)(5000 + 127);
const QEvent::Type WM_WIND_VANE = (QEvent::Type)(5000 + 128);
const QEvent::Type WM_UPDATE_WINDVANE_SHOW = (QEvent::Type)(5000 + 129);
const QEvent::Type WM_MENU_SHOW_CUST_INFO = (QEvent::Type)(5000 + 130);
const QEvent::Type WM_SHOW_MORE_LOGIN_TIPS = (QEvent::Type)(5000 + 131);
const QEvent::Type WM_ADD_PREORDER_CONFIRM = (QEvent::Type)(5000 + 132);//保单执行成交情况

const QEvent::Type WM_SHOW_ORDER_RESULT_TIPS = (QEvent::Type)(5000 + 133);
const QEvent::Type WM_SHOW_ORDER_CANCEL_TIPS = (QEvent::Type)(5000 + 134);//测单成功与否
const QEvent::Type MSG_RECV_FUTURE_QUOTATION = (QEvent::Type)(5000 + 135);//期货行情
const QEvent::Type MSG_RECV_SPREAD_QUOTATION = (QEvent::Type)(5000 + 136);//套利行情

const QEvent::Type WM_DLG_MESSAGE_FINISH = (QEvent::Type)(5000 + 1000);
const QEvent::Type WM_ON_LOGIN_SUC = (QEvent::Type)(5000 + 1001);
const QEvent::Type WM_REFRESH_FUND = (QEvent::Type)(5000 + 1002);
const QEvent::Type WM_FORCE_LOGOUT = (QEvent::Type)(5000 + 1003);
const QEvent::Type WM_SER_PREORDER_REFRESH = (QEvent::Type)(5000 + 1004);
const QEvent::Type WM_KEYBOARDDestroy = (QEvent::Type)(5000 + 1005);
const QEvent::Type WM_SURPLUS_CHANGE = (QEvent::Type)(5000 + 1006);
const QEvent::Type WM_TOTAL_SURPLUS_CHANGE = (QEvent::Type)(5000 + 1007);

const QEvent::Type WM_REFRESH_POSI			   = (QEvent::Type)(5000 + 1008);
const QEvent::Type WM_REFRESH_STORE			   = (QEvent::Type)(5000 + 1009);
const QEvent::Type WM_CUSTOM_INFO_CHANGE       = (QEvent::Type)(5000 + 1010);
const QEvent::Type WM_CUS_BASE_INFO			   = (QEvent::Type)(5000 + 1011);
const QEvent::Type WM_ADD_STOPPL_ORDER_CONFIRM = (QEvent::Type)(5000 + 1012);
const QEvent::Type WM_SelectTableRow		   = (QEvent::Type)(5000 + 1013);

const QEvent::Type WM_MYTASK = (QEvent::Type)(5000 + 1014);
const QEvent::Type WM_MY_UI_TASHK = (QEvent::Type)(5000 + 1014);



#include <QVariant>
typedef struct _TableRow
{
	QVariant    arrVar[32];
	short     arrColor[32];
    int       nDecimal;
	_TableRow()
	{
		memset(arrVar, 0, sizeof(QVariant) * 32);
		memset(arrColor, 0, sizeof(short) * 32);
        nDecimal = 0;
	}
}TableRow;


enum SORT_DIRECTOR
{
	SORT_UP = 0,
	SORT_DOWN,
	SOFT_NO
};

