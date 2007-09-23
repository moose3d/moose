#ifndef __PhoenixGraph_h__
#define __PhoenixGraph_h__
/////////////////////////////////////////////////////////////////
#include <stack>
#include <list>
#include <queue>
#include <algorithm>
#include <string.h>
#include <assert.h>
namespace Phoenix
{
  namespace Core
  {

    /////////////////////////////////////////////////////////////////
    // Declaration so we can use graphnode in GraphEdge
    template <typename RTTI> class CGraphNode;
    template <typename RTTI> class CGraph;
    /////////////////////////////////////////////////////////////////
    /// \brief A class for an directed edge between two nodes in a graph.
    template <typename RTTI>
    class CGraphEdge
    {
      friend class CGraphNode<RTTI>;
      friend class CGraph<RTTI>;
    protected:
      /// \brief A pointer to the node where this edge is leaving from.
      CGraphNode<RTTI> *m_pFrom;
      /// \brief A pointer to the node where this edge is arriving.
      CGraphNode<RTTI> *m_pTo;
      /// \brief is this edge traversed
      char		       m_bTraversed;
      /// \brief The cost/weight of this edge
      int		       m_iCost;
      ////////////////////
      /// The default constructor.
      CGraphEdge( )
      {
	m_pFrom = NULL;
	m_pTo   = NULL;
	m_bTraversed = 0;
      }
      ////////////////////
      /// The parametrized constructor.
      /// \param pFrom CGraphNode where edge starts
      /// \param pTo CGraphNode where edge leads to.
      CGraphEdge( CGraphNode<RTTI> *pFrom,
		  CGraphNode<RTTI> *pTo )
      {
	m_pFrom = pFrom;
	m_pTo   = pTo;
	// safety check, we do not allow links to be added to ourselves
	assert( pFrom != pTo );
	// Both nodes must be non-NULL
	assert( pFrom != NULL );
	assert( pTo != NULL );
	m_bTraversed = 0;
    
	// Both nodes must belong to same graph
	assert( pFrom->m_pGraph == pTo->m_pGraph );
    
      }
      ////////////////////
      /// \brief The destructor. 
      /// It is protected and should only be invoked by CGraph.
      ~CGraphEdge()
      {
	m_pFrom = NULL;
	m_pTo = NULL;
	m_bTraversed = 0;
      }

