#include <sstream>
#include <iomanip>
#include "NetLogDev.h"

CNetLogDev::CNetLogDev(CLogger * pLogger) : CLogDevice(pLogger)
{
}

CNetLogDev::~CNetLogDev(void)
{
}

int CNetLogDev::Initial(CConfig * pCfg)
{
	return CLogDevice::Initial(pCfg);
}

int CNetLogDev::Open(void)
{
	return 0;
}

void CNetLogDev::Close(void)
{

}

void CNetLogDev::Finish()
{
	CLogDevice::Finish();
}

void CNetLogDev::Register(CSubscriber* p)
{
	vector<CSubscriber*>::iterator it;
	bool blFound = false;
	DevLock();
	for (it = m_vecSubscribers.begin(); it != m_vecSubscribers.end(); ++it)
	{
		if (*it == p)
		{
			blFound = true;
			break;
		}
	}
	if (!blFound)
		m_vecSubscribers.push_back(p);
	DevUnlock();
}
	
void CNetLogDev::UnRegister(CSubscriber* p)
{
	vector<CSubscriber*>::iterator it;
	DevLock();
	for (it = m_vecSubscribers.begin(); it != m_vecSubscribers.end(); ++it)
	{
		if (*it == p)
		{
			m_vecSubscribers.erase(it);
			break;
		}
	}
	DevUnlock();
}


void CNetLogDev::Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText)
{
	if (!m_nLogLevel.test(nLevel))
		return;

	std::stringstream ss;
	ss << CurrentTime().c_str() << " " << LevelString(nLevel) << ":" << cszText;
	vector<CSubscriber*>::iterator it;
	DevLock();
	for (it = m_vecSubscribers.begin(); it != m_vecSubscribers.end(); ++it)
	{
		if (0 != *it)
			(*it)->OnNotify(ss.str());
	}
	DevUnlock();
}
