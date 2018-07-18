#ifndef _XQUEUE_IO_H
#define _XQUEUE_IO_H

#include <fstream>
#include <cassert>
#include "Workthread.h"
#include "XQueue.h"


#define WAIT_PERMIT_DEFAULT			5000000
#define BUFFER_PERMIT_DEFAULT		5000000

template<typename MSG>
class CXQueueCallback
{
public:
	CXQueueCallback(){};
	virtual ~CXQueueCallback(){};
	virtual int OnXQueuePkt(MSG& msg) = 0;
};

class CConfig;
template<typename MSG>
class CXQueueIo
{
private:
	template<typename MSG>
	class CWriter: public CWorkThread
	{
	public:
		CWriter(CXQueueIo<MSG>* p = 0):
		  m_pParent(p),
		  m_pConfig(0),
		  m_uiWaitPermit(WAIT_PERMIT_DEFAULT),
		  m_uiBufferPermit(BUFFER_PERMIT_DEFAULT),
		  m_uiMaxQuoPerWrite(1),
		  m_uiTest(0){}
		virtual ~CWriter(){}
		void Bind(CXQueueIo<MSG>* p){m_pParent = p;}
		int Init(CConfig* pConfig)
		{
			if (0 == pConfig)
				return 0;

			m_pConfig = pConfig;

			string sVal;
			if (0 == m_pConfig->GetProperty("WAIT_PERMIT", sVal))
			{
				m_uiWaitPermit = strutils::FromString<unsigned int>(sVal);
				//if (m_uiWaitPermit > WAIT_PERMIT_DEFAULT) 
				//	m_uiWaitPermit = WAIT_PERMIT_DEFAULT;
			}
			
			if (0 == m_pConfig->GetProperty("BUFFER_PERMIT", sVal))
			{
				m_uiBufferPermit = strutils::FromString<unsigned int>(sVal);
				//if (m_uiBufferPermit > BUFFER_PERMIT_DEFAULT) 
				//	m_uiBufferPermit = BUFFER_PERMIT_DEFAULT;
			}

			if (0 == m_pConfig->GetProperty("MAX_PER_WRITE", sVal))
			{
				m_uiMaxQuoPerWrite = strutils::FromString<unsigned int>(sVal);
				if (m_uiMaxQuoPerWrite > 10 || m_uiMaxQuoPerWrite <= 0)
					m_uiMaxQuoPerWrite = 1;
			}
			
			if (0 == m_pConfig->GetProperty("TEST", sVal))
			{
				m_uiTest = strutils::FromString<unsigned int>(sVal);
			}
			return 0;
		}
		int Enque(const MSG& oMsg)
		{
			m_deqCondMutex.Lock();
			m_deqMsg.push_back(oMsg);
			m_deqCondMutex.Unlock();
			m_deqCondMutex.Signal();
			return 0;
		}
	private:
		int ThreadEntry()
		{
			try
			{
				while(!m_bEndThread)
				{
					if (0 == m_pParent)
					{
						msleep(1);
						continue;
					}

					if (0 != m_pParent->IsOpen())
					{
						msleep(1);
						m_pParent->Open();
					}
					else
					{
						break;
					}
				}

				if (m_bEndThread)
					return 0;

				string sWriteVal;
				size_t nCount = 0;
				size_t nSize = 0;
				int nRtn = 0;
				while(!m_bEndThread)
				{
					m_deqCondMutex.Lock();
					while(m_deqMsg.empty() && !m_bEndThread)
						m_deqCondMutex.Wait();
					
					if (m_bEndThread)
					{
						m_deqMsg.clear();
						m_deqCondMutex.Unlock();
						break;
					}

					if (m_uiTest == 1)
					{
						if (sWriteVal.empty())
						{
							nSize = m_deqMsg.size();
							if (nSize >= m_uiBufferPermit)
							{
								for (unsigned int m = 1; m <= m_uiBufferPermit / 5; m++)
								{
									m_deqMsg.pop_front();
								}
							}
							else
							{
								nCount = min(m_uiMaxQuoPerWrite,nSize);	
								for (size_t i = 0; i < nCount; i++)
								{
									MSG& oMsg = m_deqMsg.front();
									unsigned int uiLen = 0;
									const char* pBuf = oMsg.Encode(uiLen);
									sWriteVal.append(pBuf, uiLen);
									m_deqMsg.pop_front();
								}
							}
						}
						m_deqCondMutex.Unlock();

						nRtn = m_pParent->Write(sWriteVal);
						if (0 != nRtn && nSize <= m_uiWaitPermit)
						{
							//CRLog(E_DEBUG, "wait timeout %u", nSize);
							continue;
						}
						sWriteVal.clear();
					}
					else
					{
						MSG& oMsg = m_deqMsg.front();
						nSize = m_deqMsg.size();
						if (nSize >= m_uiBufferPermit)
						{
							for (unsigned int m = 1; m <= m_uiBufferPermit / 5; m++)
							{
								m_deqMsg.pop_front();
							}
						}
						m_deqCondMutex.Unlock();
						
						nRtn = m_pParent->Send(oMsg);					
						if (0 != nRtn && nSize <= m_uiWaitPermit)
						{
							continue;
						}

						m_deqCondMutex.Lock();
						m_deqMsg.pop_front();
						m_deqCondMutex.Unlock();	
					}
				}

				CRLog(E_SYSINFO,"XQueueWriter Thread exit!");
				return 0;
			}
			catch(std::exception e)
			{
				CRLog(E_ERROR,"exception:%s!",e.what());
				return -1;
			}
			catch(...)
			{
				CRLog(E_ERROR,"Unknown exception!");
				return -1;
			}
		}
		int End()
		{
			m_deqCondMutex.Lock();
			m_deqCondMutex.Signal();
			m_deqCondMutex.Unlock();
			Wait();
			return 0;
		}
	private:
		CXQueueIo<MSG>* m_pParent;
		CConfig* m_pConfig;
		std::deque<MSG> m_deqMsg;
		CCondMutex	m_deqCondMutex;

