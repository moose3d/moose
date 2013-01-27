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

#ifndef __TransformGraph_h__
#define __TransformGraph_h__
/////////////////////////////////////////////////////////////////
#include <MooseTransform.h>
#include <MooseGraphEdge.h>
#include <MooseGraphNode.h>
#include <MooseGraph.h>
#include <MooseAPI.h>
#include <MooseResourceManager.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Scene
  {
  	class CTransformable;
  	class CGameObject;
		/////////////////////////////////////////////////////////////////
    /// Transform node template, allows several different objects to be 
    /// attached to each other via typing mechanism.
    class MOOSE_API CTransformNode : public Moose::Core::TGraphNode<Moose::Scene::CTransformNode>
    {
      friend class Moose::Core::TGraph<CTransformNode>;
    public:
      virtual ~CTransformNode();
      /// For adding new edges without compiler errors.
      Moose::Core::TGraphEdge<CTransformNode> * AddEdge( CTransformNode *pTo );
      ////////////////////
      /// Returns pointer to a transformable object that is affected by this transform node.
      virtual Moose::Scene::CTransformable * GetTransformable() = 0;
      ////////////////////
      void Reparent( CTransformNode * pNewParent );
      ////////////////////
      CTransformNode * GetParentTransformNode();
      ////////////////////
      bool Enter();
      ////////////////////
      void Leave();
    };
    /////////////////////////////////////////////////////////////////
		/// Transformable base class
		class MOOSE_API CTransformable
		{
		protected:
			Moose::Math::CTransform m_LocalTransform; ///!< Local transform
			Moose::Math::CTransform m_WorldTransform; ///!< Combined transform with parent's transform.
			bool m_bChanged; ///!< Has this transform been changed (indicates whether world transform and children should be updated).
			Moose::Scene::CTransformNode *m_pTransformNode;
		public:
			CTransformable();
			virtual ~CTransformable();
			////////////////////
			/// Returns reference to local transform.
			/// \returns Local transform
			Moose::Math::CTransform & GetLocalTransform();
			////////////////////
			/// Returns reference to local transform.
			/// \returns Local transform
			const Moose::Math::CTransform & GetLocalTransform() const;
			////////////////////
			/// Returns reference to world transform.
			/// \returns World transform
			Moose::Math::CTransform & GetWorldTransform();
			////////////////////
			/// Returns reference to world transform.
			/// \returns World transform
			const Moose::Math::CTransform & GetWorldTransform() const;
			////////////////////
			/// Assigns world transform.
			/// \param rTransform transform to be set as world
			void SetWorldTransform( const Moose::Math::CTransform & rTransform );
			////////////////////
			/// Assigns local transform.
			/// \param rTransform transform to be set as local
			void SetLocalTransform( const Moose::Math::CTransform & rTransform );
			////////////////////
			/// Is this transformable been updated.
			/// \returns true if changed, false otherwise.
			bool IsChanged() const;
			////////////////////
			/// Set change flag.
			/// \bFlag true for changed, false for unchanged.
			void SetChanged(bool bFlag );
			////////////////////
			/// Gets called whenever change is reseted, ie. set to false.
			virtual void PostTransformUpdate();
			////////////////////
			/// Returns transform node for this transformable.
			/// \returns Pointer to CTransformNode or NULL if not set.
			Moose::Scene::CTransformNode * GetTransformNode();
			////////////////////
			/// Sets transform node for this transformable. TransformNode will not be released upon object destruction.
			/// \param pNode CTransformNode to be set.
			void SetTransformNode( Moose::Scene::CTransformNode *pNode );
			////////////////////
			/// For easier usage, reparents transform node of this transformable.
			/// \param pTransformable Transformable object that must have a transform node.
			void Reparent( Moose::Scene::CTransformable *pTransformable );
		};
    ///////////////////////////////////////////////////////////////////////////
		class CObjectTransform : public CTransformNode,
                             public Moose::Core::CHandled<Moose::Scene::CGameObject>
    {
    public:
      virtual ~CObjectTransform() { TGraphNodeDestroy(); }
      Moose::Scene::CTransformable * GetTransformable();
    };
    ///////////////////////////////////////////////////////////////////////////
    class CPlainTransform : public CTransformNode,
                          public Moose::Scene::CTransformable

    {
    public:
      virtual ~CPlainTransform() { TGraphNodeDestroy(); }
      Moose::Scene::CTransformable * GetTransformable()
      {
        return this;
      }
    };
    typedef std::list<CTransformable*> TransformableList;
    ///////////////////////////////////////////////////////////////////////////
    class CTransformGraph : public Moose::Core::TGraph<CTransformNode>
    {
    protected:
      CPlainTransform *     m_pRoot;
    public:

      CTransformGraph();
      virtual ~CTransformGraph();
      ///////////////////
      /// \returns root node of transform graph.
      CTransformNode * GetRoot();
      ///////////////////
      /// Updates nodes as necessary.
      void Update();
      ///////////////////
      // Adds object into transform graph. It must be  inherited from
      // CTransformable.
      CObjectTransform * Insert( CGameObject * pObject );
      ///////////////////
			/// Removes an object from graph (with its children).
      /// \param pSubRoot Subtree root node which will be removed (including).
      void RemoveSubtree( CTransformable *pSubRoot );
      ///////////////////
      /// Collects subtree in graph (with its children).
      /// \param pSubRoot Subtree root node from which subtree will be collected (included).
      /// \param subtree list of Transformables forming a subtree.
      void CollectSubtree( CTransformable *pSubRoot, TransformableList & subtree  );
    };
    ///////////////////////////////////////////////////////////////////////////
  } // namespace Scene
} // namespace Moose
/////////////////////////////////////////////////////////////////
#endif

