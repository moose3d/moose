#include "PhoenixTransform.h"
#include "PhoenixMath.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
// void 
// CTransform::AddHook( CMatrix4x4f *pMatrix)
// {
//   m_pTransformHook = pMatrix;
// }
/////////////////////////////////////////////////////////////////
const CMatrix4x4<float> &
Phoenix::Math::CTransform::GetMatrix() 
{
  // if ( m_pTransformHook != NULL ) return *m_pTransformHook;
  if ( m_bChanged )
  {
    // First, convert quaterion into matrix (use m_mTransform).
    QuaternionToMatrix(m_qRotation, m_mTransform);
    // Concatenate transforms 
    m_mTransform =  UniformScaleMatrix(m_fScaling) * 
                    TranslationMatrix( m_vTranslation )  * 
                    m_mTransform;
    m_bChanged = 0;
  }
  return  m_mTransform;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetTranslation( const CMatrix4x4<float> &mMatrix )
{
  m_vTranslation[0] = mMatrix(0,3);
  m_vTranslation[1] = mMatrix(1,3);
  m_vTranslation[2] = mMatrix(2,3);
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetTranslation( const CVector3<float> &vTranslation )
{
  m_vTranslation = vTranslation;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetTranslation( float fX, float fY, float fZ )
{
  m_vTranslation[0] = fX;
  m_vTranslation[1] = fY;
  m_vTranslation[2] = fZ;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetRotation( const CMatrix4x4<float> &mMatrix )
{
  m_qRotation = RotationMatrixToQuaternion( mMatrix );
  m_bChanged = 1;  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetRotation( const CQuaternion &qRotation )
{
  m_qRotation = qRotation;
  m_bChanged = 1;  
}
void 
Phoenix::Math::CTransform::SetRotation( float fX, float fY, float fZ )
{
  CQuaternion qX,qY,qZ;
  qX.CreateFromAxisAngle(1,0,0,fX);
  qY.CreateFromAxisAngle(0,1,0,fY);
  qZ.CreateFromAxisAngle(0,0,1,fZ);
  
  m_qRotation = qZ * qY * qX;
  m_bChanged = 1;  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetRotation( const CVector3<float> &vAxisAngles )
{
  SetRotation( vAxisAngles[0], vAxisAngles[1], vAxisAngles[2] );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetScaling( float fScale )
{
  m_fScaling = fScale;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::Move( const CVector3<float> &vVector )
{
  m_vTranslation += vVector;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::Move( float fX, float fY, float fZ)
{
  m_vTranslation[0] += fX;
  m_vTranslation[1] += fY;
  m_vTranslation[2] += fZ;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Math::CTransform::Rotate( const CQuaternion & q )
{
  m_qRotation = q * m_qRotation;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CVector3<float> & 
Phoenix::Math::CTransform::GetTranslation() const
{
  return m_vTranslation;
}
/////////////////////////////////////////////////////////////////
