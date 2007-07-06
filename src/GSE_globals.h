/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#ifndef __GSE_globals_h__
#define __GSE_globals_h__
/////////////////////////////////////////////////////////////////
// The maximum number of characters for the GSE_object name
#define GSE_OBJECT_MAX_NAME_SIZE 128 
// The number of things in objstruct that can be cached.
#define GSE_MAX_CACHE_TARGETS 11  
// The maximum number of texture coordinate pairs per vertex.
#define GSE_MAX_TEXCOORD_ARRAYS 8
// The size of the selection buffer for OglRenderer
#define GSE_MAX_SELECTION_OBJECTS 512
// The maximum number of printable characters in fontset
#define GSE_MAX_FONT_CHARACTERS 256
/// The maximum number of active lights at a time.
#define GSE_MAX_ACTIVE_LIGHTS 8
/// The max number of supported textures
#define MAX_TEXTURES 8
/////////////////////////////////////////////////////////////////
namespace TextureFilter 
{
  /// Texture filter modes.
  enum Type 
  {
    ENV_MODULATE = 0,
    ENV_DECAL,
    ENV_BLEND,
    ENV_REPLACE,
    ENV_COLOR,
    MIN_NEAREST,
    MIN_LINEAR,
    MIN_MIP_NEAREST,
    MIN_MIP_LINEAR,
    MAG_LINEAR,
    MAG_NEAREST,
    T_WRAP_REPEAT,
    S_WRAP_REPEAT,
    T_WRAP_CLAMP,
    S_WRAP_CLAMP,
    T_WRAP_CLAMP_TO_EDGE,
    S_WRAP_CLAMP_TO_EDGE
  };
}
/////////////////////////////////////////////////////////////////
namespace TextureType
{
  /// The texture types.
  enum Type 
  {
    /// One-dimensional texture
    GSE_TEX1D = 0,
    /// Two-dimensional texture
    GSE_TEX2D = 1,
    /// Two-dimensional rectangular texture
    GSE_TEXR2D
  };
}
/////////////////////////////////////////////////////////////////
namespace TextureId
{
  /// Texture ids.
  enum Type 
  {
    TEX0 = 0,
    TEX1 = 1,
    TEX2,
    TEX3,
    TEX4,
    TEX5,
    TEX6,
    TEX7
  };
}
/////////////////////////////////////////////////////////////////
namespace TextureQuality
{
  /// Texture quality flags.
  enum Type 
  {
    PERFORMANCE = 0,
    QUALITY = 1
  };
}
/////////////////////////////////////////////////////////////////
namespace AlphaTest
{
  /// Alpha test types.
  enum Type
  {
    NEVER = 0,
    ALWAYS,
    LESS,
    LEQUAL,
    EQUAL,
    GEQUAL,
    GREATER,
    NOTEQUAL
  };
}
/////////////////////////////////////////////////////////////////
// handy macro for determining and storing new min/max.
#define STORE_MAX_MIN(NEWVAL, OLDMAX, OLDMIN) \
{\
	if ( NEWVAL > OLDMAX) \
        {\
		OLDMAX = NEWVAL;\
	}\
        else if ( NEWVAL < OLDMIN )\
        {\
		OLDMIN = NEWVAL;\
	}\
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// history: 
//
//		$Log: GSE_globals.h,v $
//		Revision 1.11  2007/05/31 13:53:17  entity
//		more doxy comments
//
//		Revision 1.10  2007/04/09 09:41:17  entity
//		alphatest added
//
//		Revision 1.9  2007/04/03 12:52:00  entity
//		moved STORE_MAX_MIN macro here
//
//		Revision 1.8  2007/04/02 12:55:50  entity
//		added CLAMP_TO_EDGE filters
//
//		Revision 1.7  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.6  2007/04/01 08:20:07  entity
//		moved texture count here
//
//		Revision 1.5  2007/04/01 08:16:06  entity
//		code cleanups
//
//		Revision 1.4  2007/03/08 13:10:14  entity
//		new lighting scheme
//
//		Revision 1.3  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.2  2006/10/13 07:13:05  entity
//		Changed GSE_Object name size to 128
//
//
//		07082006 AG/eNtity : initial version
/////////////////////////////////////////////////////////////////
