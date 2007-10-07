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
    class CGameObject
    {
    private:
      /// Model object
      Phoenix::Graphics::CModel		m_Model;
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
      /// Returns model.
      /// \returns Model object reference.
      Phoenix::Graphics::CModel & GetModel();
      ////////////////////
      /// Returns model.
      /// \returns Model object reference.
      const Phoenix::Graphics::CModel & GetModel() const;
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
      
    };
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline Phoenix::Graphics::CModel & 
Phoenix::Scene::CGameObject::GetModel()
{
  return m_Model;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Graphics::CModel & 
Phoenix::Scene::CGameObject::GetModel() const
{
  return m_Model;
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Math::CTransform & 
Phoenix::Scene::CGameObject::GetTransform()
{
  return m_Transform;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CTransform & 
Phoenix::Scene::CGameObject::GetTransform() const
{
  return m_Transform;
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
#endif