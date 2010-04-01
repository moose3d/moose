/*
 * CCapsuleCollider.h
 *
 *  Created on: 14 Mar 2010
 *      Author: entity
 */

#ifndef __CCapsuleCollider_h__
#define __CCapsuleCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixCollider.h"
#include "PhoenixMath.h"
#include "PhoenixSphere.h"
#include "PhoenixOBB.h"
#include "PhoenixRay.h"
#include "PhoenixFrustum.h"
#include "PhoenixCapsule.h"
#include "PhoenixTransform.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{

    namespace Collision
    {

        class CCapsuleCollider: public Phoenix::Collision::ICollider
        {
        protected:
            Phoenix::Volume::CCapsule m_Capsule;
        public:
            CCapsuleCollider();
            virtual ~CCapsuleCollider();
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
            Phoenix::Volume::CCapsule & GetBoundingCapsule();
            const Phoenix::Volume::CCapsule & GetBoundingCapsule() const;
            void SetBoundingCapsule(Phoenix::Volume::CCapsule & capsule);
        };

    }

}

#endif /* CCAPSULECOLLIDER_H_ */
