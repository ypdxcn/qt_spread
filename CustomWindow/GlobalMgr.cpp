
#include "GlobalMgr.h"

#include "ClientTips.h"
#include "TraderCpMgr.h"
#include "HJConstant.h"
#include "QParamEvent.h"

CGlobalMgr g_GlobalMgr;

CGlobalMgr::CGlobalMgr(void)
{
}

CGlobalMgr::~CGlobalMgr(void)
{
}

bool CGlobalMgr::AddTips( const TipsOrderInfo &stTips )
{
	int nAddPos = g_TipsManager.Add(stTips);

	QParamEvent * msg = new QParamEvent(WM_SHOW_ORDER_TIPS);
	msg->setWParam((void *)&nAddPos);
	QApplication::postEvent(g_TraderCpMgr.m_hMain, msg);


	return true;
}
