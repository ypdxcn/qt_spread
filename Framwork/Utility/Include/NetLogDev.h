#ifndef _NetLogDev_H
#define _NetLogDev_H

#include "LogDevice.h"
#include "Gess.h"

//
class UTIL_CLASS CSubscriber
{
public:
	virtual void OnNotify(const string& sMsg) = 0;
};

class UTIL_CLASS CPublisher
{
public:
	virtual void Register(CSubscriber* p) = 0;
	virtual void UnRegister(CSubscriber* p) = 0;
};

class UTIL_CLASS CNetLogDev : public CLogDevice,public CPublisher
{
public:
	CNetLogDev(CLogger * pLogger);
	~CNetLogDev(void);

public:
	int Initial(CConfig * pCfg);
	int Open(void);
	void Close(void);
	void Finish();

	void Register(CSubscriber* p);
	void UnRegister(CSubscriber* p);

	void Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText);

private:
	vector<CSubscriber*>  m_vecSubscribers;

};
#endif