		unsigned int	m_uiWaitPermit;
		unsigned int	m_uiBufferPermit;
		unsigned int	m_uiMaxQuoPerWrite;
		unsigned int	m_uiTest;
	};

	template<typename MSG>
	class CReader: public CWorkThread
	{
	public:
		CReader(CXQueueIo<MSG>* p = 0):m_pParent(p){}
		virtual ~CReader(){}
		void Bind(CXQueueIo<MSG>* p){m_pParent = p;}
	private:
		int ThreadEntry()
		{
			try
			{
				while(!m_bEndThread)
				{
					if (0 == m_pParent)
					{
						msleep(1);
						continue;
					}

					if (0 != m_pParent->IsOpen())
					{
						msleep(1);
						m_pParent->Open();
					}
					else
					{
						break;
					}
				}

				if (m_bEndThread)
					return 0;

				while(!m_bEndThread)
				{
					if (0 == m_pParent)
					{
						msleep(1);
						continue;
					}

					m_pParent->Recv();
				}

				CRLog(E_SYSINFO,"XQueueReader Thread exit!");
				return 0;
			}
			catch(std::exception e)
			{
				CRLog(E_ERROR,"exception:%s!",e.what());
				return -1;
			}
			catch(...)
			{
				CRLog(E_ERROR,"Unknown exception!");
				return -1;
			}
		}
		int End()
		{
			Wait();
			return 0;
		}
	private:
		CXQueueIo<MSG>* m_pParent;
	};

public:
	CXQueueIo(CXQueueCallback<MSG>* p = 0);
	~CXQueueIo(void);

	virtual int Init(CConfig* pConfig);
	virtual int Start();
	virtual void Stop();
	virtual void Finish();
	virtual int Bind(CXQueueCallback<MSG>* p);

	int IsOpen();
	int Read(MSG& oMsg, unsigned int uiTimeout = INFINITE);
	int Write(MSG& oMsg, unsigned int uiTimeout = INFINITE);
	int Write(string& sMsg, unsigned int uiTimeout = INFINITE);
	int Enque(MSG& oMsg);
private:
	int				m_nMode;				//0读 1写 
	CReader<MSG>	m_oReader;
	CWriter<MSG>	m_oWriter;
	CXQueueCallback<MSG>*	m_pXQueMgr;
	CConfig*		m_pCfg;

	CXQueue			m_Queue;
	string			m_sXQueName;
	unsigned int	m_uiMain;
	unsigned int	m_uiXQueLen;
	unsigned int	m_uiBlockSize;
	unsigned int	m_uiTimeout;
	char*			m_pBuf;

	unsigned int	m_uiPkts;

	int Open();
	int Recv();
	int Send(MSG& oMsg);
};


template<typename MSG>
CXQueueIo<MSG>::CXQueueIo(CXQueueCallback<MSG>* p)
:m_nMode(0)
,m_pXQueMgr(p)
,m_pCfg(0)
,m_sXQueName("XQUEUE_NAME")
,m_uiMain(0)
,m_uiXQueLen(XQUEUE_DEFAULT_QUEUE_SIZE)
,m_uiBlockSize(XQUEUE_DEFAULT_BLOCK_SIZE)
,m_uiTimeout(100)
,m_pBuf(0)
,m_uiPkts(0)
{
	 
}

