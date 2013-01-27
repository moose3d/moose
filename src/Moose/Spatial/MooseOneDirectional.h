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

#ifndef MOOSEONEDIRECTIONAL_H_
#define MOOSEONEDIRECTIONAL_H_
#include <MooseVector3.h>
#include <MooseAPI.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
    /////////////////////////////////////////////////////////////////
    /// The class for single-directional object. Direction is always normalized.
    class MOOSE_API COneDirectional
    {
    protected:
      /// Direction vector.
      Moose::Math::CVector3<float> m_vDirection;
    public:
      ////////////////////
      /// Constructor.
      COneDirectional() {}
      virtual ~COneDirectional(){}
      ////////////////////
      /// Constructor.
      COneDirectional( const Moose::Math::CVector3<float> & vDir ) { SetDirection(vDir); }
      ////////////////////
      /// Assigns direction. 
      /// \param vDirection direction vector to be assigned.Does not need to be normalized.
      inline void SetDirection( const Moose::Math::CVector3<float> & vDirection )
      {
        m_vDirection = vDirection;
        m_vDirection.Normalize();
      }
      ////////////////////
      /// Assigns direction. Does not need to be normalized.
      /// \param fX X-component of direction vector.
      /// \param fY Y-component of direction vector.
      /// \param fZ Z-component of direction vector.
      inline void SetDirection( float fX, float fY, float fZ )
      {
        m_vDirection[0] = fX;
        m_vDirection[1] = fY;
        m_vDirection[2] = fZ;
        m_vDirection.Normalize();
      }
      ////////////////////
      /// Returns normalized direction.
      /// \returns Vector with current direction.
      inline const Moose::Math::CVector3<float> & GetDirection() const
      {
        return m_vDirection;
      }
    };
    /////////////////////////////////////////////////////////////////
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEONEDIRECTIONAL_H_ */
