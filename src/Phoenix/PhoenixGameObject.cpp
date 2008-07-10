#include "PhoenixGameObject.h"
#include "PhoenixOGLRenderer.h"
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
      // delete handle, it knows how to free itself properly.
      delete *it;
    }					       
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
void 
Phoenix::Scene::CGameObject::AddRenderable( const char *szResourceName, size_t nLodLevel  )
{
  CHandle<CRenderable> *pHandle = new CHandle<CRenderable>();
  GetRenderableObjects(nLodLevel).push_back( pHandle );

  assert( (CResourceManager<CRenderable, CHandle<CRenderable> >::GetInstance())->AttachHandle( szResourceName, *pHandle ) == 0);
}
/////////////////////////////////////////////////////////////////

