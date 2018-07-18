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

#include "WorkThreadNm.h"

unsigned int CWorkThreadNm::m_uiSID = 0;
CGessMutex CWorkThreadNm::m_mutexSID;

CWorkThreadNm::CWorkThreadNm()
:m_blNetManaged(false)
,m_uiKey(0)
,m_sKey("")
,m_sMibStateIns(gc_sThreadState)
{
	m_mutexSID.Lock();
	m_uiKey = ++m_uiSID;
	m_mutexSID.Unlock();
}

CWorkThreadNm::~CWorkThreadNm() 
{
    //if (m_bRunning) 
    //    EndThread();

}

int CWorkThreadNm::BeginThread ( )
{
    if (IsWorking()) 
        return 0;

	m_oWorkerNm.Bind(this);
	m_blNetManaged = IsNetManaged(m_sKey);
	if (m_blNetManaged)
		m_sMibStateIns = gc_sThreadState + "." + m_sKey + "." + ToString<unsigned int>(m_uiKey);
	
	return CThreadInferface::BeginThread();	
}

void CWorkThreadNm::OnStart()
{
	//如果需要被管理则注册 上报 
	if (m_blNetManaged)
	{
		CNMO oNmo;
		oNmo.m_sOid = gc_sThreadState;
		oNmo.m_sOidIns = m_sMibStateIns;
		oNmo.m_sValue = ToString<int>(gc_nThreadRun);
		oNmo.m_nQuality = gc_nQuolityGood;
		oNmo.m_sTimeStamp = CGessDate::NowToString() + " " + CGessTime::NowToString();

		CNetMgr::Instance()->Register(&m_oWorkerNm,oNmo.m_sOid,oNmo.m_sOidIns);
		CNetMgr::Instance()->Report(oNmo);
	}
}

void CWorkThreadNm::OnStop()
{
	//如果需要被管理则上报 注销
	if (m_blNetManaged)
	{
		CNMO oNmo;
		oNmo.m_sOid = gc_sThreadState;
		oNmo.m_sOidIns = m_sMibStateIns;
		if (!m_bEndThread)
		{//异常退出注销前上报且触发告警,否则直接注销
			oNmo.m_sValue = ToString<int>(gc_nThreadQuit);
			oNmo.m_nQuality = gc_nQuolityGood;
			oNmo.m_sTimeStamp = CGessDate::NowToString() + " " + CGessTime::NowToString();
			CNetMgr::Instance()->Report(oNmo);
		}
		CNetMgr::Instance()->UnRegisterNmo(&m_oWorkerNm,oNmo.m_sOidIns);
		CNetMgr::Instance()->UnRegisterModule(&m_oWorkerNm);
	}

}

//单个查询接口
int CWorkThreadNm::ThreadQuery(CNMO& oNmo) const
{
	if (oNmo.m_sOidIns == m_sMibStateIns)
	{
		oNmo.m_sValue = ToString<int>(!m_bEndThread?gc_nThreadRun:gc_nThreadQuit);
		oNmo.m_sTimeStamp = CGessDate::NowToString() + " " + CGessTime::NowToString();
		oNmo.m_nQuality = gc_nQuolityGood;
	}
	return 0;
}

//批量查询接口
int CWorkThreadNm::ThreadQuery(vector< CNMO > & vNmo) const
{
	for (vector< CNMO >::iterator it = vNmo.begin(); it != vNmo.end(); ++it)
	{
		ThreadQuery(*it);
	}
	return 0;
}