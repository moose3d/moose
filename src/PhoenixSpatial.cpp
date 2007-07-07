/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
//  Implementation for Orientable3 class
/////////////////////////////////////////////////////////////////
#include "GSE_Orientable.h"
#include "GSE_Logger.h"
/////////////////////////////////////////////////////////////////
GSE_Orientable::GSE_Orientable() : m_vRight(1.0f,0.0f,0.0f),
				   m_vUpward(0.0f,1.0f,0.0f),
				   m_vForward(0.0f,0.0f,-1.0f),
				   m_qRotation(0.0f,0.0f,0.0f,1.0f)
{
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::Reset()
{
  m_vForward[GSE_Vector3::X] = 0.0;
  m_vForward[GSE_Vector3::Y] = 0.0;
  m_vForward[GSE_Vector3::Z] = -1.0;

  m_vRight[GSE_Vector3::X] = 1.0;
  m_vRight[GSE_Vector3::Y] = 0.0;
  m_vRight[GSE_Vector3::Z] = 0.0;

  m_vUpward[GSE_Vector3::X] = 0.0;
  m_vUpward[GSE_Vector3::Y] = 1.0;
  m_vUpward[GSE_Vector3::Z] = 0.0;
  
  // Unit quaternion
  m_qRotation[GSE_Quaternion::X ] =
  m_qRotation[GSE_Quaternion::Y ] =
  m_qRotation[GSE_Quaternion::Z ] = 0.0f;
  m_qRotation[GSE_Quaternion::W ] = 1.0f;
  
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateAroundX( float degrees ) 
{

  GSE_Quaternion q; 
  q.CreateFromAxisAngle( 1.0, 0.0, 0.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);
  
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateAroundY( float degrees ) 
{

  GSE_Quaternion q;
  q.CreateFromAxisAngle( 0.0, 1.0, 0.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateAroundZ( float degrees ) 
{

  GSE_Quaternion q; q.CreateFromAxisAngle( 0.0, 0.0, 1.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);

}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::Rotate( GSE_Vector3 &vAngles )
{
  Rotate( vAngles[GSE_Vector3::X],  
	  vAngles[GSE_Vector3::Y], 
	  vAngles[GSE_Vector3::Z]);

}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::Rotate( float fXAngle, float fYAngle, float fZAngle )
{

  // get rotations
  GSE_Quaternion qOne; qOne.CreateFromAxisAngle( 1.0, 0.0, 0.0, fXAngle );
  GSE_Quaternion qTwo; qTwo.CreateFromAxisAngle( 0.0, 1.0, 0.0, fYAngle );
  qOne = qTwo * qOne;
  qTwo.CreateFromAxisAngle( 0.0, 0.0, 1.0, fZAngle );

  qOne = qTwo * qOne;
  
  RotateAllDirections(qOne);
  m_qRotation = qOne * m_qRotation;
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateAroundRight( float fDegrees ) 
{

  /* get rotations using local m_vRight */
  GSE_Quaternion q; 
  q.CreateFromAxisAngle( m_vRight[GSE_Vector3::X], 
			 m_vRight[GSE_Vector3::Y], 
			 m_vRight[GSE_Vector3::Z], 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;
  m_vForward.Rotate(q);
  m_vUpward.Rotate(q);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateAroundUp( float fDegrees ) 
{

  /* get rotations using local m_vUpward */
  GSE_Quaternion q;
  q.CreateFromAxisAngle( m_vUpward[GSE_Vector3::X], 
			 m_vUpward[GSE_Vector3::Y], 
			 m_vUpward[GSE_Vector3::Z], 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;  
  m_vRight.Rotate(q);
  m_vForward.Rotate(q);

  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateAroundForward( float fDegrees ) 
{
  
  /* get rotations using local m_vForward */
  GSE_Quaternion q; q.CreateFromAxisAngle( m_vForward[GSE_Vector3::X], 
					   m_vForward[GSE_Vector3::Y], 
					   m_vForward[GSE_Vector3::Z], 
					   fDegrees );
  m_qRotation = q * m_qRotation;
  m_vRight.Rotate(q);
  m_vUpward.Rotate(q);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateLocal( GSE_Vector3 &vAngles )
{
  RotateLocal( vAngles[GSE_Vector3::X], 
	       vAngles[GSE_Vector3::Y], 
	       vAngles[GSE_Vector3::Z]);
  
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::RotateLocal( float fRightAngle, float fUpAngle, float fForwardAngle )
{


  /* Get the Rotations from LOCAL axis */
  GSE_Quaternion qOne; qOne.CreateFromAxisAngle( m_vRight[GSE_Vector3::X],
						 m_vRight[GSE_Vector3::Y],
						 m_vRight[GSE_Vector3::Z], 
						 fRightAngle );

  GSE_Quaternion qTwo; qTwo.CreateFromAxisAngle( m_vUpward[GSE_Vector3::X], 
						 m_vUpward[GSE_Vector3::Y],
						 m_vUpward[GSE_Vector3::Z],
						 fUpAngle );
  qOne = qTwo * qOne;
  qTwo.CreateFromAxisAngle( m_vForward[GSE_Vector3::X], 
			    m_vForward[GSE_Vector3::Y],
			    m_vForward[GSE_Vector3::Z],
			    fForwardAngle );
  qOne = qTwo * qOne;
  /* Apply the combined rotation to each vector */
  RotateAllDirections(qOne);

  m_qRotation = qOne * m_qRotation;
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
const GSE_Vector3 &
GSE_Orientable::GetRightVector() const
{
  return m_vRight;
}
/////////////////////////////////////////////////////////////////
const GSE_Vector3 &
GSE_Orientable::GetUpVector() const
{
  return m_vUpward;
}
/////////////////////////////////////////////////////////////////
const GSE_Vector3 &
GSE_Orientable::GetForwardVector() const
{
  return m_vForward;
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::SetDirectionForwardUp( GSE_Vector3 f, GSE_Vector3 u )
{
  Reset();

  m_vForward = f;
  m_vUpward = u;
  m_vRight = m_vForward.Cross(m_vUpward);
  m_vRight.ToUnit();
  // To ensure numerical stability
  GSE_Vector3 vDiff = f - u;
  if ( vDiff[GSE_Vector3::X] < 0.0001f &&
       vDiff[GSE_Vector3::Y] < 0.0001f &&
       vDiff[GSE_Vector3::Z] < 0.0001f )
  {
    m_qRotation.CreateFromAxisAngle( m_vRight, 180.0f);
  }
  else 
  {
    m_qRotation = GSE_Quaternion::RotationArc( m_vForward, f);
  }
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
GSE_Quaternion
GSE_Orientable::GetRotationQuaternion()
{
  return m_qRotation;
}
/////////////////////////////////////////////////////////////////
void			
GSE_Orientable::SetRotationFromInitialState( GSE_Vector3 &vForward,
					     GSE_Vector3 &vRight,
					     GSE_Vector3 &vUp,
					     GSE_Quaternion & qQuat)
{
  
  m_qRotation = qQuat;
  m_vRight   = vRight;
  m_vForward = vForward;
  m_vUpward  = vUp;

  m_vRight.Rotate(qQuat);
  m_vForward.Rotate(qQuat);
  m_vUpward.Rotate(qQuat);
  
  SetRotationChanged(1);
  
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::AppendToRotation( const GSE_Quaternion &qQuaternion)
{
  /* rotate every orientation vector by new quaternion*/
  m_vForward.Rotate(qQuaternion);
  m_vUpward.Rotate(qQuaternion);
  m_vRight.Rotate(qQuaternion);

  m_qRotation = qQuaternion * m_qRotation;
  SetRotationChanged(1);

}
/////////////////////////////////////////////////////////////////
int
GSE_Orientable::IsRotationChanged() const
{
  return m_bRotationChanged;
}
/////////////////////////////////////////////////////////////////
void
GSE_Orientable::SetRotationChanged( int bFlag )
{
  m_bRotationChanged = bFlag;
}
void
GSE_Orientable::SetRotation( const GSE_Matrix4x4f &mMatrix )
{
  // Restore initial state
  Reset();
  // Get quaternion and rotate vectors
  m_qRotation = Math::RotationMatrixToQuaternion(mMatrix);
  RotateAllDirections(m_qRotation);
  SetRotationChanged(1);
}
void
GSE_Orientable::SetRotation( const GSE_Quaternion &qRotation )
{
  // Restore initial state
  Reset();
  // Get quaternion and rotate vectors
  m_qRotation = qRotation;
  RotateAllDirections(qRotation);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
//  $Log: GSE_Orientable.cpp,v $
//  Revision 1.11  2007/05/18 08:08:09  entity
//  Vector3 constructor check & fixes
//
//  Revision 1.10  2007/03/26 17:46:53  entity
//  major cleanups and fixes
//
//  Revision 1.9  2007/03/26 12:54:06  entity
//  const & stuff
//
//  Revision 1.8  2007/03/21 08:24:19  entity
//  Get*Vector() is now const
//
//  Revision 1.7  2007/03/20 20:40:42  entity
//  const & stuff added
//
//  Revision 1.6  2007/03/15 18:25:07  entity
//  Fixed the badbadbadbad Inverse() quaternion thingy
//
//  Revision 1.5  2007/03/15 14:04:18  entity
//  SetRotation() added
//
//  Revision 1.4  2007/03/14 10:56:22  entity
//  added IsRotationChanged()
//
//  Revision 1.3  2007/01/08 12:24:00  entity
//  added GPL licence
//
//  Revision 1.2  2006/10/10 07:52:57  entity
//  some changes
/////////////////////////////////////////////////////////////////
