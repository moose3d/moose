#ifndef __PhoenixOBBCollider_h__
#define __PhoenixOBBCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixCollider.h"
#include "PhoenixBoxBound.h"
#include "PhoenixRay.h"
#include "PhoenixSphere.h"
#include "PhoenixOBB.h"
#include "PhoenixFrustum.h"
#include "PhoenixCapsule.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Collision
	{
		class COBBCollider : public Phoenix::Collision::ICollider,
							  public Phoenix::Volume::CBoxBound
		{
		public:
			COBBCollider();
			virtual ~COBBCollider();
			bool Intersects( const Phoenix::Volume::CSphere & sphere   ) const;
			bool Intersects( const Phoenix::Graphics::CFrustum & frustum ) const;
			bool Intersects( const Phoenix::Volume::COrientedBox & box ) const;
			bool Intersects( const Phoenix::Volume::CCapsule & capsule ) const;
			//bool Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb) const;
			//bool Intersects( const Phoenix::Volume::CCone & cone ) const;
			bool Intersects( const Phoenix::Math::CRay & ray, float *pfValue = NULL ) const;
			//bool Intersects( const Phoenix::Math::CPlane & plane ) const;
			//bool Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const;
			//bool Intersects( const Phoenix::Math::CLine & line ) const;
			bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const;
			bool Intersects( const Phoenix::Collision::ICollider & collider ) const;

			void Render( Phoenix::Graphics::COglRenderer & renderer );
		};
	}
}
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXOBBCOLLIDER_H_ */
