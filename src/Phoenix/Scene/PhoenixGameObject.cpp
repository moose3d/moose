#include "PhoenixGameObject.h"
#include "PhoenixCollision.h"
/////////////////////////////////////////////////////////////////
//using namespace Phoenix::Scene;
//using namespace Phoenix::Core;
//using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::~CGameObject()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::CGameObject( ) : m_nSpatialIndex(0)
{

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
	Phoenix::Volume::CSphere tmp = GetBoundingSphere();
	tmp.Move( GetWorldTransform().GetTranslation() );
	return Phoenix::Collision::SphereIntersectsSphere( sphere, tmp );
}
/////////////////////////////////////////////////////////////////
bool Phoenix::Scene::CGameObject::Intersects( const Phoenix::Graphics::CFrustum & frustum ) const
{
	Phoenix::Volume::CSphere tmp = GetBoundingSphere();
	tmp.Move( GetWorldTransform().GetTranslation() );
	return Phoenix::Collision::SphereIntersectsPolytope( tmp , frustum );
}
/////////////////////////////////////////////////////////////////
