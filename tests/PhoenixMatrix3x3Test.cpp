#include "../src/PhoenixMatrix3x3.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3)
{
  CMatrix3x3<float> matrix;
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Parametrized)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  CMatrix3x3<float> matrix2(0,0,0,
			    4,5,6,
			    0,0,0);
  float aValues[9] = { 1,2,3,4,5,6,7,8,9 };
  float aValues2[9] = { 0,0,0,4,5,6,0,0,0 };
  CHECK_ARRAY_CLOSE( aValues2, matrix2.GetArray(), 9, 0.00001f);
  
}
/////////////////////////////////////////////////////////////////

