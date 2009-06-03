/*
 * PhoenixCollider.h
 *
 *  Created on: Jun 1, 2009
 *      Author: entity
 */
///////////////////////////////////////////////////////////////////////////////
#ifndef __PhoenixCollider_h__
#define __PhoenixCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixMath.h"
#include "PhoenixVolume.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Collision
	{
		////////////////////
		/// Interface for colliders.
		class ICollider
		{
		protected:
			ICollider() {}
		public:
			virtual ~ICollider() {};
			virtual bool Intersects( const Phoenix::Volume::CSphere & sphere   ) const = 0;
			virtual bool Intersects( const Phoenix::Volume::COrientedBox & box ) const = 0;
			virtual bool Intersects( const Phoenix::Volume::CCapsule & capsule ) const = 0;
			virtual bool Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb) const = 0;
			virtual bool Intersects( const Phoenix::Volume::CCone & cone ) const = 0;
			virtual bool Intersects( const Phoenix::Math::CRay & ray ) const = 0;
			virtual bool Intersects( const Phoenix::Math::CPlane & plane ) const = 0;
			virtual bool Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const = 0;
			virtual bool Intersects( const Phoenix::Math::CLine & line ) const = 0;
			virtual bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const = 0;
		};
	}
}
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXCOLLIDER_H_ */
