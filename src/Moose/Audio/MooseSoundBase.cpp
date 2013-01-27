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

#include "MooseSoundBase.h"
#include "MooseVector3.h"
namespace libname = Moose::Sound;
/////////////////////////////////////////////////////////////////
libname::CSoundBase::CSoundBase()
{
  SetPosition(0,0,0);
  SetDirection(0,0,-1);
  SetVelocity(Moose::Math::CVector3<float>(0,0,0));
}
/////////////////////////////////////////////////////////////////
void
libname::CSoundBase::SetVelocity(const Moose::Math::CVector3<float> & vDir )
{
  m_vVelocity = vDir;
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CVector3<float> & 
libname::CSoundBase::GetVelocity() const
{
  return m_vVelocity;
}
/////////////////////////////////////////////////////////////////
