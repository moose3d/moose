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

#ifndef MOOSEQUAD_H_
#define MOOSEQUAD_H_
/////////////////////////////////////////////////////////////
#include <MoosePositional.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
  /////////////////////////////////////////////////////////////////
      // The class for a Quad
      class MOOSE_API CQuad : public Moose::Spatial::CPositional
      {
      protected:
        // The width of the quad
        float m_fWidth;
        // The height of the quad
        float m_fHeight;
        float m_fHalfWidth;
        float m_fHalfHeight;

      public:
        ////////////////////
        /// The constructor
        CQuad() : Moose::Spatial::CPositional(), m_fWidth(0.0f), m_fHeight(0.0f) {}
        ////////////////////
        /// Sets width
        /// \param fWidth Width to be assigned.
        void SetWidth( float fWidth )
        {
  	m_fWidth = fWidth;
  	m_fHalfWidth = m_fWidth * 0.5f;
        }
        ////////////////////
        /// Sets height
        /// \param fHeight Height to be assigned.
        void SetHeight( float fHeight )
        {
  	m_fHeight = fHeight;
  	m_fHalfHeight = m_fHeight * 0.5f;
        }
        float GetWidth() const
        {
  	return m_fWidth;
        }
        float GetHeight() const
        {
  	return m_fHeight;
        }
        float GetHalfWidth() const
        {
  	return m_fHalfWidth;
        }
        float GetHalfHeight() const
        {
  	return m_fHalfHeight;
        }
        void CreateFrom2DCoords(int iX1, int iY1, int iX2, int iY2 );
        friend std::ostream& Moose::Math::operator<<( std::ostream &stream, const Moose::Math::CQuad & quad );
      }; // class CQuad
      std::ostream& operator<<( std::ostream &stream, const Moose::Math::CQuad & quad );
  } // Math
} // Moose
/////////////////////////////////////////////////////////////
#endif /* MOOSEQUAD_H_ */
