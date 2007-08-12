#include "PhoenixMath.h"
#include "PhoenixCollision.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
using namespace Phoenix::Collision;
/////////////////////////////////////////////////////////////////
LINE_PLANE_INTERSECTION
Phoenix::Collision::LineIntersectsPlane( const CPlane &plane,
					 const CLine &line,
					 CVector3<float> &vCollisionPoint )
{
  
  float fDistanceOne = 0.0f, fDistanceTwo = 0.0f;
  CVector3<float> vNormal;
  vNormal.UseExternalData( const_cast<CPlane &>(plane).GetArray());
  
  fDistanceOne = vNormal.Dot( line.GetStart() ) + plane(3);
  fDistanceTwo = vNormal.Dot( line.GetEnd() ) + plane(3);
  
  ////////////////////
  // if other distance is positive and other negative, we have collision.
  // (which means that the endpoints are located in both sides of the plane)
  // Or other line point is in the plane. (distance == 0.0 )
  float fDistanceSqr = fDistanceOne * fDistanceTwo;
  
  if ( fDistanceSqr <= 0.0f )
  {
    CVector3<float> vDir = line.GetEnd()-line.GetStart();
    vDir.Normalize();
    float fNumerator = -(vNormal.Dot(line.GetStart()) + plane(3));
    // The vNormal · vDir = cos( angle( vNormal, vDir ))
    float fDenominator = vNormal.Dot(vDir);
    // Check that line is not in the plane completely, and if it is, we return some
    // point.
    if ( TOO_CLOSE_TO_ZERO(fDenominator ))
    {
      vCollisionPoint = line.GetStart();
      return LINE_IN_PLANE;
    }
    else
    {
      float fDist = fNumerator / fDenominator;    
      vCollisionPoint = line.GetStart() + (fDist * vDir);
    }
    return POINT_IN_PLANE;
  }
  else if ( fDistanceOne > 0.0f )
  {
    return LINE_FRONT_OF_PLANE;
  }
  else
  {
    return LINE_BEHIND_PLANE;
  }
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::RayIntersectsPlane( const CPlane &plane,
					const CRay &ray,
					CVector3<float> &vCollisionPoint )
{
  CVector3<float> vNormal;
  vNormal.UseExternalData( const_cast<CPlane &>(plane).GetArray() );
  // The negated distance of the vPoint1 from the plane 
  float fNumerator = -(vNormal.Dot(ray.GetPosition()) + plane(3));
  // The vNormal · vDir = cos( angle( vNormal, vDir ))
  float fDenominator = vNormal.Dot(ray.GetDirection());
  
  // Rounding errors are nasty, so let's take them into account 
  // while we make sure we don't divide by zero.
  if ( fabs(fDenominator) <= 0.0001f )
  {
    // line is parallel to the plane
    return 0;
  }
  // The distance along vDir towards vPoint2.
  float fDist = fNumerator / fDenominator;
  
  // return the point of intersection
  vCollisionPoint =  ray.GetPosition() + (fDist * ray.GetDirection());
  
  return 1;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Collision::LineIntersectsSphere( const CLine &line,
					 CVector3<float> *pvIntersection0, 
					 CVector3<float> *pvIntersection1, 
					 const CSphere &Sphere)
{
  int iIntersects = 0;
  CVector3<float> vSphereToRayStart = line.GetStart() - Sphere.GetPosition();
  
  CVector3<float> vStartToEnd = line.GetDirection();
  // Check does it intersect
  float fA = vStartToEnd.Dot(vStartToEnd);
  float fB = 2.0f * ( vStartToEnd.Dot(vSphereToRayStart));
  float fC = Sphere.GetPosition().Dot(Sphere.GetPosition()) + 
             line.GetStart().Dot(line.GetStart()) - 
             (2.0f *(Sphere.GetPosition().Dot(line.GetStart()))) - 
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
    if ( pvIntersection0 != NULL ) *pvIntersection0 = line.GetStart() + fMu * vStartToEnd;
    if ( pvIntersection1 != NULL ) *pvIntersection1 = line.GetStart() + fMu * vStartToEnd;
    iIntersects = 1;
  } 
  else  // Intersects in two points
  {
    CVector3<float> vInt0, vInt1;

    fMu = (-fB - sqrt(fIntersection)) / (2.0f * fA );
    vInt0 = line.GetStart() + fMu * vStartToEnd;
    
    fMu = (-fB + sqrt(fIntersection)) / (2.0f * fA );
    vInt1 = line.GetStart() + fMu * vStartToEnd; 

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
int
Phoenix::Collision::PointInsideTriangle( const CVector3<float> &vPoint, 
					 const CVector3<float> aVertices[3])
{
  CVector3<float> vVect1, vVect2;
  float fAngle = 0.0f;
  
  vVect1 = aVertices[0] - vPoint;
  vVect2 = aVertices[1] - vPoint;
  fAngle += AngleBetweenVectors( vVect1, vVect2);

  vVect1 = aVertices[1] - vPoint;
  vVect2 = aVertices[2] - vPoint;
  fAngle += AngleBetweenVectors( vVect1, vVect2);
  
  vVect1 = aVertices[2] - vPoint;
  vVect2 = aVertices[0] - vPoint;
  fAngle += AngleBetweenVectors( vVect1, vVect2);
  
  // for( unsigned int v=0;v<nNumVertices;v++)
//   {
//     vS.UseExternalData( &(pVertices[(v*3)]));
//     vT.UseExternalData( &(pVertices[((v+1)%nNumVertices)*3]));
//     vVect1 = vS - vPoint;
//     vVect2 = vT - vPoint;
//     fAngle += AngleBetweenVectors( vVect1, vVect2);
//   }
  
  return (fabs(fAngle) >= (0.99f * Math::PI * 2.0f));
  
}
/////////////////////////////////////////////////////////////////
float 
Phoenix::Collision::PointDistanceFromPlane( const CVector3<float> &vPoint, const CPlane & plane )
{
  CVector3<float> vNormal;
  vNormal.UseExternalData(const_cast<CPlane &>(plane).GetArray());
  return vNormal.Dot(vPoint) + plane(3);
}
/////////////////////////////////////////////////////////////////
CVector3<float> 
Phoenix::Collision::ClosestPointOnRay( const CVector3<float> &vPoint, const Phoenix::Math::CRay &ray )
{
  CVector3<float> vStartToPoint = vPoint - ray.GetPosition();
  float fDot = ray.GetDirection().Dot(vStartToPoint);
  return ray.GetPosition() + (ray.GetDirection() * fDot );
}
/////////////////////////////////////////////////////////////////
