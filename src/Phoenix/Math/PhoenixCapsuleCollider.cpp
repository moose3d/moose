#include "PhoenixCapsuleCollider.h"
#include "PhoenixCollision.h"
#include "PhoenixLogger.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixOGLRenderer.h"
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
namespace prefix=Phoenix::Collision;
using namespace Phoenix::Core;
using namespace Phoenix::Collision;
using namespace Phoenix::Volume;
using namespace Phoenix::Math;
///////////////////////////////////////////////////////////////////////////////
prefix::CCapsuleCollider::CCapsuleCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
prefix::CCapsuleCollider::~CCapsuleCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Phoenix::Volume::CSphere & sphere ) const
{
    if ( m_pTransform )
    {
        CVector3<float > vStart, vEnd;
        Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
        Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
        CCapsule capsule(vStart, vEnd, GetBoundingCapsule().GetRadius());
        return SphereIntersectsCapsule(sphere, capsule);
    } return SphereIntersectsCapsule( sphere, GetBoundingCapsule());
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Phoenix::Graphics::CFrustum & frustum ) const
{
    if ( m_pTransform )
    {
        CVector3<float > vStart, vEnd;
        Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
        Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
        CCapsule capsule(vStart, vEnd, GetBoundingCapsule().GetRadius());

        return CapsuleIntersectsPolytope(capsule, frustum);
    } return CapsuleIntersectsPolytope( GetBoundingCapsule(), frustum);
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Phoenix::Volume::COrientedBox & box ) const
{
    if ( m_pTransform )
    {
        CVector3<float > vStart, vEnd;
        Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
        Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
        CCapsule capsule(vStart, vEnd, GetBoundingCapsule().GetRadius());
        return OBBIntersectsCapsule(box, capsule);
    } return OBBIntersectsCapsule( box, GetBoundingCapsule());
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Phoenix::Volume::CCapsule & capsule ) const
{
    float fDistanceSqr;
    float fRadiiSum;
    if ( m_pTransform )
    {
        CVector3<float > vStart, vEnd;
        Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
        Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
        CCapsule capsTransf(vStart, vEnd, GetBoundingCapsule().GetRadius());
        fDistanceSqr = LineSegmentToLineSegmentDistanceSquared( capsule, capsTransf);
        fRadiiSum = capsule.GetRadius() + capsTransf.GetRadius();
    }
    else
    {
        fDistanceSqr = LineSegmentToLineSegmentDistanceSquared( capsule, GetBoundingCapsule());
        fRadiiSum = capsule.GetRadius() + GetBoundingCapsule().GetRadius();
    }
    return ( fDistanceSqr <= fRadiiSum * fRadiiSum );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Phoenix::Math::CRay & ray, float *pfValue ) const
{

    CVector3<float> vStart, vEnd;
    float fDistanceSqr;
    float fDistAlongCapsuleLineSegment;

    if ( m_pTransform )
    {
        // Transform original capsule
        Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
        Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
        // Create transformed capsule
        CCapsule capsTransf(vStart, vEnd, GetBoundingCapsule().GetRadius());
        // check closest points on ray
        ClosestPointOnRay( capsTransf.GetStart(), ray, vStart);
        ClosestPointOnRay( capsTransf.GetEnd(),   ray, vEnd);
        // Do line segment check
        CLineSegment vTmp(vStart,vEnd);
        fDistanceSqr = LineSegmentToLineSegmentDistanceSquared( vTmp, capsTransf, NULL, &fDistAlongCapsuleLineSegment);
    }
    else
    {
        ClosestPointOnRay( GetBoundingCapsule().GetStart(), ray, vStart);
        ClosestPointOnRay( GetBoundingCapsule().GetEnd(),   ray, vEnd);
        CLineSegment vTmp(vStart,vEnd);
        fDistanceSqr = LineSegmentToLineSegmentDistanceSquared( vTmp, GetBoundingCapsule(), NULL, &fDistAlongCapsuleLineSegment);
    }

    if ( pfValue )
    {
        g_Error << __PRETTY_FUNCTION__ << ": Setting pfValue not implemented." << std::endl;

    }
    return ( fDistanceSqr <= GetBoundingCapsule().GetRadiusSqr());
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const
{
    CVector3<float> vTmp;
    if ( m_pTransform )
    {
        CVector3<float > vStart, vEnd;
        Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
        Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
        CCapsule capsTransf(vStart, vEnd, GetBoundingCapsule().GetRadius());
        ClosestPointOnLineSegment( vPoint, capsTransf, vTmp);
        return ((vTmp-vPoint).LengthSqr() <= capsTransf.GetRadiusSqr());
    }
    else
    {
        ClosestPointOnLineSegment( vPoint, GetBoundingCapsule(), vTmp);
        return ((vTmp-vPoint).LengthSqr() <= GetBoundingCapsule().GetRadiusSqr());
    }

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Phoenix::Collision::ICollider & collider ) const
{
    if ( m_pTransform )
    {
        CVector3<float > vStart, vEnd;
        Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
        Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
        CCapsule capsTransf(vStart,vEnd, GetBoundingCapsule().GetRadius());
        return collider.Intersects( capsTransf );
    }
    return collider.Intersects( GetBoundingCapsule());
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CCapsuleCollider::Render( Phoenix::Graphics::COglRenderer & renderer )
{
    if ( m_pTransform ) renderer.CommitTransform( *m_pTransform );
    renderer.CommitCapsule( GetBoundingCapsule() );
    if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Volume::CCapsule &
prefix::CCapsuleCollider::GetBoundingCapsule()
{
    return m_Capsule;
}
///////////////////////////////////////////////////////////////////////////////
const Phoenix::Volume::CCapsule &
prefix::CCapsuleCollider::GetBoundingCapsule() const
{
    return m_Capsule;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CCapsuleCollider::SetBoundingCapsule( Phoenix::Volume::CCapsule & capsule )
{
    m_Capsule = capsule;
}
///////////////////////////////////////////////////////////////////////////////
