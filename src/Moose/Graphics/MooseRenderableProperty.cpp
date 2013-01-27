/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MooseRenderableProperty.h"
#include "MooseRenderableModel.h"
#include "MooseModelHelper.h"
#include "MooseRenderableModelShared.h"
#include <cassert>
/////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
using namespace Moose::Core;
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderableProperty::CRenderableProperty()
{

}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderableProperty::~CRenderableProperty()
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
Moose::Graphics::CRenderableProperty::InitializeRenderables( size_t nLodLevels )
{
  if ( !m_LodLevels.empty() ) return;
  if ( nLodLevels == 0 ) nLodLevels = 1;
  for ( size_t i=0;i<nLodLevels;i++)
  {
    m_LodLevels.push_back( Moose::Graphics::RenderableList() );
    m_LodDistances.push_back(0.0f);  // add default value for distances
  }
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderableModelShared *
Moose::Graphics::CRenderableProperty::AddRenderableModel( const char *szResourceName, size_t nLodLevel, Moose::Math::CTransform *pWorldTransform  )
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
Moose::Graphics::CRenderableProperty::AddRenderable( CRenderable *pRenderable, size_t nLodLevel, Moose::Math::CTransform *pWorldTransform )
{
  // Add renderable handle to given lod level
  GetRenderableObjects(nLodLevel).push_back( pRenderable );
  // renderables follow transformations of this object
  pRenderable->SetTransform( pWorldTransform );
}
/////////////////////////////////////////////////////////////////
size_t
Moose::Graphics::CRenderableProperty::GetNumLodLevels() const
{
  return m_LodLevels.size();
}
/////////////////////////////////////////////////////////////////
size_t
Moose::Graphics::CRenderableProperty::GetLodLevel( float fDistanceSqr ) const
{
  // there won't be several thousands of lod levels (or if there are, 
  // this should be overwritten in child class). A few tens should not matter in this.
  size_t index = 0;
  while( (index < (GetNumLodLevels()-1)) && (fDistanceSqr > m_LodDistances[index]) ) ++index;
  return index;
}
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CRenderableProperty::SetLodDistance( size_t lod, float fDistance )
{
  // store distances as squared so GetLodLevel works
  m_LodDistances[lod % GetNumLodLevels()] = fDistance * fDistance;
}
////////////////////////////////////////////////////////
Moose::Graphics::RenderableList &
Moose::Graphics::CRenderableProperty::GetRenderableObjects( size_t nLodLevel )
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
const Moose::Graphics::RenderableList &
Moose::Graphics::CRenderableProperty::GetRenderableObjects( size_t nLodLevel ) const
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::LightList &
Moose::Graphics::CRenderableProperty::GetLights()
{
	return m_lstLights;
}
/////////////////////////////////////////////////////////////////
const Moose::Graphics::LightList &
Moose::Graphics::CRenderableProperty::GetLights() const
{
	return m_lstLights;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderable *
Moose::Graphics::CRenderableProperty::GetRenderableById( size_t nLod, int iId )
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
void 
Moose::Graphics::CRenderableProperty::RemoveRenderable( size_t nLodLevel, Moose::Graphics::CRenderable *pRenderable )
{
  // sanity check for lod level
  if ( nLodLevel >= GetNumLodLevels() ) return;
  // Check objects from given lod.
  RenderableList::iterator it = GetRenderableObjects(nLodLevel).begin();
  for ( ; it != GetRenderableObjects(nLodLevel).end(); it++ )
  {
    if ( (*it) == pRenderable ) 
    {
      GetRenderableObjects(nLodLevel).erase(it);
      delete pRenderable;
      return;
    }
  }
  
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CRenderableProperty::RemoveRenderableById( size_t nLodLevel, int iId )
{
// sanity check for lod level
  if ( nLodLevel >= GetNumLodLevels() ) return;
  // Check objects from given lod.
  RenderableList::iterator it = GetRenderableObjects(nLodLevel).begin();
  for ( ; it != GetRenderableObjects(nLodLevel).end(); it++ )
  {
    if ( (*it)->GetId() == iId ) 
    {
      CRenderable *pDEADBEEF = *it;
      GetRenderableObjects(nLodLevel).erase(it);
      delete pDEADBEEF;
      return;
    }
  }
  
}
/////////////////////////////////////////////////////////////////
