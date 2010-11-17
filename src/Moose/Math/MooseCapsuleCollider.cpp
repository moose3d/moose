#include "MooseCapsuleCollider.h"
#include "MooseCollision.h"
#include "MooseLogger.h"
#include "MooseDefaultEntities.h"
#include "MooseOGLRenderer.h"
#include <iostream>
#include <algorithm>
///////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Collision;
using namespace Moose::Core;
using namespace Moose::Collision;
using namespace Moose::Volume;
using namespace Moose::Math;
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
prefix::CCapsuleCollider::Intersects( const Moose::Volume::CSphere & sphere ) const
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
prefix::CCapsuleCollider::Intersects( const Moose::Graphics::CFrustum & frustum ) const
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
prefix::CCapsuleCollider::Intersects( const Moose::Volume::COrientedBox & box ) const
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
prefix::CCapsuleCollider::Intersects( const Moose::Volume::CCapsule & capsule ) const
{


    if ( m_pTransform )
    {
      // Transform capsule
      CVector3<float > vStart, vEnd;
      Transform( GetBoundingCapsule().GetStart(), *m_pTransform, vStart);
      Transform( GetBoundingCapsule().GetEnd(), *m_pTransform, vEnd);
      CCapsule capsTransf(vStart, vEnd, GetBoundingCapsule().GetRadius());
      return CapsuleIntersectsCapsule( capsTransf, capsule);
    }
    else
    {
      return CapsuleIntersectsCapsule( GetBoundingCapsule(), capsule);
    }
    /*float fDistanceSqr;
    float fRadiiSum;*/
    /*fDistanceSqr = LineSegmentToLineSegmentDistanceSquared( capsule, capsTransf);
      fRadiiSum = capsule.GetRadius() + capsTransf.GetRadius();*/    
    /*fDistanceSqr = LineSegmentToLineSegmentDistanceSquared( capsule, GetBoundingCapsule());
      fRadiiSum = capsule.GetRadius() + GetBoundingCapsule().GetRadius();*/
    /*return ( fDistanceSqr <= fRadiiSum * fRadiiSum );*/
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCapsuleCollider::Intersects( const Moose::Math::CRay & ray, float *pfValue ) const
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
prefix::CCapsuleCollider::Intersects( const Moose::Math::CVector3<float> & vPoint ) const
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
prefix::CCapsuleCollider::Intersects( const Moose::Collision::ICollider & collider ) const
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
prefix::CCapsuleCollider::Render( Moose::Graphics::COglRenderer & renderer )
{
  if ( m_pTransform ) renderer.CommitTransform( *m_pTransform );
  renderer.CommitCapsule( GetBoundingCapsule() );
  if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
Moose::Volume::CCapsule &
prefix::CCapsuleCollider::GetBoundingCapsule()
{
    return m_Capsule;
}
///////////////////////////////////////////////////////////////////////////////
const Moose::Volume::CCapsule &
prefix::CCapsuleCollider::GetBoundingCapsule() const
{
    return m_Capsule;
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CCapsuleCollider::SetBoundingCapsule( Moose::Volume::CCapsule & capsule )
{
    m_Capsule = capsule;
}
///////////////////////////////////////////////////////////////////////////////
