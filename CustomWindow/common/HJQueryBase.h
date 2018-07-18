#pragma once

#include <QDateTime>
#include <QComboBox>
#include <QStandardItemModel>
#include <QTableView>
#include "TraderCpMgr.h"
#include <vector>
#include <QVector>
using namespace std;

typedef enum
{
	E_FIRST  = 1,
	E_FORWARD,
	E_NEXT,
	E_LAST,
} EPageBtnType;

#define CONSTANT_LIST_TO_BORDER



class CHJQueryBase
{
public:
	CHJQueryBase(void);
public:
	~CHJQueryBase(void);

public:
	// 写入
	//void WriteListDataToExcel(CString csSheetName, vector<CString>  &vecAlign);

protected: 

	//  页码有关---------------------------------------------------------------------

	// 初始化页码下拉菜单
	void InitPage();

	// 获取当前选择的页码
	int  GetCurPage();

	// 初始化每页记录数下拉菜单
	void InitPageNum();

	// 获取当前选中的每页记录数
	int  GetPageNum();

	// 点击第一页，前一页、后一页、最后一页按钮时对应的要改变当前页面combox的当前选项
	void SetComboxCurPage(EPageBtnType ePagebtnType);

	// 向服务器查询后重新加载页码下拉菜单和当前选中页码
	void ResetComboxPage(const int &iTotal, int &iCurPage);

	void OnButtonFirst();
	void OnButtonPrev();
	void OnButtonNext();
	void OnButtonLast();

	void OnSelchangeComboNum();
	void OnSelchangeComboPage();
	

	//  列表的列相关---------------------------------------------------------------------
	void LoadList();

	// 清空list数据和标题
	void ClearList();

	// 由于暂时发现没有必要独立为函数，已经被合并到LoadList()，如果有必要，可以恢复
	//void AddListColumn( CListCtrl* lst, vector<CString> &vecName, vector<int> &vecAlign, vector<int> &vecWidth );

	// 获取用户选择字段的ID，发送查询请求报文和处理返回报文的时候用到，可以修改为局部变量
	void GetUserColumnIDs(const QString &strPath);

	// 
	void GetUserAlign(vector<int> & vecAlign);

	// 查询相关---------------------------------------------------------------------

	void OnButtonQuery();
	void OnButtonQuery(QDateTime &dtBegin, QDateTime &dtEnd);

	// 获取查询的标识，在子类实现
	virtual string GetQueryID(){return "";};

	// 获取查询的条件，在子类实现
	virtual void GetQueryPara(QVector< pair<QString, QString> >  &vecPara){};

	// 向服务器提交查询请求，连接交易服务器失败则返回false，成功则返回true
	bool Query(const string &strQueryTag, QVector< pair<QString,QString> > & vecPara, const int &iPageNum/*每页的数量*/, const int &iCurPage/*当前请求的页码*/, Req6002 &stReq6002, Rsp6002 &stRsp6002);

	// 显示查询结果到界面
	void ShowQueryResult(const Req6002 &stReq6002, const Rsp6002 &stRsp6002);

	virtual QString GetSheetName(){ return "";};

	// 暂定函数体为空，子类有必要的话重写
	virtual void QueryReserver(const Req6002 &stReq6002, const Rsp6002 &stRsp6002);

	// 输出到excel按钮的触发函数
	void OnOutputToExce(QString csSheetName);

	void OnBtnSetshowcolumn(QString strNodeDate = "");

	// 判断一个字段ID是否应该格式化为浮点数
	bool IsFloatTitle( const string &strID, const string &sQueryID = "" );

	void PairAdd(QVector< pair<QString,QString> > & v,QString strtitle,QString strvalue);

	virtual void IniProdCode(QComboBox& comb,string marketid="");//初始化合约代码
	string GetProdCode(QComboBox& comb);//获取合约代码
	void IniExchCode(QComboBox& comb);
	QString GetExchCode(QComboBox& comb);
	void IniOffsetFlag(QComboBox& comb);
	string GetOffsetFlag(QComboBox& comb);
	void IniLongShort(QComboBox& comb);
	string GetLongShort(QComboBox& comb);
	void IniAccessWay(QComboBox& comb);
	string GetAccessWay(QComboBox& comb);
	void IniVarietyId(QComboBox& comb);
	string GetVarietyId(QComboBox& comb);
	void IniFeeCode(QComboBox& comb);
	string GetFeeCode(QComboBox& comb);
	void IniSubFeeCode(QComboBox& comb);
	string GetSubFeeCode(QComboBox& comb);
	void FeeCodeSelectChange(QComboBox& comb1,QComboBox& comb2);
	void ProdCodeSelectChange(QComboBox& comb1,QComboBox& comb2);

	
    //  变量---------------------------------------------------------------------
	QTableView         * m_list;
	QStandardItemModel * m_tableModle;
	QComboBox *m_cmbPage;
	QComboBox *m_cmbNum;
	QVector<int> m_vecColumnIndexs;
	ArrayListMsg m_almUserColumnIDs;
};
