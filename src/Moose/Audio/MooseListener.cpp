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

#include "MooseListener.h"
#include <cstring>
#ifdef WIN32
#include <al.h>
#elif __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
namespace libname = Moose::Sound;
using namespace Moose::Math;
/////////////////////////////////////////////////////////////////
libname::CListener::CListener()
{
  SetPosition( 0,0,0);
  SetVelocity( CVector3<float>(0,0,0));
  SetDirectionForwardUp( CVector3<float>(0,0,-1), CVector3<float>(0,1,0));
}
/////////////////////////////////////////////////////////////////
libname::CListener::~CListener()
{
  // nop
}
/////////////////////////////////////////////////////////////////
void
libname::CListener::SetVelocity(const Moose::Math::CVector3<float> & vDir )
{
  m_vVelocity = vDir;
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CVector3<float> &
libname::CListener::GetVelocity() const
{
  return m_vVelocity;
}
/////////////////////////////////////////////////////////////////
void
libname::CListener::CommitChanges()
{
  ALfloat buf[6];
  memcpy( buf, GetForwardVector().GetArray(), sizeof(float)*3);
  memcpy( &buf[3], GetUpVector().GetArray(), sizeof(float)*3);

  // set properties
  alListenerfv( AL_POSITION, GetPosition().GetArray());
  alListenerfv( AL_VELOCITY, GetVelocity().GetArray());
  alListenerfv( AL_ORIENTATION, buf );
}
/////////////////////////////////////////////////////////////////
