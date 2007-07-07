#include <UnitTest++/UnitTest++.h>
#include "PhoenixMath.h"
/////////////////////////////////////////////////////////////////
TEST(CMatrix3x3_Inverse)
{
  CMatrix3x3<float> matrix(2, 3, 8,
			   6,0,-3,
			   -1,3,2  );

  CMatrix3x3<float> mCorrectInv(  1.0f/15.0f,  2.0f/15.0f, -1.0f/15.0f,
				 -1.0f/15.0f,  4.0f/45.0f,  2.0f/5.0f,
				  2.0f/15.0f, -1.0f/15.0f, -2.0f/15.0f);

  
  float aIdentity[9] = { 1.000,  0.000,  0.000,
			 0.000,  1.000,  0.000,
			 0.000,  0.000,  1.000};
  
  CMatrix3x3<float> mInv;
  
  CHECK_ARRAY_CLOSE( aIdentity, (matrix * mCorrectInv).GetArray(), 9, 0.002f);

  CHECK( InverseMatrix( matrix, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( mCorrectInv.GetArray(), mInv.GetArray(), 9, 0.002f);
  
  CMatrix3x3<float> mId;
  mId.IdentityMatrix();

  CHECK( InverseMatrix( mId, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( aIdentity, mInv.GetArray(), 9, 0.002f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix4x4_Inverse)
{
  CMatrix4x4<float> matrix( 0.500, 23.000,  1.000,  0.000,
			    1.440, 12.000,  4.000,  9.000,
			    13.000,  0.567,  1.100,  1.340,
			    0.889, 12.440, 23.000,  0.000 );

  CMatrix4x4<float> mCorrectInv(  0.005, -0.012,  0.078, -0.002,
				  0.044,  0.000, -0.002, -0.002,
				 -0.024,  0.000, -0.002,  0.045,
				 -0.049,  0.113, -0.009, -0.017 );

  
  float aIdentity[16] = { 1.0,  0.0,  0.0, 0.0,
			  0.0,  1.0,  0.0, 0.0f,
			  0.0,  0.0,  1.0, 0.0f,
			  0.0,  0.0,  0.0, 1.0f};
  
  CMatrix4x4<float> mInv;
  
  CHECK_ARRAY_CLOSE( aIdentity, (matrix * mCorrectInv).GetArray(), 16, 0.02f);

  CHECK( InverseMatrix( matrix, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( mCorrectInv.GetArray(), mInv.GetArray(), 16, 0.001f);
  
  CMatrix4x4<float> mId;
  mId.IdentityMatrix();

  CHECK( InverseMatrix( mId, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( aIdentity, mInv.GetArray(), 16, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(CMatrix2x2_Inverse)
{
  CMatrix2x2<float> matrix(2, 3, 1,2  );
  CMatrix2x2<float> mCorrectInv( 2.000, -3.000,  -1.000,  2.000);
  float aIdentity[4] = { 1.000,  0.000,  
			 0.000,  1.000 };
  
  CMatrix2x2<float> mInv;
  CHECK_ARRAY_CLOSE( aIdentity, (matrix * mCorrectInv).GetArray(), 4, 0.002f);
  CHECK( InverseMatrix( matrix, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( mCorrectInv.GetArray(), mInv.GetArray(), 4, 0.002f);
  
  CMatrix2x2<float> mId;
  mId.IdentityMatrix();

  CHECK( InverseMatrix( mId, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( aIdentity, mInv.GetArray(), 4, 0.002f);
}
/////////////////////////////////////////////////////////////////