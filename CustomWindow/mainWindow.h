
#ifndef mainWindow_H
#define mainWindow_H

#include <QtWidgets/QMainWindow>

#include "ui_mainWindow.h"

#include "WindowTitle.h"
#include "BroadcastPacketAndStruct.h"
#include "StDef.h"
#include <QTableWidget>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include "frmDeepQuoteDlg.h"
#include "frmUnlockDlg.h"
#include "HJConstant.h"
#include <QStandardItemModel>
#include <QSplitter>
#include "ConfigImpl.h"
#include <thread>
#include <functional>
#include "ui_mainwindow.h"
#include <unordered_map>
#include "MarketTableView_qh.h"

#include "Theron/actor.h"
#include "Theron/Framework.h"



//#include "TraderCpMgr.h"
//【报单流水】字段值
const int constOrderNo = 10;
const int constOrderType = 6;
const int constInstId = 0;//合约代码
const int constExchType = 1;
const int constPrice = 2;
const int constAmount = 3;
const int constRemainAmount = 4;
const int constState = 5;
const int constApllyTime = 7;
const int constRspMsg = 8;
const int constLocalOrderNo = 9;



//【成交流水】字段值
const int constMatchNo = 0;
const int constMatchOrderNo = 1;
const int constMatchInstId = 2;
const int constMatchExchType = 3;
const int constMatchPrice = 4;
const int constMatchAmount = 5;
const int constMatchTime = 6;



struct ProdCodeInfo;
class QVBoxLayout;
class QString;
struct OrderInfo;
struct PreOrderStruct;
struct DeferPosiInfo;
//struct Rsp1020;

struct ListDataInfo
{
	string sLocalOrderNo;
	string sExchName;
	QString csPrice;
	ListDataInfo(const string &sLocalOrderNo2, const string &sExchName2, const QString &csPrice2) : sLocalOrderNo(sLocalOrderNo2), sExchName(sExchName2), csPrice(csPrice2) {};
};









class mainWindow : public QMainWindow,public CUnlockListener
{	

	Q_OBJECT
	

public:
	mainWindow(QWidget *parent = 0);
	~mainWindow();

	//DECLARE_MSG_MAP()

	//void OnTest(lparam, wparam);
	//void OnData(lparam, wparam);

	void customEvent(QEvent *e);
	//锁屏回报
	virtual void OnSuccess();

	virtual void OnFailure();


	bool IsLoginQH();
	void SetLoginQHStatus(bool bstate);
	void InitUI();
	//加载内存数据
	void InitAllData();
	void InitTableContextMenu();
	void InitSubscribe();
	void SetQuotation(QUOTATION* wparam);
	void SetOrderQuotation(QUOTATION *quote);

	//响应菜单部分功能
	void ShowAccountBar();

	void RefreshStatusLabelUI();
	//刷新账户信息
	void RefreshAccount();
	//加载数据:成交流水/交易流水
	void RefreshOrder();
	void RefreshMatch();

	//加载数据:持仓/库存/资金
	void RefreshPosi();
	void RefreshStore();
	void RefreshCapital();

	//处理持仓表格行字段数据
	QString GetPosiValue(const DeferPosiInfo &stDeferPosiInfo, int iPos);
	//处理库存表格行字段数据
	QString GetStoreValue(const StoreInfo &stStoreInfo, int iPos);
	//处理资金表格行字段数据
	QString GetFundValue(const void *rsp, int iPos);
	//处理成交表格行字段数据
	QString GetMatchValue(const void *rsp, int iPos);


	bool IsForwardMarket(const QString &csInsID);
	bool GetOrderInfo(int iRow, OrderInfo &info, bool bGetPrice);
	QString GetBuySellID(int nPos);
	QString GetTips(int index);

