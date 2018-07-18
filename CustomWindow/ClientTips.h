#ifndef _CLIENTTIPS_H
#define _CLIENTTIPS_H

//#pragma once

#include "Struct.h"
#include <vector>
#include  <QMutex>
//#include <QMutexGuard>
class CClientTips
{
public:
	CClientTips(void);
public:
	~CClientTips(void);
};

class CTipsManager
{
public:
	CTipsManager();

	// 添加一笔提示，并且返回添加的位置
	int Add( const TipsOrderInfo &TipsInfo );

	/*
	存在则返回0，不存在但是vector不为空，则返回1，数组为空则返回-1
	*/
	int GetTips( TipsOrderInfo &TipsInfo, int nPos );

	void Clear();

	int GetSize();

	// 记录用户所有的提示信息，用于以后可能给用户查看历史情况的时候
	vector<TipsOrderInfo> m_vecTips;
private:


	// 锁
	QMutex	 m_csMutex;

	// 记录当前显示的提示的个数
	int m_iShowCount;
};

extern CTipsManager g_TipsManager;

#endif
