/*
 * PhoenixCompoundCollider.h
 *
 *  Created on: 7 Feb 2010
 *      Author: entity
 */

#ifndef PHOENIXCOMPOUNDCOLLIDER_H_
#define PHOENIXCOMPOUNDCOLLIDER_H_

#include "PhoenixCollider.h"
#include <vector>
namespace Phoenix
{
	namespace Collision
	{
		typedef std::vector<ICollider *> ColliderVector;
		class CCompoundCollider: public Phoenix::Collision::ICollider
		{
		protected:
			ColliderVector m_vecColliders;
		public:
			CCompoundCollider( );
			virtual ~CCompoundCollider();

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
			void SetColliderTransform( Phoenix::Math::CTransform *pTransform );
			void Render( Phoenix::Graphics::COglRenderer & renderer );
			void AddCollider( ICollider *pCollider );
			ColliderVector & GetColliders();
			const ColliderVector & GetColliders() const;
			ICollider * At( int index );

		};
	}

}

#endif /* PHOENIXCOMPOUNDCOLLIDER_H_ */
