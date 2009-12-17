#ifndef __PhoenixGraph_h__
#define __PhoenixGraph_h__
/////////////////////////////////////////////////////////////////
#include <stack>
#include <list>
#include <queue>
#include <algorithm>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "PhoenixAPI.h"
namespace Phoenix
{
  namespace Core
  {
    template<class NODE_TYPE> class TGraphEdge;
    template<class NODE_TYPE> class TGraphNode;
#ifndef EdgeListType
#define EdgeListType   std::list< Phoenix::Core::TGraphEdge<NODE_TYPE> * >
#endif
#ifndef NodeListType
#define NodeListType   std::list< NODE_TYPE * >
#endif
    /////////////////////////////////////////////////////////////////
    template<class NODE_TYPE>
    class  TGraph
    {
			friend class TGraphNode<NODE_TYPE>;
    protected:
      NodeListType m_lstNodes;       ///!< List of nodes.
      EdgeListType m_lstEdges;       ///!< List of edges
    public:
      ////////////////////
      /// Destructor.
      virtual ~TGraph()
      {
      	RemoveEdges();
      	RemoveNodes();
      }
      ////////////////////
      /// \return Node list reference.
      NodeListType & GetNodes() { return m_lstNodes; }

      void RegisterNode( NODE_TYPE *pNode );
      ////////////////////
      /// Deletes a node.
      /// \param pNode Node to be deleted.
      void DeleteNode( NODE_TYPE *pNode );
      ////////////////////
      /// Adds an edge between pNodeFrom and pNodeTo.
      /// Returns Pointer to edge if ok, NULL on error
      TGraphEdge<NODE_TYPE> * AddEdge( NODE_TYPE *pNodeFrom, NODE_TYPE *pNodeTo)
      {
					if ( pNodeFrom == NULL )
					{
						std::cerr << "FromNode is NULL" << std::endl;
						return NULL;
					}

					if ( pNodeTo == NULL )
					{
						std::cerr << "ToNode is NULL" << std::endl;
						return NULL;
					}

					assert ( (pNodeTo->m_pGraph == pNodeFrom->m_pGraph) && "Nodes belong to different graphs!");

					TGraphEdge<NODE_TYPE> *pEdge = new TGraphEdge<NODE_TYPE>( pNodeFrom, pNodeTo);
					pNodeFrom->GetLeavingEdges().push_back( pEdge );
					pNodeTo->GetArrivingEdges().push_back( pEdge );
					m_lstEdges.push_back(pEdge);

					return pEdge;
      }
      ////////////////////
      /// Removes an edge,
      void DeleteEdge( TGraphEdge<NODE_TYPE> *pEdge );
      ////////////////////
      void RemoveLeavingEdgesFrom( NODE_TYPE *pNode );
      ////////////////////
      void RemoveArrivingEdgesFrom( NODE_TYPE *pNode );
      ////////////////////
      /// Sets each node as unvisited
      //void  SetNodesUnvisited();
      ////////////////////
      /// Sets each edge untraversed.
      //void  SetEdgesUntraversed();
      ////////////////////
      /// Sets the color of all nodes.
      /// \param nColor The color which all nodes will be colored.
      void  SetColor( unsigned int nColor );
      ////////////////////
      template <class Type>  inline Type * CreateNode()
      {
				Type *t = new Type();
				m_lstNodes.push_back(t);
				t->m_pGraph = this;
				return t;
      }
      ////////////////////
      inline const size_t GetNodeCount() const
      {
					return m_lstNodes.size();
      }
      ////////////////////
      inline const size_t GetEdgeCount() const
      {
      	return m_lstEdges.size();
      }
    protected:
      ////////////////////
      void RemoveNodes();
      ////////////////////
      void RemoveEdges();


    };

