/*
请求处理线程基类
*/
#pragma once

//#include "..\..\Framework\Utility\Include\Gess.h"

#include "ThreadInterface.h"
#include "Def.h"
#include <vector>

#ifdef _WIN32
#else
#include <mach/mach_time.h>
#endif

struct RTReqPara
{
	int          nFailTry; // 失败后尝试的次数（未启用）
	int     nFailWaitTime; // 失败后下一次等待的间隔时间（未启用）
	int      nCurReqCount; // 当前尝试的次数（未启用）
	DWORD    dLastReqTick; // 上一次请求的时间（未启用）
	DWORD        dIniWait; // 第一次运行时等待的时间（未启用）

	EReqType eReqType;     // 请求类型
	vector<string>  vPara; // 函数参数

	RTReqPara()
	{
		nFailTry = 2;
		nFailWaitTime = 0;
		nCurReqCount = 0;
#ifdef _WIN32
                dLastReqTick = ::GetTickCount64();
#else
                dLastReqTick = ::mach_absolute_time();
#endif

		dIniWait = 0;
	}
};

class CRealTimeReqBase : public CThreadInferface
{
public:
	CRealTimeReqBase(void);
public:
	~CRealTimeReqBase(void);

	// 添加请求
	void AddReq( const RTReqPara &stReq );

protected:
	/*
	发送所有的请求，父类实现
	*/
	virtual int SendReq( RTReqPara& req ) = 0;

protected:
	int ThreadEntry();
	int End();
	void OnStart() { };
	void OnStop() { };

protected:
	std::deque<RTReqPara> m_deqReq;
	CCondMutex	  m_deqCondMutex;
};
