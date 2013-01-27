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

#ifndef __MooseQuaternion_h__
#define __MooseQuaternion_h__
/////////////////////////////////////////////////////////////////
#include "MooseMathUtils.h"
#include "MooseVector3.h"
#include "MooseVector4.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Math
  {
    ////////////////////
    /// Quaternion class.
    class MOOSE_API CQuaternion  : public Moose::Math::CVector4<float>
    {

    public:
      ////////////////////
      /// The default constructor.
      CQuaternion() 
      {
      }
      ////////////////////
      /// The parametrized constructor.
      CQuaternion( float fX, float fY, float fZ, float fW) : Moose::Math::CVector4<float>(fX,fY,fZ,fW)
      {
      }
      ////////////////////
      /// The deconstructor.
      ~CQuaternion()
      {
      }
      ////////////////////
      /// Creates quaternion from axis angle in degrees
      /// \param fX X-coordinate value
      /// \param fY Y-coordinate value
      /// \param fZ Z-coordinate value.
      /// \param fDegrees Rotation angle in degrees.
      inline void CreateFromAxisAngle( float fX, float fY, float fZ, float fDegrees)
      {
        CreateFromAxisAngleRad( fX, fY, fZ, Deg2Rad(fDegrees) );
      }
      ////////////////////
      /// Creates quaternion from axis angle in degrees
      /// \param vValues Vector representing axis.
      /// \param fDegrees Rotation angle in degrees.
      inline void CreateFromAxisAngle( const Moose::Math::CVector3<float> & vValues, float fDegrees )
      {
        CreateFromAxisAngleRad( vValues[0],  vValues[1], vValues[2], Deg2Rad(fDegrees) );
      }
      ////////////////////
      /// Creates quaternion from axis angle in radians
      /// \param vValues Vector representing axis.
      /// \param fRad Rotation angle in radians.
      inline void CreateFromAxisAngleRad ( const Moose::Math::CVector3<float> & vValues, float fRad )
      {
        CreateFromAxisAngleRad( vValues[0], vValues[1], vValues[2], fRad);
      }
      ////////////////////
      /// Creates quaternion from axis angle in radians
      /// \param fX X-coordinate value
      /// \param fY Y-coordinate value
      /// \param fZ Z-coordinate value.
      /// \param fDegrees Rotation angle in radians.
      void CreateFromAxisAngleRad ( float fX, float fY, float fZ, float fRad );
      ////////////////////
      /// Creates a quaternion from axis angles (degrees) with axis X=(1,0,0), Y=(0,1,0), Z=(0,0,1).
      /// \param vValues Vector with degree values for each axis.
      void CreateFromAxisAngles( const Moose::Math::CVector3<float> &vValues );
      ////////////////////
      /// Creates a quaternion from axis angles (radians) with axis X=(1,0,0), Y=(0,1,0), Z=(0,0,1);
      /// \param vValues Vector with radian values for each axis.
      void CreateFromAxisAnglesRad( const Moose::Math::CVector3<float> &vValues );
      ////////////////////
      /// The * Operator to enable multiplying Quaternions,
      ///  q1*q2 means first rotating by q2 and then by q1
      /// \param qQuat quaternion which will be multiplied with this.
      /// \return resulting quaternion.
      CQuaternion operator * ( const CQuaternion & qQuat ) const;
      inline CQuaternion operator+( const CQuaternion & qQuat ) const 
      {
        return CQuaternion( m_aValues[0] + qQuat[0],
                            m_aValues[1] + qQuat[1],
                            m_aValues[2] + qQuat[2],
                            m_aValues[3] + qQuat[3]);
      }
      inline CQuaternion operator-( const CQuaternion & qQuat ) const 
      {
        return CQuaternion( m_aValues[0] - qQuat[0],
                            m_aValues[1] - qQuat[1],
                            m_aValues[2] - qQuat[2],
                            m_aValues[3] - qQuat[3]);
      }
      inline CQuaternion operator-() const 
      {
        return CQuaternion( -m_aValues[0],
                            -m_aValues[1],
                            -m_aValues[2],
                            -m_aValues[3]);
      }
#ifndef SWIG
      ////////////////////
      /// So the scaling can be done both ways, from left or right.
      /// \param fValue Scaling magnitude.
      /// \param qQuat Quaterion to be scaled.
      /// \returns Scaled quaternion.
      inline friend CQuaternion operator*( float fValue, const CQuaternion & qQuat )
      {
        return CQuaternion(qQuat[0] * fValue,
                           qQuat[1] * fValue,
                           qQuat[2] * fValue,
                           qQuat[3] * fValue);
      }    
#endif
      ////////////////////
      /// Returns the magnitude of the Quaternion.
      /// \return square root of sum of squares of each component.
      inline float Magnitude()
      {
        return Length();
      }
      ////////////////////
      /// Calculates the Quaternion Conjugate of unit quaternion ie. Inverse.
      inline CQuaternion GetInverse() const
      {
        float f1DivLen = 1.0f/this->Length();
        return CQuaternion( -(m_aValues[0]*f1DivLen), 
                            -(m_aValues[1]*f1DivLen),
                            -(m_aValues[2]*f1DivLen),
                            (m_aValues[3]*f1DivLen) );
      }
      ////////////////////
      /// Calculates the Quaternion Conjugate of quaternion.
      inline CQuaternion GetConjugate() const
      {
	
        return CQuaternion( -(m_aValues[0]), 
                            -(m_aValues[1]),
                            -(m_aValues[2]),
                            (m_aValues[3]) );
      }
      ////////////////////
      /// Reverses the quaternion (in rotation sense).
      inline void Reverse()
      {
        m_aValues[3] = -m_aValues[3];
      }
      ////////////////////
      /// Reverses the quaternion (in rotation sense).
      /// \returns Reversed quaterinon.
      inline CQuaternion GetReverse() const
      {
        return CQuaternion(m_aValues[0], m_aValues[1], m_aValues[2], -m_aValues[3]);
      }
      ////////////////////
      /// Sets identity quaternion (in multiplication sense).
      inline void Identity() 
      {
        m_aValues[0] = 0.0f;
        m_aValues[1] = 0.0f;
        m_aValues[2] = 0.0f;
        m_aValues[3] = 1.0f;
      }
#ifndef SWIG
      ////////////////////
      /// For simplistic debug outputting.
      /// \param stream ostream where values are printed.
      /// \param qQuat quaternion which values are printed.
      /// \returns a reference to ostream.
      friend std::ostream& operator<<(std::ostream &stream, const CQuaternion & qQuat);
#endif
      ////////////////////
      /// The assignment operator.
      inline void operator=( CQuaternion qQuat)
      {
        m_aValues[0] = qQuat.m_aValues[0];
        m_aValues[1] = qQuat.m_aValues[1];
        m_aValues[2] = qQuat.m_aValues[2];
        m_aValues[3] = qQuat.m_aValues[3];
      }
      
      static inline CQuaternion AxisAngle( const Moose::Math::CVector3<float> & vAxis, float fDegrees )
      {
        return AxisAngle( vAxis[0], vAxis[1], vAxis[2], fDegrees);
      }
      static inline CQuaternion AxisAngleRad( const Moose::Math::CVector3<float> & vAxis, float fRad )
      {
        return AxisAngleRad( vAxis[0], vAxis[1], vAxis[2], fRad);
      }
      static inline CQuaternion AxisAngles( const Moose::Math::CVector3<float> & vAxisAngles )
      {
        return AxisAngles( vAxisAngles[0], vAxisAngles[1], vAxisAngles[2]);
      }
      static inline CQuaternion AxisAnglesRad( const Moose::Math::CVector3<float> & vAxisAngles )
      { 
        return AxisAnglesRad(vAxisAngles[0], vAxisAngles[1], vAxisAngles[2]);
      }
      static inline CQuaternion AxisAngle( float fX, float fY, float fZ, float fDegrees )
      {
        return AxisAngleRad( fX,fY,fZ, Deg2Rad(fDegrees));
      }
      static inline CQuaternion AxisAngles( float fX, float fY, float fZ )
      {
        return AxisAnglesRad( Deg2Rad(fX), Deg2Rad(fY), Deg2Rad(fZ));
      }
      static CQuaternion AxisAngleRad( float fX, float fY, float fZ, float fRad );
      static CQuaternion AxisAnglesRad( float fX, float fY, float fZ );
    };
    
  }; // namespace Math
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
