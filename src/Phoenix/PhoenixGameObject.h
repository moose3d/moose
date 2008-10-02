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

  namespace Scene
  {
    /////////////////////////////////////////////////////////////////
    class PHOENIX_API CSphereBound
    {
    private:
      /// Model bounding sphere
      Phoenix::Volume::CSphere		m_BoundingSphere;
    public:
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
    class PHOENIX_API CGameObject : public Phoenix::Math::CTransformable
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
      ////////////////////
      /// Calculates world bounding box.
      /// \param box Oriented box where transformed box is stored.
      void CalculateWorldBoundingBox( Phoenix::Volume::COrientedBox & box ) const;
      ////////////////////
      /// Calculates world bounding sphere.
      /// \param sphere Sphere where transformed sphere is stored.
      void CalculateWorldBoundingSphere( Phoenix::Volume::CSphere & sphere ) const;
    };
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
