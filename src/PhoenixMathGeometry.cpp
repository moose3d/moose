#include "PhoenixCamera.h"
#include "PhoenixMathGeometry.h"
#include "PhoenixMath.h"
#include <math.h>
#include <algorithm>
/////////////////////////////////////////////////////////////////
Phoenix::Math::CPlane::CPlane()
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
  m_pValues[0] = vNormal(0);
  m_pValues[1] = vNormal(1);
  m_pValues[2] = vNormal(2);
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
    m_pValues[0] = m_pValues[1] = m_pValues[2] = m_pValues[3] = 0.0f;
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
  m_pValues[0] =  vNormal(0);
  m_pValues[1] =  vNormal(1);
  m_pValues[2] =  vNormal(2);
  m_pValues[3] = -(vNormal.Dot(vPoint));
}
/////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream &stream, CPlane plane )
{
  stream << "(" << plane(0) << "," << plane(1) << "," << plane(2) << "," << plane(3) ;
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
// Phoenix::Volume::COrientedBox 
// Phoenix::Math::CalculateOrientedBoundingBox( VertexDescriptor pVertices, unsigned int nNumVertices )
// {
//   /////////////////////////////////////////////////////////////////  
//   float fMaxR = 0.0f;
//   float fMinR = 0.0f;
//   float fMaxS = 0.0f;
//   float fMinS = 0.0f;
//   float fMaxT = 0.0f;
//   float fMinT = 0.0f;
//   float fLambda1,fLambda2,fLambda3;  
//   float fVertDotR = 0.0f;
//   float fVertDotS = 0.0f;
//   float fVertDotT = 0.0f;
//   CVector3<float>	 vTemp;
//   /////////////////////////////////////////////////////////////////
//   CMatrix3x3f mCovar = Math::CovarianceMatrix( pVertices, nNumVertices  );
//   CMatrix3x3f mEigenVectorMatrix = CMatrix3x3f::Identity();
//   Math::CalculateEigensystem( mCovar, fLambda1, fLambda2,fLambda3,
// 			      mEigenVectorMatrix);
//   /////////////////////////////////////////////////////////////////
//   CVector3<float> vR = Math::GetColumnVector(mEigenVectorMatrix, 0);
//   CVector3<float> vS = Math::GetColumnVector(mEigenVectorMatrix, 1);
//   CVector3<float> vT = Math::GetColumnVector(mEigenVectorMatrix, 2);

//   /////////////////////////////////////////////////////////////////
//   vR.Normalize;
//   vS.Normalize;
//   vT.Normalize;
//   /////////////////////////////////////////////////////////////////
//   for ( unsigned int v = 0;v<nNumVertices;v++)
//   {

//     vTemp.UseExternalData(&(pVertices[v*3]));
    
//     fVertDotR = vTemp.Dot(vR);
//     fVertDotS = vTemp.Dot(vS);
//     fVertDotT = vTemp.Dot(vT);
    
//     if ( v==0 )
//     {
      
//       fMaxR = fMinR = fVertDotR;
//       fMaxS = fMinS = fVertDotS;
//       fMaxT = fMinT = fVertDotT;
      
//     } else {
      
//       if ( fVertDotR < fMinR )		fMinR = fVertDotR;
//       else if ( fVertDotR > fMaxR )	fMaxR = fVertDotR;
      
//       if ( fVertDotS < fMinS )		fMinS = fVertDotS;
//       else if ( fVertDotS > fMaxS )	fMaxS = fVertDotS;
      
//       if ( fVertDotT < fMinT )		fMinT = fVertDotT;
//       else if ( fVertDotT > fMaxT )	fMaxT = fVertDotT;
      
//     }
//   }
  
//   COrientedBox obOrientedBox;
// #ifdef DEBUG
//   CLogger::Error() << DEBUG_HEADER << "minR " << fMinR << endl;
//   CLogger::Error() << DEBUG_HEADER << "maxR " << fMaxR << endl;
//   CLogger::Error() << DEBUG_HEADER << "minS " << fMinS << endl;
//   CLogger::Error() << DEBUG_HEADER << "maxS " << fMaxS << endl;
//   CLogger::Error() << DEBUG_HEADER << "minT " << fMinT << endl;
//   CLogger::Error() << DEBUG_HEADER << "maxT " << fMaxT << endl;
// #endif
  

//   float fRlen = fMaxR - fMinR;
//   float fSlen = fMaxS - fMinS;
//   float fTlen = fMaxT - fMinT;
  
//   float fA = (fMaxR + fMinR) * 0.5f;
//   float fB = (fMaxS + fMinS) * 0.5f;
//   float fC = (fMaxT + fMinT) * 0.5f;

//   /// assign largest eigenvector to vR, second largest to vS and smallest to vT.
//   if ( fRlen > fSlen ) 
//   {
//     /// fLambda1 > fLambda2 !!! 
//     if ( fSlen > fTlen  )
//     {
//       APPLY_CORRECT_VALUES( vR, vS, vT, fMaxR, fMinR, fMaxS, fMinS, fMaxT, fMinT );
//       obOrientedBox.SetLength( fRlen );
//       obOrientedBox.SetWidth(  fSlen );
//       obOrientedBox.SetHeight( fTlen );
//     } 
//     else
//     {
//       if ( fRlen > fTlen )
//       {
// 	APPLY_CORRECT_VALUES( vR, vT, vS, fMaxR, fMinR, fMaxT, fMinT,fMaxS, fMinS );
// 	obOrientedBox.SetLength( fRlen );
// 	obOrientedBox.SetWidth(  fTlen );
// 	obOrientedBox.SetHeight( fSlen );
//       }
//       else
//       {
// 	APPLY_CORRECT_VALUES( vT, vR, vS, fMaxT, fMinT, fMaxR, fMinR, fMaxS, fMinS );
// 	obOrientedBox.SetLength( fTlen );
// 	obOrientedBox.SetWidth(  fRlen );
// 	obOrientedBox.SetHeight( fSlen );
//       }
//     }
//   }
//   else 
//   {
//     /// fLambda1 < fLambda2 !!! 
    
//     if ( fSlen < fTlen )
//     {
//       APPLY_CORRECT_VALUES( vT, vS, vR, fMaxT, fMinT, fMaxS, fMinS, fMaxR, fMinR  );
//       obOrientedBox.SetLength( fTlen );
//       obOrientedBox.SetWidth(  fSlen );
//       obOrientedBox.SetHeight( fRlen );
//     }
//     else
//     {
//       /// fLambda3 < fLambda2

//       if ( fRlen < fTlen )
//       {
// 	APPLY_CORRECT_VALUES( vS, vT, vR, fMaxS, fMinS, fMaxT, fMinT, fMaxR, fMinR  );
// 	obOrientedBox.SetLength( fSlen );
// 	obOrientedBox.SetWidth(  fTlen );
// 	obOrientedBox.SetHeight( fRlen );
//       }
//       else 
//       {
// 	APPLY_CORRECT_VALUES( vS, vR, vT, fMaxS, fMinS, fMaxR, fMinR, fMaxT, fMinT  );
// 	obOrientedBox.SetLength( fSlen );
// 	obOrientedBox.SetWidth(  fRlen );
// 	obOrientedBox.SetHeight( fTlen );

//       }
//     }
//   }
  
//   CVector3<float> vPos = (fA*vR) + (fB*vS) + (fC*vT);
//   obOrientedBox.SetPosition( vPos );
//   //obOrientedBox.CalculatePlanes();  
//   obOrientedBox.CalculateCorners();

//   return obOrientedBox;
  
// }
// /////////////////////////////////////////////////////////////////
// Phoenix::Math::COrientedBox 
// Phoenix::Math::CalculateOrientedBoundingBox( VertexDescriptor pVertices, const CIndexBuffer &indexBuffer )
// {
//   /////////////////////////////////////////////////////////////////
//   // Variables used
//   float fMaxR = 0.0f;
//   float fMinR = 0.0f;
//   float fMaxS = 0.0f;
//   float fMinS = 0.0f;
//   float fMaxT = 0.0f;
//   float fMinT = 0.0f;
//   float fLambda1,fLambda2,fLambda3;  
//   float fVertDotR = 0.0f;
//   float fVertDotS = 0.0f;
//   float fVertDotT = 0.0f;
//   CVector3<float>	 vTemp;
//   /////////////////////////////////////////////////////////////////
//   CMatrix3x3f mCovar = Math::CovarianceMatrix( pVertices, indexBuffer );
//   CMatrix3x3f mEigenVectorMatrix = CMatrix3x3f::Identity();
//   /////////////////////////////////////////////////////////////////
//   Math::CalculateEigensystem( mCovar, fLambda1, fLambda2,fLambda3,
// 			      mEigenVectorMatrix);
//   /////////////////////////////////////////////////////////////////  
//   CVector3<float> vR = GetColumnVector(mEigenVectorMatrix, 0);
//   CVector3<float> vS = GetColumnVector(mEigenVectorMatrix, 1);
//   CVector3<float> vT = GetColumnVector(mEigenVectorMatrix, 2);
//   /////////////////////////////////////////////////////////////////
//   vR.Normalize;
//   vS.Normalize;
//   vT.Normalize;
//   /////////////////////////////////////////////////////////////////
//   for ( unsigned int i = 0;i<indexBuffer.m_nNumIndices;i++)
//   {

//     vTemp.UseExternalData(&(pVertices[indexBuffer.m_pIndices[i]*3]));

//     fVertDotR = vTemp.Dot(vR);
//     fVertDotS = vTemp.Dot(vS);
//     fVertDotT = vTemp.Dot(vT);
    
//     if ( i==0){
      
//       fMaxR = fMinR = fVertDotR;
//       fMaxS = fMinS = fVertDotS;
//       fMaxT = fMinT = fVertDotT;
      
//     } 
//     else 
//     {
      
//       if ( fVertDotR < fMinR )		fMinR = fVertDotR;
//       else if ( fVertDotR > fMaxR )	fMaxR = fVertDotR;
      
//       if ( fVertDotS < fMinS )		fMinS = fVertDotS;
//       else if ( fVertDotS > fMaxS )	fMaxS = fVertDotS;
      
//       if ( fVertDotT < fMinT )		fMinT = fVertDotT;
//       else if ( fVertDotT > fMaxT )	fMaxT = fVertDotT;
      
//     }
//   }
//   /////////////////////////////////////////////////////////////////
//   COrientedBox obOrientedBox;
// #ifdef DEBUG
//   CLogger::Error() << DEBUG_HEADER << "minR " << fMinR << endl;
//   CLogger::Error() << DEBUG_HEADER << "maxR " << fMaxR << endl;
//   CLogger::Error() << DEBUG_HEADER << "minS " << fMinS << endl;
//   CLogger::Error() << DEBUG_HEADER << "maxS " << fMaxS << endl;
//   CLogger::Error() << DEBUG_HEADER << "minT " << fMinT << endl;
//   CLogger::Error() << DEBUG_HEADER << "maxT " << fMaxT << endl;
// #endif

//   float fRlen = fMaxR - fMinR;
//   float fSlen = fMaxS - fMinS;
//   float fTlen = fMaxT - fMinT;
  
//   float fA = (fMaxR + fMinR) * 0.5f;
//   float fB = (fMaxS + fMinS) * 0.5f;
//   float fC = (fMaxT + fMinT) * 0.5f;


//   /// assign largest eigenvector to vR, second largest to vS and smallest to vT.
//   if ( fRlen > fSlen ) 
//   {
//     /// fLambda1 > fLambda2 !!! 
//     if ( fSlen > fTlen  )
//     {
//       APPLY_CORRECT_VALUES( vR, vS, vT, fMaxR, fMinR, fMaxS, fMinS, fMaxT, fMinT );
//       obOrientedBox.SetLength( fRlen );
//       obOrientedBox.SetWidth(  fSlen );
//       obOrientedBox.SetHeight( fTlen );
//     } 
//     else
//     {
//       if ( fRlen > fTlen )
//       {
// 	APPLY_CORRECT_VALUES( vR, vT, vS, fMaxR, fMinR, fMaxT, fMinT,fMaxS, fMinS );
// 	obOrientedBox.SetLength( fRlen );
// 	obOrientedBox.SetWidth(  fTlen );
// 	obOrientedBox.SetHeight( fSlen );
//       }
//       else
//       {
// 	APPLY_CORRECT_VALUES( vT, vR, vS, fMaxT, fMinT, fMaxR, fMinR, fMaxS, fMinS );
// 	obOrientedBox.SetLength( fTlen );
// 	obOrientedBox.SetWidth(  fRlen );
// 	obOrientedBox.SetHeight( fSlen );
//       }
//     }
//   }
//   else 
//   {
//     /// fLambda1 < fLambda2 !!! 
    
//     if ( fSlen < fTlen )
//     {
//       APPLY_CORRECT_VALUES( vT, vS, vR, fMaxT, fMinT, fMaxS, fMinS, fMaxR, fMinR  );
//       obOrientedBox.SetLength( fTlen );
//       obOrientedBox.SetWidth(  fSlen );
//       obOrientedBox.SetHeight( fRlen );
//     }
//     else
//     {
//       /// fLambda3 < fLambda2

//       if ( fRlen < fTlen )
//       {
// 	APPLY_CORRECT_VALUES( vS, vT, vR, fMaxS, fMinS, fMaxT, fMinT, fMaxR, fMinR  );
// 	obOrientedBox.SetLength( fSlen );
// 	obOrientedBox.SetWidth(  fTlen );
// 	obOrientedBox.SetHeight( fRlen );
//       }
//       else 
//       {
// 	APPLY_CORRECT_VALUES( vS, vR, vT, fMaxS, fMinS, fMaxR, fMinR, fMaxT, fMinT  );
// 	obOrientedBox.SetLength( fSlen );
// 	obOrientedBox.SetWidth(  fRlen );
// 	obOrientedBox.SetHeight( fTlen );

//       }
//     }
//   }

//   /////////////////////////////////////////////////////////////////
//   // The position
//   CVector3<float> vPos = (fA*vR) + (fB*vS) + (fC*vT);
//   obOrientedBox.SetPosition( vPos );
//   /////////////////////////////////////////////////////////////////
//   // The orientation will be formed so, that the forward vector will point along
//   // principal axis (largest eigenvalue ), right vector along vector with 
//   // second largest and up points to vector with lowest eigenvalue
//   ///obOrientedBox.SetOrientation(  vT, vR, vS );
//   //obOrientedBox.CalculatePlanes();
//   obOrientedBox.CalculateCorners();
  
//   /////////////////////////////////////////////////////////////////
//   return obOrientedBox;
  
// }
// /////////////////////////////////////////////////////////////////
// Phoenix::Math::CSphere
// Phoenix::Math::CalculateBoundingSphere( VertexDescriptor pVertices, unsigned int nNumVertices)
// {
//   // Minimum and maximum values for each coordinates.
//   CVector3<float> vMaxValues;
//   CVector3<float> vMinValues;

//   for ( unsigned int v = 0;v<nNumVertices; v++)
//   {
//     // 
//     float fTempX = pVertices[(v*3)];
//     float fTempY = pVertices[(v*3)+1];
//     float fTempZ = pVertices[(v*3)+2];

//     if ( v == 0 ){

//       vMaxValues[0] = vMinValues[0] = fTempX;
//       vMaxValues[1] = vMinValues[1] = fTempY;
//       vMaxValues[2] = vMinValues[2] = fTempZ;

//     } else {
      
//       STORE_MAX_MIN(fTempX, vMaxValues[0], vMinValues[0]);
//       STORE_MAX_MIN(fTempY, vMaxValues[1], vMinValues[1]);
//       STORE_MAX_MIN(fTempZ, vMaxValues[1], vMinValues[1]);
//     }
//   }

//   CSphere sphere;
//   sphere.SetPosition( (vMaxValues + vMinValues ) / 2 );
//   CVector3<float> vDist = vMaxValues - vMinValues;
  
//   float fMaxRadius = 0.0; 

//   if ( fabs(vDist[0]) > fabs(vDist[1])){

//     if ( fabs(vDist[0] > fabs(vDist[2]))){
//       fMaxRadius = vDist[0] * 0.5f;
//     } else {
//       fMaxRadius = vDist[2] * 0.5f;
//     }

//   } else {

//     if ( fabs(vDist[1] > fabs(vDist[2]))){
//       fMaxRadius = vDist[1] * 0.5f;
//     } else {
//       fMaxRadius = vDist[2] * 0.5f;
//     }

//   }
//   sphere.SetRadius( fMaxRadius  );
//   return sphere;
// }

// /////////////////////////////////////////////////////////////////
// Phoenix::Math::CSphere 
// Phoenix::Math::CalculateBoundingSphereTight( VertexDescriptor pVertices, unsigned int nNumVertices )
// {
//   // The returned sphere
//   CSphere sphere;

//   float fLambda1,fLambda2,fLambda3;  

//   // Calculate the principal axis R by component analysis
//   CMatrix3x3f mCovar = Math::CovarianceMatrix( pVertices, nNumVertices  );
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
//   vTemp.UseExternalData( &(pVertices[0]));
//   fMinExtent = fMaxExtent = vTemp.Dot(vR);
//   CVector3<float> vMin = vTemp;
//   CVector3<float> vMax = vTemp;
  
//   // for each vertex
//   for ( unsigned int v = 1;v<nNumVertices; v++)
//   {
    
//     vTemp.UseExternalData( &(pVertices[v*3]));
//     fTempDotR = vTemp.Dot( vR );

//     if ( fTempDotR > fMaxExtent ){

//       fMaxExtent = fTempDotR;
//       nMaxIndex = v;

//     } else if (fTempDotR < fMinExtent){

//       fMinExtent = fTempDotR;
//       nMinIndex = v;
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
//   for ( unsigned int v = 0;v<nNumVertices; v++)
//   {
    
//     vTemp.UseExternalData( &(pVertices[v*3]));

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
// char 
// Geometry::SphereIntersectsSphere( const CSphere &sphereOne, const CSphere &sphereTwo )
// {
//   // Get the separating axis
//   CVector3<float> vSepAxis = sphereOne.GetPosition() - sphereTwo.GetPosition();

//   // Get the sum of the radii
//   float fSumOfRadii = sphereOne.GetRadius() + sphereTwo.GetRadius();
//   fSumOfRadii = fSumOfRadii * fSumOfRadii;
//   // if the distance between the centers is less than the sum
//   // of the radii, then we have an intersection
//   return (vSepAxis.LengthSqr() < fSumOfRadii);

// }
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
// int
// Geometry::SphereIntersectsAxisAlignedBox( const CSphere &sphere, 
// 					  const CAxisAlignedBox &aaBox)
// {
//   float fDistance = 0.0f;
//   float fTmp = 0.0f;
//   /////////////////////////////////////////////////////////////////
//   // Test for X axis
//   float fAxisMax = aaBox.GetPosition().m_pValues[0] + aaBox.GetWidth();
//   float fAxisMin = aaBox.GetPosition().m_pValues[0] - aaBox.GetWidth();
//   float fAxisCenter = sphere.GetPosition().m_pValues[0];

//   if ( fAxisCenter < fAxisMin )
//   {
//     fTmp = fAxisCenter - fAxisMin;
//     fDistance += ( fTmp * fTmp );
//   } 
//   else if ( fAxisCenter > fAxisMax )
//   {
//     fTmp =  fAxisCenter - fAxisMax;
//     fDistance += ( fTmp * fTmp );
//   }
//   /////////////////////////////////////////////////////////////////
//   // Test for Y axis
//   fAxisMax = aaBox.GetPosition().m_pValues[1] + aaBox.GetHeight();
//   fAxisMin = aaBox.GetPosition().m_pValues[1] - aaBox.GetHeight();
//   fAxisCenter = sphere.GetPosition().m_pValues[1];
//   if ( fAxisCenter < fAxisMin )
//   {
//     fTmp = fAxisCenter - fAxisMin;
//     fDistance += ( fTmp * fTmp );
//   } 
//   else if ( fAxisCenter > fAxisMax )
//   {
//     fTmp =  fAxisCenter - fAxisMax;
//     fDistance += ( fTmp * fTmp );
//   }
//   /////////////////////////////////////////////////////////////////
//   // Test for Z axis
//   fAxisMax = aaBox.GetPosition().m_pValues[2] + aaBox.GetLength();
//   fAxisMin = aaBox.GetPosition().m_pValues[2] - aaBox.GetLength();
//   fAxisCenter = sphere.GetPosition().m_pValues[2];
//   if ( fAxisCenter < fAxisMin )
//   {
//     fTmp = fAxisCenter - fAxisMin;
//     fDistance += ( fTmp * fTmp );
//   } 
//   else if ( fAxisCenter > fAxisMax )
//   {
//     fTmp =  fAxisCenter - fAxisMax;
//     fDistance += ( fTmp * fTmp );
//   }
//   /////////////////////////////////////////////////////////////////
//   // Actual test, if the sum of squared distances is less than the squared
//   // radius, we have overlapping.
//   if ( fDistance > sphere.GetRadiusSqr()) 
//   {
//     return 0;
//   }
//   else 
//   {
//     return 1;
//   }
// }
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
// CSphere
// Geometry::MergeSpheres( const CSphere &sphereOne, const CSphere &sphereTwo)
// {
  
//   CVector3<float>	vCenterDiff = sphereTwo.GetPosition() - sphereOne.GetPosition();
//   float		fCenterDiffLen  = 0.0f;
//   float		fRadiusDiff = sphereTwo.GetRadius() - sphereOne.GetRadius();
//   float		fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
//   float		fLengthSqr     = vCenterDiff.LengthSqr();

//   int bSphereOneValid = sphereOne.GetRadius() > 0.00001f;
//   int bSphereTwoValid = sphereTwo.GetRadius() > 0.00001f;
//   if (  bSphereOneValid && !bSphereTwoValid ) return sphereOne;
//   if ( !bSphereOneValid &&  bSphereTwoValid ) return sphereTwo;

//   if ( fRadiusDiffSqr >= fLengthSqr )
//   {

//     if ( fRadiusDiff >= 0.0f ) // Sphere two contains the sphere one
//     {     
//       return sphereTwo;
//     } 
//     else // Sphere one contains the sphere two
//     {			
//       return sphereOne;
//     }

//   } else {

//     fCenterDiffLen = sqrt( fLengthSqr );
//     float fT = (fCenterDiffLen + sphereTwo.GetRadius() - 
// 		sphereOne.GetRadius()) /(2.0f * fCenterDiffLen);
    
//     return CSphere( sphereOne.GetPosition() + (fT * vCenterDiff),
// 		       (fCenterDiffLen + sphereTwo.GetRadius() + sphereOne.GetRadius())*0.5f);
//   }
// }
// /////////////////////////////////////////////////////////////////
// int
// Geometry::PointIntersectsOBB( const COrientedBox &obBox, 
// 			      const CVector3<float> &vPoint )
// {
  
//   CVector3<float> vPos = vPoint - obBox.GetPosition();
//   if ( fabs(vPos.Dot(obBox.GetForwardVector())) > obBox.GetHalfLength()) return 0;
//   if ( fabs(vPos.Dot(obBox.GetRightVector())) > obBox.GetHalfWidth()) return 0;
//   if ( fabs(vPos.Dot(obBox.GetUpVector())) > obBox.GetHalfHeight()) return 0;
//   return 1;
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
// COrientedBox 
// Geometry::MergeOrientedBoxes( const COrientedBox &obOne, const COrientedBox &obTwo )
// {
//   unsigned int bObOneValid =  (obOne.GetLength() * obOne.GetWidth() * obOne.GetHeight()) > 0.0001f;
//   unsigned int bObTwoValid =  (obTwo.GetLength() * obTwo.GetWidth() * obTwo.GetHeight()) > 0.0001f;
//   if (  bObOneValid && !bObTwoValid ) return obOne;
//   if ( !bObOneValid && bObTwoValid ) return obTwo;
//   if ( !bObOneValid && !bObTwoValid) return COrientedBox();

// #define OPTIMAL_BOUNDING_BOX_MERGE
// #ifdef OPTIMAL_BOUNDING_BOX_MERGE
  
//   COrientedBox obMerge;
//   CVector3<float> vDir = obTwo.GetPosition() - obOne.GetPosition();
//   vDir.Normalize;

//   CVector3<float> vCorner;
//   CVector3<float> vCornerOnLine;
//   CVector3<float> vMin(0,0,0);
//   CVector3<float> vMax(0,0,0);
//   CVector3<float> vSecondaryAxis;
//   CVector3<float> vSecAxisCandidate;

//   float fDot;
//   float fLength;
  
//   int i;
//   // For each corner in box one 
//   for (i = 0; i < 8; i++)
//   {
//     // Calculate the length on initial axis
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obOne).GetCorner( (COrientedBox::BoxCorner_t)i));
//     fDot = vDir.Dot(vCorner - obOne.GetPosition());

