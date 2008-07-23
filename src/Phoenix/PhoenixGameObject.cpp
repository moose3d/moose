#include "PhoenixGameObject.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixModelHelper.h"
#include "PhoenixDefaultEntities.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Scene;
using namespace Phoenix::Core;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Scene::CGameObject::~CGameObject()
{
  RenderableHandleList::iterator it;
  for( size_t i=0;i< m_LodLevels.size(); i++ )
  {
    it = m_LodLevels[i].begin();
    for( ; it != m_LodLevels[i].end(); it++)
    {  
      // Destroy renderable from manager
      (CResourceManager<CRenderable, CHandle<CRenderable> >::GetInstance())->Destroy(**it);
      // delete handle, it knows how to free itself properly.
      delete *it;
    }					       
    m_LodLevels[i].clear();
  }
  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Scene::CGameObject::UpdateTransforms()
{
  RenderableHandleList::iterator it;
  for( size_t i=0;i< m_LodLevels.size(); i++ )
  {
    it = m_LodLevels[i].begin();
    for( ; it != m_LodLevels[i].end(); it++)
    {  
      (*(**it))->SetWorldTransform( GetWorldTransform());
      (*(**it))->SetLocalTransform( GetLocalTransform());
    }					       
  }
}
/////////////////////////////////////////////////////////////////
CRenderable *
Phoenix::Scene::CGameObject::AddRenderable( const char *szResourceName, size_t nLodLevel, const char *szGroupName, bool bInterleaved )
{
  // Create renderable
  CRenderable *pRenderable = new CRenderable();
  // create handle to renderable
  CHandle<CRenderable> *pHandle = new CHandle<CRenderable>();
  // Add renderable handle to given lod level
  GetRenderableObjects(nLodLevel).push_back( pHandle );
  
  // Assign handle to renderable and manage object
  assert( (CResourceManager<CRenderable, CHandle<CRenderable> >::GetInstance())->Create( pRenderable, g_UniqueName, *pHandle) == 0 );
  // Attach proper data to renderable.
  g_ModelHelper->CreateRenderable( szResourceName, *pRenderable, szGroupName, bInterleaved );
  // Return new renderable object
  return pRenderable;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CGameObject::AddRenderable( CRenderable *pRenderable, size_t nLodLevel )
{

  // create handle to renderable
  CHandle<CRenderable> *pHandle = new CHandle<CRenderable>();
  // Add renderable handle to given lod level
  GetRenderableObjects(nLodLevel).push_back( pHandle );
  
  // Assign handle to renderable and manage object
  assert( (CResourceManager<CRenderable, CHandle<CRenderable> >::GetInstance())->Create( pRenderable, g_UniqueName, *pHandle) == 0 );
  
}
/////////////////////////////////////////////////////////////////
