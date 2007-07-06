/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/

#include "GSE_Camera.h"
#include "GSE_geometry.h"
#include "GSE_math.h"
#include "GSE_Logger.h"
#include <math.h>
#include <algorithm>
/////////////////////////////////////////////////////////////////
using namespace Geometry;
using Core::GSE_Logger;
using std::list;
/////////////////////////////////////////////////////////////////
GSE_Plane::GSE_Plane()
{
  m_pValues[0] = 0.0f;
  m_pValues[1] = 0.0f;
  m_pValues[2] = 0.0f;
  m_pValues[3] = 0.0f;
}
/////////////////////////////////////////////////////////////////
GSE_Plane::GSE_Plane(GSE_Vector3 vNormal, GSE_Vector3 vPoint)
{
  
  Calculate(vNormal, vPoint);
}
/////////////////////////////////////////////////////////////////
GSE_Plane::~GSE_Plane()
{
  
}
/////////////////////////////////////////////////////////////////
GSE_Plane &
GSE_Plane::SetNormal( GSE_Vector3 vNormal )
{

  vNormal.ToUnit();
  m_pValues[0] = vNormal[GSE_Vector3::X];
  m_pValues[1] = vNormal[GSE_Vector3::Y];
  m_pValues[2] = vNormal[GSE_Vector3::Z];
  
  return *this;
}
/////////////////////////////////////////////////////////////////
void 
GSE_Plane::Normalize()
{
  
  float fLength = sqrt( m_pValues[0] * m_pValues[0] + 
		  m_pValues[1] * m_pValues[1] + 
		  m_pValues[2] * m_pValues[2] );
  
  m_pValues[0] /= fLength;
  m_pValues[1] /= fLength;
  m_pValues[2] /= fLength;
  m_pValues[3] /= fLength;
  
}
/////////////////////////////////////////////////////////////////
GSE_Plane &
GSE_Plane::SetDistance( float fDistance )
{
  m_pValues[3] = fDistance;
  return *this;
}
/////////////////////////////////////////////////////////////////
void 
GSE_Plane::Calculate( GSE_Vector3 vNormal, const GSE_Vector3 & vPoint )
{
  vNormal.ToUnit();
  m_pValues[0] =  vNormal.m_pValues[GSE_Vector3::X];
  m_pValues[1] =  vNormal.m_pValues[GSE_Vector3::Y];
  m_pValues[2] =  vNormal.m_pValues[GSE_Vector3::Z];
  m_pValues[3] = -(vNormal.Dot(vPoint));
  
}
/////////////////////////////////////////////////////////////////
GSE_OrientedBox &
GSE_OrientedBox::SetOrientation(const GSE_Vector3 &vUp, 
				const GSE_Vector3 &vForward,
				const GSE_Vector3 &vRight)
{
  m_vUpward  = vUp;
  m_vRight   = vRight;
  m_vForward = vForward;
  return *this;
}
/////////////////////////////////////////////////////////////////
void 
GSE_OrientedBox::CalculatePlanes()
{
  m_Planes[TOP].Calculate(   -GetUpVector(),       GetPosition()+(GetUpVector()*GetHalfHeight()));
  m_Planes[BOTTOM].Calculate( GetUpVector(),       GetPosition()-(GetUpVector()*GetHalfHeight()));
  m_Planes[LEFT].Calculate(   GetRightVector(),    GetPosition()-(GetRightVector()*GetHalfWidth()));
  m_Planes[RIGHT].Calculate( -GetRightVector(),    GetPosition()+(GetRightVector()*GetHalfWidth()));
  m_Planes[FRONT].Calculate( -GetForwardVector(),  GetPosition()+(GetForwardVector()*GetHalfLength()));
  m_Planes[BACK].Calculate(   GetForwardVector(),  GetPosition()-(GetForwardVector()*GetHalfLength()));
}
/////////////////////////////////////////////////////////////////
void 
GSE_OrientedBox::CalculateCorners()
{
  GSE_Vector3 vTmp;
  GSE_Vector3 vTmp2;

  vTmp.UseExternalData( GetCorner( TOP_LEFT_FRONT ));
  
  vTmp =  GetPosition();
  vTmp += GetForwardVector()*GetHalfLength();
  vTmp -= GetRightVector()*GetHalfWidth(); 
  vTmp += GetUpVector()*GetHalfHeight(); 

  vTmp2 = vTmp;

  vTmp.UseExternalData( GetCorner( TOP_RIGHT_FRONT ));
  vTmp2 += GetRightVector()*GetWidth();
  vTmp = vTmp2;
  
  vTmp.UseExternalData( GetCorner( TOP_RIGHT_BACK ));
  vTmp2 += -GetForwardVector()*GetLength();
  vTmp = vTmp2;
  
  vTmp.UseExternalData( GetCorner( TOP_LEFT_BACK ));
  vTmp2 += -GetRightVector()*GetWidth();
  vTmp = vTmp2;

  vTmp.UseExternalData( GetCorner( GSE_OrientedBox::BOTTOM_LEFT_BACK ));
  vTmp2 += -GetUpVector()*GetHeight();
  vTmp = vTmp2;
  
  vTmp.UseExternalData( GetCorner( BOTTOM_RIGHT_BACK ));
  vTmp2 += GetRightVector()*GetWidth();
  vTmp = vTmp2;
  
  vTmp.UseExternalData( GetCorner( BOTTOM_RIGHT_FRONT ));
  vTmp2 += GetForwardVector()*GetLength();
  vTmp = vTmp2;
  
  vTmp.UseExternalData( GetCorner( BOTTOM_LEFT_FRONT ));
  vTmp2 += -GetRightVector()*GetWidth();
  vTmp = vTmp2;  

}
/////////////////////////////////////////////////////////////////
const float *
GSE_OrientedBox::GetCorners() const
{
  return m_aCorners;
}
/////////////////////////////////////////////////////////////////
VertexArray 
GSE_OrientedBox::GetCorner( BoxCorner_t nBoxCorner )
{
  return &(m_aCorners[nBoxCorner*3]);
}
/////////////////////////////////////////////////////////////////

#define APPLY_CORRECT_VALUES(R,S,T,rmax,rmin,smax, smin,tmax, tmin) {		\
 obOrientedBox[GSE_OrientedBox::FRONT].SetNormal(-R)[GSE_Plane::D]  = rmax;	\
  obOrientedBox[GSE_OrientedBox::BACK].SetNormal(R)[GSE_Plane::D]   = -rmin;	\
										\
  obOrientedBox[GSE_OrientedBox::RIGHT].SetNormal(-S)[GSE_Plane::D] = smax;	\
  obOrientedBox[GSE_OrientedBox::LEFT].SetNormal(S)[GSE_Plane::D]   = -smin;	\
										\
  obOrientedBox[GSE_OrientedBox::TOP].SetNormal(-T)[GSE_Plane::D]   = tmax;	\
  obOrientedBox[GSE_OrientedBox::BOTTOM].SetNormal(T)[GSE_Plane::D] = -tmin;	\
  /* The orientation will be formed so, that the forward vector will point along*/ \
  /* principal axis (largest eigenvalue ), right vector along vector with*/        \
  /* second largest and up points to vector with lowest eigenvalue*/               \
  obOrientedBox.SetOrientation(  T, R, S );                                  \
}

