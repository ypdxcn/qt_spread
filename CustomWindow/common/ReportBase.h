#pragma once

#include "TraderCpMgr.h"
#include <QStringList>
#include <QPainter>

#define DEFAULT_DPI_X 96
#define DEFAULT_DPI_Y 96

struct RptDataInfo
{
	QString strTitle;
	QString strContent;
	bool isLeftAli;//内容是否左对齐
};

struct RptColumnInfo
{
	int iColumnPercent; // 如果是0，则取平均值
	bool bReturn;       // 标题是否换行
	int iReturnBegin;   // 标题换行的开始位置，以字符数计数
};

struct Table1Info
{
	int iColumnPercent;
};

typedef enum
{
	E_REPORT  = 0,
	E_PRINT,
} EShowMode;

typedef enum
{
	E_DAY = 0,
	E_MONTH,
	E_TIHUO,
} EDataMode;

class QReportBase
{
public:
	QReportBase(void);
public:
	~QReportBase(void);

public:
	// 第一种表格的数据
	vector<RptDataInfo> m_vecRisk;       // 客户信息
	vector<RptDataInfo> m_vecFund;       // 资金信息
	vector<RptDataInfo> m_vecFundDetail; // 资金发生明细
	QString m_strGaCha;                  // 记录资金发生明细中的轧差
	// 第二种表格的数据
	/*QStringArray*/ QStringList m_arrayStorage;         // 库存信息
	/*QStringArray*/ QStringList m_arrayForward;         // 远期
	/*QStringArray*/QStringList  m_arrayDefer;           // 延期持仓信息
	/*QStringArray */QStringList m_arraySurplus;         // 本期盈亏
	/*QStringArray */QStringList m_arrayMatchFlow;       // 成交流水
	/*QStringArray*/ QStringList m_arrayFundInOutFlow;   // 出入金流水

	vector<RptColumnInfo> m_vStorage;
	vector<RptColumnInfo> m_vForward;
	vector<RptColumnInfo> m_vDefer;
	vector<RptColumnInfo> m_vSurplus;
	vector<RptColumnInfo> m_vMatchFlow;
	vector<RptColumnInfo> m_vFundInOutFlow;

	vector<Table1Info> m_vecRiskColumn;

	// 数据定义，第二种表格的数据行（即除了标题行）的个数
	int m_iStorage;
	int m_iForward;
	int m_iDefer;
	int m_iSurplus;
	int m_iMatchFlow;
	int m_iFundInOutFlow;

public:
	void IniAllData(Rsp6007 &rsp);

protected:
	struct PrintInfo
	{
		int m_nCurPage;
		int m_MaxPage;
	};
	 /*CPrintInfo*/ PrintInfo  m_printInfo;
	 void SetPage(/*CPrintInfo*/PrintInfo* pInfo, int& nHeight);

	// 日结单和月结单打印的数据初始化
	void IniRisk(Rsp6007 &rsp);
	void IniFund(Rsp6007 &rsp);
	void IniFundDetail(Rsp6007 &rsp);
	void IniStorage(Rsp6007 &rsp);
	void IniForward(Rsp6007 &rsp);
	void IniDefer(Rsp6007 &rsp);
	void IniSurplus(Rsp6007 &rsp);
	void IniMatchFlow(Rsp6007 &rsp);
	void IniFundInOutFlow(Rsp6007 &rsp);
    
	float m_xRate;
	float m_yRate;

	// 业务逻辑
	int GetX(int x)
	{
		return (int)(x*m_xRate);
	}

	int GetY(int y)
	{
		return (int)(y*m_yRate);
	}
	