	void ShowPosiDataToList();
	void DeleteInvalidPosi(const unordered_map<string, DeferPosi> &mapDeferPosi);
	void HandleOnePosi(const string& sInst, const DeferPosiInfo &stPosiDetails, bool bLong);
	int  GetPosInList(string sProdeCode, bool bLong);
	void GetPosInList(string sProdeCode, int &iPos, bool bLong, bool &bExist);
	int  GetPosToInsert(int iPos, bool bLong);
	void DealOneLine(const string &sProdCode, const DeferPosiInfo &stDeferPosiInfo, int iPos, bool bLong);
	
	//持仓
	///@rapam  blong为多，空 
	//void UpdatePosiInfo(QString &csProdCode, bool  bLong);//更新持仓的中间状态(按下“平仓”按钮，新状态，报单)

	//预埋单  处理函数
	int  OnRecvPreOrderChange(int wParam, int lParam);
	void DealOneLine_preorder(int iLinePos, const PreOrderStruct &stPreOrder);
	int  GetListPosByOrderID(int iOrderID);
	//报单流水
	void HandleSpotOrder(void *  wparam);//返回报单数据
	void HandleDeferOrder(void *  wparam);
	void HandleForwardOrder(void *  wparam);
	void HandleDDAOrder(void *  wparam);
	void HandleMAOrder(void *  wparam);
	void HandleOrderCancel(void * wParam);
	int  CheckEntrFlowExist(const string &localorderno);
	void UpdateEntrFlow(int index, const OrderBase* stOrderBase);
	void InsertNewEntrFlow(const string &&LocalOrderNo, const string&& OrderNo, const string &&InstID, QString&& ExchType, const QString &Price, int Amount, int RemainAmount, const QString &State, string ApplyTime, string RspMsg);
	void DeleteEntrFlowByLocalOrderNo(const string &sNo);
	QString TranslateRspMsg(const QString &sMsg);

	//撤单，改单逻辑函数
	//inline const ListDataInfo* GetListItemData(int iRow);
	const OrderBase* GetOrderBaseInfo(int iRow);
	void DealButtonCancel(bool bAll = false);//处理逻辑函数
	void onModifyOrdedr();

	//成交流水
	bool CheckKeyExist(string iMatchNo, string iOrderNo);
	void HandleRtnSpotMatch(void * wParam);
	void HandleRtnForwardMatch(void * wParam);
	void HandleRtnDeferMatch(void * wParam);
	void HandleRtnDDAMatch(void * wParam);
	void InsertNewMatchFlow(string && MatchNo, string &&OrderNo, string &&InstID,
							string &&ExchType, QString Price, int Volume,
							string &&MatchTime);

	//库存
	int  GetRowInList_store(const string &sProdCode);
	void DealOneLine_store(const string &sProdCode, const StoreInfo &stStoreInfo, int iRow);
	void OnInstStateUpdate(void * wparam);//交易状态改变

	//主菜单的处理函数
	void handleOneSlot();
	void handleTwoSlot();
	void handleThreeSlot();
	void handle2OneSlot();
	void handle2TwoSlot();
	void handle2ThreeSlot();
	//void handle2FourSlot();
	//void handle2FiveSlot();
	void handle3OneSlot();
	void handle3TwoSlot();
	void handle3ThreeSlot();
	void handle3FourSlot();
	void handle3FiveSlot();
	void handle3Sixsub1Slot();
	void handle3Sixsub2Slot();
	void handle3Sixsub3Slot();
	void handle3Sixsub4Slot();
	void handle3Sevensub1Slot();
	void handle3Sevensub2Slot();
	void handle3Sevensub3Slot();
	void handle3Sevensub4Slot();
	void handle3Sevensub5Slot();
	void handle3Sevensub6Slot();
	void handle3Sevensub7Slot();
	void handle4OneSlot();
	void handle4TwoSlot();
	void handle5OneSlot();

	void RefreshMultiAccount();
	void cleanWidgetContent();

public slots:
	void handle2FourSlot();
	void handle2FiveSlot();
    //右键响应菜单
	void show_contextmenuPosi(const QPoint& pos);
	void show_contextmenuStore(const QPoint& pos);
	void show_contextmenuCapital(const QPoint& pos);