GSE_OrientedBox 
Geometry::CalculateOrientedBoundingBox( VertexArray pVertices, unsigned int nNumVertices )
{
  /////////////////////////////////////////////////////////////////  
  float fMaxR = 0.0f;
  float fMinR = 0.0f;
  float fMaxS = 0.0f;
  float fMinS = 0.0f;
  float fMaxT = 0.0f;
  float fMinT = 0.0f;
  float fLambda1,fLambda2,fLambda3;  
  float fVertDotR = 0.0f;
  float fVertDotS = 0.0f;
  float fVertDotT = 0.0f;
  GSE_Vector3	 vTemp;
  /////////////////////////////////////////////////////////////////
  GSE_Matrix3x3f mCovar = Math::CovarianceMatrix( pVertices, nNumVertices  );
  GSE_Matrix3x3f mEigenVectorMatrix = GSE_Matrix3x3f::Identity();
  Math::CalculateEigensystem( mCovar, fLambda1, fLambda2,fLambda3,
			      mEigenVectorMatrix);
  /////////////////////////////////////////////////////////////////
  GSE_Vector3 vR = Math::GetColumnVector(mEigenVectorMatrix, 0);
  GSE_Vector3 vS = Math::GetColumnVector(mEigenVectorMatrix, 1);
  GSE_Vector3 vT = Math::GetColumnVector(mEigenVectorMatrix, 2);

  /////////////////////////////////////////////////////////////////
  vR.ToUnit();
  vS.ToUnit();
  vT.ToUnit();
  /////////////////////////////////////////////////////////////////
  for ( unsigned int v = 0;v<nNumVertices;v++)
  {

    vTemp.UseExternalData(&(pVertices[v*3]));
    
    fVertDotR = vTemp.Dot(vR);
    fVertDotS = vTemp.Dot(vS);
    fVertDotT = vTemp.Dot(vT);
    
    if ( v==0 )
    {
      
      fMaxR = fMinR = fVertDotR;
      fMaxS = fMinS = fVertDotS;
      fMaxT = fMinT = fVertDotT;
      
    } else {
      
      if ( fVertDotR < fMinR )		fMinR = fVertDotR;
      else if ( fVertDotR > fMaxR )	fMaxR = fVertDotR;
      
      if ( fVertDotS < fMinS )		fMinS = fVertDotS;
      else if ( fVertDotS > fMaxS )	fMaxS = fVertDotS;
      
      if ( fVertDotT < fMinT )		fMinT = fVertDotT;
      else if ( fVertDotT > fMaxT )	fMaxT = fVertDotT;
      
    }
  }
  
  GSE_OrientedBox obOrientedBox;
#ifdef DEBUG
  GSE_Logger::Error() << DEBUG_HEADER << "minR " << fMinR << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "maxR " << fMaxR << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "minS " << fMinS << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "maxS " << fMaxS << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "minT " << fMinT << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "maxT " << fMaxT << endl;
#endif
  

  float fRlen = fMaxR - fMinR;
  float fSlen = fMaxS - fMinS;
  float fTlen = fMaxT - fMinT;
  
  float fA = (fMaxR + fMinR) * 0.5f;
  float fB = (fMaxS + fMinS) * 0.5f;
  float fC = (fMaxT + fMinT) * 0.5f;

  /// assign largest eigenvector to vR, second largest to vS and smallest to vT.
  if ( fRlen > fSlen ) 
  {
    /// fLambda1 > fLambda2 !!! 
    if ( fSlen > fTlen  )
    {
      APPLY_CORRECT_VALUES( vR, vS, vT, fMaxR, fMinR, fMaxS, fMinS, fMaxT, fMinT );
      obOrientedBox.SetLength( fRlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fTlen );
    } 
    else
    {
      if ( fRlen > fTlen )
      {
	APPLY_CORRECT_VALUES( vR, vT, vS, fMaxR, fMinR, fMaxT, fMinT,fMaxS, fMinS );
	obOrientedBox.SetLength( fRlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fSlen );
      }
      else
      {
	APPLY_CORRECT_VALUES( vT, vR, vS, fMaxT, fMinT, fMaxR, fMinR, fMaxS, fMinS );
	obOrientedBox.SetLength( fTlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fSlen );
      }
    }
  }
  else 
  {
    /// fLambda1 < fLambda2 !!! 
    
    if ( fSlen < fTlen )
    {
      APPLY_CORRECT_VALUES( vT, vS, vR, fMaxT, fMinT, fMaxS, fMinS, fMaxR, fMinR  );
      obOrientedBox.SetLength( fTlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fRlen );
    }
    else
    {
      /// fLambda3 < fLambda2

      if ( fRlen < fTlen )
      {
	APPLY_CORRECT_VALUES( vS, vT, vR, fMaxS, fMinS, fMaxT, fMinT, fMaxR, fMinR  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fRlen );
      }
      else 
      {
	APPLY_CORRECT_VALUES( vS, vR, vT, fMaxS, fMinS, fMaxR, fMinR, fMaxT, fMinT  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fTlen );

      }
    }
  }
  
  GSE_Vector3 vPos = (fA*vR) + (fB*vS) + (fC*vT);
  obOrientedBox.SetPosition( vPos );
  //obOrientedBox.CalculatePlanes();  
  obOrientedBox.CalculateCorners();

  return obOrientedBox;
  
}
/////////////////////////////////////////////////////////////////
GSE_OrientedBox 
Geometry::CalculateOrientedBoundingBox( VertexArray pVertices, const GSE_IndexBuffer &indexBuffer )
{
  /////////////////////////////////////////////////////////////////
  // Variables used
  float fMaxR = 0.0f;
  float fMinR = 0.0f;
  float fMaxS = 0.0f;
  float fMinS = 0.0f;
  float fMaxT = 0.0f;
  float fMinT = 0.0f;
  float fLambda1,fLambda2,fLambda3;  
  float fVertDotR = 0.0f;
  float fVertDotS = 0.0f;
  float fVertDotT = 0.0f;
  GSE_Vector3	 vTemp;
  /////////////////////////////////////////////////////////////////
  GSE_Matrix3x3f mCovar = Math::CovarianceMatrix( pVertices, indexBuffer );
  GSE_Matrix3x3f mEigenVectorMatrix = GSE_Matrix3x3f::Identity();
  /////////////////////////////////////////////////////////////////
  Math::CalculateEigensystem( mCovar, fLambda1, fLambda2,fLambda3,
			      mEigenVectorMatrix);
  /////////////////////////////////////////////////////////////////  
  GSE_Vector3 vR = GetColumnVector(mEigenVectorMatrix, 0);
  GSE_Vector3 vS = GetColumnVector(mEigenVectorMatrix, 1);
  GSE_Vector3 vT = GetColumnVector(mEigenVectorMatrix, 2);
  /////////////////////////////////////////////////////////////////
  vR.ToUnit();
  vS.ToUnit();
  vT.ToUnit();
  /////////////////////////////////////////////////////////////////
  for ( unsigned int i = 0;i<indexBuffer.m_nNumIndices;i++)
  {

    vTemp.UseExternalData(&(pVertices[indexBuffer.m_pIndices[i]*3]));

    fVertDotR = vTemp.Dot(vR);
    fVertDotS = vTemp.Dot(vS);
    fVertDotT = vTemp.Dot(vT);
    
    if ( i==0){
      
      fMaxR = fMinR = fVertDotR;
      fMaxS = fMinS = fVertDotS;
      fMaxT = fMinT = fVertDotT;
      
    } 
    else 
    {
      
      if ( fVertDotR < fMinR )		fMinR = fVertDotR;
      else if ( fVertDotR > fMaxR )	fMaxR = fVertDotR;
      
      if ( fVertDotS < fMinS )		fMinS = fVertDotS;
      else if ( fVertDotS > fMaxS )	fMaxS = fVertDotS;
      
      if ( fVertDotT < fMinT )		fMinT = fVertDotT;
      else if ( fVertDotT > fMaxT )	fMaxT = fVertDotT;
      
    }
  }
  /////////////////////////////////////////////////////////////////
  GSE_OrientedBox obOrientedBox;
#ifdef DEBUG
  GSE_Logger::Error() << DEBUG_HEADER << "minR " << fMinR << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "maxR " << fMaxR << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "minS " << fMinS << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "maxS " << fMaxS << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "minT " << fMinT << endl;
  GSE_Logger::Error() << DEBUG_HEADER << "maxT " << fMaxT << endl;
#endif

  float fRlen = fMaxR - fMinR;
  float fSlen = fMaxS - fMinS;
  float fTlen = fMaxT - fMinT;
  
  float fA = (fMaxR + fMinR) * 0.5f;
  float fB = (fMaxS + fMinS) * 0.5f;
  float fC = (fMaxT + fMinT) * 0.5f;


  /// assign largest eigenvector to vR, second largest to vS and smallest to vT.
  if ( fRlen > fSlen ) 
  {
    /// fLambda1 > fLambda2 !!! 
    if ( fSlen > fTlen  )
    {
      APPLY_CORRECT_VALUES( vR, vS, vT, fMaxR, fMinR, fMaxS, fMinS, fMaxT, fMinT );
      obOrientedBox.SetLength( fRlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fTlen );
    } 
    else
    {
      if ( fRlen > fTlen )
      {
	APPLY_CORRECT_VALUES( vR, vT, vS, fMaxR, fMinR, fMaxT, fMinT,fMaxS, fMinS );
	obOrientedBox.SetLength( fRlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fSlen );
      }
      else
      {
	APPLY_CORRECT_VALUES( vT, vR, vS, fMaxT, fMinT, fMaxR, fMinR, fMaxS, fMinS );
	obOrientedBox.SetLength( fTlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fSlen );
      }
    }
  }
  else 
  {
    /// fLambda1 < fLambda2 !!! 
    
    if ( fSlen < fTlen )
    {
      APPLY_CORRECT_VALUES( vT, vS, vR, fMaxT, fMinT, fMaxS, fMinS, fMaxR, fMinR  );
      obOrientedBox.SetLength( fTlen );
      obOrientedBox.SetWidth(  fSlen );
      obOrientedBox.SetHeight( fRlen );
    }
    else
    {
      /// fLambda3 < fLambda2

      if ( fRlen < fTlen )
      {
	APPLY_CORRECT_VALUES( vS, vT, vR, fMaxS, fMinS, fMaxT, fMinT, fMaxR, fMinR  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fTlen );
	obOrientedBox.SetHeight( fRlen );
      }
      else 
      {
	APPLY_CORRECT_VALUES( vS, vR, vT, fMaxS, fMinS, fMaxR, fMinR, fMaxT, fMinT  );
	obOrientedBox.SetLength( fSlen );
	obOrientedBox.SetWidth(  fRlen );
	obOrientedBox.SetHeight( fTlen );

      }
    }
  }

  /////////////////////////////////////////////////////////////////
  // The position
  GSE_Vector3 vPos = (fA*vR) + (fB*vS) + (fC*vT);
  obOrientedBox.SetPosition( vPos );
  /////////////////////////////////////////////////////////////////
  // The orientation will be formed so, that the forward vector will point along
  // principal axis (largest eigenvalue ), right vector along vector with 
  // second largest and up points to vector with lowest eigenvalue
  ///obOrientedBox.SetOrientation(  vT, vR, vS );
  //obOrientedBox.CalculatePlanes();
  obOrientedBox.CalculateCorners();
  
  /////////////////////////////////////////////////////////////////
  return obOrientedBox;
  
}
/////////////////////////////////////////////////////////////////
Geometry::GSE_Sphere
Geometry::CalculateBoundingSphere( VertexArray pVertices, unsigned int nNumVertices)
{
  // Minimum and maximum values for each coordinates.
  GSE_Vector3 vMaxValues;
  GSE_Vector3 vMinValues;

  for ( unsigned int v = 0;v<nNumVertices; v++)
  {
    // 
    float fTempX = pVertices[(v*3)];
    float fTempY = pVertices[(v*3)+1];
    float fTempZ = pVertices[(v*3)+2];

    if ( v == 0 ){

      vMaxValues[GSE_Vector3::X] = vMinValues[GSE_Vector3::X] = fTempX;
      vMaxValues[GSE_Vector3::Y] = vMinValues[GSE_Vector3::Y] = fTempY;
      vMaxValues[GSE_Vector3::Z] = vMinValues[GSE_Vector3::Z] = fTempZ;

    } else {
      
      STORE_MAX_MIN(fTempX, vMaxValues[GSE_Vector3::X], vMinValues[GSE_Vector3::X]);
      STORE_MAX_MIN(fTempY, vMaxValues[GSE_Vector3::Y], vMinValues[GSE_Vector3::Y]);
      STORE_MAX_MIN(fTempZ, vMaxValues[GSE_Vector3::Y], vMinValues[GSE_Vector3::Y]);
    }
  }

  GSE_Sphere sphere;
  sphere.SetPosition( (vMaxValues + vMinValues ) / 2 );
  GSE_Vector3 vDist = vMaxValues - vMinValues;
  
  float fMaxRadius = 0.0; 

  if ( fabs(vDist[GSE_Vector3::X]) > fabs(vDist[GSE_Vector3::Y])){

    if ( fabs(vDist[GSE_Vector3::X] > fabs(vDist[GSE_Vector3::Z]))){
      fMaxRadius = vDist[GSE_Vector3::X] * 0.5f;
    } else {
      fMaxRadius = vDist[GSE_Vector3::Z] * 0.5f;
    }

  } else {

    if ( fabs(vDist[GSE_Vector3::Y] > fabs(vDist[GSE_Vector3::Z]))){
      fMaxRadius = vDist[GSE_Vector3::Y] * 0.5f;
    } else {
      fMaxRadius = vDist[GSE_Vector3::Z] * 0.5f;
    }

  }
  sphere.SetRadius( fMaxRadius  );
  return sphere;
}

