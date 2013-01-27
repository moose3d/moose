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

#ifndef MOOSETRIANGLE_H_
#define MOOSETRIANGLE_H_
#include <MooseAPI.h>
#include <MooseVector3.h>
#include <MooseVertex.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
	  /////////////////////////////////////////////////////////////////
      /// Triangle class for editors.
      class MOOSE_API CTriangle
      {
      protected:
        /// Vertices in a triangle
        CVertex			m_vVertices[3];
        /// Is this triangle selected.
        int			m_bSelected;
      public:
        ////////////////////
        /// Constructor.
        CTriangle() : m_bSelected(0)
        {

        }
        ////////////////////
        /// Is this triangle selected.
        /// \returns Selection status.
        int IsSelected() const
        {
  	return m_bSelected;
        }
        ////////////////////
        /// Sets selection status.
        /// \param bFlag Boolean selection status, true for selection, false for unselection.
        void SetSelected( int bFlag )
        {
  	m_bSelected = bFlag;
        }
        ////////////////////
        /// Returns vertex.
        /// \param nCorner Which corner vertex of triangle (0,1 or 2)
        /// \returns Reference to vertex.
        CVertex & GetVertex( unsigned int nCorner )
        {
  	return m_vVertices[ nCorner % 3];
        }
        ////////////////////
        /// Returns vertex.
        /// \param nCorner Which corner vertex of triangle (0,1 or 2)
        /// \returns Reference to vertex.
        const CVertex & GetVertex( unsigned int nCorner ) const
        {
  	return m_vVertices[ nCorner % 3];
        }
        ////////////////////
        /// Calculates triangle face normal.
        void CalculateFaceNormal( Moose::Math::CVector3<float> & vResult )
        {
  	vResult = (m_vVertices[2].GetPosition() - m_vVertices[0].GetPosition()).Cross(m_vVertices[1].GetPosition() - m_vVertices[0].GetPosition());
  	vResult.Normalize();
        }

      };
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSETRIANGLE_H_ */
