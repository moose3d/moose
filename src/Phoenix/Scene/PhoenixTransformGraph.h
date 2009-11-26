#ifndef __TransformGraph_h__
#define __TransformGraph_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixTransform.h>
#include <PhoenixGraph.h>
#include <PhoenixAPI.h>
#include <PhoenixResourceManager.h>
#include <iostream>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Scene
  {
  	class CTransformable;
  	class CGameObject;
		/////////////////////////////////////////////////////////////////
    /// Transform node template, allows several different objects to be 
    /// attached to each other via typing mechanism.
    class PHOENIX_API CTransformNode : public Phoenix::Core::TGraphNode<Phoenix::Scene::CTransformNode>
    {
      friend class Phoenix::Core::TGraph<CTransformNode>;
    public:
      ~CTransformNode();
      /// For adding new edges without compiler errors.
      Phoenix::Core::TGraphEdge<CTransformNode> * AddEdge( CTransformNode *pTo );
      ////////////////////
      /// Returns pointer to a transformable object that is affected by this transform node.
      virtual Phoenix::Scene::CTransformable * GetTransformable() = 0;
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
		class PHOENIX_API CTransformable
		{
		protected:
			Phoenix::Math::CTransform m_LocalTransform; ///!< Local transform
			Phoenix::Math::CTransform m_WorldTransform; ///!< Combined transform with parent's transform.
			bool m_bChanged; ///!< Has this transform been changed (indicates whether world transform and children should be updated).
			Phoenix::Scene::CTransformNode *m_pTransformNode;
		public:
			CTransformable();
			virtual ~CTransformable();
			////////////////////
			/// Returns reference to local transform.
			/// \returns Local transform
			Phoenix::Math::CTransform & GetLocalTransform();
			////////////////////
			/// Returns reference to local transform.
			/// \returns Local transform
			const Phoenix::Math::CTransform & GetLocalTransform() const;
			////////////////////
			/// Returns reference to world transform.
			/// \returns World transform
			Phoenix::Math::CTransform & GetWorldTransform();
			////////////////////
			/// Returns reference to world transform.
			/// \returns World transform
			const Phoenix::Math::CTransform & GetWorldTransform() const;
			////////////////////
			/// Assigns world transform.
			/// \param rTransform transform to be set as world
			void SetWorldTransform( const Phoenix::Math::CTransform & rTransform );
			////////////////////
			/// Assigns local transform.
			/// \param rTransform transform to be set as local
			void SetLocalTransform( const Phoenix::Math::CTransform & rTransform );
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
			Phoenix::Scene::CTransformNode * GetTransformNode();
			////////////////////
			/// Sets transform node for this transformable. TransformNode will not be released upon object destruction.
			/// \param pNode CTransformNode to be set.
			void SetTransformNode( Phoenix::Scene::CTransformNode *pNode );
			////////////////////
			/// For easier usage, reparents transform node of this transformable.
			/// \param pTransformable Transformable object that must have a transform node.
			void Reparent( Phoenix::Scene::CTransformable *pTransformable );
		};
    ///////////////////////////////////////////////////////////////////////////
		class CObjectTransform : public CTransformNode,
                             public Phoenix::Core::CHandled<Phoenix::Scene::CGameObject>
    {
    public:
      Phoenix::Scene::CTransformable * GetTransformable();
    };
    ///////////////////////////////////////////////////////////////////////////
    class CPlainTransform : public CTransformNode,
                          public Phoenix::Scene::CTransformable

    {
    public:
      Phoenix::Scene::CTransformable * GetTransformable()
      {
        return this;
      }
    };
    typedef std::list<CTransformable*> TransformableList;
    ///////////////////////////////////////////////////////////////////////////
    class CTransformGraph : public TGraph<CTransformNode>
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
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif

