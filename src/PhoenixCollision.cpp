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
inline int AxisTestX( const COrientedBox &box,
		      const CVector3<float> &vEdge,
		      float fAbsY,
		      float fAbsZ,
		      const CVector3<float> &vOne, 
		      const CVector3<float> &vThree )
{
  float p0 = (-vEdge(2)*vOne(1)) + (vEdge(1)*vOne(2));
  float p2 =  vEdge(1)*vThree(2) +  vEdge(2)*vThree(1);
  float fR = fAbsY * box.GetHalfHeight() + fAbsZ * box.GetHalfLength();
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
  float p0 = (-vEdge(2)*vOne(0)) + (vEdge(0)*vOne(2));
  float p2 =  vEdge(2)*vThree(0) -  vEdge(0)*vThree(2);
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

  float p0 = (-vEdge(1)*vOne(0)) - vEdge(0)*vOne(1);
  float p2 = (-vEdge(1)*vThree(0)) - vEdge(0)*vThree(1);
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
  // "Move" box into origo.
  vVertex0 -= box.GetPosition();
  vVertex1 -= box.GetPosition();
  vVertex2 -= box.GetPosition();

  // Calculate triangle edges.
  CVector3<float> vEdge0 = vVertex1 - vVertex0;
  CVector3<float> vEdge1 = vVertex2 - vVertex1;
  CVector3<float> vEdge2 = vVertex0 - vVertex2;
  
  float fEdgeX;
  float fEdgeY = fabsf( vEdge0(2) );
  float fEdgeZ = fabsf( vEdge0(1) );
  if ( !AxisTestX( box, vEdge0, fEdgeY, fEdgeZ, vVertex0, vVertex2) ) return 0;

  fEdgeY = fabsf( vEdge1(2) );
  fEdgeZ = fabsf( vEdge1(1) );
  if ( !AxisTestX( box, vEdge1, fEdgeY, fEdgeZ, vVertex0, vVertex2) ) return 0;

  fEdgeY = fabsf( vEdge2(2) );
  fEdgeZ = fabsf( vEdge2(1) );
  if ( !AxisTestX( box, vEdge2, fEdgeY, fEdgeZ, vVertex0, vVertex2) ) return 0;
  
  fEdgeX = fabsf( vEdge0(2));
  fEdgeZ = fabsf( vEdge0(0) );
  if ( !AxisTestY( box, vEdge0, fEdgeX, fEdgeZ, vVertex0, vVertex2) ) return 0;

  fEdgeX = fabsf( vEdge1(2));
  fEdgeZ = fabsf( vEdge1(0));
  if ( !AxisTestY( box, vEdge1, fEdgeX, fEdgeZ, vVertex0, vVertex2) ) return 0;

  fEdgeX = fabsf( vEdge2(2));
  fEdgeZ = fabsf( vEdge2(0));
  if ( !AxisTestY( box, vEdge2, fEdgeX, fEdgeZ, vVertex0, vVertex2) ) return 0;
  
  fEdgeX = fabsf( vEdge0(1));
  fEdgeY = fabsf( vEdge0(0));
  if ( !AxisTestZ( box, vEdge0, fEdgeX, fEdgeY, vVertex0, vVertex2) ) return 0;

  fEdgeX = fabsf( vEdge1(1));
  fEdgeY = fabsf( vEdge1(0));
  if ( !AxisTestZ( box, vEdge1, fEdgeX, fEdgeY, vVertex0, vVertex2) ) return 0;

  fEdgeX = fabsf( vEdge2(1));
  fEdgeY = fabsf( vEdge2(0));
  if ( !AxisTestZ( box, vEdge2, fEdgeX, fEdgeY, vVertex0, vVertex2) ) return 0;
  
  float fMin, fMax;
  // Test X-direction triangle AABB bs box
  FindMinMax( vVertex0(0), vVertex1(0), vVertex2(0), fMin, fMax);
  if ( fMin > box.GetHalfWidth() || fMax < -box.GetHalfWidth()) return 0;
  // Test Y-direction triangle AABB vs box
  FindMinMax( vVertex0(1), vVertex1(1), vVertex2(1), fMin, fMax);
  if ( fMin > box.GetHalfHeight() || fMax < -box.GetHalfHeight()) return 0;
  // Test Z-direction triangle AABB vs box
  FindMinMax( vVertex0(2), vVertex1(2), vVertex2(2), fMin, fMax);
  if ( fMin > box.GetHalfLength() || fMax < -box.GetHalfLength()) return 0;

  
  // Check if box intersects triangle plane, if not -> fail
  CPlane plane;
  vVertex1 = vEdge0.Cross(vEdge1);
  plane.Calculate( vVertex1, vVertex0);
  if ( !PlaneIntersectsBox( plane, box )) return 0;
  
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
/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;

/*======================== Z-tests ========================*/
#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;

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
