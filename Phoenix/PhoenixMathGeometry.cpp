#include "PhoenixCamera.h"
#include "PhoenixMathGeometry.h"
#include "PhoenixMath.h"
#include <math.h>
#include <algorithm>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Volume;
using namespace Phoenix::Math;
using namespace std;
/////////////////////////////////////////////////////////////////
Phoenix::Math::CPlane::CPlane()
{
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CPlane::CPlane( const CVector3<float> &vNormal, float fD ) : CVector4<float>(vNormal[0], vNormal[1], vNormal[2], fD)
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CPlane::CPlane( float fX, float fY, float fZ, float fD ) : CVector4<float>(fX,fY,fZ,fD)
{
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CPlane::~CPlane()
{
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::CPlane::SetNormal( CVector3<float> vNormal )
{
  vNormal.Normalize();
  m_pValues[0] = vNormal[0];
  m_pValues[1] = vNormal[1];
  m_pValues[2] = vNormal[2];
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CPlane::Normalize()
{
  float fLength = sqrtf( m_pValues[0] * m_pValues[0] + 
			 m_pValues[1] * m_pValues[1] + 
			 m_pValues[2] * m_pValues[2] );
  if ( fLength > 0.0f )
  {
    fLength = 1.0f / fLength;
    m_pValues[0] *= fLength;
    m_pValues[1] *= fLength;
    m_pValues[2] *= fLength;
    m_pValues[3] *= fLength;
  } 
  else
  {
    m_pValues[0] = m_pValues[1] = m_pValues[2] = 0.0f;
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::CPlane::SetDistance( float fDistance )
{
  m_pValues[3] = fDistance;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CPlane::Calculate( CVector3<float> vNormal, const CVector3<float> & vPoint )
{
  vNormal.Normalize();
  m_pValues[0] =  vNormal[0];
  m_pValues[1] =  vNormal[1];
  m_pValues[2] =  vNormal[2];
  m_pValues[3] = -(vNormal.Dot(vPoint));
}
/////////////////////////////////////////////////////////////////
std::ostream& 
Phoenix::Math::operator<<( std::ostream &stream, const CPlane &plane )
{
  stream << "(" << plane[0] << "," << plane[1] << "," << plane[2] << "," << plane[3] ;
  return stream;
}
/////////////////////////////////////////////////////////////////
std::ostream& 
Phoenix::Math::operator<<( std::ostream &stream, const CQuad & quad )
{
  stream << quad.GetPosition() << "," << quad.GetWidth() << "x" << quad.GetHeight() << endl;
  return stream;
}
/////////////////////////////////////////////////////////////////
#define APPLY_CORRECT_VALUES(R,S,T,rmax,rmin,smax, smin,tmax, tmin) {		\
 obOrientedBox[COrientedBox::FRONT].SetNormal(-R)[CPlane::D]  = rmax;	\
  obOrientedBox[COrientedBox::BACK].SetNormal(R)[CPlane::D]   = -rmin;	\
										\
  obOrientedBox[COrientedBox::RIGHT].SetNormal(-S)[CPlane::D] = smax;	\
  obOrientedBox[COrientedBox::LEFT].SetNormal(S)[CPlane::D]   = -smin;	\
										\
  obOrientedBox[COrientedBox::TOP].SetNormal(-T)[CPlane::D]   = tmax;	\
  obOrientedBox[COrientedBox::BOTTOM].SetNormal(T)[CPlane::D] = -tmin;	\
  /* The orientation will be formed so, that the forward vector will point along*/ \
  /* principal axis (largest eigenvalue ), right vector along vector with*/        \
  /* second largest and up points to vector with lowest eigenvalue*/               \
  obOrientedBox.SetOrientation(  T, R, S );                                  \
}

// /////////////////////////////////////////////////////////////////
// Phoenix::Math::CSphere 
// Phoenix::Math::CalculateBoundingSphereTight( VertexDescriptor pVertices, const CIndexBuffer &indexBuffer )
// {
//   // The returned sphere
//   CSphere sphere;

//   float fLambda1,fLambda2,fLambda3;  

//   // Calculate the principal axis R by component analysis
//   CMatrix3x3f mCovar = Math::CovarianceMatrix( pVertices, indexBuffer  );
//   CMatrix3x3f mEigenVectorMatrix = CMatrix3x3f::Identity();

//   Math::CalculateEigensystem( mCovar, 
// 			      fLambda1, fLambda2,fLambda3,
// 			      mEigenVectorMatrix);
  
//   CVector3<float> vR = GetColumnVector(mEigenVectorMatrix, 0);
//   vR.Normalize;
//   // Get min and max values of each point in direction R
//   CVector3<float> vTemp;
  
//   float fMinExtent, fMaxExtent, fTempDotR;
//   unsigned int nMaxIndex = 0, nMinIndex = 0;

//   // Initialize the first values
//   vTemp.UseExternalData( &(pVertices[indexBuffer.m_pIndices[0]]));
//   fMinExtent = fMaxExtent = vTemp.Dot(vR);
//   CVector3<float> vMin = vTemp;
//   CVector3<float> vMax = vTemp;
  
//   // for each vertex
//   for ( unsigned int i = 1;i<indexBuffer.m_nNumIndices; i++)
//   {
    
//     vTemp.UseExternalData( &(pVertices[indexBuffer.m_pIndices[i]*3]));
//     fTempDotR = vTemp.Dot( vR );

//     if ( fTempDotR > fMaxExtent ){

//       fMaxExtent = fTempDotR;
//       nMaxIndex = indexBuffer.m_pIndices[i];

//     } else if (fTempDotR < fMinExtent){

//       fMinExtent = fTempDotR;
//       nMinIndex = indexBuffer.m_pIndices[i];
//     }

//   }
//   // Assign initial center and radius
//   vMin.UseExternalData(&pVertices[nMinIndex*3]);
//   vMax.UseExternalData(&pVertices[nMaxIndex*3]);
  
//   //sphere.SetPosition( (vMin + vMax ) * 0.5f );
  
//   CVector3<float> vPos = vMin + ((vMax-vMin) * 0.5f);
//   sphere.SetPosition( vPos );
//   sphere.SetRadius((vMax - sphere.GetPosition()).Length());  
  
//   // Check each vertex that they actually are stored inside the sphere
//   float fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
  
//   // Temporary point when determining new center and radius
//   CVector3<float> vGPoint;

//   // The vector from center of the sphere to the current vertex
//   CVector3<float> vCenterToPoint;

//   // For each vertex
//   for ( unsigned int i = 0;i<indexBuffer.m_nNumIndices; i++)
//   {
    
//     vTemp.UseExternalData( &(pVertices[indexBuffer.m_pIndices[i]*3]));

//     float fDist = ((vTemp - sphere.GetPosition()).Length());
//     float fDistSquared = fDist * fDist;

//     if (  fDistSquared > fRadiusSquared )
//     {
//       // Point lies outside sphere, adjust center and radius
//       vCenterToPoint = (vTemp - sphere.GetPosition());
//       vCenterToPoint.Normalize;
//       vGPoint = sphere.GetPosition() - (sphere.GetRadius() * vCenterToPoint );
//       //sphere.SetPosition( vGPoint + (( vTemp-vGPoint) * 0.5));
//       sphere.SetPosition( (vGPoint + vTemp) * 0.5f);
//       sphere.SetRadius( (vTemp - sphere.GetPosition() ).Length());
      
//       fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
//     } 
//     else 
//     {
//       // Point within sphere, everything is good in 
//       // the hood tonite.
//     }
//   } // for each vertex
//   // Return the tighter sphere
//   return sphere;
// }
// /////////////////////////////////////////////////////////////////
// Phoenix::BoundingVolume::CAxisAlignedBox 
// Geometry::CalculateAlignedBox( VertexDescriptor pVertices, int nNumVertices ) 
// {
//   if ( nNumVertices > 1)
//   {
//     CVector3<float> vMin(pVertices[0],pVertices[1],pVertices[2]);
//     CVector3<float> vMax(pVertices[0],pVertices[1],pVertices[2]);
    
//     unsigned int nIndex = 0;
    
//     for(int v=1;v<nNumVertices;v++)
//     {
//       nIndex = (v*3);
//       STORE_MAX_MIN(pVertices[nIndex],   vMax[0], vMin[0]);
//       STORE_MAX_MIN(pVertices[nIndex+1], vMax[1], vMin[1]);
//       STORE_MAX_MIN(pVertices[nIndex+2], vMax[2], vMin[2]); 
//     }
//     CVector3<float> vDimension = (vMax - vMin);
//     CVector3<float> vCenter = vMin + vDimension*0.5f;
//     return CAxisAlignedBox( vCenter, vDimension[0], vDimension[1], vDimension[2]);
//   }
//   else
//   {
//     return CAxisAlignedBox();
//   }
// }
// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////
// CPlaneIntersection
// Geometry::SphereIntersectsPlane( CPlane &plane,
// 				 const CSphere &sphere,
// 				 float &fCenterDistance)
// {

//   CVector3<float> vNormal;
//   vNormal.UseExternalData(plane.m_pValues);
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
// //
// // As described in document:
// // http://www.geometrictools.com/Documentation/IntersectionSphereCone.pdf
// //
// /////////////////////////////////////////////////////////////////
// char
// Geometry::SphereIntersectsCone( const CSphere &sphere, const CCone &cone )
// {
//   char bRetval = 0;
// #ifndef DEBUG
//   CVector3<float> vU = cone.GetPosition() - 
//   (sphere.GetRadius()*cone.SinReciprocal())*cone.GetDirection();
  
//   CVector3<float> vD = sphere.GetPosition() - vU;
  
//   float fDsqr = vD.Dot(vD);

//   float fE = cone.GetDirection().Dot(vD);
  
//   if ( fE > 0.0f && fE*fE >= fDsqr*cone.CosSqr() )
//   {
//     vD = sphere.GetPosition() - cone.GetPosition();
//     fDsqr = vD.Dot(vD);
//     fE = -(cone.GetDirection().Dot(vD));
//     if ( fE > 0.0f && fE*fE >= fDsqr*cone.SinSqr() )
//       bRetval = (fDsqr <= sphere.GetRadiusSqr());
//     else
//       bRetval = 1;
//   } 
//   else 
//   {
//     bRetval = 0;
//   }

// #else
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
  

// #endif
//   return bRetval;
// }
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
//   if ( rPlane.m_pValues[0] < 0.0f)
//   {
//     vCornerMin[0] = rAABB.GetPosition().m_pValues[0] - fHalfWidth;
//     vCornerMax[0] = rAABB.GetPosition().m_pValues[0] + fHalfWidth;
//   }
//   else 
//   {
//     vCornerMax[0] = rAABB.GetPosition().m_pValues[0] - fHalfWidth;
//     vCornerMin[0] = rAABB.GetPosition().m_pValues[0] + fHalfWidth;
//   }
//   /////////////////////////////////////////////////////////////////
//   // Test y axis
//   if ( rPlane.m_pValues[1] < 0.0f)
//   {
//     vCornerMin[1] = rAABB.GetPosition().m_pValues[1] - fHalfHeight;
//     vCornerMax[1] = rAABB.GetPosition().m_pValues[1] + fHalfHeight;
//   }
//   else 
//   {
//     vCornerMax[1] = rAABB.GetPosition().m_pValues[1] - fHalfHeight;
//     vCornerMin[1] = rAABB.GetPosition().m_pValues[1] + fHalfHeight;
//   }
//   /////////////////////////////////////////////////////////////////
//   // Test z axis
//   if ( rPlane.m_pValues[2] < 0.0f)
//   {
//     vCornerMin[2] = rAABB.GetPosition().m_pValues[2] - fHalfLength;
//     vCornerMax[2] = rAABB.GetPosition().m_pValues[2] + fHalfLength;
//   }
//   else 
//   {
//     vCornerMax[2] = rAABB.GetPosition().m_pValues[2] - fHalfLength;
//     vCornerMin[2] = rAABB.GetPosition().m_pValues[2] + fHalfLength;
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

//   vNormal.UseExternalData(Plane1.m_pValues);
//   Math::SetRowVector(mMatrix, 0, vNormal);
  
//   vNormal.UseExternalData(Plane2.m_pValues);
//   Math::SetRowVector(mMatrix, 1, vNormal);

//   vNormal.UseExternalData(Plane3.m_pValues);
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
// int 
// Geometry::RayIntersectsOBB( const CVector3<float> &vRayStart, 
// 			    const CVector3<float> &vRayDir,
// 			    const COrientedBox &obBox,
// 			    float *pfValue )
// {
//   float fT_min,fT_max;
//   float fE, fF;
//   float f1DivF;
//   float fT_1,fT_2;
//   float fHalfValue;
//   int   bStartInsideBox = 1;
//   CVector3<float> vPoint = obBox.GetPosition() - vRayStart;
//   if ( fabs(vPoint.Dot(obBox.GetForwardVector())) > obBox.GetHalfLength()) bStartInsideBox = 0;
//   if ( fabs(vPoint.Dot(obBox.GetRightVector())  ) > obBox.GetHalfWidth())  bStartInsideBox = 0;
//   if ( fabs(vPoint.Dot(obBox.GetUpVector())     ) > obBox.GetHalfHeight()) bStartInsideBox = 0;

//   if ( bStartInsideBox ) 
//   {
//     if ( pfValue != NULL ) *pfValue = 0.0f;
//     return 1;
//   }
  
//   // for each dimension in OBB do 
//   for ( int i=0;i<3;i++)
//   {
//     // Get correct values for each loop
//     switch ( i )
//     {
//     case 0:
//       fE = obBox.GetForwardVector().Dot( vPoint );
//       fF = obBox.GetForwardVector().Dot( vRayDir );
//       fHalfValue = obBox.GetHalfLength();
//       break;
//     case 1:
//       fE = obBox.GetRightVector().Dot( vPoint );
//       fF = obBox.GetRightVector().Dot( vRayDir );
//       fHalfValue = obBox.GetHalfWidth();
//       break;
//     default:
//       fE = obBox.GetUpVector().Dot( vPoint );
//       fF = obBox.GetUpVector().Dot( vRayDir );
//       fHalfValue = obBox.GetHalfHeight();
//     }
    
//     if ( TOO_CLOSE_TO_ZERO(fF) )
//     {
//       if (-fE - fHalfValue > 0.0f || -fE + fHalfValue < 0.0f )
//       {
// 	return 0;
//       }
//     }
//     else 
//     { 
//       f1DivF = 1.0f / fF;
//       fT_1  = (fE + fHalfValue)*f1DivF;
//       fT_2  = (fE - fHalfValue)*f1DivF;

//       // Put values in correct order
//       if ( fT_1 > fT_2 ) 
//       {
// 	float fTmp = fT_1;
// 	fT_1 = fT_2;
// 	fT_2 = fTmp;
//       }

//       /// Min/max comparison, first loop initializes values,
//       if ( i == 0 )
//       {
// 	fT_min = fT_1;
// 	fT_max = fT_2;
//       } 
//       else
//       {
// 	if ( fT_1 > fT_min) fT_min = fT_1;
// 	if ( fT_2 < fT_max) fT_max = fT_2;
//       }
//       // Perform tests, and exit on non-intersection.
//       if ( fT_min > fT_max ) return 0;
//       if ( fT_max < 0.0f   ) return 0;
    
//     } // close to zero 
//   } // for

//   ////////////////////
//   /// Set distance from origin to closest intersection point 
//   if ( pfValue != NULL ) 
//   {
//     if ( fT_min > 0.0f) *pfValue = fT_min;
//     else                *pfValue = fT_max;
//   }    
//   // return intersection
//   return 1;

// }


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

//   mMatrix(0,0) = ob.GetForwardVector().m_pValues[0];
//   mMatrix(1,0) = ob.GetForwardVector().m_pValues[1];
//   mMatrix(2,0) = ob.GetForwardVector().m_pValues[2];

//   mMatrix(0,1) = ob.GetRightVector().m_pValues[0];
//   mMatrix(1,1) = ob.GetRightVector().m_pValues[1];
//   mMatrix(2,1) = ob.GetRightVector().m_pValues[2];

//   mMatrix(0,2) = ob.GetUpVector().m_pValues[0];
//   mMatrix(1,2) = ob.GetUpVector().m_pValues[1];
//   mMatrix(2,2) = ob.GetUpVector().m_pValues[2];
  
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
      
//       vNormal.UseExternalData( (*planeIt).m_pValues);
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
      
//       vNormal.UseExternalData( (*planeIt).m_pValues);

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
