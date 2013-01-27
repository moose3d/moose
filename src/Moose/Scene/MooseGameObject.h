/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __MooseGameObject_h__
#define __MooseGameObject_h__
/////////////////////////////////////////////////////////////////
#include "MooseMath.h"
#include "MooseAIScript.h"
#include "MooseTransformGraph.h"
#include "MooseSphereBound.h"
#include "MooseRenderable.h"
#include "MooseRenderableProperty.h"
#include "MooseAPI.h"
#include "MooseTagged.h"
#include "MooseSphereCollider.h"
/////////////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <list>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace AI
  {
    class CCollisionEnter;
    class CCollisionStay;
    class CCollisionExit;
  }
  namespace Scene
  {
  	/////////////////////////////////////////////////////////////////
  	enum Tags {
      NOT_TAGGED, 	///!< Default.
      LIGHT_TAG = 1,  	///!< Lights have their own tags
      COLLIDER_TAG = 2, ///!< Object is colliding object.
      CAMERA_TAG = 4,  ///!< Object is camera.
      USER_TAG = 8     ///!< First allowed user-specified tag value.
  	};
  	/////////////////////////////////////////////////////////////////
  	class CSpatialGraph;
    class CGameObject;
    typedef std::list<CGameObject *> GameObjectList;
    typedef std::set<CGameObject *> GameObjectSet;
    /////////////////////////////////////////////////////////////////
    /// GameObject class; base for every object in a game.
    class MOOSE_API CGameObject : public Moose::Scene::CTransformable,
                                  public Moose::Graphics::CRenderableProperty,
                                  public Moose::Core::CTagged,
                                  public Moose::Collision::CSphereCollider,
                                  public Moose::Core::CNamed,
                                  public Moose::Core::CHandled<Moose::Scene::CGameObject>,
                                  public Moose::AI::CAIObject,
                                  public Moose::Core::IUpdateable,
                                  public virtual Moose::Core::CEnableable
    {
    protected:
      friend class Moose::AI::CMessageQueue;
      unsigned int                     m_nSpatialIndex; ///!< In which spatial index of spatial graph this object is in.
      Moose::Collision::ICollider *  m_pCollider; 		///!< Specialized collider instead of Sphere.
      int m_iUserInfo;
    protected:
      void OnCollisionEnter_( const Moose::AI::CCollisionEnter *pMsg ) ;
      void OnCollisionStay_( const Moose::AI::CCollisionStay *pMsg ) ;
      void OnCollisionExit_( const Moose::AI::CCollisionExit *pMsg ) ;
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
      bool Intersects( const Moose::Volume::CSphere & sphere ) const;
      bool Intersects( const Moose::Graphics::CFrustum & frustum ) const;
      bool Intersects( const Moose::Volume::COrientedBox & box ) const;
      bool Intersects( const Moose::Math::CVector3<float> & vPoint ) const;
      bool Intersects( const Moose::Math::CRay & ray, float *pfDistance = NULL ) const;
      bool Intersects( const Moose::Collision::ICollider & collider ) const;
      bool Intersects( const Moose::Volume::CCapsule & capsule ) const;
      ////////////////////
      /// Returns bounding sphere transformed using current world transform.
      /// \return Bounding sphere in world coordinates.
      virtual Moose::Volume::CSphere GetWorldBoundingSphere() const;
      ////////////////////
      // In case we need to optimize world bounding sphere computation by adding cache for sphere,
      // this comes in handy.
      //void PostTransformUpdate();
      ////////////////////
      /// Sets collider for more accurate collision detection.
      /// \param pCollider Pointer to collider object to be set.
      void SetCollider( Moose::Collision::ICollider * pCollider );
      ////////////////////
      /// Returns collider object for more higher precision collision detection,
      /// \returns Pointer to collider, or this if alternative collider not set (NULL).
      Moose::Collision::ICollider * GetCollider();
      ////////////////////
      /// Updates game object.
      /// \param fSecondsPassed Seconds passed since last Update().
      void Update( float fSecondsPassed );
      virtual void OnCollisionEnter( Moose::Scene::CGameObject *pCollider );
      virtual void OnCollisionStay( Moose::Scene::CGameObject *pCollider );
      virtual void OnCollisionExit( Moose::Scene::CGameObject *pCollider );
      void SetUserInfo( int iValue ) { m_iUserInfo = iValue; }
      int & GetUserInfo()  { return m_iUserInfo;}
#if !defined(SWIG)
      const int GetUserInfo() const { return m_iUserInfo; }
#endif
    };
  }; // namespace Scene
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
