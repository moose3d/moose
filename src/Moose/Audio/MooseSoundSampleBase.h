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

#ifndef __CSoundSampleBase_h__
#define __CSoundSampleBase_h__
/////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <al.h>
#include <alc.h>
#elif __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <stdlib.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    /////////////////////////////////////////////////////////////////
    /// Class for generic sound sample. All other sample classes should be inherited from this.
    class MOOSE_API CSoundSampleBase
    {
    protected:
      ALenum     m_Format;
      ALsizei    m_Size;
      ALsizei    m_Freq;
      ALboolean  m_bLoop;
      size_t     m_nChannels;
      char *     m_pBufferData;
    protected:
      /////////////////////////////////////////////////////////////////
      CSoundSampleBase( const CSoundSampleBase & buf ) {}
      CSoundSampleBase( size_t nBufSize = 0 );
      /////////////////////////////////////////////////////////////////
      void		SetFormat( ALenum format );
      void		SetFreq( ALsizei f );
      void		SetNumChannels( size_t nCount );
      void		SetBufferSize( size_t nSize );
    public:  
      virtual	~CSoundSampleBase();
      /////////////////////////////////////////////////////////////////
      ALenum	GetFormat();
      ALsizei	GetFreq();
      size_t	GetNumChannels();
      virtual bool	Load( const char *szFilename ) = 0;
      size_t	GetBufferSize() const;
    };
    /////////////////////////////////////////////////////////////////
  }
}
#endif