/////////////////////////////////////////////////////////////////
Geometry::GSE_Sphere 
Geometry::CalculateBoundingSphereTight( VertexArray pVertices, unsigned int nNumVertices )
{
  // The returned sphere
  GSE_Sphere sphere;

  float fLambda1,fLambda2,fLambda3;  

  // Calculate the principal axis R by component analysis
  GSE_Matrix3x3f mCovar = Math::CovarianceMatrix( pVertices, nNumVertices  );
  GSE_Matrix3x3f mEigenVectorMatrix = GSE_Matrix3x3f::Identity();

  Math::CalculateEigensystem( mCovar, 
			      fLambda1, fLambda2,fLambda3,
			      mEigenVectorMatrix);
  
  GSE_Vector3 vR = GetColumnVector(mEigenVectorMatrix, 0);
  vR.ToUnit();
  // Get min and max values of each point in direction R
  GSE_Vector3 vTemp;

  float fMinExtent, fMaxExtent, fTempDotR;
  unsigned int nMaxIndex = 0, nMinIndex = 0;

  // Initialize the first values
  vTemp.UseExternalData( &(pVertices[0]));
  fMinExtent = fMaxExtent = vTemp.Dot(vR);
  GSE_Vector3 vMin = vTemp;
  GSE_Vector3 vMax = vTemp;
  
  // for each vertex
  for ( unsigned int v = 1;v<nNumVertices; v++)
  {
    
    vTemp.UseExternalData( &(pVertices[v*3]));
    fTempDotR = vTemp.Dot( vR );

    if ( fTempDotR > fMaxExtent ){

      fMaxExtent = fTempDotR;
      nMaxIndex = v;

    } else if (fTempDotR < fMinExtent){

      fMinExtent = fTempDotR;
      nMinIndex = v;
    }

  }
  // Assign initial center and radius
  vMin.UseExternalData(&pVertices[nMinIndex*3]);
  vMax.UseExternalData(&pVertices[nMaxIndex*3]);
  
  //sphere.SetPosition( (vMin + vMax ) * 0.5f );
  
  GSE_Vector3 vPos = vMin + ((vMax-vMin) * 0.5f);
  sphere.SetPosition( vPos );
  sphere.SetRadius((vMax - sphere.GetPosition()).Length());  
  
  // Check each vertex that they actually are stored inside the sphere
  float fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
  
  // Temporary point when determining new center and radius
  GSE_Vector3 vGPoint;

  // The vector from center of the sphere to the current vertex
  GSE_Vector3 vCenterToPoint;

  // For each vertex
  for ( unsigned int v = 0;v<nNumVertices; v++)
  {
    
    vTemp.UseExternalData( &(pVertices[v*3]));

    float fDist = ((vTemp - sphere.GetPosition()).Length());
    float fDistSquared = fDist * fDist;

    if (  fDistSquared > fRadiusSquared )
    {
      // Point lies outside sphere, adjust center and radius
      vCenterToPoint = (vTemp - sphere.GetPosition());
      vCenterToPoint.ToUnit();
      vGPoint = sphere.GetPosition() - (sphere.GetRadius() * vCenterToPoint );
      //sphere.SetPosition( vGPoint + (( vTemp-vGPoint) * 0.5));
      sphere.SetPosition( (vGPoint + vTemp) * 0.5f);
      sphere.SetRadius( (vTemp - sphere.GetPosition() ).Length());
      
      fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
    } 
    else 
    {
      // Point within sphere, everything is good in 
      // the hood tonite.
    }
  } // for each vertex
  // Return the tighter sphere
  return sphere;
}
/////////////////////////////////////////////////////////////////
Geometry::GSE_Sphere 
Geometry::CalculateBoundingSphereTight( VertexArray pVertices, const GSE_IndexBuffer &indexBuffer )
{
  // The returned sphere
  GSE_Sphere sphere;

  float fLambda1,fLambda2,fLambda3;  

  // Calculate the principal axis R by component analysis
  GSE_Matrix3x3f mCovar = Math::CovarianceMatrix( pVertices, indexBuffer  );
  GSE_Matrix3x3f mEigenVectorMatrix = GSE_Matrix3x3f::Identity();

  Math::CalculateEigensystem( mCovar, 
			      fLambda1, fLambda2,fLambda3,
			      mEigenVectorMatrix);
  
  GSE_Vector3 vR = GetColumnVector(mEigenVectorMatrix, 0);
  vR.ToUnit();
  // Get min and max values of each point in direction R
  GSE_Vector3 vTemp;
  
  float fMinExtent, fMaxExtent, fTempDotR;
  unsigned int nMaxIndex = 0, nMinIndex = 0;

  // Initialize the first values
  vTemp.UseExternalData( &(pVertices[indexBuffer.m_pIndices[0]]));
  fMinExtent = fMaxExtent = vTemp.Dot(vR);
  GSE_Vector3 vMin = vTemp;
  GSE_Vector3 vMax = vTemp;
  
  // for each vertex
  for ( unsigned int i = 1;i<indexBuffer.m_nNumIndices; i++)
  {
    
    vTemp.UseExternalData( &(pVertices[indexBuffer.m_pIndices[i]*3]));
    fTempDotR = vTemp.Dot( vR );

    if ( fTempDotR > fMaxExtent ){

      fMaxExtent = fTempDotR;
      nMaxIndex = indexBuffer.m_pIndices[i];

    } else if (fTempDotR < fMinExtent){

      fMinExtent = fTempDotR;
      nMinIndex = indexBuffer.m_pIndices[i];
    }

  }
  // Assign initial center and radius
  vMin.UseExternalData(&pVertices[nMinIndex*3]);
  vMax.UseExternalData(&pVertices[nMaxIndex*3]);
  
  //sphere.SetPosition( (vMin + vMax ) * 0.5f );
  
  GSE_Vector3 vPos = vMin + ((vMax-vMin) * 0.5f);
  sphere.SetPosition( vPos );
  sphere.SetRadius((vMax - sphere.GetPosition()).Length());  
  
  // Check each vertex that they actually are stored inside the sphere
  float fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
  
  // Temporary point when determining new center and radius
  GSE_Vector3 vGPoint;

  // The vector from center of the sphere to the current vertex
  GSE_Vector3 vCenterToPoint;

  // For each vertex
  for ( unsigned int i = 0;i<indexBuffer.m_nNumIndices; i++)
  {
    
    vTemp.UseExternalData( &(pVertices[indexBuffer.m_pIndices[i]*3]));

    float fDist = ((vTemp - sphere.GetPosition()).Length());
    float fDistSquared = fDist * fDist;

    if (  fDistSquared > fRadiusSquared )
    {
      // Point lies outside sphere, adjust center and radius
      vCenterToPoint = (vTemp - sphere.GetPosition());
      vCenterToPoint.ToUnit();
      vGPoint = sphere.GetPosition() - (sphere.GetRadius() * vCenterToPoint );
      //sphere.SetPosition( vGPoint + (( vTemp-vGPoint) * 0.5));
      sphere.SetPosition( (vGPoint + vTemp) * 0.5f);
      sphere.SetRadius( (vTemp - sphere.GetPosition() ).Length());
      
      fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
    } 
    else 
    {
      // Point within sphere, everything is good in 
      // the hood tonite.
    }
  } // for each vertex
  // Return the tighter sphere
  return sphere;
}
/////////////////////////////////////////////////////////////////
GSE_AxisAlignedBox 
Geometry::CalculateAlignedBox( VertexArray pVertices, int nNumVertices ) 
{
  if ( nNumVertices > 1)
  {
    GSE_Vector3 vMin(pVertices[0],pVertices[1],pVertices[2]);
    GSE_Vector3 vMax(pVertices[0],pVertices[1],pVertices[2]);
    
    unsigned int nIndex = 0;
    
    for(int v=1;v<nNumVertices;v++)
    {
      nIndex = (v*3);
      STORE_MAX_MIN(pVertices[nIndex],   vMax[GSE_Vector3::X], vMin[GSE_Vector3::X]);
      STORE_MAX_MIN(pVertices[nIndex+1], vMax[GSE_Vector3::Y], vMin[GSE_Vector3::Y]);
      STORE_MAX_MIN(pVertices[nIndex+2], vMax[GSE_Vector3::Z], vMin[GSE_Vector3::Z]); 
    }
    GSE_Vector3 vDimension = (vMax - vMin);
    GSE_Vector3 vCenter = vMin + vDimension*0.5f;
    return GSE_AxisAlignedBox( vCenter, vDimension[GSE_Vector3::X], vDimension[GSE_Vector3::Y], vDimension[GSE_Vector3::Z]);
  }
  else
  {
    return GSE_AxisAlignedBox();
  }
}
/////////////////////////////////////////////////////////////////
int 
Geometry::RayIntersectsSphere( GSE_Vector3 &vRayStart, 
			       GSE_Vector3 &vRayEnd,
			       GSE_Vector3 *pvIntersection0, 
			       GSE_Vector3 *pvIntersection1, 
			       GSE_Sphere &Sphere)
{
  int iIntersects = 0;
  GSE_Vector3 vSphereToRayStart = vRayStart - Sphere.GetPosition();

  GSE_Vector3 vStartToEnd = vRayEnd - vRayStart;
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
    GSE_Vector3 vInt0, vInt1;

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
GSE_PlaneIntersection
Geometry::LineIntersectsPlane( const GSE_Plane &plane,
			       const GSE_Vector3 &vPoint1,
			       const GSE_Vector3 &vPoint2)
{
  
#define EPSILON 0.0001f
  float fDistanceOne = 0.0f, fDistanceTwo = 0.0f;
  GSE_Vector3 vNormal;

  vNormal.UseExternalData(plane.m_pValues);
  
  fDistanceOne = vNormal.Dot( vPoint1 ) + plane.GetDistance();
  fDistanceTwo = vNormal.Dot( vPoint2 ) + plane.GetDistance();
  
  // if other distance is positive and other negative, we have collision.
  // (which means that the endpoints are located in both sides of the plane)
  // Or other line point is in the plane. (distance == 0.0 )
  float fDistanceSqr = fDistanceOne * fDistanceTwo;
  if ( fDistanceSqr <= 0.0f )
  {
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
#undef EPSILON
}
/////////////////////////////////////////////////////////////////
GSE_Vector3 
Geometry::LinePlaneIntersectionPoint( const GSE_Plane &plane,
				      const GSE_Vector3 &vPoint1,
				      const GSE_Vector3 &vPoint2 )
{

  
  // Get the normalized line direction vector
  GSE_Vector3 vDir = vPoint2 - vPoint1;
  vDir.ToUnit();
  GSE_Vector3 vNormal;
  vNormal.UseExternalData( plane.m_pValues );
  // The negated distance of the vPoint1 from the plane 
  float fNumerator = -(vNormal.Dot(vPoint1) + plane.GetDistance());
  // The vNormal · vDir = cos( angle( vNormal, vDir ))
  float fDenominator = vNormal.Dot(vDir);

  // Rounding errors are nasty, so let's take them into account 
  // while we make sure we don't divide by zero.
  if ( fabs(fDenominator) <= 0.0001f )
  {
    // line is parallel to the plane
    return vPoint1;
  }
  // The distance along vDir towards vPoint2.
  float fDist = fNumerator / fDenominator;

  // return the point of intersection
  return vPoint1 + (fDist * vDir);

}
/////////////////////////////////////////////////////////////////
GSE_PlaneIntersection
Geometry::SphereIntersectsPlane( GSE_Plane &plane,
				 const GSE_Sphere &sphere,
				 float &fCenterDistance)
{

  GSE_Vector3 vNormal;
  vNormal.UseExternalData(plane.m_pValues);
  float fDistance = vNormal.Dot( sphere.GetPosition()) + plane.Distance();
  fCenterDistance = fDistance;

  if ( fabs(fDistance) <= sphere.GetRadius()){
    return INTERSECTS;
  } else if ( fDistance > sphere.GetRadius()){
    return FRONT;
  }
  return BEHIND;
}
/////////////////////////////////////////////////////////////////
GSE_Vector3 
Geometry::ClosestPointOnLine( const GSE_Vector3 &vPoint,
			      const GSE_Vector3 &vLinePoint1,
			      const GSE_Vector3 &vLinePoint2 )
{

  GSE_Vector3 vOneToPoint = vPoint - vLinePoint1;

  GSE_Vector3 vLine = vLinePoint2 - vLinePoint1;
  
  float fLength = vLine.Length();
  
  vLine.ToUnit();
  
  float fDot = vLine.Dot(vOneToPoint);

  if ( fDot <= 0.0f ){
    // return the starting point
    return vLinePoint1;
    // return the end point
  } else if ( fDot >= fLength ){
    return vLinePoint2;
  }
  // return the point in the middle 
  return vLinePoint1 + (vLine * fDot );
}
/////////////////////////////////////////////////////////////////
GSE_Vector3 
Geometry::ClosestPointOnRay( const GSE_Vector3 &vPoint,
			     const GSE_Vector3 &vStartPoint,
			     const GSE_Vector3 &vDirection )
{
  GSE_Vector3 vStartToPoint = vPoint - vStartPoint;
  float fDot = vDirection.Dot(vStartToPoint);
  return vPoint + (vDirection * fDot );
}
/////////////////////////////////////////////////////////////////
char 
Geometry::SphereIntersectsSphere( const GSE_Sphere &sphereOne, const GSE_Sphere &sphereTwo )
{
  // Get the separating axis
  GSE_Vector3 vSepAxis = sphereOne.GetPosition() - sphereTwo.GetPosition();

  // Get the sum of the radii
  float fSumOfRadii = sphereOne.GetRadius() + sphereTwo.GetRadius();
  fSumOfRadii = fSumOfRadii * fSumOfRadii;
  // if the distance between the centers is less than the sum
  // of the radii, then we have an intersection
  return (vSepAxis.LengthSqr() < fSumOfRadii);

}
/////////////////////////////////////////////////////////////////
//
// As described in document:
// http://www.geometrictools.com/Documentation/IntersectionSphereCone.pdf
//
/////////////////////////////////////////////////////////////////
char
Geometry::SphereIntersectsCone( const GSE_Sphere &sphere, const GSE_Cone &cone )
{
  char bRetval = 0;
#ifndef DEBUG
  GSE_Vector3 vU = cone.GetPosition() - 
  (sphere.GetRadius()*cone.SinReciprocal())*cone.GetDirection();
  
  GSE_Vector3 vD = sphere.GetPosition() - vU;
  
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

#else
  GSE_Vector3 vU = cone.GetPosition() - (sphere.GetRadius()/cone.SinAngle())*cone.GetDirection();
  GSE_Vector3 vD = sphere.GetPosition() - vU;
    
  if ( cone.GetDirection().Dot( vD ) >= vD.Length()*cone.CosAngle()){
      
    vD = sphere.Position() - cone.Position();
    if ( -cone.GetDirection().Dot( vD ) >= vD.Length()*cone.SinAngle()){

      bRetval = vD.Length() <= sphere.GetRadius();
      GSE_Logger::Error() << "first true, second " << (bRetval ? "true" : "false" ) << std::endl;
    }
    else {
      bRetval = 1;
      GSE_Logger::Error() << "first true, second true" << std::endl;
    }
  } 
  else
  {
    GSE_Logger::Error() << "first false" << std::endl;
    bRetval = 0;
  }
  

#endif
  return bRetval;
}
/////////////////////////////////////////////////////////////////
char 
Geometry::SphereIntersectsPolygon( GSE_Sphere &sphere, 
				   VertexArray pVertices,
				   unsigned int nNumVertices,
				   GSE_Vector3 &vOffsetMovement )
{

  char bIntersection = 0;
  GSE_Vector3 vOne, vTwo, vThree;
  GSE_Plane vPlane;

  // Calculate normal vector
  vOne.UseExternalData( &(pVertices[0]));
  vTwo.UseExternalData( &(pVertices[3]));
  vThree.UseExternalData( &(pVertices[6]));
  
  GSE_Vector3 vNormal = (vTwo - vOne ).Cross(vThree - vOne);
  vNormal.ToUnit();
  vPlane.Calculate( vNormal, vOne);

  float fDistance = 0.0f;
  
  if ( SphereIntersectsPlane(vPlane, sphere, fDistance ) == INTERSECTS){

    GSE_Vector3 vOffset   = vNormal * fDistance;
    
    GSE_Vector3 vPosition = sphere.GetPosition() - vOffset;
    
    if ( InsidePolygon( vPosition, pVertices, nNumVertices) ) {
    
      bIntersection = 1;

    } else {
    
      bIntersection = SphereIntersectsPolygonEdges( sphere, pVertices, nNumVertices );
    }
  }
  
  if ( bIntersection )
  {
    if ( fDistance > 0.0f )
    {

      float fDistanceDiff = sphere.GetRadius() - fDistance;
      vOffsetMovement = vNormal * fDistanceDiff;

    } else {

      float fDistanceDiff = sphere.GetRadius() + fDistance;
      vOffsetMovement = vNormal * -fDistanceDiff;
      
    }
  }
  return bIntersection;
}
/////////////////////////////////////////////////////////////////
char 
Geometry::InsidePolygon( GSE_Vector3 &vPoint, 
			 VertexArray pVertices,
			 unsigned int nNumVertices)
{
  char bRetval = 0;
  GSE_Vector3 vS, vT, vVect1, vVect2;
  float fAngle = 0.0f;

  //GSE_Logger::Log() << DEBUG_HEADER << " Point :" << vPoint << std::endl;

  for( unsigned int v=0;v<nNumVertices;v++)
  {
    vS.UseExternalData( &(pVertices[(v*3)]));
    vT.UseExternalData( &(pVertices[((v+1)%nNumVertices)*3]));
    vVect1 = vS - vPoint;
    vVect2 = vT - vPoint;
    fAngle += AngleBetweenVectors( vVect1, vVect2);
  }
  
  if ( fabs(fAngle) >= (0.99f * Math::PI * 2.0f)){
    bRetval = 1;
  }

  return bRetval;
}
/////////////////////////////////////////////////////////////////
float 
Geometry::AngleBetweenVectors( GSE_Vector3 &vVect1, GSE_Vector3 &vVect2)
{

  float fDot = vVect1.Dot(vVect2);
  float fLengthMult = vVect1.Length() * vVect2.Length();
  float fAngle = 0.0f;

  if ( fLengthMult != 0.0f )
  {

    fAngle = acosf( fDot / fLengthMult );
    // sanity check 
    if ( isnan(fAngle) ) fAngle = 0.0f;
    
  }

  return fAngle;
}
/////////////////////////////////////////////////////////////////
char      
Geometry::SphereIntersectsPolygonEdges( GSE_Sphere &sphere, 
					VertexArray pVertices,
					unsigned int nNumVertices)
{

  GSE_Vector3 vVect1, vVect2, vPoint;
  GSE_Vector3 vPos = sphere.GetPosition();    

  for( unsigned int v=0;v<nNumVertices;v++){

    vVect1.UseExternalData( &(pVertices[v*3]));
    vVect2.UseExternalData( &(pVertices[((v+1)%nNumVertices)*3]));
    
    vPoint = ClosestPointOnLine(vPos, vVect1, vVect2 );
    
    // If closest point is within the sphere radius, we have a match
    if ( (vPoint - sphere.GetPosition()).Length() < sphere.GetRadius()){
      return 1;
    }

  }
  // No match
  return 0;
}
/////////////////////////////////////////////////////////////////
int
Geometry::SphereIntersectsAxisAlignedBox( const GSE_Sphere &sphere, 
					  const GSE_AxisAlignedBox &aaBox)
{
  float fDistance = 0.0f;
  float fTmp = 0.0f;
  /////////////////////////////////////////////////////////////////
  // Test for X axis
  float fAxisMax = aaBox.GetPosition().m_pValues[GSE_Vector3::X] + aaBox.GetWidth();
  float fAxisMin = aaBox.GetPosition().m_pValues[GSE_Vector3::X] - aaBox.GetWidth();
  float fAxisCenter = sphere.GetPosition().m_pValues[GSE_Vector3::X];

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
  fAxisMax = aaBox.GetPosition().m_pValues[GSE_Vector3::Y] + aaBox.GetHeight();
  fAxisMin = aaBox.GetPosition().m_pValues[GSE_Vector3::Y] - aaBox.GetHeight();
  fAxisCenter = sphere.GetPosition().m_pValues[GSE_Vector3::Y];
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
  fAxisMax = aaBox.GetPosition().m_pValues[GSE_Vector3::Z] + aaBox.GetLength();
  fAxisMin = aaBox.GetPosition().m_pValues[GSE_Vector3::Z] - aaBox.GetLength();
  fAxisCenter = sphere.GetPosition().m_pValues[GSE_Vector3::Z];
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
GSE_PlaneIntersection
Geometry::PlaneIntersectsAABB( const GSE_Plane &rPlane,  const GSE_AxisAlignedBox &rAABB)
{
  GSE_Vector3 vCornerMin;
  GSE_Vector3 vCornerMax;
  float fHalfWidth  = rAABB.GetWidth()*0.5f;
  float fHalfHeight = rAABB.GetHeight()*0.5f;
  float fHalfLength = rAABB.GetLength()*0.5f;
  /////////////////////////////////////////////////////////////////
  // Test x axis
  if ( rPlane.m_pValues[0] < 0.0f)
  {
    vCornerMin[GSE_Vector3::X] = rAABB.GetPosition().m_pValues[GSE_Vector3::X] - fHalfWidth;
    vCornerMax[GSE_Vector3::X] = rAABB.GetPosition().m_pValues[GSE_Vector3::X] + fHalfWidth;
  }
  else 
  {
    vCornerMax[GSE_Vector3::X] = rAABB.GetPosition().m_pValues[GSE_Vector3::X] - fHalfWidth;
    vCornerMin[GSE_Vector3::X] = rAABB.GetPosition().m_pValues[GSE_Vector3::X] + fHalfWidth;
  }
  /////////////////////////////////////////////////////////////////
  // Test y axis
  if ( rPlane.m_pValues[1] < 0.0f)
  {
    vCornerMin[GSE_Vector3::Y] = rAABB.GetPosition().m_pValues[GSE_Vector3::Y] - fHalfHeight;
    vCornerMax[GSE_Vector3::Y] = rAABB.GetPosition().m_pValues[GSE_Vector3::Y] + fHalfHeight;
  }
  else 
  {
    vCornerMax[GSE_Vector3::Y] = rAABB.GetPosition().m_pValues[GSE_Vector3::Y] - fHalfHeight;
    vCornerMin[GSE_Vector3::Y] = rAABB.GetPosition().m_pValues[GSE_Vector3::Y] + fHalfHeight;
  }
  /////////////////////////////////////////////////////////////////
  // Test z axis
  if ( rPlane.m_pValues[2] < 0.0f)
  {
    vCornerMin[GSE_Vector3::Z] = rAABB.GetPosition().m_pValues[GSE_Vector3::Z] - fHalfLength;
    vCornerMax[GSE_Vector3::Z] = rAABB.GetPosition().m_pValues[GSE_Vector3::Z] + fHalfLength;
  }
  else 
  {
    vCornerMax[GSE_Vector3::Z] = rAABB.GetPosition().m_pValues[GSE_Vector3::Z] - fHalfLength;
    vCornerMin[GSE_Vector3::Z] = rAABB.GetPosition().m_pValues[GSE_Vector3::Z] + fHalfLength;
  }   
  /// Check the intersection of the diagonal line
  return LineIntersectsPlane( rPlane, vCornerMin, vCornerMax);
}
/////////////////////////////////////////////////////////////////
GSE_PlaneIntersectionType 
Geometry::PlaneIntersectionPoint( GSE_Plane &Plane1, 
				  GSE_Plane &Plane2,
				  GSE_Plane &Plane3,
				  GSE_Vector3 &vPoint )
{

  GSE_Matrix3x3f mMatrix;
  GSE_PlaneIntersectionType iRetval = NO_INTERSECTION;
  GSE_Vector3 vNormal;

  vNormal.UseExternalData(Plane1.m_pValues);
  Math::SetRowVector(mMatrix, 0, vNormal);
  
  vNormal.UseExternalData(Plane2.m_pValues);
  Math::SetRowVector(mMatrix, 1, vNormal);

  vNormal.UseExternalData(Plane3.m_pValues);
  Math::SetRowVector(mMatrix, 2, vNormal);

  GSE_Vector3 vDs(-Plane1[GSE_Plane::D],
		  -Plane2[GSE_Plane::D],
		  -Plane3[GSE_Plane::D] );
  
  //GSE_Logger::Error() << "----" << endl;
  //GSE_Logger::Error() << mMatrix ;
  //GSE_Logger::Error() << "D " << vDs << endl;

  if ( Det(mMatrix) == 0.0f )
  {
    iRetval = NO_INTERSECTION;
  } else {
    GSE_Matrix3x3f mInvMatrix = mMatrix.Inverse();
    //GSE_Logger::Error() << "inverse:\n" << mInvMatrix ;
    vPoint = MultiplyFromRight( mInvMatrix, vDs );
    //GSE_Logger::Error() << "result: " << vPoint << endl;
    iRetval = POINT;
  }
  //GSE_Logger::Error() << "----" << endl;
  return iRetval;
  
}
/////////////////////////////////////////////////////////////////
GSE_Sphere
Geometry::MergeSpheres( const GSE_Sphere &sphereOne, const GSE_Sphere &sphereTwo)
{
  
  GSE_Vector3	vCenterDiff = sphereTwo.GetPosition() - sphereOne.GetPosition();
  float		fCenterDiffLen  = 0.0f;
  float		fRadiusDiff = sphereTwo.GetRadius() - sphereOne.GetRadius();
  float		fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
  float		fLengthSqr     = vCenterDiff.LengthSqr();

  int bSphereOneValid = sphereOne.GetRadius() > 0.00001f;
  int bSphereTwoValid = sphereTwo.GetRadius() > 0.00001f;
  if (  bSphereOneValid && !bSphereTwoValid ) return sphereOne;
  if ( !bSphereOneValid &&  bSphereTwoValid ) return sphereTwo;

  if ( fRadiusDiffSqr >= fLengthSqr )
  {

    if ( fRadiusDiff >= 0.0f ) // Sphere two contains the sphere one
    {     
      return sphereTwo;
    } 
    else // Sphere one contains the sphere two
    {			
      return sphereOne;
    }

  } else {

    fCenterDiffLen = sqrt( fLengthSqr );
    float fT = (fCenterDiffLen + sphereTwo.GetRadius() - 
		sphereOne.GetRadius()) /(2.0f * fCenterDiffLen);
    
    return GSE_Sphere( sphereOne.GetPosition() + (fT * vCenterDiff),
		       (fCenterDiffLen + sphereTwo.GetRadius() + sphereOne.GetRadius())*0.5f);
  }
}
/////////////////////////////////////////////////////////////////
int
Geometry::PointIntersectsOBB( const GSE_OrientedBox &obBox, 
			      const GSE_Vector3 &vPoint )
{
  
  GSE_Vector3 vPos = vPoint - obBox.GetPosition();
  if ( fabs(vPos.Dot(obBox.GetForwardVector())) > obBox.GetHalfLength()) return 0;
  if ( fabs(vPos.Dot(obBox.GetRightVector())) > obBox.GetHalfWidth()) return 0;
  if ( fabs(vPos.Dot(obBox.GetUpVector())) > obBox.GetHalfHeight()) return 0;
  return 1;
}
/////////////////////////////////////////////////////////////////
int 
Geometry::RayIntersectsOBB( const GSE_Vector3 &vRayStart, 
			    const GSE_Vector3 &vRayDir,
			    const GSE_OrientedBox &obBox,
			    float *pfValue )
{
  float fT_min,fT_max;
  float fE, fF;
  float f1DivF;
  float fT_1,fT_2;
  float fHalfValue;
  int   bStartInsideBox = 1;
  GSE_Vector3 vPoint = obBox.GetPosition() - vRayStart;
  if ( fabs(vPoint.Dot(obBox.GetForwardVector())) > obBox.GetHalfLength()) bStartInsideBox = 0;
  if ( fabs(vPoint.Dot(obBox.GetRightVector())  ) > obBox.GetHalfWidth())  bStartInsideBox = 0;
  if ( fabs(vPoint.Dot(obBox.GetUpVector())     ) > obBox.GetHalfHeight()) bStartInsideBox = 0;

  if ( bStartInsideBox ) 
  {
    if ( pfValue != NULL ) *pfValue = 0.0f;
    return 1;
  }
  
  // for each dimension in OBB do 
  for ( int i=0;i<3;i++)
  {
    // Get correct values for each loop
    switch ( i )
    {
    case 0:
      fE = obBox.GetForwardVector().Dot( vPoint );
      fF = obBox.GetForwardVector().Dot( vRayDir );
      fHalfValue = obBox.GetHalfLength();
      break;
    case 1:
      fE = obBox.GetRightVector().Dot( vPoint );
      fF = obBox.GetRightVector().Dot( vRayDir );
      fHalfValue = obBox.GetHalfWidth();
      break;
    default:
      fE = obBox.GetUpVector().Dot( vPoint );
      fF = obBox.GetUpVector().Dot( vRayDir );
      fHalfValue = obBox.GetHalfHeight();
    }
    
    if ( TOO_CLOSE_TO_ZERO(fF) )
    {
      if (-fE - fHalfValue > 0.0f || -fE + fHalfValue < 0.0f )
      {
	return 0;
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

      /// Min/max comparison, first loop initializes values,
      if ( i == 0 )
      {
	fT_min = fT_1;
	fT_max = fT_2;
      } 
      else
      {
	if ( fT_1 > fT_min) fT_min = fT_1;
	if ( fT_2 < fT_max) fT_max = fT_2;
      }
      // Perform tests, and exit on non-intersection.
      if ( fT_min > fT_max ) return 0;
      if ( fT_max < 0.0f   ) return 0;
    
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
  return 1;

}
/////////////////////////////////////////////////////////////////
GSE_OrientedBox 
Geometry::MergeOrientedBoxes( const GSE_OrientedBox &obOne, const GSE_OrientedBox &obTwo )
{
  unsigned int bObOneValid =  (obOne.GetLength() * obOne.GetWidth() * obOne.GetHeight()) > 0.0001f;
  unsigned int bObTwoValid =  (obTwo.GetLength() * obTwo.GetWidth() * obTwo.GetHeight()) > 0.0001f;
  if (  bObOneValid && !bObTwoValid ) return obOne;
  if ( !bObOneValid && bObTwoValid ) return obTwo;
  if ( !bObOneValid && !bObTwoValid) return GSE_OrientedBox();

#define OPTIMAL_BOUNDING_BOX_MERGE
#ifdef OPTIMAL_BOUNDING_BOX_MERGE
  
  GSE_OrientedBox obMerge;
  GSE_Vector3 vDir = obTwo.GetPosition() - obOne.GetPosition();
  vDir.ToUnit();

  GSE_Vector3 vCorner;
  GSE_Vector3 vCornerOnLine;
  GSE_Vector3 vMin(0,0,0);
  GSE_Vector3 vMax(0,0,0);
  GSE_Vector3 vSecondaryAxis;
  GSE_Vector3 vSecAxisCandidate;

  float fDot;
  float fLength;
  
  int i;
  // For each corner in box one 
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obOne).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    fDot = vDir.Dot(vCorner - obOne.GetPosition());

    // calculate possible secondary axis.
    vCornerOnLine = obOne.GetPosition() + vDir * fDot;
    vSecAxisCandidate = vCorner - vCornerOnLine;
    fLength = vSecAxisCandidate.Length();
    
    /// 
    if (fDot > vMax[0])  {     vMax[0] = fDot;  }
    else if (fDot < vMin[0]){  vMin[0] = fDot;  }
    
    // Since this is a symmetric box, we need to check only for 
    // max, since width becomes 2 * max.
    if ( fLength > vMax[1] ) 
    { 
      vMax[1]  = fLength; 
      vSecondaryAxis = vSecAxisCandidate;
    }
  }
  
  // For each corner in box two
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obTwo).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    fDot = vDir.Dot(vCorner - obOne.GetPosition());

    // calculate possible secondary axis.
    vCornerOnLine = obOne.GetPosition() + vDir * fDot;
    vSecAxisCandidate = vCorner - vCornerOnLine;
    fLength = vSecAxisCandidate.Length();
    
    /// 
    if (fDot > vMax[0])  {     vMax[0] = fDot;  }
    else if (fDot < vMin[0]){  vMin[0] = fDot;  }
    
    // Since this is a symmetric box, we need to check only for 
    // max, since width becomes 2 * max.
    if ( fLength > vMax[1] ) 
    { 
      vMax[1]  = fLength; 
      vSecondaryAxis = vSecAxisCandidate;
    }
  }
  vMin[1] = -vMax[1];
  /////////////////////////////////////////////////////////////////
  // At this point, we have initial and secondary axis with extents.
  GSE_Vector3 vTertiaryAxis = vSecondaryAxis.Cross(vDir);
  vTertiaryAxis.ToUnit();
  vSecondaryAxis.ToUnit();
  fDot = 0.0f;
  /////////////////////////////////////////////////////////////////
  // For each corner in box one 
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obOne).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    fDot = vTertiaryAxis.Dot(vCorner - obOne.GetPosition());
    if (fDot > vMax[2])     {  vMax[2] = fDot;  }

  }
  
  // For each corner in box two
  for (i = 0; i < 8; i++)
  {
    // Calculate the length on initial axis
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obTwo).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    fDot = vTertiaryAxis.Dot(vCorner - obOne.GetPosition());
    if (fDot > vMax[2])     {  vMax[2] = fDot;  }
    
  }
  vMin[2] = -vMax[2];
  
  GSE_Vector3 vExtents(vMax - vMin);
  
  obMerge.SetPosition( ((obOne.GetPosition() + vDir * vMax[0]) +  
			(obOne.GetPosition() + vDir * vMin[0])) *0.5f)  ;

  obMerge.SetOrientation( vTertiaryAxis, vDir,  vSecondaryAxis );
  obMerge.SetLength( vExtents[0] );
  obMerge.SetWidth( vExtents[1] );
  obMerge.SetHeight( vExtents[2] );
  obMerge.CalculateCorners();
  obMerge.CalculatePlanes();
  return obMerge;
