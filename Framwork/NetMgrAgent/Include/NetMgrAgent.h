#ifndef _NM_AGENT_CONFIG_H
#define _NM_AGENT_CONFIG_H
#include "Config.h"

#ifdef _WIN32
#ifdef AGENT_EXPORT
#define AGENT_API			__declspec(dllexport)
#define AGENT_CLASS		__declspec(dllexport)
#else
#define AGENT_API			__declspec(dllimport)
#define AGENT_CLASS		__declspec(dllimport)
#endif
#else
#define AGENT_API
#define AGENT_CLASS
#endif

#endif
