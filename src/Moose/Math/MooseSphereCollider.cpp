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

///////////////////////////////////////////////////////////////////////////////
#include "MooseSphereCollider.h"
#include "MooseCollision.h"
#include "MooseOGLRenderer.h"
namespace prefix = Moose::Collision;
using namespace Moose::Collision;
///////////////////////////////////////////////////////////////////////////////
prefix::CSphereCollider::CSphereCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
prefix::CSphereCollider::~CSphereCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Volume::CSphere & sphere ) const
{
	if ( m_pTransform )
	{
		Moose::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
        float fMaxScale = std::max( std::max( m_pTransform->GetScaling()[0], 
                                             m_pTransform->GetScaling()[1]),
                                            m_pTransform->GetScaling()[2] );
        tmp.SetRadius( tmp.GetRadius() * fMaxScale );
		return SphereIntersectsSphere( tmp, sphere );
	}
	return SphereIntersectsSphere(GetBoundingSphere(), sphere);
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Graphics::CFrustum & frustum ) const
{
	if ( m_pTransform )
	{
		Moose::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
        float fMaxScale = std::max( std::max( m_pTransform->GetScaling()[0], 
                                             m_pTransform->GetScaling()[1]),
                                            m_pTransform->GetScaling()[2] );
        tmp.SetRadius( tmp.GetRadius() * fMaxScale );
		return SphereIntersectsPolytope(tmp, frustum);
	}
  return SphereIntersectsPolytope(GetBoundingSphere(), frustum);
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Volume::COrientedBox & box ) const
{
	if ( m_pTransform )
	{
		Moose::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
        float fMaxScale = std::max( std::max( m_pTransform->GetScaling()[0], 
                                             m_pTransform->GetScaling()[1]),
                                            m_pTransform->GetScaling()[2] );
        tmp.SetRadius( tmp.GetRadius() * fMaxScale );
		return SphereIntersectsOBB(tmp, box);
	}
  return SphereIntersectsOBB( GetBoundingSphere(), box );
}
///////////////////////////////////////////////////////////////////////////////
/*bool
prefix::CSphereCollider::Intersects( const Moose::Volume::CCapsule & capsule ) const
{
  return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Volume::CAxisAlignedBox & aabb ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Volume::CCone & cone ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
*/
bool
prefix::CSphereCollider::Intersects( const Moose::Math::CRay & ray, float *pfValue ) const
{

	if ( m_pTransform )
		{
			Moose::Volume::CSphere tmp = GetBoundingSphere();
			tmp.Move( m_pTransform->GetTranslation());
            float fMaxScale = std::max( std::max( m_pTransform->GetScaling()[0], 
                                                 m_pTransform->GetScaling()[1]),
                                       m_pTransform->GetScaling()[2] );
            tmp.SetRadius( tmp.GetRadius() * fMaxScale );
			return RayIntersectsSphere(ray, pfValue, NULL, tmp);
		}
	return RayIntersectsSphere(ray, pfValue, NULL, this->GetBoundingSphere());
}
///////////////////////////////////////////////////////////////////////////////
/*bool
prefix::CSphereCollider::Intersects( const Moose::Math::CPlane & plane ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Math::CLineSegment & lineSegment ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Math::CLine & line ) const
{
	return false;
	}*/
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Math::CVector3<float> & vPoint ) const
{
	if ( m_pTransform )
	{
		Moose::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
        float fMaxScale = std::max( std::max( m_pTransform->GetScaling()[0], 
                                             m_pTransform->GetScaling()[1]),
                                   m_pTransform->GetScaling()[2] );
        tmp.SetRadius( tmp.GetRadius() * fMaxScale );
		return PointInsideSphere(tmp, vPoint);
	}
  return PointInsideSphere( GetBoundingSphere(), vPoint );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Collision::ICollider & collider ) const
{
	if ( m_pTransform )
	{
		Moose::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
        float fMaxScale = std::max( std::max( m_pTransform->GetScaling()[0], 
                                             m_pTransform->GetScaling()[1]),
                                   m_pTransform->GetScaling()[0] );
        tmp.SetRadius( tmp.GetRadius() * fMaxScale );
		return collider.Intersects(tmp);
	}
	return collider.Intersects(GetBoundingSphere());
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Moose::Volume::CCapsule & capsule ) const
{
    if ( m_pTransform )
    {
        Moose::Volume::CSphere tmp = GetBoundingSphere();
        tmp.Move( m_pTransform->GetTranslation());
        float fMaxScale = std::max( std::max( m_pTransform->GetScaling()[0], 
                                             m_pTransform->GetScaling()[1]),
                                   m_pTransform->GetScaling()[0] );
        tmp.SetRadius( tmp.GetRadius() * fMaxScale );
        return SphereIntersectsCapsule(tmp, capsule);
    }
    return SphereIntersectsCapsule(GetBoundingSphere(), capsule);
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CSphereCollider::Render( Moose::Graphics::COglRenderer & renderer )
{
  if ( m_pTransform ) renderer.CommitTransform( *m_pTransform );
  renderer.CommitSphere( GetBoundingSphere() );
  if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
