#ifndef _LOGGER_H
#define _LOGGER_H

#include "Singleton.h"
#include "Gess.h"
#include <cstdarg>

#ifdef AIX
#define CRLog(level, fmt, ...) \
	do \
	{ \
		CLogger * logger = CLogger::Instance(); \
		logger->Log(__FUNCTION__, __LINE__, level, 0, fmt, __VA_ARGS__); \
	} while (0)
#else
#define CRLog(level, fmt, ...) \
	do \
	{ \
		CLogger * logger = CLogger::Instance(); \
		logger->Log(__FUNCTION__, __LINE__, level, 0, fmt, ##__VA_ARGS__); \
	} while (0)
#endif

class CSubscriber;
class CLogDevice;
class CNetLogDev;
class UTIL_CLASS CLogger:public CSingleton< CLogger >
{
	friend class CSingleton< CLogger >;
protected:
	CLogger(void);
	virtual ~CLogger(void);

public:
	int Initial(CConfig * pCfg);
	int Start();
	void Stop();
	void Finish();

	int Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, int nReturn, const char * cszFmt, ...);
	void Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText);
	
	bool RegisterNetLog(CSubscriber* p);
	void UnRegisterNetLog(CSubscriber* p);
private:
	vector<CLogDevice *> m_vecDevs;

	CNetLogDev*			 m_pNetLogDev;

	int m_bStop;
};
#endif