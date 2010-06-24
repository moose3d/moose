/////////////////////////////////////////////////////////////////
#ifndef __PhoenixVector3_h__
#define __PhoenixVector3_h__
/////////////////////////////////////////////////////////////////
#include <ostream>
#include <math.h>
#include <PhoenixMathUtils.h>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    template <typename TYPE>
    class PHOENIX_API CVector3
    {
    protected:
      // The actual storage array for local values.
      TYPE  m_aValues[3];
    public:
      
      ////////////////////
      /// The default constructor.
      CVector3()
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param x x-coordinate value
      /// \param y y-coordinate value
      /// \param z z-coordinate value
      CVector3( TYPE x, TYPE y, TYPE z) 
      {
	m_aValues[0] = x;
	m_aValues[1] = y;
	m_aValues[2] = z;
      }
      ////////////////////
      /// The copy constructor.
      CVector3( const CVector3 & vVector)
      {
	
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
	
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
	m_aValues[2] = pArray[2];
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
      inline void operator=(const CVector3 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
      }
#ifndef SWIG
      ////////////////////
      /// The equality comparison operator.
      /// \returns truth value - non-zero if each corresponding component in 
      /// both vectors have the same value, zero otherwise.
      inline int operator==(const CVector3 & vVector) const
      {
	return (m_aValues[0] == vVector.m_aValues[0] &&
		m_aValues[1] == vVector.m_aValues[1] &&
	        m_aValues[2] == vVector.m_aValues[2] );
      }
      ////////////////////
      /// The inequality comparison operator.
      /// \returns truth value - non-zero if one or more components in 
      /// both vectors does not have same value, zero otherwise.
      inline int operator!=(const CVector3 & vVector) const
      {
	return (m_aValues[0] != vVector.m_aValues[0] ||
		m_aValues[1] != vVector.m_aValues[1] ||
	        m_aValues[2] != vVector.m_aValues[2] );
      }
#endif
      ////////////////////
      /// Calculates the dot product.
      inline TYPE Dot( const CVector3 & vVector ) const
      {
	return ( (m_aValues[0] * vVector.m_aValues[0]) + 
		 (m_aValues[1] * vVector.m_aValues[1]) + 
		 (m_aValues[2] * vVector.m_aValues[2]) );
      }
      ////////////////////
      /// Returns the length of this vector. 
      inline TYPE Length() const
      {
	return sqrtf( m_aValues[0]*m_aValues[0] +
		      m_aValues[1]*m_aValues[1] +
		      m_aValues[2]*m_aValues[2] );
      }
      ////////////////////
      /// Returns the squared length of this vector.
      /// \return this dot this.
      inline TYPE LengthSqr() const
      {
	return m_aValues[0]*m_aValues[0] + 
	m_aValues[1]*m_aValues[1] + 
	m_aValues[2]*m_aValues[2] ;
      }
      ////////////////////
      /// Returns the result of cross product between this vector and vVector.
      /// \returns CVector3 cross product.
      inline CVector3 Cross( const CVector3 & vVector ) const
      {
	return CVector3(   (m_aValues[1] * vVector[2]) - (m_aValues[2] * vVector[1]),
			   -((m_aValues[0] * vVector[2]) - (m_aValues[2] * vVector[0])),
			   (m_aValues[0] * vVector[1]) - (m_aValues[1] * vVector[0]));
	
      }
      ////////////////////
      /// Converts this into unit vector, ie. |this| = 1.0.
      inline void Normalize()
      {
	TYPE t1DivLength = 1.0f / Length();
	m_aValues[0] *= t1DivLength;
	m_aValues[1] *= t1DivLength;
	m_aValues[2] *= t1DivLength;
      }
      ////////////////////
      /// Returns normalized vector of this.
      /// \param CVector3 normalized vector of this.
      inline CVector3 GetNormalized() const
      {
	TYPE t1DivLength = 1.0f / Length();
	return CVector3( m_aValues[0] * t1DivLength,
			 m_aValues[1] * t1DivLength,
			 m_aValues[2] * t1DivLength );
	
      }
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector3 operator*(TYPE tScalar) const
      {
	return CVector3(m_aValues[0] * tScalar,
			m_aValues[1] * tScalar,
			m_aValues[2] * tScalar);
      }
        inline CVector3 operator*( const CVector3 & vVec ) const
        {
            return CVector3( m_aValues[0]*vVec[0],
                             m_aValues[1]*vVec[1],
                             m_aValues[2]*vVec[2] );
            
        }
#if !defined(SWIG)
        inline void operator*=( const CVector3 & vVec )
        {
            m_aValues[0]*=vVec[0];
            m_aValues[1]*=vVec[1];
            m_aValues[2]*=vVec[2];
        }
#endif
      ////////////////////
      /// The division operator.
      /// \param tDivider division value.
      /// \returns Divided vector.
      inline CVector3 operator/(TYPE tDivider) const
      {
    	  return CVector3(m_aValues[0] / tDivider,
			m_aValues[1] / tDivider,
			m_aValues[2] / tDivider);
      }
      ////////////////////
      /// The minus prefix operation.
      /// \return Negated vector.
      inline CVector3 operator-() const
      {
	return CVector3(-m_aValues[0],-m_aValues[1],-m_aValues[2]);
      }
      ////////////////////
      /// The minus operation.
      /// \param vVector Vector to be subtracted from this.
      /// \return Resulting vector.
      inline CVector3 operator-(const CVector3 & vVector) const
      {
	return CVector3( m_aValues[0]-vVector[0],
			 m_aValues[1]-vVector[1],
			 m_aValues[2]-vVector[2] );
      }
      ////////////////////
      /// Addition operator.
      /// \param vVector vector to be added.
      /// \returns sum vector.
      inline CVector3 operator+( const CVector3 & vVector) const
      {
	return CVector3(m_aValues[0] + vVector[0],
			m_aValues[1] + vVector[1],
			m_aValues[2] + vVector[2]);
      }
#ifndef SWIG
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline void operator*=(TYPE tScalar) const
      {
	m_aValues[0] *= tScalar;
	m_aValues[1] *= tScalar;
	m_aValues[2] *= tScalar;
      }
      ////////////////////
      /// Sum with assigning.
      /// \param vVector vector to be added.
      inline void operator+=(const CVector3 & vVector)
      {
	m_aValues[0] += vVector.m_aValues[0];
	m_aValues[1] += vVector.m_aValues[1];
	m_aValues[2] += vVector.m_aValues[2];
      }
      ////////////////////
      /// Subtraction with assigning.
      /// \param vVector Vector which is subtracted.
      inline void operator-=(const CVector3 & vVector)
      {
	m_aValues[0] -= vVector.m_aValues[0];
	m_aValues[1] -= vVector.m_aValues[1];
	m_aValues[2] -= vVector.m_aValues[2];
      }
      ////////////////////
      /// Divides vector with fDivider.
      /// \param tDivider Divider value.
      inline void operator/=(TYPE tDivider)
      {
	TYPE t1DivValue = 1.0f / tDivider;
	m_aValues[0] *= t1DivValue;
	m_aValues[1] *= t1DivValue;
	m_aValues[2] *= t1DivValue;
      }
      ////////////////////
      /// Multiplying with scalar must be able from both sides.
      /// \param tScalar Scaling magnitude.
      /// \param vVector Vector which is scaled.
      /// \returns Scaled vector.
      inline friend CVector3 operator*(TYPE tScalar, const CVector3 & vVect) 
      {
	return CVector3(vVect.m_aValues[0] * tScalar,
			vVect.m_aValues[1] * tScalar,
			vVect.m_aValues[2] * tScalar);
      }
      ////////////////////
      /// For more simplistic output (in debugging sense).
      /// \param stream std::ostream stream.
      /// \param vVector vector which values are printed to stream.
      /// \returns reference to std::ostream
      friend std::ostream& operator<<(std::ostream &stream, const CVector3 &vVector)
      {
	stream << vVector[0] << "," << vVector[1] << "," << vVector[2];
	return stream;
      }
#endif
    }; // CVector3

    /////////////////////////////////////////////////////////////////
    /// Template specializtion for floats; 
    /// comparisions require another approach than ==.
    /////////////////////////////////////////////////////////////////
    template<>
    class PHOENIX_API CVector3<float>
    {
    protected:
      // The actual storage array for local values.
      float  m_aValues[3];
    public:
      
      ////////////////////
      /// The default constructor.
      CVector3()
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param x x-coordinate value
      /// \param y y-coordinate value
      /// \param z z-coordinate value
      CVector3( float x, float y, float z)
      {
	m_aValues[0] = x;
	m_aValues[1] = y;
	m_aValues[2] = z;
      }
      ////////////////////
      /// The copy constructor.
      CVector3( const CVector3 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
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
	m_aValues[2] = pArray[2];
      }
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
      inline void operator=(const CVector3 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
      }
#ifndef SWIG
      ////////////////////
      /// The equality comparison operator.
      /// \returns truth value - non-zero if each corresponding component in 
      /// both vectors have the same value, zero otherwise.
      inline int operator==(const CVector3 & vVector) const
      {
	return ( QUITE_CLOSE_TO(m_aValues[0], vVector.m_aValues[0]) &&
		 QUITE_CLOSE_TO(m_aValues[1], vVector.m_aValues[1]) &&
		 QUITE_CLOSE_TO(m_aValues[2], vVector.m_aValues[2]) );
      }
      ////////////////////
      /// The inequality comparison operator.
      /// \returns truth value - non-zero if one or more components in 
      /// both vectors does not have same value, zero otherwise.
      inline int operator!=(const CVector3 & vVector) const
      {
	return (m_aValues[0] != vVector.m_aValues[0] ||
		m_aValues[1] != vVector.m_aValues[1] ||
		m_aValues[2] != vVector.m_aValues[2] );
      }
#endif
      ////////////////////
      /// Calculates the dot product.
      inline float Dot( const CVector3 & vVector ) const
      {
	return ( (m_aValues[0] * vVector.m_aValues[0]) + 
		 (m_aValues[1] * vVector.m_aValues[1]) + 
		 (m_aValues[2] * vVector.m_aValues[2]) );
      }
      ////////////////////
      /// Returns the length of this vector. 
      inline float Length() const
      {
	return sqrtf( m_aValues[0]*m_aValues[0] +
		      m_aValues[1]*m_aValues[1] +
		      m_aValues[2]*m_aValues[2] );
      }
      ////////////////////
      /// Returns the squared length of this vector.
      /// \return this dot this.
      inline float LengthSqr() const
      {
	return m_aValues[0]*m_aValues[0] + 
	m_aValues[1]*m_aValues[1] + 
	m_aValues[2]*m_aValues[2] ;
      }
      ////////////////////
      /// Returns the result of cross product between this vector and vVector.
      /// \returns CVector3 cross product.
      inline CVector3 Cross( const CVector3 & vVector ) const
      {
	return CVector3(   (m_aValues[1] * vVector[2]) - (m_aValues[2] * vVector[1]),
			   -((m_aValues[0] * vVector[2]) - (m_aValues[2] * vVector[0])),
			   (m_aValues[0] * vVector[1]) - (m_aValues[1] * vVector[0]));
	
      }
      ////////////////////
      /// Converts this into unit vector, ie. |this| = 1.0.
      inline void Normalize()
      {
	float t1DivLength = 1.0f / Length();
	m_aValues[0] *= t1DivLength;
	m_aValues[1] *= t1DivLength;
	m_aValues[2] *= t1DivLength;
      }
      ////////////////////
      /// Returns normalized vector of this.
      /// \param CVector3 normalized vector of this.
      inline CVector3 GetNormalized() const
      {
	float t1DivLength = 1.0f / Length();
	return CVector3( m_aValues[0] * t1DivLength,
			 m_aValues[1] * t1DivLength,
			 m_aValues[2] * t1DivLength );
	
      }
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector3 operator*(float tScalar) const
      {
	return CVector3(m_aValues[0] * tScalar,
			m_aValues[1] * tScalar,
			m_aValues[2] * tScalar);
      }
        inline CVector3 operator*( const CVector3 & vVec ) const
        {
            return CVector3( m_aValues[0]*vVec[0],
                            m_aValues[1]*vVec[1],
                            m_aValues[2]*vVec[2] );
            
        }
#if !defined(SWIG)
        inline void operator*=( const CVector3 & vVec )
        {
            m_aValues[0]*=vVec[0];
            m_aValues[1]*=vVec[1];
            m_aValues[2]*=vVec[2];
        }
#endif
      ////////////////////
      /// The division operator.
      /// \param tDivider division value.
      /// \returns Divided vector.
      inline CVector3 operator/(float tDivider) const
      {
    	  return CVector3(m_aValues[0] / tDivider,
    			  m_aValues[1] / tDivider,
    			  m_aValues[2] / tDivider);
      }
      ////////////////////
      /// The minus prefix operation.
      /// \return Negated vector.
      inline CVector3 operator-() const
      {
	return CVector3(-m_aValues[0],-m_aValues[1],-m_aValues[2]);
      }
      ////////////////////
      /// The minus operation.
      /// \param vVector Vector to be subtracted from this.
      /// \return Resulting vector.
      inline CVector3 operator-(const CVector3 & vVector) const
      {
	return CVector3( m_aValues[0]-vVector[0],
			 m_aValues[1]-vVector[1],
			 m_aValues[2]-vVector[2] );
      }
      ////////////////////
      /// Addition operator.
      /// \param vVector vector to be added.
      /// \returns sum vector.
      inline CVector3 operator+( const CVector3 & vVector) const
      {
	return CVector3(m_aValues[0] + vVector[0],
			m_aValues[1] + vVector[1],
			m_aValues[2] + vVector[2]);
      }
#ifndef SWIG
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline void operator*=(float tScalar)
      {
	m_aValues[0] *= tScalar;
	m_aValues[1] *= tScalar;
	m_aValues[2] *= tScalar;
      }
      ////////////////////
      /// Sum with assigning.
      /// \param vVector vector to be added.
      inline void operator+=(const CVector3 & vVector)
      {
	m_aValues[0] += vVector.m_aValues[0];
	m_aValues[1] += vVector.m_aValues[1];
	m_aValues[2] += vVector.m_aValues[2];
      }
      ////////////////////
      /// Subtraction with assigning.
      /// \param vVector Vector which is subtracted.
      inline void operator-=(const CVector3 & vVector)
      {
	m_aValues[0] -= vVector.m_aValues[0];
	m_aValues[1] -= vVector.m_aValues[1];
	m_aValues[2] -= vVector.m_aValues[2];
      }
      ////////////////////
      /// Divides vector with fDivider.
      /// \param tDivider Divider value.
      inline void operator/=(float tDivider)
      {
	float t1DivValue = 1.0f / tDivider;
	m_aValues[0] *= t1DivValue;
	m_aValues[1] *= t1DivValue;
	m_aValues[2] *= t1DivValue;
      }
      ////////////////////
      /// Multiplying with scalar must be able from both sides.
      /// \param tScalar Scaling magnitude.
      /// \param vVector Vector which is scaled.
      /// \returns Scaled vector.
      inline friend CVector3 operator*(float tScalar, const CVector3 & vVect) 
      {
	return CVector3(vVect.m_aValues[0] * tScalar,
			vVect.m_aValues[1] * tScalar,
			vVect.m_aValues[2] * tScalar);
      }
      ////////////////////
      /// For more simplistic output (in debugging sense).
      /// \param stream std::ostream stream.
      /// \param vVector vector which values are printed to stream.
      /// \returns reference to std::ostream
      friend std::ostream& operator<<(std::ostream &stream, const CVector3 &vVector)
      {
	stream << vVector[0] << "," << vVector[1] << "," << vVector[2];
	return stream;
      }
#endif
    }; // CVector3<float>
  } // namespace Math
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
