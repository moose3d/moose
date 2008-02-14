#include "../src/PhoenixVector4.h"
#include "../src/PhoenixVector3.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <iomanip>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
namespace Vector3Test
{
float g_aResultOne[3] = { 1,2,3 };
float g_aResultTwo[3] = { 9.0f,-34.0f,3000.0f };
}
using namespace Vector3Test;
/////////////////////////////////////////////////////////////////
TEST(CVector3)
{
  /// Test does this even work.
  CVector3<float> vector;

  CVector3<float> vectorInit(g_aResultOne[0], g_aResultOne[1], g_aResultOne[2]);
  /// Test with first data set
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit.GetArray(), 3, 0.001f);
  CHECK_CLOSE( g_aResultOne[0], vectorInit[0], 0.001f);
  CHECK_CLOSE( g_aResultOne[1], vectorInit[1], 0.001f);
  CHECK_CLOSE( g_aResultOne[2], vectorInit[2], 0.001f);
  /// Test with another data set
  CVector3<float> vectorInit2(g_aResultTwo[0], g_aResultTwo[1], g_aResultTwo[2]);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit2.GetArray(), 3, 0.001f);
  CHECK_CLOSE( g_aResultTwo[0], vectorInit2[0], 0.001f);
  CHECK_CLOSE( g_aResultTwo[1], vectorInit2[1], 0.001f);
  CHECK_CLOSE( g_aResultTwo[2], vectorInit2[2], 0.001f);
 
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_Copy)
{
  CVector3<float> vectorInit1(9.0f,-34.0f,3000.0f);
  CVector3<float> vectorInit2(vectorInit1);
  CHECK_ARRAY_CLOSE(  vectorInit1.GetArray(),  vectorInit2.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_Set)
{
  CVector3<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_ARRAY_CLOSE( g_aResultTwo,  vectorInit1.GetArray(), 3, 0.001f);
}
TEST(CVector3_UseExternalData)
{
  CVector3<float> vectorInit1;
  vectorInit1.UseExternalData(g_aResultTwo);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit1.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_OperatorParenthesis)
{
  CVector3<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1[0],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1[1],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[2],  vectorInit1[2],  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_OperatorAt)
{
  CVector3<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1.At(0),  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1.At(1),  0.001f);
  CHECK_CLOSE(  g_aResultTwo[2],  vectorInit1.At(2),  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_OperatorBrackets)
{
  CVector3<float> vectorInit1;
  vectorInit1.Set(g_aResultTwo);
  CHECK_CLOSE(  g_aResultTwo[0],  vectorInit1[0],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[1],  vectorInit1[1],  0.001f);
  CHECK_CLOSE(  g_aResultTwo[2],  vectorInit1[2],  0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_OperatorAssignment)
{
  CVector3<float> vectorInit1;
  CVector3<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultTwo);
  
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit1.GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE(  g_aResultTwo,  vectorInit2.GetArray(), 3, 0.001f);

  vectorInit2 = vectorInit1;
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit1.GetArray(), 3, 0.001f);
  CHECK_ARRAY_CLOSE(  g_aResultOne,  vectorInit2.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_OperatorEqualityComparison)
{
  CVector3<float> vectorInit1;
  CVector3<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultOne);
  
  CHECK( vectorInit2 == vectorInit1 );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;
  vectorInit1[2] =  0.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.0f;
  vectorInit2[2] =  0.0f;

  CHECK( vectorInit2 == vectorInit1 );
  
  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;
  vectorInit1[2] =  0.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.00000000000000000001f;
  vectorInit2[2] =  0.0f;

  CHECK( vectorInit2 == vectorInit1 );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;
  vectorInit1[2] =  0.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.00001f;
  vectorInit2[2] =  0.0f;

  CHECK( ! ( vectorInit2 == vectorInit1)  );

}
/////////////////////////////////////////////////////////////////
TEST(CVector3_OperatorInequalityComparison)
{
  CVector3<float> vectorInit1;
  CVector3<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2.Set(g_aResultTwo);
  
  CHECK( vectorInit2 != vectorInit1 );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;
  vectorInit1[2] =  0.0f;

  vectorInit2[0] = -1.0f;
  vectorInit2[1] =  1.0f;
  vectorInit2[2] =  0.0f;
  // negative test, they are the same
  CHECK( ! (vectorInit2 != vectorInit1) );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;
  vectorInit1[2] =  0.0f;

  vectorInit2[0] =  1.0f;
  vectorInit2[1] =  1.0f;
  vectorInit2[2] =  0.0f;
  
  CHECK( vectorInit2 != vectorInit1 );

  vectorInit1[0] = -1.0f;
  vectorInit1[1] =  1.0f;
  vectorInit1[2] =  0.0f;

  vectorInit2[0] =  1.0f;
  vectorInit2[1] = -1.0f;
  vectorInit2[2] =  0.0f;
  
  CHECK( vectorInit2 != vectorInit1 );

  vectorInit1[0] =  1.0f;
  vectorInit1[1] =  1.0f;
  vectorInit1[2] =  0.0f;

  vectorInit2[0] =  1.0f;
  vectorInit2[1] =  1.0f;
  vectorInit2[2] =  1.0f;

  CHECK( vectorInit2 != vectorInit1 );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_Dot)
{
  CVector3<float> vectorInit1;
  CVector3<float> vectorInit2;
  vectorInit1.Set(g_aResultOne);
  vectorInit2[0] = 3.0f;
  vectorInit2[1] = 2.0f;
  vectorInit2[2] = 1.0f;
  
  CHECK_CLOSE( 10.0f, vectorInit1.Dot(vectorInit2), 0.001f );
  
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_Length)
{
  CVector3<float> vectorInit1;
  vectorInit1.Set(g_aResultOne);
  CHECK_CLOSE( 3.741657f, vectorInit1.Length(), 0.00001f );
  
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_LengthSqr)
{
  CVector3<float> vectorInit1;
  vectorInit1.Set(g_aResultOne);
  CHECK_CLOSE( 14.0, vectorInit1.LengthSqr(), 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_Cross)
{
  CVector3<float> vectorX(1,0,0);
  CVector3<float> vectorY(0,0,1);
  CVector3<float> vectorZ(0,1,0);
  
  CHECK_ARRAY_CLOSE(vectorY.GetArray(), vectorX.Cross(vectorZ).GetArray(), 3, 0.00001f );
  CHECK_ARRAY_CLOSE(vectorZ.GetArray(), vectorY.Cross(vectorX).GetArray(), 3, 0.00001f );
  CHECK_ARRAY_CLOSE((-vectorY).GetArray(), vectorZ.Cross(vectorX).GetArray(), 3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_Normalize)
{
  CVector3<float> vectorX(5,0,0);
  CVector3<float> vectorY(345.0,-2340,4);
  CVector3<float> vectorZ(40,1,0);
  vectorX.Normalize();
  vectorY.Normalize();
  vectorZ.Normalize();
  CHECK_CLOSE( 1.0f, vectorX.Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorY.Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorZ.Length(),  0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_GetNormalized)
{
  CVector3<float> vectorX(5,0,0);
  CVector3<float> vectorY(345.0,-2340,4);
  CVector3<float> vectorZ(40,1,0);
  CHECK_CLOSE( 1.0f, vectorX.GetNormalized().Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorY.GetNormalized().Length(),  0.00001f );
  CHECK_CLOSE( 1.0f, vectorZ.GetNormalized().Length(),  0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_ScalingOperator)
{
  CVector3<float> vector(1.0f,2.0f,3.0f);
  float aScaled[3] = { 3.0f,6.0f,9.0f};
  vector = vector * 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_DivisionOperator)
{
  CVector3<float> vector(1.0f,2.0f,3.0f);
  float aScaled[3] = {1.0f/3.0f,2.0f/3.0f,3.0f/3.0f};
  vector = vector / 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_MinusPrefixOperator)
{
  CVector3<float> vector(1.0f,2.0f,3.0f);
  CVector3<float> vectorCorrect(-1.0f,-2.0f,-3.0f);
  vector = -vector;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vector.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_MinusOperator)
{
  CVector3<float> vectorOne(1.0f,2.0f,3.0f);
  CVector3<float> vectorTwo(3.0f,-3.0f,3.0f);
  CVector3<float> vectorCorrect(-2.0f,5.0f,0.0f);
  CVector3<float> vectorResult;
  vectorResult = vectorOne - vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorResult.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_PlusOperator)
{
  CVector3<float> vectorOne(1.0f,2.0f,3.0f);
  CVector3<float> vectorTwo(3.0f,-3.0f,3.0f);
  CVector3<float> vectorCorrect(4.0f,-1.0f,6.0f);
  CVector3<float> vectorResult;
  vectorResult = vectorOne + vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorResult.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_PlusAssignmentOperator)
{
  CVector3<float> vectorOne(1.0f,2.0f,3.0f);
  CVector3<float> vectorTwo(3.0f,-3.0f,3.0f);
  CVector3<float> vectorCorrect(4.0f,-1.0f,6.0f);
  vectorOne += vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorOne.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_MinusAssignmentOperator)
{
  CVector3<float> vectorOne(1.0f,2.0f,3.0f);
  CVector3<float> vectorTwo(3.0f,-3.0f,3.0f);
  CVector3<float> vectorCorrect(-2.0f,5.0f,0.0f);
  vectorOne -= vectorTwo;
  CHECK_ARRAY_CLOSE( vectorCorrect.GetArray(), vectorOne.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_DivisionAssignmentOperator)
{
  CVector3<float> vector(1.0f,2.0f,3.0f);
  float aScaled[3] = {1.0f/3.0f,2.0f/3.0f,3.0f/3.0f};
  vector /= 3.0f;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_PrefixScalingOperator)
{
  CVector3<float> vector(1.0f,2.0f,3.0f);
  float aScaled[3] = {3.0f,6.0f,9.0f};
  vector = 3.0f * vector;
  CHECK_ARRAY_CLOSE( aScaled, vector.GetArray(),  3, 0.00001f );
}
/////////////////////////////////////////////////////////////////
TEST(CVector3_StreamOutput)
{
  const char szResult[] = { "1.0,2.0,3.0" };
  CVector3<float> vector(1.0f,2.0f,3.0f);
  std::ostringstream stream;
  stream << std::setiosflags(std::ios::fixed) << std::setprecision(1) << vector << std::endl;
  CHECK_ARRAY_EQUAL( szResult, stream.str().c_str(), 11 );
}
/////////////////////////////////////////////////////////////////

