/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include <UnitTest++/UnitTest++.h>
#include <MooseTransform.h>
#include <MooseMath.h>
#include <string>
#include <iostream>
using namespace Moose::Math;
using namespace std;
/////////////////////////////////////////////////////////////////
TEST( CTransform )
{
  CTransform transform;
  CMatrix4x4<float> mId;
  CVector3<float> vTransl(0,0,0);
  CVector3<float> vScale(1,1,1);
  CQuaternion q;
  // Check initial state ( scale = 1, matrix = id, no rotation);
  mId.IdentityMatrix();
  q.Identity();
  
  CHECK_ARRAY_CLOSE( mId.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetTranslation_Matrix )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vScale(1,1,1);
  CVector3<float> vTransl(1,4,-5);
  CQuaternion q;
  q.Identity();  
  // Translate by matrix.
  Moose::Math::TranslationMatrix( vTransl, mMatrix );


  transform.SetTranslation( mMatrix );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetTranslation_Vector3 )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vTransl(1,4,-5);
  CVector3<float> vScale(1,1,1);
  CQuaternion q;
  q.Identity();  
  // Translate by vector
  Moose::Math::TranslationMatrix( vTransl, mMatrix );


  transform.SetTranslation( vTransl );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
TEST( CTransform_SetTranslation_3_Floats )
{
  CTransform transform;
  CMatrix4x4<float> mMatrix;
  CVector3<float> vTransl(1,4,-5);
  CVector3<float> vScale(1,1,1);
  CQuaternion q;
  q.Identity();  
  // Translate by values
  Moose::Math::TranslationMatrix( vTransl, mMatrix );

  
  transform.SetTranslation( vTransl[0], vTransl[1], vTransl[2] );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
  CVector3<float> vScale(1,1,1);
  CQuaternion q;
  // Rotate by 3 rotations 
  Moose::Math::RotationMatrix( vRot, mMatrix );
  RotationMatrixToQuaternion( mMatrix, q );

  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( mMatrix );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
CVector3<float> vScale(1,1,1);
  CQuaternion q;
  // Rotate by 3 rotations 
  Moose::Math::RotationMatrix( vRot, mMatrix );
  RotationMatrixToQuaternion( mMatrix, q );

  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( q );

  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
CVector3<float> vScale(1,1,1);
  CQuaternion q;
  // Rotate by 3 rotations 
  Moose::Math::RotationMatrix( vRot, mMatrix );
  RotationMatrixToQuaternion( mMatrix, q );
  
  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( vRot[0], vRot[1], vRot[2] );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
CVector3<float> vScale(1,1,1);
  CQuaternion q;
  // Rotate by 3 rotations 
  Moose::Math::RotationMatrix( vRot, mMatrix );
  RotationMatrixToQuaternion( mMatrix, q );
  
  transform.SetTranslation(vTransl);
  transform.SetScaling(1.0f);
  transform.SetRotation( vRot );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
  CVector3<float> vScale(3,3,3);
  transform.SetScaling(vScale);
  mMatrix.IdentityMatrix();
  mMatrix(0,0) = mMatrix(1,1) = mMatrix(2,2) = 3.0f;
  q.Identity();
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);

  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
CVector3<float> vScale(1,1,1);
  CQuaternion q;
  q.Identity();

  TranslationMatrix( vTranslReal, mMatrix );
  transform.Move( CVector3<float>(1,0,0) );
  transform.Move( CVector3<float>(0,2,0) );
  transform.Move( CVector3<float>(0,0,3) );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);

  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
