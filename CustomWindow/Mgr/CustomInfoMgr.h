#pragma once

//struct Rsp2040;
#include "TradePacketAndStruct.h"

#define g_CusInfoMgr CCustomInfoMgr::GetInstance()

class CCustomInfoMgr
{
private:
	CCustomInfoMgr(void);
	~CCustomInfoMgr(void);

	CCustomInfoMgr( const CCustomInfoMgr&) ;            // 不实现.
	CCustomInfoMgr& operator= ( const CCustomInfoMgr& ); // 不实现.

public:
	static CCustomInfoMgr& GetInstance();

	// 初始化类变量
	bool Initialize( Rsp1020 *pRsp1020 );
    
	// 获得客户信息
	const Rsp1020 &GetInfo( void );

	// 获得库存信息
	HashtableMsg GetStorInfo( void );

	// 获得持仓信息
	const HashtableMsg &GetPosiInfo( void );

private:
	Rsp1020 *m_rsp1020;
};
