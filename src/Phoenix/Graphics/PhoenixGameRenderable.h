#ifndef __PhoenixGameRenderable_h__
#define __PhoenixGameRenderable_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixAPI.h"
#include "PhoenixResourceManager.h"
#include "PhoenixRenderable.h"
#include "PhoenixCore.h"
#include <vector>
#include <map>
#include <list>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    
    class COglRenderer;
    
    /// One object can consist of several renderables that are drawn consequently. 
    /// Each object may have several LOD levels.
    typedef std::vector< std::pair<float,size_t> >				LodDistanceLevel;
    typedef std::list< Phoenix::Core::CHandle<Phoenix::Graphics::CRenderable> *> RenderableHandleList;
    typedef std::vector< RenderableHandleList >				        LodLevelObjects;
    /////////////////////////////////////////////////////////////////
    /// Base class for objects that can be rendered in a game.
    class PHOENIX_API CGameRenderable : public Phoenix::Core::CEnableable
    {
    private:
      /// Renderable objects
      LodLevelObjects			m_LodLevels;
    public:
      ////////////////////
      /// Constructor.
      CGameRenderable(size_t nLodLevels = 1);
      ////////////////////
      /// Destructor.
      virtual ~CGameRenderable();
      ////////////////////
      /// Returns renderable handles for given lod level.
      /// \param nLodLevel Lod level.
      /// \returns List of renderable handles.
      Phoenix::Graphics::RenderableHandleList & GetRenderableObjects( size_t nLodLevel );
      ////////////////////
      /// Returns renderable handles for given lod level.
      /// \param nLodLevel Lod level.
      /// \returns List of renderable handles.
      const Phoenix::Graphics::RenderableHandleList & GetRenderableObjects( size_t nLodLevel ) const;
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
      /// Adds new renderable handle to given lod level and 
      /// \par attaches resource by name to it. Use this if your model data is interleaved or grouped.
      /// \param szResourceName Name of the Renderable resource.
      /// \param nLodLevel To which lod level renderable is added.
      /// \param szGroupName Group name for indices. By default NULL, meaning all indices.
      /// \param bInterleaved Attempt to use interleaved data. By default, false.
      /// \param pWorldTransform Pointer to world space transform which controls this renderable position. By default, NULL (no transform is applied).
      /// \returns Pointer to a Renderable
      Phoenix::Graphics::CRenderable * AddRenderable( const char *szResourceName, size_t nLodLevel, const char *szGroupName = NULL, bool bInterleaved = false,  Phoenix::Math::CTransform *pWorldTransform = NULL);
      ////////////////////
      /// Adds existing renderable object to given lod level. Use this only for adding non-grouped renderables.
      /// \param pRenderable Renderable to be added.
      /// \param nLodLevel To which level renderable is added.
      /// \param pWorldTransform Pointer to world space transform which controls this renderable position. By default, NULL (no transform is applied).
      void AddRenderable( Phoenix::Graphics::CRenderable *pRenderable, size_t nLodLevel, Phoenix::Math::CTransform *pWorldTransform = NULL);
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
