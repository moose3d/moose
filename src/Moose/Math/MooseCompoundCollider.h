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
 * MooseCompoundCollider.h
 *
 *  Created on: 7 Feb 2010
 *      Author: entity
 */

#ifndef MOOSECOMPOUNDCOLLIDER_H_
#define MOOSECOMPOUNDCOLLIDER_H_

#include "MooseCollider.h"
#include <vector>
namespace Moose
{
	namespace Collision
	{
		typedef std::vector<ICollider *> ColliderVector;
		class CCompoundCollider: public Moose::Collision::ICollider
		{
		protected:
			ColliderVector m_vecColliders;
		public:
			CCompoundCollider( );
			virtual ~CCompoundCollider();

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
			void SetColliderTransform( Moose::Math::CTransform *pTransform );
			void Render( Moose::Graphics::COglRenderer & renderer );
			void AddCollider( ICollider *pCollider );
			ColliderVector & GetColliders();
			const ColliderVector & GetColliders() const;
			ICollider * At( int index );

		};
	}

}

#endif /* MOOSECOMPOUNDCOLLIDER_H_ */
