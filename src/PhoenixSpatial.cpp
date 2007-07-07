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
  m_vForward[CVector3::X] = 0.0;
  m_vForward[CVector3::Y] = 0.0;
  m_vForward[CVector3::Z] = -1.0;

  m_vRight[CVector3::X] = 1.0;
  m_vRight[CVector3::Y] = 0.0;
  m_vRight[CVector3::Z] = 0.0;

  m_vUpward[CVector3::X] = 0.0;
  m_vUpward[CVector3::Y] = 1.0;
  m_vUpward[CVector3::Z] = 0.0;
  
  // Unit quaternion
  m_qRotation[CQuaternion::X ] =
  m_qRotation[CQuaternion::Y ] =
  m_qRotation[CQuaternion::Z ] = 0.0f;
  m_qRotation[CQuaternion::W ] = 1.0f;
  
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
Phoenix::Spatial::COrientable::Rotate( CVector3 &vAngles )
{
  Rotate( vAngles[CVector3::X],  
	  vAngles[CVector3::Y], 
	  vAngles[CVector3::Z]);

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
  q.CreateFromAxisAngle( m_vRight[CVector3::X], 
			 m_vRight[CVector3::Y], 
			 m_vRight[CVector3::Z], 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;
  m_vForward.Rotate(q);
  m_vUpward.Rotate(q);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateAroundUp( float fDegrees ) 
{

  /* get rotations using local m_vUpward */
  CQuaternion q;
  q.CreateFromAxisAngle( m_vUpward[CVector3::X], 
			 m_vUpward[CVector3::Y], 
			 m_vUpward[CVector3::Z], 
			 fDegrees );
  
  m_qRotation = q * m_qRotation;  
  m_vRight.Rotate(q);
  m_vForward.Rotate(q);

  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateAroundForward( float fDegrees ) 
{
  
  /* get rotations using local m_vForward */
  CQuaternion q; q.CreateFromAxisAngle( m_vForward[CVector3::X], 
					   m_vForward[CVector3::Y], 
					   m_vForward[CVector3::Z], 
					   fDegrees );
  m_qRotation = q * m_qRotation;
  m_vRight.Rotate(q);
  m_vUpward.Rotate(q);
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateLocal( CVector3 &vAngles )
{
  RotateLocal( vAngles[CVector3::X], 
	       vAngles[CVector3::Y], 
	       vAngles[CVector3::Z]);
  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::RotateLocal( float fRightAngle, float fUpAngle, float fForwardAngle )
{


  /* Get the Rotations from LOCAL axis */
  CQuaternion qOne; qOne.CreateFromAxisAngle( m_vRight[CVector3::X],
						 m_vRight[CVector3::Y],
						 m_vRight[CVector3::Z], 
						 fRightAngle );

  CQuaternion qTwo; qTwo.CreateFromAxisAngle( m_vUpward[CVector3::X], 
						 m_vUpward[CVector3::Y],
						 m_vUpward[CVector3::Z],
						 fUpAngle );
  qOne = qTwo * qOne;
  qTwo.CreateFromAxisAngle( m_vForward[CVector3::X], 
			    m_vForward[CVector3::Y],
			    m_vForward[CVector3::Z],
			    fForwardAngle );
  qOne = qTwo * qOne;
  /* Apply the combined rotation to each vector */
  RotateAllDirections(qOne);

  m_qRotation = qOne * m_qRotation;
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::SetDirectionForwardUp( CVector3 f, CVector3 u )
{
  Reset();

  m_vForward = f;
  m_vUpward = u;
  m_vRight = m_vForward.Cross(m_vUpward);
  m_vRight.ToUnit();
  // To ensure numerical stability
  CVector3 vDiff = f - u;
  if ( vDiff[CVector3::X] < 0.0001f &&
       vDiff[CVector3::Y] < 0.0001f &&
       vDiff[CVector3::Z] < 0.0001f )
  {
    m_qRotation.CreateFromAxisAngle( m_vRight, 180.0f);
  }
  else 
  {
    m_qRotation = CQuaternion::RotationArc( m_vForward, f);
  }
  SetRotationChanged(1);
}
/////////////////////////////////////////////////////////////////
void			
Phoenix::Spatial::COrientable::SetRotationFromInitialState( const CVector3 &vForward,
							    const CVector3 &vRight,
							    const CVector3 &vUp,
							    const CQuaternion & qQuat)
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
Phoenix::Spatial::COrientable::AppendToRotation( const CQuaternion &qQuaternion)
{
  /* rotate every orientation vector by new quaternion*/
  m_vForward.Rotate(qQuaternion);
  m_vUpward.Rotate(qQuaternion);
  m_vRight.Rotate(qQuaternion);

  m_qRotation = qQuaternion * m_qRotation;
  SetRotationChanged(1);

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Spatial::COrientable::SetRotation( const CMatrix4x4f &mMatrix )
{
  // Restore initial state
  Reset();
  // Get quaternion and rotate vectors
  m_qRotation = Math::RotationMatrixToQuaternion(mMatrix);
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