//     // calculate possible secondary axis.
//     vCornerOnLine = obOne.GetPosition() + vDir * fDot;
//     vSecAxisCandidate = vCorner - vCornerOnLine;
//     fLength = vSecAxisCandidate.Length();
    
//     /// 
//     if (fDot > vMax[0])  {     vMax[0] = fDot;  }
//     else if (fDot < vMin[0]){  vMin[0] = fDot;  }
    
//     // Since this is a symmetric box, we need to check only for 
//     // max, since width becomes 2 * max.
//     if ( fLength > vMax[1] ) 
//     { 
//       vMax[1]  = fLength; 
//       vSecondaryAxis = vSecAxisCandidate;
//     }
//   }
  
//   // For each corner in box two
//   for (i = 0; i < 8; i++)
//   {
//     // Calculate the length on initial axis
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obTwo).GetCorner( (COrientedBox::BoxCorner_t)i));
//     fDot = vDir.Dot(vCorner - obOne.GetPosition());

//     // calculate possible secondary axis.
//     vCornerOnLine = obOne.GetPosition() + vDir * fDot;
//     vSecAxisCandidate = vCorner - vCornerOnLine;
//     fLength = vSecAxisCandidate.Length();
    
//     /// 
//     if (fDot > vMax[0])  {     vMax[0] = fDot;  }
//     else if (fDot < vMin[0]){  vMin[0] = fDot;  }
    
