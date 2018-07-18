#ifndef _NM_CONFIG_H
#define _NM_CONFIG_H
#include "Config.h"

#ifdef _WIN32
#ifdef NM_EXPORT
#define NM_API			__declspec(dllexport)
#define NM_CLASS		__declspec(dllexport)
#else
#define NM_API			__declspec(dllimport)
#define NM_CLASS		__declspec(dllimport)
#endif
#else
#define NM_API
#define NM_CLASS
#endif

#endif
