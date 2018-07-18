
#include "RealTimeReqBase.h"

CRealTimeReqBase::CRealTimeReqBase(void)
{
}

CRealTimeReqBase::~CRealTimeReqBase(void)
{
}

int CRealTimeReqBase::ThreadEntry()
{
	while( !m_bEndThread )
	{
		m_deqCondMutex.Lock();
		while(m_deqReq.empty() && !m_bEndThread)
			m_deqCondMutex.Wait();

		if (m_bEndThread)
		{
			m_deqReq.clear();
			m_deqCondMutex.Unlock();
			break;
		}

		if ( !m_deqReq.empty())
		{
			RTReqPara stReq = m_deqReq.front();
			m_deqReq.pop_front();
			m_deqCondMutex.Unlock();

			// 发送请求
			int nRet = SendReq(stReq);

			// 增加已请求次数
			stReq.nCurReqCount++;

			// 如果处理失败而且尝试的次数没有溢出，则重新添加
			if( nRet != 0 )
			{
				if( stReq.nCurReqCount < stReq.nFailTry )
				{
					AddReq(stReq);
				}
			}
		}
		else
		{
			m_deqCondMutex.Unlock();
		}
	}

	return 0;
}

void CRealTimeReqBase::AddReq( const RTReqPara &stReq )
{
	m_deqCondMutex.Lock();
	m_deqReq.push_back(stReq);
	m_deqCondMutex.Signal();
	m_deqCondMutex.Unlock();
}

int CRealTimeReqBase::End()
{
	m_bEndThread = true; 
	return 0;
}