#else
  // construct a box that contains the input boxes
  GSE_OrientedBox obMerge;

  // The first guess at the box center.  This value will be updated later
  // after the input box vertices are projected onto axes determined by an
  // average of box axes.
  obMerge.SetPosition( 0.5f*(obOne.GetPosition() + obTwo.GetPosition()));
  
  // A box's axes, when viewed as the columns of a matrix, form a rotation
  // matrix.  The input box axes are converted to quaternions.  The average
  // quaternion is computed, then normalized to unit length.  The result is
  // the slerp of the two input quaternions with t-value of 1/2.  The result
  // is converted back to a rotation matrix and its columns are selected as
  // the merged box axes.
  GSE_Quaternion qOne, qTwo;
  
  GSE_Matrix4x4f mOne = OrientedBoxAxisToRotationMatrix( obOne );
  GSE_Matrix4x4f mTwo = OrientedBoxAxisToRotationMatrix( obTwo );

  qOne = RotationMatrixToQuaternion( mOne );
  qTwo = RotationMatrixToQuaternion( mTwo );

  if ( qOne.Dot(qTwo) < 0.0f) 
  {
    qTwo = -qTwo;
  }

  GSE_Quaternion  qQuat = qOne + qTwo;
  qQuat.Normalize();

  GSE_Matrix4x4f mFinal = qQuat.ToMatrix();

  GSE_Vector3 vUp(mFinal(0,2), mFinal(1,2), mFinal(2,2) );
  GSE_Vector3 vForward(mFinal(0,0), mFinal(1,0), mFinal(2,0) );
  GSE_Vector3 vRight(mFinal(0,1), mFinal(1,1), mFinal(2,1) );
  
  obMerge.SetOrientation( vUp, vForward, vRight);

  // Project the input box vertices onto the merged-box axes.  Each axis
  // D[i] containing the current center C has a minimum projected value
  // pmin[i] and a maximum projected value pmax[i].  The corresponding end
  // points on the axes are C+pmin[i]*D[i] and C+pmax[i]*D[i].  The point C
  // is not necessarily the midpoint for any of the intervals.  The actual
  // box center will be adjusted from C to a point C' that is the midpoint
  // of each interval,
  //   C' = C + sum_{i=0}^2 0.5*(pmin[i]+pmax[i])*D[i]
  // The box extents are
  //   e[i] = 0.5*(pmax[i]-pmin[i])

  int i = 0;
  float fDot = 0.0f;
  
  GSE_Vector3 vCorner, vDiff;
  GSE_Vector3 vMin(0,0,0);
  GSE_Vector3 vMax(0,0,0);
  
  // For each corner in box one 
  for (i = 0; i < 8; i++)
  {
    // Forward axis
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obOne).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetForwardVector() );
    if (fDot > vMax[GSE_Vector3::X])  {     vMax[GSE_Vector3::X] = fDot;    }
    else if (fDot < vMin[GSE_Vector3::X]){    vMin[GSE_Vector3::X] = fDot;  }

    // right axis 
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obOne).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetRightVector() );
    if (fDot > vMax[GSE_Vector3::Y])  {     vMax[GSE_Vector3::Y] = fDot;    }
    else if (fDot < vMin[GSE_Vector3::Y]){    vMin[GSE_Vector3::Y] = fDot;  }

    // Up axis 
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obOne).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetUpVector() );
    if (fDot > vMax[GSE_Vector3::Z])  {     vMax[GSE_Vector3::Z] = fDot;    }
    else if (fDot < vMin[GSE_Vector3::Z]){    vMin[GSE_Vector3::Z] = fDot;  }
    
  }
  
  // For each corner in box two
  for (i = 0; i < 8; i++)
  {
    // Forward axis
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obTwo).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetForwardVector() );
    if (fDot > vMax[GSE_Vector3::X])  {     vMax[GSE_Vector3::X] = fDot; }
    else if (fDot < vMin[GSE_Vector3::X]){  vMin[GSE_Vector3::X] = fDot; }

    // right axis 
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obTwo).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetRightVector() );
    if (fDot > vMax[GSE_Vector3::Y])  {     vMax[GSE_Vector3::Y] = fDot; }
    else if (fDot < vMin[GSE_Vector3::Y]){  vMin[GSE_Vector3::Y] = fDot; }

    // Up axis 
    vCorner.UseExternalData( const_cast<GSE_OrientedBox &>(obTwo).GetCorner( (GSE_OrientedBox::BoxCorner_t)i));
    vDiff = vCorner - obMerge.GetPosition();
    fDot = vDiff.Dot( obMerge.GetUpVector() );
    if (fDot > vMax[GSE_Vector3::Z])  {     vMax[GSE_Vector3::Z] = fDot; }
    else if (fDot < vMin[GSE_Vector3::Z]){  vMin[GSE_Vector3::Z] = fDot; }
    
  }
  
  // [kMin,kMax] is the axis-aligned box in the coordinate system of the
  // merged box axes.  Update the current box center to be the center of
  // the new box.  Compute the extent based on the new center.
  
  GSE_Vector3 vMean    = (vMax + vMin) * 0.5f;
  GSE_Vector3 vExtents = (vMax - vMin );
  
  obMerge.SetPosition( obMerge.GetPosition() + (vMean[GSE_Vector3::X] *obMerge.GetForwardVector()));
  obMerge.SetLength( vExtents[GSE_Vector3::X] );
  
  obMerge.SetPosition( obMerge.GetPosition() + (vMean[GSE_Vector3::Y]*obMerge.GetRightVector()));
  obMerge.SetWidth( vExtents[GSE_Vector3::Y] );
  
  obMerge.SetPosition( obMerge.GetPosition() + (vMean[GSE_Vector3::Z]*obMerge.GetUpVector()));
  obMerge.SetHeight( vExtents[GSE_Vector3::Z] );

  obMerge.CalculatePlanes();
  obMerge.CalculateCorners();
  return obMerge;