    public:
      ////////////////////
      /// \brief Returns the from node.
      /// \return A pointer to CGraphNode<RTTI>.
      CGraphNode<RTTI> * GetFromNode()
      {
	return m_pFrom;
      }
      ////////////////////
      /// \brief Returns the to node.
      /// \return A pointer to CGraphNode<RTTI>
      CGraphNode<RTTI> * GetToNode()
      {
	return m_pTo;
      }
      ////////////////////
      /// \brief Sets the node traveral state.
      /// \param bFlag boolean 1 for traversed, 0 for not.
      inline void SetTraversed( char bFlag )
      {
	m_bTraversed = bFlag;
      }
      ////////////////////
      /// \brief Returns the boolean for traversal state.
      /// \return char boolean 1 for traversed, 0 otherwise.
      inline char IsTraversed()
      {
	return m_bTraversed;
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

    /// The Generic Graph Node template class. 
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
    template <typename RTTI>
    class CGraphNode 
    {
      friend class CGraph<RTTI>;
      friend class CGraphEdge<RTTI>;
    protected:
      /// A list of pointers to CGraphEdge objects leaving from this node.
      std::list<CGraphEdge<RTTI> *> m_lstLeaving;
      /// A list of pointers to CGraphEdge objects arriving to this node.
      std::list<CGraphEdge<RTTI> *> m_lstArriving;
      /// A pointer to Cgraph where this node belongs to.

      /// The symbolic color value. 
      int			      m_iColor;
      /// The name for this Scene node 
      char m_sName[Phoenix::Globals::NODE_NAME_MAX_SIZE];
      /// The boolean flag indicating has this node been visited.
      int m_bVisited;
      /// The boolean flag indicating has this node been culled.
      int m_bCulled;
      /// The boolean flag indicating has this node been changed.
      int m_bChanged;
      /// The enumeration indicating the type of this node.
      RTTI m_iType;
      // A reference to the graph object which contains this baby.
      CGraph<RTTI> *      m_pGraph;  
      ////////////////////
      /// The default constructor.
      CGraphNode() 
      {

	SetVisited(0);
	m_bCulled = 0;
	m_bChanged = 0;
	m_iType = (RTTI)0; 
	m_sName[0] = '\0';
	m_pGraph = NULL;
	m_iColor = 0;
    
      }
      ////////////////////
      /// Destructor
      ~CGraphNode();

    public:
      ////////////////////
      /// Copy constructor
      CGraphNode( const CGraphNode &Node);
      ////////////////////
      /// Returns the graph where node is in.
      inline CGraph<RTTI> * GetGraph();
      ////////////////////
      /// \brief Sets the status of visited flag.
      /// \param bFlag boolean value for visited (1 = visited, 0 = unvisited )
      void SetVisited(int bFlag );
      ////////////////////
      /// \brief Returns boolean value indicating has this node been visited.
      /// \return boolean 1 if this node has been visited, 0 if not.
      int IsVisited();
      ////////////////////
      /// \brief Sets this node culling status.
      /// \param bState boolean value 1 if culled, 0 if not culled.
      void SetCulled( int bState );
      ////////////////////
      /// \brief Returns boolean value indicated is this node culled.
      /// \return boolean 1 if culled, 0 otherwise
      int IsCulled();
      ////////////////////
      /// Sets the changed flag value to bFlag.
      /// \param bFlag boolean value indicating the new state.
      void SetChanged( int bFlag );
      ////////////////////
      /// Returns value of the m_bChanged flag.
      /// \returns boolean value if state is 'changed'.
      int IsChanged();
      ////////////////////
      /// Sets the type for this node.
      /// \param iType the Run-Time Type Identification value.
      void SetType( RTTI iType );
      ////////////////////
      /// Returns the type for this node.
      /// \returns  The Run-Time Type Identification value.
      RTTI GetType();
      ////////////////////
      /// Returns the list of edges leaving from this node
      std::list< CGraphEdge<RTTI> * > &GetLeavingEdges();
      ////////////////////
      /// Returns the list of edges arriving to this node.
      std::list< CGraphEdge<RTTI> * > &GetArrivingEdges();
      ////////////////////
      /// Returns true if this node has leaving edges
      int HasLeavingEdges();
      ////////////////////
      /// returns true if this node has arriving edges
      int HasArrivingEdges();
      ////////////////////
      /// Assigns the name of the node
      void SetName( const char *szName );
      ////////////////////
      /// returns the name of the joint
      const char * GetName();
      ////////////////////
      /// \brief Removes leaving edges from this node.
      /// Edges are removed alse from nodes where they arrive.
      void RemoveLeavingEdges();
      ////////////////////
      /// \brief Removes arriving edges from this node.
      /// Edges are removed alse from nodes where they lead to.
      void RemoveArrivingEdges();
      ////////////////////
      /// \brief Adds an edge from this to given node
      /// \param pTo A pointer to CGraphNode.
      int AddEdge( CGraphNode *pTo );
      ////////////////////
      /// \brief Removes and edge leading from this node to giben node.
      /// \param pTo A pointer to CGraphNode.
      int DeleteEdgeTo( CGraphNode *pTo );
      ////////////////////
      /// \brief Sets the color of this edge.
      /// \param iColor integer color
      void SetColor( int iColor );
      ////////////////////
      /// \brief Returns the color of this edge.
      /// \returns int the color.
      int  GetColor();
      ////////////////////
      /// \brief Returns the In-degree of this node
      /// \returns unsigned int the number of arriving edges.
      unsigned int GetInDegree();
      ////////////////////
      /// \brief Returns the Out-degree of this node
      /// \returns unsigned int the number of leaving edges.
      unsigned int GetOutDegree();
    };

    /////////////////////////////////////////////////////////////////
    template <typename R>
    class CGraph
    {
    public:  

      typedef enum 
      {
	BREADTH_FIRST      = 0,   // breadth-first traversal 
	DEPTH_FIRST        = 1,   // depth-first traversal
	DEPTH_FIRST_BY_EDGES,     // depth-first, but with every 
	// edge is considered as a new branch of a tree.
      } GraphTraversalMode;

    protected:
      /// List of nodes.
      std::list< CGraphNode< R > *> m_lstNodes;
      /// List of edges
      std::list< CGraphEdge< R > *> m_lstEdges;
      /// Graph name.
      std::string m_sName;

    public:  
      ////////////////////
      /// Constructor.
      CGraph()
      {
	m_sName = "";
      }
      ////////////////////
      /// Destructor.
      ~CGraph()
      {    
	RemoveEdges();
	RemoveNodes();
      }
      ////////////////////
      /// Deletes a node.
      /// \param pNode Node to be deleted.
      void DeleteNode( CGraphNode< R > *pNode );
      ////////////////////
      /// Adds an edge between pNodeFrom and pNodeTo.
      /// Returns zero if ok, non-zero on error
      int AddEdge( CGraphNode< R > *pNodeFrom, CGraphNode< R > *pNodeTo);
      ////////////////////
      /// Removes an edge,
      void DeleteEdge( CGraphEdge<R> *pEdge );
      ////////////////////
      void RemoveLeavingEdgesFrom( CGraphNode<R> *pNode );
      ////////////////////
      void RemoveArrivingEdgesFrom( CGraphNode<R> *pNode );
      ////////////////////
      CGraphNode<R> * SeekNodeByNameAndType( const char *szName, 
					     const R iType );
      ////////////////////
      /// Sets each node as unvisited
      void  SetNodesUnvisited();
      ////////////////////
      /// Sets each edge untraversed.
      void  SetEdgesUntraversed();
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
      inline const unsigned int GetNodeCount() const
      {
	return m_lstNodes.size();
      }
      ////////////////////
      inline const unsigned int GetEdgeCount() const
      {
	return m_lstEdges.size();
      }
    protected:
      ////////////////////
      void RemoveNodes();
      ////////////////////
      void RemoveEdges();


    };
    
    template <typename R, class TravellerType> void TravelDF( CGraphNode<R> *pStartNode, TravellerType * pTraveller );
    
  } // namespace Core
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
template<typename R>
void 
Phoenix::Core::CGraph<R>::DeleteNode( CGraphNode< R> *pNode )
{
  typename std::list<CGraphNode<R> *>::iterator it;
  it = find(m_lstNodes.begin(), m_lstNodes.end(), pNode );

  if ( it != m_lstNodes.end())
  {
    RemoveLeavingEdgesFrom( pNode );
    RemoveArrivingEdgesFrom( pNode );
    m_lstNodes.erase(it);
    delete pNode;
  } 
  else 
  {
    std::cerr << "Node " << pNode << " (" << pNode->GetName() << ") " 
	      << "is not part of this graph!" << std::endl;
  }
  
}
// -----------------------------------------------------------------
template<typename R>
int 
Phoenix::Core::CGraph<R>::AddEdge( CGraphNode<R> *pNodeFrom, CGraphNode<R> *pNodeTo )
{
  if ( pNodeFrom == NULL )
  {
    std::cerr << "FromNode is NULL" << std::endl;
    return 1;
  }
  
  if ( pNodeTo == NULL )
  {
    std::cerr << "ToNode is NULL" << std::endl;
    return 1;
  }
  
  if ( pNodeTo->m_pGraph != pNodeFrom->m_pGraph )
  {
    std::cerr << "Nodes belong to different graphs!" << std::endl;
    return 1;
  }
  
  CGraphEdge<R> *pEdge = new CGraphEdge<R>( pNodeFrom, pNodeTo);
  pNodeFrom->GetLeavingEdges().push_back( pEdge );
  pNodeTo->GetArrivingEdges().push_back( pEdge );
  m_lstEdges.push_back(pEdge);
  return 0;
}
/////////////////////////////////////////////////////////////////
template<typename R>
void
Phoenix::Core::CGraph<R>::DeleteEdge( Phoenix::Core::CGraphEdge<R> *pEdge)
{
  typename std::list< CGraphEdge<R> *>::iterator it;
  it = find(m_lstEdges.begin(), m_lstEdges.end(), pEdge );

  if ( it != m_lstEdges.end())
  {
    // Delete edge from Graph edge list 
    m_lstEdges.erase(it);
    
    std::list< CGraphEdge<R> *> &lstFromEdges = 
     pEdge->GetFromNode()->GetLeavingEdges();
    std::list< CGraphEdge<R> *> &lstToEdges = 
      pEdge->GetToNode()->GetArrivingEdges();

    // delete edge from the edge list of node where it is leaving
    it = find(lstFromEdges.begin(), lstFromEdges.end(), pEdge);
    if ( it!= lstFromEdges.end()) lstFromEdges.erase(it);

    // delete edge from the edge list of node it is pointing at
    it = find(lstToEdges.begin(), lstToEdges.end(), pEdge);
    if ( it!= lstToEdges.end()) lstToEdges.erase(it);

  }
  delete pEdge;
}
/////////////////////////////////////////////////////////////////
template<typename R>
void
Phoenix::Core::CGraph<R>::RemoveNodes()
{
  while( !m_lstNodes.empty())
  {
    CGraphNode<R> *pTmpNode = m_lstNodes.front();
    m_lstNodes.pop_front();
    delete pTmpNode;
  }
}
/////////////////////////////////////////////////////////////////
template<typename R>
void
Phoenix::Core::CGraph<R>::RemoveEdges()
{
  while( !m_lstEdges.empty() )
  {
    CGraphEdge<R> *pTmpEdge = m_lstEdges.front();
    m_lstEdges.pop_front();
    delete pTmpEdge;
  }
}
/////////////////////////////////////////////////////////////////
template<typename R>
void 
Phoenix::Core::CGraph<R>::RemoveLeavingEdgesFrom( CGraphNode<R> *pNode )
{
  typename std::list<CGraphEdge<R> *>::iterator it;
  while( !pNode->GetLeavingEdges().empty())
  {
    it = pNode->GetLeavingEdges().begin();
    DeleteEdge( *it );
  }
}
/////////////////////////////////////////////////////////////////
template<typename R>
void 
Phoenix::Core::CGraph<R>::RemoveArrivingEdgesFrom( CGraphNode<R> *pNode )
{
  typename std::list<CGraphEdge<R> *>::iterator it;
  while( !pNode->GetArrivingEdges().empty())
  {
    it = pNode->GetArrivingEdges().begin();
    DeleteEdge( *it );
  }
}
/////////////////////////////////////////////////////////////////
#define STR_EQUALS( STR1, STR2 ) ( strncmp(STR1, STR2, Phoenix::Globals::NODE_NAME_MAX_SIZE) == 0 )
/////////////////////////////////////////////////////////////////
template<typename R>
Phoenix::Core::CGraphNode<R> * 
Phoenix::Core::CGraph<R>::SeekNodeByNameAndType( const char *szName, 
				     const R iType )
{
  typename std::list< CGraphNode<R> *>::iterator it;
  it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++)
  {
    CGraphNode<R> *pNode = *it;
    if ( pNode->GetType() == iType && STR_EQUALS( pNode->GetName(), szName )) 
    {
      return pNode;
    }
  }
  
