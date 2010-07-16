/////////////////////////////////////////////////////////////////
#ifndef __MooseVector2_h__
#define __MooseVector2_h__
/////////////////////////////////////////////////////////////////
#include <ostream>
#include <math.h>
#include <MooseMathUtils.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
    template <typename TYPE>
    class MOOSE_API CVector2
    {
    protected:
       // The actual storage array for local values.
      TYPE  m_aValues[2];
    public:
      
      ////////////////////
      /// The default constructor.
      CVector2()
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param x x-coordinate value
      /// \param y y-coordinate value
      CVector2( TYPE x, TYPE y) 
      {
	m_aValues[0] = x;
	m_aValues[1] = y;
      }
      ////////////////////
      /// The copy constructor.
      CVector2( const CVector2 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
      }
      ////////////////////
      /// Returns values as array.
      /// \returns pointer to array of values.
      inline TYPE * GetArray()
      {
	return m_aValues;
      }
      ////////////////////
      /// Returns values as array.
      /// \returns pointer to array of values.
      inline const TYPE * GetArray() const
      {
	return m_aValues;
      }
      ////////////////////
      /// The parametrized constructor copies data from an existing array.
      inline void Set( const TYPE *pArray )
      {
	m_aValues[0] = pArray[0];
	m_aValues[1] = pArray[1];
      }
      
      ////////////////////
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      inline const TYPE & At( unsigned int nIndex ) const
      {
          return m_aValues[nIndex];
      }
      ////////////////////
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      inline TYPE & At( unsigned int nIndex )
      {
          return m_aValues[nIndex];
      }
#ifndef SWIG
      ////////////////////
      /// [] operator to help accessing the values, 
      /// \returns returns reference to value at index iIndex.
      inline TYPE & operator[](unsigned int iIndex)
      {
	return m_aValues[iIndex];
      }
      ////////////////////
      /// [] operator to help accessing the values, 
      /// \returns returns reference to value at index iIndex.
      inline const TYPE & operator[](unsigned int iIndex) const
      {
	return m_aValues[iIndex];
      }
#endif
      ////////////////////
      /// The assignment operator.
      /// \returns reference to this object.
      inline void operator=(const CVector2 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
      }
#ifndef SWIG
      ////////////////////
      /// The equality comparison operator.
      /// \returns truth value - non-zero if each corresponding component in 
      /// both vectors have the same value, zero otherwise.
      inline int operator==(const CVector2 & vVector) const
      {
          return (m_aValues[0] == vVector.m_aValues[0] &&
                  m_aValues[1] == vVector.m_aValues[1] );
      }
      ////////////////////
      /// The inequality comparison operator.
      /// \returns truth value - non-zero if one or more components in 
      /// both vectors does not have same value, zero otherwise.
      inline int operator!=(const CVector2 & vVector) const
      {
	return (m_aValues[0] != vVector.m_aValues[0] ||
		m_aValues[1] != vVector.m_aValues[1] );
      }
#endif
      ////////////////////
      /// Calculates the dot product.
      inline TYPE Dot( const CVector2 & vVector ) const
      {
	return ( (m_aValues[0] * vVector.m_aValues[0]) + 
		 (m_aValues[1] * vVector.m_aValues[1]) );
      }
      ////////////////////
      /// Returns the length of this vector. 
      inline TYPE Length() const
      {
	return sqrtf( m_aValues[0]*m_aValues[0] +
		      m_aValues[1]*m_aValues[1] );
      }
      ////////////////////
      /// Returns the squared length of this vector.
      /// \return this dot this.
      inline TYPE LengthSqr() const
      {
	return m_aValues[0]*m_aValues[0] + 
	       m_aValues[1]*m_aValues[1] ;
      }
      ////////////////////
      /// Converts this into unit vector, ie. |this| = 1.0.
      inline void Normalize()
      {
	TYPE t1DivLength = 1.0f / Length();
	m_aValues[0] *= t1DivLength;
	m_aValues[1] *= t1DivLength;
      }
      ////////////////////
      /// Returns normalized vector of this.
      /// \param CVector2 normalized vector of this.
      inline CVector2 GetNormalized() const
      {
	TYPE t1DivLength = 1.0f / Length();
	return CVector2( m_aValues[0] * t1DivLength,
			 m_aValues[1] * t1DivLength );
	
      }
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector2 operator*(TYPE tScalar) const
      {
	return CVector2(m_aValues[0] * tScalar,
			m_aValues[1] * tScalar );
      }
        inline CVector2 operator*( const CVector2 & vVec ) const
        {
            return CVector2(m_aValues[0]*vVec[0],
                            m_aValues[1]*vVec[1]);
        
        }
#if !defined(SWIG)
        inline void operator*=( const CVector2 & vVec )
        {
            m_aValues[0]*=vVec[0];
            m_aValues[1]*=vVec[1];
            
        }
        inline void operator*=(  TYPE value )
        {
            m_aValues[0]*=value;
            m_aValues[1]*=value;
        }
#endif
      ////////////////////
      /// The division operator.
      /// \param tDivider division value.
      /// \returns Divided vector.
      inline CVector2 operator/(TYPE tDivider) const
      {
	return CVector2(m_aValues[0] / tDivider,
			m_aValues[1] / tDivider);
      }
      ////////////////////
      /// The minus prefix operation.
      /// \return Negated vector.
      inline CVector2 operator-() const
      {
	return CVector2(-m_aValues[0],-m_aValues[1] );
      }
      ////////////////////
      /// The minus operation.
      /// \param vVector Vector to be subtracted from this.
      /// \return Resulting vector.
      inline CVector2 operator-(const CVector2 & vVector) const
      {
	return CVector2( m_aValues[0]-vVector[0],
			 m_aValues[1]-vVector[1]);
      }
      ////////////////////
      /// Addition operator.
      /// \param vVector vector to be added.
      /// \returns sum vector.
      inline CVector2 operator+( const CVector2 & vVector) const
      {
	return CVector2(m_aValues[0] + vVector[0],
			m_aValues[1] + vVector[1]);
      }
#ifndef SWIG
      ////////////////////
      /// Sum with assigning.
      /// \param vVector vector to be added.
      inline void operator+=(const CVector2 & vVector)
      {
	m_aValues[0] += vVector.m_aValues[0];
	m_aValues[1] += vVector.m_aValues[1];
      }
      ////////////////////
      /// Subtraction with assigning.
      /// \param vVector Vector which is subtracted.
      inline void operator-=(const CVector2 & vVector)
      {
	m_aValues[0] -= vVector.m_aValues[0];
	m_aValues[1] -= vVector.m_aValues[1];
      }
      ////////////////////
      /// Divides vector with fDivider.
      /// \param tDivider Divider value.
      inline void operator/=(TYPE tDivider)
      {
	TYPE t1DivValue = 1.0f / tDivider;
	m_aValues[0] *= t1DivValue;
	m_aValues[1] *= t1DivValue;
      }

      ////////////////////
      /// Multiplying with scalar must be able from both sides.
      /// \param tScalar Scaling magnitude.
      /// \param vVector Vector which is scaled.
      /// \returns Scaled vector.
      inline friend CVector2 operator*(TYPE tScalar, const CVector2 & vVect) 
      {
	return CVector2(vVect.m_aValues[0] * tScalar,
			vVect.m_aValues[1] * tScalar);
      }
      ////////////////////
      /// For more simplistic output (in debugging sense).
      /// \param stream std::ostream stream.
      /// \param vVector vector which values are printed to stream.
      /// \returns reference to std::ostream
      friend std::ostream& operator<<(std::ostream &stream, const CVector2 &vVector)
      {
	stream << vVector[0] << "," << vVector[1] ;
	return stream;
      }
#endif
    }; // CVector2
    /////////////////////////////////////////////////////////////////
    template <>
    class MOOSE_API CVector2<float>
    {
    protected:
      // The actual storage array for local values.
      float  m_aValues[2];
    public:
      
      ////////////////////
      /// The default constructor.
      CVector2()
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param x x-coordinate value
      /// \param y y-coordinate value
      CVector2( float x, float y)
      {
	m_aValues[0] = x;
	m_aValues[1] = y;
      }
      ////////////////////
      /// The copy constructor.
      CVector2( const CVector2 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
      }
      ////////////////////
      /// Returns values as array.
      /// \returns pointer to array of values.
      inline float * GetArray()
      {
	return m_aValues;
      }
      ////////////////////
      /// Returns values as array.
      /// \returns pointer to array of values.
      inline const float * GetArray() const
      {
	return m_aValues;
      }
      ////////////////////
      /// The parametrized constructor copies data from an existing array.
      inline void Set( const float *pArray )
      {
	m_aValues[0] = pArray[0];
	m_aValues[1] = pArray[1];
      }
      
      ////////////////////
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      /* inline float operator()( unsigned int nIndex ) const */
/*       { */
/* 	return m_aValues[nIndex]; */
/*       } */
      ////////////////////
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      inline const float & At( unsigned int nIndex ) const
      {
	return m_aValues[nIndex];
      }
      ////////////////////
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      inline float & At( unsigned int nIndex )
      {
          return m_aValues[nIndex];
      }
#ifndef SWIG
      ////////////////////
      /// [] operator to help accessing the values, 
      /// \returns returns reference to value at index iIndex.
      inline float & operator[](unsigned int iIndex)
      {
	return m_aValues[iIndex];
      }
      ////////////////////
      /// [] operator to help accessing the values, 
      /// \returns returns reference to value at index iIndex.
      inline const float & operator[](unsigned int iIndex) const
      {
	return m_aValues[iIndex];
      }
#endif
      ////////////////////
      /// The assignment operator.
      /// \returns reference to this object.
      inline void operator=(const CVector2 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
      }
#ifndef SWIG
      ////////////////////
      /// The equality comparison operator.
      /// \returns truth value - non-zero if each corresponding component in 
      /// both vectors have the same value, zero otherwise.
      inline int operator==(const CVector2 & vVector) const
      {
	return ( QUITE_CLOSE_TO(m_aValues[0], vVector.m_aValues[0]) &&
		 QUITE_CLOSE_TO(m_aValues[1], vVector.m_aValues[1]) );
      }
      ////////////////////
      /// The inequality comparison operator.
      /// \returns truth value - non-zero if one or more components in 
      /// both vectors does not have same value, zero otherwise.
      inline int operator!=(const CVector2 & vVector) const
      {
	return ( !QUITE_CLOSE_TO(m_aValues[0], vVector.m_aValues[0]) ||
		 !QUITE_CLOSE_TO(m_aValues[1], vVector.m_aValues[1]) );
      }
#endif
      ////////////////////
      /// Calculates the dot product.
      inline float Dot( const CVector2 & vVector ) const
      {
	return ( (m_aValues[0] * vVector.m_aValues[0]) + 
		 (m_aValues[1] * vVector.m_aValues[1]) );
      }
      ////////////////////
      /// Returns the length of this vector. 
      inline float Length() const
      {
	return sqrtf( m_aValues[0]*m_aValues[0] +
		      m_aValues[1]*m_aValues[1] );
      }
      ////////////////////
      /// Returns the squared length of this vector.
      /// \return this dot this.
      inline float LengthSqr() const
      {
	return m_aValues[0]*m_aValues[0] + 
	       m_aValues[1]*m_aValues[1] ;
      }
      ////////////////////
      /// Converts this into unit vector, ie. |this| = 1.0.
      inline void Normalize()
      {
	float t1DivLength = 1.0f / Length();
	m_aValues[0] *= t1DivLength;
	m_aValues[1] *= t1DivLength;
      }
      ////////////////////
      /// Returns normalized vector of this.
      /// \param CVector2 normalized vector of this.
      inline CVector2 GetNormalized() const
      {
	float t1DivLength = 1.0f / Length();
	return CVector2( m_aValues[0] * t1DivLength,
			 m_aValues[1] * t1DivLength );
	
      }
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector2 operator*(float tScalar) const
      {
	return CVector2(m_aValues[0] * tScalar,
			m_aValues[1] * tScalar );
      }
        inline CVector2 operator*( const CVector2 & vVec ) const
        {
            return CVector2(m_aValues[0]*vVec[0],
                            m_aValues[1]*vVec[1]);
            
        }
#if !defined(SWIG)
        inline void operator*=( const CVector2 & vVec )
        {
            m_aValues[0]*=vVec[0];
            m_aValues[1]*=vVec[1];
        }
        inline void operator*=(  float value )
        {
            m_aValues[0]*=value;
            m_aValues[1]*=value;
        }
#endif
      ////////////////////
      /// The division operator.
      /// \param tDivider division value.
      /// \returns Divided vector.
      inline CVector2 operator/(float tDivider) const
      {
	return CVector2(m_aValues[0] / tDivider,
			m_aValues[1] / tDivider);
      }
      ////////////////////
      /// The minus prefix operation.
      /// \return Negated vector.
      inline CVector2 operator-() const
      {
	return CVector2(-m_aValues[0],-m_aValues[1] );
      }
      ////////////////////
      /// The minus operation.
      /// \param vVector Vector to be subtracted from this.
      /// \return Resulting vector.
      inline CVector2 operator-(const CVector2 & vVector) const
      {
	return CVector2( m_aValues[0]-vVector[0],
			 m_aValues[1]-vVector[1]);
      }
      ////////////////////
      /// Addition operator.
      /// \param vVector vector to be added.
      /// \returns sum vector.
      inline CVector2 operator+( const CVector2 & vVector) const
      {
	return CVector2(m_aValues[0] + vVector[0],
			m_aValues[1] + vVector[1]);
      }
#ifndef SWIG
      ////////////////////
      /// Sum with assigning.
      /// \param vVector vector to be added.
      inline void operator+=(const CVector2 & vVector)
      {
	m_aValues[0] += vVector.m_aValues[0];
	m_aValues[1] += vVector.m_aValues[1];
      }
      ////////////////////
      /// Subtraction with assigning.
      /// \param vVector Vector which is subtracted.
      inline void operator-=(const CVector2 & vVector)
      {
	m_aValues[0] -= vVector.m_aValues[0];
	m_aValues[1] -= vVector.m_aValues[1];
      }
      ////////////////////
      /// Divides vector with fDivider.
      /// \param tDivider Divider value.
      inline void operator/=(float tDivider)
      {
	float t1DivValue = 1.0f / tDivider;
	m_aValues[0] *= t1DivValue;
	m_aValues[1] *= t1DivValue;
      }
      ////////////////////
      /// Multiplying with scalar must be able from both sides.
      /// \param tScalar Scaling magnitude.
      /// \param vVector Vector which is scaled.
      /// \returns Scaled vector.
      inline friend CVector2 operator*(float tScalar, const CVector2 & vVect) 
      {
	return CVector2(vVect.m_aValues[0] * tScalar,
			vVect.m_aValues[1] * tScalar);
      }
      ////////////////////
      /// For more simplistic output (in debugging sense).
      /// \param stream std::ostream stream.
      /// \param vVector vector which values are printed to stream.
      /// \returns reference to std::ostream
      friend std::ostream& operator<<(std::ostream &stream, const CVector2 &vVector)
      {
	stream << vVector[0] << "," << vVector[1] ;
	return stream;
      }
#endif
    }; // CVector2<float>
  } // namespace Math
} // namespace Moose
#endif
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
