/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __MooseWindowsWrapper_h__
#define __MooseWindowsWrapper_h__
#ifdef WIN32
/////////////////////////////////////////////////////////////////
#include <time.h>
#include <windows.h>
#include <math.h>
#include <winsock.h>
#include "MooseAPI.h"
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
MOOSE_API int gettimeofday(struct timeval *tv, struct timezone *tz);
inline double round(double x) { return (x-floor(x))>0.5 ? ceil(x) : floor(x); }
inline float  roundf(float x) { return (x-floorf(x))>0.5 ? ceilf(x) : floorf(x); }
/////////////////////////////////////////////////////////////////
#endif
#endif
