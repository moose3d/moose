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
#include "PhoenixCollider.h"
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
									public Phoenix::Volume::CSphereBound,
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
