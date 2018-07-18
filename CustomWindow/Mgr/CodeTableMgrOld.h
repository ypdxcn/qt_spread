/*
用于临时解决旧的码表管理的问题
用来管理码表
*/
#pragma once

#include "..\Mgr\AlmHtmBase.h"
#include "StDef.h"
#include <QMap>

#define g_CodeTableMgrOld CCodeTableMgrOld::GetInstance()

class CCodeTableMgrOld : public CAlmHtmBase
{
private:
	CCodeTableMgrOld( void );
	virtual ~CCodeTableMgrOld( void) ;

	CCodeTableMgrOld( const CCodeTableMgrOld&) ;            // 不实现.
	CCodeTableMgrOld& operator= ( const CCodeTableMgrOld& ); // 不实现.

public:
	static CCodeTableMgrOld& GetInstance();

	// 初始化类变量
	bool Initialize( const ArrayListMsg *pCodeTable /*= NULL */ );

private:
	// 从服务器加载码表
    bool LoadDataFromServer( void );

	// 从HashtableMsg加载
	void LoadFromHtm( HashtableMsg &htm );

	// 从ArrayListMsg加载
	void LoadFromAlm( const ArrayListMsg &alm );

	// 是否需要的字段
	bool IsNeed( const string &sValue );

	// 设置代理
	void SetDelegate( void );

	// 添加一个码表
	void AddValue( const MyCodeInfo &stCodeValue );

private:
	QVector<MyCodeInfo> *m_vExchCode;   //交易类型
	QVector<MyCodeInfo> *m_vOffsetFlag; //开平标志
	QVector<MyCodeInfo> *m_vFeeCode;    //费用代码
	QVector<MyCodeInfo> *m_vSubFeeCode; //费用代码
	QVector<MyCodeInfo> *m_vLongShort;  //持仓方向

	//list字段转换
	QVector<MyCodeInfo> *m_vMarketId;   //交易市场
	QVector<MyCodeInfo> *m_vEntrState;  //委托状态
	QVector<MyCodeInfo> *m_vCancelFlag; //撤单标志
	QVector<MyCodeInfo> *m_vTermType;   //撤销渠道
	QVector<MyCodeInfo> *m_vAccessWay;  //操作方向
	QVector<MyCodeInfo> *m_vCheckState; //复核状态
	QVector<MyCodeInfo> *m_vTakeState;  //提货状态
	QVector<MyCodeInfo> *m_vDrawType;   //提货类型
	QVector<MyCodeInfo> *m_vCertType;   //证件类型
	QVector<MyCodeInfo> *m_vAcctState;  //账户状态
	QVector<MyCodeInfo> *m_vInstState2;  //交易所合约状态
	QVector<MyCodeInfo> *m_vFCOrderState; // 用于强平单类型的转换
	QVector<MyCodeInfo> *m_vDeliFlag; // 交收标志
	QVector<MyCodeInfo> *m_vPageInfo; // 查询界面的页码信息
};
