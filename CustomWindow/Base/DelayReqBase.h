#pragma once

//#include "..\..\Framework\Utility\Include\Gess.h"

#include "ThreadInterface.h"
#include "Def.h"
#include <vector>

struct ReqPara
{
	int          nFailTry; // 失败后尝试的次数（未启用）
	int     nFailWaitTime; // 失败后下一次等待的间隔时间（未启用）
	int      nCurReqCount; // 当前尝试的次数（未启用）
	DWORD    dLastReqTick; // 上一次请求的时间（未启用）
	DWORD        dIniWait; // 第一次运行时等待的时间（未启用）

	EReqType eReqType;     // 请求类型
	vector<string>  vPara; // 函数参数

	ReqPara()
	{
		nFailTry = 2;
		nFailWaitTime = 0;
		nCurReqCount = 0;

#ifdef  _WIN32
                dLastReqTick = ::GetTickCount64();
#else
                //macx???
                dLastReqTick = ::mach_absolute_time();
#endif
		dIniWait = 0;
	}
};

class CReqHandlerBase : public CThreadInferface
{
public:
	CReqHandlerBase(void);
public:
	~CReqHandlerBase(void);

	// 添加请求
	void AddReq( const ReqPara &stReq );

	// 设置延期时间
	void SetDelayTime( int nTime );

protected:

	/*
	判断添加的一笔请求是否无效（重复或者不需要），父类实现
	*/
	virtual bool IsUseless( ReqPara &stReqInQue, const ReqPara &stReqAdd );

	/*
	发送所有的请求，父类实现
	*/
	virtual void SendReq( const deque<ReqPara> &deqReq );

protected:
	int ThreadEntry();
	int End();
	void OnStart() { };
	void OnStop() { };

protected:
	std::deque<ReqPara> m_deqReq;
	CCondMutex	  m_deqCondMutex;
	int m_nDelayTime; // 延期的时间，毫秒为单位
};
