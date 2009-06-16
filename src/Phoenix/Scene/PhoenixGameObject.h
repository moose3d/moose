#ifndef __PhoenixGameObject_h__
#define __PhoenixGameObject_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixMath.h"
#include "PhoenixTransform.h"
#include "PhoenixVolume.h"
#include "PhoenixRenderable.h"
#include "PhoenixRenderableProperty.h"
#include "PhoenixAPI.h"
#include "PhoenixTagged.h"
#include "PhoenixCollider.h"
/////////////////////////////////////////////////////////////////
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    class PHOENIX_API CSphereBound
    {
    private:
      /// Model bounding sphere
      Phoenix::Volume::CSphere		m_BoundingSphere;
    public:
      CSphereBound();
      virtual ~CSphereBound();
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere.
      Phoenix::Volume::CSphere & GetBoundingSphere();
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding sphere.
      const Phoenix::Volume::CSphere & GetBoundingSphere() const;
    };
    /////////////////////////////////////////////////////////////////
    class PHOENIX_API CBoxBound
    {
    private:
      /// Model bounding box.
      Phoenix::Volume::COrientedBox	m_BoundingBox;
    public:
      CBoxBound();
      virtual ~CBoxBound();
      ////////////////////
      /// Returns bounding box.
      /// \returns Reference to bounding box.
      Phoenix::Volume::COrientedBox & GetBoundingBox();
      ////////////////////
      /// Returns bounding sphere.
      /// \returns Reference to bounding box.
      const Phoenix::Volume::COrientedBox & GetBoundingBox() const;
    };

    /////////////////////////////////////////////////////////////////
    /// GameObject class; base for every object in a game.
    class PHOENIX_API CGameObject : public Phoenix::Math::CTransformable,
									public Phoenix::Graphics::CRenderableProperty,
									public Phoenix::Core::CTagged,
									public Phoenix::Scene::CSphereBound,
									public Phoenix::Collision::ICollider
    {
      /// In which spatial index this node is in.
      unsigned int			m_nSpatialIndex;
    public:
      ////////////////////
      /// Constructor.
      /// \param nLodLevels Number lod levels in this gameobjects.
      CGameObject();
      ////////////////////
      /// Destructor.
      virtual ~CGameObject();
      ////////////////////
      /// Returns spatial index of this node.
      /// \param index into 1-dimensional array.
      unsigned int GetSpatialIndex() const;
      ////////////////////
      /// Sets spatial index of this node.
      /// \param nIndex New index.
      void SetSpatialIndex( unsigned int nIndex );

      bool Intersects( const Phoenix::Volume::CSphere & sphere ) const;
      bool Intersects( const Phoenix::Graphics::CFrustum & frustum ) const;
    };
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
