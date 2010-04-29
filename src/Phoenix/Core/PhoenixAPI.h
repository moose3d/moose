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
#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif
#if (defined(__APPLE__) && (defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_IPHONE_OS))) 
#define PHOENIX_APPLE_IPHONE
#endif
#endif