//     // Since this is a symmetric box, we need to check only for 
//     // max, since width becomes 2 * max.
//     if ( fLength > vMax[1] ) 
//     { 
//       vMax[1]  = fLength; 
//       vSecondaryAxis = vSecAxisCandidate;
//     }
//   }
//   vMin[1] = -vMax[1];
//   /////////////////////////////////////////////////////////////////
//   // At this point, we have initial and secondary axis with extents.
//   CVector3<float> vTertiaryAxis = vSecondaryAxis.Cross(vDir);
//   vTertiaryAxis.Normalize;
//   vSecondaryAxis.Normalize;
//   fDot = 0.0f;
//   /////////////////////////////////////////////////////////////////
//   // For each corner in box one 
//   for (i = 0; i < 8; i++)
//   {
//     // Calculate the length on initial axis
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obOne).GetCorner( (COrientedBox::BoxCorner_t)i));
//     fDot = vTertiaryAxis.Dot(vCorner - obOne.GetPosition());
//     if (fDot > vMax[2])     {  vMax[2] = fDot;  }

//   }
  
//   // For each corner in box two
//   for (i = 0; i < 8; i++)
//   {
//     // Calculate the length on initial axis
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obTwo).GetCorner( (COrientedBox::BoxCorner_t)i));
//     fDot = vTertiaryAxis.Dot(vCorner - obOne.GetPosition());
//     if (fDot > vMax[2])     {  vMax[2] = fDot;  }
    
