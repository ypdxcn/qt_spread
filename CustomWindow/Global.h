#pragma once

#include <QString>
using namespace std;
#include "ArrayListMsg.h"
#include "HJDef.h"


#define CONSTANT_BEGIN_DATE_BIGGER          "查询的开始日期不能大于结束日期"  
#define CONSTANT_QUERY_NO_RECORD            "当前查询结果为空！"
#define CONSTANT_CONNECT_SERVER_FAIL_TIPS   "连接服务器失败！"
#define CONSTANT_SYSTEM_INI_NAME            "Config.ini"
#define CONSTANT_USER_INI_NAME              "config_user.ini"
#define CONSTANT_DATA_INI_NAME              "ServerData.ini"
#define CONSTANT_YES_NO_YES                 "1"
#define CONSTANT_YES_NO_NO                  "0"

// 定义所有的提示信息常量

#define TIP_NO_1	"撤销成功"
#define TIP_NO_2	"当前没有选中项！"
#define TIP_NO_3	"刷新间隔时间不正确"
#define TIP_NO_4	"自动锁屏时间不能为空！"
#define TIP_NO_5	"自动锁屏时间不能为空！"
#define TIP_NO_6	"注意：止盈止损单将发往服务器，由服务器触发！"
#define TIP_NO_7	"注意：止盈止损单将保存在本机，由本机触发！"
#define TIP_NO_8	"连接服务器失败！"
#define TIP_NO_9	"Failed to create view window"
#define TIP_NO_10	"打印设备初始化失败！"
#define TIP_NO_11	"日结单确认成功！"
#define TIP_NO_12	"认证失败"
#define TIP_NO_13	"登陆失败"
#define TIP_NO_14	"合约代码没有选中"
#define TIP_NO_15	"平仓类型没有选中"
#define TIP_NO_16	"价格不能为空"
#define TIP_NO_17	"数量不能为空"
#define TIP_NO_18	"地址不能为空"
#define TIP_NO_19	"平仓失败，放弃执行反向开仓！"
#define TIP_NO_20	"查询的开始日期不能大于结束日期"
#define TIP_NO_21	"当前查询结果为空！"
#define TIP_NO_22	"密码只能为6位的数字"
#define TIP_NO_23	"两次输入的密码不一致！"
#define TIP_NO_24	"新密码不能和旧的密码相同！"
#define TIP_NO_25	"密码不能为连续的数字！"
#define TIP_NO_26	"密码不能有连续三位相同的数字！"
#define TIP_NO_27	"修改密码成功！"
#define TIP_NO_28	"已经是首页"
#define TIP_NO_29	"已经是末页"
#define TIP_NO_30	"选择不能为空，请至少勾选一项！"
#define TIP_NO_31	"请至少选中一项！"
#define TIP_NO_32	"该项是必选项，不能去掉"
#define TIP_NO_33	"当前不是提货日，不能提货"
#define TIP_NO_34	"已向交易所发送提货申请，您可以点击查询按钮重新查询提货申请列表！"
#define TIP_NO_35	"请选择提货城市"
#define TIP_NO_36	"请选择提货仓库"
#define TIP_NO_37	"请选择提货人"
#define TIP_NO_38	"请选择证件类型"
#define TIP_NO_39	"请选择交割品种"
#define TIP_NO_40	"请选择提货日"
#define TIP_NO_41	"输入提货重量小于最小提货重量"
#define TIP_NO_42	"输入提货重量不满足步长要求"
#define TIP_NO_43	"请输入证件编号"
#define TIP_NO_44	"密码不能为空"
#define TIP_NO_45	"密码必须为数字"
#define TIP_NO_46	"确认密码不能为空"
#define TIP_NO_47	"确认密码必须为数字"
#define TIP_NO_48	"两次输入密码不一致!"
#define TIP_NO_49	"加载仓库信息失败"
#define TIP_NO_50	"解析配置文件中城市信息错误！"
#define TIP_NO_51	"已向交易所发送撤单申请，您可点击查询按钮重新查询提货申请列表！"
#define TIP_NO_52	"请先选中项"
#define TIP_NO_53	"该提货单没有申请成功！不能打印"
#define TIP_NO_54	"解析配置文件中仓库信息错误！"
#define TIP_NO_55	"已向交易所发送出库申请，您可点击查询按钮重新查询提货申请列表"
#define TIP_NO_56	"改单失败！原报单已经被撤销，如需恢复，请重新在报单窗口下单！"
#define TIP_NO_57	"改单成功！"
#define TIP_NO_58	"由于撤单不成功，所以无法改单！"
#define TIP_NO_59	"创建Excel服务失败！"
#define TIP_NO_60	"市场无买卖价, 无法发送报单!"
#define TIP_NO_61	"当前项不处于未发送状态！"
#define TIP_NO_62	"添加服务器条件单成功！"
#define TIP_NO_63	"价格方式下拉框没有选中"
#define TIP_NO_64	"注意：条件单将发往服务器，由服务器触发！"
#define TIP_NO_65	"注意：条件单将保存在本机，由本机触发！"
#define TIP_NO_66	"请选择合约代码！"
#define TIP_NO_67	"委托手数不能为空或者0！"
#define TIP_NO_68	"委托手数不是整数！"
#define TIP_NO_69	"请选择交易方向！"
#define TIP_NO_70	"请输入委托价格！"
#define TIP_NO_71	"获取风险度失败！"
#define TIP_NO_72	"请选择报单!"
#define TIP_NO_73	"报表窗口已经打开！"
#define TIP_NO_74	"开始时间不能大于结束时间"
#define TIP_NO_75	"转账成功！"
#define TIP_NO_76	"发生金额不能为空"
#define TIP_NO_77	"发生金额输入有误"
#define TIP_NO_78	"资金密码不能为空"
#define TIP_NO_79	"网络数据传输失败，请重试!"
#define TIP_NO_80	"确定删除?"
#define TIP_NO_81	"确认修改通讯设置?"
#define TIP_NO_82	"确实要申请提货吗?"
#define TIP_NO_83	"确实要撤销申请吗?"
#define TIP_NO_84	"确实要出库吗?"
#define TIP_NO_85	"未确认日结单，请确认日结单后再下单。是否现在确认日结单？"
#define TIP_NO_86	"确认移动报单界面？"
#define TIP_NO_87	"关闭此提示可能造成意外报单，确认下次不提示？"
#define TIP_NO_88	"确认清除选中数据？"
#define TIP_NO_89	"你需要修改密码才能登陆，确定修改？"
#define TIP_NO_90	"验证码错误，请重新输入"
#define TIP_NO_91	"系统中有未成交的开仓委托报单，退出系统后，若该报单成交，系统无法生成本地止损止盈条件单。确认退出快金系统吗？"
#define TIP_NO_92	"系统中有未成交的开仓委托报单，退出系统后，若该报单成交，系统无法生成自动止损止盈条件单并存放于服务器。确认退出快金系统吗？"
#define TIP_NO_93	"当前存在未发送的本地预埋单，关闭交易端后将被清除，确认退出"
#define TIP_NO_94	"确认退出交易？"



