/*
 * PhoenixSphereCollider.h
 *
 *  Created on: Jun 1, 2009
 *      Author: entity
 */

#ifndef __PhoenixSphereCollider_h__
#define __PhoenixSphereCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixCollider.h"
#include "PhoenixSphereBound.h"
#include "PhoenixRay.h"
#include "PhoenixSphere.h"
#include "PhoenixOBB.h"
#include "PhoenixFrustum.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Collision
	{
		///////////////////
		/// Sphere-based collider
	    class CSphereCollider : public ICollider,
								 public Phoenix::Volume::CSphereBound
		{
		public:
			CSphereCollider();
			virtual ~CSphereCollider();

			bool Intersects( const Phoenix::Volume::CSphere & sphere   ) const;
			bool Intersects( const Phoenix::Graphics::CFrustum & frustum ) const;
			bool Intersects( const Phoenix::Volume::COrientedBox & box ) const;
			//bool Intersects( const Phoenix::Volume::CCapsule & capsule ) const;
			//bool Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb) const;
			//bool Intersects( const Phoenix::Volume::CCone & cone ) const;
			bool Intersects( const Phoenix::Math::CRay & ray, float *pfValue = NULL ) const;
			//bool Intersects( const Phoenix::Math::CPlane & plane ) const;
			//bool Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const;
			//bool Intersects( const Phoenix::Math::CLine & line ) const;
			bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const;
			bool Intersects( const Phoenix::Collision::ICollider & collider ) const;

			void Render( Phoenix::Graphics::COglRenderer & renderer );
		}; // CSphereCollider
	};// Collision
}; // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXSPHERECOLLIDER_H_ */
