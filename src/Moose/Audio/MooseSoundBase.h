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

#ifndef __CSoundBase_h__
#define __CSoundBase_h__
/////////////////////////////////////////////////////////////////
#include "MoosePositional.h"
#include "MooseOneDirectional.h"
#include "MooseVector3.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API CSoundBase : public Moose::Spatial::CPositional,
				   public Moose::Spatial::COneDirectional
    {
    protected:
      Moose::Math::CVector3<float> m_vVelocity;
    protected:
      CSoundBase();
      virtual ~CSoundBase() {}
    public:
      virtual void Play() = 0;
      virtual bool IsPlaying() const = 0;
      virtual bool IsLooping() = 0;
      virtual void SetLooping( bool flag) = 0;
      void SetVelocity(  const Moose::Math::CVector3<float> & vDir );
      const Moose::Math::CVector3<float> & GetVelocity() const;
  
    };
    /////////////////////////////////////////////////////////////////
  }
}
#endif
