/*
交割品种代码的管理类
将交割品种代码有关的信息整合到此类管理，同时进行了功能的改进
使得可以支持在配置文件中保存Alm或者Htm两种数据（以前只支持Alm）
加强了对错误和异常的处理

由于涉及到改动太大的问题，没有去除CTraderCpMgr的m_vVariety，
暂时只是作为该数据的代理而已，以后的代码则可以调用这个类
以后重构的时候则可将数据保存到这个类，真正实现数据的管理
*/
#pragma once

#include "AlmHtmBase.h"
#include "StDef.h"
#include <QVector>

#define g_VarietyMgr CVarietyMgr::GetInstance()

class CVarietyMgr : public CAlmHtmBase
{
private:
	CVarietyMgr(void);
	~CVarietyMgr(void);

	CVarietyMgr( const CVarietyMgr&) ;            // 不实现.
	CVarietyMgr& operator= ( const CVarietyMgr& ); // 不实现.

public:
	static CVarietyMgr& GetInstance();

	// 初始化类变量
	bool Initialize( QVector<VarietyInfo> &vVariety, const ArrayListMsg *pVariety_list = NULL );

	// 获取交割品种名称
	QString GetVarietyName( const QString &sID );

	// 获取交割品种ID
	QString GetVarietyID( const QString &sName );

	// 根据品种ID获取品种名称获取品种信息
	const VarietyInfo* GetInfo( const QString &sKey, bool bID );

private:
	// 从服务器加载
	bool LoadDataFromServer( void );

	// 从ArrayListMsg加载交割品种代码
	void LoadFromAlm( const ArrayListMsg &alm );

	// 从HashtableMsg加载
	void LoadFromHtm( HashtableMsg &htm );

	// 根据交割品种ID获取品种信息
	const VarietyInfo* FindFromID( const QString &sID );

	// 根据交割品种名称获取品种信息
	const VarietyInfo* FindFromName( const QString &sName );

private:
	QVector<VarietyInfo> *m_vVariety;   // 交割品种代码
};
