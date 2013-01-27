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

#ifndef __CAudioSystem_h__
#define __CAudioSystem_h__
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
#include "MooseSingleton.h"
#include "MooseListener.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API CAudioSystem : public Moose::Core::CSingleton<Moose::Sound::CAudioSystem>
    {
      friend class Moose::Core::CSingleton<Moose::Sound::CAudioSystem>;
    protected:
      ALCcontext *m_pContext;
      ALCdevice *m_pDevice;
      CListener  m_Listener;
      CAudioSystem();
      virtual ~CAudioSystem();
    public:
      /// Attempts to open device.
      /// \param szDevice Device name or path.
      /// \returns true, if successful.
      /// \returns false on failure.
      bool OpenDevice ( const char *szDevice = NULL );
      ////////////////////
      /// Checks is (some) device opened.
      bool IsOpen() const;
      CListener &	     GetListener();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
