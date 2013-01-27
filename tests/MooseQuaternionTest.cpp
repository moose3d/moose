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

#include <MooseQuaternion.h>
#include <UnitTest++/UnitTest++.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Moose::Math;
/////////////////////////////////////////////////////////////////
#define ASET(A,V1,V2,V3,V4) { 			\
    A[0] = V1;					\
    A[1] = V2;					\
    A[2] = V3;					\
    A[3] = V4;					\
}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion)
{
  CQuaternion q;
}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_Parametrized)
{
  CQuaternion q(1,2,3,4);
  float aValues[4] = { 1,2,3,4 };
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_Normalize)
{
  CQuaternion q(1,2,3,4);
  q.Normalize();
  CHECK_CLOSE( 1.0f, q.Magnitude(), 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_CreateFromAxisAngle)
{
  CQuaternion q;
  float aValues[4];

  ASET(aValues, 0.707107, 0, 0, 0.707107 );
  q.CreateFromAxisAngle(1,0,0,90);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);
  
  q.CreateFromAxisAngle(0,1,0,90);
  ASET(aValues, 0, 0.707107, 0, 0.707107 );
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

  q.CreateFromAxisAngle(0,0,1,90);
  ASET(aValues, 0, 0, 0.707107, 0.707107 );
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

  ASET(aValues, 0.69660183f, 0, 0, -0.71745794f );
  q.CreateFromAxisAngle( 1,0,0, 271.69f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

  ASET(aValues, 0, 0.69660183f, 0, -0.71745794f );
  q.CreateFromAxisAngle( 0,1,0, 271.69f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);
  
  ASET(aValues, 0, 0, 0.69660183f, -0.71745794f );
  q.CreateFromAxisAngle( 0,0,1, 271.69f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_CreateFromAxisAngleRad)
{
  CQuaternion q;
  float aValues[4];

  ASET(aValues, 0.707107f, 0, 0, 0.707107f );
  q.CreateFromAxisAngleRad(1,0,0, 1.5707963f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

  ASET(aValues, 0, 0.707107f, 0, 0.707107f );
  q.CreateFromAxisAngleRad(0,1,0, 1.5707963f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

  ASET(aValues, 0, 0, 0.707107f, 0.707107f );
  q.CreateFromAxisAngleRad(0,0,1, 1.5707963f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

  ASET(aValues, 0.69660183f, 0, 0, -0.71745794f );
  q.CreateFromAxisAngleRad( 1,0,0, 4.741885f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);

  ASET(aValues, 0, 0.69660183f, 0, -0.71745794f );
  q.CreateFromAxisAngleRad( 0,1,0, 4.741885f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);
  
  ASET(aValues, 0, 0, 0.69660183f, -0.71745794f );
  q.CreateFromAxisAngleRad( 0,0,1, 4.741885f);
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4, 0.00001f);  
  
}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_OperatorMultiply)
{
  CQuaternion q1(1,2,3,4);
  CQuaternion q2(5,6,7,8);
  float aValues[4] = { 24, 48, 48, -6};
  float aValuesReverse[4] = {32,32, 56, -6 };
  CQuaternion qR = q1 * q2;
  CHECK_ARRAY_CLOSE( aValues, qR.GetArray(), 4, 0.00001f);  
  CQuaternion qR2 = q2 * q1;
  CHECK_ARRAY_CLOSE( aValuesReverse, qR2.GetArray(), 4, 0.00001f);  
}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_OperatorPlus)
{
  CQuaternion q1(1,2,3,4);
  CQuaternion q2(5,6,7,8);
  float aValues[4] = { 6, 8, 10, 12};

  CQuaternion qR = q1 + q2;
  CHECK_ARRAY_CLOSE( aValues, qR.GetArray(), 4, 0.00001f);  

}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_Conjugate)
{

  CQuaternion q(1,2,3,4);
  float aValues[4] = { -1,-2,-3,4 };
  CHECK_ARRAY_CLOSE( aValues, q.GetConjugate().GetArray(), 4,0.00001f);  

}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_Magnitude)
{

  CQuaternion q(1,2,3,4);
  CHECK_CLOSE(5.4772256f, q.Length(), 0.00001f);  
  
}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_Inverse)
{

  CQuaternion q(1,2,3,4);
  float aValues[4] = { -0.18257419, -0.36514837, -0.54772256, 0.73029674 };
  CHECK_ARRAY_CLOSE( aValues, q.GetInverse().GetArray(), 4,0.00001f);  

}
/////////////////////////////////////////////////////////////////
TEST(CQuaternion_Identity)
{

  CQuaternion q(1,2,3,4);
  q.Identity();
  float aValues[4] = { 0,0,0,1 };
  CHECK_ARRAY_CLOSE( aValues, q.GetArray(), 4,0.00001f);  

}
/////////////////////////////////////////////////////////////////

