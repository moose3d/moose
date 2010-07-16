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
