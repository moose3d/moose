#include "PhoenixVolume.h"
#include "PhoenixGlobals.h"
#include "PhoenixMatrix3x3.h"
#include "PhoenixMath.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Volume;
using namespace Phoenix::Graphics;
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
void
Phoenix::Volume::COrientedBox::SetOrientation(const CVector3<float> &vUp, 
					      const CVector3<float> &vForward,
					      const CVector3<float> &vRight)
{
  m_vUpward  = vUp;
  m_vRight   = vRight;
  m_vForward = vForward;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Volume::COrientedBox::CalculatePlanes()
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
Phoenix::Volume::COrientedBox::CalculateCorners()
{
  CVector3<float> vTmp;

  vTmp =  GetPosition();
  vTmp += GetForwardVector()*GetHalfLength();
  vTmp -= GetRightVector()*GetHalfWidth(); 
  vTmp += GetUpVector()*GetHalfHeight(); 
  
  GetCorner( TOP_LEFT_FRONT ) = vTmp;
  vTmp += GetRightVector()*GetWidth();
  GetCorner( TOP_RIGHT_FRONT ) = vTmp;

  vTmp += -GetForwardVector()*GetLength();
  GetCorner( TOP_RIGHT_BACK ) = vTmp;

  vTmp += -GetRightVector()*GetWidth();
  GetCorner( TOP_LEFT_BACK ) = vTmp;

  vTmp += -GetUpVector()*GetHeight();
  GetCorner( BOTTOM_LEFT_BACK ) = vTmp;

  vTmp += GetRightVector()*GetWidth();
  GetCorner( BOTTOM_RIGHT_BACK ) = vTmp;

  vTmp += GetForwardVector()*GetLength();
  GetCorner( BOTTOM_RIGHT_FRONT ) = vTmp;

  vTmp += -GetRightVector()*GetWidth();
  GetCorner( BOTTOM_LEFT_FRONT ) = vTmp;
}
/////////////////////////////////////////////////////////////////
const float *
Phoenix::Volume::COrientedBox::GetCorners() const
{
  return m_aCorners;
}
/////////////////////////////////////////////////////////////////
CVector3<float> 
Phoenix::Volume::COrientedBox::GetCorner( BBOX_CORNER_TYPE tCorner )
{
  CVector3<float> m_vVector;
  m_vVector.UseExternalData( &m_aCorners[tCorner*3] );
  return m_vVector;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Volume::CCone::SetAngle( float fAngle )
{
  m_fAngle = fAngle;
  m_fSinAngle = sinf(m_fAngle);
  m_fCosAngle = cosf(m_fAngle);
  m_fSinReciprocal = (1.0f / m_fSinAngle);
  m_fCosAngleSqr = m_fCosAngle * m_fCosAngle;
  m_fSinAngleSqr = m_fSinAngle * m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CCone::CCone() : CPositional(), m_vDirection(CVector3<float>(0,0,-1.0f)),m_fLength(1.0f)
{
  SetAngle(0.0f);
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CCone::CCone( const CVector3<float> &vPos, const CVector3<float> &vDir, float fAngle, float fLength ) : CPositional(vPos),m_vDirection(vDir)
{
  SetAngle(fAngle);
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CSphere 
Phoenix::Volume::CalculateBoundingSphere( const Phoenix::Graphics::CVertexDescriptor &vd )
{
  if ( vd.GetType() != ELEMENT_TYPE_VERTEX_3F ) 
  {
    return CSphere(CVector3<float>(0,0,0), 0.0f);
  }

  // Minimum and maximum values for each coordinates.
  CVector3<float> vMaxValues;
  CVector3<float> vMinValues;
  unsigned int nNumVertices = vd.GetSize();
  
  float *pVertices = vd.GetPointer<float>();
  
  for ( unsigned int v = 0;v<nNumVertices; v++)
  {
    float fTempX = pVertices[(v*3)];
    float fTempY = pVertices[(v*3)+1];
    float fTempZ = pVertices[(v*3)+2];

    std::cerr << "current pos: (" << fTempX << "," << fTempY << "," << fTempZ << ")" << std::endl;
    
    if ( v == 0 )
    {
      vMaxValues[0] = vMinValues[0] = fTempX;
      vMaxValues[1] = vMinValues[1] = fTempY;
      vMaxValues[2] = vMinValues[2] = fTempZ;
    } 
    else 
    {
      STORE_MAX_MIN(fTempX, vMaxValues[0], vMinValues[0]);
      STORE_MAX_MIN(fTempY, vMaxValues[1], vMinValues[1]);
      STORE_MAX_MIN(fTempZ, vMaxValues[2], vMinValues[2]);
    }
  }
  std::cerr << "Min extent: " << vMinValues << std::endl;
  std::cerr << "Max extent: " << vMaxValues << std::endl;
  CSphere sphere;
  sphere.SetPosition( (vMaxValues + vMinValues ) / 2 );
  CVector3<float> vDist = vMaxValues - vMinValues;
  
  float fMaxRadius = 0.0; 

  if ( fabs(vDist[0]) > fabs(vDist[1])){

    if ( fabs(vDist[0] > fabs(vDist[2]))){
      fMaxRadius = vDist[0] * 0.5f;
    } else {
      fMaxRadius = vDist[2] * 0.5f;
    }

  } else {

    if ( fabs(vDist[1] > fabs(vDist[2]))){
      fMaxRadius = vDist[1] * 0.5f;
    } else {
      fMaxRadius = vDist[2] * 0.5f;
    }

  }
  sphere.SetRadius( fMaxRadius  );
  return sphere;
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CSphere 
Phoenix::Volume::CalculateBoundingSphereTight( const Phoenix::Graphics::CVertexDescriptor &vd )
{

  // The returned sphere

  if ( vd.GetType() != ELEMENT_TYPE_VERTEX_3F ) 
  {
    return CSphere( CVector3<float>(0,0,0), 0.0f);
  }
  CSphere sphere;
  unsigned int nNumVertices = vd.GetSize();
  
  float fLambda1,fLambda2,fLambda3;  
  
  // Calculate the principal axis R by component analysis
  CMatrix3x3<float> mCovar = CovarianceMatrix( vd  );
  CMatrix3x3<float> mEigenVectorMatrix;
  mEigenVectorMatrix.IdentityMatrix();
  CalculateEigensystem( mCovar, 
			fLambda1, fLambda2,fLambda3,
			mEigenVectorMatrix);
  
  CVector3<float> vR = GetColumnVector(mEigenVectorMatrix, 0);
  vR.Normalize();
  // Get min and max values of each point in direction R
  CVector3<float> vTemp;

  float fMinExtent, fMaxExtent, fTempDotR;
  unsigned int nMaxIndex = 0, nMinIndex = 0;

  // Initialize the first values
  vTemp.UseExternalData( &(vd.GetPointer<float>()[0]));
  fMinExtent = fMaxExtent = vTemp.Dot(vR);
  CVector3<float> vMin = vTemp;
  CVector3<float> vMax = vTemp;
  
  // for each vertex
  for ( unsigned int v = 1;v<nNumVertices; v++)
  {
    
    vTemp.UseExternalData( &(vd.GetPointer<float>()[v*3]));
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
  vMin.UseExternalData(&vd.GetPointer<float>()[nMinIndex*3]);
  vMax.UseExternalData(&vd.GetPointer<float>()[nMaxIndex*3]);
  
  //sphere.SetPosition( (vMin + vMax ) * 0.5f );
  
  CVector3<float> vPos = vMin + ((vMax-vMin) * 0.5f);
  sphere.SetPosition( vPos );
  sphere.SetRadius((vMax - sphere.GetPosition()).Length());  
  
  // Check each vertex that they actually are stored inside the sphere
  float fRadiusSquared = sphere.GetRadius() * sphere.GetRadius();
  
  // Temporary point when determining new center and radius
  CVector3<float> vGPoint;

  // The vector from center of the sphere to the current vertex
  CVector3<float> vCenterToPoint;

  // For each vertex
  for ( unsigned int v = 0;v<nNumVertices; v++)
  {
    vTemp.UseExternalData( &(vd.GetPointer<float>()[v*3]));

    float fDist = ((vTemp - sphere.GetPosition()).Length());
    float fDistSquared = fDist * fDist;

    if (  fDistSquared > fRadiusSquared )
    {
      // Point lies outside sphere, adjust center and radius
      vCenterToPoint = (vTemp - sphere.GetPosition());
      vCenterToPoint.Normalize();
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
