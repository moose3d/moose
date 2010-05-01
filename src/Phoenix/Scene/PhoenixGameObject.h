#ifndef __PhoenixGameObject_h__
#define __PhoenixGameObject_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixMath.h"
#if !defined(PHOENIX_APPLE_IPHONE)
#include "PhoenixAIScript.h"
#endif
#include "PhoenixTransformGraph.h"
#include "PhoenixSphereBound.h"
#include "PhoenixRenderable.h"
#include "PhoenixRenderableProperty.h"
#include "PhoenixAPI.h"
#include "PhoenixTagged.h"
#include "PhoenixSphereCollider.h"
/////////////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <list>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
  	/////////////////////////////////////////////////////////////////
  	enum Tags {
  		NOT_TAGGED, 	///!< Default.
  		LIGHT_TAG = 1,  	///!< Lights have their own tags
  		COLLIDER_TAG = 2, ///!< Areas that are used only for collision checking.
  		USER_TAG = 4     ///!< First allowed user-specified tag value.
  	};
  	/////////////////////////////////////////////////////////////////
  	class CSpatialGraph;
    /////////////////////////////////////////////////////////////////
    /// GameObject class; base for every object in a game.
    class PHOENIX_API CGameObject : public Phoenix::Scene::CTransformable,
																		public Phoenix::Graphics::CRenderableProperty,
																		public Phoenix::Core::CTagged,
																		public Phoenix::Collision::CSphereCollider,
																		public Phoenix::Core::CNamed,
																		public Phoenix::Core::CHandled<Phoenix::Scene::CGameObject>,
#if !defined(PHOENIX_APPLE_IPHONE)
                                                                        public Phoenix::AI::CAIObject,
#endif
                                                                        public Phoenix::Core::IUpdateable,
																		public Phoenix::Core::CEnableable
		{
    protected:

      unsigned int                     m_nSpatialIndex; ///!< In which spatial index of spatial graph this object is in.
      Phoenix::Collision::ICollider *  m_pCollider; 		///!< Specialized collider instead of Sphere.
      std::list<CGameObject *> 				 m_lstColliders; 	///!< List of possible colliders.
    public:
      ////////////////////
      /// Constructor.
      CGameObject();
      ////////////////////
      /// Destructor.
      virtual ~CGameObject();
      ////////////////////
      /// Initializes the game object after creation. By default, it loads the script file.
      virtual void Init();
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
      bool Intersects( const Phoenix::Math::CRay & ray, float *pfDistance = NULL ) const;
      bool Intersects( const Phoenix::Collision::ICollider & collider ) const;
      bool Intersects( const Phoenix::Volume::CCapsule & capsule ) const;
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
      ////////////////////
      /// Updates game object.
      /// \param fSecondsPassed Seconds passed since last Update().
      void Update( float fSecondsPassed );
      ////////////////////
      /// Updates neighbour list, should be called infrequently.
      /// \param fRadius Radius of sphere around game object which encloses possible colliders.
      /// \param graph Reference to spatial graph used in search.
      void UpdateColliders( float fRadius, Phoenix::Scene::CSpatialGraph & graph );
      ////////////////////
      /// Checks collisions between neighbors and enqueues messages for scripts accordingly.
      void CheckCollisions();
    };
    typedef std::list<CGameObject *> GameObjectList;
  }; // namespace Scene
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
