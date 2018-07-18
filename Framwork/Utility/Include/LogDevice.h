#ifndef _LOGDEVICE_H
#define _LOGDEVICE_H

#include "Gess.h"
using namespace std;

#define MAX_LOG_TEXT	15 * 1024
#define BIT_SET_LEN		32

class CLogger;
class UTIL_CLASS CLogDevice
{
public:
	CLogDevice(CLogger * pLogger);
	~CLogDevice(void);

protected:
	std::bitset<BIT_SET_LEN> m_nLogLevel;
	CLogger * m_pLogger;

public:
	virtual int Initial(CConfig * pCfg);
	virtual int Open(void) = 0;
	virtual void Close(void) = 0;
	virtual void Finish();

	virtual void Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText) = 0;

private:
	typedef struct tagLevelValue
	{
		char * str;
		GESS_LOG_LEVEL val;
	} LEVEL_VALUE;
	static LEVEL_VALUE m_LevelValues[];
	CGessMutex m_csLog;

protected:
	GESS_LOG_LEVEL LevelValue(const std::string & sLevel);
	char * LevelString(GESS_LOG_LEVEL nLevel);
	string CurrentTime();
	void DevLock();
	void DevUnlock();
};
#endif