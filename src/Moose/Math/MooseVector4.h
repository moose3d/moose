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

/////////////////////////////////////////////////////////////////
#ifndef __MooseVector4_h__
#define __MooseVector4_h__
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
    class MOOSE_API CVector4
    {
    protected:
      // The actual storage array for local values.
      TYPE  m_aValues[4];
    public:
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
      /// The default constructor.
      CVector4()
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param x x-coordinate value
      /// \param y y-coordinate value
      /// \param z z-coordinate value
      /// \param w w-coordinate value
      CVector4( TYPE x, TYPE y, TYPE z, TYPE w)
      {
	m_aValues[0] = x;
	m_aValues[1] = y;
	m_aValues[2] = z;
	m_aValues[3] = w;
      }
      ////////////////////
      /// The copy constructor.
      CVector4( const CVector4 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
	m_aValues[3] = vVector.m_aValues[3];
      }
      ////////////////////
      /// The parametrized constructor copies data from an existing array.
      inline void Set( const TYPE *pArray )
      {
	m_aValues[0] = pArray[0];
	m_aValues[1] = pArray[1];
	m_aValues[2] = pArray[2];
	m_aValues[3] = pArray[3];
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
      /// Returns value of an element.
      /// \param nIndex Element index.
      /// \return value of the element.
      /* inline TYPE operator()( unsigned int nIndex ) const */
/*       { */
/* 	return m_aValues[nIndex]; */
/*       } */
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
      ////////////////////
      /// The assignment operator.
      inline CVector4 & operator=(CVector4 vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
	m_aValues[3] = vVector.m_aValues[3];
	return *this;
      }
#ifndef SWIG
      ////////////////////
      /// The comparison operator.
      /// \returns truth value - non-zero if corresponding each corresponding component in 
      /// both vectors have the same value, zero otherwise.
      inline int operator==(const CVector4 & vVector) const
      {
	return (m_aValues[0] == vVector.m_aValues[0] &&
		m_aValues[1] == vVector.m_aValues[1] &&
	        m_aValues[2] == vVector.m_aValues[2] && 
		m_aValues[3] == vVector.m_aValues[3]);
      }
      ////////////////////
      /// The inequality comparison operator.
      /// \returns truth value - non-zero if one or more components in 
      /// both vectors does not have same value, zero otherwise.
      inline int operator!=(const CVector4 & vVector) const
      {
	return (m_aValues[0] != vVector.m_aValues[0] ||
		m_aValues[1] != vVector.m_aValues[1] ||
	        m_aValues[2] != vVector.m_aValues[2] ||
		m_aValues[3] != vVector.m_aValues[3]);
      }
#endif
      ////////////////////
      /// Calculates the dot product.
      inline TYPE Dot( const CVector4 & vVector ) const
      {
	return ( (m_aValues[0] * vVector.m_aValues[0]) + 
		 (m_aValues[1] * vVector.m_aValues[1]) + 
		 (m_aValues[2] * vVector.m_aValues[2]) +
		 (m_aValues[3] * vVector.m_aValues[3]) );
      }
      ////////////////////
      /// Returns the length of this vector. 
      inline TYPE Length() const
      {
	return sqrt( m_aValues[0]*m_aValues[0] +
		     m_aValues[1]*m_aValues[1] +
		     m_aValues[2]*m_aValues[2] + 
		     m_aValues[3]*m_aValues[3] );
      }
      ////////////////////
      /// Returns the squared length of this vector.
      /// \return this dot this.
      inline TYPE LengthSqr() const
      {
	return m_aValues[0]*m_aValues[0] + 
	       m_aValues[1]*m_aValues[1] + 
	       m_aValues[2]*m_aValues[2] +
	       m_aValues[3]*m_aValues[3];
      }
      
      ////////////////////
      /// Converts this into unit vector, ie. |this| = 1.0.
      inline void Normalize()
      {
	TYPE t1DivLength = 1.0f / Length();
	m_aValues[0] *= t1DivLength;
	m_aValues[1] *= t1DivLength;
	m_aValues[2] *= t1DivLength;
	m_aValues[3] *= t1DivLength;
      }
      ////////////////////
      /// Returns normalized vector of this.
      /// \param CVector4 normalized vector of this.
      inline CVector4 GetNormalized() const
      {
	TYPE t1DivLength = 1.0f / Length();
	return CVector4( m_aValues[0] * t1DivLength,
			 m_aValues[1] * t1DivLength,
			 m_aValues[2] * t1DivLength,
			 m_aValues[3] * t1DivLength );
	
      }
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector4 operator*(TYPE tScalar) const
      {
	return CVector4(m_aValues[0] * tScalar,
			m_aValues[1] * tScalar,
			m_aValues[2] * tScalar,
			m_aValues[3] * tScalar);
      }
        inline CVector4 operator*( const CVector4 & vVec ) const
        {
            return CVector4( m_aValues[0]*vVec[0],
                            m_aValues[1]*vVec[1],
                            m_aValues[2]*vVec[2],
                            m_aValues[3]*vVec[3]);
            
        }
#if !defined(SWIG)
        inline void operator*=( const CVector4 & vVec )
        {
            m_aValues[0]*=vVec[0];
            m_aValues[1]*=vVec[1];
            m_aValues[2]*=vVec[2];
            m_aValues[3]*=vVec[3];
        }
#endif
      ////////////////////
      /// The division operator.
      /// \param tDivider division value.
      /// \returns Divided vector.
      inline CVector4 operator/(TYPE tDivider) const
      {
	return CVector4(m_aValues[0] / tDivider,
			m_aValues[1] / tDivider,
			m_aValues[2] / tDivider,
			m_aValues[3] / tDivider);
      }
      ////////////////////
      /// The negation operation.
      /// \return Negated vector.
      inline CVector4 operator-() const
      {
	return CVector4(-m_aValues[0],-m_aValues[1],-m_aValues[2],-m_aValues[3]);
	
      }
      ////////////////////
      /// The subtraction operation.
      /// \param vVector Vector to be subtracted from this.
      /// \return Resulting vector.
      inline CVector4 operator-(const CVector4 & vVector) const
      {
	return CVector4( m_aValues[0]-vVector[0],
			 m_aValues[1]-vVector[1],
			 m_aValues[2]-vVector[2],
			 m_aValues[3]-vVector[3]);
      }
      ////////////////////
      /// Addition operator.
      /// \param vVector vector to be added.
      /// \returns sum vector.
      inline CVector4 operator+( const CVector4 & vVector) const
      {
	return CVector4(m_aValues[0] + vVector[0],
			m_aValues[1] + vVector[1],
			m_aValues[2] + vVector[2],
			m_aValues[3] + vVector[3]);
      }
#ifndef SWIG
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector4 & operator*=(TYPE tScalar)
      {
	m_aValues[0] *= tScalar;
	m_aValues[1] *= tScalar;
	m_aValues[2] *= tScalar;
	m_aValues[3] *= tScalar;
	return *this;
      }
      ////////////////////
      /// Sum with assigning.
      /// \param vVector vector to be added.
      inline void operator+=(const CVector4 & vVector)
      {
	m_aValues[0] += vVector.m_aValues[0];
	m_aValues[1] += vVector.m_aValues[1];
	m_aValues[2] += vVector.m_aValues[2];
	m_aValues[3] += vVector.m_aValues[3];
      }
      ////////////////////
      /// Subtraction with assigning.
      /// \param vVector Vector which is subtracted.
      inline void operator-=(const CVector4 & vVector)
      {
	m_aValues[0] -= vVector.m_aValues[0];
	m_aValues[1] -= vVector.m_aValues[1];
	m_aValues[2] -= vVector.m_aValues[2];
	m_aValues[3] -= vVector.m_aValues[3];
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
	m_aValues[3] *= t1DivValue;
      }
      ////////////////////
      /// Multiplying with scalar must be able from both sides.
      /// \param tScalar Scaling magnitude.
      /// \param vVector Vector which is scaled.
      /// \returns Scaled vector.
      inline friend CVector4 operator*(TYPE tScalar, const CVector4 & vVect) 
      {
	return CVector4(vVect.m_aValues[0] * tScalar,
			vVect.m_aValues[1] * tScalar,
			vVect.m_aValues[2] * tScalar,
			vVect.m_aValues[3] * tScalar);
      }
      ////////////////////
      /// For more simplistic output (in debugging sense).
      /// \param stream std::ostream stream.
      /// \param vVector vector which values are printed to stream.
      /// \returns reference to std::ostream
      friend std::ostream& operator<<(std::ostream &stream, const CVector4 &vVector)
      {
	stream << vVector[0] << "," << vVector[1] << "," << vVector[2] << "," << vVector[3];
	return stream;
      }
#endif
    }; // CVector4
    /////////////////////////////////////////////////////////////////
    /// specialized template
    template <>
    class MOOSE_API CVector4<float>
    {
    protected:
      // The actual storage array for local values.
      float  m_aValues[4];
    public:
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
      /// The default constructor.
      CVector4() 
      {
      }
      ////////////////////
      /// Parametrized constructor.
      /// \param x x-coordinate value
      /// \param y y-coordinate value
      /// \param z z-coordinate value
      /// \param w w-coordinate value
      CVector4( float x, float y, float z, float w )
      {
	m_aValues[0] = x;
	m_aValues[1] = y;
	m_aValues[2] = z;
	m_aValues[3] = w;
      }
      ////////////////////
      /// The copy constructor.
      CVector4( const CVector4 & vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
	m_aValues[3] = vVector.m_aValues[3];
      }
      ////////////////////
      /// The parametrized constructor copies data from an existing array.
      inline void Set( const float *pArray )
      {
	m_aValues[0] = pArray[0];
	m_aValues[1] = pArray[1];
	m_aValues[2] = pArray[2];
	m_aValues[3] = pArray[3];
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
      ////////////////////
      /// The assignment operator.
      inline CVector4 & operator=(CVector4 vVector)
      {
	m_aValues[0] = vVector.m_aValues[0];
	m_aValues[1] = vVector.m_aValues[1];
	m_aValues[2] = vVector.m_aValues[2];
	m_aValues[3] = vVector.m_aValues[3];
	return *this;
      }
#ifndef SWIG
      ////////////////////
      /// The comparison operator.
      /// \returns truth value - non-zero if corresponding each corresponding component in 
      /// both vectors have the same value, zero otherwise.
      inline int operator==(const CVector4 & vVector) const
      {
	return (QUITE_CLOSE_TO(m_aValues[0], vVector.m_aValues[0]) &&
		QUITE_CLOSE_TO(m_aValues[1], vVector.m_aValues[1]) &&
		QUITE_CLOSE_TO(m_aValues[2], vVector.m_aValues[2]) && 
		QUITE_CLOSE_TO(m_aValues[3], vVector.m_aValues[3]) );
      }

      ////////////////////
      /// The inequality comparison operator.
      /// \returns truth value - non-zero if one or more components in 
      /// both vectors does not have same value, zero otherwise.
      inline int operator!=(const CVector4 & vVector) const
      {
	return (m_aValues[0] != vVector.m_aValues[0] ||
		m_aValues[1] != vVector.m_aValues[1] ||
	        m_aValues[2] != vVector.m_aValues[2] ||
		m_aValues[3] != vVector.m_aValues[3]);
      }
#endif
      ////////////////////
      /// Calculates the dot product.
      inline float Dot( const CVector4 & vVector ) const
      {
	return ( (m_aValues[0] * vVector.m_aValues[0]) + 
		 (m_aValues[1] * vVector.m_aValues[1]) + 
		 (m_aValues[2] * vVector.m_aValues[2]) +
		 (m_aValues[3] * vVector.m_aValues[3]) );
      }
      ////////////////////
      /// Returns the length of this vector. 
      inline float Length() const
      {
	return sqrt( m_aValues[0]*m_aValues[0] +
		     m_aValues[1]*m_aValues[1] +
		     m_aValues[2]*m_aValues[2] + 
		     m_aValues[3]*m_aValues[3] );
      }
      ////////////////////
      /// Returns the squared length of this vector.
      /// \return this dot this.
      inline float LengthSqr() const
      {
	return m_aValues[0]*m_aValues[0] + 
	       m_aValues[1]*m_aValues[1] + 
	       m_aValues[2]*m_aValues[2] +
	       m_aValues[3]*m_aValues[3];
      }
      
      ////////////////////
      /// Converts this into unit vector, ie. |this| = 1.0.
      inline void Normalize()
      {
	float t1DivLength = 1.0f / Length();
	m_aValues[0] *= t1DivLength;
	m_aValues[1] *= t1DivLength;
	m_aValues[2] *= t1DivLength;
	m_aValues[3] *= t1DivLength;
      }
      ////////////////////
      /// Returns normalized vector of this.
      /// \param CVector4 normalized vector of this.
      inline CVector4 GetNormalized() const
      {
	float t1DivLength = 1.0f / Length();
	return CVector4( m_aValues[0] * t1DivLength,
			 m_aValues[1] * t1DivLength,
			 m_aValues[2] * t1DivLength,
			 m_aValues[3] * t1DivLength );
	
      }
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector4 operator*(float tScalar) const
      {
	return CVector4(m_aValues[0] * tScalar,
			m_aValues[1] * tScalar,
			m_aValues[2] * tScalar,
			m_aValues[3] * tScalar);
      }
        
        inline CVector4 operator*( const CVector4 & vVec ) const
        {
            return CVector4( m_aValues[0]*vVec[0],
                            m_aValues[1]*vVec[1],
                            m_aValues[2]*vVec[2],
                            m_aValues[3]*vVec[3]);
            
        }
#if !defined(SWIG)
        inline void operator*=( const CVector4 & vVec )
        {
            m_aValues[0]*=vVec[0];
            m_aValues[1]*=vVec[1];
            m_aValues[2]*=vVec[2];
            m_aValues[3]*=vVec[3];
        }
#endif        
        
      ////////////////////
      /// The division operator.
      /// \param tDivider division value.
      /// \returns Divided vector.
      inline CVector4 operator/(float tDivider) const
      {
	return CVector4(m_aValues[0] / tDivider,
			m_aValues[1] / tDivider,
			m_aValues[2] / tDivider,
			m_aValues[3] / tDivider);
      }
      ////////////////////
      /// The negation operation.
      /// \return Negated vector.
      inline CVector4 operator-() const
      {
	return CVector4(-m_aValues[0],-m_aValues[1],-m_aValues[2],-m_aValues[3]);
	
      }
      ////////////////////
      /// The subtraction operation.
      /// \param vVector Vector to be subtracted from this.
      /// \return Resulting vector.
      inline CVector4 operator-(const CVector4 & vVector) const
      {
	return CVector4( m_aValues[0]-vVector[0],
			 m_aValues[1]-vVector[1],
			 m_aValues[2]-vVector[2],
			 m_aValues[3]-vVector[3]);
      }
      ////////////////////
      /// Addition operator.
      /// \param vVector vector to be added.
      /// \returns sum vector.
      inline CVector4 operator+( const CVector4 & vVector) const
      {
	return CVector4(m_aValues[0] + vVector[0],
			m_aValues[1] + vVector[1],
			m_aValues[2] + vVector[2],
			m_aValues[3] + vVector[3]);
      }
#ifndef SWIG
      ////////////////////
      /// Scaling operation.
      /// \param tScalar scaling magnitude.
      inline CVector4 & operator*=(float tScalar)
      {
	m_aValues[0] *= tScalar;
	m_aValues[1] *= tScalar;
	m_aValues[2] *= tScalar;
	m_aValues[3] *= tScalar;
	return *this;
      }
      ////////////////////
      /// Sum with assigning.
      /// \param vVector vector to be added.
      inline void operator+=(const CVector4 & vVector)
      {
	m_aValues[0] += vVector.m_aValues[0];
	m_aValues[1] += vVector.m_aValues[1];
	m_aValues[2] += vVector.m_aValues[2];
	m_aValues[3] += vVector.m_aValues[3];
      }
      ////////////////////
      /// Subtraction with assigning.
      /// \param vVector Vector which is subtracted.
      inline void operator-=(const CVector4 & vVector)
      {
	m_aValues[0] -= vVector.m_aValues[0];
	m_aValues[1] -= vVector.m_aValues[1];
	m_aValues[2] -= vVector.m_aValues[2];
	m_aValues[3] -= vVector.m_aValues[3];
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
	m_aValues[3] *= t1DivValue;
      }
      ////////////////////
      /// Multiplying with scalar must be able from both sides.
      /// \param tScalar Scaling magnitude.
      /// \param vVector Vector which is scaled.
      /// \returns Scaled vector.
      inline friend CVector4 operator*(float tScalar, const CVector4 & vVect) 
      {
	return CVector4(vVect.m_aValues[0] * tScalar,
			vVect.m_aValues[1] * tScalar,
			vVect.m_aValues[2] * tScalar,
			vVect.m_aValues[3] * tScalar);
      }
      ////////////////////
      /// For more simplistic output (in debugging sense).
      /// \param stream std::ostream stream.
      /// \param vVector vector which values are printed to stream.
      /// \returns reference to std::ostream
      friend std::ostream& operator<<(std::ostream &stream, const CVector4 &vVector)
      {
	stream << vVector[0] << "," << vVector[1] << "," << vVector[2] << "," << vVector[3];
	return stream;
      }
#endif
    }; // CVector4<float>
  } // namespace Math
} // namespace Moose
#endif
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