  return NULL;
}

#undef STR_EQUALS
/////////////////////////////////////////////////////////////////
template <typename R>
void
Phoenix::Core::CGraph<R>::SetNodesUnvisited( )
{
  
  typename std::list<CGraphNode<R> *>::iterator it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++)
  {
    (*it)->SetVisited(0);
  }
}
/////////////////////////////////////////////////////////////////
template <typename R>
void
Phoenix::Core::CGraph<R>::SetEdgesUntraversed( )
{

  typename std::list<CGraphEdge<R> *>::iterator it = m_lstEdges.begin();

  for( ; it != m_lstEdges.end(); it++)
  {
    (*it)->SetTraversed(0);
  }
}
/////////////////////////////////////////////////////////////////
template <typename R>
void  
Phoenix::Core::CGraph<R>::SetColor( unsigned int nColor )
{
  typename std::list<CGraphNode<R> *>::iterator it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++) 
  {
    (*it)->SetColor(nColor);
  }
}
/////////////////////////////////////////////////////////////////
#define HAS_UNTRAVERSED_EDGES( N ) ( (unsigned int)(N->GetColor()) < N->GetOutDegree())
#define IS_VISITED( N ) ( N->GetColor() > 0 )
/////////////////////////////////////////////////////////////////
template <typename R>
Phoenix::Core::CGraphNode<R> * SeekNextWithType( CGraphNode<R> *pStartNode, R type )
{
  CGraphNode<R> *pCandidate = NULL;
  std::queue< CGraphNode<R> *>  queueNodes;
  std::list< CGraphEdge<R> *> listEdges;
  typename std::list< CGraphEdge<R> *>::iterator it;

  // Sanity check
  if ( pStartNode == NULL ) return NULL;
      
  // Set all nodes unvisited
  pStartNode->GetGraph()->SetColor(0);
  queueNodes.push(pStartNode);
  while( !queueNodes.empty() )
  {
    pCandidate = queueNodes.front(); queueNodes.pop();
    pCandidate->SetColor(1);
      
    if ( pCandidate->GetType() == type ) return pCandidate;
    listEdges = pCandidate->GetLeavingEdges();
    // for each neighbor do
    for( it = listEdges.begin(); it != listEdges.end(); it++)
    {
      // if neighbor is not yet visited, put it in the queue
      if ( (*it)->GetToNode()->GetColor() == 0 ) queueNodes.push( (*it)->GetToNode() );
    }
  }
  // No such node was found
  return NULL;
};
////////////////////////////////////////////////////////////////////////////
template <typename R, class TravellerType>
void  
Phoenix::Core::TravelDF( CGraphNode<R> *pStartNode, TravellerType * pTraveller )
{
  std::stack< CGraphNode<R> *> stckNodes;
  std::list< CGraphEdge<R> *>  lstEdges;
  CGraphEdge<R> *pEdge = NULL;
  if ( pStartNode ==NULL ) 
  {
    std::cerr << "pStartNode is NULL" << std::endl;
    return;
  } 

  pStartNode->GetGraph()->SetColor(0);         // Clear number of processed nodes
  stckNodes.push(pStartNode);

  while ( !stckNodes.empty())
  {
    CGraphNode<R> *pNode = stckNodes.top();
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
      //pNode->SetVisited(1);
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

/////////////////////////////////////////////////////////////////
template<typename R>
Phoenix::Core::CGraphNode<R>::CGraphNode( const CGraphNode &Node) 
{
  m_bVisited = Node.m_bVisited;
  m_bCulled = Node.m_bCulled;
  m_bChanged = Node.m_bChanged;
  m_iType = Node.m_iType; 
  m_pGraph = m_pGraph;
  strncpy(m_sName, Node.m_sName, Phoenix::Globals::NODE_NAME_MAX_SIZE);
  
  m_lstLeaving = Node.m_lstLeaving;
  m_lstArriving = Node.m_lstArriving;

}
/////////////////////////////////////////////////////////////////
template<typename R>
inline Phoenix::Core::CGraph<R> *
Phoenix::Core::CGraphNode<R>::GetGraph()
{
  return m_pGraph;
}
/////////////////////////////////////////////////////////////////
/// The destructor, should be invoked with extreme care since it only removes this node
/// not the children nor does it fix the parent pointers of the children.
template<typename R>
Phoenix::Core::CGraphNode<R>::~CGraphNode()
{

}
/////////////////////////////////////////////////////////////////
template<typename R>
void
Phoenix::Core::CGraphNode<R>::SetVisited(int bFlag )
{
  m_bVisited = bFlag;
}
/////////////////////////////////////////////////////////////////
template<typename R>
int
Phoenix::Core::CGraphNode<R>::IsVisited()
{
  return m_bVisited;
}
/////////////////////////////////////////////////////////////////
template<typename R>
void
Phoenix::Core::CGraphNode<R>::SetCulled( int bState )
{
  m_bCulled = bState;
}
/////////////////////////////////////////////////////////////////
template<typename R>
int
Phoenix::Core::CGraphNode<R>::IsCulled()
{
  return m_bCulled;
}
/////////////////////////////////////////////////////////////////
template<typename R>
void
Phoenix::Core::CGraphNode<R>::SetType( R iType )
{
  m_iType = iType;
}
/////////////////////////////////////////////////////////////////
template<typename R>
R
Phoenix::Core::CGraphNode<R>::GetType()
{
  return m_iType;
}
/////////////////////////////////////////////////////////////////
template<typename R>
std::list<Phoenix::Core::CGraphEdge<R> *> &
Phoenix::Core::CGraphNode<R>::GetLeavingEdges()
{
  return m_lstLeaving;
}
/////////////////////////////////////////////////////////////////
template<typename R>
std::list<Phoenix::Core::CGraphEdge< R > * > &
Phoenix::Core::CGraphNode<R>::GetArrivingEdges()
{
  return m_lstArriving;
}
/////////////////////////////////////////////////////////////////
template<typename R>
int
Phoenix::Core::CGraphNode<R>::HasLeavingEdges()
{
  return (m_lstLeaving.size() > 0 );
}
/////////////////////////////////////////////////////////////////
template<typename R>
int
Phoenix::Core::CGraphNode<R>::HasArrivingEdges()
{
  return (m_lstArriving.size() > 0 );
}
/////////////////////////////////////////////////////////////////
template<typename R>
void 
Phoenix::Core::CGraphNode<R>::SetName( const char *szName )
{
  strncpy(m_sName,szName,Phoenix::Globals::NODE_NAME_MAX_SIZE);
}

/////////////////////////////////////////////////////////////////
template<typename R>
const char * 
Phoenix::Core::CGraphNode<R>::GetName()
{
  return m_sName;
}
/////////////////////////////////////////////////////////////////
template<typename R>
void 
Phoenix::Core::CGraphNode<R>::SetChanged( int bFlag )
{
  m_bChanged = bFlag;
}
/////////////////////////////////////////////////////////////////
// Returns value of the m_bChanged flag
template<typename R>
int
Phoenix::Core::CGraphNode<R>::IsChanged()
{
  return m_bChanged;
}
/////////////////////////////////////////////////////////////////
template<typename R>
int 
Phoenix::Core::CGraphNode<R>::AddEdge( CGraphNode<R> *pTo )
{
  return m_pGraph->AddEdge( this, pTo );
}
/////////////////////////////////////////////////////////////////
template<typename R>
int 
Phoenix::Core::CGraphNode<R>::DeleteEdgeTo( CGraphNode<R> *pTo )
{
  typename std::list<CGraphEdge<R> *>::iterator it = this->GetLeavingEdges().begin();
  // For each leaving edge in this 
  for(;it!=this->GetLeavingEdges().end();it++)
  {
    // if edge points to pTo, we delete it.
    if ( (*it)->GetToNode() == pTo )
    {
      CGraphEdge<R> *pEdge = *it;
      m_pGraph->DeleteEdge( pEdge );
      break;
    }
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
template<typename R>
void 
Phoenix::Core::CGraphNode<R>::RemoveLeavingEdges()
{
  m_pGraph->RemoveLeavingEdgesFrom( this );
}
/////////////////////////////////////////////////////////////////
template<typename R>
void 
Phoenix::Core::CGraphNode<R>::RemoveArrivingEdges()
{
  m_pGraph->RemoveArrivingEdgesFrom( this );
}
/////////////////////////////////////////////////////////////////
template<typename R>
void
Phoenix::Core::CGraphNode<R>::SetColor( int iColor )
{
  m_iColor = iColor;
}
/////////////////////////////////////////////////////////////////
template<typename R>
int 
Phoenix::Core::CGraphNode<R>::GetColor()
{
  return m_iColor;
}
/////////////////////////////////////////////////////////////////
template<typename R>
unsigned int
Phoenix::Core::CGraphNode<R>::GetInDegree()
{
  return m_lstArriving.size();
}
/////////////////////////////////////////////////////////////////
template<typename R>
unsigned int
Phoenix::Core::CGraphNode<R>::GetOutDegree()
{
  return m_lstLeaving.size();
}
/////////////////////////////////////////////////////////////////
#endif
