#include "PhoenixFrustum.h"
#include <list>
/////////////////////////////////////////////////////////////////
using std::list;
using namespace Phoenix::Graphics;
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
#define PLANE_DOT_POS( PLANE, POS ) (PLANE[0]*POS[0] + PLANE[1]*POS[1] + PLANE[2]*POS[2] + PLANE[3])
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrustum::CFrustum()
{
  // Add planes to the list 
  CPlane top,bottom,left,right,near,far;

  AddPlane( top );
  AddPlane( bottom );
  AddPlane( left );
  AddPlane( right );
  AddPlane( near );
  AddPlane( far );

  // Make pointers to the planes
  list<CPlane>::iterator it = Planes().begin();
  
  m_aPlanes[TOP]    = &(*it); it++;
  m_aPlanes[BOTTOM] = &(*it); it++;  
  m_aPlanes[LEFT]   = &(*it); it++;  
  m_aPlanes[RIGHT]  = &(*it); it++;  
  m_aPlanes[BACK]   = &(*it); it++;  
  m_aPlanes[FRONT]    = &(*it); 

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CFrustum::SetPlane(FRUSTUM_PLANE nIndex, CPlane &vPlane)
{
  *(m_aPlanes[nIndex]) = vPlane;
}
/////////////////////////////////////////////////////////////////
CPlane &
Phoenix::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex )
{
  return *(m_aPlanes[nIndex]);
}
/////////////////////////////////////////////////////////////////
CVector3<float>
Phoenix::Graphics::CFrustum::GetCorner( FRUSTUM_CORNER nIndex )
{
  return m_aCorners[nIndex];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CFrustum::SetCorner( FRUSTUM_CORNER nIndex, const CVector3<float> & vPoint )
{
  m_aCorners[nIndex] = vPoint;  
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::CFrustum::IntersectsAABB( const Phoenix::Volume::CAxisAlignedBox &aaBox )
{

  CVector3<float> vNormal;
  CVector3<float> vX(1,0,0), vY(0,1,0), vZ(0,0,1);
  float fEffectiveRadius;

  for(int iPlane=0;iPlane<NUM_FRUSTUM_PLANES;iPlane++)
  {  
    CPlane &plane = GetPlane( (FRUSTUM_PLANE)iPlane );
    vNormal.UseExternalData( plane.GetArray());
    fEffectiveRadius = fabsf((aaBox.GetHalfWidth() *vX).Dot(vNormal)) + 
                       fabsf((aaBox.GetHalfHeight()*vY).Dot(vNormal)) + 
                       fabsf((aaBox.GetHalfLength()*vZ).Dot(vNormal));
    if( PLANE_DOT_POS(plane, aaBox.GetPosition()) <= -fEffectiveRadius) return 0;    
  }
  
  // Frustum intersects box.
  return 1;

}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::CFrustum::IntersectsCube( const Phoenix::Volume::CAxisAlignedCube &aaCube )
{

  CVector3<float> vNormal;
  CVector3<float> vX(1,0,0), vY(0,1,0), vZ(0,0,1);
  float fEffectiveRadius;
  CVector3<float> vHalfWidthX = aaCube.GetHalfWidth()*vX;
  CVector3<float> vHalfWidthY = aaCube.GetHalfWidth()*vY;
  CVector3<float> vHalfWidthZ = aaCube.GetHalfWidth()*vZ;
  
  for(int iPlane=0;iPlane<NUM_FRUSTUM_PLANES;iPlane++)
  {  
    CPlane &plane = GetPlane( (FRUSTUM_PLANE)iPlane );
    vNormal.UseExternalData( plane.GetArray());
    fEffectiveRadius = fabsf(vHalfWidthX.Dot(vNormal)) + 
                       fabsf(vHalfWidthY.Dot(vNormal)) + 
                       fabsf(vHalfWidthZ.Dot(vNormal));
    /*std::cerr << "planea " 
	      << iPlane << " effectiveradius" << fEffectiveRadius << " and dotpos: "
	      << PLANE_DOT_POS(plane, aaCube.GetPosition()) << std::endl;*/
    if( PLANE_DOT_POS(plane, aaCube.GetPosition()) <= -fEffectiveRadius) return 0;    
  }

  // Frustum intersects cube.
  return 1;
}
/////////////////////////////////////////////////////////////////
// Phoenix::Graphics::CFrustum::CFrustumIntersection_t
// Phoenix::Graphics::CFrustum::IntersectsAABB( const CAxisAlignedBox &aaBox )
// {
  
//   CPlaneIntersection iRel;
  
//   for(int iPlane=0;iPlane<NUM_FRUSTUM_PLANES;iPlane++){

//     iRel = PlaneIntersectsAABB( GetPlane( (CFrustumPlane)iPlane ), aaBox);
//     // If the object is behind of any of the planes, 
//     // then it is outside the frustum
//     if (iRel == Geometry::BEHIND)
//     {
//       return OUTSIDE;
//     }
//     // If the object intersects any of the planes, 
//     // then it is intersecting the frustum
//     if (iRel == Geometry::INTERSECTS)
//     {
//       return INTERSECTION;
//     }
//   }  
//   // if we end up here, the object is neither behind or intersecting of any of the 
//   // planes. Hence, it is inside.
//   return INSIDE;

// }
// /////////////////////////////////////////////////////////////////
// Phoenix::Graphics::CFrustum::CFrustumIntersection_t
// Phoenix::Graphics::CFrustum::IntersectsSphere( const CSphere &sphere )
// {
  
//   Geometry::CPlaneIntersection iRel;
//   float fTmp = 0.0f;

//   for(int iPlane=0;iPlane<NUM_FRUSTUM_PLANES;iPlane++){

//     iRel = Geometry::SphereIntersectsPlane( GetPlane( (CFrustumPlane)iPlane ), sphere, fTmp);
   
//     // If the object is behind of any of the planes, 
//     // then it is outside the frustum
//     if (iRel == Geometry::BEHIND){
// #ifdef DEBUG
//       Core::CLogger::Error() << "Sphere outside plane " << GetPlane((CFrustumPlane)iPlane) ;
//       switch((CFrustumPlane)iPlane)
//       {
//       case TOP:
// 	Core::CLogger::Error() << "TOP" << endl;
// 	break;
//       case BOTTOM:
// 	Core::CLogger::Error() << "BOTTOM" << endl;
// 	break;
//       case LEFT:
// 	Core::CLogger::Error() << "LEFT" << endl;
// 	break;
//       case RIGHT:
// 	Core::CLogger::Error() << "RIGHT" << endl;
// 	break;
//       case BACK:
// 	Core::CLogger::Error() << "BACK" << endl;
// 	break;
//       case FRONT:
// 	Core::CLogger::Error() << "FRONT" << endl;
// 	break;
//       }
// #endif
//       return OUTSIDE;

//     }
//     // If the object intersects any of the planes, 
//     // then it is intersecting the frustum
//     if (iRel == Geometry::INTERSECTS) {
//       return INTERSECTION;
//     }
//   }  
//   // if we end up here, the object is neither behind or intersecting of any of the 
//   // planes. Hence, it is inside.
//   return INSIDE;
  
// }


// Phoenix::Graphics::CFrustum::CFrustumIntersection_t
// Phoenix::Graphics::CFrustum::IntersectsOrientedBox( const COrientedBox &obBox)
// {
  
//   char bUseLineSegmentTest = 0;
  
//   // The effective radius of the box respect to a plane
//   float fEffRadius	= 0.0f;
//   float fDot		= 0.0f;
//   float fDot2		= 0.0f;

//   CVector3 vNormal;
  
//   // When box length (principal axis) is much greater than the two others,
//   // it is better to use line segment test for better accuracy.
//   bUseLineSegmentTest = (( obBox.GetLength()-obBox.GetWidth() ) > (0.5f * obBox.GetWidth())) || 
//                         ( (obBox.GetLength()-obBox.GetHeight()) > (0.5f * obBox.GetHeight()));
  
//   if ( bUseLineSegmentTest ) 
//   {

//     // The line seqment
//     CVector3 vQ1 = obBox.GetPosition() + (obBox.GetForwardVector()*(obBox.GetLength() * 0.5f)) ;
//     CVector3 vQ2 = vQ1 - (obBox.GetForwardVector()*obBox.GetLength()) ;
    
//     for(int iPlane=0;iPlane<NUM_FRUSTUM_PLANES;iPlane++){
      
//       vNormal.UseExternalData( m_aPlanes[iPlane]->m_pValues);
//       // Since axes from COrientable are always Unit length,
//       // we include proper dimensions in the equation.
//       fEffRadius = 0.5f * ( fabsf( (obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal) ) +
// 			    fabsf( (obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal) ) );
      
//       // Calculate 4D dot product between a plane and the line endpoints
//       fDot  = vNormal.Dot( vQ1) + (*m_aPlanes[iPlane])[CPlane::D];
//       fDot2 = vNormal.Dot( vQ2) + (*m_aPlanes[iPlane])[CPlane::D];

//       if (fDot <= -fEffRadius && fDot2 <= -fEffRadius)
//       {
// 	return OUTSIDE;

//       } else if ( (fDot < -fEffRadius && fDot2 > -fEffRadius)){
// 	// Cut off the part from the cylinder which lies outside the frustum
// 	float fT = (fEffRadius + fDot2) / vNormal.Dot(vQ2-vQ1) ;
	
// 	vQ1 = vQ2 + (fT*(vQ1-vQ2));

//       } else if ( fDot2 < -fEffRadius && fDot > -fEffRadius ){
	
// 	// Cut off the part from the cylinder which lies outside the frustum
// 	float fT = (fEffRadius + fDot) / vNormal.Dot(vQ1-vQ2) ;
// 	vQ2 = vQ1 + (fT*(vQ2-vQ1));
//       }
      
//     }  
    
//   } else {


//     for(int iPlane=0;iPlane<NUM_FRUSTUM_PLANES;iPlane++){
      
//       vNormal.UseExternalData( m_aPlanes[iPlane]->m_pValues);

//       // Since axes from COrientable are always Unit length,
//       // we include proper dimensions in the equation.
      
//       fEffRadius = 0.5f * ( fabsf((obBox.GetForwardVector()*obBox.GetLength()).Dot(vNormal)) +
// 			   fabsf((obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal)) +
// 			   fabsf((obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal)) );
      
//       // Calculate 4D dot product between plane and box center
//       fDot  = vNormal.Dot( obBox.GetPosition()) + (*m_aPlanes[iPlane])[CPlane::D];

//       if ( fDot <= -fEffRadius )
//       {
// 	return OUTSIDE;
//       }
      
//     }  
//   }
  
//   return INTERSECTION;
// }

#undef PLANE_DOT_POS
