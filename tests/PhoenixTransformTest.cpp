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
