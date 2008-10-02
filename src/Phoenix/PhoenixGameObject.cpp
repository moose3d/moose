#include "PhoenixGameObject.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixModelHelper.h"
#include "PhoenixDefaultEntities.h"
#include <assert.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Scene;
using namespace Phoenix::Core;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::~CGameObject()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::CGameObject( ) : m_nSpatialIndex(0)
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CSphere & 
Phoenix::Scene::CSphereBound::GetBoundingSphere()
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Volume::CSphere & 
Phoenix::Scene::CSphereBound::GetBoundingSphere() const
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::COrientedBox & 
Phoenix::Scene::CBoxBound::GetBoundingBox()
{
  return m_BoundingBox;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Volume::COrientedBox & 
Phoenix::Scene::CBoxBound::GetBoundingBox() const
{
  return m_BoundingBox;
}
/////////////////////////////////////////////////////////////////
unsigned int 
Phoenix::Scene::CGameObject::GetSpatialIndex() const 
{
  return m_nSpatialIndex;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Scene::CGameObject::SetSpatialIndex( unsigned int nIndex )
{
  m_nSpatialIndex = nIndex;
}
/////////////////////////////////////////////////////////////////

