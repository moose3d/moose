#include "MooseGameObject.h"
#include "MooseCollision.h"
#include "MooseSpatialGraph.h"
#include "MooseCollisionEvent.h"
#include <list>
#include <iterator>
#include <algorithm>
/////////////////////////////////////////////////////////////////
using namespace Moose::Scene;
using namespace Moose::Core;
using namespace Moose::Graphics;
using namespace Moose::AI;
using namespace std;
/////////////////////////////////////////////////////////////////
void 
Moose::Scene::CGameObject::OnCollisionEnter_( const Moose::AI::CCollisionEnter *pMsg ) 
{
    this->OnCollisionEnter( pMsg->GetCollider());
}
void
Moose::Scene::CGameObject::OnCollisionStay_( const Moose::AI::CCollisionStay *pMsg )
{
    this->OnCollisionStay( pMsg->GetCollider());
}
void 
Moose::Scene::CGameObject::OnCollisionExit_( const Moose::AI::CCollisionExit *pMsg )
{
    this->OnCollisionExit( pMsg->GetCollider());
}
//////////////////////////////////////////////////////////////////
// Empty stubs to be overwritten in child classes.
void 
Moose::Scene::CGameObject::OnCollisionEnter( Moose::Scene::CGameObject *pCollider )
{

}
void 
Moose::Scene::CGameObject::OnCollisionStay( Moose::Scene::CGameObject *pCollider )
{

}
void 
Moose::Scene::CGameObject::OnCollisionExit( Moose::Scene::CGameObject *pCollider )
{

}
/////////////////////////////////////////////////////////////////
Moose::Scene::CGameObject::~CGameObject()
{
	if ( m_pCollider != this )
	{
		delete m_pCollider;
	}
}
/////////////////////////////////////////////////////////////////
Moose::Scene::CGameObject::CGameObject( ) : m_nSpatialIndex(0), m_pCollider(NULL), 
                                              CAIObject( this )


{
        m_pCollider = this;
        SetEnabled(true); // by default, each object is active
        SetColliderTransform( & GetWorldTransform() ); // collider tracks world transform
    // Register handlers for enter-stay-exit for collisions.
    GetMessageQueue().RegisterHandler(this, &CGameObject::OnCollisionEnter_);
    GetMessageQueue().RegisterHandler(this, &CGameObject::OnCollisionStay_);
    GetMessageQueue().RegisterHandler(this, &CGameObject::OnCollisionExit_);

}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CGameObject::Init()
{
#if !defined(MOOSE_APPLE_IPHONE)
	LoadScript();
#else 
    RegisterUserCommands();
#endif
}

/////////////////////////////////////////////////////////////////
unsigned int
Moose::Scene::CGameObject::GetSpatialIndex() const
{
  return m_nSpatialIndex;
}
/////////////////////////////////////////////////////////////////
void
Moose::Scene::CGameObject::SetSpatialIndex( unsigned int nIndex )
{
  m_nSpatialIndex = nIndex;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Scene::CGameObject::Intersects( const Moose::Volume::CSphere & sphere ) const
{
	return Moose::Collision::SphereIntersectsSphere( sphere, GetWorldBoundingSphere() );
}
/////////////////////////////////////////////////////////////////
bool
Moose::Scene::CGameObject::Intersects( const Moose::Graphics::CFrustum & frustum ) const
{
	return Moose::Collision::SphereIntersectsPolytope( GetWorldBoundingSphere() , frustum );
}
/////////////////////////////////////////////////////////////////
bool
Moose::Scene::CGameObject::Intersects( const Moose::Volume::COrientedBox & box ) const
{
  return Moose::Collision::SphereIntersectsOBB( GetWorldBoundingSphere(), box );
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::Scene::CGameObject::Intersects( const Moose::Math::CVector3<float> & vPoint ) const
{
  return Moose::Collision::PointInsideSphere( GetWorldBoundingSphere(), vPoint );
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::Scene::CGameObject::Intersects( const Moose::Math::CRay & ray, float *pfDistance ) const
{
  return Moose::Collision::RayIntersectsSphere( ray, pfDistance, NULL, GetWorldBoundingSphere());
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::Scene::CGameObject::Intersects( const Moose::Collision::ICollider & collider ) const
{
  return collider.Intersects( GetWorldBoundingSphere() );
}
//////////////////////////////////////////////////////////////////////////////
bool
Moose::Scene::CGameObject::Intersects( const Moose::Volume::CCapsule & capsule ) const
{
    return Moose::Collision::SphereIntersectsCapsule( GetWorldBoundingSphere(), capsule);
}
//////////////////////////////////////////////////////////////////////////////
Moose::Volume::CSphere
Moose::Scene::CGameObject::GetWorldBoundingSphere() const
{
	//CVector3<float> vTmp;

	/*Transform( GetBoundingSphere().GetPosition(),
				const_cast<Moose::Math::CTransform &>(GetWorldTransform()).GetMatrix(),
				vTmp );*/
    float fScaleMax = std::max( std::max( fabsf(GetWorldTransform().GetScaling()[0]),fabsf(GetWorldTransform().GetScaling()[1])), fabsf(GetWorldTransform().GetScaling()[2]));
	return Moose::Volume::CSphere( GetBoundingSphere().GetPosition() +GetWorldTransform().GetTranslation(),
																	 GetBoundingSphere().GetRadius()*fScaleMax );
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CGameObject::SetCollider( Moose::Collision::ICollider *pCollider )
{
	m_pCollider = pCollider;

	if ( m_pCollider == NULL )
	{
		m_pCollider = this;
	} 
    else 
    {
      // assign gameobject's world transform to collider
      m_pCollider->SetColliderTransform( &GetWorldTransform());
    }
}
////////////////////////////////////////////////////////////////////////////////
Moose::Collision::ICollider *
Moose::Scene::CGameObject::GetCollider()
{
	return m_pCollider;
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CGameObject::Update( float fSecondsPassed )
{
  UpdateScript(fSecondsPassed);
}

