#include "PhoenixVolume.h"
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
Phoenix::Math::COrientedBox &
Phoenix::Math::COrientedBox::SetOrientation(const CVector3 &vUp, 
					    const CVector3 &vForward,
					    const CVector3 &vRight)
{
  m_vUpward  = vUp;
  m_vRight   = vRight;
  m_vForward = vForward;
  return *this;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::COrientedBox::CalculatePlanes()
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
Phoenix::Math::COrientedBox::CalculateCorners()
{
  CVector3 vTmp;
  CVector3 vTmp2;

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

  vTmp.UseExternalData( GetCorner( COrientedBox::BOTTOM_LEFT_BACK ));
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
Phoenix::Math::COrientedBox::GetCorners() const
{
  return m_aCorners;
}
/////////////////////////////////////////////////////////////////
CVector3<float> 
Phoenix::Math::COrientedBox::GetCorner( BBOX_CORNER_TYPE tCorner )
{
  CVector3<float> m_vVector;
  m_vVector.UseExternalData( &(m_aCorners[tCorner]) );
  return m_vVector;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CCone::SetAngle( float fAngle )
{
  m_fAngle = fAngle;
  m_fSinAngle = sinf(m_fAngle);
  m_fCosAngle = cosf(m_fAngle);
  m_fSinReciprocal = (1.0f / m_fSinAngle);
  m_fCosAngleSqr = m_fCosAngle * m_fCosAngle;
  m_fSinAngleSqr = m_fSinAngle * m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
CPhoenix::Math::CCone::Cone() : CPositional(), m_vDirection(CVector3<float>(0,0,-1.0f)),m_fLength(1.0f)
{
  SetAngle(0.0f);
}
/////////////////////////////////////////////////////////////////
CPhoenix::Math::CCone::CCone( const CVector3<float> &vPos, const CVector3<float> &vDir, float fAngle, float fLength ) : CPositional(vPos),m_vDirection(vDir)
{
  SetAngle(fAngle);
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
