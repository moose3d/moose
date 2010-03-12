#include "PhoenixMath.h"
#include "PhoenixCollision.h"
#include "PhoenixIndexArray.h"
#include "PhoenixVertexDescriptor.h"
#include <iostream>
#include <assert.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
using namespace Phoenix::Collision;
using namespace Phoenix::Graphics;
using namespace Phoenix::Spatial;
using std::cerr;
using std::endl;
/////////////////////////////////////////////////////////////////
#define PLANE_DOT_POS( PLANE, POS ) (PLANE[0]*POS[0] + PLANE[1]*POS[1] + PLANE[2]*POS[2] + PLANE[3])
/////////////////////////////////////////////////////////////////
LINESEGM_PLANE_INTERSECTION
Phoenix::Collision::LineSegmentIntersectsPlane( const CPlane &plane,
						const CLineSegment &line,
						CVector3<float> &vCollisionPoint )
{

	float fDistanceOne = 0.0f, fDistanceTwo = 0.0f;
  CVector3<float> vNormal;
  vNormal.Set( const_cast<CPlane &>(plane).GetArray());

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
  vNormal.Set( const_cast<CPlane &>(plane).GetArray() );
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
Phoenix::Collision::RayIntersectsPlane( const CPlane &plane,
					const CRay &ray,
					float *pfDistance)
{
  CVector3<float> vNormal;
  vNormal.Set( const_cast<CPlane &>(plane).GetArray() );
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
  // return the point of intersection
  if ( pfDistance )
  {
  	// The distance along vDir towards vPoint2.
  	*pfDistance = fNumerator / fDenominator;
	}

  return 1;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::RayIntersectsSphere( const CRay &ray,
					 CVector3<float> *pvIntersection0,
					 CVector3<float> *pvIntersection1,
					 const CSphere &Sphere)
{

  int iIntersects = 0;
  CVector3<float> vL = Sphere.GetPosition() - ray.GetPosition();
  float fS = vL.Dot(ray.GetDirection());
  float fLsqr = vL.Dot(vL);
  float fRsqr = Sphere.GetRadiusSqr();

  if ( fS < -EPSILON && fLsqr > fRsqr)
  {
    return 0;
  }

  float fMsqr = fLsqr - fS*fS;

  if ( fMsqr > fRsqr ) return 0;

  // are there one or two intersection points
  float fQ = sqrtf( fRsqr - fMsqr);
  iIntersects =  TOO_CLOSE_TO_ZERO(fQ) ? 1 : 2;

  // calculate intersections only when needed.
  if ( pvIntersection0 != NULL || pvIntersection1 != NULL )
  {
    float fQ = sqrtf( fRsqr - fMsqr);

    if ( fLsqr > fRsqr && iIntersects == 2 )
    {
      float fTfirst = fS - fQ;
      float fTsecond = fS + fQ;
      iIntersects = 2;
      if ( pvIntersection0 != NULL )
      {
      	*pvIntersection0 = ray.GetPosition()+fTfirst*ray.GetDirection();
      }
      if ( pvIntersection1 != NULL )
      {
      	*pvIntersection1 = ray.GetPosition()+fTsecond*ray.GetDirection();
      }

    }
    else
    {
      float fT = fS + fQ;
      CVector3<float> vPoint = ray.GetPosition()+fT*ray.GetDirection();
      if ( pvIntersection0 != NULL )
      {
	*pvIntersection0 = vPoint;
      }
      if ( pvIntersection1 != NULL )
      {
	*pvIntersection1 = vPoint;
      }
    }

  }
  return iIntersects;
  //   int iIntersects = 0;
//   CVector3<float> vSphereToRayStart = ray.GetPosition() - Sphere.GetPosition();

//   // Check does it intersect
//   float fA = ray.GetDirection().Dot(ray.GetDirection());
//   float fB = 2.0f * ( ray.GetDirection().Dot(vSphereToRayStart));
//   float fC = Sphere.GetPosition().Dot(Sphere.GetPosition()) +
//              line.GetStart().Dot(line.GetStart()) -
//              (2.0f *(Sphere.GetPosition().Dot(line.GetStart()))) -
//              Sphere.GetRadiusSqr();


//   float fIntersection = fB * fB - 4.0f * fA * fC;
//   float fMu = 0.0f;
//   // No intersection
//   if ( fIntersection < -0.001f )
//   {
//     iIntersects = 0;
//   } // Intersects in one point
//   else if ( fIntersection >= -0.001f && fIntersection <= 0.0f)
//   {
//     fMu = -fB / (2.0f * fA );
//     if ( pvIntersection0 != NULL ) *pvIntersection0 = line.GetStart() + fMu * ray.GetDirection();
//     if ( pvIntersection1 != NULL ) *pvIntersection1 = line.GetStart() + fMu * ray.GetDirection();
//     iIntersects = 1;
//   }
//   else  // Intersects in two points
//   {
//     CVector3<float> vInt0, vInt1;

//     fMu = (-fB - sqrt(fIntersection)) / (2.0f * fA );
//     vInt0 = line.GetStart() + fMu * ray.GetDirection();

//     fMu = (-fB + sqrt(fIntersection)) / (2.0f * fA );
//     vInt1 = line.GetStart() + fMu * ray.GetDirection();

//     if ( pvIntersection0 != NULL )
//     {
//       *pvIntersection0 = vInt0;
//     }
//     if ( pvIntersection1 != NULL )
//     {
//       *pvIntersection1 = vInt1;
//     }
//     iIntersects = 2;

//   }
//   return iIntersects;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::RayIntersectsSphere( const CRay &ray,
					 float *pfIntersection0,
					 float *pfIntersection1,
					 const CSphere &Sphere)
{

  int iIntersects = 0;
  CVector3<float> vL = Sphere.GetPosition() - ray.GetPosition();
  float fS = vL.Dot(ray.GetDirection());
  float fLsqr = vL.Dot(vL);
  float fRsqr = Sphere.GetRadiusSqr();

  if ( fS < -EPSILON && fLsqr > fRsqr)
  {
    return 0;
  }

  float fMsqr = fLsqr - fS*fS;

  if ( fMsqr > fRsqr ) return 0;

  // are there one or two intersection points
  float fQ = sqrtf( fRsqr - fMsqr);
  iIntersects =  TOO_CLOSE_TO_ZERO(fQ) ? 1 : 2;

  // calculate intersections only when needed.
  if ( pfIntersection0 != NULL || pfIntersection1 != NULL )
  {
    float fQ = sqrtf( fRsqr - fMsqr);

    if ( fLsqr > fRsqr && iIntersects == 2 )
    {
      float fTfirst = fS - fQ;
      float fTsecond = fS + fQ;
      iIntersects = 2;
      if ( pfIntersection0 != NULL )
      {
      	*pfIntersection0 = fTfirst;
      }
      if ( pfIntersection1 != NULL )
      {
      	*pfIntersection1 = fTsecond;
      }

    }
    else
    {
      float fT = fS + fQ;

      if ( pfIntersection0 != NULL )
      {
      	*pfIntersection0 = fT;
      }
      if ( pfIntersection1 != NULL )
      {
      	*pfIntersection1 = fT;
      }
    }

  }
  return iIntersects;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::PointInsideTriangle( const CVector3<float> & vPoint, const CVector3<float> & vVertex0,
					 const CVector3<float> & vVertex1,
					 const CVector3<float> & vVertex2 )
{
  CVector3<float> vVect1, vVect2;
  float fAngle = 0.0f;
  if ( vPoint == vVertex0) return true;
  if ( vPoint == vVertex1) return true;
  if ( vPoint == vVertex2) return true;
  vVect1 = vVertex0 - vPoint;
  vVect2 = vVertex1 - vPoint;
  fAngle += AngleBetweenVectors( vVect1, vVect2);

  vVect1 = vVertex1 - vPoint;
  vVect2 = vVertex2 - vPoint;
  fAngle += AngleBetweenVectors( vVect1, vVect2);

  vVect1 = vVertex2 - vPoint;
  vVect2 = vVertex0 - vPoint;
  fAngle += AngleBetweenVectors( vVect1, vVect2);

  return (fabs(fAngle) >= (0.99f * Math::PImul2));
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Collision::LineSegmentIntersectsTriangle( const Phoenix::Math::CLineSegment & line,
						   const Phoenix::Math::CVector3<float> & vVertex0,
						   const Phoenix::Math::CVector3<float> & vVertex1,
						   const Phoenix::Math::CVector3<float> & vVertex2,
						   Phoenix::Math::CVector3<float> &vPointOfIntersection )
{

  CPlane triPlane;
  //CVector3<float> vPoint;
  // Calculate triangle plane and check does it intersect the plane.
  triPlane.Calculate( (vVertex1-vVertex0).Cross( vVertex2-vVertex0), vVertex0);
  LINESEGM_PLANE_INTERSECTION tType = LineSegmentIntersectsPlane( triPlane, line, vPointOfIntersection );
  if ( tType == POINT_IN_PLANE)
  {
    // if line intersects plane, then proceed to test triangle with interesection point.
    return PointInsideTriangle( vPointOfIntersection, vVertex0, vVertex1, vVertex2 );
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::RayIntersectsTriangle( const Phoenix::Math::CRay & ray,
					   const Phoenix::Math::CVector3<float> & vVertex0,
					   const Phoenix::Math::CVector3<float> & vVertex1,
					   const Phoenix::Math::CVector3<float> & vVertex2,
					   float *pfDistance )
{
  CPlane triPlane;
  // Calculate triangle plane and check does it intersect the plane.
  triPlane.Calculate( (vVertex1-vVertex0).Cross( vVertex2-vVertex0), vVertex0);

  if ( RayIntersectsPlane( triPlane, ray, pfDistance) )
  {
  	CVector3<float> vPointOfIntersection = ray.GetPosition() + (*pfDistance * ray.GetDirection());
  	return PointInsideTriangle( vPointOfIntersection, vVertex0, vVertex1, vVertex2 );
  }
  return false;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::RayIntersectsTriangle( const Phoenix::Math::CRay & ray,
					   const Phoenix::Math::CVector3<float> & vVertex0,
					   const Phoenix::Math::CVector3<float> & vVertex1,
					   const Phoenix::Math::CVector3<float> & vVertex2,
					   Phoenix::Math::CVector3<float> &vPointOfIntersection )
{
  CPlane triPlane;
  // Calculate triangle plane and check does it intersect the plane.
  triPlane.Calculate( (vVertex1-vVertex0).Cross( vVertex2-vVertex0), vVertex0);


  if ( RayIntersectsPlane( triPlane, ray, vPointOfIntersection) )
  {
    return PointInsideTriangle( vPointOfIntersection, vVertex0, vVertex1, vVertex2 );
  }
  return false;
}
/////////////////////////////////////////////////////////////////
float
Phoenix::Collision::LineSegmentToLineSegmentDistanceSquared( const Phoenix::Math::CLineSegment & line0, const Phoenix::Math::CLineSegment & line1 )
{
  // Direction is calculated from end points.
  CVector3<float> vDir0 = line0.GetDirection();
  CVector3<float> vDir1 = line1.GetDirection();

  float fV0DotV1      = vDir0.Dot( vDir1 );
  float fV0DotV0      = vDir0.Dot( vDir0 );
  float fV1DotV1      = vDir1.Dot( vDir1 );

  float fDenominator = (fV0DotV1*fV0DotV1) - (fV0DotV0*fV1DotV1);

  // if lines are parallel (enough)
  if ( TOO_CLOSE_TO_ZERO(fDenominator) )
  {
    // Check distances between each start and end
    float fDistSqr ;
    CVector3<float> v00 = line1.GetStart() - line0.GetStart();
    CVector3<float> v10 = line1.GetStart() - line0.GetEnd();
    CVector3<float> v01 = line1.GetEnd() - line0.GetStart();
    CVector3<float> v11 = line1.GetEnd() - line0.GetEnd();


    float f00sign = v00.Dot(vDir0);
    float f10sign = v10.Dot(vDir0);
    float f01sign = v01.Dot(vDir0);
    float f11sign = v11.Dot(vDir0);

    if ( (f00sign < 0.0f && f10sign < 0.0f && f01sign < 0.0f && f11sign < 0.0f) ||
	 (f00sign > 0.0f && f10sign > 0.0f && f01sign > 0.0f && f11sign > 0.0f) )
    {
      // lines do not overlap on direction vector, minimal distance is
      // mnin distance between start and end point combinations.
      fDistSqr = v00.LengthSqr();
      float fTmp = v10.LengthSqr();
      if ( fTmp < fDistSqr ) fDistSqr = fTmp;

      fTmp = v01.LengthSqr();
      if ( fTmp < fDistSqr ) fDistSqr = fTmp;

      fTmp = v11.LengthSqr();
      if ( fTmp < fDistSqr ) fDistSqr = fTmp;
    }
    else
    {
      vDir0.Normalize();
      f00sign = v00.Dot(vDir0);

      // lines overlap on direction vector, closest point on ray will do
      // vClosestPoint = ray.GetPosition() + (ray.GetDirection() * (ray.GetDirection().Dot(vPoint - ray.GetPosition())) );
      fDistSqr = (line1.GetStart() - (line0.GetStart()+(vDir0*f00sign))).LengthSqr();
    }
    return fDistSqr;
  }
  else
  {
    // skewed lines, calculate distance square
    float fMult = 1.0f / fDenominator;
    CVector3<float> startDiff = line1.GetStart() - line0.GetStart();
    float fStartDiffDotV0 = startDiff.Dot( vDir0 );
    float fStartDiffDotV1 = startDiff.Dot( vDir1 );

    float fT0 = fMult * (-fV1DotV1 * fStartDiffDotV0 + fV0DotV1*fStartDiffDotV1);
    float fT1 = fMult * (-fV0DotV1 * fStartDiffDotV0 + fV0DotV0*fStartDiffDotV1);

    return ( (line0.GetStart() + fT0*vDir0) -
	     (line1.GetStart() + fT1*vDir1)   ).LengthSqr();
  }
}
/////////////////////////////////////////////////////////////////
float
Phoenix::Collision::PointDistanceFromPlane( const CVector3<float> &vPoint, const CPlane & plane )
{
  CVector3<float> vNormal;
  vNormal.Set(const_cast<CPlane &>(plane).GetArray());
  return vNormal.Dot(vPoint) + plane[3];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Collision::ClosestPointOnRay( const CVector3<float> &vPoint, const Phoenix::Math::CRay &ray, CVector3<float> & vClosestPoint )
{
  //CVector3<float> vStartToPoint = vPoint - ray.GetPosition();
  //vClosestPoint = ray.GetPosition() + (ray.GetDirection() * fDot );
  float fDot = ray.GetDirection().Dot(vPoint - ray.GetPosition());
  if ( fDot > 0.0f )
    vClosestPoint = ray.GetPosition() + (ray.GetDirection() * fDot );
  else
    vClosestPoint = ray.GetPosition();
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Collision::ClosestPointOnLine( const CVector3<float> &vPoint, const Phoenix::Math::CLine &line, CVector3<float> & vClosestPoint )
{
  vClosestPoint = line.GetPosition() + (line.GetDirection() * (line.GetDirection().Dot(vPoint - line.GetPosition())) );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Collision::ClosestPointOnLineSegment( const CVector3<float> &vPoint, const Phoenix::Math::CLineSegment & line, CVector3<float> & vClosestPoint )
{

  CVector3<float> vStartToPoint = vPoint - line.GetStart();
  CVector3<float> vLine = line.GetEnd()-line.GetStart();
  float fLength = vLine.Length();
  float fDot = vStartToPoint.Dot(line.GetDirection());

  if      ( fDot <= 0.0f    ) { vClosestPoint = line.GetStart();  }               // return the starting point
  else if ( fDot >= fLength ) { vClosestPoint = line.GetEnd();    }               // return the end point
  else                        { vClosestPoint = line.GetStart() + (line.GetDirection()*fDot); } // return the point in the middle
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

  Rotate( CVector3<float>(vVertex0), mRotation, vVertex0 );
  Rotate( CVector3<float>(vVertex1), mRotation, vVertex1 );
  Rotate( CVector3<float>(vVertex2), mRotation, vVertex2 );

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
  vNormal.Set(const_cast<CPlane &>(plane).GetArray());

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
  vNormal.Set(const_cast<CPlane &>(plane).GetArray());

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
  vNormal.Set(const_cast<Phoenix::Math::CPlane &>(plane).GetArray());
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
int
Phoenix::Collision::SphereIntersectsSphere( const Phoenix::Volume::CSphere &sphereOne, const Phoenix::Volume::CSphere &sphereTwo )
{
  // Get the separating axis
  CVector3<float> vSepAxis = sphereOne.GetPosition() - sphereTwo.GetPosition();

  // Get the sum of the radii
  float fSumOfRadii = sphereOne.GetRadius() + sphereTwo.GetRadius();
  fSumOfRadii = fSumOfRadii * fSumOfRadii;
  // if the distance between the centers is less than the sum
  // of the radii, then we have an intersection
  return (vSepAxis.LengthSqr() < fSumOfRadii);
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::SphereIntersectsCapsule( const Phoenix::Volume::CSphere & sphere, const Phoenix::Volume::CCapsule & capsule )
{
  // Calculate closest point to sphere center on line segment of capsule
  CVector3<float> vPointOnSegment;
  ClosestPointOnLineSegment( sphere.GetPosition(), capsule, vPointOnSegment);
  // get sum from squared radius of capsule and sphere
  float fSumOfRadiiSqr = sphere.GetRadius() + capsule.GetRadius();
  fSumOfRadiiSqr = fSumOfRadiiSqr * fSumOfRadiiSqr;
  // compare it to squared distance from line segment point. if under sum of radii, we're intersecting.
  return ((vPointOnSegment-sphere.GetPosition()).LengthSqr() < fSumOfRadiiSqr);
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::OBBIntersectsCapsule( const Phoenix::Volume::COrientedBox & box, const Phoenix::Volume::CCapsule & capsule )
{
  CVector3<float> vPointOnSegment;
  ClosestPointOnLineSegment( box.GetPosition(), capsule, vPointOnSegment );
  CVector3<float> vSegmentToBox = box.GetPosition()-vPointOnSegment;
  float fDistanceSqr = vSegmentToBox.LengthSqr();
  vSegmentToBox.Normalize();
  float fEffRadius = 0.5f * ( fabsf((box.GetForwardVector()*box.GetLength()).Dot(vSegmentToBox)) +
			      fabsf((box.GetRightVector()*box.GetWidth()).Dot(vSegmentToBox)) +
			      fabsf((box.GetUpVector()*box.GetHeight()).Dot(vSegmentToBox)) );

  return ( fDistanceSqr < (capsule.GetRadiusSqr() + (fEffRadius*fEffRadius)) );
}
/////////////////////////////////////////////////////////////////
Phoenix::Collision::S2S_COLLISION_TYPE
Phoenix::Collision::SphereIntersectsSphere( const Phoenix::Volume::CSphere &sphereOne,
					    const Phoenix::Math::CVector3<float> &vVelocityOne,
					    const Phoenix::Volume::CSphere &sphereTwo,
					    const Phoenix::Math::CVector3<float> &vVelocityTwo,
					    Phoenix::Math::CVector3<float> & vIntersectionPoint,
					    float &fRelativeTime )
{
  CVector3<float> vA = sphereOne.GetPosition() - sphereTwo.GetPosition();
  CVector3<float> vB = vVelocityOne - vVelocityTwo;

  float fASqr = vA.Dot(vA);
  float fBSqr = vB.Dot(vB);

  float fRadiusSumSqr = sphereOne.GetRadius() + sphereTwo.GetRadius();
  fRadiusSumSqr *= fRadiusSumSqr;

  // Check are sphers colliding already
  if ( fASqr < fRadiusSumSqr )
  {
    return S2S_ALREADY;
  }
  // Check are velocities equal ( moving into same direction )
  if ( TOO_CLOSE_TO_ZERO(fBSqr ))
  {
    return S2S_NEVER;
  }

  float fAdotB = vA.Dot(vB);
  float fAdotBSqr = fAdotB * fAdotB;

  float fDSqr = fASqr - fAdotBSqr / fBSqr;
  // Check will they ever intersect?
  if ( fDSqr > fRadiusSumSqr )
  {
    return S2S_NEVER;
  }

  // Calculate intersection point and relative time.
  fRelativeTime = (-fAdotB - sqrtf(fAdotBSqr - fBSqr *(fASqr - fRadiusSumSqr))) / fBSqr;

  CVector3<float> vPosOne, vPosTwo, vNormal;

  vPosOne = sphereOne.GetPosition() + (fRelativeTime * vVelocityOne);
  vPosTwo = sphereTwo.GetPosition() + (fRelativeTime * vVelocityTwo);
  vNormal = vPosTwo - vPosOne;
  vNormal.Normalize();
  vIntersectionPoint = vPosOne + sphereOne.GetRadius() * vNormal;

  return S2S_SOON;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Collision::CalculateDecalMesh( const CDecalVolume & decalVolume,
					const Phoenix::Graphics::CVertexDescriptor & vertices,
					const Phoenix::Graphics::CIndexArray & indices,
					std::list< std::list< Phoenix::Math::CVector3<float> > > & lstTriangleFans )
{
  assert( indices.GetPrimitiveType() == PRIMITIVE_TRI_LIST );
  CVector3<float> vPoint0, vPoint1, vPoint2, vTriNormal;


  if ( indices.IsShortIndices())
  {
    unsigned short int index0,index1,index2;
    // for each triangle
    for( size_t i=0;i<indices.GetNumIndices();i+=3)
    {
      // retrieve actual vertex coordinates
      index0 = indices.GetPointer<unsigned short int>()[i];
      index1 = indices.GetPointer<unsigned short int>()[i+1];
      index2 = indices.GetPointer<unsigned short int>()[i+2];

      vPoint0.Set( &(vertices.GetPointer<float>()[index0*3]) );
      vPoint1.Set( &(vertices.GetPointer<float>()[index1*3]) );
      vPoint2.Set( &(vertices.GetPointer<float>()[index2*3]) );

      std::list<Phoenix::Math::CPlane>::iterator it;
      // Check that triangle normal points same direction as decal volume's.
      if ( ((vPoint1-vPoint0).Cross(vPoint2-vPoint0)).Dot(decalVolume.GetNormalVector()) > 0.0f )
      {
	std::list< CVector3<float> > lstVertices;

	lstVertices.push_back( vPoint0 );
	lstVertices.push_back( vPoint1 );
	lstVertices.push_back( vPoint2 );

	lstTriangleFans.push_back( lstVertices );

      }

    } // for( size_t ...

    // Do actual clipping against planes.
    std::list< std::list< Phoenix::Math::CVector3<float> > >::iterator fan_iterator;


    for( fan_iterator  = lstTriangleFans.begin();
	 fan_iterator != lstTriangleFans.end();
	 fan_iterator++)
    {
      std::vector<Phoenix::Math::CPlane>::const_iterator plane_it;
      for( plane_it = const_cast<CDecalVolume &>(decalVolume).GetPlanes().begin();
	   plane_it != const_cast<CDecalVolume &>(decalVolume).GetPlanes().end();
	   plane_it++)
      {
	ClipPolygon( *plane_it, *fan_iterator );
      }
    }

  }
  else
  {
    assert( 0 && "NOt short indices." );
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Collision::CalculateDecalMesh( const CDecalVolume & decalVolume,
					const Phoenix::Graphics::CVertexDescriptor & vertices,
					const Phoenix::Graphics::CVertexDescriptor & normals,
					const Phoenix::Graphics::CIndexArray & indices,
					std::list< std::list< Phoenix::Spatial::CVertex > > & lstTriangleFans )
{
  assert( indices.GetPrimitiveType() == PRIMITIVE_TRI_LIST );

  CVector3<float> vTriNormal;
  CVertex vPoint0, vPoint1, vPoint2;
  float *pTmp = NULL;
  if ( indices.IsShortIndices())
  {
    unsigned short int index0,index1,index2;
    // for each triangle
    for( size_t i=0;i<indices.GetNumIndices();i+=3)
    {
      // retrieve actual vertex coordinates
      index0 = indices.GetPointer<unsigned short int>()[i];
      index1 = indices.GetPointer<unsigned short int>()[i+1];
      index2 = indices.GetPointer<unsigned short int>()[i+2];
      if ( vertices.GetType() == ELEMENT_TYPE_V3F_N3F_T2F )
      {
	pTmp = vertices.GetPointer<float>(index0);           vPoint0.SetPosition( pTmp[0], pTmp[1], pTmp[2] );
	pTmp = &(vertices.GetPointer<float>(index0)[3]);      vPoint0.SetNormal( pTmp[0], pTmp[1], pTmp[2] );

	pTmp = vertices.GetPointer<float>(index1);           vPoint1.SetPosition( pTmp[0], pTmp[1], pTmp[2] );
	pTmp = &(vertices.GetPointer<float>(index1)[3]);      vPoint1.SetNormal( pTmp[0], pTmp[1], pTmp[2] );

	pTmp = vertices.GetPointer<float>(index2);           vPoint2.SetPosition( pTmp[0], pTmp[1], pTmp[2] );
	pTmp = &(vertices.GetPointer<float>(index2)[3]);      vPoint2.SetNormal( pTmp[0], pTmp[1], pTmp[2] );
      }
      else 
      {
	pTmp = vertices.GetPointer<float>(index0);     vPoint0.SetPosition( pTmp[0], pTmp[1], pTmp[2] );
	pTmp = normals.GetPointer<float>(index0);      vPoint0.SetNormal( pTmp[0], pTmp[1], pTmp[2] );

	pTmp = vertices.GetPointer<float>(index1);     vPoint1.SetPosition( pTmp[0], pTmp[1], pTmp[2] );
	pTmp = normals.GetPointer<float>(index1);      vPoint1.SetNormal( pTmp[0], pTmp[1], pTmp[2] );

	pTmp = vertices.GetPointer<float>(index2);     vPoint2.SetPosition( pTmp[0], pTmp[1], pTmp[2] );
	pTmp = normals.GetPointer<float>(index2);      vPoint2.SetNormal( pTmp[0], pTmp[1], pTmp[2] );
      }
      std::list<Phoenix::Math::CPlane>::iterator it;
      // Check that triangle normal points same direction as decal volume's.
      if ( ((vPoint1.GetPosition()-vPoint0.GetPosition()).
	    Cross(vPoint2.GetPosition()-vPoint0.GetPosition())).
	   Dot(decalVolume.GetNormalVector()) > 0.0f )
      {
	std::list< CVertex > lstVertices;

	lstVertices.push_back( vPoint0 );
	lstVertices.push_back( vPoint1 );
	lstVertices.push_back( vPoint2 );

	lstTriangleFans.push_back( lstVertices );
      }

    } // for( size_t ...

    // Do actual clipping against planes.
    std::list< std::list< Phoenix::Spatial::CVertex > >::iterator fan_iterator;
    std::list< Phoenix::Spatial::CVertex >::iterator v_iterator;


    for( fan_iterator  = lstTriangleFans.begin();
	 fan_iterator != lstTriangleFans.end();
	 fan_iterator++ )
    {

      std::vector<Phoenix::Math::CPlane>::const_iterator plane_it;
      for( plane_it  = const_cast<CDecalVolume &>(decalVolume).GetPlanes().begin();
	   plane_it != const_cast<CDecalVolume &>(decalVolume).GetPlanes().end();
	   plane_it++)
      {
	ClipPolygon( *plane_it, *fan_iterator );
	v_iterator = (*fan_iterator).begin();

	for( ; v_iterator != (*fan_iterator).end(); v_iterator++ )
	{

	  float fAlpha = ( decalVolume.GetNormalVector().Dot((*v_iterator).GetNormal()));
	  (*v_iterator).GetColor()[3] = (unsigned char)(255 * fAlpha);
	  CVector3<float> vDecalToVertex = (*v_iterator).GetPosition() - decalVolume.GetPosition();
	  float fS = (decalVolume.GetTangentVector().Dot( vDecalToVertex  ) / decalVolume.GetWidth()) + 0.5f;
	  float fT = (decalVolume.GetBitangentVector().Dot( vDecalToVertex ) / decalVolume.GetHeight()) + 0.5f;
	  (*v_iterator).SetTextureCoordinates( fS, fT );

	} /// ...v_iterator
      } /// ...plane_iteartor


    } /// fan_iterator...

    /// Remove empty fan lists from vector
    fan_iterator = lstTriangleFans.begin();
    while( fan_iterator != lstTriangleFans.end() )
    {
      if ( (*fan_iterator).empty() )
	fan_iterator = lstTriangleFans.erase(fan_iterator);
      else
	++fan_iterator;
    }
  }
  else
  {
    assert( 0 && "NOt short indices." );
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Collision::ClipPolygon( const Phoenix::Math::CPlane & plane, std::list< Phoenix::Math::CVector3<float> > & lstVertices )
{

  const int POS_SIDE =  1;
  const int NEG_SIDE =  -1;

  std::list< Phoenix::Math::CVector3<float> > lstVerticesNew;


  if ( lstVertices.size() < 3 ) return;

  // Determine side of first vertex in respect to plane
  std::list< Phoenix::Math::CVector3<float> >::iterator it;
  it = lstVertices.begin();

  CVector3<float> vCurrPoint = *it;
  int iCurrSide = 0;
  int iNextSide = 0;
  float fNextDot = 0.0f;
  float fCurrDot = plane[0] * (*it)[0] +
                   plane[1] * (*it)[1] +
                   plane[2] * (*it)[2] + plane[3];

  iCurrSide      = fCurrDot > -EPSILON ? POS_SIDE : NEG_SIDE;

  // if first vertex is on positive side, then push it to list
  if ( iCurrSide == POS_SIDE ) { lstVerticesNew.push_back( *it );  }


  // step to next vertex
  it++;

  for( ; it != lstVertices.end(); it++)
  {
    CVector3<float> & vNextPoint = *it;

    fNextDot = plane[0] * vNextPoint[0] +
               plane[1] * vNextPoint[1] +
	       plane[2] * vNextPoint[2] + plane[3];

    iNextSide =  fNextDot > -EPSILON ? POS_SIDE : NEG_SIDE;

    // If vertices are on different sides of plane,
    if ( iNextSide != iCurrSide )
    {
      CVector3<float> vTmp = (vCurrPoint-vNextPoint);
      float fT             = fCurrDot / (plane[0] * vTmp[0] +
					 plane[1] * vTmp[1] +
					 plane[2] * vTmp[2]);

      CVector3<float> vClipPoint = vCurrPoint + fT * (vNextPoint - vCurrPoint);
      lstVerticesNew.push_back(vClipPoint);
    }

    if ( iNextSide == POS_SIDE )
    {
      // If vertices are on positive side of plane
      lstVerticesNew.push_back( (*it) );
    }

    vCurrPoint = vNextPoint;
    fCurrDot   = fNextDot;
    iCurrSide  = iNextSide;
  }
  /////////////////////////////////////////////////////////////////
  // check situation from last vertex to first
  CVector3<float> & vNextPoint = lstVertices.front();
  fNextDot = plane[0] * vNextPoint[0] +
             plane[1] * vNextPoint[1] +
	     plane[2] * vNextPoint[2] + plane[3];
  iNextSide =  fNextDot > -EPSILON ? POS_SIDE : NEG_SIDE;

  // If vertices are on different sides of plane,
  if ( iNextSide != iCurrSide )
  {
    CVector3<float> vTmp = (vCurrPoint-vNextPoint);
    float fT             = fCurrDot / ( plane[0] * vTmp[0] +
				        plane[1] * vTmp[1] +
				        plane[2] * vTmp[2] );

    CVector3<float> vClipPoint = vCurrPoint + fT * (vNextPoint - vCurrPoint);
    lstVerticesNew.push_back(vClipPoint);
  }
  // swap vertex lists
  lstVerticesNew.swap( lstVertices );

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Collision::ClipPolygon( const Phoenix::Math::CPlane & plane, std::list< Phoenix::Spatial::CVertex > & lstVertices )
{

  const int POS_SIDE =  1;
  const int NEG_SIDE =  -1;

  std::list< Phoenix::Spatial::CVertex > lstVerticesNew;


  if ( lstVertices.size() < 3 ) return;

  // Determine side of first vertex in respect to plane
  std::list< Phoenix::Spatial::CVertex >::iterator it;
  it = lstVertices.begin();

  CVertex vCurrPoint = *it;
  int iCurrSide = 0;
  int iNextSide = 0;
  float fNextDot = 0.0f;
  float fCurrDot = plane[0] * (*it).GetPosition()[0] +
                   plane[1] * (*it).GetPosition()[1] +
                   plane[2] * (*it).GetPosition()[2] + plane[3];

  iCurrSide      = fCurrDot > -EPSILON ? POS_SIDE : NEG_SIDE;

  // if first vertex is on positive side, then push it to list
  if ( iCurrSide == POS_SIDE ) { lstVerticesNew.push_back( *it );  }


  // step to next vertex
  it++;

  for( ; it != lstVertices.end(); it++)
  {
    CVertex & vNextPoint = *it;

    fNextDot = plane[0] * vNextPoint.GetPosition()[0] +
               plane[1] * vNextPoint.GetPosition()[1] +
	       plane[2] * vNextPoint.GetPosition()[2] + plane[3];

    iNextSide =  fNextDot > -EPSILON ? POS_SIDE : NEG_SIDE;

    // If vertices are on different sides of plane,
    if ( iNextSide != iCurrSide )
    {
      CVector3<float> vTmp = (vCurrPoint.GetPosition()-vNextPoint.GetPosition());
      float fT             = fCurrDot / (plane[0] * vTmp[0] +
					 plane[1] * vTmp[1] +
					 plane[2] * vTmp[2]);

      CVector3<float> vClipPoint = vCurrPoint.GetPosition() + fT * (vNextPoint.GetPosition() - vCurrPoint.GetPosition());
      CVertex vertex;
      vertex.SetPosition( vClipPoint );
      vertex.SetNormal( (vCurrPoint.GetPosition() + fT * ( vNextPoint.GetNormal() - vCurrPoint.GetNormal())).GetNormalized() );
      lstVerticesNew.push_back(vertex);
    }

    if ( iNextSide == POS_SIDE )
    {
      // If vertices are on positive side of plane
      lstVerticesNew.push_back( (*it) );
    }

    vCurrPoint = vNextPoint;
    fCurrDot   = fNextDot;
    iCurrSide  = iNextSide;
  }
  /////////////////////////////////////////////////////////////////
  // check situation from last vertex to first
  CVertex & vNextPoint = lstVertices.front();
  fNextDot = plane[0] * vNextPoint.GetPosition()[0] +
             plane[1] * vNextPoint.GetPosition()[1] +
             plane[2] * vNextPoint.GetPosition()[2] + plane[3];
  iNextSide =  fNextDot > -EPSILON ? POS_SIDE : NEG_SIDE;

  // If vertices are on different sides of plane,
  if ( iNextSide != iCurrSide )
  {
    CVector3<float> vTmp = (vCurrPoint.GetPosition()-vNextPoint.GetPosition());
    float fT             = fCurrDot / ( plane[0] * vTmp[0] +
				        plane[1] * vTmp[1] +
				        plane[2] * vTmp[2] );

    CVector3<float> vClipPoint = vCurrPoint.GetPosition() + fT * (vNextPoint.GetPosition() - vCurrPoint.GetPosition());
    CVertex vertex;
    vertex.SetPosition( vClipPoint );
    lstVerticesNew.push_back(vertex);

  }
  // swap vertex lists
  lstVerticesNew.swap( lstVertices );

}
/////////////////////////////////////////////////////////////////
Phoenix::Collision::VOLUME_INTERSECTION
Phoenix::Collision::OBBIntersectsPolytope( const COrientedBox & obBox, const Phoenix::Volume::CPolytope & poly )
{


  // The effective radius of the box respect to a plane
  float fEffRadius;
  float fDot;

  CVector3<float> vNormal;
  /////////////////////////////////////////////////////////////////
  // When box length (principal axis) is much greater than the two others,
  // it is better to use line segment test for better accuracy.
  /////////////////////////////////////////////////////////////////
  bool bUseLineSegmentTest = (( obBox.GetLength()-obBox.GetWidth() ) > (obBox.GetHalfWidth())) ||
                             (( obBox.GetLength()-obBox.GetHeight()) > (obBox.GetHalfHeight()));


  if ( bUseLineSegmentTest )
  {
    float fDot2;
    // The line seqment
    CVector3<float> vQ1 = obBox.GetPosition() + (obBox.GetForwardVector()*(obBox.GetLength() * 0.5f)) ;
    CVector3<float> vQ2 = vQ1 - (obBox.GetForwardVector()*obBox.GetLength()) ;
    std::vector<Phoenix::Math::CPlane>::const_iterator planeIt;

    // for each plane do
    for( planeIt = poly.GetPlanes().begin(); planeIt != poly.GetPlanes().end(); planeIt++ ){

      vNormal.Set( planeIt->GetArray() );

      // Since axes from COrientable are always Unit length,
      // we include proper dimensions in the equation.
      fEffRadius = 0.5f * ( fabsf( (obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal) ) +
			    fabsf( (obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal) ) );

      // Calculate 4D dot product between a plane and the line endpoints
      fDot  = vNormal.Dot( vQ1) + (*planeIt)[3];
      fDot2 = vNormal.Dot( vQ2) + (*planeIt)[3];

      if (fDot <= -fEffRadius && fDot2 <= -fEffRadius)
      {
	return OUTSIDE;

      } else if ( (fDot < -fEffRadius && fDot2 > -fEffRadius)){

	// Cut off the part from the cylinder which lies outside the frustum
	float fT = (fEffRadius + fDot2) / vNormal.Dot(vQ2-vQ1) ;

	vQ1 = vQ2 + (fT*(vQ1-vQ2));

      } else if ( fDot2 < -fEffRadius && fDot > -fEffRadius ){

	// Cut off the part from the cylinder which lies outside the frustum
	float fT = (fEffRadius + fDot) / vNormal.Dot(vQ1-vQ2) ;
	vQ2 = vQ1 + (fT*(vQ2-vQ1));
      }

    }

  } else {

    std::vector<Phoenix::Math::CPlane>::const_iterator planeIt;
    // for each plane do
    for( planeIt = poly.GetPlanes().begin(); planeIt != poly.GetPlanes().end(); planeIt++ ){

      vNormal.Set( planeIt->GetArray());

      // Since axes from COrientable are always Unit length,
      // we include proper dimensions in the equation.

      fEffRadius = 0.5f * ( fabsf((obBox.GetForwardVector()*obBox.GetLength()).Dot(vNormal)) +
			    fabsf((obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal)) +
			    fabsf((obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal)) );

      // Calculate 4D dot product between plane and box center
      fDot  = vNormal.Dot( obBox.GetPosition()) + (*planeIt)[3];

      if ( fDot <= -fEffRadius )
      {
	return OUTSIDE;
      }

    }
  }
  // Ok, volumes intersect.
  return INTERSECTION;
}
/////////////////////////////////////////////////////////////////
VOLUME_INTERSECTION
Phoenix::Collision::SphereIntersectsPolytope( const CSphere &sphere, const Phoenix::Volume::CPolytope & poly )
{
  float fDistance = 0.0f;
  int iType;
  bool bIntersection = false;
  std::vector<Phoenix::Math::CPlane>::const_iterator planeIt;
  for( planeIt = poly.GetPlanes().begin();planeIt != poly.GetPlanes().end(); planeIt++ )
  {
    iType = Phoenix::Collision::SphereIntersectsPlane( *planeIt, sphere, fDistance);

    // If the object is behind of any of the planes (on negative half-side),
    // then it is outside the frustum
    if (iType < 0)
    {
      return OUTSIDE;
    }

    // If the object intersects any of the planes,
    // then might be intersecting the frustum
    if (iType == 0)
    {
      bIntersection = 1;
    }
  }
  // If sphere intersected one or more planes, it is partially in frustum.
  if ( bIntersection )
    return INTERSECTION;

  // if we end up here, the object is neither behind or intersecting of any of the
  // planes. Hence, it is inside.
  return INSIDE;
}
/////////////////////////////////////////////////////////////////
Phoenix::Collision::VOLUME_INTERSECTION
Phoenix::Collision::AABBIntersectsPolytope( const Phoenix::Volume::CAxisAlignedBox &aabb, const Phoenix::Volume::CPolytope & poly )
{
  CVector3<float> vNormal;
  CVector3<float> vX(1,0,0), vY(0,1,0), vZ(0,0,1);
  float fEffectiveRadius;

  std::vector<Phoenix::Math::CPlane>::const_iterator it;
  for( it = poly.GetPlanes().begin(); it != poly.GetPlanes().end(); it++)
  {
    const CPlane &plane = *it;
    vNormal.Set( const_cast<float *>(plane.GetArray()));
    fEffectiveRadius = fabsf((aabb.GetHalfWidth() *vX).Dot(vNormal)) +
                       fabsf((aabb.GetHalfHeight()*vY).Dot(vNormal)) +
                       fabsf((aabb.GetHalfLength()*vZ).Dot(vNormal));
    if( PLANE_DOT_POS(plane, aabb.GetPosition()) <= -fEffectiveRadius) return OUTSIDE;
  }

  // Frustum intersects box.
  return INTERSECTION;
}
/////////////////////////////////////////////////////////////////
Phoenix::Collision::VOLUME_INTERSECTION
Phoenix::Collision::AABBIntersectsPolytope( const Phoenix::Volume::CAxisAlignedCube &aabb, const Phoenix::Volume::CPolytope & poly )
{

  CVector3<float> vNormal;
  float fEffectiveRadius;

  CVector3<float> vHalfWidthX(aabb.GetHalfWidth(),0.0f,0.0f);
  CVector3<float> vHalfWidthY(0.0f, aabb.GetHalfWidth(), 0.0f);
  CVector3<float> vHalfWidthZ(0.0f, 0.0f, aabb.GetHalfWidth());

  std::vector<Phoenix::Math::CPlane>::const_iterator it;
  for( it = poly.GetPlanes().begin(); it != poly.GetPlanes().end(); it++)
  {
    const CPlane &plane = *it;
    vNormal.Set( const_cast<float *>(plane.GetArray()));
    fEffectiveRadius = fabsf(vHalfWidthX.Dot(vNormal)) +
                       fabsf(vHalfWidthY.Dot(vNormal)) +
                       fabsf(vHalfWidthZ.Dot(vNormal));

    if( PLANE_DOT_POS(plane, aabb.GetPosition()) <= -fEffectiveRadius) return OUTSIDE;
  }

  // Frustum intersects cube.
  return INTERSECTION;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::RayIntersectsOBB( const CRay &ray, const COrientedBox &obBox,  float *pfValue )
{
  float fT_min,fT_max;
  float fE, fF;
  float f1DivF;
  float fT_1,fT_2;
  float fHalfValue;
  int   bStartInsideBox = 1;
  CVector3<float> vPoint = obBox.GetPosition() - ray.GetPosition();
  bool bFirstTime = true;
  if ( fabs(vPoint.Dot(obBox.GetForwardVector())) > obBox.GetHalfLength()) bStartInsideBox = 0;
  if ( fabs(vPoint.Dot(obBox.GetRightVector())  ) > obBox.GetHalfWidth())  bStartInsideBox = 0;
  if ( fabs(vPoint.Dot(obBox.GetUpVector())     ) > obBox.GetHalfHeight()) bStartInsideBox = 0;

  if ( bStartInsideBox )
  {
    if ( pfValue != NULL ) *pfValue = 0.0f;
    return true;
  }

  // for each dimension in OBB do
  for ( int i=0;i<3;i++)
  {
    // Get correct values for each loop
    switch ( i )
    {
    case 0:
      fE = obBox.GetForwardVector().Dot( vPoint );
      fF = obBox.GetForwardVector().Dot( ray.GetDirection() );
      fHalfValue = obBox.GetHalfLength();
      break;
    case 1:
      fE = obBox.GetRightVector().Dot( vPoint );
      fF = obBox.GetRightVector().Dot( ray.GetDirection() );
      fHalfValue = obBox.GetHalfWidth();
      break;
    default:
      fE = obBox.GetUpVector().Dot( vPoint );
      fF = obBox.GetUpVector().Dot( ray.GetDirection() );
      fHalfValue = obBox.GetHalfHeight();
    }

    if ( TOO_CLOSE_TO_ZERO(fF) )
    {
      if (-fE - fHalfValue > 0.0f || -fE + fHalfValue < 0.0f )
      {
	return false;
      }
    }
    else
    {
      f1DivF = 1.0f / fF;
      fT_1  = (fE + fHalfValue)*f1DivF;
      fT_2  = (fE - fHalfValue)*f1DivF;

      // Put values in correct order
      if ( fT_1 > fT_2 )
      {
	float fTmp = fT_1;
	fT_1 = fT_2;
	fT_2 = fTmp;
      }

      /// Min/max comparison, initialise min/max on first time.
      if ( bFirstTime )
      {
	fT_min = fT_1;
	fT_max = fT_2;
	bFirstTime = false;
      }
      else
      {
	if ( fT_1 > fT_min) fT_min = fT_1;
	if ( fT_2 < fT_max) fT_max = fT_2;
      }
      // Perform tests, and exit on non-intersection.
      if ( fT_min > fT_max ) return false;
      if ( fT_max < 0.0f   ) return false;

    } // close to zero
  } // for

  ////////////////////
  /// Set distance from origin to closest intersection point
  if ( pfValue != NULL )
  {
    if ( fT_min > 0.0f) *pfValue = fT_min;
    else                *pfValue = fT_max;
  }
  // return intersection
  return true;

}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::RayIntersectsAACube( const CRay &ray, const CAxisAlignedCube &aaCube,  float *pfValue )
{
	CAxisAlignedBox box( aaCube.GetPosition(), aaCube.GetWidth(), aaCube.GetWidth(), aaCube.GetWidth());
	return RayIntersectsAABB( ray, box, pfValue );
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::RayIntersectsAABB( const CRay &ray, const CAxisAlignedBox &aaBox,  float *pfValue )
{
  float fT_min,fT_max;
  float fE, fF;
  float f1DivF;
  float fT_1,fT_2;
  float fHalfValue;
  int   bStartInsideBox = 1;
  CVector3<float> vPoint = aaBox.GetPosition() - ray.GetPosition();
  bool bFirstTime = true;
  if ( fabs(vPoint[2]) > aaBox.GetHalfLength()) bStartInsideBox = 0;
  if ( fabs(vPoint[0]) > aaBox.GetHalfWidth())  bStartInsideBox = 0;
  if ( fabs(vPoint[1]) > aaBox.GetHalfHeight()) bStartInsideBox = 0;

  if ( bStartInsideBox )
  {
    if ( pfValue != NULL ) *pfValue = 0.0f;
    return true;
  }

  // for each dimension in AABB do
  for ( int i=0;i<3;i++)
  {
    // Get correct values for each loop
    switch ( i )
    {
    case 0:
      fE = vPoint[0];
      fF = ray.GetDirection()[0];
      fHalfValue = aaBox.GetHalfWidth();
      break;
    case 1:
      fE = vPoint[1];
      fF = ray.GetDirection()[1];
      fHalfValue = aaBox.GetHalfHeight();
      break;
    default:
      fE = vPoint[2];
      fF = ray.GetDirection()[2];
      fHalfValue = aaBox.GetHalfLength();
    }

    if ( TOO_CLOSE_TO_ZERO(fF) )
    {
      if (-fE - fHalfValue > 0.0f || -fE + fHalfValue < 0.0f )
      {
	return false;
      }
    }
    else
    {
      f1DivF = 1.0f / fF;
      fT_1  = (fE + fHalfValue)*f1DivF;
      fT_2  = (fE - fHalfValue)*f1DivF;

      // Put values in correct order
      if ( fT_1 > fT_2 )
      {
	float fTmp = fT_1;
	fT_1 = fT_2;
	fT_2 = fTmp;
      }

      /// Min/max comparison, initialise min/max on first time.
      if ( bFirstTime )
      {
	fT_min = fT_1;
	fT_max = fT_2;
	bFirstTime = false;
      }
      else
      {
	if ( fT_1 > fT_min) fT_min = fT_1;
	if ( fT_2 < fT_max) fT_max = fT_2;
      }
      // Perform tests, and exit on non-intersection.
      if ( fT_min > fT_max ) return false;
      if ( fT_max < 0.0f   ) return false;

    } // close to zero
  } // for

  ////////////////////
  /// Set distance from origin to closest intersection point
  if ( pfValue != NULL )
  {
    if ( fT_min > 0.0f) *pfValue = fT_min;
    else                *pfValue = fT_max;
  }
  // return intersection
  return true;

}

/////////////////////////////////////////////////////////////////
//
// As described in document:
// http://www.geometrictools.com/Documentation/IntersectionSphereCone.pdf
//
/////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::SphereIntersectsCone( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::CCone &cone )
{
  bool bRetval = 0;

   CVector3<float> vU = cone.GetPosition() - (sphere.GetRadius()*cone.SinReciprocal())*cone.GetDirection();

   CVector3<float> vD = sphere.GetPosition() - vU;

   float fDsqr = vD.Dot(vD);

   float fE = cone.GetDirection().Dot(vD);

   if ( fE > 0.0f && fE*fE >= fDsqr*cone.CosSqr() )
   {
     vD = sphere.GetPosition() - cone.GetPosition();
     fDsqr = vD.Dot(vD);
     fE = -(cone.GetDirection().Dot(vD));
     if ( fE > 0.0f && fE*fE >= fDsqr*cone.SinSqr() )
       bRetval = (fDsqr <= sphere.GetRadiusSqr());
     else
       bRetval = 1;
   }
   else
   {
     bRetval = 0;
   }
   // #ifndef DEBUG
   //#else
   //   CVector3<float> vU = cone.GetPosition() - (sphere.GetRadius()/cone.SinAngle())*cone.GetDirection();
   //   CVector3<float> vD = sphere.GetPosition() - vU;

   //   if ( cone.GetDirection().Dot( vD ) >= vD.Length()*cone.CosAngle()){

   //     vD = sphere.Position() - cone.Position();
   //     if ( -cone.GetDirection().Dot( vD ) >= vD.Length()*cone.SinAngle()){

   //       bRetval = vD.Length() <= sphere.GetRadius();
   //       CLogger::Error() << "first true, second " << (bRetval ? "true" : "false" ) << std::endl;
   //     }
   //     else {
   //       bRetval = 1;
   //       CLogger::Error() << "first true, second true" << std::endl;
   //     }
   //   }
   //   else
   //   {
   //     CLogger::Error() << "first false" << std::endl;
   //     bRetval = 0;
   //   }
   //#endif

   return bRetval;
}
///////////////////////////////////////////////////////////////////////////////
bool
Phoenix::Collision::OBBIntersectsOBB( const Phoenix::Volume::COrientedBox & box1, const Phoenix::Volume::COrientedBox & box2 )
{

  // u = length
  // v = height
  // w = width

  CVector3<float> vFwd( box2.GetForwardVector().Dot(box1.GetForwardVector()),
			box2.GetUpVector().Dot(box1.GetForwardVector()),
			box2.GetRightVector().Dot(box1.GetForwardVector()) );

  CVector3<float> vUp( box2.GetForwardVector().Dot(box1.GetUpVector()),
		       box2.GetUpVector().Dot(box1.GetUpVector()),
		       box2.GetRightVector().Dot(box1.GetUpVector()) );

  CVector3<float> vRight( box2.GetForwardVector().Dot(box1.GetRightVector()),
			  box2.GetUpVector().Dot(box1.GetRightVector()),
			  box2.GetRightVector().Dot(box1.GetRightVector()) );
	
  CMatrix3x3<float> mR (  vFwd[0], vUp[0], vRight[0],
			  vFwd[1], vUp[1], vRight[1],
			  vFwd[2], vUp[2], vRight[2] );
	
  CVector3<float> tmp = box2.GetPosition()-box1.GetPosition();
  CVector3<float> vT( tmp.Dot(box1.GetForwardVector()),
		      tmp.Dot(box1.GetUpVector()),
		      tmp.Dot(box1.GetRightVector()));
  // Tests for a_u, a_v, a_w:
	
  if ( fabs(vT[0]) > box1.GetHalfLength() +
       box2.GetHalfLength() * fabsf(mR(0,0)) +
       box2.GetHalfHeight() * fabsf(mR(0,1)) +
       box2.GetHalfWidth()  * fabsf(mR(0,2)) )
  {
    return false;
  }
  if ( fabs(vT[1]) > box1.GetHalfHeight() +
       box2.GetHalfLength() * fabsf(mR(1,0)) +
       box2.GetHalfHeight() * fabsf(mR(1,1)) +
       box2.GetHalfWidth()  * fabsf(mR(1,2)) )
  {
    return false;
  }

  if ( fabs(vT[2]) > box1.GetHalfWidth() +
       box2.GetHalfLength() * fabsf(mR(2,0)) +
       box2.GetHalfHeight() * fabsf(mR(2,1)) +
       box2.GetHalfWidth()  * fabsf(mR(2,2)) )
  {
    return false;
  }
  // tests for b_u, b_v, b_w:

  if ( fabsf(vT[0] * mR(0,0) + vT[1] * mR(1,0) + vT[2] * mR(2,0)) > box2.GetHalfLength() +
       box1.GetHalfLength()*fabsf(mR(0,0)) +
       box1.GetHalfHeight()*fabsf(mR(1,0)) +
       box1.GetHalfWidth()*fabsf(mR(2,0)) )
  {
    return false;
  }
  if ( fabsf(vT[0] * mR(0,1) + vT[1] * mR(1,1) + vT[2] * mR(2,1)) > box2.GetHalfHeight() +
       box1.GetHalfLength()*fabsf(mR(0,1)) +
       box1.GetHalfHeight()*fabsf(mR(1,1)) +
       box1.GetHalfWidth()*fabsf(mR(2,1)) )
  {
    return false;
  }

  if ( fabsf(vT[0] * mR(0,2) + vT[1] * mR(1,2) + vT[2] * mR(2,2)) > box2.GetHalfWidth() +
       box1.GetHalfLength()*fabsf(mR(0,2)) +
       box1.GetHalfHeight()*fabsf(mR(1,2)) +
       box1.GetHalfWidth()*fabsf(mR(2,2)) )
  {
    return false;
  }
  // tests for combination of each edge from OBBs.
  if ( fabsf( vT[2]*mR(1,0) - vT[1]*mR(2,0) ) > 
       
       box1.GetHalfHeight()*fabsf(mR(2,0)) + 
       box1.GetHalfWidth() *fabsf(mR(1,0)) +
       box2.GetHalfHeight()*fabsf(mR(0,2)) +
       box2.GetHalfWidth() *fabsf(mR(0,1)) )
  {
    return false;
  }
  
  if ( fabsf( vT[2]*mR(1,1) - vT[1]*mR(2,1)) >
       box1.GetHalfHeight()*fabsf(mR(2,1)) + 
       box1.GetHalfWidth()*fabsf(mR(1,1)) +
       box2.GetHalfLength()*fabsf( mR(0,2)) +
       box2.GetHalfWidth()*fabsf( mR(0,0))       )
  {
    return false;
  }

  if ( fabsf( vT[2]*mR(1,2) - vT[1]*mR(2,2)) > 
       box1.GetHalfHeight() * fabsf(mR(2,2)) +
       box1.GetHalfWidth()  * fabsf(mR(1,2)) +
       box2.GetHalfLength() * fabsf(mR(0,1)) +
       box2.GetHalfHeight() * fabsf(mR(0,0)) )
  {
    return false;
  }
  
  if ( fabsf( vT[0] * mR(2,0) - vT[2]*mR(0,0)) > 
       box1.GetHalfLength() * fabsf(mR(2,0)) + 
       box1.GetHalfWidth()  * fabsf(mR(0,0)) + 
       box2.GetHalfHeight() * fabsf(mR(1,2)) + 
       box2.GetHalfWidth()  * fabsf(mR(1,1)))
  {
    return false;
  }
  
  if ( fabsf( vT[0] * mR(2,1) - vT[2]*mR(0,1)) > 
       box1.GetHalfLength() * fabsf(mR(2,1)) + 
       box1.GetHalfWidth()  * fabsf(mR(0,1)) + 
       box2.GetHalfLength() * fabsf(mR(1,2)) + 
       box2.GetHalfWidth()  * fabsf(mR(1,0)))
  {
    return false;
  }

  if ( fabsf( vT[0] * mR(2,2) - vT[2]*mR(0,2)) > 
       box1.GetHalfLength() * fabsf(mR(2,2)) + 
       box1.GetHalfWidth()  * fabsf(mR(0,2)) + 
       box2.GetHalfLength() * fabsf(mR(1,1)) + 
       box2.GetHalfHeight() * fabsf(mR(1,0)))
  {
    return false;
  }
  // aw x bu
  if ( fabsf( vT[1]*mR(0,0) - vT[0]*mR(1,0)) > 
       box1.GetHalfLength()*fabsf(mR(1,0))+
       box1.GetHalfHeight()*fabsf(mR(0,0))+
       box2.GetHalfHeight()*fabsf(mR(2,2))+
       box2.GetHalfWidth() *fabsf(mR(2,1)))
  {
    return false;
  }
  if ( fabsf( vT[1]*mR(0,1) - vT[0]*mR(1,1)) > 
       box1.GetHalfLength()*fabsf(mR(1,1))+
       box1.GetHalfHeight()*fabsf(mR(0,1))+
       box2.GetHalfLength()*fabsf(mR(2,2))+
       box2.GetHalfWidth() *fabsf(mR(2,0)))
  {
    return false;
  }
  if ( fabsf( vT[1]*mR(0,2) - vT[0]*mR(1,2)) > 
       box1.GetHalfLength()*fabsf(mR(1,2))+
       box1.GetHalfHeight()*fabsf(mR(0,2))+
       box2.GetHalfLength()*fabsf(mR(2,1))+
       box2.GetHalfHeight() *fabsf(mR(2,0)))
  {
    return false;
  }
  return true;
}
///////////////////////////////////////////////////////////////////////////////
bool 
Phoenix::Collision::SphereIntersectsAABB( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::CAxisAlignedBox &aabb)
{
    float fDistance = 0.0f;
  float fTmp = 0.0f;
  /////////////////////////////////////////////////////////////////
  // Test for X axis
  float fAxisMax = aabb.GetMax()[0];
  float fAxisMin = aabb.GetMin()[0];
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
  fAxisMax = aabb.GetMax()[1];
  fAxisMin = aabb.GetMin()[1];
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
  fAxisMax = aabb.GetMax()[2];
  fAxisMin = aabb.GetMin()[2];
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
    return false;
  }
  else
  {
    return true;
  }

}
////////////////////////////////////////////////////////////////////////////////
bool 
Phoenix::Collision::SphereIntersectsOBB( const Phoenix::Volume::CSphere &sphere, const Phoenix::Volume::COrientedBox & box )
{
  // convert sphere into OBB's space, and use origo-centered aabb test against sphere.
  CVector3<float> vCenter = sphere.GetPosition() - box.GetPosition();
  
  return SphereIntersectsAABB( CSphere(CVector3<float>( vCenter.Dot( box.GetForwardVector()), 
							vCenter.Dot(box.GetUpVector()), 
							vCenter.Dot( box.GetRightVector())), sphere.GetRadius()),
			       CAxisAlignedBox( CVector3<float>(0,0,0), 
						box.GetLength(), 
						box.GetHeight(), 
						box.GetWidth()));
}
////////////////////////////////////////////////////////////////////////////////
