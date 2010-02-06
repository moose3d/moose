///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSphereCollider.h"
#include "PhoenixCollision.h"
#include "PhoenixOGLRenderer.h"
namespace prefix = Phoenix::Collision;
using namespace Phoenix::Collision;
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
prefix::CSphereCollider::Intersects( const Phoenix::Volume::CSphere & sphere ) const
{
	if ( m_pTransform )
	{
		Phoenix::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
		return SphereIntersectsSphere( tmp, sphere );
	}
	return SphereIntersectsSphere(GetBoundingSphere(), sphere);
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Graphics::CFrustum & frustum ) const
{
	if ( m_pTransform )
	{
		Phoenix::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
		return SphereIntersectsPolytope(tmp, frustum);
	}
  return SphereIntersectsPolytope(GetBoundingSphere(), frustum);
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Volume::COrientedBox & box ) const
{
	if ( m_pTransform )
	{
		Phoenix::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
		return SphereIntersectsOBB(tmp, box);
	}
  return SphereIntersectsOBB( GetBoundingSphere(), box );
}
///////////////////////////////////////////////////////////////////////////////
/*bool
prefix::CSphereCollider::Intersects( const Phoenix::Volume::CCapsule & capsule ) const
{
  return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Volume::CCone & cone ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
*/
bool
prefix::CSphereCollider::Intersects( const Phoenix::Math::CRay & ray, float *pfValue ) const
{

	if ( m_pTransform )
		{
			Phoenix::Volume::CSphere tmp = GetBoundingSphere();
			tmp.Move( m_pTransform->GetTranslation());
			return RayIntersectsSphere(ray, pfValue, NULL, tmp);
		}
	return RayIntersectsSphere(ray, pfValue, NULL, this->GetBoundingSphere());
}
///////////////////////////////////////////////////////////////////////////////
/*bool
prefix::CSphereCollider::Intersects( const Phoenix::Math::CPlane & plane ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Math::CLine & line ) const
{
	return false;
	}*/
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const
{
	if ( m_pTransform )
	{
		Phoenix::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
		return PointInsideSphere(tmp, vPoint);
	}
  return PointInsideSphere( GetBoundingSphere(), vPoint );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CSphereCollider::Intersects( const Phoenix::Collision::ICollider & collider ) const
{
	if ( m_pTransform )
	{
		Phoenix::Volume::CSphere tmp = GetBoundingSphere();
		tmp.Move( m_pTransform->GetTranslation());
		return collider.Intersects(tmp);
	}
	return collider.Intersects(GetBoundingSphere());
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CSphereCollider::Render( Phoenix::Graphics::COglRenderer & renderer )
{
	if ( m_pTransform ) renderer.CommitTransform( *m_pTransform );
	renderer.CommitSphere( GetBoundingSphere() );
	if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
