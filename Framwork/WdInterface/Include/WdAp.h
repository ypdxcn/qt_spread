#ifndef _WATCHDOG_AP_INTERFACE_H
#define _WATCHDOG_AP_INTERFACE_H


#ifdef _WIN32
#ifdef WDAP_EXPORT
#define WDAP_API			__declspec(dllexport)
#define WDAP_CLASS		__declspec(dllexport)
#else
#define WDAP_API			__declspec(dllimport)
#define WDAP_CLASS		__declspec(dllimport)
#endif
#else
#define WDAP_API
#define WDAP_CLASS
#endif

#endif