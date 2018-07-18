#include "MemShareAlive.h"
#include "strutils.h"
using namespace std;
using namespace strutils;

CMemShareAlive::CMemShareAlive(E_PROC_TYPE eType)
:m_pAliveInf(0)
,m_uiNoAlive(0)
,m_uiProcessType(eType)
{
}

CMemShareAlive::~CMemShareAlive(void)
{
}

string CMemShareAlive::ToString()
{
	
	string sRtn = strutils::ToString<unsigned int>(m_pAliveInf->uiAppNodeID);
	sRtn += ",";
	sRtn += strutils::ToString<unsigned int>(m_pAliveInf->uiAppAlive);
	sRtn += ",";
	sRtn += strutils::ToString<unsigned int>(m_uiNoAlive);
	sRtn += ",";
	sRtn += strutils::ToString<unsigned int>(m_pAliveInf->uiAppProcessID);
	sRtn += ",";
	sRtn += strutils::ToString<unsigned int>(m_pAliveInf->uiDogAlive);
	sRtn += ",";
	sRtn += strutils::ToString<unsigned int>(m_pAliveInf->uiDogProcessID);
	sRtn += ",";
	sRtn += strutils::ToHexString<unsigned int>(m_pAliveInf->uiAppQuitCmd);
	sRtn += ",";
	sRtn += strutils::ToHexString<unsigned int>(m_pAliveInf->uiAppIQuitNtf);
	sRtn += ",";
	sRtn += strutils::ToHexString<unsigned int>(m_pAliveInf->uiDogIQuitNtf);
	sRtn += ",";
	sRtn += strutils::ToHexString<unsigned int>(m_pAliveInf->uiOthQuitCmd);
	sRtn += ",";
	sRtn += strutils::ToString<unsigned int>(m_pAliveInf->uiOthQuitNodeID);
	return sRtn;
}

BOOL CMemShareAlive::Create(LPCTSTR lpszTblName, BOOL bReadOnly)
{
	size_t nSize = sizeof(ALIVE_INF);
	BOOL blRtn = MapMemory(lpszTblName,nSize,bReadOnly);
	if (TRUE == blRtn)
	{
		char * pTmp = (char*)m_lpData;		
		m_pAliveInf = (ALIVE_INF*)pTmp;

		if (IsActiveCreate())
		{
			m_pAliveInf->uiAppNodeID = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiAppAlive = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiAppProcessID = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiDogAlive = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiDogProcessID = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiAppQuitCmd = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiAppIQuitNtf = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiDogIQuitNtf = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiOthQuitCmd = MEMSHARE_INVALID_VALUE;
			m_pAliveInf->uiOthQuitNodeID = MEMSHARE_INVALID_VALUE;
		}
	}
	return blRtn;
}

void CMemShareAlive::Close()
{
	UnMap();
}

ALIVE_STATUS CMemShareAlive::IsAlive()
{
	ALIVE_STATUS eRtn = E_UNKNOWN_ALIVE;
	if (E_PROCESS_DOG == m_uiProcessType)
	{
		if (0 != m_pAliveInf->uiAppAlive)
		{
			if (MEMSHARE_INVALID_VALUE != m_pAliveInf->uiAppAlive)
			{
				m_pAliveInf->uiAppAlive = 0;
				m_uiNoAlive = 0;
				eRtn = E_IS_ALIVE;
			}
		}
		else
		{
			m_uiNoAlive++;
			if (m_uiNoAlive > MAX_NO_ALIVE)
			{
				eRtn = E_NO_ALIVE;
			}
		}
	}
	else
	{
		if (0 != m_pAliveInf->uiDogAlive)
		{
			if (MEMSHARE_INVALID_VALUE != m_pAliveInf->uiDogAlive)
			{
				m_pAliveInf->uiDogAlive = 0;
				m_uiNoAlive = 0;
				eRtn = E_IS_ALIVE;
			}
		}
		else
		{
			m_uiNoAlive++;
			if (m_uiNoAlive > MAX_NO_ALIVE)
			{
				eRtn = E_NO_ALIVE;
			}
		}
	}
	return eRtn;
}

