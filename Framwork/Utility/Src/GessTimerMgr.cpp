#include "GessTimerMgr.h"
#include "Logger.h"
#include "strutils.h"

using namespace std;

CGessTimerMgr::CGessTimerMgr()
:m_ulSpanNow(1)
,m_ulMin(1)
,m_clkTotalDiff(0)
{
}

CGessTimerMgr::~CGessTimerMgr()
{
}

//初始化 期望的精度
int CGessTimerMgr::Init(unsigned long ulTmSpan)
{
	//取实现者支持的最小间隔
	m_ulMin = Min();
	//虚拟硬定时器间隔取两者较大者
	m_ulSpanNow = max(ulTmSpan,m_ulMin);

	m_clkLastWait = clock();
	return 0;
}

void CGessTimerMgr::Finish()
{
	m_mapTimers.clear();
}

int CGessTimerMgr::Start()
{
	return BeginThread();
}

void CGessTimerMgr::Stop()
{
	EndThread();
}

//创建定时器
int CGessTimerMgr::CreateTimer(CGessTimer* p, unsigned long ulTmSpan, const string& sKey)
{
	if (0 == p)
		return -1;

	typedef multimap<CGessTimer*,T_TIMER> MMAP_TM;
	typedef MMAP_TM::iterator MMAP_IT;
	typedef pair<MMAP_IT,MMAP_IT> RANGE_TM;
	MMAP_IT it;

	try
	{
		m_csTimes.Lock();
		RANGE_TM range = m_mapTimers.equal_range(p);
		for (it = range.first; it != range.second; )
		{
			if ((*it).second.sKey == sKey)
			{
				m_csTimes.Unlock();
				CRLog(E_SYSINFO,"重复创建定时器:%s",sKey.c_str());
				return -1;
			}
		}

		T_TIMER t;
		t.pTimer = p;
		t.ulSpan = max(m_ulSpanNow,ulTmSpan);
		t.sKey = sKey;

		//CTriggerWorker* pTrigger = new CTriggerWorker(this);
		t.pTrigger = 0;//pTrigger;
		clock_t clkNow = clock();
		unsigned long ulDiff = (clkNow - m_clkLastWait) / CLOCKS_PER_SEC;
		ulDiff = min(ulDiff,m_ulSpanNow);
		t.ulLeft = t.ulSpan - (m_ulSpanNow - ulDiff);
		m_mapTimers.insert(make_pair(p,t));
		m_csTimes.Unlock();
	}
	catch(...)
	{
		m_csTimes.Unlock();
		CRLog(E_ERROR,"%s","Unknow Exception CreateTimer");
	}
	return 0;
}

int CGessTimerMgr::DestroyTimer(CGessTimer* p,const string& sKey)
{
	typedef multimap<CGessTimer*,T_TIMER> MMAP_TM;
	typedef MMAP_TM::iterator MMAP_IT;
	typedef pair<MMAP_IT,MMAP_IT> RANGE_TM;
	MMAP_IT it;

	try
	{
		m_csTimes.Lock();
		RANGE_TM range = m_mapTimers.equal_range(p);
		for (it = range.first; it != range.second; )
		{
			//if (0 != (*it).second.pTrigger)
			//	delete (*it).second.pTrigger;

			//m_mapTimers.erase(it++);
			if ((*it).second.sKey == sKey)
			{
				m_mapTimers.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
		m_csTimes.Unlock();
	}
	catch(...)
	{
		m_csTimes.Unlock();
		CRLog(E_ERROR,"%s","Unknow Exception DestroyTimer");
	}
	return 0;
}

string CGessTimerMgr::ToString()
{
	string sRtn = "\r\n";
	multimap<CGessTimer*,T_TIMER>::iterator it;	
	try
	{
		m_csTimes.Lock();
		for (it = m_mapTimers.begin(); it != m_mapTimers.end(); ++it)
		{
			sRtn += "key:";
			sRtn += (*it).second.sKey;
			sRtn += " ulSpan:";
			sRtn += strutils::ToString<unsigned long>((*it).second.ulSpan);
			sRtn += " ulLeft:";
			sRtn += strutils::ToString<unsigned long>((*it).second.ulLeft);
			sRtn += " pTimer:";
			sRtn += strutils::ToString<unsigned long>(reinterpret_cast<unsigned long>((*it).second.pTimer));
			sRtn += "\r\n";
		}
		m_csTimes.Unlock();
	}
	catch(...)
	{
		m_csTimes.Unlock();
		CRLog(E_ERROR,"%s","Unknown exception!");
	}
	return sRtn;
}

//检测是否需要触发各个定时器
void CGessTimerMgr::OnTimeout()
{
	multimap<CGessTimer*,T_TIMER>::iterator it;
	try
	{
		m_csTimes.Lock();
		for (it = m_mapTimers.begin(); it != m_mapTimers.end(); )
		{
			if ((*it).second.ulLeft >= m_ulSpanNow)
			{
			   (*it).second.ulLeft -= m_ulSpanNow; 
			   ++it;
			}
			else if ((*it).second.ulLeft > 0)
			{
			   (*it).second.ulLeft = m_ulSpanNow;
			   ++it;
			}
			else
			{
				//(*it).second.pTrigger->Fire((*it).second);
				//(*it).second.ulLeft = (*it).second.ulSpan;
				//++it;
				(*it).second.ulLeft = (*it).second.ulSpan;
				unsigned long ulTmp = (*it).second.ulSpan;
				int nRtn = (*it).second.pTimer->TimeOut((*it).second.sKey,ulTmp);
				if (0 != nRtn)
				{
					m_mapTimers.erase(it++);//++it;
				}
				else
				{
					if (ulTmp != (*it).second.ulSpan)
					{
						(*it).second.ulSpan = max(m_ulSpanNow,ulTmp);
						(*it).second.ulLeft = (*it).second.ulSpan;
					}
					++it;
				}
			}
		}
		m_csTimes.Unlock();
	}
	catch(...)
	{
		m_csTimes.Unlock();
		CRLog(E_ERROR,"%s","Unknow Exception OnTimeout");
	}
}

void CGessTimerMgr::UpdateInterval(CGessTimer* p,const string& sKey, unsigned long ulSpan)
{
	typedef multimap<CGessTimer*,T_TIMER> MMAP_TM;
	typedef MMAP_TM::iterator MMAP_IT;
	typedef pair<MMAP_IT,MMAP_IT> RANGE_TM;
	MMAP_IT it;

	try
	{
		m_csTimes.Lock();
		RANGE_TM range = m_mapTimers.equal_range(p);
		for (it = range.first; it != range.second; )
		{
			if ((*it).second.pTimer == p && (*it).second.sKey == sKey)
			{
				(*it).second.ulSpan = max(m_ulMin,ulSpan);
			}
		}
		m_csTimes.Unlock();
	}
	catch(...)
	{
		m_csTimes.Unlock();
		CRLog(E_ERROR,"%s","Unknow Exception UpdateInterval");
	}
}

int CGessTimerMgr::ThreadEntry()
{
	try
	{
		while (!m_bEndThread)
		{
			m_csTimes.Lock();
			m_clkLastWait = clock();
			m_csTimes.Unlock();

			try
			{
				int nRtn = TimerWait(m_ulSpanNow);
				if (0 == nRtn)
				{
					clock_t clkDiff = m_ulSpanNow*CLOCKS_PER_SEC - (clock() - m_clkLastWait);
					if (clkDiff > 0)
						m_clkTotalDiff += clkDiff;

					if (m_clkTotalDiff >= 2*CLOCKS_PER_SEC)
					{
						unsigned long ulDiff = m_clkTotalDiff / CLOCKS_PER_SEC;

						_msleep(ulDiff);
						//CRLog(E_SYSINFO, "TimerWait span:%u", m_clkTotalDiff);
						m_clkTotalDiff = 0;
					}
					OnTimeout();
				}
				else
				{
					_msleep(m_ulSpanNow);
										
					//clock_t clkNow = clock();
					//CRLog(E_SYSINFO, "TimerWait nRtn:%d, span:%u", nRtn,(clkNow - m_clkLastWait));

					OnTimeout();
				}
			}
			catch(...)
			{
				CRLog(E_ERROR,"%s","Unknow Exception TimerWait/OnTimeout");
				_msleep(1);
			}
		}
		return 0;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknow Exception TimerMgr ThreadEntry");
		return -1;
	}
}