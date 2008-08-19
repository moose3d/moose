#ifndef __PhoenixWindowsWrapper_h__
#define __PhoenixWindowsWrapper_h__
/////////////////////////////////////////////////////////////////
#include <time.h>
#include <windows.h>
#include <math.h>
#include <winsock.h>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
/////////////////////////////////////////////////////////////////
struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};
/////////////////////////////////////////////////////////////////
PHOENIX_API int gettimeofday(struct timeval *tv, struct timezone *tz);
inline double round(double x) { return (x-floor(x))>0.5 ? ceil(x) : floor(x); }
inline float  roundf(float x) { return (x-floorf(x))>0.5 ? ceilf(x) : floorf(x); }
/////////////////////////////////////////////////////////////////
#endif
