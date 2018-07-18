#pragma once

//#define WIN32

#ifdef WIN32
#ifdef CTPSAMPLE_LIB
#define CTPSAMPLE_EXPORT __declspec(dllexport)
#else
#define CTPSAMPLE_EXPORT __declspec(dllimport)

#endif

//#define CALLBACK __stdcall

#else

#define CTPSAMPLE_EXPORT
//#define CALLBACK
#endif

//// 普通类导出
//class XJSON_API JObject……
//
////  模板类导出
//
//template <class T>
//class XJSON_TEMPLATE JObjArray : public JObject…
//
//// 函数导出
//XJSON_API int GetVersion()……



