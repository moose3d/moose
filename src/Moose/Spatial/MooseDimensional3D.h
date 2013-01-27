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

#ifndef MOOSEDIMENSIONAL3D_H_
#define MOOSEDIMENSIONAL3D_H_
/////////////////////////////////////////////////////////////
#include <MooseAPI.h>
#include <MooseDimensional2D.h>
namespace Moose
{
  namespace Spatial
  {
  /////////////////////////////////////////////////////////////////
     /// A class which provides dimensions in 3D - width, height and length.
     class MOOSE_API CDimensional3D : public CDimensional2D
     {
     protected:
       /// The length.
       float m_fLength;
       /// Length * 0.5
       float m_fHalfLength;
       ////////////////////
       /// The constructor. Initializes paramaters.
       CDimensional3D() :
 	CDimensional2D(),
 	m_fLength(0.0f),
 	m_fHalfLength(0.0f)
       {

       }
       ////////////////////
       /// The parametrized constructor.
       /// \param fWidth The width of the object
       /// \param fHeight The height of the object
       /// \param fLength The length of the object
       CDimensional3D( float fWidth, float fHeight, float fLength )
       {
 	SetWidth(fWidth);
 	SetHeight(fHeight);
 	SetLength(fLength);
       }
     public:
       ////////////////////
       /// Assigns the length.
       /// \param fLength The value for length.
       inline void SetLength( float fLength )
       {
 	m_fLength = fLength;
 	m_fHalfLength = m_fLength * 0.5f;
       }
       ////////////////////
       /// Returns the length.
       /// \returns The length.
       inline float GetLength() const
       {
 	return m_fLength;
       }
       ////////////////////
       /// Returns half of the length
       /// \returns half of the length
       inline float GetHalfLength() const
       {
 	return m_fHalfLength;
       }
     }; // CDimensional3D
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEDIMENSIONAL3D_H_ */
