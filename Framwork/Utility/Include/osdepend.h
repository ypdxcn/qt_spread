#ifndef _OSDEPEND_H
#define _OSDEPEND_H

#ifdef _WIN32
// modified by Jerry Lee, 2011-3-30
// begin
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 /* Require Windows NT5 (2K, XP, 2K3) */
#endif

// end

#include <direct.h>
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#pragma warning( disable : 4267 4251 )


//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif


#ifndef S_IRWXU 
#define S_IRWXU 0
#endif
#ifndef S_IRWXG
#define S_IRWXG 0
#endif
#ifndef S_IRWXO
#define S_IRWXO 0
#endif
#define MKDIR(path,mode) mkdir(path)
#define strcasecmp(s1, s2)	stricmp(s1, s2)
#define strncasecmp(s1, s2, len) strnicmp(s1, s2, len)

//kenny  vs2015屏蔽下面
//#define snprintf(buf, count, fmt, ...)	_snprintf(buf, count, fmt, __VA_ARGS__)

#if defined(_MSC_VER) && _MSC_VER<1900
#  define snprintf(buf, count, fmt, ...)	_snprintf(buf, count, fmt, __VA_ARGS__)
#endif

#define PATH_SLASH	"\\"
#define _msleep(n)	Sleep(1000*n)
#define tsocket_t SOCKET
#define socklen_t int

#define F_SETFD 0
#define fcntl
#else
#define MKDIR(path,mode) mkdir(path,mode)
#define PATH_SLASH	"/"
//kenny  qt里面有mslepp ，LINUX系统是一秒
#define _msleep(n)	sleep(n)
#define msleep(n)	sleep(n)
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#define tsocket_t int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define closesocket(s) close(s)
#endif

//#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <fcntl.h>

////berkly socket
//int SetNonBlock(tsocket_t s)
//{
//	int nFlag = fcntl(s, F_GETFL, 0);
//	if (nFlag < 0) 
//	{ 
//		return -1;
//	} 
//
//	if (fcntl(s, F_SETFL, nFlag | O_NONBLOCK) < 0) 
//	{ 
//		return -1;
//	} 
//	return 0;
//}
//
////winsock
//int SetNonBlock(tsocket_t s)
//{
//	unsigned long ulMode = 0;
//	if (SOCKET_ERROR == ioctlsocket(s, FIONBIO, &iMode)
//	{
//		return -1;
//	}
//
//	return 0;
//}

#if defined (_MSC_VER)
#pragma warning(disable : 4996) // 关闭废弃函数的警告
#endif

#endif