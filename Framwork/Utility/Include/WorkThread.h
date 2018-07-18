#ifndef _WORKTHREAD_BASIC_H
#define _WORKTHREAD_BASIC_H

#include "ThreadInterface.h"

class UTIL_CLASS  CWorkThread: public CThreadInferface
{
public:
	CWorkThread (){}
	virtual ~CWorkThread(){}

protected:
	virtual void OnStart() {}
	virtual void OnStop() {}

}; 
#endif