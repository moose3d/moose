#ifndef __TransformGraph_h__
#define __TransformGraph_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixTransform.h>
#include <PhoenixGraph.h>
#include "PhoenixAPI.h"
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
      /// Returns pointer to a transformable object that is affected by this transformnode.
      virtual Phoenix::Math::CTransformable * GetTransformable() = 0;
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
  } // namespace Scene
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif

