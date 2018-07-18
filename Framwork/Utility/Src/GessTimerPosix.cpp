#include "GessTimerMgrPosix.h"


unsigned long CGessTimerMgrImp::Min(){return 1UL;}

int CGessTimerMgrImp::TimerWait(unsigned long ulSpanNow)
{
	struct timespec tInterval;
	tInterval.tv_sec = time(0) + ulSpanNow;
	tInterval.tv_nsec = 0;
	m_condTimerSignal.Lock();
	int nRtn = m_condTimerSignal.TimeWaitAbs(tInterval);
	m_condTimerSignal.Unlock();

	if (ETIMEDOUT == nRtn)
		nRtn = 0;

	return nRtn;
}

int CGessTimerMgrImp::End()
{
	//应用层定时处理函数无法知道是否阻塞,暂不同步wait
	return 0;
}