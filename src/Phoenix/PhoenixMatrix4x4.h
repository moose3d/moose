#ifndef __PhoenixMatrix4x4_h__
#define __PhoenixMatrix4x4_h__
#include <iostream>
#include "PhoenixMathUtils.h"
#include "PhoenixAPI.h"
namespace Phoenix 
{
  namespace Math 
  {
    /////////////////////////////////////////////////////////////////
    /// The matrix template class 
    template <typename TYPE>
    class PHOENIX_API CMatrix4x4 
    {
    protected:
      TYPE m_aValues[16];
    public:
      ////////////////////
      /// The default constructor.
      CMatrix4x4()  {}

      ////////////////////
      /// Parametrized constructor.
      /// \param m00 index value at row 0, column 0
      /// \param m01 index value at row 0, column 1
      /// \param m02 index value at row 0, column 2
      /// \param m03 index value at row 0, column 3
      /// \param m10 index value at row 1, column 0
      /// \param m11 index value at row 1, column 1
      /// \param m12 index value at row 1, column 2
      /// \param m13 index value at row 1, column 3
      /// \param m20 index value at row 2, column 0
      /// \param m21 index value at row 2, column 1
      /// \param m22 index value at row 2, column 2
      /// \param m23 index value at row 2, column 3
      /// \param m30 index value at row 3, column 0
      /// \param m31 index value at row 3, column 1
      /// \param m32 index value at row 3, column 2
      /// \param m33 index value at row 3, column 3
      CMatrix4x4( TYPE m00, TYPE m01, TYPE m02, TYPE m03,
		   TYPE m10, TYPE m11, TYPE m12, TYPE m13,
		   TYPE m20, TYPE m21, TYPE m22, TYPE m23,
		   TYPE m30, TYPE m31, TYPE m32, TYPE m33 )  
      {
	m_aValues[0]  = m00; m_aValues[1]  = m01; m_aValues[2]  = m02; m_aValues[3]  = m03;
	m_aValues[4]  = m10; m_aValues[5]  = m11; m_aValues[6]  = m12; m_aValues[7]  = m13;
	m_aValues[8]  = m20; m_aValues[9]  = m21; m_aValues[10] = m22; m_aValues[11] = m23;
	m_aValues[12] = m30; m_aValues[13] = m31; m_aValues[14] = m32; m_aValues[15] = m33;
      }
      ////////////////////
      /// Assigns matrix values.
      /// \param m00 index value at row 0, column 0
      /// \param m01 index value at row 0, column 1
      /// \param m02 index value at row 0, column 2
      /// \param m03 index value at row 0, column 3
      /// \param m10 index value at row 1, column 0
      /// \param m11 index value at row 1, column 1
      /// \param m12 index value at row 1, column 2
      /// \param m13 index value at row 1, column 3
      /// \param m20 index value at row 2, column 0
      /// \param m21 index value at row 2, column 1
      /// \param m22 index value at row 2, column 2
      /// \param m23 index value at row 2, column 3
      /// \param m30 index value at row 3, column 0
      /// \param m31 index value at row 3, column 1
      /// \param m32 index value at row 3, column 2
      /// \param m33 index value at row 3, column 3
      void Set( TYPE m00, TYPE m01, TYPE m02, TYPE m03,
		TYPE m10, TYPE m11, TYPE m12, TYPE m13,
		TYPE m20, TYPE m21, TYPE m22, TYPE m23,
		TYPE m30, TYPE m31, TYPE m32, TYPE m33 )  
      {
	m_aValues[0]  = m00; m_aValues[1]  = m01; m_aValues[2]  = m02; m_aValues[3]  = m03;
	m_aValues[4]  = m10; m_aValues[5]  = m11; m_aValues[6]  = m12; m_aValues[7]  = m13;
	m_aValues[8]  = m20; m_aValues[9]  = m21; m_aValues[10] = m22; m_aValues[11] = m23;
	m_aValues[12] = m30; m_aValues[13] = m31; m_aValues[14] = m32; m_aValues[15] = m33;
      }
      ////////////////////
      // Returns a singledimensional array of floats in Row-Major mode.
      // The values of the first row are listed first,
      // then the second row, etc.
      TYPE *GetArray() 
      {
	return m_aValues;
      }
      ////////////////////
      // Returns a singledimensional array of floats in Row-Major mode.
      // The values of the first row are listed first,
      // then the second row, etc.
      const TYPE *GetArray() const
      {
	return m_aValues;
      }
      ////////////////////
      /// The effective operation of this * mMatrix.
      /// This multiplies mMatrix from the left side.
      /// \param mMatrix right side of multiplication.
      /// \returns CMatrix4x4 The multiplication result.
      CMatrix4x4 operator*(const CMatrix4x4 &mMatrix) const
      {

	return CMatrix4x4( m_aValues[0] * mMatrix.m_aValues[0]+
			   m_aValues[1] * mMatrix.m_aValues[4]+
			   m_aValues[2] * mMatrix.m_aValues[8]+
			   m_aValues[3] * mMatrix.m_aValues[12],
			   
			   m_aValues[0] * mMatrix.m_aValues[1]+
			   m_aValues[1] * mMatrix.m_aValues[5]+
			   m_aValues[2] * mMatrix.m_aValues[9]+
			   m_aValues[3] * mMatrix.m_aValues[13],

			   m_aValues[0] * mMatrix.m_aValues[2]+
			   m_aValues[1] * mMatrix.m_aValues[6]+
			   m_aValues[2] * mMatrix.m_aValues[10]+
			   m_aValues[3] * mMatrix.m_aValues[14],

			   m_aValues[0] * mMatrix.m_aValues[3]+
			   m_aValues[1] * mMatrix.m_aValues[7]+
			   m_aValues[2] * mMatrix.m_aValues[11]+
			   m_aValues[3] * mMatrix.m_aValues[15],

			   m_aValues[4] * mMatrix.m_aValues[0]+
			   m_aValues[5] * mMatrix.m_aValues[4]+
			   m_aValues[6] * mMatrix.m_aValues[8]+
			   m_aValues[7] * mMatrix.m_aValues[12],
			   
			   m_aValues[4] * mMatrix.m_aValues[1]+
			   m_aValues[5] * mMatrix.m_aValues[5]+
			   m_aValues[6] * mMatrix.m_aValues[9]+
			   m_aValues[7] * mMatrix.m_aValues[13],

			   m_aValues[4] * mMatrix.m_aValues[2]+
			   m_aValues[5] * mMatrix.m_aValues[6]+
			   m_aValues[6] * mMatrix.m_aValues[10]+
			   m_aValues[7] * mMatrix.m_aValues[14],

			   m_aValues[4] * mMatrix.m_aValues[3]+
			   m_aValues[5] * mMatrix.m_aValues[7]+
			   m_aValues[6] * mMatrix.m_aValues[11]+
			   m_aValues[7] * mMatrix.m_aValues[15],

			   m_aValues[8] * mMatrix.m_aValues[0]+
			   m_aValues[9] * mMatrix.m_aValues[4]+
			   m_aValues[10] * mMatrix.m_aValues[8]+
			   m_aValues[11] * mMatrix.m_aValues[12],
			   
			   m_aValues[8] * mMatrix.m_aValues[1]+
			   m_aValues[9] * mMatrix.m_aValues[5]+
			   m_aValues[10] * mMatrix.m_aValues[9]+
			   m_aValues[11] * mMatrix.m_aValues[13],

			   m_aValues[8] * mMatrix.m_aValues[2]+
			   m_aValues[9] * mMatrix.m_aValues[6]+
			   m_aValues[10] * mMatrix.m_aValues[10]+
			   m_aValues[11] * mMatrix.m_aValues[14],

			   m_aValues[8] * mMatrix.m_aValues[3]+
			   m_aValues[9] * mMatrix.m_aValues[7]+
			   m_aValues[10] * mMatrix.m_aValues[11]+
			   m_aValues[11] * mMatrix.m_aValues[15],

			   m_aValues[12] * mMatrix.m_aValues[0]+
			   m_aValues[13] * mMatrix.m_aValues[4]+
			   m_aValues[14] * mMatrix.m_aValues[8]+
			   m_aValues[15] * mMatrix.m_aValues[12],
			   
			   m_aValues[12] * mMatrix.m_aValues[1]+
			   m_aValues[13] * mMatrix.m_aValues[5]+
			   m_aValues[14] * mMatrix.m_aValues[9]+
			   m_aValues[15] * mMatrix.m_aValues[13],

			   m_aValues[12] * mMatrix.m_aValues[2]+
			   m_aValues[13] * mMatrix.m_aValues[6]+
			   m_aValues[14] * mMatrix.m_aValues[10]+
			   m_aValues[15] * mMatrix.m_aValues[14],

			   m_aValues[12] * mMatrix.m_aValues[3]+
			   m_aValues[13] * mMatrix.m_aValues[7]+
			   m_aValues[14] * mMatrix.m_aValues[11]+
			   m_aValues[15] * mMatrix.m_aValues[15]
			   );
      }
      ////////////////////
      /// The assignment operator.
      /// \param m Matrix where values are copied to this.
      void operator=(const CMatrix4x4 &m)
      {
	m_aValues[0] = m.m_aValues[0];
	m_aValues[1] = m.m_aValues[1];
	m_aValues[2] = m.m_aValues[2];
	m_aValues[3] = m.m_aValues[3];
	m_aValues[4] = m.m_aValues[4];
	m_aValues[5] = m.m_aValues[5];
	m_aValues[6] = m.m_aValues[6];
	m_aValues[7] = m.m_aValues[7];
	m_aValues[8] = m.m_aValues[8];
	m_aValues[9] = m.m_aValues[9];
	m_aValues[10] = m.m_aValues[10];
	m_aValues[11] = m.m_aValues[11];
	m_aValues[12] = m.m_aValues[12];
	m_aValues[13] = m.m_aValues[13];
	m_aValues[14] = m.m_aValues[14];
	m_aValues[15] = m.m_aValues[15];
      }
      ////////////////////
      /// Adds to matrices together.
      /// \param m Matrix to be added to this.
      /// \return CMatrix4x4 sum matrix.
      CMatrix4x4 operator+( const CMatrix4x4 &m) const
      {

	return CMatrix4x4( m_aValues[0] + m.m_aValues[0],
			   m_aValues[1] + m.m_aValues[1],
			   m_aValues[2] + m.m_aValues[2],
			   m_aValues[3] + m.m_aValues[3],
			   m_aValues[4] + m.m_aValues[4],
			   m_aValues[5] + m.m_aValues[5],
			   m_aValues[6] + m.m_aValues[6],
			   m_aValues[7] + m.m_aValues[7],
			   m_aValues[8] + m.m_aValues[8],
			   m_aValues[9] + m.m_aValues[9],
			   m_aValues[10] + m.m_aValues[10],
			   m_aValues[11] + m.m_aValues[11],
			   m_aValues[12] + m.m_aValues[12],
			   m_aValues[13] + m.m_aValues[13],
			   m_aValues[14] + m.m_aValues[14],
			   m_aValues[15] + m.m_aValues[15]);
      }
      ////////////////////
      /// Subtracts matrix from this matrix.
      /// \param m Matrix to be subtracted from this.
      /// \return CMatrix4x4 subtraction result.
      CMatrix4x4 operator-( const CMatrix4x4 &m) const
      {
	return CMatrix4x4( m_aValues[0] - m.m_aValues[0],
			   m_aValues[1] - m.m_aValues[1],
			   m_aValues[2] - m.m_aValues[2],
			   m_aValues[3] - m.m_aValues[3],
			   m_aValues[4] - m.m_aValues[4],
			   m_aValues[5] - m.m_aValues[5],
			   m_aValues[6] - m.m_aValues[6],
			   m_aValues[7] - m.m_aValues[7],
			   m_aValues[8] - m.m_aValues[8],
			   m_aValues[9] - m.m_aValues[9],
			   m_aValues[10] - m.m_aValues[10],
			   m_aValues[11] - m.m_aValues[11],
			   m_aValues[12] - m.m_aValues[12],
			   m_aValues[13] - m.m_aValues[13],
			   m_aValues[14] - m.m_aValues[14],
			   m_aValues[15] - m.m_aValues[15]);
      }
      ////////////////////
      /// Negates matrix values.
      /// \return CMatrix4x4 with negated values.
      CMatrix4x4 operator-() const
      {
	return CMatrix4x4( -m_aValues[0],  -m_aValues[1],  -m_aValues[2],   -m_aValues[3],
			   -m_aValues[4],  -m_aValues[5],  -m_aValues[6],   -m_aValues[7],
			   -m_aValues[8],  -m_aValues[9],  -m_aValues[10],  -m_aValues[11],
			   -m_aValues[12], -m_aValues[13], -m_aValues[14],  -m_aValues[15]);
      }
      ////////////////////
      /// Multiplies matrix with scalar value.
      /// \param value The value used in multiplication.
      /// \return CMatrix4x4 multiplication result.
      CMatrix4x4 operator*( TYPE value ) const
      {
	return CMatrix4x4( m_aValues[0] * value,
			   m_aValues[1] * value,
			   m_aValues[2] * value,
			   m_aValues[3] * value,
			   m_aValues[4] * value,
			   m_aValues[5] * value,
			   m_aValues[6] * value,
			   m_aValues[7] * value,
			   m_aValues[8] * value,
			   m_aValues[9] * value,
			   m_aValues[10] * value,
			   m_aValues[11] * value,
			   m_aValues[12] * value,
			   m_aValues[13] * value,
			   m_aValues[14] * value,
			   m_aValues[15] * value);
      }
      ////////////////////
      /// Divides matrix with scalar value value.
      /// \param value division value, must not be zero!!!.
      /// \return CMatrix4x4 division result.
      CMatrix4x4 operator/( TYPE value ) const
      {
	TYPE t1DivValue = 1.0f / value;
	return CMatrix4x4( m_aValues[0] * t1DivValue,
			   m_aValues[1] * t1DivValue,
			   m_aValues[2] * t1DivValue,
			   m_aValues[3] * t1DivValue,
			   m_aValues[4] * t1DivValue,
			   m_aValues[5] * t1DivValue,
			   m_aValues[6] * t1DivValue,
			   m_aValues[7] * t1DivValue,
			   m_aValues[8] * t1DivValue,
			   m_aValues[9] * t1DivValue,
			   m_aValues[10] * t1DivValue,
			   m_aValues[11] * t1DivValue,
			   m_aValues[12] * t1DivValue,
			   m_aValues[13] * t1DivValue,
			   m_aValues[14] * t1DivValue,
			   m_aValues[15] * t1DivValue);
      }
      ////////////////////
      /// Returns value (logically ) located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-3.
      /// \param iCol Column id. Must be 0-3.
      /// \returns Value at matrix[ iRow, iCol ].
      inline const TYPE At(unsigned int iRow, unsigned int iCol) const
      {
	return m_aValues[(iRow*4)+iCol];
      }
      ////////////////////
      /// Returns reference to value logically located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-3.
      /// \param iCol Column id. Must be 0-3.
      /// \returns reference to value at matrix[ iRow, iCol ].
      inline TYPE &operator()( unsigned int iRow, unsigned int iCol )
      {
	return m_aValues[(iRow*4)+iCol];
      }
      ////////////////////
      /// Returns reference to value logically located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-3.
      /// \param iCol Column id. Must be 0-3.
      /// \returns reference to value at matrix[ iRow, iCol ].
      inline const TYPE &operator()( unsigned int iRow, unsigned int iCol ) const
      {
	return m_aValues[(iRow*4)+iCol];
      }
      ////////////////////
      /// Transposes matrix.
      void Transpose()
      {
	TYPE tTemp = m_aValues[1];
	m_aValues[1] = m_aValues[4];
	m_aValues[4] = tTemp;

	tTemp = m_aValues[2];
	m_aValues[2] = m_aValues[8];
	m_aValues[8] = tTemp;

	tTemp = m_aValues[3];
	m_aValues[3] = m_aValues[12];
	m_aValues[12] = tTemp;
	
	tTemp = m_aValues[6];
	m_aValues[6] = m_aValues[9];
	m_aValues[9] = tTemp;

	tTemp = m_aValues[7];
	m_aValues[7] = m_aValues[13];
	m_aValues[13] = tTemp;

	tTemp = m_aValues[11];
	m_aValues[11] = m_aValues[14];
	m_aValues[14] = tTemp;
	
      }
      ////////////////////
      /// Returns transposed matrix as a copy.
      CMatrix4x4 GetTransposition() const
      {
	return CMatrix4x4( m_aValues[0],  m_aValues[4],  m_aValues[8],   m_aValues[12],
			   m_aValues[1],  m_aValues[5],  m_aValues[9],   m_aValues[13],
			   m_aValues[2],  m_aValues[6],  m_aValues[10],  m_aValues[14],
			   m_aValues[3],  m_aValues[7],  m_aValues[11],  m_aValues[15]);
      }
      ////////////////////
      /// Returns the sum of the diagonal elements.
      inline TYPE Trace() const
      {
	return m_aValues[0] + m_aValues[5] + m_aValues[10] + m_aValues[15];
      }
      ////////////////////
      /// Division operator with assign.
      /// \param divider value for division.
      inline void operator/=(TYPE divider)
      {
	TYPE t1DivValue = 1.0f / divider;
	m_aValues[0] *= t1DivValue;
	m_aValues[1] *= t1DivValue;
	m_aValues[2] *= t1DivValue;
	m_aValues[3] *= t1DivValue;
	m_aValues[4] *= t1DivValue;
	m_aValues[5] *= t1DivValue;
	m_aValues[6] *= t1DivValue;
	m_aValues[7] *= t1DivValue;
	m_aValues[8] *= t1DivValue;
	m_aValues[9] *= t1DivValue;
	m_aValues[10] *= t1DivValue;
	m_aValues[11] *= t1DivValue;
	m_aValues[12] *= t1DivValue;
	m_aValues[13] *= t1DivValue;
	m_aValues[14] *= t1DivValue;
	m_aValues[15] *= t1DivValue;
      }
      ////////////////////
      /// Swaps the two rows.
      /// \param iRow1 Row one.
      /// \param iRow2 Row two.
      inline void SwapRows(unsigned int iRow1, unsigned int iRow2)
      {
	TYPE tmpRow[4];
	// copy iRow1th row to tmpRow
	for(unsigned int iCol=0;iCol<4;iCol++){
	  tmpRow[iCol] = (*this)(iRow1, iCol);
	}
	// copy iRow2th row to iRow1th row
	for(unsigned int iCol=0;iCol<4;iCol++){
	  (*this)(iRow1,iCol) = (*this)(iRow2, iCol);
	}
	// copy tmpRow row to iRow2
	for(unsigned int iCol=0;iCol<4;iCol++){
	  (*this)(iRow2, iCol) = tmpRow[iCol];
	}
      }
      ////////////////////
      /// Divides row with given value.
      /// \param iRow Which row is divided.
      /// \param tDivider value which is the divider.
      inline void DivideRowBy(unsigned int iRow, TYPE tDivider )
      {
	TYPE t1DivValue = 1.0f / tDivider;
	(*this)(iRow,0) *= t1DivValue;
	(*this)(iRow,1) *= t1DivValue;
	(*this)(iRow,2) *= t1DivValue;
	(*this)(iRow,3) *= t1DivValue;
      }
      ////////////////////
      /// Multiplies row with value.
      /// \param iRow Which row is multiplied.
      /// \param tMultiplier multiplication value.
      inline void MultiplyRowBy(unsigned int iRow, TYPE tMultiplier )
      {
	
	(*this)(iRow,0) *= tMultiplier;
	(*this)(iRow,1) *= tMultiplier;
	(*this)(iRow,2) *= tMultiplier;
	(*this)(iRow,3) *= tMultiplier;

      }
      ////////////////////
      /// Divides column by value.
      /// \param iCol Which column is divided.
      /// \param tDivider value which is the divider.
      inline void DivideColumnBy(unsigned int iCol, TYPE tDivider )
      {

	TYPE t1DivValue = 1.0f / tDivider;
	(*this)(0,iCol) *= t1DivValue;
	(*this)(1,iCol) *= t1DivValue;
	(*this)(2,iCol) *= t1DivValue;
	(*this)(3,iCol) *= t1DivValue;
	
      }
      ////////////////////
      /// Multiplies column with value.
      /// \param iCol Which column is multiplied.
      /// \param tMultiplier multiplication value.
      inline void MultiplyColumnBy(unsigned int iCol, TYPE tMultiplier )
      {
	(*this)(0,iCol) *= tMultiplier;
	(*this)(1,iCol) *= tMultiplier;
	(*this)(2,iCol) *= tMultiplier;
	(*this)(3,iCol) *= tMultiplier;
      }
      ////////////////////
      /// Prints the matrix values to stream.
      /// \param stream The output stream where matrix is printed.
      /// \param mat Matrix to be printed.
      /// \return a reference to output stream.
      friend std::ostream& operator<<(std::ostream &stream, const CMatrix4x4 &mat)
      {
	unsigned int iIndex = 0;
      
	for( iIndex = 0; iIndex<16; iIndex++)
	{
	
	  if ( iIndex != 0 && !(iIndex % 4)){
	    stream << std::endl;
	  }
	  stream << mat.m_aValues[iIndex] << " ";
	
	}
	stream << std::endl;
	return stream;
      }
      ////////////////////
      /// Initializes a matrix to Identity matrix.
      inline void IdentityMatrix()
      {
	m_aValues[0] = 1; m_aValues[1] = 0; m_aValues[2] = 0; m_aValues[3] = 0;
	m_aValues[4] = 0; m_aValues[5] = 1; m_aValues[6] = 0; m_aValues[7] = 0;
	m_aValues[8] = 0; m_aValues[9] = 0; m_aValues[10] = 1; m_aValues[11] = 0;
	m_aValues[12] = 0; m_aValues[13] = 0; m_aValues[14] = 0; m_aValues[15] = 1;
      }
      ////////////////////
      /// Zeroes givem matrix.
      inline void ZeroMatrix()
      {
	m_aValues[0] = 0; m_aValues[1] = 0; m_aValues[2] = 0; m_aValues[3] = 0;
	m_aValues[4] = 0; m_aValues[5] = 0; m_aValues[6] = 0; m_aValues[7] = 0;
	m_aValues[8] = 0; m_aValues[9] = 0; m_aValues[10] = 0; m_aValues[11] = 0;
	m_aValues[12] = 0; m_aValues[13] = 0; m_aValues[14] = 0; m_aValues[15] = 0;
	
      }
    }; // template CMatrix
  }; // namespace Math
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
