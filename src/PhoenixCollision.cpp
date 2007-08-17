#include "PhoenixMath.h"
#include "PhoenixCollision.h"
#include <iostream>
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
inline int AxisTestX( const COrientedBox &box,
		      const CVector3<float> &vEdge,
		      float fAbsY,
		      float fAbsZ,
		      const CVector3<float> &vOne, 
		      const CVector3<float> &vThree )
{
  float p0 =  vEdge(1)*vOne(2)   -  vEdge(2)*vOne(1);
  float p2 =  vEdge(1)*vThree(2) -  vEdge(2)*vThree(1);
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
  float p0 = vEdge(2)*vOne(0)   -  vEdge(0)*vOne(2);
  float p2 = vEdge(2)*vThree(0) -  vEdge(0)*vThree(2);
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

  float p0 = vEdge(0)*vOne(1) - vEdge(1)*vOne(0);
  float p2 = vEdge(0)*vThree(1) - vEdge(1)*vThree(0); 
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
  
  CMatrix4x4<float> mRotation( box.GetRightVector()(0),   box.GetUpVector()(0),  box.GetForwardVector()(0),  0,
			       box.GetRightVector()(1),   box.GetUpVector()(1),  box.GetForwardVector()(1),  0,         
			       box.GetRightVector()(2),   box.GetUpVector()(2),  box.GetForwardVector()(2),  0,
			       0,                         0,                     0,                          1 );
  mRotation.Transpose();
  
  vVertex0 = Rotate( vVertex0, mRotation );
  vVertex1 = Rotate( vVertex1, mRotation );
  vVertex2 = Rotate( vVertex2, mRotation );
  
  // Calculate triangle edges.
  CVector3<float> vEdge0 = vVertex1 - vVertex0;
  CVector3<float> vEdge1 = vVertex2 - vVertex1;
  CVector3<float> vEdge2 = vVertex0 - vVertex2;


  float fAx;
  float fAy = fabsf( vEdge0(2) );
  float fAz = fabsf( vEdge0(1) );
  
  
  if ( !AxisTestX( box, vEdge0, fAy, fAz, vVertex0, vVertex2) ) 
  {
    cerr << "fail 1" << endl;
    return 0;
  }

  fAy = fabsf( vEdge1(2) );
  fAz = fabsf( vEdge1(1) );
  if ( !AxisTestX( box, vEdge1, fAy, fAz, vVertex0, vVertex2) ) 
  {
    cerr << "fail 2" << endl;
    return 0;
  }

  fAy = fabsf( vEdge2(2) );
  fAz = fabsf( vEdge2(1) );
  if ( !AxisTestX( box, vEdge2, fAy, fAz, vVertex1, vVertex2) ) 
  {
    cerr << "fail 3" << endl;
    return 0;
  }
  
  fAx = fabsf( vEdge0(2));
  fAz = fabsf( vEdge0(0) );
  if ( !AxisTestY( box, vEdge0, fAx, fAz, vVertex0, vVertex2) ) 
  {
    cerr << "fail 4" << endl;
    return 0;
  }

  fAx = fabsf( vEdge1(2));
  fAz = fabsf( vEdge1(0));
  if ( !AxisTestY( box, vEdge1, fAx, fAz, vVertex0, vVertex2) ) 
  {
    cerr << "fail 5" << endl;
    return 0;
  }

  fAx = fabsf( vEdge2(2));
  fAz = fabsf( vEdge2(0));
  if ( !AxisTestY( box, vEdge2, fAx, fAz, vVertex1, vVertex2) ) 
  {
    cerr << "fail 6" << endl;
    return 0;
  }
  
  fAx = fabsf( vEdge0(1));
  fAy = fabsf( vEdge0(0));
  if ( !AxisTestZ( box, vEdge0, fAx, fAy, vVertex0, vVertex2) ) 
  {
    cerr << "fail 7" << endl;
    return 0;
  }

  fAx = fabsf( vEdge1(1));
  fAy = fabsf( vEdge1(0));
  if ( !AxisTestZ( box, vEdge1, fAx, fAy, vVertex0, vVertex2) ) 
  {
    cerr << "fail 8" << endl;
    return 0;
  }

  fAx = fabsf( vEdge2(1));
  fAy = fabsf( vEdge2(0));
  if ( !AxisTestZ( box, vEdge2, fAx, fAy, vVertex1, vVertex2) ) 
  {
    cerr << "fail 9" << endl;
    return 0;
  }
  
  float fMin, fMax;
  // Test X-direction triangle AABB bs box
  FindMinMax( vVertex0(0), vVertex1(0), vVertex2(0), fMin, fMax);
  if ( fMin > box.GetHalfWidth() || fMax < -box.GetHalfWidth()) 
  {
    cerr << "fail 10" << endl;
    return 0;
  }
  // Test Y-direction triangle AABB vs box
  FindMinMax( vVertex0(1), vVertex1(1), vVertex2(1), fMin, fMax);
  if ( fMin > box.GetHalfHeight() || fMax < -box.GetHalfHeight()) 
  {
    cerr << "fail 11" << endl;
    return 0;
  }
  // Test Z-direction triangle AABB vs box
  FindMinMax( vVertex0(2), vVertex1(2), vVertex2(2), fMin, fMax);
  if ( fMin > box.GetHalfLength() || fMax < -box.GetHalfLength()) 
  {
    cerr << "fail 12" << endl;
    return 0;
  }


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
    cerr << "fail 13" << endl;
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
  float fTmp = fabsf(box.GetHalfLength()*vNormal(2));
  fValue = fTmp;

  fTmp = fabsf(box.GetHalfWidth()*vNormal(1));
  fValue += fTmp;

  fTmp = fabsf(box.GetHalfHeight()*vNormal(0));
  fValue += fTmp;

  return ( fDistance <= fValue);
}
/////////////////////////////////////////////////////////////////
// int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3])

