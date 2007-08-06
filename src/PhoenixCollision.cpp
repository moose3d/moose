#include "PhoenixCollision.h"
using namespace Phoenix::Collision;
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
LINE_PLANE_INTERSECTION_TYPE
Phoenix::Collision::LineIntersectsPlane( const CPlane &plane,
					 const CVector3<float> &vPoint1,
					 const CVector3<float> &vPoint2,
					 CVector3<float> &vCollisionPoint )
{
  

  float fDistanceOne = 0.0f, fDistanceTwo = 0.0f;
  CVector3<float> vNormal;

  vNormal.UseExternalData( const_cast<CPlane &>(plane).GetArray());
  
  fDistanceOne = vNormal.Dot( vPoint1 ) + plane(3);
  fDistanceTwo = vNormal.Dot( vPoint2 ) + plane(3);
  
  // if other distance is positive and other negative, we have collision.
  // (which means that the endpoints are located in both sides of the plane)
  // Or other line point is in the plane. (distance == 0.0 )
  float fDistanceSqr = fDistanceOne * fDistanceTwo;
  if ( fDistanceSqr <= 0.0f )
  {
    vCollisionPoint = vPoint1 + (fDistanceOne * vNormal);
    return INTERSECTS;
  }
  else if ( fDistanceOne > 0.0f )
  {
    return FRONT;
  }
  else
  {
    return BEHIND;
  }

}
/////////////////////////////////////////////////////////////////
 // CVector3<float> 
// Phoenix::Collision::LinePlaneIntersectionPoint( const CPlane &plane,
// 						const CVector3<float> &vPoint1,
// 						const CVector3<float> &vPoint2 )
// {

  
//   // Get the normalized line direction vector
//   CVector3 vDir = vPoint2 - vPoint1;
//   vDir.ToUnit();
//   CVector3 vNormal;
//   vNormal.UseExternalData( plane.m_pValues );
//   // The negated distance of the vPoint1 from the plane 
//   float fNumerator = -(vNormal.Dot(vPoint1) + plane.GetDistance());
//   // The vNormal · vDir = cos( angle( vNormal, vDir ))
//   float fDenominator = vNormal.Dot(vDir);

//   // Rounding errors are nasty, so let's take them into account 
//   // while we make sure we don't divide by zero.
//   if ( fabs(fDenominator) <= 0.0001f )
//   {
//     // line is parallel to the plane
//     return vPoint1;
//   }
//   // The distance along vDir towards vPoint2.
//   float fDist = fNumerator / fDenominator;

//   // return the point of intersection
//   return vPoint1 + (fDist * vDir);

// }
/////////////////////////////////////////////////////////////////
int 
Phoenix::Collision::RayIntersectsSphere( const CVector3<float> &vRayStart, 
					 const CVector3<float> &vRayEnd,
					 CVector3<float> *pvIntersection0, 
					 CVector3<float> *pvIntersection1, 
					 const CSphere &Sphere)
{
  int iIntersects = 0;
  CVector3<float> vSphereToRayStart = vRayStart - Sphere.GetPosition();

  CVector3<float> vStartToEnd = vRayEnd - vRayStart;
  // Check does it intersect
  float fA = vStartToEnd.Dot(vStartToEnd);
  float fB = 2.0f * ( vStartToEnd.Dot(vSphereToRayStart));
  float fC = Sphere.GetPosition().Dot(Sphere.GetPosition()) + 
             vRayStart.Dot(vRayStart) - 
             (2.0f *(Sphere.GetPosition().Dot(vRayStart))) - 
             Sphere.GetRadiusSqr();


  float fIntersection = fB * fB - 4.0f * fA * fC;
  float fMu = 0.0f;
  // No intersection
  if ( fIntersection < -0.001f ) 
  {
    iIntersects = 0;
  } // Intersects in one point
  else if ( fIntersection >= -0.001f && fIntersection <= 0.0f)
  {
    fMu = -fB / (2.0f * fA );
    if ( pvIntersection0 != NULL ) *pvIntersection0 = vRayStart + fMu * vStartToEnd;
    if ( pvIntersection1 != NULL ) *pvIntersection1 = vRayStart + fMu * vStartToEnd;
    iIntersects = 1;
  } 
  else  // Intersects in two points
  {
    CVector3<float> vInt0, vInt1;

    fMu = (-fB - sqrt(fIntersection)) / (2.0f * fA );
    vInt0 = vRayStart + fMu * vStartToEnd;
    
    fMu = (-fB + sqrt(fIntersection)) / (2.0f * fA );
    vInt1 = vRayStart + fMu * vStartToEnd; 

    if ( pvIntersection0 != NULL ) 
    {
      *pvIntersection0 = vInt0;
    }
    if ( pvIntersection1 != NULL ) 
    {
      *pvIntersection1 = vInt1;
    }
    iIntersects = 2;

  }
  return iIntersects;
}
/////////////////////////////////////////////////////////////////