#define RSP_SUCCESS_CODE "00000000"

#define QUOTATION_INI_SPLITER1              "｜"
#define QUOTATION_INI_SPLITER2              "∧"

#define CONSTANT_LOGIN_TYPE_CODE            "1" // 黄金交易编码登录
#define CONSTANT_LOGIN_TYPE_BANKNO          "2" // 银行账号登录

#define TRADE_LIST_HEIGHT                   21   // 交易界面的list控件默认的高度

#define Interface_Style_Classic             0
const QString g_sFinalDate  = "20140601";



class CGlobal
{
public:
	CGlobal(void);
	~CGlobal(void);

public:
	bool IsTipsAddPreOrder();

public:
	QString m_strUserID;            // 用户名（用户ID）
	QString m_strPwdMD5;            // MD5加密后的用户密码
	QString m_strUserName;          // 客户简称
	QString m_strSessionID;         // 会话ID
	QString m_strSessionKey;        // 会话密钥
	QString m_strAcctType;          // 账户类型
	
	int m_strNo_Affirm_Rpt_Num;    // 日结单未确定天数
	QString m_strExchDate;          // 系统交易日
	QString m_strLast_Exch_Date;    // 上一交易日
	QString m_strSysState;          // 系统状态
	QString m_strSysDate;           // 系统日期

