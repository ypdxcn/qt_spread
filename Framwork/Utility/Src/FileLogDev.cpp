#include "FileLogDev.h"
#include "Logger.h"
#include <iostream>

using namespace std;

#define FILELOG_MAX_SIZE	8192

CFileLogDev::CFileLogDev(CLogger * pLogger) 
: CLogDevice(pLogger)
,m_nMaxSize(FILELOG_MAX_SIZE * 1024) // 以K字节为单位
{
	m_sFilePathAbs = ".";
	m_sFilePathAbs += PATH_SLASH;
	m_sFilePathAbs += DEFUALT_LOG_PATH PATH_SLASH;
	m_sFilePathRel = "";
	m_sFilePath = m_sFilePathAbs + m_sFilePathRel;

	m_sPrefix = "";
	m_sPostfix = ".log";

	for (int i = 0; i < E_LOG_LEVEL_MAX; i++)
	{
		m_stLevelLogFile[i] = 0;
	}
}

CFileLogDev::~CFileLogDev(void)
{
}

int CFileLogDev::Initial(CConfig * pCfg)
{
	if (CLogDevice::Initial(pCfg) != 0)
		return -1;

	char szPathBuf[2048];
	getcwd(szPathBuf,sizeof(szPathBuf));
	m_sFilePathAbs = szPathBuf;
	m_sFilePathAbs += PATH_SLASH ;
	m_sFilePathAbs += DEFUALT_LOG_PATH PATH_SLASH;
	m_sFilePath = m_sFilePathAbs + m_sFilePathRel;

	m_nMaxSize = 0;
	string sValue("0");
	if (0 == pCfg->GetProperty("maxsize", sValue))
	{
		m_nMaxSize = FromString<unsigned int>(sValue);
	}
	if (m_nMaxSize == 0 || m_nMaxSize > 100 * 1024)
	{
		cout << "日志文件最大尺寸未配置，采用默认值" << FILELOG_MAX_SIZE << "KB" << endl;
		m_nMaxSize = FILELOG_MAX_SIZE * 1024; // 以K字节为单位
	}
	else
	{
		cout << "日志文件最大尺寸设置为" << m_nMaxSize << "KB" << endl;
		m_nMaxSize = m_nMaxSize * 1024;
	}

	sValue.clear();
	if (0 == pCfg->GetProperty("path", sValue))
	{
		bool blFlag = false;
		string::size_type iPos = 0;
		for (iPos = sValue.length() - 1; iPos >= 0; iPos--)
		{
			if (blFlag)
			{
				if (sValue[iPos] != '/' && sValue[iPos] != '\\')
				{
					break;
				}
			}
			else
			{
				if (sValue[iPos] == '/' || sValue[iPos] == '\\')
				{
					blFlag = true;
				}
				else
				{
					break;
				}
			}
		}

		if (iPos >= 0)
		{
			m_sFilePathAbs = sValue.substr(0,iPos + 1) + PATH_SLASH;
		}
	}

	if (0 == pCfg->GetProperty("prefix", sValue))
	{
		m_sPrefix = trim(sValue);
		if (m_sPrefix != "")
			m_sPrefix += "_";
	}

	if (0 == pCfg->GetProperty("postfix", sValue))
	{
		m_sPostfix = trim(sValue);
		if (m_sPostfix != "")
			m_sPostfix = "." + m_sPostfix;
	}

	int nLevelNum = 0;
	if (0 == pCfg->GetProperty("num", sValue))
	{
		nLevelNum = FromString<int>(sValue);
	}

	//缺省日志文件
	LOG_FILE* pLogFileDef = new LOG_FILE;
	pLogFileDef->nFileSize = 0;
	pLogFileDef->pfLogFile = 0;
	pLogFileDef->nFlag = 1;
	pLogFileDef->sFileName = "LogDef";

	string sFullName = m_sPrefix + "LogDef" + m_sPostfix;
	m_mapLogFile[sFullName] = pLogFileDef;
	
	for (int i = 1; i <= nLevelNum; i++)
	{
		GESS_LOG_LEVEL eLevel = E_LOG_LEVEL_MAX;
		string sName = "level" + ToString<int>(i);
		if (0 == pCfg->GetProperty(sName, sValue))
		{
			eLevel = LevelValue(sValue);
			if (E_LOG_LEVEL_MAX == eLevel)
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		string sFile = "file" + ToString<int>(i);
		if (0 == pCfg->GetProperty(sFile, sValue))
		{
			sFullName = m_sPrefix + trim(sValue) + m_sPostfix;
			map<string,LOG_FILE *>::iterator it = m_mapLogFile.find(sFullName);
			if (it == m_mapLogFile.end())
			{
				LOG_FILE* pLogFile = new LOG_FILE;
				pLogFile->nFileSize = 0;
				pLogFile->pfLogFile = 0;
				pLogFile->nFlag = 0;
				pLogFile->sFileName = trim(sValue);

				m_mapLogFile[sFullName] = pLogFile;
				m_stLevelLogFile[eLevel] = pLogFile;
			}
			else
			{
				m_stLevelLogFile[eLevel] = (*it).second;
			}
		}
		else
		{
			
		}
	}

	for (int j = 0; j < E_LOG_LEVEL_MAX; j++)
	{
		if (0 == m_stLevelLogFile[j])
		{
			m_stLevelLogFile[j] = pLogFileDef;
		}
	}
	return 0;
}

void CFileLogDev::Finish()
{
	CLogDevice::Finish();
	
	map<string,LOG_FILE *>::iterator it;
	for (it = m_mapLogFile.begin(); it != m_mapLogFile.end(); ++it)
	{
		delete (*it).second;
	}
	m_mapLogFile.clear();
}

int CFileLogDev::Open(void)
{
	m_sFilePathRel = CGessDate::NowToString();
	m_sFilePathRel += PATH_SLASH;
	m_sFilePath = m_sFilePathAbs + m_sFilePathRel;

	if (0 != MKDIR(m_sFilePath.c_str(),S_IRWXU | S_IRWXG | S_IRWXO))
	{
		//cout << "\"" << m_sFilePath << "\"" << "日志目录已存在!" << endl;
	}

	map<string,LOG_FILE*>::iterator it;
	for (it = m_mapLogFile.begin(); it != m_mapLogFile.end(); ++it)
	{
		string sFileFull = m_sFilePath + m_sPrefix + (*it).second->sFileName + m_sPostfix;
		(*it).second->pfLogFile = fopen(sFileFull.c_str(), "a");
		if(!(*it).second->pfLogFile)
		{
			cout << "日志文件:" << sFileFull << " 打开失败, ERRNO:" <<  errno << endl;
			continue;
		}

		struct stat st;
		if (stat(sFileFull.c_str(), &st) == 0)
		{
			(*it).second->nFileSize = st.st_size;
		}
		else
		{
			cout << "日志文件" << sFileFull << "获取尺寸失败" << endl;
		}
	}
	return 0;
}

void CFileLogDev::Close(void)
{
	map<string,LOG_FILE*>::iterator it;
	for (it = m_mapLogFile.begin(); it != m_mapLogFile.end(); ++it)
	{
		if((*it).second->pfLogFile)
		{
			fclose((*it).second->pfLogFile);
			(*it).second->pfLogFile = 0;
		}
	}
}

void CFileLogDev::Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText)
{
	if (E_LOG_LEVEL_MAX == nLevel)
		return;

	if (!m_nLogLevel.test(nLevel))
		return;

	if (0 == m_stLevelLogFile[nLevel])
		return;

	if(m_stLevelLogFile[nLevel]->pfLogFile == 0)
	{
		if (0 != Open(nLevel))
			return;
	}


	if (MAX_LOG_TEXT <= strlen(cszText) + 100)
		return;

	char szBuf[MAX_LOG_TEXT];
	size_t nSize;
	nSize = snprintf(szBuf, sizeof(szBuf), "%s %s %s:%d %s\n", CurrentTime().c_str(), LevelString(nLevel), cszFile, nLine, cszText);
	szBuf[MAX_LOG_TEXT - 1] = 0;

	// 记录到日志文件
	DevLock();
	if (m_oDate.IsToday())
	{
		fwrite(szBuf, nSize, 1, m_stLevelLogFile[nLevel]->pfLogFile);
		fflush(m_stLevelLogFile[nLevel]->pfLogFile);

		// 记录文件尺寸增加
		m_stLevelLogFile[nLevel]->nFileSize += nSize;

		// 检查文件尺寸
		if (m_stLevelLogFile[nLevel]->nFileSize > m_nMaxSize)
		{
			SwitchSize(m_stLevelLogFile[nLevel]);
		}
	}
	else
	{
		m_oDate.Day2Today();
		SwitchDate();

		if(m_stLevelLogFile[nLevel]->pfLogFile != 0)
		{
			fwrite(szBuf, nSize, 1, m_stLevelLogFile[nLevel]->pfLogFile);
			fflush(m_stLevelLogFile[nLevel]->pfLogFile);
		}
	}

	DevUnlock();
}

