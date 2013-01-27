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

#ifndef __MooseAlphaTestOperation_h__
#define __MooseAlphaTestOperation_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseOGLConsts.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class grouping alpha testing properties.
    class MOOSE_API CAlphaTestOperation : public Moose::Core::CEnableable
    {
    private:
      Moose::Graphics::ALPHA_TEST_TYPE m_tTestType;
      float	      m_fRefValue;
    public:
      ////////////////////
      /// Constructor.
      CAlphaTestOperation() : m_tTestType(Moose::Graphics::ALPHA_ALWAYS), m_fRefValue(0.0f) {}
      ////////////////////
      /// Assigns test type.
      /// \param tType ALPHA_TEST_TYPE.
      inline void SetTest( Moose::Graphics::ALPHA_TEST_TYPE tType )
      { 
	m_tTestType = tType; 
      }
      ////////////////////
      /// Returns current alpha test.
      /// \returns Current ALPHA_TEST_TYPE.
      inline Moose::Graphics::ALPHA_TEST_TYPE GetTest() const 
      { 
	return m_tTestType; 
      }
      ////////////////////
      /// Assigns reference value.
      /// \param fValue Reference value to be set.
      inline void SetReference( float fValue )
      {
	m_fRefValue = fValue;
      }
      ////////////////////
      /// Returns reference value.
      /// \brief Returns current reference value.
      inline float GetReference () const
      {
	return m_fRefValue;
      }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
