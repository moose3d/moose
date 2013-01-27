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

#ifndef __MooseOBBCollider_h__
#define __MooseOBBCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "MooseCollider.h"
#include "MooseBoxBound.h"
#include "MooseRay.h"
#include "MooseSphere.h"
#include "MooseOBB.h"
#include "MooseFrustum.h"
#include "MooseCapsule.h"
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Collision
	{
		class COBBCollider : public Moose::Collision::ICollider,
							  public Moose::Volume::CBoxBound
		{
		public:
			COBBCollider();
			virtual ~COBBCollider();
			bool Intersects( const Moose::Volume::CSphere & sphere   ) const;
			bool Intersects( const Moose::Graphics::CFrustum & frustum ) const;
			bool Intersects( const Moose::Volume::COrientedBox & box ) const;
			bool Intersects( const Moose::Volume::CCapsule & capsule ) const;
			//bool Intersects( const Moose::Volume::CAxisAlignedBox & aabb) const;
			//bool Intersects( const Moose::Volume::CCone & cone ) const;
			bool Intersects( const Moose::Math::CRay & ray, float *pfValue = NULL ) const;
			//bool Intersects( const Moose::Math::CPlane & plane ) const;
			//bool Intersects( const Moose::Math::CLineSegment & lineSegment ) const;
			//bool Intersects( const Moose::Math::CLine & line ) const;
			bool Intersects( const Moose::Math::CVector3<float> & vPoint ) const;
			bool Intersects( const Moose::Collision::ICollider & collider ) const;

			void Render( Moose::Graphics::COglRenderer & renderer );
		};
	}
}
///////////////////////////////////////////////////////////////////////////////
#endif /* MOOSEOBBCOLLIDER_H_ */
