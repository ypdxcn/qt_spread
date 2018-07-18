#ifndef _BLOCK_MSG_H
#define _BLOCK_MSG_H

#include "Gess.h"

#ifdef _WIN32
#ifdef BLOCKMSG_EXPORT
#define BLOCKMSG_API			__declspec(dllexport)
#define BLOCKMSG_CLASS		__declspec(dllexport)
#else
#define BLOCKMSG_API			__declspec(dllimport)
#define BLOCKMSG_CLASS		__declspec(dllimport)
#endif
#else
#ifdef BLOCKMSG_EXPORT
#define BLOCKMSG_API         __attribute__   ((visibility ("default")))
#define BLOCKMSG_CLASS       __attribute__   ((visibility ("default")))
#else
#define BLOCKMSG_API
#define BLOCKMSG_CLASS
#endif
#endif



#endif