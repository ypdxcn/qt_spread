#pragma once

#include "RealTimeReqBase.h"

class CRealTimeReqHandler : public CRealTimeReqBase
{
public:
	CRealTimeReqHandler(void);
public:
	~CRealTimeReqHandler(void);

protected:
	int SendReq( RTReqPara& req );
};

extern CRealTimeReqHandler g_RTReqHandler;