#endif
}
/////////////////////////////////////////////////////////////////
float 
Geometry::PointDistanceFromPlane( GSE_Vector3 &vPoint, GSE_Plane plane )
{
  
  GSE_Vector3 vNormal;

  vNormal.UseExternalData(plane.m_pValues);

  float fDistance = vNormal.Dot(vPoint) + plane.Distance();

  return fDistance;

}
/////////////////////////////////////////////////////////////////
char	
Geometry::PointWithinKDOP( GSE_Vector3 &vPoint, GSE_KDOP &kDop )
{

  std::list<GSE_Plane>::iterator it = kDop.Planes().begin();

  // Check the distance to each plane
  for(;it!= kDop.Planes().end();it++)
  {
    // In case the point is behind some plane, it cannot be within the area 
    // limited by kDop.
    
    if ( PointDistanceFromPlane( vPoint, *it ) < 0.0f){
      return 0;
    }
  }

  // point is in front of every plane, so it is inside.
  return 1;
}
/////////////////////////////////////////////////////////////////
GSE_Matrix4x4f
Geometry::OrientedBoxAxisToRotationMatrix ( const GSE_OrientedBox &ob )
{

  GSE_Matrix4x4f mMatrix = GSE_Matrix4x4f::Identity();

  mMatrix(0,0) = ob.GetForwardVector().m_pValues[0];
  mMatrix(1,0) = ob.GetForwardVector().m_pValues[1];
  mMatrix(2,0) = ob.GetForwardVector().m_pValues[2];

  mMatrix(0,1) = ob.GetRightVector().m_pValues[0];
  mMatrix(1,1) = ob.GetRightVector().m_pValues[1];
  mMatrix(2,1) = ob.GetRightVector().m_pValues[2];

  mMatrix(0,2) = ob.GetUpVector().m_pValues[0];
  mMatrix(1,2) = ob.GetUpVector().m_pValues[1];
  mMatrix(2,2) = ob.GetUpVector().m_pValues[2];
  
  return mMatrix;
}
/////////////////////////////////////////////////////////////////
Geometry::GSE_IntersectionType  
Geometry::SphereIntersectsKDOP( GSE_Sphere &Sphere, GSE_KDOP &kDop )
{
  /////////////////////////////////////////////////////////////////
  Geometry::GSE_PlaneIntersection iRel;
  float fTmp = 0.0f;
  list<GSE_Plane>::iterator planeIt = kDop.Planes().begin();
  /////////////////////////////////////////////////////////////////
  for( ;planeIt!=kDop.Planes().end(); planeIt++)
  {
    iRel = Geometry::SphereIntersectsPlane( *planeIt, Sphere, fTmp);
    // If the object is behind of any of the planes, 
    // then it is outside the KDOP
    if (iRel == Geometry::BEHIND)
    {
      return Geometry::OUTSIDE;
    }
    // If the object intersects any of the planes, 
    // then it is intersecting the frustum
    if (iRel == Geometry::INTERSECTS) 
    {
      return Geometry::INTERSECTION;
    }
  }  
  /////////////////////////////////////////////////////////////////
  // if we end up here, the object is neither behind or intersecting of any of the 
  // planes. Hence, it is inside.
  return INSIDE;
}
/////////////////////////////////////////////////////////////////
Geometry::GSE_IntersectionType  
Geometry::OrientedBoxIntersectsKDOP( GSE_OrientedBox &obBox, GSE_KDOP &kDop )
{
 
  char bUseLineSegmentTest = 0;
  
  // The effective radius of the box respect to a plane
  float fEffRadius	= 0.0f;
  float fDot		= 0.0f;
  float fDot2		= 0.0f;

  GSE_Vector3 vNormal;
  
  // When box length (principal axis) is much greater than the two others,
  // it is better to use line segment test for better accuracy.
  bUseLineSegmentTest = (( obBox.GetLength()-obBox.GetWidth() ) > (0.5f * obBox.GetWidth())) || 
  ( (obBox.GetLength()-obBox.GetHeight()) > (0.5f * obBox.GetHeight()));
  
  if ( bUseLineSegmentTest ) 
  {
    
    // The line seqment
    GSE_Vector3 vQ1 = obBox.GetPosition() + (obBox.GetForwardVector()*(obBox.GetLength() * 0.5f)) ;
    GSE_Vector3 vQ2 = vQ1 - (obBox.GetForwardVector()*obBox.GetLength()) ;
    list<GSE_Plane>::iterator planeIt = kDop.Planes().begin();
    
    for( ; planeIt!=kDop.Planes().end(); planeIt++)
    {
      
      vNormal.UseExternalData( (*planeIt).m_pValues);
      // Since axes from GSE_Orientable are always Unit length,
      // we include proper dimensions in the equation.
      fEffRadius = 0.5f * ( fabsf( (obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal) ) +
			    fabsf( (obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal) ) );
      
      // Calculate 4D dot product between a plane and the line endpoints
      fDot  = vNormal.Dot( vQ1) + (*planeIt)[GSE_Plane::D];
      fDot2 = vNormal.Dot( vQ2) + (*planeIt)[GSE_Plane::D];

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
    
  } 
  else 
  {

    list<GSE_Plane>::iterator planeIt = kDop.Planes().begin();
    for( ;planeIt!=kDop.Planes().end(); planeIt++)
    {
      
      vNormal.UseExternalData( (*planeIt).m_pValues);

      // Since axes from GSE_Orientable are always Unit length,
      // we include proper dimensions in the equation.
      
      fEffRadius = 0.5 * ( fabsf((obBox.GetForwardVector()*obBox.GetLength()).Dot(vNormal)) +
			   fabsf((obBox.GetRightVector()*obBox.GetWidth()).Dot(vNormal)) +
			   fabsf((obBox.GetUpVector()*obBox.GetHeight()).Dot(vNormal)) );
      
      // Calculate 4D dot product between plane and box center
      fDot  = vNormal.Dot( obBox.GetPosition()) + (*planeIt)[GSE_Plane::D];

      if ( fDot <= -fEffRadius )
      {
	return OUTSIDE;
      }
      
    }  
  }
  
  return INTERSECTION;
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_geometry.cpp,v $
// Revision 1.30  2007/05/20 15:10:05  entity
// ClosestPointOnRay  + more optimal MergeOBB
//
// Revision 1.29  2007/05/19 19:42:58  entity
// copy constructor-assignment operator fix on CalculateCorners
//
// Revision 1.28  2007/05/19 11:28:13  entity
// oriented box merge
//
// Revision 1.27  2007/05/18 08:08:09  entity
// Vector3 constructor check & fixes
//
// Revision 1.26  2007/05/11 11:47:06  entity
// a hopefully more accurate bounding volume calculation
//
// Revision 1.25  2007/05/10 21:01:47  entity
// RayIntersectsOBB improved
//
// Revision 1.24  2007/05/10 19:12:28  entity
// RayIntersectsOBB added
//
// Revision 1.23  2007/04/06 11:52:11  entity
// undef removed
//
// Revision 1.22  2007/04/03 12:52:59  entity
// store_max_min macro move
//
// Revision 1.21  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
// Revision 1.20  2007/03/30 08:14:18  entity
// fixed cornerMax-cornerMin bug in PlaneIntersectsAABB(...)
//
// Revision 1.19  2007/03/29 06:35:59  entity
// CalculatAlignedBox() added
//
/////////////////////////////////////////////////////////////////
