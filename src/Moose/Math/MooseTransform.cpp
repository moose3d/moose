#include "MooseTransform.h"
#include "MooseMath.h"
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Moose::Math;
using namespace std;
/////////////////////////////////////////////////////////////////
// void 
// CTransform::AddHook( CMatrix4x4f *pMatrix)
// {
//   m_pTransformHook = pMatrix;
// }
/////////////////////////////////////////////////////////////////
const CMatrix4x4<float> &
Moose::Math::CTransform::GetMatrix() 
{
  // if ( m_pTransformHook != NULL ) return *m_pTransformHook;
  if ( m_bChanged )
  {
    CMatrix4x4<float> mTmp;
    // Constructs SRT-transform (scale first). 
    ScaleMatrix(m_vScaling, m_mTransform);
    QuaternionToMatrix(m_qRotation, mTmp);
    m_mTransform  = mTmp * m_mTransform;
    //TranslationMatrix( m_vTranslation, mTmp );
    // Concatenate transforms 
    //m_mTransform =  mTmp * m_mTransform;
      m_mTransform(0,3) = m_vTranslation[0];
      m_mTransform(1,3) = m_vTranslation[1];
      m_mTransform(2,3) = m_vTranslation[2];
      m_bChanged = 0;
  }
  return m_mTransform;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetTranslation( const CMatrix4x4<float> &mMatrix )
{
  m_vTranslation[0] = mMatrix(0,3);
  m_vTranslation[1] = mMatrix(1,3);
  m_vTranslation[2] = mMatrix(2,3);
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetTranslation( const CVector3<float> &vTranslation )
{
  m_vTranslation = vTranslation;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetTranslation( float fX, float fY, float fZ )
{
  m_vTranslation[0] = fX;
  m_vTranslation[1] = fY;
  m_vTranslation[2] = fZ;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetRotation( const CMatrix4x4<float> &mMatrix )
{
  RotationMatrixToQuaternion( mMatrix, m_qRotation );
  m_bChanged = 1;  
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetRotation( const CQuaternion &qRotation )
{
  m_qRotation = qRotation;
  m_bChanged = 1;  
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetRotation( float fX, float fY, float fZ )
{
  CQuaternion qX,qY,qZ;
  qX.CreateFromAxisAngleRad(1,0,0,fX);
  qY.CreateFromAxisAngleRad(0,1,0,fY);
  qZ.CreateFromAxisAngleRad(0,0,1,fZ);
  
  m_qRotation = qZ * qY * qX;
  m_bChanged = 1;  
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetRotation( const CVector3<float> &vAxisAngles )
{
  SetRotation( vAxisAngles[0], vAxisAngles[1], vAxisAngles[2] );
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CQuaternion &
Moose::Math::CTransform::GetRotation( ) const
{
  return m_qRotation;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::SetScaling( float fScale )
{
    SetScaling(fScale,fScale,fScale);
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CTransform::SetScaling( float fX, float fY, float fZ)
{
  m_vScaling[0] = fX;
  m_vScaling[1] = fY;
  m_vScaling[2] = fZ;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CTransform::SetScaling( const CVector3<float> & vScale )
{
  m_vScaling = vScale;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
const CVector3<float> &
Moose::Math::CTransform::GetScaling() const
{
  return m_vScaling;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::Move( const CVector3<float> &vVector )
{
  m_vTranslation += vVector;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::Move( float fX, float fY, float fZ)
{
  m_vTranslation[0] += fX;
  m_vTranslation[1] += fY;
  m_vTranslation[2] += fZ;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CTransform::Rotate( const CQuaternion & q )
{
  m_qRotation = q * m_qRotation;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CVector3<float> & 
Moose::Math::CTransform::GetTranslation() const
{
  return m_vTranslation;
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::CTransform::SetIdentity()
{
  m_bChanged = false;
  m_vScaling[0] = m_vScaling[1] = m_vScaling[2] = 1.0f;
  m_mTransform.IdentityMatrix();
  m_qRotation.Identity();
  m_vTranslation[0] = m_vTranslation[1] = m_vTranslation[2] = 0.0f;
}
/////////////////////////////////////////////////////////////////
void
Moose::Math::Multiply( const Moose::Math::CTransform & rTLeft,
													  const Moose::Math::CTransform & rTRight,
														Moose::Math::CTransform & rTransformResult )
{
  CMatrix4x4<float> mResult;
  CVector3<float> vRightTransl  = rTRight.GetTranslation();

  // Set combined scaling and rotation
  rTransformResult.SetScaling( rTRight.GetScaling()*rTLeft.GetScaling());
  rTransformResult.SetRotation( rTLeft.GetRotation()*rTRight.GetRotation());
  // Set translation
  
RotateVector( rTLeft.GetRotation(), vRightTransl );
    vRightTransl *= rTLeft.GetScaling();
  rTransformResult.SetTranslation( vRightTransl + rTLeft.GetTranslation());
}
////////////////////////////////////////////////////////////////////////////////
Moose::Math::CVector3<float>
Moose::Math::CTransform::GetForwardVector()
{
  const CMatrix4x4<float> & matrix = GetMatrix();
  return CVector3<float>(matrix(0,2), matrix(1,2), matrix(2,2));
}
////////////////////////////////////////////////////////////////////////////////
Moose::Math::CVector3<float>
Moose::Math::CTransform::GetRightVector() 
{
  const CMatrix4x4<float> & matrix = GetMatrix();
  return CVector3<float>(matrix(0,0), matrix(1,0), matrix(2,0));
}
////////////////////////////////////////////////////////////////////////////////
Moose::Math::CVector3<float>
Moose::Math::CTransform::GetUpVector() 
{
  const CMatrix4x4<float> & matrix = GetMatrix();
  return CVector3<float>(matrix(0,1), matrix(1,1), matrix(2,1));
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Math::CTransform::Scale( const CVector3<float> & vScale )
{
  m_vScaling += vScale;
  SetChanged(true);
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Math::CTransform::Scale( float fX, float fY, float fZ )
{
  m_vScaling[0] += fX;
  m_vScaling[1] += fY;
  m_vScaling[2] += fZ;  
  SetChanged(true);
}
////////////////////////////////////////////////////////////////////////////////
