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

#ifndef __MOOSEDIMENSIONAL2D_H__
#define __MOOSEDIMENSIONAL2D_H__
/////////////////////////////////////////////////////////////
#include <MooseAPI.h>
#include <MooseDimensional1D.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
  /////////////////////////////////////////////////////////////////
     /// A class which provides dimensions in 2D - width, height.
     class MOOSE_API CDimensional2D : public CDimensional1D
     {
     protected:
       /// The height.
       float m_fHeight;
       /// Height * 0.5
       float m_fHalfHeight;
       ////////////////////
       /// The constructor. Initializes paramaters.
       CDimensional2D() :
 	CDimensional1D(),
 	m_fHeight(0.0f),
 	m_fHalfHeight(0.0f)
       {

       }
       ////////////////////
       /// The parametrized constructor.
       /// \param fWidth The width of the object
       /// \param fHeight The height of the object
       CDimensional2D( float fWidth, float fHeight)
       {
 	SetWidth(fWidth);
 	SetHeight(fHeight);
       }
     public:
       ////////////////////
       /// Assigns the height.
       /// \param fHeight The value for height.
       inline void SetHeight( float fHeight )
       {
 	m_fHeight = fHeight;
 	m_fHalfHeight = m_fHeight * 0.5f;
       }
       ////////////////////
       /// Returns the height.
       /// \returns The height.
       inline float GetHeight() const
       {
 	return m_fHeight;
       }
       ////////////////////
       /// Returns half of the height
       /// \returns half of the height
       inline float GetHalfHeight() const
       {
 	return m_fHalfHeight;
       }
     }; // CDimensional2D
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEDIMENSIONAL2D_H_ */