template<typename MSG>
CXQueueIo<MSG>::~CXQueueIo(void)
{
	if (0 != m_pBuf)
		delete []m_pBuf;
}

//初始化配置，创建线程
template<typename MSG>
int CXQueueIo<MSG>::Init(CConfig* pCfg)
{
	assert(0 != pCfg);
	if (0 == pCfg)
		return -1;

	m_pCfg = pCfg;
		
	string sVal;
	if (0 == m_pCfg->GetProperty("XQUE_NAME", sVal))
	{
		m_sXQueName = sVal;
	}
	
	if (0 == m_pCfg->GetProperty("MODE", sVal))
	{
		m_nMode = strutils::FromString<unsigned int>(sVal);
		if (m_nMode != 0 && m_nMode != 1)
			m_nMode = 0;
	}

	if (0 == m_pCfg->GetProperty("MAIN", sVal))
	{
		m_uiMain = strutils::FromString<unsigned int>(sVal);
	}

	if (0 == m_pCfg->GetProperty("XQUE_LEN", sVal))
	{
		m_uiXQueLen = strutils::FromString<unsigned int>(sVal);
		if (m_uiXQueLen < XQUEUE_MIN_QUEUE_SIZE || m_uiXQueLen > XQUEUE_MAX_QUEUE_SIZE) 
			m_uiXQueLen = XQUEUE_DEFAULT_QUEUE_SIZE;
	}

	if (0 == m_pCfg->GetProperty("BLOCK_SIZE", sVal))
	{
		m_uiBlockSize = strutils::FromString<unsigned int>(sVal);
		if (m_uiBlockSize < XQUEUE_MIN_BLOCK_SIZE) 
			m_uiBlockSize = XQUEUE_MIN_BLOCK_SIZE;
	}

	if (0 == m_pCfg->GetProperty("XQUE_TIMEOUT", sVal))
	{
		m_uiTimeout = strutils::FromString<unsigned int>(sVal);
	}

	m_oWriter.Init(pCfg);

	m_pBuf = new char[m_uiBlockSize * m_uiXQueLen];
	memset(m_pBuf, 0x00, m_uiBlockSize * m_uiXQueLen);
	return 0;
}

//启动调度线程及工作线程
template<typename MSG>
int CXQueueIo<MSG>::Start()
{
	int nRtn = 0;
	if (m_uiMain == 1)
	{
		int nRet = m_Queue.Create(m_sXQueName.c_str(), m_uiXQueLen, m_uiBlockSize);
		if (nRet)
		{	
			CRLog(E_ERROR,"创建共享内存信息队列[%s]失败 %d", m_sXQueName.c_str(), nRet);
			nRtn = -1;
		}
	}
	else
	{
		int nRet = m_Queue.Open(m_sXQueName.c_str());
		if (!m_Queue.IsOpen())
		{	
			CRLog(E_ERROR,"打开共享内存信息队列[%s]失败 %d", m_sXQueName.c_str(), nRet);
			nRtn = 0;
		}
	}

	//启动调度线程
	if (m_nMode == 0)
	{
		m_oReader.Bind(this);
		m_oReader.BeginThread();
	}
	else if (m_nMode == 1)
	{
		m_oWriter.Bind(this);
		m_oWriter.BeginThread();
	}
	return nRtn;
}

//停止工作线程池及调度线程
template<typename MSG>
void CXQueueIo<MSG>::Stop()
{
	//停止调度线程
	CRLog(E_APPINFO,"%s","Stop Reader/Writer Thread");
	if (m_nMode == 0)
	{
		m_oReader.EndThread();
	}
	else if (m_nMode == 1)
	{
		m_oWriter.EndThread();
	}
	m_Queue.Close();
}

//清理资源
template<typename MSG>
void CXQueueIo<MSG>::Finish()
{	
	if (0 != m_pBuf)
	{
		delete []m_pBuf;
		m_pBuf = 0;
	}
}

template<typename MSG>
int CXQueueIo<MSG>::Bind(CXQueueCallback<MSG>* p)
{
	m_pXQueMgr = p;
	return 0;
}

template<typename MSG>
int CXQueueIo<MSG>::IsOpen()
{
	if (m_Queue.IsOpen())
		return 0;
	else
		return -1;
}

