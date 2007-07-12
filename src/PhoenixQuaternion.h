#ifndef __PhoenixQuaternion_h__
#define __PhoenixQuaternion_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixMathUtils.h"
#include "PhoenixVector3.h"
#include "PhoenixVector4.h"
/////////////////////////////////////////////////////////////////
using Phoenix::Math::CVector3;
using Phoenix::Math::CVector4;
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Math
  {
    ////////////////////
    /// Quaternion class.
    class CQuaternion  : public CVector4<float>
    {

    public:
      ////////////////////
      /// The default constructor.
      CQuaternion() 
      {
      }
      ////////////////////
      /// The parametrized constructor.
      CQuaternion( float fX, float fY, float fZ, float fW) : CVector4<float>(fX,fY,fZ,fW)
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
      inline void CreateFromAxisAngle( const CVector3<float> & vValues, float fDegrees )
      {
	CreateFromAxisAngleRad( vValues(0),  vValues(1), vValues(2), Deg2Rad(fDegrees) );
      }
      ////////////////////
      /// Creates quaternion from axis angle in radians
      /// \param vValues Vector representing axis.
      /// \param fRad Rotation angle in radians.
      inline void CreateFromAxisAngleRad ( const CVector3<float> & vValues, float fRad )
      {
	CreateFromAxisAngleRad( vValues(0), vValues(1), vValues(2), fRad);
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
      void CreateFromAxisAngles( const CVector3<float> &vValues );
      ////////////////////
      /// Creates a quaternion from axis angles (radians) with axis X=(1,0,0), Y=(0,1,0), Z=(0,0,1);
      /// \param vValues Vector with radian values for each axis.
      void CreateFromAxisAnglesRad( const CVector3<float> &vValues );
      ////////////////////
      /// The * Operator to enable multiplying Quaternions,
      ///  q1*q2 means first rotating by q2 and then by q1
      /// \param qQuat quaternion which will be multiplied with this.
      /// \return resulting quaternion.
      CQuaternion operator * ( const CQuaternion & qQuat ) const;
      inline CQuaternion operator+( const CQuaternion & qQuat ) const 
      {
	return CQuaternion( m_pValues[0] + qQuat(0),
			    m_pValues[1] + qQuat(1),
			    m_pValues[2] + qQuat(2),
			    m_pValues[3] + qQuat(3));
      }
      inline CQuaternion operator-( const CQuaternion & qQuat ) const 
      {
	return CQuaternion( m_pValues[0] - qQuat(0),
			    m_pValues[1] - qQuat(1),
			    m_pValues[2] - qQuat(2),
			    m_pValues[3] - qQuat(3));
      }
      inline CQuaternion operator-() const 
      {
	return CQuaternion( -m_pValues[0],
			    -m_pValues[1],
			    -m_pValues[2],
			    -m_pValues[3]);
      }
      ////////////////////
      /// So the scaling can be done both ways, from left or right.
      /// \param fValue Scaling magnitude.
      /// \param qQuat Quaterion to be scaled.
      /// \returns Scaled quaternion.
      inline friend CQuaternion operator*( float fValue, const CQuaternion & qQuat )
      {
	return CQuaternion(qQuat(0) * fValue,
			   qQuat(1) * fValue,
			   qQuat(2) * fValue,
			   qQuat(3) * fValue);
      }    
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
	return CQuaternion( -(m_pValues[0]*f1DivLen), 
			    -(m_pValues[1]*f1DivLen),
			    -(m_pValues[2]*f1DivLen),
			     (m_pValues[3]*f1DivLen) );
      }
      ////////////////////
      /// Calculates the Quaternion Conjugate of quaternion.
      inline CQuaternion GetConjugate() const
      {
	
	return CQuaternion( -(m_pValues[0]), 
			    -(m_pValues[1]),
			    -(m_pValues[2]),
			     (m_pValues[3]) );
      }
      ////////////////////
      /// Reverses the quaternion (in rotation sense).
      inline void Reverse()
      {
	m_pValues[3] = -m_pValues[3];
      }
      ////////////////////
      /// Reverses the quaternion (in rotation sense).
      /// \returns Reversed quaterinon.
      inline CQuaternion GetReverse() const
      {
	return CQuaternion(m_pValues[0], m_pValues[1], m_pValues[2], -m_pValues[3]);
      }
      ////////////////////
      /// Sets identity quaternion (in multiplication sense).
      inline void Identity() 
      {
	m_pValues[0] = 0.0f;
	m_pValues[1] = 0.0f;
	m_pValues[2] = 0.0f;
	m_pValues[3] = 1.0f;
      }
      ////////////////////
      /// For simplistic debug outputting.
      /// \param stream ostream where values are printed.
      /// \param qQuat quaternion which values are printed.
      /// \returns a reference to ostream.
      friend std::ostream& operator<<(std::ostream &stream, const CQuaternion & qQuat);
      
      ////////////////////
      /// The assignment operator.
      inline void operator=( CQuaternion qQuat)
      {
	m_pValues[0] = qQuat.m_pValues[0];
	m_pValues[1] = qQuat.m_pValues[1];
	m_pValues[2] = qQuat.m_pValues[2];
	m_pValues[3] = qQuat.m_pValues[3];
      }
    };
  }; // namespace Math
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
