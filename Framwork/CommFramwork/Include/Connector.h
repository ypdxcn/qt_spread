#ifndef _CONNECTOR_H
#define _CONNECTOR_H

#include "WorkThreadNm.h"
#include "Comm.h"

class CProtocolCommClient;
class COMM_CLASS CConnector:public CWorkThreadNm,public CAction
{
public: 
	CConnector(CProtocolCommClient *p);
	virtual ~CConnector();

	int Init(CConfig *	pConfig);
	void Finish();
	int Start();
	void Stop();

	//通知连接
	void NotifyConnect();
private: 
	int ThreadEntry();
	int End();

	//主动类线程状态是否需要被网管
	bool IsNetManaged(string& sKeyName);

	unsigned long m_ulKey;
	vector< pair<string,int> > m_vPair;
	size_t m_nIndex;

	CProtocolCommClient * m_pCommClient;
	CCondMutex m_condMutexConnect;

	CGessSem	m_oSem;
	void V() {m_oSem.V();}
	void P() {m_oSem.P();}

	static unsigned long m_ulSID;
	static CGessMutex    m_mutexSID;
};
#endif