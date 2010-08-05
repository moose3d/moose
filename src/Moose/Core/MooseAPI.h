#ifndef __MooseAPI_h__
#define __MooseAPI_h__
/////////////////////////////////////////////////////////////////// 
// windows needs these for DLLs
// while compiling DLL library, define DLLEXPORT.
#if defined(WIN32) && defined(DLLBUILD)
#   if defined(DLLEXPORT) 
#     define MOOSE_API __declspec(dllexport)
#   else 
#     define MOOSE_API __declspec(dllimport)
#   endif
#else
# define MOOSE_API  /* Unix / static build does not need this */
#endif
/////////////////////////////////////////////////////////////////
#ifdef MOOSE_APPLE_IPHONE
#undef MOOSE_APPLE_IPHONE
#endif
#ifdef MOOSE_APPLE_IPHONE_SIMULATOR
#undef MOOSE_APPLE_IPHONE_SIMULATOR
#endif

#if defined(__APPLE__)

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE 
#define MOOSE_APPLE_IPHONE
#endif

#if TARGET_IPHONE_SIMULATOR
#define MOOSE_APPLE_IPHONE_SIMULATOR
#endif

#endif
// For ffmpeg types 
#define __STDC_CONSTANT_MACROS
#endif

