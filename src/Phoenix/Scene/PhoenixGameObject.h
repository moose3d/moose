#ifndef __PhoenixGameObject_h__
#define __PhoenixGameObject_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixMath.h"
#include "PhoenixTransform.h"
#include "PhoenixSphereBound.h"
#include "PhoenixRenderable.h"
#include "PhoenixRenderableProperty.h"
#include "PhoenixAPI.h"
#include "PhoenixTagged.h"
#include "PhoenixSphereCollider.h"
/////////////////////////////////////////////////////////////////
#include <vector>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    /// GameObject class; base for every object in a game.
    class PHOENIX_API CGameObject : public Phoenix::Math::CTransformable,
									 public Phoenix::Graphics::CRenderableProperty,
									 public Phoenix::Core::CTagged,
									 public Phoenix::Collision::CSphereCollider
    {
      /// In which spatial index this node is in.
      unsigned int					  		m_nSpatialIndex;
      Phoenix::Collision::ICollider * 		m_pCollider;
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
      ////////////////////
      bool Intersects( const Phoenix::Volume::CSphere & sphere ) const;
      bool Intersects( const Phoenix::Graphics::CFrustum & frustum ) const;
      bool Intersects( const Phoenix::Volume::COrientedBox & box ) const;
      bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const;
      ////////////////////
      /// Returns bounding sphere transformed using current world transform.
      /// \return Bounding sphere in world coordinates.
      Phoenix::Volume::CSphere GetWorldBoundingSphere() const;
      ////////////////////
      // In case we need to optimize world bounding sphere computation by adding cache for sphere,
      // this comes in handy.
      //void PostTransformUpdate();
      ////////////////////
      /// Sets collider for more accurate collision detection.
      /// \param pCollider Pointer to collider object to be set.
      void SetCollider( Phoenix::Collision::ICollider * pCollider );
      ////////////////////
      /// Returns collider object for more higher precision collision detection,
      /// \returns Pointer to collider, or this if alternative collider not set (NULL).
      Phoenix::Collision::ICollider * GetCollider();

    };
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
