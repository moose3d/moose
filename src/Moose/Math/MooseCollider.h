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

///////////////////////////////////////////////////////////////////////////////
#ifndef __MooseCollider_h__
#define __MooseCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "MooseMath.h"
#include "MooseSphere.h"
#include "MooseOBB.h"
#include "MooseRay.h"
#include "MooseFrustum.h"
#include "MooseCapsule.h"
#include "MooseTransform.h"
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
	namespace Graphics
	{
		class COglRenderer;

	}
	namespace Collision
	{
        ////////////////////
		/// Interface for colliders.
		class ICollider
		{
		protected:
			Moose::Math::CTransform * m_pTransform;
        protected:
			ICollider() : m_pTransform(NULL) { }
		public:
			virtual ~ICollider() {};
			virtual bool Intersects( const Moose::Volume::CSphere & sphere   ) const = 0;
			virtual bool Intersects( const Moose::Graphics::CFrustum & frustum ) const = 0;
			virtual bool Intersects( const Moose::Volume::COrientedBox & box ) const = 0;
			virtual bool Intersects( const Moose::Math::CRay & ray, float *pfDistance = NULL ) const = 0;
// TODO implement intersection tests
			virtual bool Intersects( const Moose::Volume::CCapsule & capsule ) const = 0;
//			virtual bool Intersects( const Moose::Volume::CAxisAlignedBox & aabb) const = 0;
//			virtual bool Intersects( const Moose::Volume::CCone & cone ) const = 0;

//			virtual bool Intersects( const Moose::Math::CPlane & plane ) const = 0;
//			virtual bool Intersects( const Moose::Math::CLineSegment & lineSegment ) const = 0;
//			virtual bool Intersects( const Moose::Math::CLine & line ) const = 0;
			virtual bool Intersects( const Moose::Math::CVector3<float> & vPoint ) const = 0;
			virtual bool Intersects( const Moose::Collision::ICollider & collider ) const = 0;
			virtual void SetColliderTransform( Moose::Math::CTransform *pTransform ) { m_pTransform = pTransform; }
			Moose::Math::CTransform * GetColliderTransform() { return m_pTransform; }
      ///////////////////
			/// Makes it possible to generate visual representations of colliders.
			virtual void Render( Moose::Graphics::COglRenderer & renderer ) = 0;
        };
	}
}
///////////////////////////////////////////////////////////////////////////////
#endif /* MOOSECOLLIDER_H_ */