//   }
//   vMin[2] = -vMax[2];
  
//   CVector3<float> vExtents(vMax - vMin);
  
//   obMerge.SetPosition( ((obOne.GetPosition() + vDir * vMax[0]) +  
// 			(obOne.GetPosition() + vDir * vMin[0])) *0.5f)  ;

//   obMerge.SetOrientation( vTertiaryAxis, vDir,  vSecondaryAxis );
//   obMerge.SetLength( vExtents[0] );
//   obMerge.SetWidth( vExtents[1] );
//   obMerge.SetHeight( vExtents[2] );
//   obMerge.CalculateCorners();
//   obMerge.CalculatePlanes();
//   return obMerge;
// #else
//   // construct a box that contains the input boxes
//   COrientedBox obMerge;

//   // The first guess at the box center.  This value will be updated later
//   // after the input box vertices are projected onto axes determined by an
//   // average of box axes.
//   obMerge.SetPosition( 0.5f*(obOne.GetPosition() + obTwo.GetPosition()));
  
//   // A box's axes, when viewed as the columns of a matrix, form a rotation
//   // matrix.  The input box axes are converted to quaternions.  The average
//   // quaternion is computed, then normalized to unit length.  The result is
//   // the slerp of the two input quaternions with t-value of 1/2.  The result
//   // is converted back to a rotation matrix and its columns are selected as
//   // the merged box axes.
//   CQuaternion qOne, qTwo;
  