// {



//   /*    use separating axis theorem to test overlap between triangle and box */

//   /*    need to test for overlap in these directions: */

//   /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */

//   /*       we do not even need to test these) */

//   /*    2) normal of the triangle */

//   /*    3) crossproduct(edge from tri, {x,y,z}-directin) */

//   /*       this gives 3x3=9 more tests */

//    float v0[3],v1[3],v2[3];

// //   float axis[3];

//    float min,max,p0,p1,p2,rad,fex,fey,fez;		// -NJMP- "d" local variable removed

//    float normal[3],e0[3],e1[3],e2[3];



//    /* This is the fastest branch on Sun */

//    /* move everything so that the boxcenter is in (0,0,0) */

//    SUB(v0,triverts[0],boxcenter);

//    SUB(v1,triverts[1],boxcenter);

//    SUB(v2,triverts[2],boxcenter);



//    /* compute triangle edges */

//    SUB(e0,v1,v0);      /* tri edge 0 */

//    SUB(e1,v2,v1);      /* tri edge 1 */

//    SUB(e2,v0,v2);      /* tri edge 2 */



//    /* Bullet 3:  */

//    /*  test the 9 tests first (this was faster) */

//    fex = fabsf(e0[X]);

//    fey = fabsf(e0[Y]);

//    fez = fabsf(e0[Z]);

//    AXISTEST_X01(e0[Z], e0[Y], fez, fey);

//    AXISTEST_Y02(e0[Z], e0[X], fez, fex);

//    AXISTEST_Z12(e0[Y], e0[X], fey, fex);



//    fex = fabsf(e1[X]);

//    fey = fabsf(e1[Y]);

//    fez = fabsf(e1[Z]);

//    AXISTEST_X01(e1[Z], e1[Y], fez, fey);

//    AXISTEST_Y02(e1[Z], e1[X], fez, fex);

//    AXISTEST_Z0(e1[Y], e1[X], fey, fex);



//    fex = fabsf(e2[X]);

//    fey = fabsf(e2[Y]);

//    fez = fabsf(e2[Z]);

//    AXISTEST_X2(e2[Z], e2[Y], fez, fey);

//    AXISTEST_Y1(e2[Z], e2[X], fez, fex);

//    AXISTEST_Z12(e2[Y], e2[X], fey, fex);



//    /* Bullet 1: */

//    /*  first test overlap in the {x,y,z}-directions */

//    /*  find min, max of the triangle each direction, and test for overlap in */

//    /*  that direction -- this is equivalent to testing a minimal AABB around */

//    /*  the triangle against the AABB */




//    /* test in X-direction */

//    FINDMINMAX(v0[X],v1[X],v2[X],min,max);

//    if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;



//    /* test in Y-direction */

//    FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);

//    if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;



//    /* test in Z-direction */

//    FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);

//    if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;



//    /* Bullet 2: */

//    /*  test if the box intersects the plane of the triangle */

//    /*  compute plane equation of triangle: normal*x+d=0 */

//    CROSS(normal,e0,e1);

//    // -NJMP- (line removed here)

//    if(!planeBoxOverlap(normal,v0,boxhalfsize)) return 0;	// -NJMP-



//    return 1;   /* box and triangle overlaps */

// }
