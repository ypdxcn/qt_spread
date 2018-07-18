#ifndef _XQUEUE_DLL_H
#define _XQUEUE_DLL_H

#include <tchar.h>
#define TRACE	__noop

#ifdef _WIN32
#ifdef XQUEUE_EXPORT
#define XQUEUE_API			__declspec(dllexport)
#define XQUEUE_CLASS		__declspec(dllexport)
#else
#define XQUEUE_API			__declspec(dllimport)
#define XQUEUE_CLASS		__declspec(dllimport)
#endif
#else
#define XQUEUE_API
#define XQUEUE_CLASS
#endif

#endif