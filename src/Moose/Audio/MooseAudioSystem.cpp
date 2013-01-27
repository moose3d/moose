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

#include "MooseAudioSystem.h"
#include "MooseListener.h"
#include <iostream>
using namespace std;
namespace libname = Moose::Sound;
/////////////////////////////////////////////////////////////////
#define CHECK_ALC( A ) {			        \
  const ALCchar * tmp = alcGetString( NULL, A );\
  cerr << #A << " : " << (tmp ? tmp : "NULL");	\
  if ( alcGetError(NULL) == ALC_INVALID_ENUM )		\
  {							\
    cerr << "No such enum: '" << #A << "'";		\
  }							\
  cerr << endl;						\
}
/////////////////////////////////////////////////////////////////
#define CHECK_AL( A ) {			\
  const ALchar * tmp = alGetString( A );\
  cerr << #A << " : " << (tmp ? tmp : "NULL");	\
  if ( alGetError() == AL_INVALID_ENUM )		\
  {							\
    cerr << "No such enum: '" << #A << "'";		\
  }							\
  cerr << endl;						\
}
/////////////////////////////////////////////////////////////////
bool
libname::CAudioSystem::IsOpen() const
{
  return (alcGetCurrentContext() != NULL);
}
/////////////////////////////////////////////////////////////////
bool
libname::CAudioSystem::OpenDevice( const char *szDev )
{
  m_pDevice = alcOpenDevice( szDev );
  if (m_pDevice == NULL)
  {
    if ( szDev != NULL )
    {
      cerr << "Failure, device \'" << szDev << "\' cannot be opened" << endl; 
    }
    else 
    {
      cerr << "Failure, default device cannot be opened." << endl; 
    }
    return false;
  }

  //Create context(s)
  m_pContext=alcCreateContext(m_pDevice, NULL);

  //Set active context
  alcMakeContextCurrent(m_pContext);
  
  //alutInit(0, NULL);
  // Clear Error Code
  alGetError();
  cerr << "==== OpenAL System  ==== " << endl;
  CHECK_ALC( ALC_DEVICE_SPECIFIER );
  CHECK_ALC( ALC_DEFAULT_DEVICE_SPECIFIER);
  CHECK_ALC( ALC_EXTENSIONS);

  CHECK_AL( AL_VENDOR);
  CHECK_AL( AL_VERSION);
  CHECK_AL( AL_RENDERER);
  CHECK_AL( AL_EXTENSIONS);

  return true;
}
/////////////////////////////////////////////////////////////////
libname::CAudioSystem::CAudioSystem()
{
  
}
/////////////////////////////////////////////////////////////////
libname::CAudioSystem::~CAudioSystem()
{
  if ( m_pContext )
    alcDestroyContext( m_pContext );
  m_pContext = NULL;
  if ( m_pDevice )
    alcCloseDevice( m_pDevice );
  m_pDevice = NULL;
}

/////////////////////////////////////////////////////////////////
libname::CListener &
libname::CAudioSystem::GetListener()
{
  return m_Listener;
}
/////////////////////////////////////////////////////////////////
