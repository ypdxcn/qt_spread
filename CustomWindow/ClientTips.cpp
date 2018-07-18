#include "ClientTips.h"

CTipsManager g_TipsManager;

CClientTips::CClientTips(void)
{
}

CClientTips::~CClientTips(void)
{
}

CTipsManager::CTipsManager()
{
	m_iShowCount = 0;
}

int CTipsManager::Add( const TipsOrderInfo &TipsInfo )
{
	int nAddPos = -1;

	m_csMutex.lock();
	m_vecTips.push_back(TipsInfo);
	nAddPos = m_vecTips.size()-1;
	m_csMutex.unlock();

	return nAddPos;
}

int CTipsManager::GetTips( TipsOrderInfo &stTips, int nPos )
{
	//QMutexGuard guard(m_csMutex);
	QMutexLocker  guard(&m_csMutex);

	int nSize = m_vecTips.size();
	if(  nSize == 0 )
	{
		return -1;
	}

	// 如果不合法，则采用最后一条
	if( nPos < 0 || nPos > nSize-1 )
	{
		stTips =  m_vecTips[nSize-1];
		return 1;
	}
	else
	{
		stTips = m_vecTips[nPos];
		return 0;
	}
}

void CTipsManager::Clear()
{
	QMutexLocker guard(&m_csMutex);

	m_vecTips.clear();
}

int CTipsManager::GetSize()
{
	return m_vecTips.size();
}
