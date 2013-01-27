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

#ifndef MOOSEDECALVOLUME_H_
#define MOOSEDECALVOLUME_H_
#include "MoosePolytope.h"
#include "MoosePositional.h"
#include "MooseDimensional3D.h"
#include "MooseOrientable.h"
#include "MooseVector3.h"
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Volume
  {
  	/////////////////////////////////////////////////////////////////
    /// Decal volume class. Used for creating scorch marks, for instance.
    class MOOSE_API CDecalVolume : public Moose::Volume::CPolytope,
				     public Moose::Spatial::CPositional,
				     public Moose::Spatial::CDimensional3D,
				     protected Moose::Spatial::COrientable
    {
    public:
      ////////////////////
      /// Constructor. Creates a volume with plane normals pointing towards center.
      /// \param vPosition	Decal center.
      /// \param vNormal	Decal unit normal vector.
      /// \param vNormal	Decal unit tangent vector.
      /// \param fWidth		Width of decal.
      /// \param fHeight	Height of decal.
      /// \param fLength	Length of decal - used for front and back plane clipping.
      CDecalVolume( const Moose::Math::CVector3<float> & vPosition,
		    const Moose::Math::CVector3<float> & vNormal,
		    const Moose::Math::CVector3<float> & vTangent,
		    float fWidth, float fHeight, float fLength) : Moose::Spatial::CPositional(vPosition),
      Moose::Spatial::CDimensional3D( fWidth, fHeight, fLength )
      {
				SetDirectionForwardUp( vNormal, vTangent );

				// T  P
				// left

				Moose::Math::CPlane plane;
				plane.Calculate( -GetTangentVector(), GetPosition() + GetTangentVector()*GetHalfWidth());
				AddPlane( plane );
				// right
				plane.Calculate( GetTangentVector(), GetPosition() - GetTangentVector()*GetHalfWidth());
				AddPlane( plane );

				// B  P
				// Bottom
				plane.Calculate( -GetBitangentVector(), GetPosition() + GetBitangentVector()*GetHalfHeight() );
				AddPlane( plane );
				// top
				plane.Calculate( GetBitangentVector(), GetPosition() - GetBitangentVector()*GetHalfHeight()  );
				AddPlane( plane );

				// N P
				// front
				plane.Calculate( -GetNormalVector(), GetPosition() + GetNormalVector() * GetHalfLength() );
				AddPlane( plane );
				// back
				plane.Calculate(  GetNormalVector(), GetPosition() - GetNormalVector() * GetHalfLength() );
				AddPlane( plane );
      }

      const Moose::Math::CVector3<float> & GetNormalVector() const
      {
      	return GetForwardVector();
      }

      const Moose::Math::CVector3<float> & GetTangentVector() const
      {
      	return GetUpVector();
      }

      const Moose::Math::CVector3<float> & GetBitangentVector() const
      {
      	return GetRightVector();
      }
    }; //class CDecalVolume
  } // Volume
} // Moose
#endif /* MOOSEDECALVOLUME_H_ */