//   CMatrix4x4f mOne = OrientedBoxAxisToRotationMatrix( obOne );
//   CMatrix4x4f mTwo = OrientedBoxAxisToRotationMatrix( obTwo );

//   qOne = RotationMatrixToQuaternion( mOne );
//   qTwo = RotationMatrixToQuaternion( mTwo );

//   if ( qOne.Dot(qTwo) < 0.0f) 
//   {
//     qTwo = -qTwo;
//   }

//   CQuaternion  qQuat = qOne + qTwo;
//   qQuat.Normalize();

//   CMatrix4x4f mFinal = qQuat.ToMatrix();

//   CVector3<float> vUp(mFinal(0,2), mFinal(1,2), mFinal(2,2) );
//   CVector3<float> vForward(mFinal(0,0), mFinal(1,0), mFinal(2,0) );
//   CVector3<float> vRight(mFinal(0,1), mFinal(1,1), mFinal(2,1) );
  
//   obMerge.SetOrientation( vUp, vForward, vRight);

//   // Project the input box vertices onto the merged-box axes.  Each axis
//   // D[i] containing the current center C has a minimum projected value
//   // pmin[i] and a maximum projected value pmax[i].  The corresponding end
//   // points on the axes are C+pmin[i]*D[i] and C+pmax[i]*D[i].  The point C
//   // is not necessarily the midpoint for any of the intervals.  The actual
//   // box center will be adjusted from C to a point C' that is the midpoint
//   // of each interval,
//   //   C' = C + sum_{i=0}^2 0.5*(pmin[i]+pmax[i])*D[i]
//   // The box extents are
//   //   e[i] = 0.5*(pmax[i]-pmin[i])