template<typename MSG>
int CXQueueIo<MSG>::Open()
{
	int nRtn = 0;
	if (m_uiMain == 1)
	{
		int nRet = m_Queue.Create(m_sXQueName.c_str(), m_uiXQueLen, m_uiBlockSize);
		if (nRet)
		{
			nRtn = -1;
		}
		else
		{
			CRLog(E_APPINFO, "创建XQUEUE[%s]成功!", m_sXQueName.c_str());
		}
	}
	else
	{
		int nRet = m_Queue.Open(m_sXQueName.c_str());
		if (!m_Queue.IsOpen())
		{	
			nRtn = -1;
		}
		else
		{
			CRLog(E_APPINFO, "打开XQUEUE[%s]成功!", m_sXQueName.c_str());
		}
	}
	return nRtn;
}

template<typename MSG>
int CXQueueIo<MSG>::Enque(MSG& oMsg)
{	
	return m_oWriter.Enque(oMsg);
}

template<typename MSG>
int CXQueueIo<MSG>::Read(MSG& oMsg, unsigned int uiTimeout)
{
	try
	{
		int nRtn = -1;
		size_t nSize = m_uiBlockSize * m_uiXQueLen;
		size_t nMessageId = 0;

		long lRet = m_Queue.Read((LPBYTE)m_pBuf, &nSize, &nMessageId, NULL, m_uiTimeout);			
		switch (lRet)
		{
		case XQUEUE_ERROR_SUCCESS:
			nRtn = 0;
			break;
		case XQUEUE_ERROR_TIMEOUT:
			return -1;//XQUEUE_ERROR_TIMEOUT;
		case XQUEUE_ERROR_NO_DATA:
			return -1;//XQUEUE_ERROR_TIMEOUT;
		default:
			CRLog(E_ERROR, "读错误!(%d)",lRet);
			return -1;//(int)lRet;
		}

		oMsg.Decode(m_pBuf, nSize);	
		return 0;
	}
	catch(std::exception e)
	{
		CRLog(E_ERROR,"exception:%s!",e.what());
		return -1;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknown exception!");
		return -1;
	}
}

template<typename MSG>
int CXQueueIo<MSG>::Write(MSG& oMsg, unsigned int uiTimeout)
{
	try
	{
		int nRtn = -1;
		if (m_Queue.IsOpen())
		{
			size_t nSize = 0;
			const char * pBuf = oMsg.Encode(nSize);
			if (nSize > 0)
			{
				long lRet = m_Queue.Write((LPBYTE)(pBuf), nSize, m_uiTimeout);
				switch (lRet)
				{
				case XQUEUE_ERROR_SUCCESS:				
					nRtn = 0;
					break;
				case XQUEUE_ERROR_TIMEOUT:
					nRtn = -1;
					break;
				default:
					CRLog(E_ERROR, "向共享内存消息写消息失败(%d)", lRet);
					nRtn = -1;
					break;
				}
			}
		}

		return nRtn;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknown exception!");
		return -1;
	}
}

template<typename MSG>
int CXQueueIo<MSG>::Write(string& sMsg, unsigned int uiTimeout)
{
	try
	{
		int nRtn = -1;
		if (m_Queue.IsOpen())
		{
			long lRet = m_Queue.Write((LPBYTE)(sMsg.data()), sMsg.length(), m_uiTimeout);
			switch (lRet)
			{
			case XQUEUE_ERROR_SUCCESS:				
				nRtn = 0;
				break;
			case XQUEUE_ERROR_TIMEOUT:
				nRtn = -1;
				break;
			default:
				CRLog(E_ERROR, "向共享内存消息写消息失败(%d)", lRet);
				nRtn = -1;
				break;
			}
		}

		return nRtn;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknown exception!");
		return -1;
	}
}

template<typename MSG>
int CXQueueIo<MSG>::Send(MSG& oMsg)
{
	int nRtn = Write(oMsg, m_uiTimeout);
	if (0 == nRtn)
		m_uiPkts++;

	return nRtn;
}

template<typename MSG>
int CXQueueIo<MSG>::Recv()
{
	try
	{
		MSG oMsg;
		int nRtn = Read(oMsg, m_uiTimeout);
		if (nRtn != 0)
			return nRtn;

		if (0 != m_pXQueMgr)
			m_pXQueMgr->OnXQueuePkt(oMsg);

		m_uiPkts++;
		if (m_uiPkts % 5000 == 0)
		{
			//CRLog(E_STATICS, "Read pkts:%u From %s", m_uiPkts, m_sXQueName.c_str());
		}
		return 0;
	}
	catch(std::exception e)
	{
		CRLog(E_ERROR,"exception:%s!",e.what());
		return -1;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknown exception!");
		return -1;
	}
}
#endif