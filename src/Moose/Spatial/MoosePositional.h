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

#ifndef __MOOSEPOSITIONAL_H__
#define __MOOSEPOSITIONAL_H__
/////////////////////////////////////////////////////////////

#include <MooseVector3.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
  /////////////////////////////////////////////////////////////////
      /// The base class for Objects with position.
      class MOOSE_API CPositional
      {
      protected:
        /// Vector where position is stored.
        Moose::Math::CVector3<float> m_vPosition;
        /// has the position changed.
        int	      m_bPositionChanged;
      public:
        ////////////////////
        /// Default constructor.
        CPositional() : m_vPosition(0.0f,0.0f,0.0f),m_bPositionChanged(0)
        {

        }
        ////////////////////
        /// Parametrized constructor.
        /// \param vPosition vector for location-
        CPositional( const Moose::Math::CVector3<float> & vPosition ) : m_vPosition(vPosition),m_bPositionChanged(0)
        {
        }
        ////////////////////
        /// Destructor.
        virtual ~CPositional() {}
        ////////////////////
        /// Assigns position.
        /// \param vPosition vector of which values are used.
        inline void SetPosition( const Moose::Math::CVector3<float> &vPosition )
        {
  	m_vPosition = vPosition;
  	SetPositionChanged(1);
        }
        ////////////////////
        /// Assigns the position.
        /// \param fX position on X-axis.
        /// \param fY position on Y-axis.
        /// \param fZ position on Z-axis.
        inline void SetPosition( float fX, float fY, float fZ )
        {
  	m_vPosition[0] = fX;
  	m_vPosition[1] = fY;
  	m_vPosition[2] = fZ;
  	SetPositionChanged(1);
        }
        ////////////////////
        /// Returns position.
        /// \param current position.
        inline const Moose::Math::CVector3<float> & GetPosition() const
        {
  	return m_vPosition;
        }

        ////////////////////
        /// Adds vector to current position.
        /// \param vPosition the movement applied to current position.
        inline void Move( const Moose::Math::CVector3<float> & vPosition )
        {
  	m_vPosition = m_vPosition + vPosition;
  	SetPositionChanged(1);
        }
        ////////////////////
        /// Returns true if position has changed.
        /// \returns boolean
        int IsPositionChanged() const
        {
  	return m_bPositionChanged;
        }
        ////////////////////
        /// Sets the position change flag.
        /// \param bFlag true for enable, false for disable.
        inline void SetPositionChanged(int bFlag )
        {
  	m_bPositionChanged = bFlag;
        }
      };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEPOSITIONAL_H_ */
