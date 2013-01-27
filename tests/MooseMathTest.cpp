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
#include <MooseMath.h>
#include <MooseTriangle.h>
using namespace Moose::Math;
using namespace Moose::Graphics;
using namespace Moose::Spatial;
using namespace Moose::Volume;
using std::cerr;
using std::endl;
#include <vector>
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
TEST(CMatrix4x4_Multiply_Vector4)
{
  CMatrix4x4<float> matrix(1, 2, 3, 4,
			   5 ,6 ,7 ,8,
			   9, 10, 11, 12,
			   13, 14, 15, 16);
  CVector4<float> vector(5,6,7,2);
  CVector4<float> vectorTest;
  CVector4<float> vResult(46,126,206,286);
  
  vectorTest = matrix * vector;
  CHECK_ARRAY_CLOSE( vResult.GetArray(), vectorTest.GetArray(), 4, 0.001f);

}
/////////////////////////////////////////////////////////////////
TEST(RotationArc)
{
  CVector3<float> vec1(1,0,0);
  CVector3<float> vec2(0,1,0);
  CQuaternion q;
  RotationArc( vec1, vec2, q);  
  CVector3<float> vRes = vec1;

  RotateVector( q, vRes );
  CHECK_ARRAY_CLOSE( vec2.GetArray(), vRes.GetArray(), 3, 0.001f);
}
/////////////////////////////////////////////////////////////////
TEST(RotationMatrixToQuaternion)
{
  CMatrix4x4<float> mMatrix, mMatrixRes;
  CQuaternion q;
  RotationMatrix( 1, 4, -5, mMatrix);
  RotationMatrixToQuaternion( mMatrix,q );
  QuaternionToMatrix( q, mMatrixRes );
  CHECK_ARRAY_CLOSE( mMatrix.GetArray(), mMatrixRes.GetArray(), 16, 0.001f);
  
}
/////////////////////////////////////////////////////////////////
TEST(CalculateTangentArray_CalculateTangents)
{
  CVertexDescriptor *pVertices  = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, 3);
  CVertexDescriptor *pNormals   = new CVertexDescriptor(ELEMENT_TYPE_NORMAL_3F, 3);
  CVertexDescriptor *pTexCoords = new CVertexDescriptor(ELEMENT_TYPE_TEX_2F, 3);
  CVertexDescriptor *pTangents  = new CVertexDescriptor(ELEMENT_TYPE_ATTRIB_4F, 3);

  CIndexArray  *pIndices = new CIndexArray( PRIMITIVE_TRI_LIST, 3);
  

  CTriangle triangle;
  triangle.GetVertex(0).SetPosition( CVector3<float>(0,0,0));
  triangle.GetVertex(1).SetPosition( CVector3<float>(1,0,0));
  triangle.GetVertex(2).SetPosition( CVector3<float>(1,-4,-1));

  triangle.GetVertex(0).SetNormal( CVector3<float>(1,1,0).GetNormalized());
  triangle.GetVertex(1).SetNormal( CVector3<float>(0,1,0).GetNormalized());
  triangle.GetVertex(2).SetNormal( CVector3<float>(0,1,1).GetNormalized());

  triangle.GetVertex(0).SetTextureCoordinates( 0,0 );
  triangle.GetVertex(1).SetTextureCoordinates( 1,0 );
  triangle.GetVertex(2).SetTextureCoordinates( 1,1 );

  pVertices->GetPointer<float>()[0] = triangle.GetVertex(0).GetPosition()[0];
  pVertices->GetPointer<float>()[1] = triangle.GetVertex(0).GetPosition()[1];
  pVertices->GetPointer<float>()[2] = triangle.GetVertex(0).GetPosition()[2];

  pVertices->GetPointer<float>()[3] = triangle.GetVertex(1).GetPosition()[0];
  pVertices->GetPointer<float>()[4] = triangle.GetVertex(1).GetPosition()[1];
  pVertices->GetPointer<float>()[5] = triangle.GetVertex(1).GetPosition()[2];
  
  pVertices->GetPointer<float>()[6] = triangle.GetVertex(2).GetPosition()[0];
  pVertices->GetPointer<float>()[7] = triangle.GetVertex(2).GetPosition()[1];
  pVertices->GetPointer<float>()[8] = triangle.GetVertex(2).GetPosition()[2];

  pNormals->GetPointer<float>()[0] = triangle.GetVertex(0).GetNormal()[0];
  pNormals->GetPointer<float>()[1] = triangle.GetVertex(0).GetNormal()[1];
  pNormals->GetPointer<float>()[2] = triangle.GetVertex(0).GetNormal()[2];
  
  pNormals->GetPointer<float>()[3] = triangle.GetVertex(1).GetNormal()[0];
  pNormals->GetPointer<float>()[4] = triangle.GetVertex(1).GetNormal()[1];
  pNormals->GetPointer<float>()[5] = triangle.GetVertex(1).GetNormal()[2];

  pNormals->GetPointer<float>()[6] = triangle.GetVertex(2).GetNormal()[0];
  pNormals->GetPointer<float>()[7] = triangle.GetVertex(2).GetNormal()[1];
  pNormals->GetPointer<float>()[8] = triangle.GetVertex(2).GetNormal()[2];

  pTexCoords->GetPointer<float>()[0] = triangle.GetVertex(0).GetTextureCoordinates()[0];
  pTexCoords->GetPointer<float>()[1] = triangle.GetVertex(0).GetTextureCoordinates()[1];

  pTexCoords->GetPointer<float>()[2] = triangle.GetVertex(1).GetTextureCoordinates()[0];
  pTexCoords->GetPointer<float>()[3] = triangle.GetVertex(1).GetTextureCoordinates()[1];

  pTexCoords->GetPointer<float>()[4] = triangle.GetVertex(2).GetTextureCoordinates()[0];
  pTexCoords->GetPointer<float>()[5] = triangle.GetVertex(2).GetTextureCoordinates()[1];

  pIndices->GetPointer<unsigned short int>()[0] = 0;
  pIndices->GetPointer<unsigned short int>()[1] = 1;
  pIndices->GetPointer<unsigned short int>()[2] = 2;

  CalculateTangentArray( *pVertices, *pNormals,
			 *pTexCoords, *pIndices,
			 *pTangents);

 

  std::vector<CTriangle> vecTriangles;
  vecTriangles.push_back(triangle);
  
  CalculateTangents(  vecTriangles );
  
  CVector4<float> vTangent;

  vTangent.Set( &pTangents->GetPointer<float>()[0]);
  CHECK_ARRAY_CLOSE( vecTriangles[0].GetVertex(0).GetAttrib4().GetArray(), vTangent.GetArray(), 4, 0.001f);
  
  vTangent.Set( &pTangents->GetPointer<float>()[4]);
  CHECK_ARRAY_CLOSE( vecTriangles[0].GetVertex(1).GetAttrib4().GetArray(), vTangent.GetArray(), 4, 0.001f);

  vTangent.Set( &pTangents->GetPointer<float>()[8]);
  CHECK_ARRAY_CLOSE( vecTriangles[0].GetVertex(2).GetAttrib4().GetArray(), vTangent.GetArray(), 4, 0.001f);
  
  

  delete pNormals; pNormals = NULL;
  delete pVertices; pVertices = NULL;
  delete pTexCoords; pTexCoords = NULL;
  delete pTangents; pTangents = NULL;
  delete pIndices; pIndices = NULL;
}
/////////////////////////////////////////////////////////////////
TEST(OrientedBoxAxisToRotationMatrix)
{
  COrientedBox box;
  box.SetWidth(1.0);
  box.SetHeight(1.0);
  box.SetLength(1.0);
  box.SetOrientation( CVector3<float>(0,1,0),  // up
		      CVector3<float>(1,0,0), // fwd
		      CVector3<float>(0,0,1)   // right
		      );
  CMatrix4x4<float> mRotation;
  
  CMatrix4x4<float> mIdentity;
  mIdentity.IdentityMatrix();
  OrientedBoxAxisToRotationMatrix( box, mRotation);
  
  CHECK_ARRAY_CLOSE( mIdentity.GetArray(), mRotation.GetArray(), 16, 0.001f);

  box.SetOrientation( CVector3<float>(0,1,0), 
		      CVector3<float>(0,0,-1), 
		      CVector3<float>(1,0,0) );
  
  OrientedBoxAxisToRotationMatrix(box, mRotation);
  CVector3<float> vZ(0,0,-1);
  CVector3<float> vResult;
  CVector3<float> vZReal(1,0,0); // forward, up, right
  Rotate( vZ, mRotation, vResult);

  cerr << vResult << endl;
  CHECK_ARRAY_CLOSE( vZReal.GetArray(), vResult, 3, 0.001f);

}
/////////////////////////////////////////////////////////////////