CVector3<float> vScale(1,1,1);
  CQuaternion q;
  q.Identity();

  TranslationMatrix( vTranslReal, mMatrix );
  transform.Move( 1,0,0 );
  transform.Move( 0,2,0 );
  transform.Move( 0,0,3 );
  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
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
CVector3<float> vScale(1,1,1);
  CQuaternion q, qFinal;
  q.Identity();

  mMatrix.IdentityMatrix();

  
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( q.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);

  RotationMatrix( 4,0,0, mMatrix );
  q.CreateFromAxisAngleRad( 1,0,0, 2 );
  qFinal.CreateFromAxisAngleRad( 1,0,0, 4 );
  transform.Rotate( q );
  transform.Rotate( q );
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), transform.GetMatrix().GetArray(), 16,0.001f);
  CHECK_ARRAY_CLOSE( transform.GetScaling(), vScale.GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( vTransl.GetArray(), transform.GetTranslation().GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE( qFinal.GetArray(), transform.GetRotation().GetArray(), 4, 0.001f);
} 
/////////////////////////////////////////////////////////////////
#define V(X,Y,Z) CVector3<float>(X,Y,Z)
TEST( CTransform_Multiply )
{


  CTransform transform1;
  CTransform transform2;
  CTransform transformResult;
  CVector3<float> vScale(1,1,1);
  CVector3<float> vTransl[] = { V(0,2,0),  V(0,0,3),
				V(0,0,0),  V(0,0,0),
				};
  CVector3<float> vRot[]    = { V(0,45,0), V(33,0,0),
				V(0,0,0),  V(0,0,0), };
  float fScale[]            = { 3.0f,      2.0f,
				1.0f,	   1.0f,
                                5.0f,      6.0f,
			      };
  
  CMatrix4x4<float> mReal;
  /////////////////////////////////////////////////////////////////
  // Full swing
  transform1.SetScaling(fScale[0]);
  transform1.SetRotation(vRot[0]);
  transform1.SetTranslation(vTransl[0]);

  transform2.SetScaling(fScale[1]);
  transform2.SetRotation(vRot[1]);
  transform2.SetTranslation(vTransl[1]);
  
  Moose::Math::Multiply( transform1, transform2, transformResult );
  mReal = transform1.GetMatrix()*transform2.GetMatrix();
  CHECK_ARRAY_CLOSE( mReal.GetArray(), transformResult.GetMatrix().GetArray(), 16, 0.001f);

  transform1.SetScaling(fScale[1]);
  transform1.SetRotation(vRot[1]);
  transform1.SetTranslation(vTransl[1]);

  transform2.SetScaling(fScale[0]);
  transform2.SetRotation(vRot[0]);
  transform2.SetTranslation(vTransl[0]);
  
  Moose::Math::Multiply( transform1, transform2, transformResult );
  mReal = transform1.GetMatrix()*transform2.GetMatrix();
  CHECK_ARRAY_CLOSE( mReal.GetArray(), transformResult.GetMatrix().GetArray(), 16, 0.001f);

  /////////////////////////////////////////////////////////////////
  // simple scaling  test
  const float SCALE = fScale[4]*fScale[5];

  float fResult[16] = { SCALE, 0,0,0,
			0, SCALE, 0,0,
			0,0,SCALE, 0,
			0,0,0,1 };
  transform1.SetScaling(fScale[4]);
  transform1.SetRotation(vRot[2]);
  transform1.SetTranslation(vTransl[2]);

  transform2.SetScaling(fScale[5]);
  transform2.SetRotation(vRot[3]);
  transform2.SetTranslation(vTransl[3]);
  
  Moose::Math::Multiply( transform1, transform2, transformResult );
  mReal = transform1.GetMatrix()*transform2.GetMatrix();
  CHECK_ARRAY_CLOSE( fResult, transformResult.GetMatrix().GetArray(), 16, 0.001f);

  /////////////////////////////////////////////////////////////////
  // No transl, rot, and scale 1 should yield Identity matrix.
  float fIdentity[] = { 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
			
  transform1.SetScaling(fScale[2]);
  transform1.SetRotation(vRot[2]);
  transform1.SetTranslation(vTransl[2]);

  transform2.SetScaling(fScale[3]);
  transform2.SetRotation(vRot[3]);
  transform2.SetTranslation(vTransl[3]);
  
  Moose::Math::Multiply( transform1, transform2, transformResult );
  mReal = transform1.GetMatrix()*transform2.GetMatrix();
  CHECK_ARRAY_CLOSE( fIdentity, transformResult.GetMatrix().GetArray(), 16, 0.001f);
  
} 
/////////////////////////////////////////////////////////////////
