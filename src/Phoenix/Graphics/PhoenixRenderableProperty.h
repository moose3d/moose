#ifndef __PhoenixGameRenderable_h__
#define __PhoenixGameRenderable_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixAPI.h"
#include "PhoenixResourceManager.h"
#include "PhoenixRenderable.h"
#include "PhoenixRenderableModel.h"
#include "PhoenixRenderableModelShared.h"
#include "PhoenixCore.h"
#include <vector>
#include <map>
#include <list>
#include <set>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {

    class COglRenderer;
    /// One object can consist of several renderables that are drawn consequently.
    /// Each object may have several LOD levels.
    typedef std::vector< float >			    LodLevels;
    typedef std::list< Phoenix::Graphics::CRenderable *>    RenderableList;
    typedef std::vector< RenderableList >		    LodLevelObjects;

    /////////////////////////////////////////////////////////////////
    /// Class for game objects that can be rendered in a game.
    class PHOENIX_API CRenderableProperty : public Phoenix::Core::CViewable
    {
    private:
      LodLevelObjects			m_LodLevels; ///!< Renderable objects.
      RenderableList		        m_lstLights; ///!< Lights that have effect on this Object.
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
      Phoenix::Graphics::RenderableList & GetRenderableObjects( size_t nLodLevel );
      ////////////////////
      /// Returns renderable handles for given lod level.
      /// \param nLodLevel Lod level.
      /// \returns List of renderable handles.
      const Phoenix::Graphics::RenderableList & GetRenderableObjects( size_t nLodLevel ) const;
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
      Phoenix::Graphics::CRenderableModelShared * AddRenderableModel( const char *szModelName, size_t nLodLevel, Phoenix::Math::CTransform *pWorldTransform = NULL );
      ////////////////////
      /// Adds existing renderable object to given lod level.
      /// \param pRenderable Renderable to be added.
      /// \param nLodLevel To which level renderable is added.
      /// \param pWorldTransform Pointer to world space transform which controls this renderable position. By default, NULL (no transform is applied).
      void AddRenderable( Phoenix::Graphics::CRenderable *pRenderable, size_t nLodLevel, Phoenix::Math::CTransform *pWorldTransform = NULL) ;
      ////////////////////
      /// Returns list of pointers to Renderables (lights).
      /// \returns List containing pointers to Renderables.
      RenderableList & GetLights();
      ////////////////////
      /// Returns list of pointers to Renderables (lights).
      /// \returns List containing pointers to Renderables.
      const RenderableList & GetLights() const;
      ////////////////////
      /// Returns renderable with id iId from lod iLod, if found.
      /// \param nLod Lod level where search is applied.
      /// \param iId  Id of renderable to be sought.
      /// \returns Pointer to renderable if found
      /// \returns NULL if not found or if invalid iLod is given.
      Phoenix::Graphics::CRenderable * GetRenderableById( size_t nLod, int iId );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
