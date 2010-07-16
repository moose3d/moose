/*
 * MooseSphereCollider.h
 *
 *  Created on: Jun 1, 2009
 *      Author: entity
 */

#ifndef __MooseSphereCollider_h__
#define __MooseSphereCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "MooseCollider.h"
#include "MooseSphereBound.h"
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
		///////////////////
		/// Sphere-based collider
	    class CSphereCollider : public Moose::Collision::ICollider,
								 public Moose::Volume::CSphereBound
		{
		public:
			CSphereCollider();
			virtual ~CSphereCollider();

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
		}; // CSphereCollider
	};// Collision
}; // Moose
///////////////////////////////////////////////////////////////////////////////
#endif /* MOOSESPHERECOLLIDER_H_ */
