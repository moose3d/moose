///////////////////////////////////////////////////////////////////////////////
#ifndef __PhoenixCollider_h__
#define __PhoenixCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixMath.h"
#include "PhoenixVolume.h"
#include "PhoenixFrustum.h"
#include "PhoenixTransform.h"
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
			Phoenix::Math::CTransform * m_pTransform;
		protected:
			ICollider() : m_pTransform(NULL) { }
		public:
			virtual ~ICollider() {};
			virtual bool Intersects( const Phoenix::Volume::CSphere & sphere   ) const = 0;
			virtual bool Intersects( const Phoenix::Graphics::CFrustum & frustum ) const = 0;
			virtual bool Intersects( const Phoenix::Volume::COrientedBox & box ) const = 0;

// TODO implement intersection tests

//			virtual bool Intersects( const Phoenix::Volume::CCapsule & capsule ) const = 0;
//			virtual bool Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb) const = 0;
//			virtual bool Intersects( const Phoenix::Volume::CCone & cone ) const = 0;
//			virtual bool Intersects( const Phoenix::Math::CRay & ray ) const = 0;
//			virtual bool Intersects( const Phoenix::Math::CPlane & plane ) const = 0;
//			virtual bool Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const = 0;
//			virtual bool Intersects( const Phoenix::Math::CLine & line ) const = 0;
			virtual bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const = 0;
			virtual bool Intersects( const Phoenix::Collision::ICollider & collider ) const = 0;
			void SetTransform( Phoenix::Math::CTransform *pTransform ) { m_pTransform = pTransform; }
			Phoenix::Math::CTransform * GetTransform() { return m_pTransform; }
		};
	}
}
///////////////////////////////////////////////////////////////////////////////
#endif /* PHOENIXCOLLIDER_H_ */
