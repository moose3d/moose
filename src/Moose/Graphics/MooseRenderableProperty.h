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

#ifndef __MooseGameRenderable_h__
#define __MooseGameRenderable_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
#include "MooseLight.h"
#include "MooseResourceManager.h"
#include "MooseRenderable.h"
#include "MooseRenderableModel.h"
#include "MooseRenderableModelShared.h"
#include "MooseCore.h"
#include <vector>
#include <map>
#include <list>
#include <set>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {

    class COglRenderer;
    /// One object can consist of several renderables that are drawn consequently.
    /// Each object may have several LOD levels.
    typedef std::vector< float >			    LodLevels;
    typedef std::list< Moose::Graphics::CLight *>    LightList;
    typedef std::list< Moose::Graphics::CRenderable *>    RenderableList;
    typedef std::vector< RenderableList >		    LodLevelObjects;

    /////////////////////////////////////////////////////////////////
    /// Class for game objects that can be rendered in a game.
    class MOOSE_API CRenderableProperty : public Moose::Core::CViewable
    {
    private:
      LodLevelObjects			m_LodLevels; ///!< Renderable objects.
      LightList		            m_lstLights; ///!< Lights that have effect on this Object.
      LodLevels			        m_LodDistances; ///!< Maximum distance values per index (lod). First value is max value for lod level 0, second value is max for lod level 1, etc. Values must be stored as squared distances.
    public:
      ////////////////////
      /// Constructor.
      CRenderableProperty();
      ////////////////////
      /// Destructor.
      virtual ~CRenderableProperty();
      ////////////////////
      /// Allocates renderable handle lists according to lod levels.
      /// Must be called before any renderables are added.
      /// \param nLodLevels Number of LODs in this object.
      void InitializeRenderables( size_t nLodLevels = 1 );
      ////////////////////
      /// Returns renderable handles for given lod level.
      /// \param nLodLevel Lod level.
      /// \returns List of renderable handles.
      Moose::Graphics::RenderableList & GetRenderableObjects( size_t nLodLevel );
      ////////////////////
      /// Returns renderable handles for given lod level.
      /// \param nLodLevel Lod level.
      /// \returns List of renderable handles.
      const Moose::Graphics::RenderableList & GetRenderableObjects( size_t nLodLevel ) const;
      ////////////////////
      /// Returns number of lod levels in this object.
      /// \returns Number of lod levels.
      size_t GetNumLodLevels() const;
      ////////////////////
      /// Returns lod level based on distance.
      /// \param fDistanceSqr Distance squard to object.
      /// \return Lod index
      virtual size_t GetLodLevel( float fDistanceSqr ) const;
      ////////////////////
      /// Sets lod level distance.
      /// \param lod  Lod level (0,1,2, ...).
      /// \param fDistance Distance for this lod level (NOT squared distance). 
      /// \par You must make sure that distances are in ascending order.
      void SetLodDistance( size_t lod, float fDistance );
      ////////////////////
      /// Adds new renderable handle to given lod level and
      /// \par attaches resource by name to it.
      /// \param szModelName Name of the Model resource.
      /// \param nLodLevel To which lod level modelrenderable is added.
      /// \param pWorldTransform Pointer to world space transform which controls this renderable position. By default, NULL (no transform is applied).
      /// \returns Pointer to a RenderableModel
      Moose::Graphics::CRenderableModelShared * AddRenderableModel( const char *szModelName, size_t nLodLevel, Moose::Math::CTransform *pWorldTransform = NULL );
      ////////////////////
      /// Adds existing renderable object to given lod level.
      /// \param pRenderable Renderable to be added.
      /// \param nLodLevel To which level renderable is added.
      /// \param pWorldTransform Pointer to world space transform which controls this renderable position. By default, NULL (no transform is applied).
      void AddRenderable( Moose::Graphics::CRenderable *pRenderable, size_t nLodLevel, Moose::Math::CTransform *pWorldTransform = NULL) ;
      ////////////////////
      /// Removes renderable (and deletes it) from given lod level, if it exists.
      /// \param pRenderable Renderable to be deleted.
      /// \param nLodLevel From which level renderable is deleted from.
      void RemoveRenderable( size_t nLodLevel, Moose::Graphics::CRenderable *pRenderable );
      ////////////////////
      /// Removes renderable (and deletes it) from given lod level, if it exists.
      /// \param nLodLevel From which level renderable is deleted from.
      /// \param iId id of renderable to be deleted.
      void RemoveRenderableById( size_t nLodLevel, int iId );
      ////////////////////
      /// Returns list of pointers to Renderables (lights).
      /// \returns List containing pointers to Renderables.
      LightList & GetLights();
      ////////////////////
      /// Returns list of pointers to Renderables (lights).
      /// \returns List containing pointers to Renderables.
      const LightList & GetLights() const;
      ////////////////////
      /// Returns renderable with id iId from lod iLod, if found.
      /// \param nLod Lod level where search is applied.
      /// \param iId  Id of renderable to be sought.
      /// \returns Pointer to renderable if found
      /// \returns NULL if not found or if invalid iLod is given.
      Moose::Graphics::CRenderable * GetRenderableById( size_t nLod, int iId );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