//日志文件大小超过设定值处理
int CFileLogDev::SwitchSize(LOG_FILE* pLogFile)
{
	if (0 == pLogFile)
		return -1;

	int nRtn = 0;
	string sFileName = m_sPrefix + pLogFile->sFileName + m_sPostfix;
	string sFullName = m_sFilePath + sFileName;
	string sTime = CGessTime::NowToString();
	string sNameNew = m_sPrefix + pLogFile->sFileName + "_" + sTime + m_sPostfix;
	string sFullNameNew = m_sFilePath + sNameNew;
	if (0 != pLogFile->pfLogFile)
	{
		fclose(pLogFile->pfLogFile);
		//pLogFile->pfLogFile = 0;
		if (0 == rename(sFullName.c_str(), sFullNameNew.c_str()))
			pLogFile->nFileSize = 0;
	}

	pLogFile->pfLogFile = fopen(sFullName.c_str(), "a");
	if(!pLogFile->pfLogFile)
	{
		cout << "SwitchSize日志文件:" << sFullName << " 打开失败, ERRNO:" <<  errno << endl;
		return -1;
	}

	struct stat st;
	if (stat(sFullName.c_str(), &st) == 0)
	{
		pLogFile->nFileSize = st.st_size;
	}
	else
	{
		cout << "日志文件" << sFullName << "获取尺寸失败" << endl;
	}
	
	return 0;
}

