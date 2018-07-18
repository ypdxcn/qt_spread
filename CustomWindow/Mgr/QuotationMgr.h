#pragma once

#include <QString>
#include <QMap>
#include "BroadcastPacketAndStruct.h"
typedef enum
{
	PriceType_Buy1,
	PriceType_Sell1,
	PriceType_Last,
	PriceType_LastSettle,
	PriceType_LastClose,
}EPriceType;

#define g_QuoMgr CQuotationMgr::GetInstance()

class CQuotationMgr
{
private:
	CQuotationMgr(void);
	~CQuotationMgr(void);

	CQuotationMgr( const CQuotationMgr&) ;            // 不实现.
	CQuotationMgr& operator= ( const CQuotationMgr& ); // 不实现.

public:
	static CQuotationMgr& GetInstance();

	// 初始化类变量
	bool Initialize( map<string, QUOTATION> &mapQuotation );

	// 获取某个品种格式化后的价格
	int GetFormatPrice( QString &csPrice, const QString &sInsID, EPriceType eType );

	// 根据价格类型获取某个品种的某个价格（买卖1价、最新价）
	uint GetPrice( const QUOTATION &qt, EPriceType eType ) const;

	// 获取买卖1价
	uint GetBSPrice( const QUOTATION &qt, EPriceType eType ) const;

	// 获取一个合法价
	uint GetValidePrice( const QUOTATION &qt ) const;

	double GetBSPriceEx( const QUOTATION &qt, EPriceType eType ) const;

	const QUOTATION* GetQuotation( const QString &sInsID ) const;

private:
	double m_fFactor;
	map<string, QUOTATION> *m_mapQuotation;
};