	void show_contextmenuOrderReturn(const QPoint& pos);
	//右键功能响应
	void actionPosiOneSlot();
	void actionPosiTwoSlot();
	void actionPosiThreeSlot();

	void actionStoreOneSlot();
	void actionCapitalOneSlot();

	void actionOrderReturnOneSlot();//改单

	void titleButtonClick(WindowTitle::ButtonStatus status);
	void slotChangeQuote(const QString &string);
	void ResetLayoutSlot();

	void OnBnClickedButtonCov();//平仓
	void OnBnClickedButtonOpp();//反手
	
	void OnBnClickedButtonCancel();//撤单
	void OnBnClickedButtonCancelAll();//撤掉全部
	void OnBnClickedButtonModify();//改单

	void OnBnClickedButtonPreDelete();//删除
	void OnBnClickedButtonPreSend();//立即发送
	void OnBnClickedButtonPreClear();//清空已发送

	void slotSendOrder();
	void slotPreOrder();
	void slotChangeOrderType();
	void slotChangeOrderType2();
	void slotChangeOrderType3();
    void slotChangeOpenCloseType();
	void slotChangeOpenCloseType2();

	void slotChangeBuyType();
	void slotChangeSellType();
	void slotLabelClicked();

	void slotPosiTableDoubleClick(const QModelIndex & QMindex);
	void showPosiOperationDetails(const QModelIndex &current, const QModelIndex &previous);
	void showStoreOperationDetails(const QModelIndex &current, const QModelIndex &previous);
	
	void slotDeepQuote();
	void slotCloseDeepQuoteDlg();
	void slotChangeAccount();
	void slotLoginAccount(const QString & str);
protected:
	virtual  void paintEvent(QPaintEvent *event);
	
	void    SwitchMode(uint && uMode);//20180417  rvalue
	bool    CheckInput();//报单检查
	QString GetCurExchCodeId();
	bool    GetCurProdCodeInfo(ProdCodeInfo &info);
	void    ShowCurStore(const ProdCodeInfo &info);
	int     GetUsefulStore(const string &sProdCode);
	void    CalculateMax();
	void    GetCangWeiT5(const string& prodCode, QString& usefullong, QString& usefulshort);
	void    GetCangWeiTd(const string& prodCode, QString& usefullong, QString& usefulshort);
	void    ShowPriceInfo(const string &&sInsID, const QUOTATION&& qt);
	void    ShowLastPrice(const QUOTATION &qt);
    uint    GetLastPrice(const QUOTATION &qt);
    uint    GetLastPrice2(const QUOTATION &qt);
	void    AskModeChange();

private:
	void initParam();

	void loadPosiUI();
	void loadStoreUI();
	void loadCapitalUI();

	void InitAccountUI();
	void InitTabViewUI();
	void InitOrderUI();
	void InitOrderReturnUI();
	void InitLocalDBUI();
	void InitTransReturnUI();
	void InitStatusLabelUI();
	void InitSplitterUI();

	void InitDeepQuoteUI();
	void disableKaipingOrderCtl();
	void enableKaiPingOrderCtl();
	void disableYqZlJsOrderCtl();
	void enableYqZlJsOrderCtl();
	void changToClassicLayout();
	void changToOldLayout();

	void createOrderUI();
	void createQuoteUI();
	void createSpreadOrderUI();
	void createSpreadQuoteUI();
	void createSpreadProdSetUI();

	void ShowDeepQuoteUI();
	void InitNormalQuoteUI();
	void ShowNormalQuoteUI();
	void InitSpreadNormalQuoteUI();

public:
	Ui::mainWindowClass ui;

	bool bLoadAllData;
	int  classiclayout;//classiclayout=0 原始布局  =11 经典布局

	frmDeepQuoteDlg *deepQuoteDlg;//深度行情非模对话框
	bool    showDeepQuote;
	WindowTitle* m_title;

