#include "ConsoleLogDev.h"

CConsoleLogDev::CConsoleLogDev(CLogger * pLogger) : CLogDevice(pLogger)
{
}

CConsoleLogDev::~CConsoleLogDev(void)
{
}

int CConsoleLogDev::Initial(CConfig * pCfg)
{
	return CLogDevice::Initial(pCfg);
}

int CConsoleLogDev::Open(void)
{
	return 0;
}

void CConsoleLogDev::Close(void)
{
}

void CConsoleLogDev::Finish()
{
	CLogDevice::Finish();
}

void CConsoleLogDev::Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText)
{
	if (!m_nLogLevel.test(nLevel))
		return;

	DevLock();
	printf("%s %s %s\n", CurrentTime().c_str(), LevelString(nLevel), cszText);
	
	DevUnlock();
}