    template <class NODE_TYPE, class TRAVELLER_TYPE >
    void TravelDF( NODE_TYPE, TRAVELLER_TYPE * pTraveller );
    ////////////////////
    /// Travels nodes starting from given node using depth-first algorithm.
    template<class NODE_TYPE>
    void TravelDF( NODE_TYPE *pStartNode );
  } // namespace Core
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#define HAS_UNTRAVERSED_EDGES( N ) ( (unsigned int)(N->GetColor()) < N->GetOutDegree())
#define IS_VISITED( N ) ( N->GetColor() > 0 )
/////////////////////////////////////////////////////////////////
template <class NODE_TYPE, class TRAVELLER_TYPE>
void
Phoenix::Core::TravelDF( NODE_TYPE *pStartNode, TRAVELLER_TYPE * pTraveller )
{
  std::stack< NODE_TYPE *> stckNodes;
  EdgeListType  lstEdges;
  TGraphEdge<NODE_TYPE> *pEdge = NULL;
  if ( pStartNode ==NULL )
  {
    std::cerr << "pStartNode is NULL" << std::endl;
    return;
  }

  pStartNode->GetGraph()->SetColor(0);         // Clear number of processed nodes
  stckNodes.push(pStartNode);

  while ( !stckNodes.empty())
  {
    NODE_TYPE *pNode = stckNodes.top();
    if ( IS_VISITED(pNode) )
    {
      if ( HAS_UNTRAVERSED_EDGES(pNode))
      {
		pEdge = lstEdges.front();
		lstEdges.pop_front();
		pEdge->GetFromNode()->SetColor(pEdge->GetFromNode()->GetColor()+1);
		stckNodes.push(pEdge->GetToNode());
      }
      else
      {
		pTraveller->Leave(pNode );
		//pNode->SetVisited(0);
		pNode->SetColor(0);
		stckNodes.pop();
      }
    }
    else
    {
      int bCulled = pTraveller->Enter(pNode);

      if ( pNode->HasLeavingEdges() && !bCulled)
      {
		lstEdges.insert( lstEdges.begin(), pNode->GetLeavingEdges().begin(),
				 pNode->GetLeavingEdges().end());
		pEdge = lstEdges.front();
		lstEdges.pop_front();
		pEdge->GetFromNode()->SetColor( pEdge->GetFromNode()->GetColor() + 1 );
		stckNodes.push( pEdge->GetToNode());
      }
      else
      {
		pTraveller->Leave(pNode);
		//pNode->SetVisited(0);
		pNode->SetColor(0);
		stckNodes.pop();
      }
    }
  }
  // Just to be safe, cleanup. stckNodes IS empty if we're here.
  lstEdges.clear();
}
///////////////////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void
Phoenix::Core::TravelDF( NODE_TYPE *pStartNode )
{
  typename std::stack< NODE_TYPE *> stckNodes;
  EdgeListType  lstEdges;
  TGraphEdge<NODE_TYPE> *pEdge = NULL;
  if ( pStartNode == NULL )
  {
    std::cerr << "pStartNode is NULL" << std::endl;
    return;
  }

  pStartNode->GetGraph()->SetColor(0);         // Clear number of processed nodes
  stckNodes.push(pStartNode);

  while ( !stckNodes.empty())
  {
    NODE_TYPE *pNode = stckNodes.top();
    if ( IS_VISITED(pNode) )
    {
      if ( HAS_UNTRAVERSED_EDGES(pNode))
      {
		pEdge = lstEdges.front();
		lstEdges.pop_front();
		// Mark unprocessed
		pEdge->GetFromNode()->SetColor(pEdge->GetFromNode()->GetColor()+1);
		stckNodes.push(pEdge->GetToNode());
      }
      else
      {
		pNode->Leave();
		pNode->SetColor(0);
		stckNodes.pop();
      }
    }
    else
    {
      bool bCulled = pNode->Enter();

      if ( pNode->HasLeavingEdges() && !bCulled)
      {
    	  lstEdges.insert( lstEdges.begin(), pNode->GetLeavingEdges().begin(),
					  pNode->GetLeavingEdges().end());
			pEdge = lstEdges.front();
			lstEdges.pop_front();
			pEdge->GetFromNode()->SetColor( pEdge->GetFromNode()->GetColor() + 1 );
			stckNodes.push( pEdge->GetToNode());
      }
      else
      {
		pNode->Leave();
		pNode->SetColor(0);
		stckNodes.pop();
      }
    }
  }
  // Just to be safe, cleanup. stckNodes IS empty if we're here.
  lstEdges.clear();
}
/////////////////////////////////////////////////////////////////
/* template<class NODE_TYPE> */
/* template<class GRAPH_EDGE> */
/* GRAPH_EDGE * */
/* Phoenix::Core::TGraph<NODE_TYPE>::AddEdge( NODE_TYPE *pNodeFrom, NODE_TYPE *pNodeTo ) */
/* { */
/*   if ( pNodeFrom == NULL ) */
/*   { */
/*     std::cerr << "FromNode is NULL" << std::endl; */
/*     return NULL; */
/*   } */

/*   if ( pNodeTo == NULL ) */
/*   { */
/*     std::cerr << "ToNode is NULL" << std::endl; */
/*     return NULL; */
/*   } */

/*   assert ( (pNodeTo->m_pGraph == pNodeFrom->m_pGraph) && "Nodes belong to different graphs!"); */

/*   GRAPH_EDGE *pEdge = new GRAPH_EDGE( pNodeFrom, pNodeTo); */
/*   pNodeFrom->GetLeavingEdges().push_back( pEdge ); */
/*   pNodeTo->GetArrivingEdges().push_back( pEdge ); */
/*   m_lstEdges.push_back(pEdge); */
/*   return pEdge; */
/* } */
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include "PhoenixGraph.tpp"

#endif

