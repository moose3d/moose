#include "../src/PhoenixVector2.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <sstream>
#include <iomanip>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
namespace Vector2Test
{
float g_aResultOne[2] = { 1,2 };
float g_aResultTwo[2] = { 9.0f,-34.0f };
}
using namespace Vector2Test;
/////////////////////////////////////////////////////////////////
TEST(CVector2)
{
  /// Test does this even work.
  CVector2<float> vector;

  CVector2<float> vectorInit(g_aResultOne[0], g_aResultOne[1]);
  /// Test with first data set
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit.GetArray(), 2, 0.001f);
  CHECK_CLOSE( g_aResultOne[0], vectorInit[0], 0.001f);
  CHECK_CLOSE( g_aResultOne[1], vectorInit[1], 0.001f);
  /// Test with another data set
  CVector2<float> vectorInit2(g_aResultTwo[0], g_aResultTwo[1]);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit2.GetArray(), 2, 0.001f);
  CHECK_CLOSE( g_aResultTwo[0], vectorInit2[0], 0.001f);
  CHECK_CLOSE( g_aResultTwo[1], vectorInit2[1], 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_Copy)
{
  CVector2<float> vectorInit1(9.0f,-34.0f);
  CVector2<float> vectorInit2(vectorInit1);
  CHECK_ARRAY_CLOSE(  vectorInit1.GetArray(),  vectorInit2.GetArray(), 2, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_Set)
{
  CVector2<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_ARRAY_CLOSE( g_aResultTwo,  vectorInit1.GetArray(), 2, 0.001f);
}
TEST(CVector2_UseExternalData)
{
  CVector2<float> vectorInit1;
  vectorInit1.UseExternalData(g_aResultTwo);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit1.GetArray(), 2, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_OperatorParenthesis)
{
  CVector2<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1[0],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1[1],  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_OperatorAt)
{
  CVector2<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1.At(0),  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1.At(1),  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_OperatorBrackets)
{
  CVector2<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1[0],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1[1],  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_OperatorAssignment)
{
  CVector2<float> vectorInit1;
  CVector2<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultTwo);
  
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit1.GetArray(), 2, 0.001f);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit2.GetArray(), 2, 0.001f);

  vectorInit2 = vectorInit1;
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit1.GetArray(), 2, 0.001f);
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit2.GetArray(), 2, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_OperatorEqualityComparison)
{
  CVector2<float> vectorInit1;
  CVector2<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultOne);
  
  CHECK( vectorInit2 == vectorInit1 );
  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.0f;

  CHECK( vectorInit2 == vectorInit1 );
  
  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.00000000000000000001f;


  CHECK( vectorInit2 == vectorInit1 );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.00001f;

  CHECK( ! ( vectorInit2 == vectorInit1)  );

}
/////////////////////////////////////////////////////////////////
TEST(CVector2_OperatorInequalityComparison)
{
  CVector2<float> vectorInit1;
  CVector2<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultTwo);
  
  CHECK( vectorInit2 != vectorInit1 );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.0f;

  // negative test, they are the same
  CHECK( ! (vectorInit2 != vectorInit1) );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;

  vectorInit2[0] =  1.0f;
  vectorInit2[1] =  1.0f;
  
  CHECK( vectorInit2 != vectorInit1 );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;

  vectorInit2[0] =  1.0f;
  vectorInit2[1] = -1.0f;
  
  CHECK( vectorInit2 != vectorInit1 );

  vectorInit1[0] =  0.0f;
  vectorInit1[1] =  1.0f;

  vectorInit2[0] =  1.0f;
  vectorInit2[1] =  1.0f;

  CHECK( vectorInit2 != vectorInit1 );

}
/////////////////////////////////////////////////////////////////
TEST(CVector2_Dot)
{
  CVector2<float> vectorInit1;
  CVector2<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2[0] = 2.0f;
  vectorInit2[1] = 1.0f;
  
  CHECK_CLOSE( 4.0f, vectorInit1.Dot(vectorInit2), 0.001f );
  
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_Length)
{
  CVector2<float> vectorInit1;
  vectorInit1.Set(g_aResultOne);
  CHECK_CLOSE( 2.23606f, vectorInit1.Length(), 0.00001f );
  
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_LengthSqr)
{
  CVector2<float> vectorInit1;
  vectorInit1.Set(g_aResultOne);
  CHECK_CLOSE( 5.0f, vectorInit1.LengthSqr(), 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_Normalize)
{
  CVector2<float> vectorX(5,0);
  CVector2<float> vectorY(345.0,-2340);
  CVector2<float> vectorZ(40,1);
  vectorX.Normalize();
  vectorY.Normalize();
  vectorZ.Normalize();
  CHECK_CLOSE( 1.0f, vectorX.Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorY.Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorZ.Length(),  0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_GetNormalized)
{
  CVector2<float> vectorX(5,0);
  CVector2<float> vectorY(345.0,-2340);
  CVector2<float> vectorZ(40,1);
  CHECK_CLOSE( 1.0f, vectorX.GetNormalized().Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorY.GetNormalized().Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorZ.GetNormalized().Length(),  0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_ScalingOperator)
{
  CVector2<float> vector(1.0f,2.0f);
  float aScaled[2] = { 3.0f,6.0f};
  vector = vector * 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_DivisionOperator)
{
  CVector2<float> vector(1.0f,2.0f);
  float aScaled[2] = {1.0f/3.0f,2.0f/3.0f};
  vector = vector / 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_MinusPrefixOperator)
{
  CVector2<float> vector(1.0f,2.0f);
  CVector2<float> vectorCorrect(-1.0f,-2.0f);
  vector = -vector;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vector.GetArray(),  2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_MinusOperator)
{
  CVector2<float> vectorOne(1.0f,2.0f);
  CVector2<float> vectorTwo(3.0f,-3.0f);
  CVector2<float> vectorCorrect(-2.0f,5.0f);
  CVector2<float> vectorResult;
  vectorResult = vectorOne - vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorResult.GetArray(),  2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_PlusOperator)
{
  CVector2<float> vectorOne(1.0f,2.0f);
  CVector2<float> vectorTwo(3.0f,-3.0f);
  CVector2<float> vectorCorrect(4.0f,-1.0f);
  CVector2<float> vectorResult;
  vectorResult = vectorOne + vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorResult.GetArray(), 2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_PlusAssignmentOperator)
{
  CVector2<float> vectorOne(1.0f,2.0f);
  CVector2<float> vectorTwo(3.0f,-3.0f);
  CVector2<float> vectorCorrect(4.0f,-1.0f);
  vectorOne += vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorOne.GetArray(), 2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_MinusAssignmentOperator)
{
  CVector2<float> vectorOne(1.0f,2.0f);
  CVector2<float> vectorTwo(3.0f,-3.0f);
  CVector2<float> vectorCorrect(-2.0f,5.0f);
  vectorOne -= vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorOne.GetArray(), 2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_DivisionAssignmentOperator)
{
  CVector2<float> vector(1.0f,2.0f);
  float aScaled[2] = {1.0f/3.0f,2.0f/3.0f};
  vector /= 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_PrefixScalingOperator)
{
  CVector2<float> vector(1.0f,2.0f);
  float aScaled[2] = {3.0f,6.0f};
  vector = 3.0f * vector;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(), 2, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector2_StreamOutput)
{
  const char szResult[] = { "1.0,2.0" };
  CVector2<float> vector(1.0f,2.0f);
  std::ostringstream stream;
  stream << std::setiosflags(std::ios::fixed) << std::setprecision(1) << vector << std::endl;
  CHECK_ARRAY_EQUAL( szResult, stream.str().c_str(), 7 );
}
/////////////////////////////////////////////////////////////////

