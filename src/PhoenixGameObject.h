#ifndef __PhoenixGameObject_h__
#define __PhoenixGameObject_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixModel.h"
#include "PhoenixMath.h"
#include "PhoenixTransform.h"
#include "PhoenixVolume.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    /// GameObject class; base for every object in a game.
    template <typename TYPE>
    class CGameObject
    {
    private:
      /// Model object
      Phoenix::Core::CHandle<Phoenix::Graphics::CModel>    m_hModel;
      /// Object type
      TYPE				m_Type;
      /// World tranform 
      Phoenix::Math::CTransform		m_Transform;
      /// Model bounding sphere
      Phoenix::Volume::CSphere		m_BoundingSphere;
      /// Model bounding box.
      //Phoenix::Volume::COrientedBox	m_BoundingBox;
      /// In which spatial index this node is in.
      unsigned int			m_nSpatialIndex;
    public:
      ////////////////////
      /// Constructor.
      CGameObject();
      ////////////////////
      /// Destructor.
      ~CGameObject();
      ////////////////////
      /// Returns model handle.
      /// \returns Model handle reference.
      Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & GetModelHandle();
      ////////////////////
      /// Returns model handle.
      /// \returns Model handle reference.
      const Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & GetModelHandle() const;
      ////////////////////
      /// Returns world transform for this object.
      /// \returns Transform reference.
      Phoenix::Math::CTransform & GetTransform();
      ////////////////////
      /// Returns world transform for this object.
      /// \returns Transform reference.
      const Phoenix::Math::CTransform & GetTransform() const;      
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere. 
      Phoenix::Volume::CSphere & GetBoundingSphere();
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere.
      const Phoenix::Volume::CSphere & GetBoundingSphere() const;
      ////////////////////
      /// Returns spatial index of this node.
      /// \param index into 1-dimensional array.
      unsigned int GetSpatialIndex() const;
      ////////////////////
      /// Sets spatial index of this node.
      /// \param nIndex New index.
      void SetSpatialIndex( unsigned int nIndex );
      ////////////////////
      /// Returns the type of this object.
      /// \returns object type
      const TYPE GetType() const;
      ////////////////////
      /// Sets object type.
      /// \param type Object type.
      void SetType(TYPE type);
    };
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Scene::CGameObject<TYPE>::CGameObject() : m_Type((TYPE)0)
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Scene::CGameObject<TYPE>::~CGameObject()
{
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & 
Phoenix::Scene::CGameObject<TYPE>::GetModelHandle()
{
  return m_hModel;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const Phoenix::Core::CHandle<Phoenix::Graphics::CModel> & 
Phoenix::Scene::CGameObject<TYPE>::GetModelHandle() const
{
  return m_hModel;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Math::CTransform & 
Phoenix::Scene::CGameObject<TYPE>::GetTransform()
{
  return m_Transform;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const Phoenix::Math::CTransform & 
Phoenix::Scene::CGameObject<TYPE>::GetTransform() const
{
  return m_Transform;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Volume::CSphere & 
Phoenix::Scene::CGameObject<TYPE>::GetBoundingSphere()
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const Phoenix::Volume::CSphere & 
Phoenix::Scene::CGameObject<TYPE>::GetBoundingSphere() const
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int 
Phoenix::Scene::CGameObject<TYPE>::GetSpatialIndex() const 
{
  return m_nSpatialIndex;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void 
Phoenix::Scene::CGameObject<TYPE>::SetSpatialIndex( unsigned int nIndex )
{
  m_nSpatialIndex = nIndex;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const TYPE
Phoenix::Scene::CGameObject<TYPE>::GetType() const
{
  return m_Type;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Phoenix::Scene::CGameObject<TYPE>::SetType(TYPE type) 
{
  m_Type = type;
}
/////////////////////////////////////////////////////////////////
#endif