	QVBoxLayout* m_layout;
	/////////////////////////////////////////////////////////////////////////////////////////////
	//元素打包保存变量
	QWidget* accountBarWidget;//总资金窗口

	QTabWidget *TabListQuotation;//报价TAB
	MarketTableView_qh *tableViewMarketPrice_spread;


	QTabWidget *TabOrderBoard;//下单板

	QWidget    *orderWidget;//下单窗口
	QWidget    *quoteWidget;//报价窗口
	QHBoxLayout *OrderBoardMainLayout;
	//下单板区域
	QVBoxLayout * hLayout_left;
	//报价五档的行情
	QVBoxLayout * hLayout_right;

	QWidget *leftMainWin;
	//套利下单版-----------------------------------------------
	QWidget    *spreadOrderWidget;//下单窗口
	QWidget    *spreadQuoteWidget;//报价窗口

	QHBoxLayout *spreadOrderBoardMainLayout;
	//下单板区域
	QVBoxLayout * spreadHLayout_left;
	//报价五档的行情
	QVBoxLayout * spreadHLayout_right;
	QWidget * spreadLeftMainWin;
	//----------------------------------------------------------
	//套利合约设置
	QWidget    *spreadProdWidget;



	//----------------------------------------------------------
	//报单流水
	QPushButton * quoteButton;
	QTabWidget  * TabRight;

	QTableView *            orderReturnTable;	
	QWidget *               orderReturnWidget;
    QStandardItemModel *	tableModel_order;

	QTableView *preorderReturnTable;//预埋单表
	QWidget    *preOrderReturnWidget;//预埋单
    QStandardItemModel *	tableModel_preorder;


	QTableView *spreadTable;
	QWidget    *spreadRunWidget;
	QStandardItemModel *	tableModel_spread;

	
		
	//持仓/库存/资金
	QTabWidget* tabWidget_own;

	QTableView *  Tabview_1;
	QWidget *     posiWidget;
	QStandardItemModel * tableModel_posi;

	QTableView *  Tabview_2 ;
	QWidget *     storeWidget;
	QStandardItemModel *tableModel_store;

	QTableView *  Tabview_3 ;
	QWidget *   capitalWidget;
	QStandardItemModel *tableModel_capital;
	//交易成交流水
	QTabWidget *  TabTrans;
	//QWidget *   transWidget;
	QStandardItemModel *tableModel_match;

	QTableView *TabView_trans;

	QSplitter  * m_pSplitterOrderAndOrderReturn;
	QSplitter  * m_pSplitterLocalDBAndTransReturn;
	QSplitter  * m_pSplitterAllTithoutLabel;

	QStatusBar *statusbar;

	//报单参数
	int  type;//延期 0  ，中立仓 1，交收 2
	int  openclose;//open:0    close:1
	int  bullsell;//buy:0   sell:1


	// 让界面线程来执行的数据更新任务
	template<class F, class... Args>
	auto doUpdateUiTask(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;

	std::queue< std::function<void()> > tasks;//任务队列
	std::mutex      queue_mutex;	// 同步信号

	int  OnUITask(void* wparam, void* lparam);

private:
	//期货是否有账户登陆
	bool  m_loginQh;

	int m_uiLockCount;
	//定义状态值
	bool  showCapital;

	//持仓区的右键菜单
	QMenu *menuPosi;
	QMenu *menuStore;//未用
	QMenu *menuCapital;//未用

	QAction *m_actionPosiOne;//反手平仓
	QAction *m_actionPosiTwo;//平仓试算
	QAction *m_actionPosiThree;//刷新持仓信息
	QAction *m_actionStoreOne;//
	QAction *m_actionCapitalOne;//


	 //报单回报区域的右键菜单
	QMenu    *menuOrderReturn;
	QAction *m_actionOrderOne;//反手平仓

	CConfigImpl*		    m_pConfig;
	std::mutex      m_quote_mutex;
};

#endif // mainWindow_H
