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
  
  CHECK_ARRAY_CLOSE( aIdentity, (matrix * mCorrectInv).GetArray(), 9, 0.0001f);

  CHECK( InverseMatrix( matrix, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( mCorrectInv.GetArray(), mInv.GetArray(), 9, 0.0001f);
  
  CMatrix3x3<float> mId;
  mId.IdentityMatrix();

  CHECK( InverseMatrix( mId, mInv ) == 0 );
  CHECK_ARRAY_CLOSE( aIdentity, mInv.GetArray(), 9, 0.0001f);
}
/////////////////////////////////////////////////////////////////
