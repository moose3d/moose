/////////////////////////////////////////////////////////////////
#include "PhoenixSpatial.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Math;
using namespace Phoenix::Spatial;
/////////////////////////////////////////////////////////////////
Phoenix::Spatial::COrientable::COrientable() : m_vRight(1.0f,0.0f,0.0f),
					       m_vUpward(0.0f,1.0f,0.0f),
					       m_vForward(0.0f,0.0f,-1.0f),
					       m_qRotation(0.0f,0.0f,0.0f,1.0f)
{
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::Reset()
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
Phoenix::Spatial::COrientable::RotateAroundX( float degrees ) 
{

  CQuaternion q; 
  q.CreateFromAxisAngle( 1.0, 0.0, 0.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);
  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateAroundY( float degrees ) 
{

  CQuaternion q;
  q.CreateFromAxisAngle( 0.0, 1.0, 0.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateAroundZ( float degrees ) 
{

  CQuaternion q; q.CreateFromAxisAngle( 0.0, 0.0, 1.0, degrees );
  m_qRotation = q * m_qRotation;
  RotateAllDirections( q );
  SetRotationChanged(1);

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::Rotate( const CVector3<float> &vAngles )
{
  Rotate( vAngles(0), vAngles(1), vAngles(2));

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::Rotate( float fXAngle, float fYAngle, float fZAngle )
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
Phoenix::Spatial::COrientable::RotateAroundRight( float fDegrees ) 
{

  /* get rotations using local m_vRight */
  CQuaternion q; 
  q.CreateFromAxisAngle( m_vRight(0), 
			 m_vRight(1), 
			 m_vRight(2), 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;
  RotateVector(q, m_vForward);
  RotateVector(q, m_vUpward);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateAroundUp( float fDegrees ) 
{

  /* get rotations using local m_vUpward */
  CQuaternion q;
  q.CreateFromAxisAngle( m_vUpward(0), 
			 m_vUpward(1), 
			 m_vUpward(2), 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;  
  RotateVector(q,m_vRight);
  RotateVector(q,m_vForward);

  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateAroundForward( float fDegrees ) 
{
  
  /* get rotations using local m_vForward */
  CQuaternion q; q.CreateFromAxisAngle( m_vForward(0), m_vForward(1),
					m_vForward(2), fDegrees );
  m_qRotation = q * m_qRotation;
  RotateVector(q,m_vRight);
  RotateVector(q,m_vUpward);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateLocal( CVector3<float> &vAngles )
{
  RotateLocal( vAngles(0), vAngles(1), vAngles(2));
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateLocal( float fRightAngle, float fUpAngle, float fForwardAngle )
{


  /* Get the Rotations from LOCAL axis */
  CQuaternion qOne; qOne.CreateFromAxisAngle( m_vRight(0),
					      m_vRight(1),
					      m_vRight(2), 
					      fRightAngle );
  
  CQuaternion qTwo; qTwo.CreateFromAxisAngle( m_vUpward(0), 
					      m_vUpward(1),
					      m_vUpward(2),
					      fUpAngle );
  qOne = qTwo * qOne;
  qTwo.CreateFromAxisAngle( m_vForward(0), 
			    m_vForward(1),
			    m_vForward(2),
			    fForwardAngle );
  qOne = qTwo * qOne;
  /* Apply the combined rotation to each vector */
  RotateAllDirections(qOne);

  m_qRotation = qOne * m_qRotation;
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::SetDirectionForwardUp( CVector3<float> f, CVector3<float> u )
{
  Reset();

  m_vForward = f;
  m_vUpward = u;
  m_vRight = m_vForward.Cross(m_vUpward);
  m_vRight.Normalize();
  // To ensure numerical stability
  CVector3<float> vDiff = f - u;
  if ( vDiff(0) < 0.0001f &&
       vDiff(1) < 0.0001f &&
       vDiff(2) < 0.0001f )
  {
    m_qRotation.CreateFromAxisAngle( m_vRight, 180.0f);
  }
  else 
  {
    m_qRotation = RotationArc( m_vForward, f);
  }
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void			
Phoenix::Spatial::COrientable::SetRotationFromInitialState( const CVector3<float> &vForward,
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
Phoenix::Spatial::COrientable::AppendToRotation( const CQuaternion &qQuaternion)
{
  /* rotate every orientation vector by new quaternion*/
  RotateVector(qQuaternion,m_vForward);
  RotateVector(qQuaternion,m_vUpward);
  RotateVector(qQuaternion,m_vRight );

  m_qRotation = qQuaternion * m_qRotation;
  SetRotationChanged(1);
  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::SetRotation( const CMatrix4x4<float> &mMatrix )
{
  // Restore initial state
  Reset();
  // Get quaternion and rotate vectors
  m_qRotation = RotationMatrixToQuaternion(mMatrix);
  RotateAllDirections(m_qRotation);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::SetRotation( const CQuaternion &qRotation )
{
  // Restore initial state
  Reset();
  // Get quaternion and rotate vectors
  m_qRotation = qRotation;
  RotateAllDirections(qRotation);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
