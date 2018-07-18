#ifndef _GESS_TIMER_POSIX_H_
#define _GESS_TIMER_POSIX_H_ 

#include "GessTimerMgr.h"
#include "Gess.h"
#include "Singleton.h"

using namespace std;

class UTIL_CLASS CGessTimerMgrImp: public CGessTimerMgr,public CSingleton< CGessTimerMgrImp >
{
	friend class CSingleton< CGessTimerMgrImp >;
protected:
	CGessTimerMgrImp(){}
	virtual ~CGessTimerMgrImp(){}
    
protected:
	unsigned long Min();
	int TimerWait(unsigned long ulSpanNow);
	int End();

private:
	CCondMutex	m_condTimerSignal;
};

#endif