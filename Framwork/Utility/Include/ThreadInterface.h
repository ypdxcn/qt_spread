#ifndef _THREAD_INTERFACE_H
#define _THREAD_INTERFACE_H

//#include <pthread.h>
#include "Gess.h"

class UTIL_CLASS  CThreadInferface
{
public:
    CThreadInferface ();
    virtual ~CThreadInferface();

    virtual int BeginThread();
    virtual void EndThread();

	bool IsWorking(){return 0 != m_bRunning;}
	
protected:
	virtual	int ThreadEntry() = 0;
	virtual	int End() = 0;
	virtual void OnStart() = 0;
	virtual void OnStop() = 0;
	void Wait();

	pthread_t		m_tThread;
    int	            m_bEndThread;

private:
	int				m_bRunning;	
	static void * InvokeThreadEntry(void * pArg);
}; 
#endif