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

#ifndef __COpenALObject_h__
#define __COpenALObject_h__
/////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <al.h>
#elif __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
#include <ostream>
#include <iostream>
#include <assert.h>
#include <string>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
#define ReportALErrors()					\
{								\
  ALenum error = alGetError();					\
  if ( error != AL_NO_ERROR)					\
  {								\
    std::cerr << GetOpenALErrorString(error) << std::endl;	\
  }								\
}
/////////////////////////////////////////////////////////////////
#define ReportAndHaltOnALErrors()				\
{								\
  ALenum error =  alGetError();					\
  if ( error != AL_NO_ERROR)					\
  {								\
    std::cerr << GetOpenALErrorString(error) << std::endl;	\
    assert( false );						\
  }								\
}
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    template< size_t OBJECTS >
    class MOOSE_API CALObjectArray
    {
    private:
      ALuint m_Objects[OBJECTS];
    public:

      CALObjectArray();
      virtual ~CALObjectArray() {}
      ALuint & GetALObject( size_t nObject = 0) { return m_Objects[nObject]; }
      std::string GetOpenALErrorString( ALenum error );
    };
  }
}
/////////////////////////////////////////////////////////////////
template< size_t OBJECTS >
Moose::Sound::CALObjectArray<OBJECTS>::CALObjectArray()
{
  for( size_t i=0;i<OBJECTS; i++)
  {
    m_Objects[i] = 0;
  }
}
/////////////////////////////////////////////////////////////////
template< size_t OBJECTS >
std::string
Moose::Sound::CALObjectArray<OBJECTS>::GetOpenALErrorString( ALenum error )
{
  switch( error )
  {
  case AL_NO_ERROR:
    return std::string("AL_NO_ERROR");
    break;

  case AL_INVALID_NAME:
    return std::string("AL_INVALID_NAME");
    break;

  case AL_INVALID_ENUM:
    return std::string("AL_INVALID_ENUM");
    break;

  case AL_INVALID_VALUE:
    return std::string("AL_INVALID_VALUE");
    break;

  case AL_INVALID_OPERATION:
    return std::string("AL_INVALID_OPERATION");
    break;
  case AL_OUT_OF_MEMORY:
    return std::string("AL_OUT_OF_MEMORY");
    break;
  default:
    return std::string("Unknown enum???");
    break;
  };
}
///////////////////////////////////////////////////////////////////////////
#endif
