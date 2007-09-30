#include "PhoenixMath.h"
#include "PhoenixCollision.h"
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
using namespace Phoenix::Collision;
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
LINE_PLANE_INTERSECTION
Phoenix::Collision::LineIntersectsPlane( const CPlane &plane,
					 const CLine &line,
					 CVector3<float> &vCollisionPoint )
{
  
  float fDistanceOne = 0.0f, fDistanceTwo = 0.0f;
  CVector3<float> vNormal;
  vNormal.UseExternalData( const_cast<CPlane &>(plane).GetArray());
  
  fDistanceOne = vNormal.Dot( line.GetStart() ) + plane[3];
  fDistanceTwo = vNormal.Dot( line.GetEnd() ) + plane[3];
  
  ////////////////////
  // if other distance is positive and other negative, we have collision.
  // (which means that the endpoints are located in both sides of the plane)
  // Or other line point is in the plane. (distance == 0.0 )
  float fDistanceSqr = fDistanceOne * fDistanceTwo;
  
  if ( fDistanceSqr <= 0.0f )
  {
    CVector3<float> vDir = line.GetEnd()-line.GetStart();
    vDir.Normalize();
    float fNumerator = -(vNormal.Dot(line.GetStart()) + plane[3]);
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
  float fNumerator = -(vNormal.Dot(ray.GetPosition()) + plane[3]);
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
  return vNormal.Dot(vPoint) + plane[3];
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
inline int AxisTestX( const COrientedBox &box,
		      const CVector3<float> &vEdge,
		      float fAbsY,
		      float fAbsZ,
		      const CVector3<float> &vOne, 
		      const CVector3<float> &vThree )
{
  float p0 =  vEdge[1]*vOne[2]   -  vEdge[2]*vOne[1];
  float p2 =  vEdge[1]*vThree[2] -  vEdge[2]*vThree[1];
  float fR =  fAbsY * box.GetHalfHeight() + fAbsZ * box.GetHalfLength();
  float fMin, fMax;

  if ( p0 < p2 )
  {
    fMin = p0;
    fMax = p2;
  }
  else
  {
    fMin = p2;
    fMax = p0;
  }
  if ( fMin > fR || fMax < -fR) return 0;
  return 1;
}
/////////////////////////////////////////////////////////////////
inline int AxisTestY( const COrientedBox &box,
		      const CVector3<float> &vEdge,
		      float fAbsX,
		      float fAbsZ,
		      const CVector3<float> &vOne, 
		      const CVector3<float> &vThree )
{
  float p0 = vEdge[2]*vOne[0]   -  vEdge[0]*vOne[2];
  float p2 = vEdge[2]*vThree[0] -  vEdge[0]*vThree[2];
  float fR = fAbsX * box.GetHalfWidth() + fAbsZ * box.GetHalfLength();
  float fMin, fMax;

  if ( p0 < p2 )
  {
    fMin = p0;
    fMax = p2;
  }
  else
  {
    fMin = p2;
    fMax = p0;
  }
  
  if ( fMin > fR || fMax < -fR) return 0;
  return 1;

}
/////////////////////////////////////////////////////////////////
inline int AxisTestZ( const COrientedBox &box,
		      const CVector3<float> &vEdge,
		      float fAbsX,
		      float fAbsY,
		      const CVector3<float> &vOne, 
		      const CVector3<float> &vThree )
{

  float p0 = vEdge[0]*vOne[1] - vEdge[1]*vOne[0];
  float p2 = vEdge[0]*vThree[1] - vEdge[1]*vThree[0]; 
  float fR = fAbsX * box.GetHalfWidth() + fAbsY * box.GetHalfHeight();
  float fMin, fMax;
  
  if ( p0 < p2 )
  {
    fMin = p0;
    fMax = p2;
  }
  else
  {
    fMin = p2;
    fMax = p0;
  }
  
  if ( fMin > fR || fMax < -fR) return 0;
  return 1;

}
/////////////////////////////////////////////////////////////////
inline void FindMinMax( float fA, float fB, float fC, float & fMin, float & fMax )
{
  if ( fA < fB )
  {
    // A < B
    if ( fB < fC  )
    {
      fMin = fA;
      fMax = fC;
    }
    // C < B
    else if ( fA < fC )
    {
      fMin = fA;
      fMax = fB;
    }
    else
    {
      fMin = fC;
      fMax = fB;
    }
  }
  else
  {
    // B < A
    if ( fC < fB )
    {
      fMin = fC;
      fMax = fA;
    }
    // B < C
    else if ( fA < fC)
    {
      fMin = fB;
      fMax = fC;
    }
    else
    {
      fMin = fB;
      fMax = fA;
    }
  }
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::TriangleIntersectsOBB( CVector3<float> vVertex0,
					   CVector3<float> vVertex1,
					   CVector3<float> vVertex2,
					   const COrientedBox &box )
{
  
  CVector3<float> vVertex0_Orig = vVertex0;
  CVector3<float> vVertex1_Orig = vVertex1;
  CVector3<float> vVertex2_Orig = vVertex2;

  // "Move" box into origo by translating and rotating vertices.
  vVertex0 -= box.GetPosition();
  vVertex1 -= box.GetPosition();
  vVertex2 -= box.GetPosition();

  using std::cerr;
  using std::endl;
  // Create rotation for oriented box.
  CMatrix4x4<float> mRotation( box.GetRightVector()[0],   box.GetUpVector()[0],  box.GetForwardVector()[0],  0,
			       box.GetRightVector()[1],   box.GetUpVector()[1],  box.GetForwardVector()[1],  0,         
			       box.GetRightVector()[2],   box.GetUpVector()[2],  box.GetForwardVector()[2],  0,
			       0,                         0,                     0,                          1 );
  // Inverse rotation; transpose is sufficient for rotation matrices.
  mRotation.Transpose();
  
  vVertex0 = Rotate( vVertex0, mRotation );
  vVertex1 = Rotate( vVertex1, mRotation );
  vVertex2 = Rotate( vVertex2, mRotation );
  
  // Calculate triangle edges.
  CVector3<float> vEdge0 = vVertex1 - vVertex0;
  CVector3<float> vEdge1 = vVertex2 - vVertex1;
  CVector3<float> vEdge2 = vVertex0 - vVertex2;
  
  ////////////////////
  // These have been slightly optimized by calculating absolute values only once.
  float fAx = fabsf( vEdge0[0] );
  float fAy = fabsf( vEdge0[2] );
  float fAz = fabsf( vEdge0[1] );
  if ( !AxisTestX( box, vEdge0, fAy, fAz, vVertex0, vVertex2) )  return 0; 
  if ( !AxisTestY( box, vEdge0, fAy, fAx, vVertex0, vVertex2) )  return 0;
  if ( !AxisTestZ( box, vEdge0, fAz, fAx, vVertex0, vVertex2) )  return 0;
  
  fAx = fabsf( vEdge1[0] );
  fAy = fabsf( vEdge1[2] );
  fAz = fabsf( vEdge1[1] );
  if ( !AxisTestX( box, vEdge1, fAy, fAz, vVertex0, vVertex2) )  return 0;  
  if ( !AxisTestY( box, vEdge1, fAy, fAx, vVertex0, vVertex2) )  return 0;
  if ( !AxisTestZ( box, vEdge1, fAz, fAx, vVertex0, vVertex2) )  return 0;

  fAx = fabsf( vEdge2[0] );
  fAy = fabsf( vEdge2[2] );
  fAz = fabsf( vEdge2[1] );
  if ( !AxisTestX( box, vEdge2, fAy, fAz, vVertex1, vVertex2) )  return 0; 
  if ( !AxisTestY( box, vEdge2, fAy, fAx, vVertex1, vVertex2) )  return 0;
  if ( !AxisTestZ( box, vEdge2, fAz, fAx, vVertex1, vVertex2) )  return 0;

  
  float fMin, fMax;
  // Test X-direction triangle AABB bs box
  FindMinMax( vVertex0[0], vVertex1[0], vVertex2[0], fMin, fMax);
  if ( fMin > box.GetHalfWidth() || fMax < -box.GetHalfWidth())  return 0;

  // Test Y-direction triangle AABB vs box
  FindMinMax( vVertex0[1], vVertex1[1], vVertex2[1], fMin, fMax);
  if ( fMin > box.GetHalfHeight() || fMax < -box.GetHalfHeight())  return 0;

  // Test Z-direction triangle AABB vs box
  FindMinMax( vVertex0[2], vVertex1[2], vVertex2[2], fMin, fMax);
  if ( fMin > box.GetHalfLength() || fMax < -box.GetHalfLength())  return 0;

  ////////////////////
  // There might be some advantages in using AABB for PlaneIntersectsBox,
  // try this when you try to optimize further.
  ////////////////////
  // CAxisAlignedBox aabb;
  //   aabb.SetPosition(0,0,0);
  //   aabb.SetWidth( box.GetWidth());
  //   aabb.SetHeight( box.GetHeight());
  //   aabb.SetLength( box.GetLength());
  ////////////////////

  // Check if box intersects triangle plane, if not -> fail
  CPlane plane;
  vVertex1 = (vVertex1_Orig - vVertex0_Orig).Cross(vVertex2_Orig-vVertex1_Orig);
  plane.Calculate( vVertex1, vVertex0_Orig);
  if ( !PlaneIntersectsBox( plane, box )) 
  {
    return 0;
  }
  
  return 1;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::PlaneIntersectsBox( const CPlane &plane,
					const COrientedBox &box )
{
  /// Calculate |distance| from box center to plane
  float fDistance = fabsf(PointDistanceFromPlane( box.GetPosition(), plane));

  // Using separate axis theorem (SAT), perform checking.
  CVector3<float> vNormal;
  vNormal.UseExternalData(const_cast<CPlane &>(plane).GetArray());

  float fValue;
  float fTmp = fabsf(box.GetHalfLength()*(vNormal.Dot(box.GetForwardVector())));
  fValue = fTmp;

  fTmp = fabsf(box.GetHalfWidth()*(vNormal.Dot(box.GetRightVector())));
  fValue += fTmp;

  fTmp = fabsf(box.GetHalfHeight()*(vNormal.Dot(box.GetUpVector())));
  fValue += fTmp;

  return ( fDistance <= fValue);
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::PlaneIntersectsBox( const CPlane &plane,
					const CAxisAlignedBox &box )
{
  /// Calculate |distance| from box center to plane
  float fDistance = fabsf(PointDistanceFromPlane( box.GetPosition(), plane));

  // Using separate axis theorem (SAT), perform checking.
  CVector3<float> vNormal;
  vNormal.UseExternalData(const_cast<CPlane &>(plane).GetArray());

  float fValue;
  float fTmp = fabsf(box.GetHalfLength()*vNormal[2]);
  fValue = fTmp;

  fTmp = fabsf(box.GetHalfWidth()*vNormal[1]);
  fValue += fTmp;

  fTmp = fabsf(box.GetHalfHeight()*vNormal[0]);
  fValue += fTmp;

  return ( fDistance <= fValue);
}
/////////////////////////////////////////////////////////////////
int   
Phoenix::Collision::PointIntersectsOBB( const CVector3<float> &vPoint,
					const Phoenix::Volume::COrientedBox &obBox )
{
  // calculate vector from box center to point, and take scalar projection
  // for every box axis. If absolute value of all projections are smaller than 
  // respective box dimensions, point is inside.
  CVector3<float> vPos = vPoint - obBox.GetPosition();
  if ( fabs(vPos.Dot(obBox.GetForwardVector())) > obBox.GetHalfLength()) return 0;
  if ( fabs(vPos.Dot(obBox.GetRightVector())) > obBox.GetHalfWidth()) return 0;
  if ( fabs(vPos.Dot(obBox.GetUpVector())) > obBox.GetHalfHeight()) return 0;
  return 1;
}
/////////////////////////////////////////////////////////////////
int  
Phoenix::Collision::CollisionPoint3Planes( const Phoenix::Math::CPlane & plane1,
					   const Phoenix::Math::CPlane & plane2,
					   const Phoenix::Math::CPlane & plane3,
					   Phoenix::Math::CVector3<float> &vCollisionPoint)
{
  CMatrix3x3<float> mMatrix( plane1[0], plane1[1], plane1[2], 
			     plane2[0], plane2[1], plane2[2], 
			     plane3[0], plane3[1], plane3[2] );
  
  CMatrix3x3<float> mInverse;
  CVector3<float> vVect(-plane1[3], -plane2[3], -plane3[3]);
  
  if ( InverseMatrix( mMatrix, mInverse ) )
  {
    return 1;
  }
  
  vCollisionPoint = mInverse * vVect;

  return 0;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Collision::SphereIntersectsPlane( const Phoenix::Math::CPlane &plane, 
					   const Phoenix::Volume::CSphere &sphere, 
					   float &fCenterDistance)
{

  CVector3<float> vNormal;
  vNormal.UseExternalData(const_cast<Phoenix::Math::CPlane &>(plane).GetArray());
  float fDistance = vNormal.Dot(sphere.GetPosition()) + plane[3];
  fCenterDistance = fDistance;
  // Check does sphere intersect
  if ( fabs(fDistance) <= sphere.GetRadius())
  {
    return 0;
  } // if not, check is it on positive half-side 
  else if ( fDistance > sphere.GetRadius())
  {
    return 1;
  }
  // It is on negative half-side.
  return -1;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::SphereIntersectsAACube( const CSphere &sphere, 
					    const CAxisAlignedCube &aaBox)
{
  float fDistance = 0.0f;
  float fTmp = 0.0f;
  /////////////////////////////////////////////////////////////////
  // Test for X axis
  float fAxisMax = aaBox.GetPosition()[0] + aaBox.GetHalfWidth();
  float fAxisMin = aaBox.GetPosition()[0] - aaBox.GetHalfWidth();
  float fAxisCenter = sphere.GetPosition()[0];

  if ( fAxisCenter < fAxisMin )
  {
    fTmp = fAxisCenter - fAxisMin;
    fDistance += ( fTmp * fTmp );
  } 
  else if ( fAxisCenter > fAxisMax )
  {
    fTmp =  fAxisCenter - fAxisMax;
    fDistance += ( fTmp * fTmp );
  }
  /////////////////////////////////////////////////////////////////
  // Test for Y axis
  fAxisMax = aaBox.GetPosition()[1] + aaBox.GetHalfWidth();
  fAxisMin = aaBox.GetPosition()[1] - aaBox.GetHalfWidth();
  fAxisCenter = sphere.GetPosition()[1];
  if ( fAxisCenter < fAxisMin )
  {
    fTmp = fAxisCenter - fAxisMin;
    fDistance += ( fTmp * fTmp );
  } 
  else if ( fAxisCenter > fAxisMax )
  {
    fTmp =  fAxisCenter - fAxisMax;
    fDistance += ( fTmp * fTmp );
  }
  /////////////////////////////////////////////////////////////////
  // Test for Z axis
  fAxisMax = aaBox.GetPosition()[2] + aaBox.GetHalfWidth();
  fAxisMin = aaBox.GetPosition()[2] - aaBox.GetHalfWidth();
  fAxisCenter = sphere.GetPosition()[2];
  if ( fAxisCenter < fAxisMin )
  {
    fTmp = fAxisCenter - fAxisMin;
    fDistance += ( fTmp * fTmp );
  } 
  else if ( fAxisCenter > fAxisMax )
  {
    fTmp =  fAxisCenter - fAxisMax;
    fDistance += ( fTmp * fTmp );
  }
  /////////////////////////////////////////////////////////////////
  // Actual test, if the sum of squared distances is less than the squared
  // radius, we have overlapping.
  if ( fDistance > sphere.GetRadiusSqr()) 
  {
    return 0;
  }
  else 
  {
    return 1;
  }

}
/////////////////////////////////////////////////////////////////
