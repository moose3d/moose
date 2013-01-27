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

/*
 * CCapsuleCollider.h
 *
 *  Created on: 14 Mar 2010
 *      Author: entity
 */

#ifndef __CCapsuleCollider_h__
#define __CCapsuleCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "MooseCollider.h"
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

    namespace Collision
    {

        class CCapsuleCollider: public Moose::Collision::ICollider
        {
        protected:
            Moose::Volume::CCapsule m_Capsule;
        public:
            CCapsuleCollider();
            virtual ~CCapsuleCollider();
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
            Moose::Volume::CCapsule & GetBoundingCapsule();
            const Moose::Volume::CCapsule & GetBoundingCapsule() const;
            void SetBoundingCapsule(Moose::Volume::CCapsule & capsule);
        };

    }

}

#endif /* CCAPSULECOLLIDER_H_ */
