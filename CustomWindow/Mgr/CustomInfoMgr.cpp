#include "CustomInfoMgr.h"

CCustomInfoMgr::CCustomInfoMgr(void)
{
}

CCustomInfoMgr::~CCustomInfoMgr(void)
{
}

CCustomInfoMgr& CCustomInfoMgr::GetInstance()
{
	static CCustomInfoMgr theSingleton;
	return theSingleton;
}

bool CCustomInfoMgr::Initialize( Rsp1020 *pRsp1020 )
{
	m_rsp1020 = pRsp1020;

	return true;
}

const Rsp1020 &CCustomInfoMgr::GetInfo( void )
{
	return *m_rsp1020;
}

const HashtableMsg &CCustomInfoMgr::GetPosiInfo( void )
{
	return m_rsp1020->htm_td_info;
}

// 不知为啥，只能返回值，不能返回引用，否则调用的时候会报错
HashtableMsg CCustomInfoMgr::GetStorInfo( void )
{
	return m_rsp1020->htm_td_info;
}
