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

#ifndef __MooseGlobals_h__
#define __MooseGlobals_h__
#include <cstdlib>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Globals
  {
    /// Maximum number of characters in a font.
    const unsigned int MAX_FONT_CHARACTERS = 256;
    const unsigned int NODE_NAME_MAX_SIZE =  128;
    /// magic number for hash table size and other stuff, too.
    const size_t MOOSE_MAGIC_NUMBER = 33;
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
     
  }
}
#define MOOSE_DEPRECATED __attribute__((deprecated))
/////////////////////////////////////////////////////////////////
#define IMG_OK              0x1
#define IMG_ERR_NO_FILE     0x2
#define IMG_ERR_MEM_FAIL    0x4
#define IMG_ERR_BAD_FORMAT  0x8
#define IMG_ERR_UNSUPPORTED 0x40
/////////////////////////////////////////////////////////////////
#define TEXTURE_HANDLE_COUNT 8
#endif
