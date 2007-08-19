/////////////////////////////////////////////////////////////////
#ifndef __PhoenixVector2_h__
#define __PhoenixVector2_h__
/////////////////////////////////////////////////////////////////
#include <ostream>
#include <math.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    template <typename TYPE>
    class CVector2
    {
    protected:
      /// Is the data referenced vie m_pValues member pointer.
      int m_bExternalData;
      // The actual storage array for local values.
      TYPE  m_aValues[2];
      /// A pointer to an array of TYPEs (m_aValues).
      /// It is with the programmer to utilize this correctly.
      TYPE *m_pValues; 
      /////////////////////////////////////////////////////////////////
    public:
      
      ////////////////////
      /// The default constructor.
      CVector2() : m_bExternalData(0), m_pValues(&m_aValues[0]) 
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param x x-coordinate value
      /// \param y y-coordinate value
      CVector2( TYPE x, TYPE y) : m_bExternalData(0), m_pValues(&m_aValues[0])
      {
	m_aValues[0] = x;
	m_aValues[1] = y;
      }
      ////////////////////
      /// The copy constructor.
      CVector2( const CVector2 & vVector)
      {
	if ( vVector.m_bExternalData )
	{
	  m_pValues = vVector.m_pValues;
	  m_bExternalData = 1;
	} 
	else
	{
	  m_pValues    = &m_aValues[0];
	  m_pValues[0] = vVector.m_pValues[0];
	  m_pValues[1] = vVector.m_pValues[1];
	  m_bExternalData = 0;
	}
      }
      ////////////////////
      /// Returns values as array.
      /// \returns pointer to array of values.
      inline TYPE * GetArray()
      {
	return m_pValues;
      }
      ////////////////////
      /// Returns values as array.
      /// \returns pointer to array of values.
      inline const TYPE * GetArray() const
      {
	return m_pValues;
      }
      ////////////////////
      /// The parametrized constructor copies data from an existing array.
      inline void Set( TYPE *pArray )
      {
	m_pValues[0] = pArray[0];
	m_pValues[1] = pArray[1];
      }
      ////////////////////
      /// Makes the vector to handle data from pArray, it won't be released if object is deleted.
      void UseExternalData( TYPE *pArray )
      {
	m_pValues = pArray;
	m_bExternalData = 1;
      }
      ////////////////////
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      inline TYPE operator()( unsigned int nIndex ) const
      {
	return m_pValues[nIndex];
      }
      ////////////////////
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      inline TYPE At( unsigned int nIndex ) const
      {
	return m_pValues[nIndex];
      }
      ////////////////////
      /// [] operator to help accessing the values, 
      /// \returns returns reference to value at index iIndex.
      inline TYPE & operator[](unsigned int iIndex)
      {
	return m_pValues[iIndex];
      }
      ////////////////////
      /// The assignment operator.
      /// \returns reference to this object.
      inline void operator=(const CVector2 & vVector)
      {
	m_pValues[0] = vVector.m_pValues[0];
	m_pValues[1] = vVector.m_pValues[1];
      }
      ////////////////////
      /// The equality comparison operator.
      /// \returns truth value - non-zero if each corresponding component in 
      /// both vectors have the same value, zero otherwise.
      inline int operator==(const CVector2 & vVector) const
      {
	return (m_pValues[0] == vVector.m_pValues[0] &&
		m_pValues[1] == vVector.m_pValues[1] );
      }
      ////////////////////
      /// The inequality comparison operator.
      /// \returns truth value - non-zero if one or more components in 
      /// both vectors does not have same value, zero otherwise.
      inline int operator!=(const CVector2 & vVector) const
      {
	return (m_pValues[0] != vVector.m_pValues[0] &&
		m_pValues[1] != vVector.m_pValues[1] );
      }
      ////////////////////
      /// Calculates the dot product.
      inline TYPE Dot( const CVector2 & vVector ) const
      {
	return ( (m_pValues[0] * vVector.m_pValues[0]) + 
		 (m_pValues[1] * vVector.m_pValues[1]) );
      }
      ////////////////////
      /// Returns the length of this vector. 
      inline TYPE Length() const
      {
	return sqrtf( m_pValues[0]*m_pValues[0] +
		      m_pValues[1]*m_pValues[1] );
      }
      ////////////////////
      /// Returns the squared length of this vector.
      /// \return this dot this.
      inline TYPE LengthSqr() const
      {
	return m_pValues[0]*m_pValues[0] + 
	       m_pValues[1]*m_pValues[1] ;
      }
      ////////////////////
      /// Converts this into unit vector, ie. |this| = 1.0.
      inline void Normalize()
      {
	TYPE t1DivLength = 1.0f / Length();
	m_pValues[0] *= t1DivLength;
	m_pValues[1] *= t1DivLength;
      }
      ////////////////////
      /// Returns normalized vector of this.
      /// \param CVector2 normalized vector of this.
      inline CVector2 GetNormalized() const
      {
	TYPE t1DivLength = 1.0f / Length();
	return CVector2( m_pValues[0] * t1DivLength,
			 m_pValues[1] * t1DivLength );
	
      }
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector2 operator*(TYPE tScalar) const
      {
	return CVector2(m_pValues[0] * tScalar,
			m_pValues[1] * tScalar );
      }
      ////////////////////
      /// The division operator.
      /// \param tDivider division value.
      /// \returns Divided vector.
      inline CVector2 operator/(TYPE tDivider) const
      {
	return CVector2(m_pValues[0] / tDivider,
			m_pValues[1] / tDivider);
      }
      ////////////////////
      /// The minus prefix operation.
      /// \return Negated vector.
      inline CVector2 operator-() const
      {
	return CVector2(-m_pValues[0],-m_pValues[1] );
      }
      ////////////////////
      /// The minus operation.
      /// \param vVector Vector to be subtracted from this.
      /// \return Resulting vector.
      inline CVector2 operator-(const CVector2 & vVector) const
      {
	return CVector2( m_pValues[0]-vVector(0),
			 m_pValues[1]-vVector(1));
      }
      ////////////////////
      /// Addition operator.
      /// \param vVector vector to be added.
      /// \returns sum vector.
      inline CVector2 operator+( const CVector2 & vVector) const
      {
	return CVector2(m_pValues[0] + vVector(0),
			m_pValues[1] + vVector(1));
      }
      ////////////////////
      /// Sum with assigning.
      /// \param vVector vector to be added.
      inline void operator+=(const CVector2 & vVector)
      {
	m_pValues[0] += vVector.m_pValues[0];
	m_pValues[1] += vVector.m_pValues[1];
      }
      ////////////////////
      /// Subtraction with assigning.
      /// \param vVector Vector which is subtracted.
      inline void operator-=(const CVector2 & vVector)
      {
	m_pValues[0] -= vVector.m_pValues[0];
	m_pValues[1] -= vVector.m_pValues[1];
      }
      ////////////////////
      /// Divides vector with fDivider.
      /// \param tDivider Divider value.
      inline void operator/=(TYPE tDivider)
      {
	TYPE t1DivValue = 1.0f / tDivider;
	m_pValues[0] *= t1DivValue;
	m_pValues[1] *= t1DivValue;
      }
      ////////////////////
      /// Multiplying with scalar must be able from both sides.
      /// \param tScalar Scaling magnitude.
      /// \param vVector Vector which is scaled.
      /// \returns Scaled vector.
      inline friend CVector2 operator*(TYPE tScalar, const CVector2 & vVect) 
      {
	return CVector2(vVect.m_pValues[0] * tScalar,
			vVect.m_pValues[1] * tScalar);
      }
      ////////////////////
      /// For more simplistic output (in debugging sense).
      /// \param stream std::ostream stream.
      /// \param vVector vector which values are printed to stream.
      /// \returns reference to std::ostream
      friend std::ostream& operator<<(std::ostream &stream, const CVector2 &vVector)
      {
	stream << vVector(0) << "," << vVector(1) ;
	return stream;
      }

    }; // CVector2
  } // namespace Math
} // namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
