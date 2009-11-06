#include "PhoenixRenderableProperty.h"
#include "PhoenixRenderableModel.h"
#include "PhoenixModelHelper.h"
#include "PhoenixRenderableModelShared.h"
#include <cassert>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableProperty::CRenderableProperty()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableProperty::~CRenderableProperty()
{
  RenderableList::iterator it;
  for( size_t i=0;i< m_LodLevels.size(); i++ )
  {
    it = m_LodLevels[i].begin();
    for( ; it != m_LodLevels[i].end(); it++)
    {
       delete *it;
    }
    m_LodLevels[i].clear();
  }
}
////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableProperty::InitializeRenderables( size_t nLodLevels )
{
	if ( nLodLevels == 0 ) nLodLevels = 1;
	  for ( size_t i=0;i<nLodLevels;i++)
	    m_LodLevels.push_back( Phoenix::Graphics::RenderableList() );
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableModelShared *
Phoenix::Graphics::CRenderableProperty::AddRenderableModel( const char *szResourceName, size_t nLodLevel, bool bInterleaved, Phoenix::Math::CTransform *pWorldTransform  )
{
  // Create renderable
  CRenderableModelShared *pRenderable = new CRenderableModelShared();
  pRenderable->GetModelHandle() = szResourceName;

  // Add renderable handle to given lod level
  GetRenderableObjects(nLodLevel).push_back( pRenderable );

  // Renderable follows the transformation passed as paramater
  pRenderable->SetTransform( pWorldTransform );
  // Return new renderable object
  return pRenderable;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableProperty::AddRenderable( CRenderable *pRenderable, size_t nLodLevel, Phoenix::Math::CTransform *pWorldTransform )
{
  // Add renderable handle to given lod level
  GetRenderableObjects(nLodLevel).push_back( pRenderable );
  // renderables follow transformations of this object
  pRenderable->SetTransform( pWorldTransform );
}
/////////////////////////////////////////////////////////////////
size_t
Phoenix::Graphics::CRenderableProperty::GetNumLodLevels() const
{
  return m_LodLevels.size();
}
/////////////////////////////////////////////////////////////////
size_t
Phoenix::Graphics::CRenderableProperty::GetLodLevel( float fDistanceSqr ) const
{
  return 0;
}
////////////////////////////////////////////////////////
Phoenix::Graphics::RenderableList &
Phoenix::Graphics::CRenderableProperty::GetRenderableObjects( size_t nLodLevel )
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
const Phoenix::Graphics::RenderableList &
Phoenix::Graphics::CRenderableProperty::GetRenderableObjects( size_t nLodLevel ) const
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::RenderableList &
Phoenix::Graphics::CRenderableProperty::GetLights()
{
	return m_lstLights;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Graphics::RenderableList &
Phoenix::Graphics::CRenderableProperty::GetLights() const
{
	return m_lstLights;
}
/////////////////////////////////////////////////////////////////
