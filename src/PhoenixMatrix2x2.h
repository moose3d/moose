#ifndef __PhoenixMatrix2x2_h__
#define __PhoenixMatrix2x2_h__
namespace Phoenix 
{
  namespace Math 
  {
    /////////////////////////////////////////////////////////////////
    /// The matrix template class 
    template <typename TYPE>
    class CMatrix2x2
    {
    protected:
      TYPE m_aValues[4];
    public:
      ////////////////////
      /// The default constructor.
      CMatrix2x2()  {}

      ////////////////////
      /// Parametrized constructor.
      /// \param m00 index value at row 0, column 0
      /// \param m01 index value at row 0, column 1
      /// \param m10 index value at row 1, column 0
      /// \param m11 index value at row 1, column 1
      CMatrix2x2( TYPE m00, TYPE m01,  
		  TYPE m10, TYPE m11 )  
      {
	m_aValues[0]  = m00; m_aValues[1]  = m01; 
	m_aValues[2]  = m10; m_aValues[3]  = m11; 
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
      /// The effective operation of this * mMatrix.
      /// This multiplies mMatrix from the left side.
      /// \param mMatrix right side of multiplication.
      /// \returns CMatrix2x2 The multiplication result.
      CMatrix2x2 operator*(const CMatrix2x2 &mMatrix) const
      {

	return CMatrix2x2( m_aValues[0] * mMatrix.m_aValues[0] + 
			   m_aValues[1] * mMatrix.m_aValues[2],

			   m_aValues[0] * mMatrix.m_aValues[1] + 
			   m_aValues[1] * mMatrix.m_aValues[3],

			   m_aValues[2] * mMatrix.m_aValues[0] +
			   m_aValues[3] * mMatrix.m_aValues[2],

			   m_aValues[2] * mMatrix.m_aValues[1] +
			   m_aValues[3] * mMatrix.m_aValues[3] );
      }
      ////////////////////
      /// The assignment operator.
      /// \param m Matrix where values are copied to this.
      void operator=(const CMatrix2x2 &m)
      {
	m_aValues[0] = m.m_aValues[0];
	m_aValues[1] = m.m_aValues[1];
	m_aValues[2] = m.m_aValues[2];
	m_aValues[3] = m.m_aValues[3];

      }
      ////////////////////
      /// Adds to matrices together.
      /// \param m Matrix to be added to this.
      /// \return CMatrix2x2 sum matrix.
      CMatrix2x2 operator+( const CMatrix2x2 &m) const
      {

	return CMatrix2x2( m_aValues[0] + m.m_aValues[0],
			   m_aValues[1] + m.m_aValues[1],
			   m_aValues[2] + m.m_aValues[2],
			   m_aValues[3] + m.m_aValues[3] );
      }
      ////////////////////
      /// Subtracts matrix from this matrix.
      /// \param m Matrix to be subtracted from this.
      /// \return CMatrix2x2 subtraction result.
      CMatrix2x2 operator-( const CMatrix2x2 &m) const
      {
	return CMatrix2x2( m_aValues[0] - m.m_aValues[0],
			   m_aValues[1] - m.m_aValues[1],
			   m_aValues[2] - m.m_aValues[2],
			   m_aValues[3] - m.m_aValues[3] );
      }
      ////////////////////
      /// Negates matrix values.
      /// \return CMatrix2x2 with negated values.
      CMatrix2x2 operator-() const
      {
	return CMatrix2x2( -m_aValues[0],  -m_aValues[1],  
			   -m_aValues[2],   -m_aValues[3] );
      }
      ////////////////////
      /// Multiplies matrix with scalar value.
      /// \param value The value used in multiplication.
      /// \return CMatrix2x2 multiplication result.
      CMatrix2x2 operator*( TYPE value ) const
      {
	return CMatrix2x2( m_aValues[0] * value,
			   m_aValues[1] * value,
			   m_aValues[2] * value,
			   m_aValues[3] * value);
      }
      ////////////////////
      /// Divides matrix with scalar value value.
      /// \param value division value, must not be zero!!!.
      /// \return CMatrix2x2 division result.
      CMatrix2x2 operator/( TYPE value ) const
      {
	TYPE t1DivValue = 1.0f / value;
	return CMatrix2x2( m_aValues[0] * t1DivValue,
			   m_aValues[1] * t1DivValue,
			   m_aValues[2] * t1DivValue,
			   m_aValues[3] * t1DivValue);
      }
      ////////////////////
      /// Returns value (logically ) located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-1.
      /// \param iCol Column id. Must be 0-1.
      /// \returns Value at matrix[ iRow, iCol ].
      inline const TYPE At(unsigned int iRow, unsigned int iCol) const
      {
	return m_aValues[(iRow*2)+iCol];
      }
      ////////////////////
      /// Returns reference to value logically located at the
      /// iRow:th row and iCol:th column of the matrix. 
      /// \param iRow Row id. Must be 0-1.
      /// \param iCol Column id. Must be 0-1.
      /// \returns reference to value at matrix[ iRow, iCol ].
      inline TYPE &operator()( unsigned int iRow, unsigned int iCol )
      {
	return m_aValues[(iRow*2)+iCol];
      }
      
      ////////////////////
      /// Transposes matrix.
      void Transpose()
      {
	TYPE tTemp = m_aValues[1];
	m_aValues[1] = m_aValues[2];
	m_aValues[2] = tTemp;
      }
      ////////////////////
      /// Returns transposed matrix as a copy.
      CMatrix2x2 GetTransposition() const
      {
	return CMatrix2x2( m_aValues[0],  m_aValues[2],
			   m_aValues[1],  m_aValues[3]);
      }
      ////////////////////
      /// Returns the sum of the diagonal elements.
      inline TYPE Trace() const
      {
	return m_aValues[0] + m_aValues[3];
      }
      ////////////////////
      /// Division operator with assign.
      /// \param divider value for division.
      void operator/=(TYPE divider)
      {
	TYPE t1DivValue = 1.0f / divider;
	m_aValues[0] *= t1DivValue;
	m_aValues[1] *= t1DivValue;
	m_aValues[2] *= t1DivValue;
	m_aValues[3] *= t1DivValue;
      }
      ////////////////////
      /// Swaps the two rows.
      /// \param iRow1 Row one.
      /// \param iRow2 Row two.
      void SwapRows(unsigned int iRow1, unsigned int iRow2)
      {
	TYPE tmpRow[2];
	// copy iRow1th row to tmpRow
	tmpRow[0] = (*this)(iRow1, 0);
	tmpRow[1] = (*this)(iRow1, 1);
	// copy iRow2th row to iRow1th row
	(*this)(iRow1,0) = (*this)(iRow2, 0);
	(*this)(iRow1,1) = (*this)(iRow2, 1);
	// copy tmpRow row to iRow2
	(*this)(iRow2, 0) = tmpRow[0];
	(*this)(iRow2, 1) = tmpRow[1];
      }
      ////////////////////
      /// Divides row with given value.
      /// \param iRow Which row is divided.
      /// \param tDivider value which is the divider.
      void DivideRowBy(unsigned int iRow, TYPE tDivider )
      {
	TYPE t1DivValue = 1.0f / tDivider;
	(*this)(iRow,0) *= t1DivValue;
	(*this)(iRow,1) *= t1DivValue;
      }
      ////////////////////
      /// Multiplies row with value.
      /// \param iRow Which row is multiplied.
      /// \param tMultiplier multiplication value.
      void MultiplyRowBy(unsigned int iRow, TYPE tMultiplier )
      {
	
	(*this)(iRow,0) *= tMultiplier;
	(*this)(iRow,1) *= tMultiplier;
	
      }
      ////////////////////
      /// Divides column by value.
      /// \param iCol Which column is divided.
      /// \param tDivider value which is the divider.
      void DivideColumnBy(unsigned int iCol, TYPE tDivider )
      {

	TYPE t1DivValue = 1.0f / tDivider;
	(*this)(0,iCol) *= t1DivValue;
	(*this)(1,iCol) *= t1DivValue;
		
      }
      ////////////////////
      /// Multiplies column with value.
      /// \param iCol Which column is multiplied.
      /// \param tMultiplier multiplication value.
      void MultiplyColumnBy(unsigned int iCol, TYPE tMultiplier )
      {
	(*this)(0,iCol) *= tMultiplier;
	(*this)(1,iCol) *= tMultiplier;
      }
      ////////////////////
      /// Inverses matrix.
      /// From http://www.devmaster.net/wiki/Matrix#Inverting_Matrices
      /// 4x4 matrix inverse using Gauss-Jordan algorithm with row pivoting
      /// originally written by Nathan Reed, now released into the public domain.
      /// \returns inverted matrix.
      CMatrix2x2 Inverse()
      {
	CMatrix2x2 mOrig;    // The matrix whose inverse we want to take
	CMatrix2x2 mInverse; // initialize to 4x4 identity matrix
	CMatrix2x2::Identity(mInverse);
	unsigned int i, j, pivot;
	mOrig = *this;      
	
	// loop through columns
	for (j = 0; j < 2; ++j)
	{
	  // select pivot element: maximum magnitude in this column
	  for (pivot = 0, i = 1; i < 2; ++i){
	    if (fabs(mOrig(i,j)) > fabs(mOrig(pivot,j))){
	      pivot = i;
	    }
	  }

	  if ( TOO_CLOSE_TO_ZERO( mOrig(pivot,j)) )
	  {
	    std::cerr << "Matrix inverse does not exist!"  << std::endl;
	    return CMatrix2x2(1,0,0,1);
	  }
	
	  // interchange rows to put pivot element on the diagonal
	  // skip if already on diagonal
	  if (pivot != j)
	  {
	    mOrig.SwapRows( pivot, j );
	    mInverse.SwapRows( pivot, j);
	  }
	
	  // divide row by pivot element
	  // skip if already equal to 1
	  if ( mOrig(j,j) != 1.0f )
	  {
	    float temp = mOrig(j,j);
	    mOrig.DivideRowBy(j,temp);
	    mInverse.DivideRowBy(j,temp);
	    // Now the pivot element is 1
	  }
	
	  // subtract this row from others to make the rest of column j zero
	  for (i = 0; i < 2; ++i)
	  {
	    // skip rows that are already zero
	    if ((i != j) && !TOO_CLOSE_TO_ZERO( mOrig(i,j) ) )
	    {
	      float scale = -mOrig(i,j);
	      for(unsigned int iCol=0;iCol<2;iCol++)
	      {
		mOrig(i, iCol )    += mOrig(j,iCol)*scale;
		mInverse(i, iCol ) += mInverse(j,iCol)*scale;
	      }
	    }
	  }
	}
	// When these operations have been completed, 'a' should have been transformed 
	// to the identity matrix and 'b' should have been transformed into the inverse 
	// of the original 'a'.
	return mInverse;
      }
      ////////////////////
      /// Prints the matrix values to stream.
      /// \param stream The output stream where matrix is printed.
      /// \param mat Matrix to be printed.
      /// \return a reference to output stream.
      friend std::ostream& operator<<(std::ostream &stream, const CMatrix2x2 &mat)
      {
	unsigned int iIndex = 0;
	for( iIndex = 0; iIndex<4; iIndex++)
	{
	  if ( iIndex != 0 && !(iIndex % 2)){
	    stream << std::endl;
	  }
	  stream << mat.m_aValues[iIndex] << " ";
	}
	stream << std::endl;
	return stream;
      }
      ////////////////////
      /// Initializes a matrix to Identity matrix.
      void IdentityMatrix()
      {
	m_aValues[0] = 1; m_aValues[1] = 0; 
	m_aValues[2] = 0; m_aValues[3] = 1; 
      }
      ////////////////////
      /// Zeroes matrix.
      void ZeroMatrix()
      {
	m_aValues[0] = 0; m_aValues[1] = 0; 
	m_aValues[2] = 0; m_aValues[3] = 0; 
      }
    }; // template CMatrix2x2
  } // namespace Math
} // namespace Phoenix
#endif
