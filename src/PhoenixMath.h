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
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Math 
  {
    ////////////////////
    /// Generates a Rotation matrix which corresponds to rotation of fRadians rad
    /// over axis (fX, fY, fZ)
    CMatrix4x4<float> RotationMatrix(float fX, float fY, float fZ, float fRadians);
    ////////////////////
    /// Generates a Rotation matrix which corresponds to rotation of fRadians rad
    /// over axis vAxis
    CMatrix4x4<float> RotationMatrix(const CVector3<float> &vAxis, float fRadians);
    ////////////////////
    /// Generates a rotation matrix from (x,y,z) -axis rotations 
    /// rotation fX radians using X-axis, fY rad over Y-axis and fZ rad over Z-axis.
    CMatrix4x4<float> RotationMatrix(float fX, float fY, float fZ );
    ////////////////////
    /// Generates a rotation matrix from (x,y,z) -axis rotations 
    /// rotation vRadians[X] radians using X-axis, 
    /// vRadians[Y] rad over Y-axis and vRadians[Z] rad over Z-axis.
    CMatrix4x4<float> RotationMatrix(const CVector3<float> & vRadians );
    ////////////////////
    /// Generates a translation matrix .
    /// \param fX X-axis translation.
    /// \param fY Y-axis translation.
    /// \param fZ Z-axis translation.
    /// \returns Translation Matrix.
    inline CMatrix4x4<float> TranslationMatrix(float fX, float fY, float fZ)
    {
      return CMatrix4x4<float>( 1,0,0,fX,
				0,1,0,fY,
				0,0,1,fZ,
				0,0,0,1);
    }
    ////////////////////
    /// Generates a translation matrix.
    /// \param vValues Vector with translations for x,y,z axes.
    /// \returns Translation Matrix.
    inline CMatrix4x4<float> TranslationMatrix(const CVector3<float> &vValues)
    {
      return CMatrix4x4<float>( 1,0,0, vValues(0),
				0,1,0, vValues(1),
				0,0,1, vValues(2),
				0,0,0, 1);

    }
    ////////////////////
    /// Creates uniform scale matrix.
    /// \param fScale scaling value.
    /// \returns Matrix with scaling property.
    inline CMatrix4x4<float> UniformScaleMatrix( float fScale )
    {
      return CMatrix4x4<float>( fScale,  0,0,0,
				0, fScale, 0,0,
				0,0,  fScale,0,
				0,0,0,       1 );
    }
    

    // Constructs a Row-Major TBN (Tangent, Bitangent, Normal)-matrix for bump mapping
    CMatrix3x3<float> GetTBNMatrix( CVector3<float> vPoint0, CVector3<float> vPoint1, CVector3<float> vPoint2,
				    CVector2<float> vTexCoord0, CVector2<float> vTexCoord2, CVector2<float> vTexCoord3);

    // Returns the determinant of the 4x4 float matrix
    float	Det(const CMatrix4x4<float> &mMatrix);
    // Returns the determinant of the 3x3 float matrix
    float Det(const CMatrix3x3<float> &mMatrix);
    // Returns the determinant of the 3x3 float matrix
    float Det(const CMatrix2x2<float> &mMatrix);
    // Returns the iRowth row vector from 3x3 matrix.
    // iRow in range [0..SIZE-1 ]
    CVector3<float> GetRowVector(CMatrix3x3<float> mMatrix, unsigned int iRow);
    // Returns the iColth column vector from 3x3 matrix.
    // iCol in range [0..SIZE-1 ]
    CVector3<float> GetColumnVector(CMatrix3x3<float> mMatrix, unsigned int iCol);
    // Copies values from the vVector to iRowth row of the matrix mMatrix
    void SetRowVector(CMatrix3x3<float> &mMatrix, unsigned int iRow, CVector3<float> vVector);
    // Copies values from the vVector to iColth column of the matrix mMatrix
    void SetColumnVector(CMatrix3x3<float> &mMatrix, unsigned int iCol, CVector3<float> vVector);
    // Translates the vector v by matrix m ( doesn't use scaling or rotation )
    CVector3<float> Translate(CVector3<float> v, CMatrix4x4<float> m);
    // Translates the vector v by the inverse of translation matrix m ( doesn't use scaling or rotation )
    CVector3<float> TranslateInverse(CVector3<float> v, CMatrix4x4<float> m);
    // Rotates vector v by matrix m ( doesn't use scaling or translation )
    CVector3<float> Rotate( const CVector3<float> & v, const CMatrix4x4<float> &m);
    // Transforms vector v by matrix (scaling, rotation and translation is applied)
    CVector3<float> Transform( const CVector3<float> &v, const CMatrix4x4<float> &m);
    // Multiplies vector vVector from right by matrix mMatrix  ( mMatrix * vVector );
    CVector3<float> MultiplyFromRight( CMatrix3x3<float> mMatrix, CVector3<float> &vVector);
    ////////////////////
    /// Removes row and column from matrix, creating new submatrix.
    /// \param mMatrix orignal matrix.
    /// \param iRowSkip which row is removed.
    /// \param iColSkip which column is removed.
    /// \returns New smaller square matrix.
    CMatrix3x3<float> RemoveRowAndColumn(CMatrix4x4<float> mMatrix, 
					 unsigned int iRowSkip, 
					 unsigned int iColSkip);
    ////////////////////
    /// Removes row and column from matrix, creating new submatrix.
    /// \param mMatrix orignal matrix.
    /// \param iRowSkip which row is removed.
    /// \param iColSkip which column is removed.
    /// \returns New smaller square matrix.
    CMatrix2x2<float> RemoveRowAndColumn(CMatrix3x3<float> mMatrix, 
					 unsigned int iRowSkip, 
					 unsigned int iColSkip);
    
    // Returns the covariance matrix of the vertices in the pVertexArray
    //CMatrix3x3<float> CovarianceMatrix( float *pVertexArray, unsigned int nNumVertices );

    // Returns the covariance matrix of the vertices in the pVertexArray using the given indices 
    //CMatrix3x3<float> CovarianceMatrix( float *pVertexArray, const CIndexArray &indexBuffer );

    // Calculates the eigenvectors and eigenvalues for matrix mMatrix 
    // using the Jacobi method. For matrices with dimensions less than 10,
    // the algorithm is efficient.

    void CalculateEigensystem( CMatrix3x3<float> &mMatrix, 
			       float &fLambda1, float &fLambda2, float &fLambda3,
			       CMatrix3x3<float> &mEigenvectorMatrix);
  
    ////////////////////
    /// Converts a rotation matrix into a quaternion.
    CQuaternion RotationMatrixToQuaternion( const CMatrix4x4<float> &mMatrix );
    CVector3<float> RotateAroundPoint( const CVector3<float> & vVector, 
				       const CVector3<float> & vPoint, 
				       const CQuaternion & ref );
    ////////////////////
    /// Rotates vector using quaternion.
    /// \param qRotation CQuaternion expressing rotation.
    /// \param vVector target of rotation.
    void RotateVector( const CQuaternion &qRotation, CVector3<float> &vVector);
    ////////////////////
    /// Returns a random value between min and max (included).
    template <typename Type> 
    Type GetRandom( Type min, Type max )
    {
      Type val = max - min ;
      if ( min == max)
	return min;
      return  min + (Type)( (val *rand()) / (RAND_MAX+1) );
    }
  
    ////////////////////
    /// Converts Quaternion qQuat into rotation axis vAxis and
    /// and angle fAngle. vAxis is not normalized, ie. ||vAxis|| != 1.0f
    void QuaternionToRotationAxisAndAngle( const CQuaternion &qQuat, 
					   CVector3<float> &vAxis,
					   float &fAngle );
    ////////////////////
    /// Converts quaternion into rotation matrix
    void QuaternionToMatrix( const CQuaternion &qQuat, CMatrix4x4<float> &mMatrix);


    ////////////////////
    /// Performs a spherical linear interpolation between quaternions qFrom and qTo.
    /// \param qFrom Rotation which is the beginning.
    /// \param qTo Rotation which is the end.
    /// \float fInterpolation between 0.0f (qFrom) and 1.0f (qTo).
    CQuaternion Slerp(CQuaternion qFrom, CQuaternion qTo, float fInterpolation);
    ////////////////////
    /// The numerically stable minimal rotation arc between vectors v0 and v1.
    /// Note that it must be v0 != -v1!
    /// \param v0 some vector.
    /// \param v1 another vector.
    /// \returns Quaternion expressing rotation from v0 to v1. If (v0 == -v1), result is undefined.
    CQuaternion RotationArc( CVector3<float> v0, CVector3<float> v1);
    ////////////////////
    /// Inverses matrix.
    /// 4x4 matrix inverse using Gauss-Jordan algorithm with row pivoting.
    /// \param mOrig Matrix which will be inverted.
    /// \param mInverse Matrix where inverted matrix will be stored.
    /// \returns zero, if matrix is invertible and assigns mInverse as the inverted matrix
    ///		 non-zero if matrix is not invertible. mInverse in this case is undefined.
    int InverseMatrix( CMatrix4x4<float> mOrig, CMatrix4x4<float> &mInverse);
    ////////////////////
    /// Inverses matrix.
    /// 3x3 matrix inverse using Gauss-Jordan algorithm with row pivoting.
    /// \param mOrig Matrix which will be inverted.
    /// \param mInverse Matrix where inverted matrix will be stored.
    /// \returns zero, if matrix is invertible and assigns mInverse as the inverted matrix
    ///		 non-zero if matrix is not invertible. mInverse in this case is undefined.
    int InverseMatrix( CMatrix3x3<float> mOrig, CMatrix3x3<float> &mInverse);
    ////////////////////
    /// Inverses matrix.
    /// 2x2 matrix inverse using Gauss-Jordan algorithm with row pivoting.
    /// \param mOrig Matrix which will be inverted.
    /// \param mInverse Matrix where inverted matrix will be stored.
    /// \returns zero, if matrix is invertible and assigns mInverse as the inverted matrix
    ///		 non-zero if matrix is not invertible. mInverse in this case is undefined.
    int InverseMatrix( CMatrix2x2<float> mOrig, CMatrix2x2<float> &mInverse);

  }; // namespace Math
}; // namespace Phoenix
#endif

      

