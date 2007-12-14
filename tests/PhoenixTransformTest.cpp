#include <UnitTest++/UnitTest++.h>
#include "PhoenixTransform.h"
#include "PhoenixMath.h"
#include <string>
#include <iostream>
using namespace Phoenix::Math;
using namespace std;
/////////////////////////////////////////////////////////////////
TEST( CTransform )
{
  CTransform transform;
  CMatrix4x4<float> mId;
  CVector3<float> vTransl(0,0,0);
  CQuaternion q;
  // Check initial state ( scale = 1, matrix = id, no rotation);
  mId.IdentityMatrix();
  q.Identity();

  CHECK_ARRAY_CLOSE( mId.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetTranslation_Matrix )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vTransl(1,4,-5);
  CQuaternion q;
  q.Identity();  
  // Translate by matrix.
  mMatrix = Phoenix::Math::TranslationMatrix( vTransl );


  transform.SetTranslation( mMatrix );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetTranslation_Vector3 )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vTransl(1,4,-5);
  CQuaternion q;
  q.Identity();  
  // Translate by vector
  mMatrix = Phoenix::Math::TranslationMatrix( vTransl );


  transform.SetTranslation( vTransl );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetTranslation_3_Floats )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vTransl(1,4,-5);
  CQuaternion q;
  q.Identity();  
  // Translate by values
  mMatrix = Phoenix::Math::TranslationMatrix( vTransl );


  transform.SetTranslation( vTransl[0], vTransl[1], vTransl[2] );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetRotation_Matrix )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(1,4,-5);
  CVector3<float> vTransl(0,0,0);
  CQuaternion q;
  // Rotate by 3 rotations 
  mMatrix = Phoenix::Math::RotationMatrix( vRot );
  q = RotationMatrixToQuaternion( mMatrix );

  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( mMatrix );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetRotation_Quaternion )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(1,4,-5);
  CVector3<float> vTransl(0,0,0);
  CQuaternion q;
  // Rotate by 3 rotations 
  mMatrix = Phoenix::Math::RotationMatrix( vRot );
  q = RotationMatrixToQuaternion( mMatrix );

  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( q );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetRotation_3_Floats )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(1,4,-5);
  CVector3<float> vTransl(0,0,0);
  CQuaternion q;
  // Rotate by 3 rotations 
  mMatrix = Phoenix::Math::RotationMatrix( vRot );
  q = RotationMatrixToQuaternion( mMatrix );
  
  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( vRot[0], vRot[1], vRot[2] );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetRotation_Vector3 )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(1,4,-5);
  CVector3<float> vTransl(0,0,0);
  CQuaternion q;
  // Rotate by 3 rotations 
  mMatrix = Phoenix::Math::RotationMatrix( vRot );
  q = RotationMatrixToQuaternion( mMatrix );
  
  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( vRot );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetScaling )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(0,0,0);
  CVector3<float> vTransl(0,0,0);
  CQuaternion q;
  float fScaleValue = 3.0f;
  transform.SetScaling(fScaleValue);
  mMatrix.IdentityMatrix();
  mMatrix(0,0) = mMatrix(1,1) = mMatrix(2,2) = 3.0f;
  q.Identity();
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( fScaleValue, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_Move_Vector )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(0,0,0);
  CVector3<float> vTranslReal(1,2,3);

  CQuaternion q;
  q.Identity();

  mMatrix = TranslationMatrix( vTranslReal );
  transform.Move( CVector3<float>(1,0,0) );
  transform.Move( CVector3<float>(0,2,0) );
  transform.Move( CVector3<float>(0,0,3) );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTranslReal.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_Move_3_Floats )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(0,0,0);
  CVector3<float> vTranslReal(1,2,3);

  CQuaternion q;
  q.Identity();

  mMatrix = TranslationMatrix( vTranslReal );
  transform.Move( 1,0,0 );
  transform.Move( 0,2,0 );
  transform.Move( 0,0,3 );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTranslReal.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_Rotate )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vRot(0,0,0);
  CVector3<float> vTransl(0,0,0);

  CQuaternion q, qFinal;
  q.Identity();

  mMatrix.IdentityMatrix();

  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);

  mMatrix = RotationMatrix( 4,0,0 );
  q.CreateFromAxisAngleRad( 1,0,0, 2 );
  qFinal.CreateFromAxisAngleRad( 1,0,0, 4 );
  transform.Rotate( q );
  transform.Rotate( q );
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_CLOSE( 1.0f, transform.GetScaling(), 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( qFinal.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_Multiply )
{
  CTransform transform1;
  CTransform transform2;
  CTransform transformResult;
  
  CVector3<float> vTransl1(0,2,0);
  CVector3<float> vTransl2(0,0,3);

  CVector3<float> vRot1(0,45,0);
  CVector3<float> vRot2(33,0,0);
  
  float fScale1 = 3.0f;
  float fScale2 = 2.0f;
  
  transform1.SetScaling(fScale1);
  transform1.SetRotation(vRot1);
  transform1.SetTranslation(vTransl1);

  transform2.SetScaling(fScale2);
  transform2.SetRotation(vRot2);
  transform2.SetTranslation(vTransl2);

  Phoenix::Math::Multiply( transform1, transform2, transformResult );
  
  cerr << "tr matrix:" << endl 
       << transform1.GetMatrix()*transform2.GetMatrix() << endl;
  cerr << "tr result:" << endl 
       << transformResult.GetMatrix() << endl;

  
  CHECK_ARRAY_CLOSE( (transform1.GetMatrix()*transform2.GetMatrix()).GetArray(), transformResult.GetMatrix().GetArray(), 16, 0.001f);
} 
/////////////////////////////////////////////////////////////////
