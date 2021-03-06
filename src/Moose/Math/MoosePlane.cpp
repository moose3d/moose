/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MoosePlane.h"
#include "MooseMath.h"
#include "MooseVector4.h"
#include <math.h>
#include <algorithm>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Moose::Volume;
using namespace Moose::Math;
using namespace std;
/////////////////////////////////////////////////////////////////
Moose::Math::CPlane::CPlane()
{
}
/////////////////////////////////////////////////////////////////
Moose::Math::CPlane::CPlane( const CVector3<float> &vNormal, float fD ) : CVector4<float>(vNormal[0], vNormal[1], vNormal[2], fD)
{

}
/////////////////////////////////////////////////////////////////
Moose::Math::CPlane::CPlane( float fX, float fY, float fZ, float fD ) : CVector4<float>(fX,fY,fZ,fD)
{
}
/////////////////////////////////////////////////////////////////
Moose::Math::CPlane::~CPlane()
{
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CPlane::SetNormal( CVector3<float> vNormal )
{
  vNormal.Normalize();
  m_aValues[0] = vNormal[0];
  m_aValues[1] = vNormal[1];
  m_aValues[2] = vNormal[2];
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CPlane::Normalize()
{
  float fLength = sqrtf( m_aValues[0] * m_aValues[0] +
			 m_aValues[1] * m_aValues[1] +
			 m_aValues[2] * m_aValues[2] );
  if ( fLength > 0.0f )
  {
    fLength = 1.0f / fLength;
    m_aValues[0] *= fLength;
    m_aValues[1] *= fLength;
    m_aValues[2] *= fLength;
    m_aValues[3] *= fLength;
  }
  else
  {
    m_aValues[0] = m_aValues[1] = m_aValues[2] = 0.0f;
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CPlane::SetDistance( float fDistance )
{
  m_aValues[3] = fDistance;
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CPlane::Calculate( CVector3<float> vNormal, const CVector3<float> & vPoint )
{
  vNormal.Normalize();
  m_aValues[0] =  vNormal[0];
  m_aValues[1] =  vNormal[1];
  m_aValues[2] =  vNormal[2];
  m_aValues[3] = -(vNormal.Dot(vPoint));
}
/////////////////////////////////////////////////////////////////
#ifndef SWIG
std::ostream&
Moose::Math::operator<<( std::ostream &stream, const CPlane &plane )
{
  stream << "(" << plane[0] << "," << plane[1] << "," << plane[2] << "," << plane[3] ;
  return stream;
}
#endif
// /////////////////////////////////////////////////////////////////
// CPlaneIntersection
// Geometry::SphereIntersectsPlane( CPlane &plane,
// 				 const CSphere &sphere,
// 				 float &fCenterDistance)
// {

//   CVector3<float> vNormal;
//   vNormal.UseExternalData(plane.m_aValues);
//   float fDistance = vNormal.Dot( sphere.GetPosition()) + plane.Distance();
//   fCenterDistance = fDistance;

//   if ( fabs(fDistance) <= sphere.GetRadius()){
//     return INTERSECTS;
//   } else if ( fDistance > sphere.GetRadius()){
//     return FRONT;
//   }
//   return BEHIND;
// }
// /////////////////////////////////////////////////////////////////
// CVector3<float>
// Geometry::ClosestPointOnLine( const CVector3<float> &vPoint,
// 			      const CVector3<float> &vLinePoint1,
// 			      const CVector3<float> &vLinePoint2 )
// {

//   CVector3<float> vOneToPoint = vPoint - vLinePoint1;

//   CVector3<float> vLine = vLinePoint2 - vLinePoint1;

//   float fLength = vLine.Length();

//   vLine.Normalize;

//   float fDot = vLine.Dot(vOneToPoint);

//   if ( fDot <= 0.0f ){
//     // return the starting point
//     return vLinePoint1;
//     // return the end point
//   } else if ( fDot >= fLength ){
//     return vLinePoint2;
//   }
//   // return the point in the middle
//   return vLinePoint1 + (vLine * fDot );
// }
// /////////////////////////////////////////////////////////////////
// CVector3<float>
// Geometry::ClosestPointOnRay( const CVector3<float> &vPoint,
// 			     const CVector3<float> &vStartPoint,
// 			     const CVector3<float> &vDirection )
// {
//   CVector3<float> vStartToPoint = vPoint - vStartPoint;
//   float fDot = vDirection.Dot(vStartToPoint);
//   return vPoint + (vDirection * fDot );
// }
// /////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////
// char
// Geometry::SphereIntersectsPolygon( CSphere &sphere,
// 				   VertexDescriptor pVertices,
// 				   unsigned int nNumVertices,
// 				   CVector3<float> &vOffsetMovement )
// {

//   char bIntersection = 0;
//   CVector3<float> vOne, vTwo, vThree;
//   CPlane vPlane;

//   // Calculate normal vector
//   vOne.UseExternalData( &(pVertices[0]));
//   vTwo.UseExternalData( &(pVertices[3]));
//   vThree.UseExternalData( &(pVertices[6]));

//   CVector3<float> vNormal = (vTwo - vOne ).Cross(vThree - vOne);
//   vNormal.Normalize;
//   vPlane.Calculate( vNormal, vOne);

//   float fDistance = 0.0f;

//   if ( SphereIntersectsPlane(vPlane, sphere, fDistance ) == INTERSECTS){

//     CVector3<float> vOffset   = vNormal * fDistance;

//     CVector3<float> vPosition = sphere.GetPosition() - vOffset;

//     if ( InsidePolygon( vPosition, pVertices, nNumVertices) ) {

//       bIntersection = 1;

//     } else {

//       bIntersection = SphereIntersectsPolygonEdges( sphere, pVertices, nNumVertices );
//     }
//   }

//   if ( bIntersection )
//   {
//     if ( fDistance > 0.0f )
//     {

//       float fDistanceDiff = sphere.GetRadius() - fDistance;
//       vOffsetMovement = vNormal * fDistanceDiff;

//     } else {

//       float fDistanceDiff = sphere.GetRadius() + fDistance;
//       vOffsetMovement = vNormal * -fDistanceDiff;

//     }
//   }
//   return bIntersection;
// }
// /////////////////////////////////////////////////////////////////
// char
// Geometry::InsidePolygon( CVector3<float> &vPoint,
// 			 VertexDescriptor pVertices,
// 			 unsigned int nNumVertices)
// {
//   char bRetval = 0;
//   CVector3<float> vS, vT, vVect1, vVect2;
//   float fAngle = 0.0f;

//   //CLogger::Log() << DEBUG_HEADER << " Point :" << vPoint << std::endl;

//   for( unsigned int v=0;v<nNumVertices;v++)
//   {
//     vS.UseExternalData( &(pVertices[(v*3)]));
//     vT.UseExternalData( &(pVertices[((v+1)%nNumVertices)*3]));
//     vVect1 = vS - vPoint;
//     vVect2 = vT - vPoint;
//     fAngle += AngleBetweenVectors( vVect1, vVect2);
//   }

//   if ( fabs(fAngle) >= (0.99f * Math::PI * 2.0f)){
//     bRetval = 1;
//   }

//   return bRetval;
// }
// /////////////////////////////////////////////////////////////////
// char
// Geometry::SphereIntersectsPolygonEdges( CSphere &sphere,
// 					VertexDescriptor pVertices,
// 					unsigned int nNumVertices)
// {

//   CVector3<float> vVect1, vVect2, vPoint;
//   CVector3<float> vPos = sphere.GetPosition();

//   for( unsigned int v=0;v<nNumVertices;v++){

//     vVect1.UseExternalData( &(pVertices[v*3]));
//     vVect2.UseExternalData( &(pVertices[((v+1)%nNumVertices)*3]));

//     vPoint = ClosestPointOnLine(vPos, vVect1, vVect2 );

//     // If closest point is within the sphere radius, we have a match
//     if ( (vPoint - sphere.GetPosition()).Length() < sphere.GetRadius()){
//       return 1;
//     }

//   }
//   // No match
//   return 0;
// }
// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// CPlaneIntersection
// Geometry::PlaneIntersectsAABB( const CPlane &rPlane,  const CAxisAlignedBox &rAABB)
// {
//   CVector3<float> vCornerMin;
//   CVector3<float> vCornerMax;
//   float fHalfWidth  = rAABB.GetWidth()*0.5f;
//   float fHalfHeight = rAABB.GetHeight()*0.5f;
//   float fHalfLength = rAABB.GetLength()*0.5f;
//   /////////////////////////////////////////////////////////////////
//   // Test x axis
//   if ( rPlane.m_aValues[0] < 0.0f)
//   {
//     vCornerMin[0] = rAABB.GetPosition().m_aValues[0] - fHalfWidth;
//     vCornerMax[0] = rAABB.GetPosition().m_aValues[0] + fHalfWidth;
//   }
//   else
//   {
//     vCornerMax[0] = rAABB.GetPosition().m_aValues[0] - fHalfWidth;
//     vCornerMin[0] = rAABB.GetPosition().m_aValues[0] + fHalfWidth;
//   }
//   /////////////////////////////////////////////////////////////////
//   // Test y axis
//   if ( rPlane.m_aValues[1] < 0.0f)
//   {
//     vCornerMin[1] = rAABB.GetPosition().m_aValues[1] - fHalfHeight;
//     vCornerMax[1] = rAABB.GetPosition().m_aValues[1] + fHalfHeight;
//   }
//   else
//   {
//     vCornerMax[1] = rAABB.GetPosition().m_aValues[1] - fHalfHeight;
//     vCornerMin[1] = rAABB.GetPosition().m_aValues[1] + fHalfHeight;
//   }
//   /////////////////////////////////////////////////////////////////
//   // Test z axis
//   if ( rPlane.m_aValues[2] < 0.0f)
//   {
//     vCornerMin[2] = rAABB.GetPosition().m_aValues[2] - fHalfLength;
//     vCornerMax[2] = rAABB.GetPosition().m_aValues[2] + fHalfLength;
//   }
//   else
//   {
//     vCornerMax[2] = rAABB.GetPosition().m_aValues[2] - fHalfLength;
//     vCornerMin[2] = rAABB.GetPosition().m_aValues[2] + fHalfLength;
//   }
//   /// Check the intersection of the diagonal line
//   return LineIntersectsPlane( rPlane, vCornerMin, vCornerMax);
// }
// /////////////////////////////////////////////////////////////////
// CPlaneIntersectionType
// Geometry::PlaneIntersectionPoint( CPlane &Plane1,
// 				  CPlane &Plane2,
// 				  CPlane &Plane3,
// 				  CVector3<float> &vPoint )
// {

//   CMatrix3x3f mMatrix;
//   CPlaneIntersectionType iRetval = NO_INTERSECTION;
//   CVector3<float> vNormal;

//   vNormal.UseExternalData(Plane1.m_aValues);
//   Math::SetRowVector(mMatrix, 0, vNormal);

//   vNormal.UseExternalData(Plane2.m_aValues);
//   Math::SetRowVector(mMatrix, 1, vNormal);

//   vNormal.UseExternalData(Plane3.m_aValues);
//   Math::SetRowVector(mMatrix, 2, vNormal);

//   CVector3<float> vDs(-Plane1[CPlane::D],
// 		  -Plane2[CPlane::D],
// 		  -Plane3[CPlane::D] );

//   //CLogger::Error() << "----" << endl;
//   //CLogger::Error() << mMatrix ;
//   //CLogger::Error() << "D " << vDs << endl;

//   if ( Det(mMatrix) == 0.0f )
//   {
//     iRetval = NO_INTERSECTION;
//   } else {
//     CMatrix3x3f mInvMatrix = mMatrix.Inverse();
//     //CLogger::Error() << "inverse:\n" << mInvMatrix ;
//     vPoint = MultiplyFromRight( mInvMatrix, vDs );
//     //CLogger::Error() << "result: " << vPoint << endl;
//     iRetval = POINT;
//   }
//   //CLogger::Error() << "----" << endl;
//   return iRetval;

// }

// /////////////////////////////////////////////////////////////////



// /////////////////////////////////////////////////////////////////
// char
// Geometry::PointWithinKDOP( CVector3<float> &vPoint, CKDOP &kDop )
// {

//   std::list<CPlane>::iterator it = kDop.Planes().begin();

//   // Check the distance to each plane
//   for(;it!= kDop.Planes().end();it++)
//   {
//     // In case the point is behind some plane, it cannot be within the area
//     // limited by kDop.

//     if ( PointDistanceFromPlane( vPoint, *it ) < 0.0f){
//       return 0;
//     }
//   }

//   // point is in front of every plane, so it is inside.
//   return 1;
// }
// /////////////////////////////////////////////////////////////////
// CMatrix4x4f
// Geometry::OrientedBoxAxisToRotationMatrix ( const COrientedBox &ob )
// {

//   CMatrix4x4f mMatrix = CMatrix4x4f::Identity();

//   mMatrix(0,0) = ob.GetForwardVector().m_aValues[0];
//   mMatrix(1,0) = ob.GetForwardVector().m_aValues[1];
//   mMatrix(2,0) = ob.GetForwardVector().m_aValues[2];

//   mMatrix(0,1) = ob.GetRightVector().m_aValues[0];
//   mMatrix(1,1) = ob.GetRightVector().m_aValues[1];
//   mMatrix(2,1) = ob.GetRightVector().m_aValues[2];

//   mMatrix(0,2) = ob.GetUpVector().m_aValues[0];
//   mMatrix(1,2) = ob.GetUpVector().m_aValues[1];
//   mMatrix(2,2) = ob.GetUpVector().m_aValues[2];

//   return mMatrix;
// }
// /////////////////////////////////////////////////////////////////
// Geometry::CIntersectionType
// Geometry::SphereIntersectsKDOP( CSphere &Sphere, CKDOP &kDop )
// {
//   /////////////////////////////////////////////////////////////////
//   Geometry::CPlaneIntersection iRel;
//   float fTmp = 0.0f;
//   list<CPlane>::iterator planeIt = kDop.Planes().begin();
//   /////////////////////////////////////////////////////////////////
//   for( ;planeIt!=kDop.Planes().end(); planeIt++)
//   {
//     iRel = Geometry::SphereIntersectsPlane( *planeIt, Sphere, fTmp);
//     // If the object is behind of any of the planes,
//     // then it is outside the KDOP
//     if (iRel == Geometry::BEHIND)
//     {
//       return Geometry::OUTSIDE;
//     }
//     // If the object intersects any of the planes,
//     // then it is intersecting the frustum
//     if (iRel == Geometry::INTERSECTS)
//     {
//       return Geometry::INTERSECTION;
//     }
//   }
//   /////////////////////////////////////////////////////////////////
//   // if we end up here, the object is neither behind or intersecting of any of the
//   // planes. Hence, it is inside.
//   return INSIDE;
// }
// /////////////////////////////////////////////////////////////////
// Geometry::CIntersectionType
// Geometry::OrientedBoxIntersectsKDOP( COrientedBox &obBox, CKDOP &kDop )
// {

//   char bUseLineSegmentTest = 0;

//   // The effective radius of the box respect to a plane
//   float fEffRadius	= 0.0f;
//   float fDot		= 0.0f;
//   float fDot2		= 0.0f;

//   CVector3<float> vNormal;

//   // When box length (principal axis) is much greater than the two others,
//   // it is better to use line segment test for better accuracy.
//   bUseLineSegmentTest = (( obBox.GetLength()-obBox.GetWidth() ) > (0.5f * obBox.GetWidth())) ||
//   ( (obBox.GetLength()-obBox.GetHeight()) > (0.5f * obBox.GetHeight()));

//   if ( bUseLineSegmentTest )
//   {

//     // The line seqment
//     CVector3<float> vQ1 = obBox.GetPosition() + (obBox.GetForwardVector()*(obBox.GetLength() * 0.5f)) ;
//     CVector3<float> vQ2 = vQ1 - (obBox.GetForwardVector()*obBox.GetLength()) ;
//     list<CPlane>::iterator planeIt = kDop.Planes().begin();

//     for( ; planeIt!=kDop.Planes().end(); planeIt++)
//     {

//       vNormal.UseExternalData( (*planeIt).m_aValues);
//       // Since axes from COrientable are always Unit length,
//       // we include proper dimensions in the equation.
//       fEffRadius = 0.5f * ( fabsf( (obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal) ) +
// 			    fabsf( (obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal) ) );

//       // Calculate 4D dot product between a plane and the line endpoints
//       fDot  = vNormal.Dot( vQ1) + (*planeIt)[CPlane::D];
//       fDot2 = vNormal.Dot( vQ2) + (*planeIt)[CPlane::D];

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

//   }
//   else
//   {

//     list<CPlane>::iterator planeIt = kDop.Planes().begin();
//     for( ;planeIt!=kDop.Planes().end(); planeIt++)
//     {

//       vNormal.UseExternalData( (*planeIt).m_aValues);

//       // Since axes from COrientable are always Unit length,
//       // we include proper dimensions in the equation.

//       fEffRadius = 0.5 * ( fabsf((obBox.GetForwardVector()*obBox.GetLength()).Dot(vNormal)) +
// 			   fabsf((obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal)) +
// 			   fabsf((obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal)) );

//       // Calculate 4D dot product between plane and box center
//       fDot  = vNormal.Dot( obBox.GetPosition()) + (*planeIt)[CPlane::D];

//       if ( fDot <= -fEffRadius )
//       {
// 	return OUTSIDE;
//       }

//     }
//   }

//   return INTERSECTION;
// }
// /////////////////////////////////////////////////////////////////
