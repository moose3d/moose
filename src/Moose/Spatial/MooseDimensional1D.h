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

#ifndef __MOOSEDIMENSIONAL1D_H__
#define __MOOSEDIMENSIONAL1D_H__
/////////////////////////////////////////////////////////////
#include <MooseAPI.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
  ////////////////////
  /// A class which provides dimensions in 1D (or n-dimensional object with equivalent size in all dimensions.
  class MOOSE_API CDimensional1D
  {
  protected:
    /// The width.
    float m_fWidth;
    /// Width * 0.5
    float m_fHalfWidth;
    ////////////////////
    /// Constructor. Initializes paramaters.
    CDimensional1D() :
	m_fWidth(0.0f),
	m_fHalfWidth(0.0f)
    {

    }
    ////////////////////
    /// The parametrized constructor.
    /// \param fWidth The width of the object
    CDimensional1D( float fWidth )
    {
	SetWidth(fWidth);
    }
  public:
    virtual ~CDimensional1D(){}
    ////////////////////
    /// Assigns the width.
    /// \param fWidth The value for width.
    inline void SetWidth ( float fWidth  )
    {
	m_fWidth = fWidth;
	m_fHalfWidth = m_fWidth * 0.5f;
    }
    ////////////////////
    /// Returns the width.
    /// \returns The width.
    inline float GetWidth() const
    {
	return m_fWidth;
    }
    ////////////////////
    /// Returns half of the width
    /// \returns half of the width
    inline float GetHalfWidth() const
    {
	return m_fHalfWidth;
    }

  }; // CDimensional1D
  }
}
/////////////////////////////////////////////////////////////
#endif /* MOOSEONEDIMENSIONAL_H_ */
