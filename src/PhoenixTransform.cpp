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
    // Constructs STR-transform.
    // First, convert quaterion into matrix (use m_mTransform).
    QuaternionToMatrix(m_qRotation, m_mTransform);
    // Concatenate transforms 
    m_mTransform =  UniformScaleMatrix(m_fScaling)*
                    TranslationMatrix( m_vTranslation ) * 
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
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::CTransformable::SetWorldTransform( const Phoenix::Math::CTransform & rTransform )
{
  m_WorldTransform = rTransform;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Math::Multiply( const Phoenix::Math::CTransform & rTransformRight, 
			 const Phoenix::Math::CTransform & rTransformLeft, 
			 Phoenix::Math::CTransform & rTransformResult )
{
  CMatrix4x4<float> mResult;
  CVector3<float> vRightTransl = rTransformRight.GetTranslation();
  CVector3<float> vRightTransl2 = rTransformRight.GetTranslation();
  // Set combined scaling and rotation
  rTransformResult.SetScaling( rTransformRight.GetScaling()*rTransformLeft.GetScaling());
  rTransformResult.SetRotation( rTransformRight.GetRotation()*rTransformLeft.GetRotation());
  
  // Set translation
  RotateVector( rTransformLeft.GetRotation(), vRightTransl );
  cerr << "Rotated with q: " << vRightTransl << endl; 
  
  QuaternionToMatrix(rTransformLeft.GetRotation(), mResult);
  cerr << "Rotated with matrix: " << Rotate( vRightTransl2, mResult ) << endl; 
  
  rTransformResult.SetTranslation( (rTransformLeft.GetScaling()*vRightTransl) + rTransformLeft.GetTranslation());
  
}
/////////////////////////////////////////////////////////////////
