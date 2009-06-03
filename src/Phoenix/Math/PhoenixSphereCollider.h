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
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Collision
	{
		///////////////////
		/// Sphere-based collider
		class PhoenixSphereCollider : public ICollider
		{
		public:
			PhoenixSphereCollider();
			virtual ~PhoenixSphereCollider();

			bool Intersects( const Phoenix::Volume::CSphere & sphere   ) const;
			bool Intersects( const Phoenix::Volume::COrientedBox & box ) const;
			bool Intersects( const Phoenix::Volume::CCapsule & capsule ) const;
			bool Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb) const;
			bool Intersects( const Phoenix::Volume::CCone & cone ) const;
			bool Intersects( const Phoenix::Math::CRay & ray ) const;
			bool Intersects( const Phoenix::Math::CPlane & plane ) const;
			bool Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const;
			bool Intersects( const Phoenix::Math::CLine & line ) const;
			bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const;
		}; // PhoenixSphereCollider
	};// Collision
}; // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXSPHERECOLLIDER_H_ */
