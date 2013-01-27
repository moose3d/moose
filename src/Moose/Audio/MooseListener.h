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

#ifndef __CListener_h__
#define __CListener_h__
/////////////////////////////////////////////////////////////////
#include "MooseVector3.h"
#include "MooseOrientable.h"
#include "MoosePositional.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
#ifndef SWIG
    class CAudioSystem;
#endif
    class MOOSE_API CListener : public Moose::Spatial::COrientable,
		      public Moose::Spatial::CPositional
    {
#ifndef SWIG
        friend class CAudioSystem;
#endif
    protected:
      Moose::Math::CVector3<float> m_vVelocity;

    public:
      CListener();
      virtual ~CListener();
      void SetVelocity(  const Moose::Math::CVector3<float> & vDir );
      const Moose::Math::CVector3<float> & GetVelocity() const;
      void CommitChanges();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
