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
    /////////////////////////////////////////////////////////////////
    /// Transform node template, allows several different objects to be 
    /// attached to each other via typing mechanism.
    class PHOENIX_API CTransformNode : public Phoenix::Core::TGraphNode<Phoenix::Scene::CTransformNode>
    {
      friend class Phoenix::Core::TGraph<CTransformNode>;
    public:
      /// For adding new edges without compiler errors.
      Phoenix::Core::TGraphEdge<CTransformNode> * AddEdge( CTransformNode *pTo )
      {
    	  return m_pGraph->AddEdge( this, pTo );
      }
      ////////////////////
      /// Returns pointer to a transformable object that is affected by this transform node.
      virtual Phoenix::Math::CTransformable * GetTransformable() = 0;
      ////////////////////
      void Reparent( CTransformNode * pNewParent )
      {
    	  CTransformNode *pCurrentParent = GetArrivingEdges().front()->GetFromNode();
    	  // This removes all edges between current parent and this
    	  DeleteAllEdgesTo(pCurrentParent);
    	  // Remove possibly existing edges between new parent and this
    	  pNewParent->DeleteAllEdgesTo(this);
    	  // Add edges between new parent and this
    	  pNewParent->AddEdge( this );

      }
      ////////////////////
      bool Enter()
      {
					Phoenix::Math::CTransformable *pThis   = GetTransformable();
					Phoenix::Math::CTransformable *pParent = NULL;

					if ( pThis == NULL)
					{
						std::cerr << "Transformable == NULL" << std::endl;
						return 0;
					}

					if ( HasArrivingEdges() )
					{
						pParent = GetArrivingEdges().front()->GetFromNode()->GetTransformable();

						// If there is no ship, skip handling and use local transform.
						if ( pParent == NULL )
						{
							if ( pThis->IsChanged() )
								pThis->SetWorldTransform( pThis->GetLocalTransform());
							return 0;
						}

						if ( pParent->IsChanged() || pThis->IsChanged() )
						{
							pThis->SetChanged(true);
							Multiply( pParent->GetWorldTransform(), pThis->GetLocalTransform(), pThis->GetWorldTransform() );
						}

					}
					else
					{
						if ( pThis->IsChanged() ) pThis->SetWorldTransform( pThis->GetLocalTransform());
					}
					return 0;
      }
      ////////////////////
      void Leave()
      {
				Phoenix::Math::CTransformable *pTmp = GetTransformable();
				if( pTmp != NULL ) pTmp->SetChanged(false);
      }
    };
    ///////////////////////////////////////////////////////////////////////////
    template<class TYPE>
    class CObjectTransform : public CTransformNode,
                              public Phoenix::Core::CHandled<TYPE>
    {
    public:
      Phoenix::Math::CTransformable * GetTransformable()
      {
        return static_cast<Phoenix::Math::CTransformable *>( *(this->GetObjectHandle()) );
      }
    };
    ///////////////////////////////////////////////////////////////////////////
    class CPlainTransform : public CTransformNode,
                          public Phoenix::Math::CTransformable

    {
    public:
      Phoenix::Math::CTransformable * GetTransformable()
      {
        return this;
      }
    };
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
      // Adds object into transform graph.
      template <class TYPE > CObjectTransform<TYPE> * Insert( TYPE * pObject )
      {
      	CObjectTransform<TYPE> *pTR = new CObjectTransform<TYPE>();
      	RegisterNode(pTR);

        assert ( !pObject->GetObjectHandle().IsNull() &&
								"Object handle is null, this makes adding to transform graph"
								"via HANDLE pretty difficult. Attach handle to object via "
								"ResourceManager after creation immediately "
								"ResourceManager::Create( Object, name, HANDLE)");
        pTR->GetObjectHandle() = pObject->GetObjectHandle();
        GetRoot()->AddEdge( pTR );
        return pTR;
}
    };
    ///////////////////////////////////////////////////////////////////////////
  } // namespace Scene
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif

