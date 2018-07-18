#pragma once

#include "TradePacketAndStruct.h"
#include <QMessageBox>
class CTranMsg
{
public:
	CTranMsg(void);
public:
	~CTranMsg(void);

	static bool Handle1001( Rsp1001 &rsp );

	static bool Handle1002( Rsp1002 &rsp );

	static bool Handle1004( Rsp1004 &rsp );

	static bool Handle9010( Rsp9010 &rsp, Req9010 &req );

	static bool Handle6002( Rsp6002 &rsp, Req6002 &req, QVector< pair<QString,QString> > &vecPara );

	static bool Handle9030( Rsp9030 &rsp, const QString &sParaID );

	static bool Handle5101( Rsp5101 &rsp, Req5101 &req );

	static bool Handle4074( Rsp4074 &rsp, Req4074 &req );

	static bool Handle3201( Rsp3201 &rsp );
};


