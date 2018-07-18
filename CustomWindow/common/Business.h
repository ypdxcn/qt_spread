#pragma once

#include "Struct.h"

namespace Business
{
	BSFLAG GetBS( const string& sBS );

	KPFLAG GetKP( const string& offSetFlag );

	QString GetOrderInfo( const string &sProdCode, double dPrice, int iAmount, const string &sExchID );
};