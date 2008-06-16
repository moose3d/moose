#ifndef __PhoenixGameObject_h__
#define __PhoenixGameObject_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixModel.h"
#include "PhoenixMath.h"
#include "PhoenixTransform.h"
#include "PhoenixVolume.h"
#include "PhoenixRenderable.h"
/////////////////////////////////////////////////////////////////
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    /// GameObject class; base for every object in a game. One object 
    /// can consist of several models. 
    /// ************************************************************
    /// \warn THIS NEEDS RETHINKING. 
    /// \warn Should a gameobject consist of several models,
    /// \warn or just one model with different indices for different LODs and 
    /// \warn animated sections?
    /// \warn Or does gameobject rendering have anything to do with this all?
    /// ************************************************************
    class CGameObject : public Phoenix::Graphics::CRenderable,
			public Phoenix::Math::CTransformable
    {
    private:
      /// Model objects
      std::vector< Phoenix::Core::CHandle<Phoenix::Graphics::CModel> > m_vecModelHandles;
      /// Model bounding sphere
      Phoenix::Volume::CSphere		m_BoundingSphere;
      /// Model bounding box.
      Phoenix::Volume::COrientedBox	m_BoundingBox;
      /// In which spatial index this node is in.
      unsigned int			m_nSpatialIndex;
    public:
      ////////////////////
      /// Constructor. 
      /// \param nNumModels Number models in this gameobject.
      CGameObject( size_t nNumModels = 1);
      ////////////////////
      /// Destructor.
      virtual ~CGameObject() {}
      ////////////////////
      /// Returns model handle.
      /// \param nIndex Which model handle is returned.
      /// \returns Model handle reference.
      Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & GetModelHandle( size_t nIndex = 0);
      ////////////////////
      /// Returns model handle.
      /// \param nIndex Which model handle is returned.
      /// \returns Model handle reference.
      const Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & GetModelHandle( size_t nIndex = 0) const;
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
      /// Returns the type of this object.
      /// \returns object type
      //const TYPE GetType() const;
      ////////////////////
      /// Sets object type.
      /// \param type Object type.
      //void SetType(TYPE type);
      ////////////////////
      /// Returns number of model handles in this object.
      /// \returns Number of allocated model handles.
      size_t GetNumModels() const;
    };
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline 
Phoenix::Scene::CGameObject::CGameObject( size_t nNumModels ) : m_nSpatialIndex(0)
{
  if ( nNumModels == 0 ) nNumModels = 1;
  for ( size_t i=0;i<nNumModels;i++)
    m_vecModelHandles.push_back(Phoenix::Core::CHandle<Phoenix::Graphics::CModel>());
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & 
Phoenix::Scene::CGameObject::GetModelHandle( size_t nIndex )
{
  assert( nIndex < m_vecModelHandles.size());
  return m_vecModelHandles[nIndex];
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & 
Phoenix::Scene::CGameObject::GetModelHandle( size_t nIndex) const
{
  assert( nIndex < m_vecModelHandles.size());
  return m_vecModelHandles[nIndex];
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
// inline const TYPE
// Phoenix::Scene::CGameObject::GetType() const
// {
//   return m_Type;
// }
// /////////////////////////////////////////////////////////////////
// inline void
// Phoenix::Scene::CGameObject::SetType(TYPE type) 
// {
//   m_Type = type;
// }
/////////////////////////////////////////////////////////////////
inline size_t 
Phoenix::Scene::CGameObject::GetNumModels() const
{
  return m_vecModelHandles.size();
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
#endif
