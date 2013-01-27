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

#ifndef __MOOSELINE_H__
#define __MOOSELINE_H__
/////////////////////////////////////////////////////////////
#include <MooseVector3.h>
#include <MooseOneDirectional.h>
#include <MoosePositional.h>
/////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
  /////////////////////////////////////////////////////////////////
      /// Abstraction for 3d lines. Infinitely thin and long, extending both directions
      /// from reference point.
      class MOOSE_API CLine : public Moose::Spatial::COneDirectional,
  		  public Moose::Spatial::CPositional
      {
      public:
        ////////////////////
        /// Default constructor.
        /// \param vPos Reference point through which line goes.
        /// \param vDir Line direction - should .
        CLine( const Moose::Math::CVector3<float> & vPos = Moose::Math::CVector3<float>(0,0,0),
  	     const Moose::Math::CVector3<float> & vDir = Moose::Math::CVector3<float>(0,0,-1) ) : Moose::Spatial::COneDirectional( vDir ),
  										       Moose::Spatial::CPositional(vPos){ }

      }; // class CLine
  } // Math
} // Moose
/////////////////////////////////////////////////////////////
#endif /* MOOSELINE_H_ */
