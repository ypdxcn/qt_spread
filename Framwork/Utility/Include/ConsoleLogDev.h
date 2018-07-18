#ifndef _ConsoleLogDev_H
#define _ConsoleLogDev_H

#include "LogDevice.h"
#include "Gess.h"

class UTIL_CLASS CConsoleLogDev : public CLogDevice
{
public:
	CConsoleLogDev(CLogger * pLogger);
	~CConsoleLogDev(void);

public:
	int Initial(CConfig * pCfg);
	int Open(void);
	void Close(void);
	void Finish();

	void Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText);
};
#endif