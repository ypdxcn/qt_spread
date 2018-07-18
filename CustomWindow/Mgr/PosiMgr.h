#pragma once

#include <QMap>
#include "StDef.h"
#define g_PosiMgr CPosiMgr::GetInstance()

class CPosiMgr
{
private:
	CPosiMgr(void);
	~CPosiMgr(void);

	CPosiMgr( const CPosiMgr&) ;            // 不实现.
	CPosiMgr& operator= ( const CPosiMgr& ); // 不实现.

public:
	static CPosiMgr& GetInstance();

	void Initialize( QMap<QString, DeferPosi> &mapDeferPosi, QMap<QString, StoreInfo> &mapStoreInfo );

	const DeferPosi* GetPosiInfo( const QString &sInsID );

	// 获取某个品种某个方向上的持仓信息
	const DeferPosiInfo* GetPosiInfo( const QString &sInsID, const QString &sExchID );

	// 获取某个品种某个方向上的持仓信息
	const DeferPosiInfo* GetPosiInfo( const QString &sInsID, bool bLong );

	// 根据交易类型ID判断是多仓还是空仓
	bool IsLong( bool &bLong, const QString &sExchID );

	// 获得库存信息
	const StoreInfo* GetStoreInfo( const QString &sInsID );

private:
	// 持仓信息
	QMap<QString, DeferPosi> *m_QMapDeferPosi;

	// 库存信息
	QMap<QString, StoreInfo> *m_QMapStoreInfo; // 
};
