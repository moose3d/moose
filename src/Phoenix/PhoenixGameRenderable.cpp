#include "PhoenixGameRenderable.h"
#include "PhoenixModelHelper.h"
#include <cassert>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CGameRenderable::CGameRenderable( size_t nLodLevels )
{
  if ( nLodLevels == 0 ) nLodLevels = 1;
  for ( size_t i=0;i<nLodLevels;i++)
    m_LodLevels.push_back( Phoenix::Graphics::RenderableHandleList() );
}

/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CGameRenderable::~CGameRenderable()
{
  RenderableHandleList::iterator it;
  for( size_t i=0;i< m_LodLevels.size(); i++ )
  {
    it = m_LodLevels[i].begin();
    for( ; it != m_LodLevels[i].end(); it++)
    {  
      // Destroy renderable from manager
      (Phoenix::Core::CResourceManager<CRenderable, Phoenix::Core::CHandle<CRenderable> >::GetInstance())->Destroy(**it);
      // delete handle, it knows how to free itself properly.
      delete *it;
    }					       
    m_LodLevels[i].clear();
  }
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderable *
Phoenix::Graphics::CGameRenderable::AddRenderable( const char *szResourceName, size_t nLodLevel, const char *szGroupName, bool bInterleaved, Phoenix::Math::CTransform *pWorldTransform  )
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
  // renderables follow transformations of this object
  pRenderable->SetTransform( pWorldTransform );
  // Return new renderable object
  return pRenderable;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CGameRenderable::AddRenderable( CRenderable *pRenderable, size_t nLodLevel, Phoenix::Math::CTransform *pWorldTransform )
{
  // Create handle to renderable
  CHandle<CRenderable> *pHandle = new CHandle<CRenderable>();
  // Add renderable handle to given lod level
  GetRenderableObjects(nLodLevel).push_back( pHandle );
  
  // Assign handle to renderable and manage object
  assert( (CResourceManager<CRenderable, CHandle<CRenderable> >::GetInstance())->Create( pRenderable, g_UniqueName, *pHandle) == 0 );
  // renderables follow transformations of this object
  pRenderable->SetTransform( pWorldTransform );  
}
/////////////////////////////////////////////////////////////////
size_t 
Phoenix::Graphics::CGameRenderable::GetNumLodLevels() const
{
  return m_LodLevels.size();
}
/////////////////////////////////////////////////////////////////
size_t
Phoenix::Graphics::CGameRenderable::GetLodLevel( float fDistanceSqr ) const
{
  return 0;
}
////////////////////////////////////////////////////////
Phoenix::Graphics::RenderableHandleList & 
Phoenix::Graphics::CGameRenderable::GetRenderableObjects( size_t nLodLevel )
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
const Phoenix::Graphics::RenderableHandleList & 
Phoenix::Graphics::CGameRenderable::GetRenderableObjects( size_t nLodLevel ) const
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
