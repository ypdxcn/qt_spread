#ifndef _FILE_LOG_DEV_H
#define _FILE_LOG_DEV_H

#include "GessDate.h"
#include "GessTime.h"
#include "LogDevice.h"
#include "Gess.h"

class UTIL_CLASS CFileLogDev : public CLogDevice
{
public:
	CFileLogDev(CLogger * pLogger);
	~CFileLogDev(void);

public:
	int Initial(CConfig * pCfg);
	int Open(void);
	void Close(void);
	void Finish();

	void Log(const char * cszFile, long nLine, GESS_LOG_LEVEL nLevel, const char * cszText);
private:
	typedef struct tagLogFile
	{
		std::string sFileName;		//日志文件名
		FILE * pfLogFile;			//日志文件句柄
		size_t nFileSize;			//日志文件大小
		int nFlag;					//0用户定义 1缺省
	} LOG_FILE, *PLOG_FILE;
private:
	int Open(GESS_LOG_LEVEL eLevel);
	int SwitchSize(LOG_FILE* pLogFile);
	int SwitchDate();

	std::string m_sPostfix;			//日志文件后缀
	std::string m_sPrefix;			//日志文件前缀
	std::string m_sFilePathAbs;		//日志文件目录路径固定部分
	std::string m_sFilePathRel;		//日志文件目录路径变化部分
	std::string m_sFilePath;		//日志文件目录路径
	size_t m_nMaxSize;				//日志文件最大尺寸

	LOG_FILE*	m_stLevelLogFile[E_LOG_LEVEL_MAX];
	map<string,LOG_FILE *>	m_mapLogFile;
	CGessDate	m_oDate;
};
#endif