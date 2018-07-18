#pragma once

struct TipsOrderInfo;

class CGlobalMgr
{
public:
	CGlobalMgr(void);
public:
	~CGlobalMgr(void);

	// 添加一个提示
	bool AddTips( const TipsOrderInfo &stTips ); 
};

extern CGlobalMgr g_GlobalMgr;
