#include "osdepend.h"
#include "LogDevice.h"
#include "Logger.h"
#include "GessDate.h"
#include "GessTime.h"
#include <iostream>

using namespace std;

CLogDevice::LEVEL_VALUE CLogDevice::m_LevelValues[] =
{
	{"E_CRITICAL", E_CRITICAL},
	{"E_ERROR", E_ERROR},
	{"E_WARNING", E_WARNING},
	{"E_NOTICE", E_NOTICE},
	{"E_SYSINFO", E_SYSINFO},	
	{"E_PROINFO", E_PROINFO},
	{"E_APPINFO" , E_APPINFO},
	{"E_DEBUG", E_DEBUG},
	{"E_STATICS",E_STATICS}
};

CLogDevice::CLogDevice(CLogger * pLogger)
{
	m_nLogLevel.reset();
	m_pLogger = pLogger;
}

CLogDevice::~CLogDevice(void)
{
}

int CLogDevice::Initial(CConfig * pCfg)
{
	// 解析日志级别
	vector<string> vValues;
	vector<string>::iterator it;

	pCfg->GetProperty("level", vValues);
	
	if (vValues.size() < 1)
	{
		cout << "日志级别配置错误" << endl;
		return -1;
	}

	m_nLogLevel.reset();
	for (it = vValues.begin(); it != vValues.end(); it++)
	{
		GESS_LOG_LEVEL eLevel = LevelValue(*it);
		if(eLevel != E_LOG_LEVEL_MAX)
		{
			m_nLogLevel.set(LevelValue(*it));
		}
	}

	return 0;
}

void CLogDevice::Finish()
{
}

GESS_LOG_LEVEL CLogDevice::LevelValue(const std::string & sLevel)
{
	int nSize = sizeof(m_LevelValues)/sizeof(LEVEL_VALUE);
	for (int i = 0; i < nSize; i++)
	{
		if (sLevel.compare(m_LevelValues[i].str) == 0)
			return m_LevelValues[i].val;
	}

	return E_LOG_LEVEL_MAX;
}

char * CLogDevice::LevelString(GESS_LOG_LEVEL nLevel)
{
	int nSize = sizeof(m_LevelValues)/sizeof(LEVEL_VALUE);
	for (int i = 0; i < nSize; i++)
	{
		if (nLevel == m_LevelValues[i].val)
			return m_LevelValues[i].str;
	}

	return "E_UNKNOWN";
}

//当前时间
string CLogDevice::CurrentTime()
{
	string sDateTime = CGessDate::NowToString() + " " + CGessTime::NowToString(":");
	return sDateTime;
}

void CLogDevice::DevLock()
{
	m_csLog.Lock();
}

void CLogDevice::DevUnlock()
{
	m_csLog.Unlock();
}
