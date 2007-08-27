#include "../src/PhoenixVector4.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <iomanip>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
namespace Vector4Test
{
  float g_aResultOne[4] = { 1,2,3,4 };
  float g_aResultTwo[4] = { 9.0f,-34.0f,3000.0f, -4.20f };
}
using namespace Vector4Test;
/////////////////////////////////////////////////////////////////
TEST(CVector4)
{
  /// Test does this even work.
  CVector4<float> vector;

  CVector4<float> vectorInit(g_aResultOne[0], g_aResultOne[1], g_aResultOne[2], g_aResultOne[3]);
  /// Test with first data set
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit.GetArray(), 4, 0.001f);
  CHECK_CLOSE( g_aResultOne[0], vectorInit[0], 0.001f);
  CHECK_CLOSE( g_aResultOne[1], vectorInit[1], 0.001f);
  CHECK_CLOSE( g_aResultOne[2], vectorInit[2], 0.001f);
  CHECK_CLOSE( g_aResultOne[3], vectorInit[3], 0.001f);
  /// Test with another data set
  CVector4<float> vectorInit2(g_aResultTwo[0], g_aResultTwo[1], g_aResultTwo[2], g_aResultTwo[3]);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit2.GetArray(), 4, 0.001f);
  CHECK_CLOSE( g_aResultTwo[0], vectorInit2[0], 0.001f);
  CHECK_CLOSE( g_aResultTwo[1], vectorInit2[1], 0.001f);
  CHECK_CLOSE( g_aResultTwo[2], vectorInit2[2], 0.001f);
  CHECK_CLOSE( g_aResultTwo[3], vectorInit2[3], 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_Copy)
{
  CVector4<float> vectorInit1(9.0f,-34.0f,3000.0f, -4.20);
  CVector4<float> vectorInit2(vectorInit1);
  CHECK_ARRAY_CLOSE(  vectorInit1.GetArray(),  vectorInit2.GetArray(), 4, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_Set)
{
  CVector4<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_ARRAY_CLOSE( g_aResultTwo,  vectorInit1.GetArray(), 4, 0.001f);
}
TEST(CVector4_UseExternalData)
{
  CVector4<float> vectorInit1;
  vectorInit1.UseExternalData(g_aResultTwo);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit1.GetArray(), 4, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_OperatorParenthesis)
{
  CVector4<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1[0],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1[1],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[2],  vectorInit1[2],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[3],  vectorInit1[3],  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_OperatorAt)
{
  CVector4<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1.At(0),  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1.At(1),  0.001f);
  CHECK_CLOSE(  g_aResultTwo[2],  vectorInit1.At(2),  0.001f);
  CHECK_CLOSE(  g_aResultTwo[3],  vectorInit1.At(3),  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_OperatorBrackets)
{
  CVector4<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1[0],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1[1],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[2],  vectorInit1[2],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[3],  vectorInit1[3],  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_OperatorAssignment)
{
  CVector4<float> vectorInit1;
  CVector4<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultTwo);
  
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit1.GetArray(), 4, 0.001f);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit2.GetArray(), 4, 0.001f);

  vectorInit2 = vectorInit1;
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit1.GetArray(), 4, 0.001f);
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit2.GetArray(), 4, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_OperatorEqualityComparison)
{
  CVector4<float> vectorInit1;
  CVector4<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultOne);
  
  CHECK( vectorInit2 == vectorInit1 );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_OperatorInequalityComparison)
{
  CVector4<float> vectorInit1;
  CVector4<float> vectorInit2;
  
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultTwo);
  
  CHECK( vectorInit2 != vectorInit1 );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_Dot)
{
  CVector4<float> vectorInit1;
  CVector4<float> vectorInit2;
  
  vectorInit1.Set(g_aResultOne);
  vectorInit2[0] = 4.0f;
  vectorInit2[1] = 3.0f;
  vectorInit2[2] = 2.0f;
  vectorInit2[3] = 1.0f;
  CHECK_CLOSE( 20.0f, vectorInit1.Dot(vectorInit2), 0.001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_Length)
{
  CVector4<float> vectorInit1;
  vectorInit1.Set(g_aResultOne);
  CHECK_CLOSE( 5.47722f, vectorInit1.Length(), 0.00001f );
  
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_LengthSqr)
{
  CVector4<float> vectorInit1;
  vectorInit1.Set(g_aResultOne);
  CHECK_CLOSE( 30.0, vectorInit1.LengthSqr(), 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_Normalize)
{
  CVector4<float> vectorX(5,0,0,0);
  CVector4<float> vectorY(345.0,-2340,4,25.5f);
  CVector4<float> vectorZ(40,1,0,34000.0f);
  vectorX.Normalize();
  vectorY.Normalize();
  vectorZ.Normalize();
  CHECK_CLOSE( 1.0f, vectorX.Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorY.Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorZ.Length(),  0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_GetNormalized)
{
  CVector4<float> vectorX(5,0,0,0);
  CVector4<float> vectorY(345.0,-2340,4,25.5f);
  CVector4<float> vectorZ(40,1,0,34000.0f);
  CHECK_CLOSE( 1.0f, vectorX.GetNormalized().Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorY.GetNormalized().Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorZ.GetNormalized().Length(),  0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_ScalingOperator)
{
  CVector4<float> vector(1.0f,2.0f,3.0f,4.0f);
  float aScaled[4] = { 3.0f,6.0f,9.0f,12.0f};
  vector = vector * 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_DivisionOperator)
{
  CVector4<float> vector(1.0f,2.0f,3.0f,4.0f);
  float aScaled[4] = {1.0f/3.0f,2.0f/3.0f,3.0f/3.0f,4.0/3.0f};
  vector = vector / 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_MinusPrefixOperator)
{
  CVector4<float> vector(1.0f,2.0f,3.0f,4.0f);
  CVector4<float> vectorCorrect(-1.0f,-2.0f,-3.0f,-4.0f);
  vector = -vector;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vector.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_MinusOperator)
{
  CVector4<float> vectorOne(1.0f,2.0f,3.0f,4.0f);
  CVector4<float> vectorTwo(3.0f,-3.0f,3.0f,3.0f);
  CVector4<float> vectorCorrect(-2.0f,5.0f,0.0f,1.0f);
  CVector4<float> vectorResult;
  vectorResult = vectorOne - vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorResult.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_PlusOperator)
{
  CVector4<float> vectorOne(1.0f,2.0f,3.0f,4.0f);
  CVector4<float> vectorTwo(3.0f,-3.0f,3.0f,3.0f);
  CVector4<float> vectorCorrect(4.0f,-1.0f,6.0f, 7.0f);
  CVector4<float> vectorResult;
  vectorResult = vectorOne + vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorResult.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_PlusAssignmentOperator)
{
  CVector4<float> vectorOne(1.0f,2.0f,3.0f,4.0f);
  CVector4<float> vectorTwo(3.0f,-3.0f,3.0f,3.0f);
  CVector4<float> vectorCorrect(4.0f,-1.0f,6.0f, 7.0f);
  vectorOne += vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorOne.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_MinusAssignmentOperator)
{
  CVector4<float> vectorOne(1.0f,2.0f,3.0f,4.0f);
  CVector4<float> vectorTwo(3.0f,-3.0f,3.0f,3.0f);
  CVector4<float> vectorCorrect(-2.0f,5.0f,0.0f,1.0f);
  vectorOne -= vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorOne.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_DivisionAssignmentOperator)
{
  CVector4<float> vector(1.0f,2.0f,3.0f,4.0f);
  float aScaled[4] = {1.0f/3.0f,2.0f/3.0f,3.0f/3.0f,4.0f/3.0f};
  vector /= 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_PrefixScalingOperator)
{
  CVector4<float> vector(1.0f,2.0f,3.0f,4.0f);
  float aScaled[4] = {3.0f,6.0f,9.0f,12.0f};
  vector = 3.0f * vector;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  4, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector4_StreamOutput)
{
  const char szResult[] = { "1.0,2.0,3.0,4.0" };
  CVector4<float> vector(1.0f,2.0f,3.0f,4.0f);
  std::ostringstream stream;
  stream << std::setiosflags(std::ios::fixed) << std::setprecision(1) << vector << std::endl;
  CHECK_ARRAY_EQUAL( szResult, stream.str().c_str(), 15 );
}
/////////////////////////////////////////////////////////////////

