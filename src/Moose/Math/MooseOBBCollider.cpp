///////////////////////////////////////////////////////////////////////////////
#include "MooseOBBCollider.h"
#include "MooseCollision.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Collision;
using namespace Moose::Collision;
using namespace Moose::Volume;
using namespace Moose::Math;
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
prefix::COBBCollider::Intersects( const Moose::Volume::CSphere & sphere ) const
{
	if ( this->m_pTransform )
	{
        COrientedBox box = GetBoundingBox();
		box.Move( m_pTransform->GetTranslation());
		box.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		box.SetWidth(box.GetWidth()*vScale[0]);
        box.SetHeight(box.GetHeight()*vScale[1]);
        box.SetLength(box.GetLength()*vScale[2]);
        
		//box.CalculateCorners();
		//box.CalculatePlanes();
		return SphereIntersectsOBB( sphere, box );
	} else return SphereIntersectsOBB( sphere, GetBoundingBox() );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Graphics::CFrustum & frustum ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox box = GetBoundingBox();
		box.Move( m_pTransform->GetTranslation());
		box.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		box.SetWidth(box.GetWidth()*vScale[0]);
        box.SetHeight(box.GetHeight()*vScale[1]);
        box.SetLength(box.GetLength()*vScale[2]);
        //box.CalculateCorners();
		//box.CalculatePlanes();
		return OBBIntersectsPolytope( box, frustum);
	} else return OBBIntersectsPolytope( GetBoundingBox(), frustum );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Volume::COrientedBox & box ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);
        //thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return OBBIntersectsOBB( thisbox, box);
	} else return OBBIntersectsOBB( GetBoundingBox(), box );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Math::CRay & ray, float *pfValue ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);//thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return RayIntersectsOBB( ray, thisbox, pfValue);
	} else return RayIntersectsOBB( ray, GetBoundingBox(), pfValue );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Math::CVector3<float> & vPoint ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);//thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return PointIntersectsOBB( vPoint, thisbox);
	} else return PointIntersectsOBB( vPoint, GetBoundingBox());
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Collision::ICollider & collider ) const
{
	if ( m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);//thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return collider.Intersects(thisbox);
	} else return collider.Intersects(GetBoundingBox());
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::COBBCollider::Render( Moose::Graphics::COglRenderer & renderer )
{
	if ( m_pTransform ) renderer.CommitTransform( *m_pTransform );
	renderer.CommitBox( GetBoundingBox(), true );
	if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Volume::CCapsule & capsule ) const
{
    if ( m_pTransform )
    {

        COrientedBox thisbox = GetBoundingBox();
        thisbox.Move( m_pTransform->GetTranslation());
        thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);
        // thisbox.CalculateCorners();
        //thisbox.CalculatePlanes();
        return Moose::Collision::OBBIntersectsCapsule( thisbox, capsule);
    } else return Moose::Collision::OBBIntersectsCapsule( GetBoundingBox(), capsule);
}
///////////////////////////////////////////////////////////////////////////////