//   int i = 0;
//   float fDot = 0.0f;
  
//   CVector3<float> vCorner, vDiff;
//   CVector3<float> vMin(0,0,0);
//   CVector3<float> vMax(0,0,0);
  
//   // For each corner in box one 
//   for (i = 0; i < 8; i++)
//   {
//     // Forward axis
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obOne).GetCorner( (COrientedBox::BoxCorner_t)i));
//     vDiff = vCorner - obMerge.GetPosition();
//     fDot = vDiff.Dot( obMerge.GetForwardVector() );
//     if (fDot > vMax[0])  {     vMax[0] = fDot;    }
//     else if (fDot < vMin[0]){    vMin[0] = fDot;  }

//     // right axis 
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obOne).GetCorner( (COrientedBox::BoxCorner_t)i));
//     vDiff = vCorner - obMerge.GetPosition();
//     fDot = vDiff.Dot( obMerge.GetRightVector() );
//     if (fDot > vMax[1])  {     vMax[1] = fDot;    }
//     else if (fDot < vMin[1]){    vMin[1] = fDot;  }

//     // Up axis 
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obOne).GetCorner( (COrientedBox::BoxCorner_t)i));
//     vDiff = vCorner - obMerge.GetPosition();
//     fDot = vDiff.Dot( obMerge.GetUpVector() );
//     if (fDot > vMax[2])  {     vMax[2] = fDot;    }
//     else if (fDot < vMin[2]){    vMin[2] = fDot;  }
    
