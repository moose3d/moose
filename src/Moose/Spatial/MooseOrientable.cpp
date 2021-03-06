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
#include "MooseOrientable.h"
#include "MooseMath.h"
/////////////////////////////////////////////////////////////////
using namespace Moose::Math;
using namespace Moose::Spatial;
/////////////////////////////////////////////////////////////////
Moose::Spatial::COrientable::COrientable() : m_vRight( 1.0f,0.0f,0.0f),
					       m_vUpward(0.0f,1.0f,0.0f),
					       m_vForward(0.0f,0.0f,-1.0f),
					       m_qRotation(0.0f,0.0f,0.0f,1.0f)
{
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::Reset()
{
  m_vForward[0] = 0.0f;
  m_vForward[1] = 0.0f;
  m_vForward[2] = -1.0f;

  m_vRight[0] = 1.0f;
  m_vRight[1] = 0.0f;
  m_vRight[2] = 0.0f;

  m_vUpward[0] = 0.0f;
  m_vUpward[1] = 1.0f;
  m_vUpward[2] = 0.0f;
  
  m_qRotation.Identity();
  
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateAroundX( float degrees ) 
{

  CQuaternion q; 
  q.CreateFromAxisAngle( 1.0, 0.0, 0.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);
  
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateAroundY( float degrees ) 
{

  CQuaternion q;
  q.CreateFromAxisAngle( 0.0, 1.0, 0.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateAroundZ( float degrees ) 
{

  CQuaternion q; q.CreateFromAxisAngle( 0.0f, 0.0f, 1.0f, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);

}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::Rotate( const CVector3<float> &vAngles )
{
  Rotate( vAngles[0], vAngles[1], vAngles[2]);

}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::Rotate( float fXAngle, float fYAngle, float fZAngle )
{

  // get rotations
  CQuaternion qOne; qOne.CreateFromAxisAngle( 1.0, 0.0, 0.0, fXAngle );
  CQuaternion qTwo; qTwo.CreateFromAxisAngle( 0.0, 1.0, 0.0, fYAngle );
  qOne = qTwo * qOne;
  qTwo.CreateFromAxisAngle( 0.0, 0.0, 1.0, fZAngle );

  qOne = qTwo * qOne;
  
  RotateAllDirections(qOne);
  m_qRotation = qOne * m_qRotation;
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateAroundRight( float fDegrees ) 
{

  /* get rotations using local m_vRight */
  CQuaternion q; 
  q.CreateFromAxisAngle( m_vRight[0], 
			 m_vRight[1], 
			 m_vRight[2], 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;
  RotateVector(q, m_vForward);
  RotateVector(q, m_vUpward);
    m_vForward.Normalize();
    m_vUpward.Normalize();
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateAroundUp( float fDegrees ) 
{

  /* get rotations using local m_vUpward */
  CQuaternion q;
  q.CreateFromAxisAngle( m_vUpward[0], 
			 m_vUpward[1], 
			 m_vUpward[2], 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;  
  RotateVector(q,m_vRight);
  RotateVector(q,m_vForward);
    m_vRight.Normalize();
    m_vForward.Normalize();
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateAroundForward( float fDegrees ) 
{
  
  /* get rotations using local m_vForward */
  CQuaternion q; q.CreateFromAxisAngle( m_vForward[0], m_vForward[1],
					m_vForward[2], fDegrees );
  m_qRotation = q * m_qRotation;
  RotateVector(q,m_vRight);
  RotateVector(q,m_vUpward);
  SetRotationChanged(1);
    m_vRight.Normalize();
    m_vUpward.Normalize();
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateLocal( CVector3<float> &vAngles )
{
  RotateLocal( vAngles[0], vAngles[1], vAngles[2]);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::RotateLocal( float fRightAngle, float fUpAngle, float fForwardAngle )
{


  /* Get the Rotations from LOCAL axis */
  CQuaternion qOne; qOne.CreateFromAxisAngle( m_vRight[0],
					      m_vRight[1],
					      m_vRight[2], 
					      fRightAngle );
  
  CQuaternion qTwo; qTwo.CreateFromAxisAngle( m_vUpward[0], 
					      m_vUpward[1],
					      m_vUpward[2],
					      fUpAngle );
  qOne = qTwo * qOne;
  qTwo.CreateFromAxisAngle( m_vForward[0], 
			    m_vForward[1],
			    m_vForward[2],
			    fForwardAngle );
  qOne = qTwo * qOne;
  /* Apply the combined rotation to each vector */
  RotateAllDirections(qOne);

  m_qRotation = qOne * m_qRotation;
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::SetDirectionForwardUp( CVector3<float> f, CVector3<float> u )
{
  Reset();

  m_vForward = f;
  m_vUpward = u;
  m_vRight = m_vForward.Cross(m_vUpward);
  m_vRight.Normalize();
    m_vForward.Normalize();
    m_vRight.Normalize();
  // To ensure numerical stability
  CVector3<float> vDiff = f - u;
  if ( vDiff[0] < 0.0001f ||
       vDiff[1] < 0.0001f ||
       vDiff[2] < 0.0001f )
  {
    m_qRotation.CreateFromAxisAngle( m_vRight, 180.0f);
  }
  else 
  {
    Moose::Math::RotationArc( m_vForward, f, m_qRotation );
  }
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void			
Moose::Spatial::COrientable::SetRotationFromInitialState( const CVector3<float> &vForward,
							    const CVector3<float> &vRight,
							    const CVector3<float> &vUp,
							    const CQuaternion & qQuat)
{
  m_qRotation = qQuat;
  m_vRight   = vRight;
  m_vForward = vForward;
  m_vUpward  = vUp;

  RotateVector(qQuat,m_vRight);
  RotateVector(qQuat,m_vForward);
  RotateVector(qQuat,m_vUpward);
  
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::AppendToRotation( const CQuaternion &qQuaternion)
{
  /* rotate every orientation vector by new quaternion*/
  RotateVector(qQuaternion,m_vForward);
  RotateVector(qQuaternion,m_vUpward);
  RotateVector(qQuaternion,m_vRight );

  m_qRotation = qQuaternion * m_qRotation;
  SetRotationChanged(1);
    m_vForward.Normalize();
    m_vUpward.Normalize();
    m_vRight.Normalize();
  
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::SetRotation( const CMatrix4x4<float> &mMatrix )
{
  // Restore initial state
  Reset();
  // Get quaternion and rotate vectors
  RotationMatrixToQuaternion(mMatrix, m_qRotation);
  RotateAllDirections(m_qRotation);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Moose::Spatial::COrientable::SetRotation( const CQuaternion &qRotation )
{
  // Restore initial state
  Reset();
  // Get quaternion and rotate vectors
  m_qRotation = qRotation;
  RotateAllDirections(qRotation);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
