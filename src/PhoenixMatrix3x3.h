#ifndef __PhoenixMatrix3x3_h__
#define __PhoenixMatrix3x3_h__
/////////////////////////////////////////////////////////////////
#include <iostream>
#include <math.h>
#include "PhoenixMathUtils.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Math 
  {
    /////////////////////////////////////////////////////////////////
    /// The matrix template class 
    template <typename TYPE>
    class CMatrix3x3
    {
    protected:
      TYPE m_aValues[9];
    public:
      ////////////////////
      /// The default constructor.
      CMatrix3x3()  {}

      ////////////////////
      /// Parametrized constructor.
      /// \param m00 index value at row 0, column 0
      /// \param m01 index value at row 0, column 1
      /// \param m02 index value at row 0, column 2
      /// \param m10 index value at row 1, column 0
      /// \param m11 index value at row 1, column 1
      /// \param m12 index value at row 1, column 2
      /// \param m20 index value at row 2, column 0
      /// \param m21 index value at row 2, column 1
      /// \param m22 index value at row 2, column 2
      CMatrix3x3( TYPE m00, TYPE m01, TYPE m02, 
		  TYPE m10, TYPE m11, TYPE m12, 
		  TYPE m20, TYPE m21, TYPE m22  )  
      {
	m_aValues[0]  = m00; m_aValues[1]  = m01; m_aValues[2]  = m02; 
	m_aValues[3]  = m10; m_aValues[4]  = m11; m_aValues[5]  = m12; 
	m_aValues[6]  = m20; m_aValues[7]  = m21; m_aValues[8]  = m22; 
      }

      ////////////////////
      // Returns a singledimensional array of floats in Row-Major mode.
      // The values of the first row are listed first,
      // then the second row, etc.
      inline TYPE *GetArray() 
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
      /// \returns CMatrix3x3 The multiplication result.
      inline CMatrix3x3 operator*(const CMatrix3x3 &mMatrix) const
      {

	return CMatrix3x3( m_aValues[0] * mMatrix.m_aValues[0]+
			   m_aValues[1] * mMatrix.m_aValues[3]+
			   m_aValues[2] * mMatrix.m_aValues[6],

			   m_aValues[0] * mMatrix.m_aValues[1]+
			   m_aValues[1] * mMatrix.m_aValues[4]+
			   m_aValues[2] * mMatrix.m_aValues[7],

			   m_aValues[0] * mMatrix.m_aValues[2]+
			   m_aValues[1] * mMatrix.m_aValues[5]+
			   m_aValues[2] * mMatrix.m_aValues[8],
			   
			   m_aValues[3] * mMatrix.m_aValues[0]+
			   m_aValues[4] * mMatrix.m_aValues[3]+
			   m_aValues[5] * mMatrix.m_aValues[6],

			   m_aValues[3] * mMatrix.m_aValues[1]+
			   m_aValues[4] * mMatrix.m_aValues[4]+
			   m_aValues[5] * mMatrix.m_aValues[7],

			   m_aValues[3] * mMatrix.m_aValues[2]+
			   m_aValues[4] * mMatrix.m_aValues[5]+
			   m_aValues[5] * mMatrix.m_aValues[8],

			   m_aValues[6] * mMatrix.m_aValues[0]+
			   m_aValues[7] * mMatrix.m_aValues[3]+
			   m_aValues[8] * mMatrix.m_aValues[6],

			   m_aValues[6] * mMatrix.m_aValues[1]+
			   m_aValues[7] * mMatrix.m_aValues[4]+
			   m_aValues[8] * mMatrix.m_aValues[7],

			   m_aValues[6] * mMatrix.m_aValues[2]+
			   m_aValues[7] * mMatrix.m_aValues[5]+
			   m_aValues[8] * mMatrix.m_aValues[8] );
      }
      ////////////////////
      /// The assignment operator.
      /// \param m Matrix where values are copied to this.
      inline void operator=(const CMatrix3x3 &m)
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

      }
      ////////////////////
      /// Adds to matrices together.
      /// \param m Matrix to be added to this.
      /// \return CMatrix3x3 sum matrix.
      inline CMatrix3x3 operator+( const CMatrix3x3 &m) const
      {

	return CMatrix3x3( m_aValues[0] + m.m_aValues[0],
			   m_aValues[1] + m.m_aValues[1],
			   m_aValues[2] + m.m_aValues[2],
			   m_aValues[3] + m.m_aValues[3],
			   m_aValues[4] + m.m_aValues[4],
			   m_aValues[5] + m.m_aValues[5],
			   m_aValues[6] + m.m_aValues[6],
			   m_aValues[7] + m.m_aValues[7],
			   m_aValues[8] + m.m_aValues[8] );
      }
      ////////////////////
      /// Subtracts matrix from this matrix.
      /// \param m Matrix to be subtracted from this.
      /// \return CMatrix3x3 subtraction result.
      inline CMatrix3x3 operator-( const CMatrix3x3 &m) const
      {
	return CMatrix3x3( m_aValues[0] - m.m_aValues[0],
			   m_aValues[1] - m.m_aValues[1],
			   m_aValues[2] - m.m_aValues[2],
			   m_aValues[3] - m.m_aValues[3],
			   m_aValues[4] - m.m_aValues[4],
			   m_aValues[5] - m.m_aValues[5],
			   m_aValues[6] - m.m_aValues[6],
			   m_aValues[7] - m.m_aValues[7],
			   m_aValues[8] - m.m_aValues[8] );
      }
      ////////////////////
      /// Negates matrix values.
      /// \return CMatrix3x3 with negated values.
      inline CMatrix3x3 operator-() const
      {
	return CMatrix3x3( -m_aValues[0],  -m_aValues[1],  -m_aValues[2],   
			   -m_aValues[3],  -m_aValues[4],  -m_aValues[5],  
			   -m_aValues[6],   -m_aValues[7], -m_aValues[8]);
      }
      ////////////////////
      /// Multiplies matrix with scalar value.
      /// \param value The value used in multiplication.
      /// \return CMatrix3x3 multiplication result.
      inline CMatrix3x3 operator*( TYPE value ) const
      {
	return CMatrix3x3( m_aValues[0] * value,
			   m_aValues[1] * value,
			   m_aValues[2] * value,
			   m_aValues[3] * value,
			   m_aValues[4] * value,
			   m_aValues[5] * value,
			   m_aValues[6] * value,
			   m_aValues[7] * value,
			   m_aValues[8] * value);
      }
      ////////////////////
      /// Multiplies matrix with scalar value.
      /// \param value The value used in multiplication.
      inline void operator*=( TYPE value ) 
      {
	m_aValues[0] *= value,
	m_aValues[1] *= value,
	m_aValues[2] *= value,
	m_aValues[3] *= value,
	m_aValues[4] *= value,
	m_aValues[5] *= value,
	m_aValues[6] *= value,
	m_aValues[7] *= value,
	m_aValues[8] *= value;
      }
      ////////////////////
      /// Divides matrix with scalar value value.
      /// \param value division value, must not be zero!!!.
      /// \return CMatrix3x3 division result.
      inline CMatrix3x3 operator/( TYPE value ) const
      {
	TYPE t1DivValue = 1.0f / value;
	return CMatrix3x3( m_aValues[0] * t1DivValue,
			   m_aValues[1] * t1DivValue,
			   m_aValues[2] * t1DivValue,
			   m_aValues[3] * t1DivValue,
			   m_aValues[4] * t1DivValue,
			   m_aValues[5] * t1DivValue,
			   m_aValues[6] * t1DivValue,
			   m_aValues[7] * t1DivValue,
			   m_aValues[8] * t1DivValue);
      }
      ////////////////////
      /// Returns value (logically ) located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-2.
      /// \param iCol Column id. Must be 0-2.
      /// \returns Value at matrix[ iRow, iCol ].
      inline const TYPE At(unsigned int iRow, unsigned int iCol) const
      {
	return m_aValues[(iRow*3)+iCol];
      }
      ////////////////////
      /// Returns reference to value logically located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-2.
      /// \param iCol Column id. Must be 0-2.
      /// \returns reference to value at matrix[ iRow, iCol ].
      inline TYPE &operator()( unsigned int iRow, unsigned int iCol )
      {
	return m_aValues[(iRow*3)+iCol];
      }
      ////////////////////
      /// Returns reference to value logically located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-2.
      /// \param iCol Column id. Must be 0-2.
      /// \returns reference to value at matrix[ iRow, iCol ].
      inline const TYPE &operator()( unsigned int iRow, unsigned int iCol ) const
      {
	return m_aValues[(iRow*3)+iCol];
      }
      ////////////////////
      /// Transposes matrix.
      inline void Transpose()
      {
	TYPE tTemp = m_aValues[1];
	m_aValues[1] = m_aValues[3];
	m_aValues[3] = tTemp;

	tTemp = m_aValues[2];
	m_aValues[2] = m_aValues[6];
	m_aValues[6] = tTemp;

	tTemp = m_aValues[5];
	m_aValues[5] = m_aValues[7];
	m_aValues[7] = tTemp;
	
      }
      ////////////////////
      /// Returns transposed matrix as a copy.
      inline CMatrix3x3 GetTransposition() const
      {
	return CMatrix3x3( m_aValues[0],  m_aValues[3],  m_aValues[6],
			   m_aValues[1],  m_aValues[4],  m_aValues[7],
			   m_aValues[2],  m_aValues[5],  m_aValues[8] );
      }
      ////////////////////
      /// Returns the sum of the diagonal elements.
      inline TYPE Trace() const
      {
	return m_aValues[0] + m_aValues[4] + m_aValues[8];
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
      }
      ////////////////////
      /// Swaps the two rows.
      /// \param iRow1 Row one. Must be 0-2
      /// \param iRow2 Row two. Must be 0-2
      inline void SwapRows(unsigned int iRow1, unsigned int iRow2)
      {
	TYPE tmpRow[3];
	// copy iRow1th row to tmpRow
	tmpRow[0] = (*this)(iRow1, 0);
	tmpRow[1] = (*this)(iRow1, 1);
	tmpRow[2] = (*this)(iRow1, 2);
	// copy iRow2th row to iRow1th row
	(*this)(iRow1,0) = (*this)(iRow2, 0);
	(*this)(iRow1,1) = (*this)(iRow2, 1);
	(*this)(iRow1,2) = (*this)(iRow2, 2);
	// copy tmpRow row to iRow2
	(*this)(iRow2, 0) = tmpRow[0];
	(*this)(iRow2, 1) = tmpRow[1];
	(*this)(iRow2, 2) = tmpRow[2];
      }
      ////////////////////
      /// Divides row with given value.
      /// \param iRow Which row is divided. Must be 0-2.
      /// \param tDivider value which is the divider.
      inline void DivideRowBy(unsigned int iRow, TYPE tDivider )
      {
	TYPE t1DivValue = 1.0f / tDivider;
	(*this)(iRow,0) *= t1DivValue;
	(*this)(iRow,1) *= t1DivValue;
	(*this)(iRow,2) *= t1DivValue;
      }
      ////////////////////
      /// Multiplies row with value.
      /// \param iRow Which row is multiplied. Must be 0-2.
      /// \param tMultiplier multiplication value.
      inline void MultiplyRowBy(unsigned int iRow, TYPE tMultiplier )
      {
	
	(*this)(iRow,0) *= tMultiplier;
	(*this)(iRow,1) *= tMultiplier;
	(*this)(iRow,2) *= tMultiplier;
	
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
      }
      
      

      ////////////////////
      /// Prints the matrix values to stream.
      /// \param stream The output stream where matrix is printed.
      /// \param mat Matrix to be printed.
      /// \return a reference to output stream.
      friend std::ostream& operator<<(std::ostream &stream, const CMatrix3x3 &mat)
      {
	unsigned int iIndex = 0;
	for( iIndex = 0; iIndex<9; iIndex++)
	{
	  if ( iIndex != 0 && !(iIndex % 3)){
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
	m_aValues[0] = 1; m_aValues[1] = 0; m_aValues[2] = 0; 
	m_aValues[3] = 0; m_aValues[4] = 1; m_aValues[5] = 0; 
	m_aValues[6] = 0; m_aValues[7] = 0; m_aValues[8] = 1; 
      }
      ////////////////////
      /// Zeroes matrix.
      inline void ZeroMatrix()
      {
	m_aValues[0] = 0; m_aValues[1] = 0; m_aValues[2] = 0; 
	m_aValues[3] = 0; m_aValues[4] = 0; m_aValues[5] = 0; 
	m_aValues[6] = 0; m_aValues[7] = 0; m_aValues[8] = 0; 
      }
    }; // template CMatrix3x3
    
  }; // namespace Math
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
