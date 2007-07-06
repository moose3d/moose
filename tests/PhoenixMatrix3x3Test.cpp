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
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);
  CHECK_ARRAY_CLOSE( aValues2, matrix2.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Multiply)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  CMatrix3x3<float> matrix2(2,4,6,
			    8,10,12,
			    14,16,18);

  CMatrix3x3<float> matrixOne(  3.4f, 123.0f,   0.0f,
				12.0f,  43.0f,  55.0f,
				34.342f,   1.0f,   5.0f);

  CMatrix3x3<float> matrixTwo(  90.0f, 234.0f,  12.0f,
				9.0f,   0.35f,   2.0f,
				46.760f,   3.230f,  34.0f);

  CMatrix3x3<float> matrixResult(1413.0f,  838.65f,  286.8f,
				 4038.8f, 3000.7f, 2100.0f,
				 3333.607f, 8052.598f,  584.108f);
  
  float aValues[9] = { 60.000,  72.000,  84.000,
		       132.000, 162.000, 192.000,
		       204.000, 252.000, 300.000 };
  CMatrix3x3<float> mResult = matrix * matrix2;
  
  CHECK_ARRAY_CLOSE( aValues, mResult.GetArray(), 9, 0.00001f);
  
  mResult = matrixOne * matrixTwo;
  CHECK_ARRAY_CLOSE( matrixResult.GetArray(), mResult.GetArray(), 9, 0.1f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Assignment)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);

  CMatrix3x3<float> matrix2(2,4,6,
			    8,10,12,
			    14,16,18);

  float aValues[9] = { 1,2,3,
		       4,5,6,
		       7,8,9 };
  float aValues2[9] = { 2,4,6,
		       8,10,12,
		       14,16,18};
  
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);
  CHECK_ARRAY_CLOSE( aValues2, matrix2.GetArray(), 9, 0.00001f);
  matrix = matrix2;
  CHECK_ARRAY_CLOSE( aValues2, matrix.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Identity)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);

  float aValues[9] = { 1,0,0,0,1,0,0,0,1 };
  matrix.IdentityMatrix();
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Zero)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);

  float aValues[9] = { 1,0,0,0,1,0,0,0,1 };
  matrix.IdentityMatrix();
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorPlus)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);

  CMatrix3x3<float> matrix2(2,4,6,
			    8,10,12,
			    14,16,18);
  
  float aValues[9] = { 3,6,9,
		       12,15,18,
		       21,24,27 };
  
  CHECK_ARRAY_CLOSE( aValues, (matrix + matrix2).GetArray(), 9, 0.00001f);
  

}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorMinus)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);

  CMatrix3x3<float> matrix2(2,4,6,
			    8,10,12,
			    14,16,18);
  
  float aValues[9] = { -1.0f,-2.0f,-3.0f,
		       -4.0f, -5.0f, -6.0f,
		       -7.0f,-8.0f, -9.0f };
  
  CHECK_ARRAY_CLOSE( aValues, (matrix - matrix2).GetArray(), 9, 0.00001f);

}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorMinusSingle)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { -1.0f,-2.0f,-3.0f,
		       -4.0f, -5.0f, -6.0f,
		       -7.0f,-8.0f, -9.0f };
  
  CHECK_ARRAY_CLOSE( aValues, (-matrix).GetArray(), 9, 0.00001f);

}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorScale)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { -1.0f,-2.0f,-3.0f,
		       -4.0f, -5.0f, -6.0f,
		       -7.0f,-8.0f, -9.0f };
  
  CHECK_ARRAY_CLOSE( aValues, (matrix * -1.0f).GetArray(), 9, 0.00001f);

}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorScaleAssign)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { -1.0f,-2.0f,-3.0f,
		       -4.0f, -5.0f, -6.0f,
		       -7.0f,-8.0f, -9.0f };
  matrix *= -1.0f;
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);

}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorDivide)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { -1.0f, -2.0f, -3.0f,
		       -4.0f, -5.0f, -6.0f,
		       -7.0f, -8.0f, -9.0f };
  
  CHECK_ARRAY_CLOSE( aValues, (matrix / -1.0f).GetArray(), 9, 0.00001f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorDivideAssignment)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { -1.0f, -2.0f, -3.0f,
		       -4.0f, -5.0f, -6.0f,
		       -7.0f, -8.0f, -9.0f };
  matrix/=-1.0f;
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_At)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);

  CHECK_CLOSE( 1.0f, matrix.At(0,0), 0.00001f);
  CHECK_CLOSE( 2.0f, matrix.At(0,1), 0.00001f);
  CHECK_CLOSE( 3.0f, matrix.At(0,2), 0.00001f);
  CHECK_CLOSE( 4.0f, matrix.At(1,0), 0.00001f);
  CHECK_CLOSE( 5.0f, matrix.At(1,1), 0.00001f);
  CHECK_CLOSE( 6.0f, matrix.At(1,2), 0.00001f);
  CHECK_CLOSE( 7.0f, matrix.At(2,0), 0.00001f);
  CHECK_CLOSE( 8.0f, matrix.At(2,1), 0.00001f);
  CHECK_CLOSE( 9.0f, matrix.At(2,2), 0.00001f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_OperatorParenthesis)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);

  CHECK_CLOSE( 1.0f, matrix(0,0), 0.00001f);
  CHECK_CLOSE( 2.0f, matrix(0,1), 0.00001f);
  CHECK_CLOSE( 3.0f, matrix(0,2), 0.00001f);
  CHECK_CLOSE( 4.0f, matrix(1,0), 0.00001f);
  CHECK_CLOSE( 5.0f, matrix(1,1), 0.00001f);
  CHECK_CLOSE( 6.0f, matrix(1,2), 0.00001f);
  CHECK_CLOSE( 7.0f, matrix(2,0), 0.00001f);
  CHECK_CLOSE( 8.0f, matrix(2,1), 0.00001f);
  CHECK_CLOSE( 9.0f, matrix(2,2), 0.00001f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Transpose)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { 1,4,7,
		       2,5,8,
		       3,6,9 };
  matrix.Transpose();
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_GetTransposition)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { 1,4,7,
		       2,5,8,
		       3,6,9 };

  CHECK_ARRAY_CLOSE( aValues, matrix.GetTransposition().GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Trace)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  CHECK_CLOSE( 15.0f, matrix.Trace(), 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_SwapRows)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { 7,8,9,
		       4,5,6,
		       1,2,3};
  matrix.SwapRows( 0, 2 );
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_DivideRowBy)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { 1,2,3,
		       4,5,6,
		       -7,-8,-9};
  matrix.DivideRowBy( 2, -1.0f );
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);

  float aValues2[9] = { 0.5f,1.0f,1.5f,
			4,5,6,
		        -7,-8,-9};
  matrix.DivideRowBy( 0, 2.0f );
  CHECK_ARRAY_CLOSE( aValues2, matrix.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_MultiplyRowBy)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { 1,2,3,
		       4,5,6,
		       -7,-8,-9};
  matrix.MultiplyRowBy( 2, -1.0f );
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);

  float aValues2[9] = { 0.5f,1.0f,1.5f,
			4,5,6,
		        -7,-8,-9};
  matrix.MultiplyRowBy( 0, 0.5f );
  CHECK_ARRAY_CLOSE( aValues2, matrix.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_DivideColumnBy)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { 1,2,-3,
		       4,5,-6,
		       7,8,-9};
  matrix.DivideColumnBy( 2, -1.0f );
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);

  float aValues2[9] = { 1,2,-1.5f,
			4,5,-3.0f,
		        7,8,-4.5f};
  matrix.DivideColumnBy( 2, 2.0f );
  CHECK_ARRAY_CLOSE( aValues2, matrix.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_MultiplyColumnBy)
{
  CMatrix3x3<float> matrix(1,2,3,
			   4,5,6,
			   7,8,9);
  
  float aValues[9] = { 1,2,-3,
		       4,5,-6,
		       7,8,-9};
  matrix.MultiplyColumnBy( 2, -1.0f );
  CHECK_ARRAY_CLOSE( aValues, matrix.GetArray(), 9, 0.00001f);

  float aValues2[9] = { 1,2,-1.5f,
			4,5,-3.0f,
		        7,8,-4.5f};
  matrix.MultiplyColumnBy( 2, 0.5f );
  CHECK_ARRAY_CLOSE( aValues2, matrix.GetArray(), 9, 0.00001f);
}
/////////////////////////////////////////////////////////////////
