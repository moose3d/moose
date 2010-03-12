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
  {
    m_LodLevels.push_back( Phoenix::Graphics::RenderableList() );
    m_LodDistances.push_back(0.0f);  // add default value for distances
  }
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableModelShared *
Phoenix::Graphics::CRenderableProperty::AddRenderableModel( const char *szResourceName, size_t nLodLevel, Phoenix::Math::CTransform *pWorldTransform  )
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
  // there won't be several thousands of lod levels (or if there are, 
  // this should be overwritten in child class). A few tens should not matter in this.
  size_t index = 0;
  while( (index < (GetNumLodLevels()-1)) && (fDistanceSqr > m_LodDistances[index]) ) ++index;
  return index;
}
////////////////////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CRenderableProperty::SetLodDistance( size_t lod, float fDistance )
{
  // store distances as squared so GetLodLevel works
  m_LodDistances[lod % GetNumLodLevels()] = fDistance * fDistance;
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
Phoenix::Graphics::CRenderable *
Phoenix::Graphics::CRenderableProperty::GetRenderableById( size_t nLod, int iId )
{
	// sanity check for lod level
	if ( nLod >= GetNumLodLevels() ) return NULL;
	// Check objects from given lod.
	RenderableList::iterator it = GetRenderableObjects(nLod).begin();
	for ( ; it != GetRenderableObjects(nLod).end(); it++ )
	{
		if ( (*it)->GetId() == iId ) return *it;
	}
	return NULL;
}
/////////////////////////////////////////////////////////////////