ALIVE_STATUS CMemShareAlive::IsAlive(unsigned int& uiProcessID)
{
	ALIVE_STATUS eRtn = E_UNKNOWN_ALIVE;
	if (E_PROCESS_DOG == m_uiProcessType)
	{
		if (0 != m_pAliveInf->uiAppAlive)
		{
			if (MEMSHARE_INVALID_VALUE != m_pAliveInf->uiAppAlive)
			{
				m_pAliveInf->uiAppAlive = 0;
				m_uiNoAlive = 0;
				eRtn = E_IS_ALIVE;
			}
		}
		else
		{
			m_uiNoAlive++;
			if (m_uiNoAlive > MAX_NO_ALIVE)
			{
				eRtn = E_NO_ALIVE;
			}
		}
		uiProcessID = m_pAliveInf->uiAppProcessID;
	}
	else
	{
		if (0 != m_pAliveInf->uiDogAlive)
		{
			if (MEMSHARE_INVALID_VALUE != m_pAliveInf->uiDogAlive)
			{
				m_pAliveInf->uiDogAlive = 0;
				m_uiNoAlive = 0;
				eRtn = E_IS_ALIVE;
			}
		}
		else
		{
			m_uiNoAlive++;
			if (m_uiNoAlive > MAX_NO_ALIVE)
			{
				eRtn = E_NO_ALIVE;
			}
		}
	}
	return eRtn;
}


void CMemShareAlive::IamAlive()
{
	if (E_PROCESS_DOG == m_uiProcessType)
	{
		m_pAliveInf->uiDogAlive = 1;
	}
	else
	{
		m_pAliveInf->uiAppAlive = 1;		
	}
}

void CMemShareAlive::IamAlive(unsigned int uiProcessID)
{
	if (E_PROCESS_DOG == m_uiProcessType)
	{
		m_pAliveInf->uiDogAlive = 1;
		m_pAliveInf->uiDogProcessID = uiProcessID;	
	}
	else
	{
		m_pAliveInf->uiAppAlive = 1;
		m_pAliveInf->uiAppProcessID = uiProcessID;	
	}
}

void CMemShareAlive::IQuitNtf()
{
	if (E_PROCESS_DOG == m_uiProcessType)
	{
		m_pAliveInf->uiDogIQuitNtf = QUIT_CMD;
	}
	else
	{
		m_pAliveInf->uiAppIQuitNtf = QUIT_CMD;
	}
}

void CMemShareAlive::YQuitCmd()
{
	if (E_PROCESS_DOG == m_uiProcessType)
	{
		m_pAliveInf->uiAppQuitCmd = QUIT_CMD;
	}
}

void CMemShareAlive::HQuitCmd(unsigned int uiCmd, unsigned int uiOthQuitNodeID)
{
	if (E_PROCESS_APP == m_uiProcessType)
	{
		m_pAliveInf->uiOthQuitCmd = uiCmd;
		m_pAliveInf->uiOthQuitNodeID = uiOthQuitNodeID;
	}
}

BOOL CMemShareAlive::IsIQuitCmd()
{
	BOOL blRtn = FALSE;
	if (E_PROCESS_APP == m_uiProcessType)
	{
		if (QUIT_CMD == m_pAliveInf->uiAppQuitCmd)
		{
			m_pAliveInf->uiAppQuitCmd = MEMSHARE_INVALID_VALUE;
			blRtn = TRUE;
		}
	}
	return blRtn;
}

BOOL CMemShareAlive::IsHQuitCmd(unsigned int& uiOthQuitCmd, unsigned int& uiOthQuitNodeID)
{
	BOOL blRtn = FALSE;
	if (E_PROCESS_DOG == m_uiProcessType && MEMSHARE_INVALID_VALUE != m_pAliveInf->uiOthQuitCmd)
	{
		uiOthQuitCmd = m_pAliveInf->uiOthQuitCmd;
		uiOthQuitNodeID =  m_pAliveInf->uiOthQuitNodeID;

		m_pAliveInf->uiOthQuitCmd = MEMSHARE_INVALID_VALUE;
		m_pAliveInf->uiOthQuitNodeID = MEMSHARE_INVALID_VALUE;
		blRtn = TRUE;
	}
	return blRtn;
}

void CMemShareAlive::SetNodeID(unsigned int uiNodeID)
{
	if (E_PROCESS_APP == m_uiProcessType)
	{
		m_pAliveInf->uiAppNodeID = uiNodeID;
	}
}

unsigned int CMemShareAlive::AppNodeID()
{
	return m_pAliveInf->uiAppNodeID;
}