//   }
  
//   // For each corner in box two
//   for (i = 0; i < 8; i++)
//   {
//     // Forward axis
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obTwo).GetCorner( (COrientedBox::BoxCorner_t)i));
//     vDiff = vCorner - obMerge.GetPosition();
//     fDot = vDiff.Dot( obMerge.GetForwardVector() );
//     if (fDot > vMax[0])  {     vMax[0] = fDot; }
//     else if (fDot < vMin[0]){  vMin[0] = fDot; }

//     // right axis 
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obTwo).GetCorner( (COrientedBox::BoxCorner_t)i));
//     vDiff = vCorner - obMerge.GetPosition();
//     fDot = vDiff.Dot( obMerge.GetRightVector() );
//     if (fDot > vMax[1])  {     vMax[1] = fDot; }
//     else if (fDot < vMin[1]){  vMin[1] = fDot; }

//     // Up axis 
//     vCorner.UseExternalData( const_cast<COrientedBox &>(obTwo).GetCorner( (COrientedBox::BoxCorner_t)i));
//     vDiff = vCorner - obMerge.GetPosition();
//     fDot = vDiff.Dot( obMerge.GetUpVector() );
//     if (fDot > vMax[2])  {     vMax[2] = fDot; }
//     else if (fDot < vMin[2]){  vMin[2] = fDot; }
    
