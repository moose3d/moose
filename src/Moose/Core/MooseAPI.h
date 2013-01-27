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

// SDL helper
#if (SDL_MAJOR_VERSION==1 && SDL_MINOR_VERSION==3) || (SDL_MAJOR_VERSION == 2)
#define MOOSE_USE_OPENGL3 
#endif

#endif

