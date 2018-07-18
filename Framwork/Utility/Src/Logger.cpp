/******************************************************************************
版    权:深圳市雁联计算系统有限公司.
模块名称:Logger.cpp
创建者	:张伟
创建日期:2008.07.22
版    本:1.0				
模块描述:封装日志记录,单例
主要函数:GetInstance() 获取日志类实例
         Log() 用于日志记录的接口函数
修改记录:
******************************************************************************/
#include "NetLogDev.h"
#include "ConsoleLogDev.h"
#include "FileLogDev.h"
#include "Logger.h"
#include "osdepend.h"
#include <iostream>


using namespace std;

CLogger::CLogger(void)
:m_pNetLogDev(0)
 ,m_bStop(true)
{
}

CLogger::~CLogger(void)
{
}

int CLogger::Initial(CConfig * pCfg)
{
	vector<string> vValues;
	string sValue;

	pCfg->GetProperty("type", vValues);
	if (vValues.size() < 1)
	{
		cout << "日志配置项错误" << endl;
		return -1;
	}

	size_t nSize = vValues.size();
	cout << nSize << "个日志设备" << endl;

	CLogDevice * pLogDev = 0;
	for (size_t i = 0; i < nSize; i++)
	{   // 各种日志设备
		cout << "日志设备" << vValues[i] << endl;
		
		CConfig * pTypeCfg = pCfg->GetProperties(vValues[i]);
		if (pTypeCfg == 0)
		{
			cout << "日志设备" << vValues[i] << "缺少配置" << endl;
			continue;
		}
		// 创建各种设备实例
		pTypeCfg->GetProperty("device", sValue);
		if (sValue.compare("console") == 0)
		{
			pLogDev = new CConsoleLogDev(this);
			m_vecDevs.push_back(pLogDev);
		}
		else if (sValue.compare("file") == 0)
		{
			pLogDev = new CFileLogDev(this);
			m_vecDevs.push_back(pLogDev);
		}
		else if (sValue.compare("netlog") == 0)
		{
			m_pNetLogDev = new CNetLogDev(this);			
			pLogDev = m_pNetLogDev;
			m_vecDevs.push_back(pLogDev);
		}
		else
		{
			cout << "目前不支持类型为" << sValue << "的日志设备" << endl;
			continue;
		}

		if (pLogDev->Initial(pTypeCfg) != 0)
		{
			cout << "日志设备" << vValues[i] << "初始化失败" << endl;
			continue;
		}
	}

	return 0;
}


void CLogger::Finish()
{
	Stop();

	for (vector<CLogDevice *>::iterator it = m_vecDevs.begin(); it != m_vecDevs.end(); ++it)
	{
		if (*it != 0)
		{
			(*it)->Finish();
			delete *it;
		}
	}
	m_pNetLogDev = 0;
}

int CLogger::Start()
{
	// 简单避免重复启动
	if (!m_bStop)
		return 0;

	cout << "启动日志" << endl;

	for (vector<CLogDevice *>::iterator it = m_vecDevs.begin(); it != m_vecDevs.end(); ++it)
	{
		if (*it != 0)
		{
			if ((*it)->Open() != 0)
			{   // 打开失败，清除该设备
				(*it)->Finish();
				delete *it;
			}
		}
	}

	m_bStop = false;
	return 0;
}

void CLogger::Stop()
{
	m_bStop = true;
}

int CLogger::Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, int nReturn, const char * cszFmt, ...)
{
	va_list ap;

	char szBuf[MAX_LOG_TEXT];
	va_start(ap, cszFmt);
	vsnprintf(szBuf, sizeof(szBuf) - 1, cszFmt, ap);
	va_end(ap);

	Log(cszFile, nLine, nLevel, szBuf);

	return nReturn;
}

void CLogger::Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText)
{
	if (m_bStop)
	{
		
	}
	else
	{   // 使用日志设备记录
		for (vector<CLogDevice *>::iterator it = m_vecDevs.begin(); it != m_vecDevs.end(); ++it)
		{
			if ((*it) != 0)
				(*it)->Log(cszFile, nLine, nLevel, cszText);
		}
	}
}

bool CLogger::RegisterNetLog(CSubscriber* p)
{
	if (0 == m_pNetLogDev)
		return false;

	m_pNetLogDev->Register(p);
	return true;
}

void CLogger::UnRegisterNetLog(CSubscriber* p)
{
	if (0 == m_pNetLogDev)
		return;

	m_pNetLogDev->UnRegister(p);
}