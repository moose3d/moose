///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSphereCollider.h"
#include "PhoenixCollision.h"
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
		SphereIntersectsPolytope(tmp, frustum);
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
		SphereIntersectsOBB(tmp, box);
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
bool
prefix::CSphereCollider::Intersects( const Phoenix::Math::CRay & ray ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
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
		PointInsideSphere(tmp, vPoint);
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
		collider.Intersects(tmp);
	}
  return collider.Intersects(GetBoundingSphere());
}
///////////////////////////////////////////////////////////////////////////////
