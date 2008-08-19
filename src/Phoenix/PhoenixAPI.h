#ifndef __PhoenixAPI_h__
#define __PhoenixAPI_h__
/////////////////////////////////////////////////////////////////// 
// windows needs these for DLLs
// while compiling DLL library, define DLLEXPORT.
#if defined(WIN32)
# if defined(DLLEXPORT) 
#   define PHOENIX_API __declspec(dllexport)
# else 
#   define PHOENIX_API __declspec(dllimport)
# endif
#else
# define PHOENIX_API  /* Unix does not need this */
#endif
/////////////////////////////////////////////////////////////////

#endif
