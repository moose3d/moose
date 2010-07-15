#ifndef __PhoenixAPI_h__
#define __PhoenixAPI_h__
/////////////////////////////////////////////////////////////////// 
// windows needs these for DLLs
// while compiling DLL library, define DLLEXPORT.
#if defined(WIN32) && defined(DLLBUILD)
#   if defined(DLLEXPORT) 
#     define PHOENIX_API __declspec(dllexport)
#   else 
#     define PHOENIX_API __declspec(dllimport)
#   endif
#else
# define PHOENIX_API  /* Unix / static build does not need this */
#endif
/////////////////////////////////////////////////////////////////
#ifdef PHOENIX_APPLE_IPHONE
#undef PHOENIX_APPLE_IPHONE
#endif
#ifdef PHOENIX_APPLE_IPHONE_SIMULATOR
#undef PHOENIX_APPLE_IPHONE_SIMULATOR
#endif

#if defined(__APPLE__)

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE 
#define PHOENIX_APPLE_IPHONE
#endif

#if TARGET_IPHONE_SIMULATOR
#define PHOENIX_APPLE_IPHONE_SIMULATOR
#endif

#endif

#endif

