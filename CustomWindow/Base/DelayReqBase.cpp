#include "DelayReqBase.h"

CReqHandlerBase::CReqHandlerBase(void)
{
//kenny  20180424
#ifdef  _WIN32
    m_nDelayTime = 700;
#else
    m_nDelayTime = 2;
#endif

}

CReqHandlerBase::~CReqHandlerBase(void)
{
}

int CReqHandlerBase::ThreadEntry()
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

		m_deqCondMutex.Unlock();

	
//kenny  20180424
#ifdef  _WIN32
    	Sleep(m_nDelayTime);
#else
    sleep(m_nDelayTime);
#endif

		// 将所有的复制，然后清空原来的请求
		deque<ReqPara> deqReq;
		m_deqCondMutex.Lock();
		deqReq = m_deqReq;
		m_deqReq.clear();
		m_deqCondMutex.Unlock();

		// 发送请求
		SendReq(deqReq);
	}

	return 0;
}

void CReqHandlerBase::AddReq( const ReqPara &stReq )
{
	m_deqCondMutex.Lock();

	// 遍历
	bool bAdd = true;
	for( std::deque<ReqPara>::iterator it = m_deqReq.begin(); it != m_deqReq.end(); it++ )
	{
		if( it->eReqType == stReq.eReqType )
		{
			// 如果是无效，则不添加
			if( IsUseless(*it,stReq) )
			{
				bAdd = false;
				break;
			}
		}
	}
	if( bAdd )
	{
		m_deqReq.push_back(stReq);
		m_deqCondMutex.Signal();
	}

	m_deqCondMutex.Unlock();
}

int CReqHandlerBase::End()
{
	m_bEndThread = true; 
	return 0;
}

bool CReqHandlerBase::IsUseless( ReqPara &stReqInQue, const ReqPara &stReqAdd )
{
	return true;
}

void CReqHandlerBase::SendReq( const deque<ReqPara> &deqReq )
{
	
}

void CReqHandlerBase::SetDelayTime( int nTime )
{
	m_nDelayTime = nTime;
}
