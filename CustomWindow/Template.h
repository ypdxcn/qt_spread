
#include "TraderCpMgr.h"
#include "Global.h"
#include <QMessageBox>
#include "HJGlobalFun.h"

template<class REQ, class RSP>
bool TranTemplate( RSP& rsp, REQ& req, const QString &sExchCode )
{
	if(0 == g_TraderCpMgr.HandlePacket(rsp, req, sExchCode))
	{
		if (strcmp(rsp.rsp_code.c_str(), RSP_SUCCESS_CODE) == 0)
		{
			return true;
		}
		else
		{	//QMessageBox(rsp.rsp_msg);
			//kenny 20180208
			//QMessageBox::warning(NULL, "Title", CHJGlobalFun::str2qstr(rsp.rsp_msg), QMessageBox::Yes);
		}
	}
	else
		//QMessageBox(CONSTANT_CONNECT_SERVER_FAIL_TIPS);
	     QMessageBox::warning(NULL, "Title", CONSTANT_CONNECT_SERVER_FAIL_TIPS, QMessageBox::Yes);

	return false;
}