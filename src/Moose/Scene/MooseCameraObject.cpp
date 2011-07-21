#include <cassert>
#include "MooseCameraObject.h"

using namespace Moose::Math;
using namespace Moose::Scene;
using namespace Moose::Core;
using namespace Moose::Graphics;
/////////////////////////////////////////////////////////////////
const float CLOSE_TO_ZERO = 0.01f;
/////////////////////////////////////////////////////////////////
Moose::Scene::CCameraObject::CCameraObject() :
	m_fSpeed(0.0f),
    m_fZoomSpeed(0.0f),
    m_MoveDir( OUT ),
    m_bMoving(false),
    m_vVelocity(0,0,0)
{
    // Set some reasonable defaults
    SetNearClipping(0.01f);
    SetFarClipping(100.0f);
    Zoom(0.0f);
    SetTag( CAMERA_TAG);
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::MoveLeft()
{
    m_MoveDir = LEFT;
    m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::MoveRight()
{
    m_MoveDir = RIGHT;
    m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::MoveForward()
{
    m_MoveDir = FORWARD;
    m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::MoveBackward()
{
    m_MoveDir = BACKWARD;
    m_bMoving = true;

}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::Stop()
{
    m_bMoving = false;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Scene::CCameraObject::IsMoving()
{
    return m_bMoving;
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::SetFollowParent( bool bFlag )
{
	m_Settings.m_bFollowParent = bFlag;
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::Update( float fSeconds )
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
            m_vVelocity = -vXZ;

            break;
        case RIGHT:

            vXZ = GetRightVector();
            vXZ[1] = 0.0f;
            m_vVelocity = vXZ;

            break;
        case FORWARD:

            vXZ = GetForwardVector();
            vXZ[1] = 0.0f;
            m_vVelocity = vXZ;

            break;
        case BACKWARD:

            vXZ = GetForwardVector();
            vXZ[1] = 0.0f;
            m_vVelocity = -vXZ;

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
                //m_vVelocity[0] = m_vVelocity[1] = m_vVelocity[2] = 0.0f;
                m_fSpeed = 0.0f;
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
        SetRotation( GetWorldTransform().GetRotation());
        SetPosition( GetWorldTransform().GetTranslation());
        //-(GetForwardVector() * this->GetTrackballDistance()) );
    }
    UpdateScript(fSeconds);
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::HandleMouseRotation( const CVector2<int> & vOldPos, const CVector2<int> & vNewPos )
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
    GetLocalTransform().Rotate(q);
    //RotateAroundPoint(vPoint, q);
    q.CreateFromAxisAngle( GetRightVector(), vNewPos[1] - vOldPos[1]);
    AppendToRotation(q);
	//RotateAroundPoint( m_vFocusPoint, q );
    // propagate rotation to transformations too
    GetLocalTransform().Rotate(q);

}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::SetZoom( float fZoomLevel )
{
    SetTrackballDistance( fZoomLevel );
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::Zoom( float fZoomStep )
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
Moose::Scene::CCameraObject::ZoomIn( float fZoomStep )
{

    m_fZoomSpeed += fZoomStep;

}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::ZoomOut( float fZoomStep )
{
    m_fZoomSpeed -= fZoomStep;
}
/////////////////////////////////////////////////////////////////
Moose::Scene::CCameraObject::MoveDir
Moose::Scene::CCameraObject::GetMoveDir()
{
    return m_MoveDir;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Scene::CCameraObject::IsZoomAtMaximum()
{
    return (GetTrackballDistance() >= m_Settings.m_fMaxZoomOut);
}
/////////////////////////////////////////////////////////////////
bool
Moose::Scene::CCameraObject::IsZoomAtMinimum()
{
    return (GetTrackballDistance() <= 0.1f );
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CCameraObject::CreateSelectionFrustum( const CQuad &quad, CFrustum & selectionFrustum )
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

    selectionFrustum.GetPlane( Moose::Volume::TOP).Calculate( (vFarLeftUp-vNearLeftUp).Cross(vNearRightUp-vNearLeftUp).GetNormalized(), vNearLeftUp );
    selectionFrustum.GetPlane( Moose::Volume::BOTTOM).Calculate( (vNearRightDown-vNearLeftDown).Cross(vFarLeftDown-vNearLeftDown).GetNormalized(), vNearLeftDown );
    selectionFrustum.GetPlane( Moose::Volume::LEFT).Calculate( (vFarLeftDown-vNearLeftDown).Cross(vNearLeftUp-vNearLeftDown).GetNormalized(), vNearLeftDown );
    selectionFrustum.GetPlane( Moose::Volume::RIGHT).Calculate( (vNearRightUp-vNearRightDown).Cross(vFarRightDown-vNearRightDown).GetNormalized(), vNearRightUp );

    selectionFrustum.GetPlane( Moose::Volume::FRONT) = Frustum().GetPlane( Moose::Volume::FRONT) ;
    selectionFrustum.GetPlane( Moose::Volume::BACK) =  Frustum().GetPlane( Moose::Volume::BACK ) ;

}
/////////////////////////////////////////////////////////////////

