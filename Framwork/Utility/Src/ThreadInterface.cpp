/******************************************************************************
版    权:深圳市雁联计算系统有限公司.
模块名称:WorkThread.cpp
创建者	:张伟
创建日期:2008.07.22
版    本:1.0				
模块描述:封装线程api
主要函数:BeginThread() 启动线程
         EndThread()  终止线程
		 ThreadEntry() 子类需实现的需函数
修改记录:
******************************************************************************/

#include "ThreadInterface.h"


CThreadInferface::CThreadInferface()
:m_bRunning(false)
,m_bEndThread(false)
{
}

CThreadInferface::~CThreadInferface() 
{
    //if (m_bRunning) 
    //    EndThread();

}

int CThreadInferface::BeginThread ( )
{
    if (m_bRunning) 
        return 0;

    m_bEndThread = false;
	m_bRunning = true;
	
	return pthread_create(&m_tThread, 0, InvokeThreadEntry, this);
	
}

void CThreadInferface::EndThread ()
{
    if (m_bRunning)
    {
        m_bEndThread = true;    // Notify thread to die	

		End();
        m_bRunning = false;
    }
}

void * CThreadInferface::InvokeThreadEntry(void * pArg)
{
    CThreadInferface *pWorkThread = reinterpret_cast<CThreadInferface*>(pArg);

	if( pWorkThread )
	{
		pWorkThread->OnStart();

		//线程函数
		int uRtn = pWorkThread->ThreadEntry();

		pWorkThread->OnStop();
	}

	return 0;
}

void CThreadInferface::Wait()
{
	pthread_t tid = pthread_self();
	if ( 0 == pthread_equal(m_tThread,tid))
		pthread_join(m_tThread, 0);
}