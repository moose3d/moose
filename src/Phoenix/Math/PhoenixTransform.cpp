#include "PhoenixTransform.h"
#include "PhoenixMath.h"
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
using namespace std;
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
    CMatrix4x4<float> mTmp;
    // Constructs STR-transform.
    // First, convert quaterion into matrix (use m_mTransform).
    QuaternionToMatrix(m_qRotation, m_mTransform);
    TranslationMatrix( m_vTranslation, mTmp );
    // Concatenate transforms 
    m_mTransform =  mTmp * m_mTransform; 
    UniformScaleMatrix(m_fScaling, mTmp);
    m_mTransform = m_mTransform * mTmp;
    m_bChanged = 0;
  }
  return m_mTransform;
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
  RotationMatrixToQuaternion( mMatrix, m_qRotation );
  m_bChanged = 1;  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetRotation( const CQuaternion &qRotation )
{
  m_qRotation = qRotation;
  m_bChanged = 1;  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetRotation( float fX, float fY, float fZ )
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
Phoenix::Math::CTransform::SetRotation( const CVector3<float> &vAxisAngles )
{
  SetRotation( vAxisAngles[0], vAxisAngles[1], vAxisAngles[2] );
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CQuaternion &
Phoenix::Math::CTransform::GetRotation( ) const
{
  return m_qRotation;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetScaling( float fScale )
{
  m_fScaling = fScale;
  m_bChanged = 1;
}
/////////////////////////////////////////////////////////////////
float
Phoenix::Math::CTransform::GetScaling() const
{
  return m_fScaling;
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
void
Phoenix::Math::CTransform::SetIdentity()
{
  m_bChanged = false;
  m_fScaling = 1.0f;
  m_mTransform.IdentityMatrix();
  m_qRotation.Identity();
  m_vTranslation[0] = m_vTranslation[1] = m_vTranslation[2] = 0.0f;
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CTransformable::CTransformable()
{
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CTransform & 
Phoenix::Math::CTransformable::GetLocalTransform()
{
  return m_LocalTransform;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CTransform & 
Phoenix::Math::CTransformable::GetLocalTransform() const
{
  return m_LocalTransform;
}
/////////////////////////////////////////////////////////////////
Phoenix::Math::CTransform & 
Phoenix::Math::CTransformable::GetWorldTransform()
{
  return m_WorldTransform;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CTransform & 
Phoenix::Math::CTransformable::GetWorldTransform() const
{
  return m_WorldTransform;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransformable::SetLocalTransform( const Phoenix::Math::CTransform & rTransform )
{
  m_LocalTransform = rTransform;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransformable::SetWorldTransform( const Phoenix::Math::CTransform & rTransform )
{
  m_WorldTransform = rTransform;
  SetChanged(true);
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Math::CTransform::IsChanged() const
{
  return m_bChanged;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransform::SetChanged(bool bFlag )
{
  m_bChanged = bFlag;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::Multiply( const Phoenix::Math::CTransform & rTLeft, 
			 const Phoenix::Math::CTransform & rTRight, 
			 Phoenix::Math::CTransform & rTransformResult )
{
  CMatrix4x4<float> mResult;
  CVector3<float> vRightTransl  = rTRight.GetTranslation();

  // Set combined scaling and rotation
  rTransformResult.SetScaling( rTRight.GetScaling()*rTLeft.GetScaling());
  rTransformResult.SetRotation( rTLeft.GetRotation()*rTRight.GetRotation());
  vRightTransl *= rTLeft.GetScaling();
  // Set translation
  RotateVector( rTLeft.GetRotation(), vRightTransl );
  rTransformResult.SetTranslation( vRightTransl + rTLeft.GetTranslation());
  
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Math::CTransformable::IsChanged() const
{
  return m_bChanged;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransformable::SetChanged(bool bFlag )
{
  m_bChanged = bFlag;
}
/////////////////////////////////////////////////////////////////
