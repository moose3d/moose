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

#ifndef MOOSEAACUBE_H_
#define MOOSEAACUBE_H_
#include "MooseVector3.h"
#include "MoosePositional.h"
#include "MooseDimensional1D.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseTransform.h"
#include <iostream>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
  /////////////////////////////////////////////////////////////////
    /// Class for axis-aligned (x,y,z) cube.
    class MOOSE_API CAxisAlignedCube : public Moose::Spatial::CPositional,
					 public Moose::Spatial::CDimensional1D
    {
    public:
      ////////////////////
      /// The constructor.
      CAxisAlignedCube()    {}
      ////////////////////
      /// The parameterized constructor.
      CAxisAlignedCube( const Moose::Math::CVector3<float> &vCenter, float fEdgeLength ) :
    		  Moose::Spatial::CPositional( vCenter ), Moose::Spatial::CDimensional1D( fEdgeLength ) {  }
      Moose::Math::CVector3<float> GetMin() const
      {
        return GetPosition()-Moose::Math::CVector3<float>(GetHalfWidth(),GetHalfWidth(),GetHalfWidth());
      }
      Moose::Math::CVector3<float> GetMax() const
      {
        return GetPosition()+Moose::Math::CVector3<float>(GetHalfWidth(),GetHalfWidth(),GetHalfWidth());
      }
    }; // AACube
  } // Volume
} // Moose
#endif /* MOOSEAACUBE_H_ */
