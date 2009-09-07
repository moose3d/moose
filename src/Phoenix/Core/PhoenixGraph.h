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
    
    /////////////////////////////////////////////////////////////////
    // Declaration so we can use graphnode in GraphEdge

    template<class NODE_TYPE> class TGraph;
    template<class NODE_TYPE> class TGraphNode;
    /////////////////////////////////////////////////////////////////
    /// A class for an directed edge between two nodes in a graph.
    /// This is a template, because we wish to prevent dynamic_casting during
    /// traversal of nodes. Each node is accessed via an edge, and an edge must return a 
    /// specific type for allowing access to members of a subclass without casting (return subclass type).
    /// 
    template<class NODE_TYPE>
    class PHOENIX_API TGraphEdge
    {
      friend class TGraphNode<NODE_TYPE>;
      friend class TGraph<NODE_TYPE>;
    protected:

      NODE_TYPE *m_pFrom;       /// !< A pointer to the node where this edge is leaving from.
      NODE_TYPE *m_pTo;         /// !< A pointer to the node where this edge is arriving.
      int        m_iCost; ///!< cost/weight of this edge
      ////////////////////
      /// The default constructor.
      TGraphEdge( )
      {
		m_pFrom = NULL;
		m_pTo   = NULL;
      }
      ////////////////////
      /// The parametrized constructor.
      /// \param pFrom CGraphNode where edge starts
      /// \param pTo CGraphNode where edge leads to.
      TGraphEdge( NODE_TYPE *pFrom, NODE_TYPE *pTo );

      ////////////////////
      /// \brief The destructor. 
      /// It is protected and should only be invoked by CGraph.
      virtual ~TGraphEdge()
      {
		m_pFrom = NULL;
		m_pTo = NULL;
      }

    public:
      ////////////////////
      /// \brief Returns the from node.
      /// \return A pointer to CGraphNode
      NODE_TYPE * GetFromNode()      
      {	
	return m_pFrom;      
      }
      ////////////////////
      /// \brief Returns the to node.
      /// \return A pointer to CGraphNode
      NODE_TYPE * GetToNode()
      {
	return m_pTo;
      }
      ////////////////////
      /// \brief Returns the cost of this edge.
      /// \return  the cost of this edge as int
      inline int GetCost()
      {
    	  return m_iCost;
      }
      ////////////////////
      /// \brief Assigns the cost for this edge.
      /// \param iCost integer value for the cost.
      void SetCost( int iCost )
      {
    	  m_iCost = iCost;
      }

    };


    /*   Root Node */
    /*     /    |   |_  */
    /*    /     |     | */
    /*  Cam1   Cam2   Cam3 */
    /*   \_     |      | */
    /*     \_   |      | */
    /*       \  |     Group 2 */
    /*     Group 1     /  | */
    /*       / |      /   | */
    /*      /  |    Obj3  Obj4 */
    /*   Obj1 Obj2 */

    /// The Generic Graph Node  class. 
    /// 
    ///  In addition, the graph SHOULD resemble more of a tree than a fully-fledged
    ///  graph, but it is handy to have possibility for configurations like one
    ///  below: 
    ///  \dot
    ///  digraph RenderGraph {
    ///     root [ label="Root node" ];
    ///     cam1 [ label="Camera 1" ];
    ///     cam2 [ label="Camera 2" ];
    ///     cam3 [ label="Camera 3" ];
    ///     group1 [ label="Group 1" ];
    ///     group2 [ label="Group 2" ];
    ///     obj1 [label="Object 1" ];
    ///     obj2 [label="Object 2" ];
    ///     obj3 [label="Object 3" ];
    ///     obj4 [label="Object 4" ];
    ///
    ///     root -> cam1;
    ///     root -> cam2;
    ///     root -> cam3;
    ///     cam1 -> group1;
    ///     cam2 -> group1;
    ///     cam3 -> group2;
    ///     group1 -> obj1;
    ///     group1 -> obj2;
    ///     group2 -> obj3;
    ///     group2 -> obj4;
    ///  }
    ///
    ///  \enddot
    /// Where Cameras 1 and 2 have can have the same objects (Obj1 & Obj2) drawn, 
    /// whereas Objects Obj3 and Obj4 are visible only using Camera 3.

