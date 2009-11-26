#include "PhoenixGameObject.h"
#include "PhoenixCollision.h"
#include "PhoenixSpatialGraph.h"
#include <list>
/////////////////////////////////////////////////////////////////
//using namespace Phoenix::Scene;
//using namespace Phoenix::Core;
//using namespace Phoenix::Graphics;
using std::list;
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::~CGameObject()
{
	if ( m_pCollider != this )
	{
		delete m_pCollider;
	}
}
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::CGameObject( ) : m_nSpatialIndex(0), 
					      m_pCollider(NULL), 
					      CAIObject( this )
{
        m_pCollider = this;
        SetEnabled(true); // by default, each object is active
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CGameObject::Init()
{
	LoadScript();
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
Phoenix::Volume::CSphere
Phoenix::Scene::CGameObject::GetWorldBoundingSphere() const
{
	CVector3<float> vTmp;
	Transform( GetBoundingSphere().GetPosition(),
				const_cast<Phoenix::Math::CTransform &>(GetWorldTransform()).GetMatrix(),
				vTmp );
	return Phoenix::Volume::CSphere( vTmp, GetBoundingSphere().GetRadius() );
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
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CGameObject::UpdateColliders( float fRadius, Phoenix::Scene::CSpatialGraph & graph )
{
	m_lstColliders.clear();
	graph.CollectObjects( Phoenix::Volume::CSphere(GetWorldTransform().GetTranslation(), fRadius), m_lstColliders );
}
////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CGameObject::CheckCollisions()
{
	list<CGameObject *>::iterator it = m_lstColliders.begin();

	for ( ; it != m_lstColliders.end(); it++)
	{
		// ignore collision on itself
		if ( *it == this) continue;
		// enqueue messages if intersection occurs
		if ( this->Intersects( **it ) )
		{
			(*it)->EnqueueMessage("OnCollisionEnter");
			this->EnqueueMessage("OnCollisionEnter");
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