//日期切换处理
int CFileLogDev::SwitchDate()
{
	m_sFilePathRel = m_oDate.ToString("-");
	m_sFilePathRel += PATH_SLASH;
	m_sFilePath = m_sFilePathAbs + m_sFilePathRel;

	if (0 != MKDIR(m_sFilePath.c_str(),S_IRWXU | S_IRWXG | S_IRWXO))
	{
		cout << "目录已存在!" << m_sFilePath << endl;
	}

	map<string,LOG_FILE*>::iterator it;
	for (it = m_mapLogFile.begin(); it != m_mapLogFile.end(); ++it)
	{
		if (0 != (*it).second->pfLogFile)
		{
			fclose((*it).second->pfLogFile);
		}

		string sFileFull = m_sFilePath + m_sPrefix + (*it).second->sFileName + m_sPostfix;
		(*it).second->pfLogFile = fopen(sFileFull.c_str(), "a");
		if(!(*it).second->pfLogFile)
		{
			cout << "SwitchDate日志文件:" << sFileFull << " 打开失败, ERRNO:" <<  errno << endl;
			continue;
		}

		struct stat st;
		if (stat(sFileFull.c_str(), &st) == 0)
		{
			(*it).second->nFileSize = st.st_size;
		}
		else
		{
			cout << "日志文件" << sFileFull << "获取尺寸失败" << endl;
		}
	}
	return 0;
}

//
int CFileLogDev::Open(GESS_LOG_LEVEL eLevel)
{
	if (E_LOG_LEVEL_MAX == eLevel)
		return -1;

	if (0 == m_stLevelLogFile[eLevel])
		return -1;

	int nRtn = 0;
	string sFullName = m_sFilePath + m_sPrefix + m_stLevelLogFile[eLevel]->sFileName + m_sPostfix;
	DevLock();
	if (0 == m_stLevelLogFile[eLevel]->pfLogFile)
	{
		m_stLevelLogFile[eLevel]->pfLogFile = fopen(sFullName.c_str(), "a");
		if (0 == m_stLevelLogFile[eLevel]->pfLogFile)
		{
			nRtn = -1;
		}
		else
		{
			struct stat st;
			if (stat(sFullName.c_str(), &st) == 0)
			{
				m_stLevelLogFile[eLevel]->nFileSize = st.st_size;
			}
		}
	}
	DevUnlock();
	return nRtn;
}