	// 画图相关
	void Draw(QPainter* pDC, PrintInfo* pInfo);
	//
	void DrawPart1(int &iLeftTopy, QPainter* pDC);
	// 
	void DrawPart2(int &iLeftTopy, QPainter* pDC);
	//
	void DrawTitle(QPainter* pDC);
	//画第一种表格
	int DrawTable(QPainter* pDC, int iLeftTopY, int iLineCount, int iTitleCount, int iTitleWid, const vector<RptDataInfo> &vec, QString strLTitle, QString strRTitle);
	//画第一种表格拓展
	int DrawTableEx(QPainter* pDC, int iLeftTopY, int iLineCount, int iTitleCount, int iTitleWid, const vector<RptDataInfo> &vec, QString strLTitle, QString strRTitle, const vector<Table1Info> &vColumnInfo);
	//画资金发生明细中的轧差一行
	int DrawFunDetail(QPainter* pDC, int iLeftTopY, QString strTitle, QString strContetn);//画资金发生明细的最后一行
	//画第二种表格
	//int DrawTable2(QPainter* pDC, int iLeftTopY, int iLineCount, int iColCount, const QStringArray &strArray, QString strLTitle, QString strRTitle, int iLAliBegin = 0);
	//画第二种表格
	int DrawTable2(QPainter* pDC, int iLeftTopY, int iLineCount, int iColCount, const QStringList &strArray, QString strLTitle, QString strRTitle, const vector<RptColumnInfo> &vColumnInfo, int iLAliBegin = 0,  bool bColumnEven = true);

/*提货相关****************************************/
private:
	QString m_strTiHuoTop[17];
	QString m_strVarietyId;
	QString m_strAmount;
	QString m_strTradeName;
public:
	void DrawTiHuo(QPainter* pDC);
	void IniTiHuoData(Rsp5103 &rsp);

/*公共代码**********************************/
public:
	 int m_iType;            // 0为日结单打印，1为月结单，2为提货单打印
	// 整个报表的标题
	QString m_strTitle;

	// 设置显示的模式，是报表还是打印预览
	void SetShowMode(EShowMode eShowMode);

protected:
	// 画页码信息，子类可以重写
	virtual void WritePage(QPainter* pDC, PrintInfo* pInfo);

private:
	

protected:
	short m_iMaxPage;
	EShowMode m_eShowMode;

	//公共数据
	int TABLE_LINE_HEIGHT;
	int TABLE_TITLE_HEIGHT;
	int TABLE_WIDTH;
	int LEFT_X;
	int RIGHT_X;
	int TABLE_TO_TITLE_DIS;
	int TABLE_BEGIN_Y;
	int PAGE_BEGIN_X;
	int PAGE_BEGIN_Y;
	int TABLE_PAGE2_BEGIN_Y;

	//标题数据
	int TITLE_BEGIN_X;
	int TITLE_BEGIN_Y;
	int LINE_Y;

	int TABLE1_LINE_TITLE_COUNT;
	int TABLE2_TITLE_HEIGHT;
	int CUS_INFO_COL_TITLE_WIDTH;
	int CUS_INFO_LINE_COUNT;
	int FUN_INFO_COL_TITLE_WIDTH;
	int FUN_INFO_LINE_COUNT;
	int FUN_DETAIL_COL_TITLE_WIDTH;
	int FUN_DETAIL_LINE_COUNT;

	int STORE_INFO_COLUME_COUNT;
	int CHICANG_INFO_COLUME_COUNT;
    int YINGKUI_INFO_COLUME_COUNT;
	int MATCHFLOW_COLUME_COUNT;
	int FUNINOUTFLOW_COLUME_COUNT;

	int TEXT_DISTANCE_X;
	int TEXT_DISTANCE_Y;
	int MY_FONT_SIZE;

	int TIHUO_TITLE_BEGIN_X;
	int TIHUO_TITLE_BEGIN_Y;

	int TITLE_TOP_DISTANCE;
	int TOP_BEGIN_X;
	int TOP_BEGIN_Y;
	int TOP_LINE_HEIGHT;
	int TOP_TITLE_WIDTH;
	int TOP_CONTENT_EIDTH;

	int TOP_MIDDLE_DISTANCE;
	int MIDDLE_BEGIN_X;
	int MIDDLE_BEGIN_Y;
	int MIDDLE_COLUMN_WIDTH;
	int MIDDLE_LINE_HEIGTH;
	int MIDDLE_TEXT_DISTANCE_Y;

	int MIDDLE_BOTTOM_DISTANCE;
	int BOTTOM_TITLE_WIDTH;
	int BOTTOM_CONTENT_EIDTH;


	int			m_nPageHeight;				//打印纸高度
	int			m_nPageWidth;				//打印纸宽度
};
