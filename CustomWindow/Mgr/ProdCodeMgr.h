/*
合约代码的管理类
将合约代码有关的信息整合到此类管理，同时进行了功能的改进
使得可以支持在配置文件中保存Alm或者Htm两种数据（以前只支持Alm）
加强了对错误和异常的处理

由于涉及到改动太大的问题，没有去除CTraderCpMgr的m_vProdCode，
暂时只是作为该数据的代理而已，以后的代码则可以调用这个类
以后重构的时候则可将数据保存到这个类，真正实现数据的管理
*/
#pragma once

#include "AlmHtmBase.h"
#include "StDef.h"
#include <QVector>
#include "HJDef.h"

#define g_ProdCodeMgr CProdCodeMgr::GetInstance()

class CProdCodeMgr : public CAlmHtmBase
{
private:
	CProdCodeMgr(void);
	~CProdCodeMgr(void);

	CProdCodeMgr( const CProdCodeMgr&) ;            // 不实现.
	CProdCodeMgr& operator= ( const CProdCodeMgr& ); // 不实现.

public:
	static CProdCodeMgr& GetInstance();

	// 初始化类变量
	bool Initialize( QVector<ProdCodeInfo> &vProdCode, const ArrayListMsg *pVariety_list /*= NULL */ );

	// 获取一个品种的信息
	const ProdCodeInfo* GetProdeCode( const QString &sInsID );

	MarketType GetMarketType( const ProdCodeInfo* pInfo );

private:
	// 从服务器加载
	bool LoadDataFromServer( void );

	// 从ArrayListMsg加载合约代码
	void LoadFromAlm( const ArrayListMsg &alm );

	// 从HashtableMsg加载
	void LoadFromHtm( HashtableMsg &htm );

private:
	QVector<ProdCodeInfo> *m_vProdCode; //交易类型
};
