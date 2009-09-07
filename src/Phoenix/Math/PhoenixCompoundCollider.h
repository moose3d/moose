/*
 * PhoenixSphereCollider.h
 *
 *  Created on: Jun 1, 2009
 *      Author: entity
 */
#ifndef __PhoenixCompoundCollider_h__
#define __PhoenixCompoundCollider_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixCollider.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{
	namespace Collision
	{
	        typedef std::list< ICollider *> ColliderList;
		///////////////////
		/// Collider with several separate colliders.
	        class PhoenixCompoundCollider : public ICollider
		{
		protected:
		  std::list< ICollider *> m_lstColliders;
		public:
			PhoenixCompoundCollider();
			virtual ~PhoenixCompoundCollider();

			bool Intersects( const Phoenix::Volume::CSphere & sphere   ) const;
			bool Intersects( const Phoenix::Graphics::CFrustum & frustum ) const;
			bool Intersects( const Phoenix::Volume::COrientedBox & box ) const;
			//bool Intersects( const Phoenix::Volume::CCapsule & capsule ) const;
			//bool Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb) const;
			//bool Intersects( const Phoenix::Volume::CCone & cone ) const;
			//bool Intersects( const Phoenix::Math::CRay & ray ) const;
			//bool Intersects( const Phoenix::Math::CPlane & plane ) const;
			//bool Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const;
			//bool Intersects( const Phoenix::Math::CLine & line ) const;
			bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const;
			bool Intersects( const Phoenix::Collision::ICollider & collider ) const;

		}; // PhoenixCompoundCollider
	};// Collision
}; // Phoenix
///////////////////////////////////////////////////////////////////////////////
#endif