//   }
  
//   // [kMin,kMax] is the axis-aligned box in the coordinate system of the
//   // merged box axes.  Update the current box center to be the center of
//   // the new box.  Compute the extent based on the new center.
  
//   CVector3<float> vMean    = (vMax + vMin) * 0.5f;
//   CVector3<float> vExtents = (vMax - vMin );
  
//   obMerge.SetPosition( obMerge.GetPosition() + (vMean[0] *obMerge.GetForwardVector()));
//   obMerge.SetLength( vExtents[0] );
  
//   obMerge.SetPosition( obMerge.GetPosition() + (vMean[1]*obMerge.GetRightVector()));
//   obMerge.SetWidth( vExtents[1] );
  
//   obMerge.SetPosition( obMerge.GetPosition() + (vMean[2]*obMerge.GetUpVector()));
//   obMerge.SetHeight( vExtents[2] );

//   obMerge.CalculatePlanes();
//   obMerge.CalculateCorners();
//   return obMerge;
// #endif
// }
// /////////////////////////////////////////////////////////////////
// float 
// Geometry::PointDistanceFromPlane( CVector3<float> &vPoint, CPlane plane )
// {
  
//   CVector3<float> vNormal;

//   vNormal.UseExternalData(plane.m_pValues);

//   float fDistance = vNormal.Dot(vPoint) + plane.Distance();

//   return fDistance;

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
