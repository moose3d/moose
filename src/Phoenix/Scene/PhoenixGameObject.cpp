#include "PhoenixGameObject.h"
#include "PhoenixCollision.h"
#include "PhoenixSpatialGraph.h"
#include "PhoenixCollisionEvent.h"
#include <list>
#include <iterator>
#include <algorithm>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Scene;
using namespace Phoenix::Core;
using namespace Phoenix::Graphics;
using namespace Phoenix::AI;
using namespace std;
/////////////////////////////////////////////////////////////////
void 
Phoenix::Scene::CGameObject::OnCollisionEnter_( const Phoenix::AI::CCollisionEnter *pMsg ) 
{
    this->OnCollisionEnter( pMsg->GetCollider());
}
void
Phoenix::Scene::CGameObject::OnCollisionStay_( const Phoenix::AI::CCollisionStay *pMsg )
{
    this->OnCollisionStay( pMsg->GetCollider());
}
void 
Phoenix::Scene::CGameObject::OnCollisionExit_( const Phoenix::AI::CCollisionExit *pMsg )
{
    this->OnCollisionExit( pMsg->GetCollider());
}
//////////////////////////////////////////////////////////////////
// Empty stubs to be overwritten in child classes.
void 
Phoenix::Scene::CGameObject::OnCollisionEnter( Phoenix::Scene::CGameObject *pCollider )
{

}
void 
Phoenix::Scene::CGameObject::OnCollisionStay( Phoenix::Scene::CGameObject *pCollider )
{

}
void 
Phoenix::Scene::CGameObject::OnCollisionExit( Phoenix::Scene::CGameObject *pCollider )
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::~CGameObject()
{
	if ( m_pCollider != this )
	{
		delete m_pCollider;
	}
}
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::CGameObject( ) : m_nSpatialIndex(0), m_pCollider(NULL), 
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
Phoenix::Scene::CGameObject::Init()
{
#if !defined(PHOENIX_APPLE_IPHONE)
	LoadScript();
#else 
    RegisterUserCommands();
#endif
}

/////////////////////////////////////////////////////////////////
unsigned int
Phoenix::Scene::CGameObject::GetSpatialIndex() const
{
  return m_nSpatialIndex;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CGameObject::SetSpatialIndex( unsigned int nIndex )
{
  m_nSpatialIndex = nIndex;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CGameObject::Intersects( const Phoenix::Volume::CSphere & sphere ) const
{
	return Phoenix::Collision::SphereIntersectsSphere( sphere, GetWorldBoundingSphere() );
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CGameObject::Intersects( const Phoenix::Graphics::CFrustum & frustum ) const
{
	return Phoenix::Collision::SphereIntersectsPolytope( GetWorldBoundingSphere() , frustum );
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CGameObject::Intersects( const Phoenix::Volume::COrientedBox & box ) const
{
  return Phoenix::Collision::SphereIntersectsOBB( GetWorldBoundingSphere(), box );
}
////////////////////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CGameObject::Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const
{
  return Phoenix::Collision::PointInsideSphere( GetWorldBoundingSphere(), vPoint );
}
////////////////////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CGameObject::Intersects( const Phoenix::Math::CRay & ray, float *pfDistance ) const
{
  return Phoenix::Collision::RayIntersectsSphere( ray, pfDistance, NULL, GetWorldBoundingSphere());
}
////////////////////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CGameObject::Intersects( const Phoenix::Collision::ICollider & collider ) const
{
  return collider.Intersects( GetWorldBoundingSphere() );
}
//////////////////////////////////////////////////////////////////////////////
bool
Phoenix::Scene::CGameObject::Intersects( const Phoenix::Volume::CCapsule & capsule ) const
{
    return Phoenix::Collision::SphereIntersectsCapsule( GetWorldBoundingSphere(), capsule);
}
//////////////////////////////////////////////////////////////////////////////
Phoenix::Volume::CSphere
Phoenix::Scene::CGameObject::GetWorldBoundingSphere() const
{
	//CVector3<float> vTmp;

	/*Transform( GetBoundingSphere().GetPosition(),
				const_cast<Phoenix::Math::CTransform &>(GetWorldTransform()).GetMatrix(),
				vTmp );*/
    float fScaleMax = std::max( std::max( fabsf(GetWorldTransform().GetScaling()[0]),fabsf(GetWorldTransform().GetScaling()[1])), fabsf(GetWorldTransform().GetScaling()[2]));
	return Phoenix::Volume::CSphere( GetBoundingSphere().GetPosition() +GetWorldTransform().GetTranslation(),
																	 GetBoundingSphere().GetRadius()*fScaleMax );
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CGameObject::SetCollider( Phoenix::Collision::ICollider *pCollider )
{
	m_pCollider = pCollider;
	if ( m_pCollider == NULL )
	{
		m_pCollider = this;
	}
}
////////////////////////////////////////////////////////////////////////////////
Phoenix::Collision::ICollider *
Phoenix::Scene::CGameObject::GetCollider()
{
	return m_pCollider;
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CGameObject::Update( float fSecondsPassed )
{
  UpdateScript(fSecondsPassed);
}

