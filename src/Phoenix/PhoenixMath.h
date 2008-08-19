/******************************************************************
 * TODO: Write SRT-transform class. 3d game engine design, pp. 144-145.
 *
 *
 ******************************************************************/ 
#ifndef __PhoenixMath_h__
#define __PhoenixMath_h__
/////////////////////////////////////////////////////////////////
#include <iostream>
#include <math.h>
#include "PhoenixVector2.h"
#include "PhoenixVector3.h"
#include "PhoenixVector4.h"
#include "PhoenixMatrix4x4.h"
#include "PhoenixMatrix3x3.h"
#include "PhoenixMatrix2x2.h"
#include "PhoenixQuaternion.h"
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixMathGeometry.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Math 
  {
    
    ////////////////////
    /// Generates a Rotation matrix which corresponds to rotation of fRadians rad
    /// over axis (fX, fY, fZ)
    PHOENIX_API void RotationMatrix(float fX, float fY, float fZ, float fRadians, CMatrix4x4<float> & matrix);
    ////////////////////
    /// Generates a Rotation matrix which corresponds to rotation of fRadians rad
    /// over axis vAxis
    PHOENIX_API void RotationMatrix(const Phoenix::Math::CVector3<float> &vAxis, float fRadians, CMatrix4x4<float> & matrix);
    ////////////////////
    /// Generates a rotation matrix from (x,y,z) -axis rotations 
    /// rotation fX radians using X-axis, fY rad over Y-axis and fZ rad over Z-axis.
    PHOENIX_API void RotationMatrix(float fX, float fY, float fZ, CMatrix4x4<float> & matrix  );
    ////////////////////
    /// Generates a rotation matrix from (x,y,z) -axis rotations 
    /// rotation vRadians[X] radians using X-axis, 
    /// vRadians[Y] rad over Y-axis and vRadians[Z] rad over Z-axis.
    PHOENIX_API void RotationMatrix(const Phoenix::Math::CVector3<float> & vRadians, CMatrix4x4<float> & matrix  );

    ////////////////////
    /// Generates a translation matrix .
    /// \param fX X-axis translation.
    /// \param fY Y-axis translation.
    /// \param fZ Z-axis translation.
    /// \param Result matrix.
    inline void TranslationMatrix(float fX, float fY, float fZ, CMatrix4x4<float> & matrix ) 
    {
      matrix.Set(1,0,0,fX,
		 0,1,0,fY,
		 0,0,1,fZ,
		 0,0,0,1);
    }
    ////////////////////
    /// Generates a translation matrix.
    /// \param vValues Vector with translations for x,y,z axes.
    /// \param Result matrix.
    inline void TranslationMatrix(const Phoenix::Math::CVector3<float> &vValues,
				  CMatrix4x4<float> & matrix )
    {
      matrix.Set( 1,0,0, vValues[0],
		  0,1,0, vValues[1],
		  0,0,1, vValues[2],
		  0,0,0, 1);

    }
    ////////////////////
    /// Creates uniform scale matrix.
    /// \param fScale scaling value.
    /// \param Result matrix.
    inline void UniformScaleMatrix( float fScale, CMatrix4x4<float> &matrix )
    {
      matrix.Set( fScale,  0,0,0,
		  0, fScale, 0,0,
		  0,0,  fScale,0,
		  0,0,0,       1 );
    }
    
    ////////////////////
    /// Constructs a Row-Major TBN (Tangent, Bitangent, Normal)-matrix for bump mapping
    PHOENIX_API void GetTBNMatrix( Phoenix::Math::CVector3<float> vPoint0, 
		       Phoenix::Math::CVector3<float> vPoint1, 
		       Phoenix::Math::CVector3<float> vPoint2,
		       Phoenix::Math::CVector2<float> vTexCoord0, 
		       Phoenix::Math::CVector2<float> vTexCoord2, 
		       Phoenix::Math::CVector2<float> vTexCoord3,
		       CMatrix3x3<float> & matrix );
    
    ////////////////////
    /// Constructs tangent array of 4-vectors for bump mapping. w-coordinate indicates handedness (1.0f or -1.0f).
    /// \param vertices Vertex coordinates.
    /// \param normals Vertex normals
    /// \param texCoords Texture coordinates for base texture.
    /// \param indices Index array which defines shapes of model (Must be TRI_LIST).
    /// \param tangents CVertexDescriptor where tangents are stored. Must be allocated beforehand with type ELEMENT_TYPE_ATTRIB_4F.
    PHOENIX_API void CalculateTangentArray( const Phoenix::Graphics::CVertexDescriptor &vertices,
				const Phoenix::Graphics::CVertexDescriptor &normals,
				const Phoenix::Graphics::CVertexDescriptor &texCoords,
				const Phoenix::Graphics::CIndexArray &indices,
				Phoenix::Graphics::CVertexDescriptor &tangents);
    ////////////////////
    /// Constructs tangent array of 4-vectors for bump mapping. w-coordinate indicates handedness (1.0f or -1.0f).
    /// \param vecTriangles Triangles to which vertices tangents are calculated.
    template<class TRIANGLE_TYPE>
    PHOENIX_API void CalculateTangents( std::vector<TRIANGLE_TYPE> & vecTriangles );
    ////////////////////
    // Returns the determinant of the 4x4 float matrix
    PHOENIX_API float	Det(const Phoenix::Math::CMatrix4x4<float> &mMatrix);
    ////////////////////
    // Returns the determinant of the 3x3 float matrix
    PHOENIX_API float Det(const Phoenix::Math::CMatrix3x3<float> &mMatrix);
    ////////////////////
    // Returns the determinant of the 3x3 float matrix
    PHOENIX_API float Det(const Phoenix::Math::CMatrix2x2<float> &mMatrix);
    ////////////////////
    // Returns the iRowth row vector from 3x3 matrix.
    // iRow in range [0..SIZE-1 ]
    PHOENIX_API void GetRowVector(const Phoenix::Math::CMatrix3x3<float> & mMatrix, unsigned int iRow, CVector3<float> & result);
    ////////////////////
    // Returns the iColth column vector from 3x3 matrix.
    // iCol in range [0..SIZE-1 ]
    PHOENIX_API void GetColumnVector(const Phoenix::Math::CMatrix3x3<float> & mMatrix, unsigned int iCol, CVector3<float> & result);
    ////////////////////
    /// Copies values from the vVector to iRowth row of the matrix mMatrix
    PHOENIX_API void SetRowVector(Phoenix::Math::CMatrix3x3<float> &mMatrix, unsigned int iRow, const CVector3<float> & vVector);
    ////////////////////
    /// Copies values from the vVector to iColth column of the matrix mMatrix
    PHOENIX_API void SetColumnVector(Phoenix::Math::CMatrix3x3<float> &mMatrix, unsigned int iCol, const CVector3<float> & vVector);
    ////////////////////
    /// Translates the vector v by matrix m ( doesn't use scaling or rotation )
    PHOENIX_API void Translate(const CVector3<float> &v, const Phoenix::Math::CMatrix4x4<float> & m, CVector3<float> & result) ;
    ////////////////////
    // Translates the vector v by the inverse of translation matrix m ( doesn't use scaling or rotation )
    PHOENIX_API void TranslateInverse(const Phoenix::Math::CVector3<float> & v, const Phoenix::Math::CMatrix4x4<float> & m,  CVector3<float> & result);
    ////////////////////
    // Rotates vector v by matrix m ( doesn't use scaling or translation )
    PHOENIX_API void Rotate( const Phoenix::Math::CVector3<float> & v, const Phoenix::Math::CMatrix4x4<float> &m, CVector3<float>  & result);
    ////////////////////
    // Transforms vector v by matrix (scaling, rotation and translation is applied)
    PHOENIX_API void Transform( const Phoenix::Math::CVector3<float> &v, const Phoenix::Math::CMatrix4x4<float> &m, CVector3<float> & result);
    ////////////////////
    // Multiplies vector vVector from right by matrix mMatrix  ( mMatrix * vVector );
    PHOENIX_API void MultiplyFromRight( const Phoenix::Math::CMatrix3x3<float> & mMatrix, const Phoenix::Math::CVector3<float> &vVector, CVector3<float> & result);
    ////////////////////
    /// Removes row and column from matrix, creating new submatrix.
    /// \param mMatrix orignal matrix.
    /// \param iRowSkip which row is removed.
    /// \param iColSkip which column is removed.
    /// \param result New smaller square matrix.
    PHOENIX_API void RemoveRowAndColumn(const CMatrix4x4<float> & mMatrix, 
			    unsigned int iRowSkip, 
			    unsigned int iColSkip,
			    CMatrix3x3<float> & result);
    ////////////////////
    /// Removes row and column from matrix, creating new submatrix.
    /// \param mMatrix orignal matrix.
    /// \param iRowSkip which row is removed.
    /// \param iColSkip which column is removed.
    /// \param result New smaller square matrix.
    PHOENIX_API void RemoveRowAndColumn(const CMatrix3x3<float> & mMatrix, 
			    unsigned int iRowSkip, 
			    unsigned int iColSkip,
			    CMatrix2x2<float> & result);
    ////////////////////    
    // Returns the covariance matrix of the vertices in the pVertexDescriptor.
    PHOENIX_API void CovarianceMatrix( const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor,
			   CMatrix3x3<float> & result);

    ////////////////////
    // Returns the covariance matrix of the vertices in the pVertexDescriptor using the given indices 
    PHOENIX_API void CovarianceMatrix(  const Phoenix::Graphics::CVertexDescriptor &vertexDescriptor, const Phoenix::Graphics::CIndexArray &indexBuffer, CMatrix3x3<float> & result );

    ////////////////////
    // Calculates the eigenvectors and eigenvalues for matrix mMatrix 
    // using the Jacobi method. For matrices with dimensions less than 10,
    // the algorithm is efficient.
    PHOENIX_API void CalculateEigensystem( CMatrix3x3<float> &mMatrix, 
			       float &fLambda1, float &fLambda2, float &fLambda3,
			       CMatrix3x3<float> &mEigenvectorMatrix);
  
    ////////////////////
    /// Converts a rotation matrix into a quaternion.
    PHOENIX_API void RotationMatrixToQuaternion( const CMatrix4x4<float> &mMatrix, CQuaternion & result );
    PHOENIX_API void RotateAroundPoint( const CVector3<float> & vVector, 
			    const CVector3<float> & vPoint, 
			    const CQuaternion & ref,
			    CVector3<float> & result);
    ////////////////////
    /// Rotates vector using quaternion.
    /// \param qRotation CQuaternion expressing rotation.
    /// \param vVector target of rotation.
   PHOENIX_API  void RotateVector( const CQuaternion &qRotation, CVector3<float> &vVector);
    ////////////////////
    /// Returns a random value between min and max (included).
    template <typename Type> 
    Type GetRandom( Type min, Type max )
    {
      Type val = max - min ;
      if ( min == max)
	return min;
      return  min + (Type)( val *(rand() / (RAND_MAX+1.0f)));
    }
    
    ////////////////////
    /// Converts Quaternion qQuat into rotation axis vAxis and
    /// and angle fAngle. vAxis is not normalized, ie. ||vAxis|| != 1.0f
    PHOENIX_API void QuaternionToRotationAxisAndAngle( const CQuaternion &qQuat, 
					   CVector3<float> &vAxis,
					   float &fAngle );
    ////////////////////
    /// Converts quaternion into rotation matrix.
    /// \param qQuat Quaternion.
    /// \param mMatrix Matrix where result is stored.
    PHOENIX_API void QuaternionToMatrix( const CQuaternion &qQuat, CMatrix4x4<float> &mMatrix);
    ////////////////////
    /// Returns minimal rotation angle between two vectors.
    /// \return rotation angle.
    PHOENIX_API float AngleBetweenVectors( const CVector3<float> &vVect1, const CVector3<float> &vVect2);
    ////////////////////
    /// The numerically stable minimal rotation arc between vectors v0 and v1.
    /// Note that it must be v0 != -v1!
    /// \param v0 some vector.
    /// \param v1 another vector.
    /// \param result Resulting quaternion expressing rotation from v0 to v1. If (v0 == -v1), result is undefined.
    PHOENIX_API void  RotationArc( CVector3<float> v0, CVector3<float> v1, CQuaternion & result );
    ////////////////////
    /// Performs a spherical linear interpolation between quaternions qFrom and qTo.
    /// \param qFrom Rotation which is the beginning.
    /// \param qTo Rotation which is the end.
    /// \param fInterpolation between 0.0f (qFrom) and 1.0f (qTo).
    /// \param result Resulting quaternion.
    PHOENIX_API void Slerp(CQuaternion qFrom, CQuaternion qTo, float fInterpolation, CQuaternion & result) ;
    
    ////////////////////
    /// Inverses matrix.
    /// 2x2 matrix inverse using Gauss-Jordan algorithm with row pivoting.
    /// \param mOrig Matrix which will be inverted.
    /// \param mInverse Matrix where inverted matrix will be stored.
    /// \returns zero, if matrix is invertible and assigns mInverse as the inverted matrix
    ///		 non-zero if matrix is not invertible. mInverse in this case is undefined.
    PHOENIX_API int InverseMatrix( CMatrix2x2<float> mOrig, CMatrix2x2<float> &mInverse);
    ////////////////////
    /// Inverses matrix.
    /// 3x3 matrix inverse using Gauss-Jordan algorithm with row pivoting.
    /// \param mOrig Matrix which will be inverted.
    /// \param mInverse Matrix where inverted matrix will be stored.
    /// \returns zero, if matrix is invertible and assigns mInverse as the inverted matrix
    ///		 non-zero if matrix is not invertible. mInverse in this case is undefined.
    PHOENIX_API int InverseMatrix( CMatrix3x3<float> mOrig, CMatrix3x3<float> &mInverse);    
    ////////////////////
    /// Inverses matrix.
    /// 4x4 matrix inverse using Gauss-Jordan algorithm with row pivoting.
    /// \param mOrig Matrix which will be inverted.
    /// \param mInverse Matrix where inverted matrix will be stored.
    /// \returns zero, if matrix is invertible and assigns mInverse as the inverted matrix
    ///		 non-zero if matrix is not invertible. mInverse in this case is undefined.
    PHOENIX_API int InverseMatrix( CMatrix4x4<float> mOrig, CMatrix4x4<float> &mInverse);
    ////////////////////
    /// Multiplies vector with matrix from left side.
    /// \param mMatrix Multiplier matrix.
    /// \param vVector Vector to be multiplied.
    /// \returns Resulting vector.
    PHOENIX_API CVector4<float> operator*( const CMatrix4x4<float> &mMatrix, const CVector4<float> &vVector);
    ////////////////////
    /// Multiplies vector with matrix from left side.
    /// \param mMatrix Multiplier matrix.
    /// \param vVector Vector to be multiplied.
    /// \returns Resulting vector.
    PHOENIX_API CVector3<float> operator*( const CMatrix3x3<float> &mMatrix, const CVector3<float> &vVector);
  }; // namespace Math
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
template <class TRIANGLE_TYPE>
void 
Phoenix::Math::CalculateTangents( std::vector<TRIANGLE_TYPE> & vecTriangles )
{

  // One tangent for each vertex.
  CVector3<float>  *aTangents = new CVector3<float>[vecTriangles.size()*3];
  // This array is for calculating handness
  CVector3<float> *aTangents2 = new CVector3<float>[vecTriangles.size()*3];
  // initialize tangents to zero
  for(size_t n=0;n<vecTriangles.size()*3;n++)
  {
    aTangents[n][0] = 0.0f;
    aTangents[n][1] = 0.0f;
    aTangents[n][2] = 0.0f;

    aTangents2[n][0] = 0.0f;
    aTangents2[n][1] = 0.0f;
    aTangents2[n][2] = 0.0f;

  }

  CVector3<float> vTmp2;
  CVector3<float> v1,v2,v3;
  CVector2<float> w1,w2,w3;
  CVector3<float> vSdir,vTdir;
  CVector3<float> vTmp;
  CVector4<float> vTangent;

  // for each triangle do
  for( size_t nTri=0; nTri < vecTriangles.size(); nTri++)
  {
    
    v1 = vecTriangles[nTri].GetVertex(0).GetPosition();
    v2 = vecTriangles[nTri].GetVertex(1).GetPosition();
    v3 = vecTriangles[nTri].GetVertex(2).GetPosition();
    
    w1 = vecTriangles[nTri].GetVertex(0).GetTextureCoordinates();
    w2 = vecTriangles[nTri].GetVertex(1).GetTextureCoordinates();
    w3 = vecTriangles[nTri].GetVertex(2).GetTextureCoordinates();

    float x1 = v2[0] - v1[0];
    float x2 = v3[0] - v1[0];
    float y1 = v2[1] - v1[1];
    float y2 = v3[1] - v1[1];
    float z1 = v2[2] - v1[2];
    float z2 = v3[2] - v1[2];
        
    float s1 = w2[0] - w1[0];
    float s2 = w3[0] - w1[0];
    float t1 = w2[1] - w1[1];
    float t2 = w3[1] - w1[1];
        
    float r = 1.0f / (s1 * t2 - s2 * t1);
    vSdir[0] = (t2 * x1 - t1 * x2) * r;
    vSdir[1] = (t2 * y1 - t1 * y2) * r;
    vSdir[2] = (t2 * z1 - t1 * z2) * r;
    
    vTdir[0] = (s1 * x2 - s2 * x1) * r;
    vTdir[1] = (s1 * y2 - s2 * y1) * r;
    vTdir[2] = (s1 * z2 - s2 * z1) * r;
    
    aTangents[nTri*3]   += vSdir;
    aTangents[nTri*3+1] += vSdir;
    aTangents[nTri*3+2] += vSdir;

    aTangents2[nTri*3]   += vTdir;
    aTangents2[nTri*3+1] += vTdir;
    aTangents2[nTri*3+2] += vTdir;

  }

  // for each vertex do
  for (size_t nTri = 0; nTri < vecTriangles.size(); nTri++)
  {
    for( size_t nVert = 0; nVert < 3; nVert++ )
    {
      
      v1 = vecTriangles[nTri].GetVertex(nVert).GetNormal();
      v2 = aTangents[(nTri*3)+nVert];
      
      // Gram-Schmidt orthogonalize
      vTmp2 = v2 - (v1 * (v1.Dot(v2)));
      vTmp2.Normalize();

      vTangent[0] = vTmp2[0];
      vTangent[1] = vTmp2[1];
      vTangent[2] = vTmp2[2];
      
      // Calculate handedness
      vTmp = aTangents2[nTri*3+nVert];
      vTangent[3] = v1.Cross(v2).Dot(vTmp) < 0.0f ? -1.0f : 1.0f;
      vecTriangles[nTri].GetVertex(nVert).SetAttrib4( vTangent );
    }
  }
  delete aTangents;
  delete aTangents2;
  aTangents = aTangents2 = NULL;

}
/////////////////////////////////////////////////////////////////
#endif
