/*
用来管理码表
*/
#pragma once

#include "..\Mgr\AlmHtmBase.h"
#include <QMap>
#include <QMultiMap>
#include <QPair>
#include "HJDef.h"

struct CodeValue
{
	QString  code_id              ; // 代码编号    
	QString  code_desc            ; // 代码描述

	CodeValue(const QString &sCode_id,  const QString &sCode_desc) : code_id(sCode_id),code_desc(sCode_desc){}
};

#define g_CodeTableMgr CCodeTableMgr::GetInstance()

class CCodeTableMgr : public CAlmHtmBase
{
private:
	CCodeTableMgr( void );
	virtual ~CCodeTableMgr( void) ;

	CCodeTableMgr( const CCodeTableMgr&) ;            // 不实现.
	CCodeTableMgr& operator= ( const CCodeTableMgr& ); // 不实现.

public:
	static CCodeTableMgr& GetInstance();

	// 初始化类变量
	bool Initialize( const ArrayListMsg *pCodeTable /*= NULL */ );

	// 获取一组码表
	QVector<const CodeValue*> GetGroupCode( const QString &sCodeType );

	// 根据码表字段ID或者码表字段的值
	QString GetCodeNameFromID( const QString &sCodeType, const QString &sCodeID);

	// 根据码表字段值来获取码表字段ID
	QString GetCodeIDFromName( const QString &sCodeType, const QString &sCodeName);

	// 根据市场获取其交易方式
	void GetExchTypeName( QVector<const CodeValue*> &vecCode, MarketType eMarket );

	// 去掉交易方式中不需要的（exch_code｜4061｜撤单）
	void EraseUselessExchType( QVector<const CodeValue*> &vecCode );

private:
	// 从服务器加载码表
    bool LoadDataFromServer( void );

	// 从HashtableMsg加载
	void LoadFromHtm( HashtableMsg &htm );

	// 从ArrayListMsg加载
	void LoadFromAlm( const ArrayListMsg &alm );

	// 是否需要的字段
	bool IsNeed( const QString &sValue );

private:
	typedef QMultiMap <QString, CodeValue>::const_iterator CIT; 
	typedef QPair<CIT, CIT> Range;
	QMultiMap<QString, CodeValue> m_mapCode;
};
