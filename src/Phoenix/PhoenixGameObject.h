#ifndef __PhoenixGameObject_h__
#define __PhoenixGameObject_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixMath.h"
#include "PhoenixTransform.h"
#include "PhoenixVolume.h"
#include "PhoenixRenderable.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics 
  {
    class COglRenderer;
  }
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    /// GameObject class; base for every object in a game. One object 
    /// can consist of several renderables that are drawn consequently. 
    /// Each object may have several LOD levels.
    typedef std::vector< std::pair<float,size_t> >				LodDistanceLevel;
    typedef std::list< Phoenix::Core::CHandle<Phoenix::Graphics::CRenderable> *> RenderableHandleList;
    typedef std::vector< RenderableHandleList >				        LodLevelObjects;

    class PHOENIX_API CGameObject : public Phoenix::Math::CTransformable
    {
    private:
      /// Renderable objects
      LodLevelObjects			m_LodLevels;
      /// Model bounding sphere
      Phoenix::Volume::CSphere		m_BoundingSphere;
      /// Model bounding box.
      Phoenix::Volume::COrientedBox	m_BoundingBox;
      /// In which spatial index this node is in.
      unsigned int			m_nSpatialIndex;
      /// which renderable is selected according to distance.
      LodDistanceLevel			m_LodDistanceLevel;       
      /// Which lod level is active.
      size_t				m_LodLevel;      
      /// Should this object be rendered.
      bool				m_bIsRenderable;
    public:
      ////////////////////
      /// Constructor. 
      /// \param nLodLevels Number lod levels in this gameobjects.
      CGameObject( size_t nLodLevels = 1 );
      ////////////////////
      /// Destructor.
      virtual ~CGameObject();
      ////////////////////
      /// Returns renderable handles for given lod level.
      /// \param nLodLevel Lod level.
      /// \returns List of renderable handles.
      Phoenix::Scene::RenderableHandleList & GetRenderableObjects( size_t nLodLevel );
      ////////////////////
      /// Returns renderable handles for given lod level.
      /// \param nLodLevel Lod level.
      /// \returns List of renderable handles.
      const Phoenix::Scene::RenderableHandleList & GetRenderableObjects( size_t nLodLevel ) const;
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere. 
      Phoenix::Volume::CSphere & GetBoundingSphere();
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere.
      const Phoenix::Volume::CSphere & GetBoundingSphere() const;
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere. 
      Phoenix::Volume::COrientedBox & GetBoundingBox();
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere.
      const Phoenix::Volume::COrientedBox & GetBoundingBox() const;
      ////////////////////
      /// Returns spatial index of this node.
      /// \param index into 1-dimensional array.
      unsigned int GetSpatialIndex() const;
      ////////////////////
      /// Sets spatial index of this node.
      /// \param nIndex New index.
      void SetSpatialIndex( unsigned int nIndex );
      ////////////////////
      /// Calculates world bounding box.
      /// \param box Oriented box where transformed box is stored.
      void CalculateWorldBoundingBox( Phoenix::Volume::COrientedBox & box ) const;
      ////////////////////
      /// Calculates world bounding sphere.
      /// \param sphere Sphere where transformed sphere is stored.
      void CalculateWorldBoundingSphere( Phoenix::Volume::CSphere & sphere ) const;
      ////////////////////
      /// Returns number of lod levels in this object.
      /// \returns Number of lod levels.
      size_t GetNumLodLevels() const;
      
      //void SelectRenderable( float fLodValue );
      //void AddLodDistanceAndIndex( float fDistance, size_t nIndex );*/
      
      ////////////////////
      /// Updates all renderable transforms to match current transform.
      /// Call this every time you move object.
      //void UpdateTransforms();
      ////////////////////
      /// Adds new renderable handle to given lod level and 
      /// \par attaches resource by name to it.
      /// \param szResourceName Name of the Renderable resource.
      /// \param nLodLevel To which lod level renderable is added.
      /// \param szGroupName Group name for indices. By default NULL, meaning all indices.
      /// \param bInterleaved Attempt to use interleaved data. By default, false.
      /// \returns Renderable pointer.
      Phoenix::Graphics::CRenderable * AddRenderable( const char *szResourceName, size_t nLodLevel, const char *szGroupName = NULL, bool bInterleaved = false);
      ////////////////////
      /// Adds existing renderable object to given lod level.
      /// \param pRenderable Renderable to be added.
      /// \param nLodLevel To which level renderable is added.
      void AddRenderable( Phoenix::Graphics::CRenderable *pRenderable, size_t nLodLevel);
      bool ShouldBeRendered() const;
      void SetRenderable( bool bFlag );
      
      virtual size_t GetLodLevel( float fDistanceSqr ) const;
    };
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline 
Phoenix::Scene::CGameObject::CGameObject( size_t nLodLevels ) : m_nSpatialIndex(0),m_bIsRenderable(true)
{
  if ( nLodLevels == 0 ) nLodLevels = 1;
  for ( size_t i=0;i<nLodLevels;i++)
    m_LodLevels.push_back( Phoenix::Scene::RenderableHandleList() );
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Volume::CSphere & 
Phoenix::Scene::CGameObject::GetBoundingSphere()
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Volume::CSphere & 
Phoenix::Scene::CGameObject::GetBoundingSphere() const
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Volume::COrientedBox & 
Phoenix::Scene::CGameObject::GetBoundingBox()
{
  return m_BoundingBox;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Volume::COrientedBox & 
Phoenix::Scene::CGameObject::GetBoundingBox() const
{
  return m_BoundingBox;
}
/////////////////////////////////////////////////////////////////
inline unsigned int 
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
inline size_t 
Phoenix::Scene::CGameObject::GetNumLodLevels() const
{
  return m_LodLevels.size();
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Scene::CGameObject::CalculateWorldBoundingBox( Phoenix::Volume::COrientedBox & box ) const
{
  // Transform box according to world transformation
  box = GetBoundingBox();
  box.Move( GetWorldTransform().GetTranslation() );
  box.AppendToRotation( GetWorldTransform().GetRotation() );
  // update box values
  box.CalculatePlanes();
  box.CalculateCorners();
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Scene::CGameObject::CalculateWorldBoundingSphere( Phoenix::Volume::CSphere & sphere ) const
{
  sphere = GetBoundingSphere();
  sphere.Move( GetWorldTransform().GetTranslation());
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Scene::RenderableHandleList & 
Phoenix::Scene::CGameObject::GetRenderableObjects( size_t nLodLevel )
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Scene::RenderableHandleList & 
Phoenix::Scene::CGameObject::GetRenderableObjects( size_t nLodLevel ) const
{
  if ( nLodLevel < m_LodLevels.size())
  {
    return m_LodLevels[nLodLevel];
  }
  return m_LodLevels[m_LodLevels.size()-1];
}
/////////////////////////////////////////////////////////////////
inline bool
Phoenix::Scene::CGameObject::ShouldBeRendered() const
{
  return m_bIsRenderable;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Scene::CGameObject::SetRenderable( bool bFlag )
{
  m_bIsRenderable = bFlag;
}
/////////////////////////////////////////////////////////////////
inline size_t
Phoenix::Scene::CGameObject::GetLodLevel( float fDistanceSqr ) const
{
  return 0;
}
////////////////////////////////////////////////////////
#endif
