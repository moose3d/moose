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

#ifndef __COggStreamSample_h__
#define __COggStreamSample_h__
#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)
/////////////////////////////////////////////////////////////////
#include "MooseALSampleTypes.h"
#include <vorbis/vorbisfile.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API COggStreamSample : public CALStreamSample
    {
    private:
      OggVorbis_File  m_OggFile;
      FILE *	  m_pFile;
      long		  m_lFileSize;
    public:
      COggStreamSample();
      ~COggStreamSample();
      bool Load( const char *szFilename );
      bool Stream( ALuint buffer );
      void Close();
      bool Rewind();
    };
    /////////////////////////////////////////////////////////////////
  }
}
#endif
#endif