#define EdgeListType   std::list< TGraphEdge<NODE_TYPE> * >
#define NodeListType   std::list< NODE_TYPE * >

    template< class NODE_TYPE>
    class TGraphNode
    {
      friend class TGraphEdge<NODE_TYPE>;
    protected:
      EdgeListType m_lstLeaving;     ///!< A list of pointers to CGraphEdge objects leaving from this node.
      EdgeListType m_lstArriving;    ///!< A list of pointers to CGraphEdge objects arriving to this node.
      int	   m_iColor;         ///!< Symbolic color value. 


      TGraph<NODE_TYPE> *      m_pGraph;   ///!< A reference to the graph object which contains this baby.
      ////////////////////
      /// Default constructor.
      TGraphNode()
      {
		m_pGraph = NULL;
		m_iColor = 0;
      }
      ////////////////////
      /// Destructor. Should be invoked with extreme care since it only removes this node
      /// not the children nor does it fix the parent pointers of the children.
      virtual ~TGraphNode() {}
    public:
      ////////////////////
      /// Copy constructor
      TGraphNode( const TGraphNode &Node );
      ////////////////////
      /// Returns the graph where node is in.
      TGraph<NODE_TYPE> * GetGraph();
      ////////////////////
      /// Returns the list of edges leaving from this node
      EdgeListType &GetLeavingEdges();
      ////////////////////
      /// Returns the list of edges arriving to this node.
      EdgeListType &GetArrivingEdges();
      ////////////////////
      /// Returns true if this node has leaving edges
      int HasLeavingEdges();
      ////////////////////
      /// returns true if this node has arriving edges
      int HasArrivingEdges();
      ////////////////////
      ///  Removes leaving edges from this node.
      /// Edges are removed alse from nodes where they arrive.
      void RemoveLeavingEdges();
      ////////////////////
      ///  Removes arriving edges from this node.
      /// Edges are removed alse from nodes where they lead to.
      void RemoveArrivingEdges();
      ////////////////////
      ///  Adds an edge from this to given node
      /// \param pTo A pointer to CGraphNode.
      Phoenix::Core::TGraphEdge<NODE_TYPE> * AddEdge( NODE_TYPE *pTo )
      {
	return m_pGraph->AddEdge( this, pTo );
      }
      ////////////////////
      ///  Removes and edge leading from this node to given node.
      /// \param pTo A pointer to CGraphNode.
      /// \returns true if an edge was found and removed. False otherwise.
      bool DeleteEdgeTo( NODE_TYPE *pTo );
      ////////////////////
      ///  Removes and edge leading from this node to given node.
	  /// \param pTo A pointer to CGraphNode.
	  void DeleteAllEdgesTo( NODE_TYPE *pTo );
      ////////////////////
      ///  Sets the color of this edge.
      /// \param iColor integer color
      void SetColor( int iColor );
      ////////////////////
      ///  Returns the color of this edge.
      /// \returns int the color.
      int  GetColor();
      ////////////////////
      ///  Returns the In-degree of this node
      /// \returns the number of arriving edges.
      size_t GetInDegree();
      ////////////////////
      ///  Returns the Out-degree of this node
      /// \returns the number of leaving edges.
      size_t GetOutDegree();
      ////////////////////
      /// Executed when node is entered the first time during current pass.
      /// \returns true if children should not be processed.
      /// \returns false if children should be processed.
      virtual bool Enter() = 0;
      ////////////////////
      /// Executed when node (and possibly its children) have been processed.
      virtual void Leave() = 0;
    };
    /////////////////////////////////////////////////////////////////
    template<class NODE_TYPE>
    class  TGraph
    {
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

#include "PhoenixGraph.cpp"

#endif
      
