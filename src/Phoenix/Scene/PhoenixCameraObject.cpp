#include <cassert>
#include "PhoenixCameraObject.h"

using namespace Phoenix::Math;
using namespace Phoenix::Scene;
using namespace Phoenix::Core;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
const float CLOSE_TO_ZERO = 0.01f;
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CCameraObject::CCameraObject() :
	m_fSpeed(0.0f),
  m_fZoomSpeed(0.0f),
  m_vVelocity(0,0,0),
  m_MoveDir( OUT ),
  m_bMoving(false)
{

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::MoveLeft()
{
  m_MoveDir = LEFT;
  m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::MoveRight()
{
  m_MoveDir = RIGHT;
  m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::MoveForward()
{
  m_MoveDir = FORWARD;
  m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::MoveBackward()
{
  m_MoveDir = BACKWARD;
  m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::Stop()
{
  m_bMoving = false;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CCameraObject::IsMoving()
{
  return m_bMoving;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::SetFollowParent( bool bFlag )
{
	m_Settings.m_bFollowParent = bFlag;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::Update( float fSeconds )
{
  float timeSeconds = fSeconds;

  if ( m_bMoving )
  {
    CVector3<float> vXZ;
    switch ( m_MoveDir)
    {
    case LEFT:

      vXZ = GetRightVector();
      vXZ[1] = 0.0f;
      m_vVelocity -= vXZ;

      break;
    case RIGHT:

      vXZ = GetRightVector();
      vXZ[1] = 0.0f;
      m_vVelocity += vXZ;

      break;
    case FORWARD:

      vXZ = GetForwardVector();
      vXZ[1] = 0.0f;
      m_vVelocity += vXZ;

      break;
    case BACKWARD:

      vXZ = GetForwardVector();
      vXZ[1] = 0.0f;
      m_vVelocity -= vXZ;

      break;
    default:
      break;
    }

    m_vVelocity.Normalize();
    m_fSpeed += m_Settings.m_fAccelSpeed * timeSeconds;
    if ( m_fSpeed > m_Settings.m_fMaxSpeed) m_fSpeed = m_Settings.m_fMaxSpeed;
    GetLocalTransform().Move( m_vVelocity * m_fSpeed * timeSeconds );
  }
  else
  {
    if ( m_fSpeed > CLOSE_TO_ZERO )
    {
      float fDecrement = m_fSpeed * m_fSpeed * timeSeconds;
      if ( fDecrement < CLOSE_TO_ZERO ) fDecrement = CLOSE_TO_ZERO;
      m_fSpeed -= fDecrement;
      if ( m_fSpeed > CLOSE_TO_ZERO)
      {
      	GetLocalTransform().Move( m_vVelocity * m_fSpeed * timeSeconds );
      }
      else
      {
      	m_vVelocity[0] = m_vVelocity[1] = m_vVelocity[2] = 0.0f;
      }
    }

  }


  if ( m_fZoomSpeed > CLOSE_TO_ZERO )
  {
    // This needs clearing up.
    // max zoom = furthest away
    // when were moving to max zoom, our speed is positive
    // so we need to _reduce_ speed.
    m_fZoomSpeed -= m_Settings.m_fZoomDeaccelSpeed * timeSeconds;
    if ( IsZoomAtMaximum() || m_fZoomSpeed <= CLOSE_TO_ZERO )
    {
      m_fZoomSpeed = 0.0f;
    }
    else
    {
      Zoom( m_fZoomSpeed * timeSeconds );
    }
  }
  else if ( m_fZoomSpeed < -CLOSE_TO_ZERO )
  {
    // min zoom = closest
    // when were moving to min zoom, our speed is negative
    // so we need to _increase_ speed.
    m_fZoomSpeed += m_Settings.m_fZoomDeaccelSpeed * timeSeconds;
    if ( IsZoomAtMinimum() || m_fZoomSpeed >= -CLOSE_TO_ZERO )
    {
      m_fZoomSpeed = 0.0f;
    }
    else
    {
      Zoom( m_fZoomSpeed * timeSeconds );
    }

  }

  if ( m_Settings.m_bFollowParent )
  {
  	assert( GetTransformNode() != NULL);
  	CTransformable *pParent = GetTransformNode()->GetParentTransformNode()->GetTransformable();
		assert( pParent != NULL );

  	if ( m_Settings.m_bUseParentRotation )
  	{
  		SetRotation( pParent->GetWorldTransform().GetRotation() );
  		// Update position from world transforms of parent node.
  		SetPosition( pParent->GetWorldTransform().GetTranslation() );
  	}
  	else
  	{
  		SetPosition( pParent->GetWorldTransform().GetTranslation() - (GetForwardVector() * this->GetTrackballDistance()) );
  	}
  }
  else
  {
    SetPosition( GetLocalTransform().GetTranslation() -
								(GetForwardVector() * this->GetTrackballDistance()) );
  }
  UpdateScript(fSeconds);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::HandleMouseRotation( const CVector2<int> & vOldPos, const CVector2<int> & vNewPos )
{


  CTransformNode * pParent = GetTransformNode()->GetParentTransformNode();
  assert( pParent != NULL && "Parent transform is null in cameraobject");

  if ( m_Settings.m_bFollowParent == true )
  {

  	if ( m_Settings.m_bUseParentRotation == true ) return;

  	// Rotate around target position, if available.
    m_vFocusPoint = pParent->GetTransformable()->GetWorldTransform().GetTranslation();

  }
  else
  {
  	m_vFocusPoint = GetFocusPoint();
  }
  CQuaternion q;

  q.CreateFromAxisAngle( 0,1,0, vOldPos[0] - vNewPos[0] );
  CVector3<float> vPoint = m_vFocusPoint;
  vPoint[1] = GetPosition()[1];
  AppendToRotation(q);
  //RotateAroundPoint(vPoint, q);
  q.CreateFromAxisAngle( GetRightVector(), vNewPos[1] - vOldPos[1]);
  AppendToRotation(q);
	//RotateAroundPoint( m_vFocusPoint, q );


}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::SetZoom( float fZoomLevel )
{
  SetTrackballDistance( fZoomLevel );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::Zoom( float fZoomStep )
{
  // store previous point
  CVector3<float> vPosOld = GetFocusPoint();
  // move tracking point
  SetTrackballDistance( GetTrackballDistance() + fZoomStep );

  if ( IsZoomAtMinimum())      SetTrackballDistance(0.10f);
  else if ( IsZoomAtMaximum()) SetTrackballDistance(m_Settings.m_fMaxZoomOut);
  // get new tracking point
  CVector3<float> vPosNew = GetFocusPoint();
  // move camera so tracking point keeps stationary.
  Move(vPosOld-vPosNew);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::ZoomIn( float fZoomStep )
{

  m_fZoomSpeed += fZoomStep;

}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::ZoomOut( float fZoomStep )
{
  m_fZoomSpeed -= fZoomStep;
}
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CCameraObject::MoveDir
Phoenix::Scene::CCameraObject::GetMoveDir()
{
  return m_MoveDir;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CCameraObject::IsZoomAtMaximum()
{
  return (GetTrackballDistance() >= m_Settings.m_fMaxZoomOut);
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CCameraObject::IsZoomAtMinimum()
{
  return (GetTrackballDistance() <= 0.1f );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CCameraObject::CreateSelectionFrustum( const CQuad &quad, CFrustum & selectionFrustum )
{
  //selectionFrustum.GetPlanes().clear();



  CVector3<float> vFarLeftUp = UnProject( quad.GetPosition()[0]-quad.GetHalfWidth(),
					  quad.GetPosition()[1]+quad.GetHalfHeight(),
					  1.0f);

  CVector3<float> vFarLeftDown = UnProject( quad.GetPosition()[0]-quad.GetHalfWidth(),
					    quad.GetPosition()[1]-quad.GetHalfHeight(),
					    1.0f);

  CVector3<float> vFarRightUp = UnProject( quad.GetPosition()[0]+quad.GetHalfWidth(),
					   quad.GetPosition()[1]+quad.GetHalfHeight(),
					   1.0f);
  CVector3<float> vFarRightDown = UnProject( quad.GetPosition()[0]+quad.GetHalfWidth(),
					     quad.GetPosition()[1]-quad.GetHalfHeight(),
					     1.0f);

  CVector3<float> vNearLeftDown = UnProject( quad.GetPosition()[0]-quad.GetHalfWidth(),
					     quad.GetPosition()[1]-quad.GetHalfHeight(),
					     0.0f);
  CVector3<float> vNearLeftUp = UnProject( quad.GetPosition()[0]-quad.GetHalfWidth(),
					   quad.GetPosition()[1]+quad.GetHalfHeight(),
					   0.0f);
  CVector3<float> vNearRightUp = UnProject( quad.GetPosition()[0]+quad.GetHalfWidth(),
					    quad.GetPosition()[1]+quad.GetHalfHeight(),
					    0.0f);
  CVector3<float> vNearRightDown = UnProject( quad.GetPosition()[0]+quad.GetHalfWidth(),
					      quad.GetPosition()[1]-quad.GetHalfHeight(),
					      0.0f);





  selectionFrustum.GetPlane( Phoenix::Volume::TOP).Calculate( (vFarLeftUp-vNearLeftUp).Cross(vNearRightUp-vNearLeftUp).GetNormalized(), vNearLeftUp );
  selectionFrustum.GetPlane( Phoenix::Volume::BOTTOM).Calculate( (vNearRightDown-vNearLeftDown).Cross(vFarLeftDown-vNearLeftDown).GetNormalized(), vNearLeftDown );
  selectionFrustum.GetPlane( Phoenix::Volume::LEFT).Calculate( (vFarLeftDown-vNearLeftDown).Cross(vNearLeftUp-vNearLeftDown).GetNormalized(), vNearLeftDown );
  selectionFrustum.GetPlane( Phoenix::Volume::RIGHT).Calculate( (vNearRightUp-vNearRightDown).Cross(vFarRightDown-vNearRightDown).GetNormalized(), vNearRightUp );

  selectionFrustum.GetPlane( Phoenix::Volume::FRONT) = Frustum().GetPlane( Phoenix::Volume::FRONT) ;
  selectionFrustum.GetPlane( Phoenix::Volume::BACK) =  Frustum().GetPlane( Phoenix::Volume::BACK ) ;

}
/////////////////////////////////////////////////////////////////

