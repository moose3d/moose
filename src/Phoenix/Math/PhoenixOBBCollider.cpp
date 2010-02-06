///////////////////////////////////////////////////////////////////////////////
#include "PhoenixOBBCollider.h"
#include "PhoenixCollision.h"
#include "PhoenixOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
namespace prefix=Phoenix::Collision;
using namespace Phoenix::Collision;
using namespace Phoenix::Volume;
///////////////////////////////////////////////////////////////////////////////
prefix::COBBCollider::COBBCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
prefix::COBBCollider::~COBBCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Phoenix::Volume::CSphere & sphere ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox box = GetBoundingBox();
		box.Move( m_pTransform->GetTranslation());
		box.AppendToRotation( m_pTransform->GetRotation() );
		box.CalculateCorners();
		box.CalculatePlanes();
		return SphereIntersectsOBB( sphere, box );
	} else return SphereIntersectsOBB( sphere, GetBoundingBox() );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Phoenix::Graphics::CFrustum & frustum ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox box = GetBoundingBox();
		box.Move( m_pTransform->GetTranslation());
		box.AppendToRotation( m_pTransform->GetRotation() );
		box.CalculateCorners();
		box.CalculatePlanes();
		return OBBIntersectsPolytope( box, frustum);
	} else return OBBIntersectsPolytope( GetBoundingBox(), frustum );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Phoenix::Volume::COrientedBox & box ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
		thisbox.CalculateCorners();
		thisbox.CalculatePlanes();
		return OBBIntersectsOBB( thisbox, box);
	} else return OBBIntersectsOBB( GetBoundingBox(), box );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Phoenix::Math::CRay & ray, float *pfValue ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
		thisbox.CalculateCorners();
		thisbox.CalculatePlanes();
		return RayIntersectsOBB( ray, thisbox, pfValue);
	} else return RayIntersectsOBB( ray, GetBoundingBox(), pfValue );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
		thisbox.CalculateCorners();
		thisbox.CalculatePlanes();
		return PointIntersectsOBB( vPoint, thisbox);
	} else return PointIntersectsOBB( vPoint, GetBoundingBox());
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Phoenix::Collision::ICollider & collider ) const
{
	if ( m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
		thisbox.CalculateCorners();
		thisbox.CalculatePlanes();
		return collider.Intersects(thisbox);
	} else return collider.Intersects(GetBoundingBox());
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::COBBCollider::Render( Phoenix::Graphics::COglRenderer & renderer )
{
	if ( m_pTransform ) renderer.CommitTransform( *m_pTransform );
	renderer.CommitBox( GetBoundingBox(), true );
	if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