	QString m_strBranchName;        // 客户所属代理机构名称
	QString m_strBankNo;            // 银行账号
	QString m_strNetAgent;          // 网络运营商id
	QString m_strNetEnvionment;     // 网络环境id 

#if (defined _VERSION_JSZX) || (defined _VERSION_ZHLHY)
	bool m_bAutoTransfer;          // 是否采用自动出入金
#endif
	
	bool m_bCommitLeft;
	int m_iInterfaceStyle;
	bool m_bAskConfirm;
	bool m_bAlwaysShowUpPanel;
	bool m_bInsTriggerInfo;
	bool m_bShowQuotation;
	int m_iStorePos;
	int m_iPosiPos;

	double m_dQTFactor;            // 行情报文中的数据因子
	bool   m_bRiskMode1;
	QString m_strSystemPath;       // 系统文件的路径，即exe文件的路径
	
	ArrayListMsg m_almTake_Man_List;        // 客户提货人列表
	ArrayListMsg m_almNoRead_Bulletin_List; // 未读公告
	ArrayListMsg m_alm_take_sheet_date;     // 最近提货日

	bool m_bLocked; // 锁屏标志
	unsigned int m_TimeOutScreen; // 锁屏超时时间
	bool m_bLogining;  // 正在登陆，即登陆框弹出状态
	bool m_bConfirmUnorder; // 撤单确认
	bool m_bChiCang_Click; // 双击持仓、库存直接报单，单击更新下单面板的信息
	bool m_bChiCang_Dbclick; // 双击标志位
	bool m_bAlwaysShowBS5; // 保证买卖5可见
	bool m_bChangeBS; // 选择5档行情的买卖的时候更改买卖方向

	bool m_bOppCovConfirm; // 市价反手平仓确认
	bool m_bShowDeferMode; // 是否显示延期的方式行
	bool m_bCovPosiConfirm;
	bool m_bPreOrderConfirm; // 预埋单双击时确认
	bool m_bPosiOFConfirm; // 报单时超过当前持仓量提示

	bool m_bTipsAskSus;
	bool m_bTipsAskFail;
	bool m_bTipsOrderMatch;
	bool m_bTipsCancelSus;
	bool m_bTipsCancelFail;
	bool m_bTipsAddPreOrder; // 添加预埋单提示

	QString m_csSoundAskSus;
	QString m_csSoundAskFail;
	QString m_csSoundOrderMatch;
	QString m_csSoundCancelSus;
	QString m_csSoundCancelFail;

	bool m_bIsTrial;

	bool m_bShowCusInfo; // 是否在顶部显示客户信息

	// 代理相关
	int m_nProxyType; // 代理类型 0=无代理，1=http11,2=sock4,3=sock5
	QString m_sProxyIP; // 代理服务器地址
	int m_nProxyPort; // 代理服务器端口
	QString m_sProxyUser,m_sProxyPassword; // 用户名、密码
    int m_nUseIE; // added by Jerry Lee, 2013-4-1, 增加使用IE设置

	QString m_sTargetIP; //当前使用的目标IP
	int m_nTargetPort;   //当前使用的目标Port
	// 代理相关结束

	bool m_bAutoMode;

	QString m_csDataIniPath;
	QString m_csSysIniPath;

	ELoginType m_eRunMode;
	bool m_bShowStatusBar;

	bool m_bUseSerPreOrder;

	bool m_bShowLastLoginInfo;

	bool m_bEnableIniFile;//ini文件是否可修改
	bool m_bForceTerminate;//add lqh 5.13创建目录失败，强制退出

	int  m_ndpiX;         //DPIX
	int  m_ndpiY;         //DPIY

    // added by Jerry Lee, 2013-02-28
    //void ChangeUserIniFile(LPCTSTR lpszUserCode);
public:
	bool IsRunSingle();

    void SetCommonPath();

	// 获取用户配置文件的路径
	QString GetUserIniPath();

	// 获取系统配置文件的路径
	const QString &GetSystemIniPath();

	//
	const QString &GetDataIniPath();

	// 
	QString GetUserDataIniPath();

	// 根据list配置文件的名字获取其对应的list的配置文件的路径
	QString GetListIniPath(const QString & csFileName);

	// 获取系统目录的路径
	QString& GetSystemPath();

	void WriteLog( QString csLog );
	//bool TestCreateFile( QString strFilePath );
	//bool ShowTips();
private:
    QString m_csUserIniFile; // added by Jerry Lee, 2013-02-28

	
};

extern CGlobal  g_Global;




