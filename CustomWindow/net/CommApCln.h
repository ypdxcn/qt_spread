#ifndef _COMM_AP_CLIENT_H
#define _COMM_AP_CLIENT_H
#include "CommAp.h"

#ifdef WWWIN32
#ifdef COMMAPCLIENT_EXPORT
#define COMMAPCLIENT_API			__declspec(dllexport)
#define COMMAPCLIENT_CLASS		__declspec(dllexport)
#else
#define COMMAPCLIENT_API			__declspec(dllimport)
#define COMMAPCLIENT_CLASS		__declspec(dllimport)
#endif
#else
#define COMMAPCLIENT_API
#define COMMAPCLIENT_CLASS
#endif

#endif