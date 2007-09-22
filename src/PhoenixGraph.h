/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/

// -------------------------------------------------------------------------
// $Id: GSE_graph.h,v 1.23 2007/03/20 12:20:13 entity Exp $
// desc		: header and implementation for template graph class.
// author	: AG/eNtity 
// history	: 
//		$Log: GSE_graph.h,v $
//		Revision 1.23  2007/03/20 12:20:13  entity
//		SeekNextWithType added
//
//		Revision 1.22  2007/03/20 10:42:36  entity
//		removed graphnodetraveller interface completely,
//		commented code cleaned up,
//		removed traverse (never used & tested )
//
//		Revision 1.21  2007/03/20 10:35:07  entity
//		removed deprecated seek method
//
//		Revision 1.20  2007/03/12 14:52:32  entity
//		Separated TravelDF and made it template function so GraphNodeTraveller class became unnecessary
//
//		Revision 1.19  2007/03/12 13:55:22  entity
//		Removed virtual Destroy()
//
//		Revision 1.18  2007/03/11 12:04:25  entity
//		GetGraph() added to GraphNode
//
//		Revision 1.17  2007/03/09 12:27:20  entity
//		GetNodeCount and GetEdgeCount added
//
//		Revision 1.16  2007/03/08 20:24:56  entity
//		CreateNode<> template instead of AddNode()
//
//		Revision 1.15  2007/03/05 11:25:20  entity
//		GraphNodeProcessor to GraphNodeTraveller; Removed GraphNode::Render(),
//		removed GSE_Renderer-references.
//
//		Revision 1.14  2007/03/05 10:11:29  entity
//		better doxygen comments.
//
//		Revision 1.13  2007/03/04 21:49:49  entity
//		GraphNodeProcessor added, SetColor() for Graph, some fixes for RenderDF()
//
//		Revision 1.12  2007/03/04 10:55:48  entity
//		GSE_GraphNode: GetInDegree, GetOutDegree added +
//		RenderDF() non-recursive graph traversal algorithm first version
//
//		Revision 1.11  2007/02/26 20:27:46  entity
//		doxygen comments added
//
//		Revision 1.10  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.9  2006/10/14 20:02:58  entity
//		Some updates
//
//		Revision 1.8  2006/10/14 19:01:47  entity
//		SetName(): parameter as const char *
//
//		Revision 1.7  2006/10/14 16:25:39  entity
//		Added couple #includes to make Graphs compile ok
//
//		Revision 1.6  2006/10/13 14:38:25  entity
//		#ifdef DEBUG replaced with GSE_DEBUG macro, code cleanups
//
//		Revision 1.5  2006/10/10 08:17:44  entity
//		cleaned up some comments
//
//		Revision 1.4  2006/10/08 09:40:13  entity
//		fixed GraphNode->Destroy() not to cause segfault
//
//		Revision 1.3  2006/10/08 08:34:25  entity
//		better Graph<R>::AddEdge() error handling
//
// -------------------------------------------------------------------------
#ifndef __GSE_graph__
#define __GSE_graph__

#include "GSE_interfaces.h"
#include "GSE_Logger.h"
#include "GSE_debug.h"

#include <stack>
#include <list>
#include <queue>
#include <algorithm>
#include <string.h>
#include <assert.h>
using std::list;
using std::stack;
using std::queue;
using std::sort;
// Declaration so we can use graphnode in GraphEdge
template <typename RTTI> class GSE_GraphNode;
template <typename RTTI> class GSE_Graph;
// -----------------------------------------------------------------
#define NODE_NAME_MAX_SIZE 128
/// \brief A class for an directed edge between two nodes in a graph.
template <typename RTTI>
class GSE_GraphEdge
{
  friend class GSE_GraphNode<RTTI>;
  friend class GSE_Graph<RTTI>;
 protected:
  /// \brief A pointer to the node where this edge is leaving from.
  GSE_GraphNode<RTTI> *m_pFrom;
  /// \brief A pointer to the node where this edge is arriving.
  GSE_GraphNode<RTTI> *m_pTo;
  /// \brief is this edge traversed
  char		       m_bTraversed;
  /// \brief The cost/weight of this edge
  int		       m_iCost;
  // --------------------------------------------------
  /// The default constructor.
  GSE_GraphEdge( )
  {
    m_pFrom = NULL;
    m_pTo   = NULL;
    m_bTraversed = 0;
  }
  // --------------------------------------------------
  /// The parametrized constructor.
  /// \param pFrom GSE_GraphNode where edge starts
  /// \param pTo GSE_GraphNode where edge leads to.
  GSE_GraphEdge( GSE_GraphNode<RTTI> *pFrom,
		 GSE_GraphNode<RTTI> *pTo )
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

  /// \brief The destructor. 
  /// It is protected and should only be invoked by GSE_Graph.
  ~GSE_GraphEdge()
  {
    m_pFrom = NULL;
    m_pTo = NULL;
    m_bTraversed = 0;
  }

 public:

  /// \brief Returns the from node.
  /// \return A pointer to GSE_GraphNode<RTTI>.
  GSE_GraphNode<RTTI> * GetFromNode()
  {
    return m_pFrom;
  }
  /// \brief Returns the to node.
  /// \return A pointer to GSE_GraphNode<RTTI>
  GSE_GraphNode<RTTI> * GetToNode()
  {
    return m_pTo;
  }
  /// \brief Sets the node traveral state.
  /// \param bFlag boolean 1 for traversed, 0 for not.
  inline void SetTraversed( char bFlag )
  {
    m_bTraversed = bFlag;
  }
  /// \brief Returns the boolean for traversal state.
  /// \return char boolean 1 for traversed, 0 otherwise.
  inline char IsTraversed()
  {
    return m_bTraversed;
  }
  /// \brief Returns the cost of this edge.
  /// \return  the cost of this edge as int
  inline int GetCost()
  {
    return m_iCost;
  }
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

/*! \brief The Generic Graph Node template class. */
/*! 
  In addition, the graph SHOULD resemble more of a tree than a fully-fledged
  graph, but it is handy to have possibility for configurations like one
  below: 
  \dot
  digraph RenderGraph {
     root [ label="Root node" ];
     cam1 [ label="Camera 1" ];
     cam2 [ label="Camera 2" ];
     cam3 [ label="Camera 3" ];
     group1 [ label="Group 1" ];
     group2 [ label="Group 2" ];
     obj1 [label="Object 1" ];
     obj2 [label="Object 2" ];
     obj3 [label="Object 3" ];
     obj4 [label="Object 4" ];

     root -> cam1;
     root -> cam2;
     root -> cam3;
     cam1 -> group1;
     cam2 -> group1;
     cam3 -> group2;
     group1 -> obj1;
     group1 -> obj2;
     group2 -> obj3;
     group2 -> obj4;
  }

  \enddot
 Where Cameras 1 and 2 have can have the same objects (Obj1 & Obj2) drawn, 
 whereas Objects Obj3 and Obj4 are visible only using Camera 3.
*/
template <typename RTTI>
class GSE_GraphNode 
{
  friend class GSE_Graph<RTTI>;
  friend class GSE_GraphEdge<RTTI>;
 protected:
  /// A list of pointers to GSE_GraphEdge objects leaving from this node.
  list<GSE_GraphEdge<RTTI> *> m_lstLeaving;
  /// A list of pointers to GSE_GraphEdge objects arriving to this node.
  list<GSE_GraphEdge<RTTI> *> m_lstArriving;
  /// A pointer to GSE_graph where this node belongs to.

  /// The symbolic color value. 
  int			      m_iColor;
  /// The name for this Scene node 
  char m_sName[NODE_NAME_MAX_SIZE];
  /// The boolean flag indicating has this node been visited.
  char m_bVisited;
  /// The boolean flag indicating has this node been culled.
  char m_bCulled;
  /// The boolean flag indicating has this node been changed.
  char m_bChanged;
  /// The enumeration indicating the type of this node.
  RTTI m_iType;
  // A reference to the graph object which contains this baby.
  GSE_Graph<RTTI> *      m_pGraph;  
  /// The default constructor.
  GSE_GraphNode() 
  {

    SetVisited(0);
    m_bCulled = 0;
    m_bChanged = 0;
    m_iType = (RTTI)0; 
    m_sName[0] = '\0';
    m_pGraph = NULL;
    m_iColor = 0;
    
  }
  /// The destructor
  ~GSE_GraphNode();

 public:

  /// Copy constructor
  GSE_GraphNode( const GSE_GraphNode &Node);
  
  /// Returns the graph where node is in.
  inline GSE_Graph<RTTI> * GetGraph();
  
  /// \brief Sets the status of visited flag.
  /// \param bFlag boolean value for visited (1 = visited, 0 = unvisited )
  void SetVisited(char bFlag );
  
  /// \brief Returns boolean value indicating has this node been visited.
  /// \return boolean 1 if this node has been visited, 0 if not.
  char IsVisited();
  
  /// \brief Renders this node and children with given renderer.
  /// \param pRenderer The renderer which is used in the process.
  //void Render( GSE_GraphNodeTraveller<RTTI> *pRenderer );
  
  /// \brief Sets this node culling status.
  /// \param bState boolean value 1 if culled, 0 if not culled.
  void SetCulled( char bState );

  /// \brief Returns boolean value indicated is this node culled.
  /// \return boolean 1 if culled, 0 otherwise
  char IsCulled();

  /// Sets the changed flag value to bFlag.
  /// \param bFlag boolean value indicating the new state.
  void SetChanged( char bFlag );
  
  /// Returns value of the m_bChanged flag.
  /// \returns boolean value if state is 'changed'.
  char IsChanged();
  
  /// Sets the type for this node.
  /// \param iType the Run-Time Type Identification value.
  void SetType( RTTI iType );

  /// Returns the type for this node.
  /// \returns  The Run-Time Type Identification value.
  RTTI GetType();
  
  /// Returns the list of edges leaving from this node
  list< GSE_GraphEdge<RTTI> * > &GetLeavingEdges();
  
  /// Returns the list of edges arriving to this node.
  list< GSE_GraphEdge<RTTI> * > &GetArrivingEdges();
  
  /// Returns true if this node has leaving edges
  char HasLeavingEdges();

  /// returns true if this node has arriving edges
  char HasArrivingEdges();

  /// Assigns the name of the node
  void SetName( const char *szName );

  /// returns the name of the joint
  const char * GetName();
  /// \brief Removes leaving edges from this node.
  /// Edges are removed alse from nodes where they arrive.
  void RemoveLeavingEdges();
  /// \brief Removes arriving edges from this node.
  /// Edges are removed alse from nodes where they lead to.
  void RemoveArrivingEdges();
  /// \brief Adds an edge from this to given node
  /// \param pTo A pointer to GSE_GraphNode.
  int AddEdge( GSE_GraphNode *pTo );
  /// \brief Removes and edge leading from this node to giben node.
  /// \param pTo A pointer to GSE_GraphNode.
  int DeleteEdgeTo( GSE_GraphNode *pTo );
  /// \brief Sets the color of this edge.
  /// \param iColor integer color
  void SetColor( int iColor );
  /// \brief Returns the color of this edge.
  /// \returns int the color.
  int  GetColor();
  /// \brief Returns the In-degree of this node
  /// \returns unsigned int the number of arriving edges.
  unsigned int GetInDegree();
  /// \brief Returns the Out-degree of this node
  /// \returns unsigned int the number of leaving edges.
  unsigned int GetOutDegree();
};

// ------------------------------------------------------------------------
//
// GraphNode Implementation
// 
// ------------------------------------------------------------------------
template<typename R>
GSE_GraphNode<R>::GSE_GraphNode( const GSE_GraphNode &Node) 
{
  m_bVisited = Node.m_bVisited;
  m_bCulled = Node.m_bCulled;
  m_bChanged = Node.m_bChanged;
  m_iType = Node.m_iType; 
  m_pGraph = m_pGraph;
  strncpy(m_sName, Node.m_sName, NODE_NAME_MAX_SIZE);
  
  m_lstLeaving = Node.m_lstLeaving;
  m_lstArriving = Node.m_lstArriving;

}

template<typename R>
inline GSE_Graph<R> *
GSE_GraphNode<R>::GetGraph()
{
  return m_pGraph;
}

//
// Class GSE_GraphNode
//
// The destructor, should be invoked with extreme care since it only removes this node
// not the children nor does it fix the parent pointers of the children.
// ----------------------------------------------------------------------------
template<typename R>
GSE_GraphNode<R>::~GSE_GraphNode()
{

}

// ----------------------------------------------------------------------
template<typename R>
void
GSE_GraphNode<R>::SetVisited(char bFlag )
{
  m_bVisited = bFlag;
}
// ----------------------------------------------------------------------
template<typename R>
char
GSE_GraphNode<R>::IsVisited()
{
  return m_bVisited;
}
// ----------------------------------------------------------------------
template<typename R>
void
GSE_GraphNode<R>::SetCulled( char bState )
{
  m_bCulled = bState;
}
// ----------------------------------------------------------------------
template<typename R>
char
GSE_GraphNode<R>::IsCulled()
{
  return m_bCulled;
}
// ----------------------------------------------------------------------
template<typename R>
void
GSE_GraphNode<R>::SetType( R iType )
{
  m_iType = iType;
}
// ----------------------------------------------------------------------
template<typename R>
R
GSE_GraphNode<R>::GetType()
{
  return m_iType;
}
// ----------------------------------------------------------------------
template<typename R>
list<GSE_GraphEdge<R> *> &
GSE_GraphNode<R>::GetLeavingEdges()
{
  return m_lstLeaving;
}
// ----------------------------------------------------------------------
template<typename R>
list<GSE_GraphEdge< R > * > &
GSE_GraphNode<R>::GetArrivingEdges()
{
  return m_lstArriving;
}
// ----------------------------------------------------------------------
template<typename R>
char
GSE_GraphNode<R>::HasLeavingEdges()
{
  return (m_lstLeaving.size() > 0 );
}
// ----------------------------------------------------------------------
template<typename R>
char
GSE_GraphNode<R>::HasArrivingEdges()
{
  return (m_lstArriving.size() > 0 );
}

// ----------------------------------------------------------------------

template<typename R>
void 
GSE_GraphNode<R>::SetName( const char *szName )
{
  strncpy(m_sName,szName,NODE_NAME_MAX_SIZE);
}

// ----------------------------------------------------------------------
template<typename R>
const char * 
GSE_GraphNode<R>::GetName()
{
  return m_sName;
}
// ----------------------------------------------------------------------
template<typename R>
void 
GSE_GraphNode<R>::SetChanged( char bFlag )
{
  m_bChanged = bFlag;
}
// ----------------------------------------------------------------------
// Returns value of the m_bChanged flag
template<typename R>
char
GSE_GraphNode<R>::IsChanged()
{
  return m_bChanged;
}
// ----------------------------------------------------------------------
template<typename R>
int 
GSE_GraphNode<R>::AddEdge( GSE_GraphNode<R> *pTo )
{
  return m_pGraph->AddEdge( this, pTo );
}
// ----------------------------------------------------------------------
template<typename R>
int 
GSE_GraphNode<R>::DeleteEdgeTo( GSE_GraphNode<R> *pTo )
{
  typename list<GSE_GraphEdge<R> *>::iterator it = this->GetLeavingEdges().begin();
  // For each leaving edge in this 
  for(;it!=this->GetLeavingEdges().end();it++)
  {
    // if edge points to pTo, we delete it.
    if ( (*it)->GetToNode() == pTo )
    {
      GSE_GraphEdge<R> *pEdge = *it;
      m_pGraph->DeleteEdge( pEdge );
      break;
    }
  }
  return 0;

}
// ----------------------------------------------------------------------
template<typename R>
void 
GSE_GraphNode<R>::RemoveLeavingEdges()
{
  m_pGraph->RemoveLeavingEdgesFrom( this );
}
// ----------------------------------------------------------------------
template<typename R>
void 
GSE_GraphNode<R>::RemoveArrivingEdges()
{
  m_pGraph->RemoveArrivingEdgesFrom( this );
}
// ----------------------------------------------------------------------
template<typename R>
void
GSE_GraphNode<R>::SetColor( int iColor )
{
  m_iColor = iColor;
}
// ----------------------------------------------------------------------
template<typename R>
int 
GSE_GraphNode<R>::GetColor()
{
  return m_iColor;
}
// ----------------------------------------------------------------------
template<typename R>
unsigned int
GSE_GraphNode<R>::GetInDegree()
{
  return m_lstArriving.size();
}
// ----------------------------------------------------------------------
template<typename R>
unsigned int
GSE_GraphNode<R>::GetOutDegree()
{
  return m_lstLeaving.size();
}
// ----------------------------------------------------------------------
template <typename R>
class GSE_Graph
{
 public:  
  // -----------------------------------------------------------------
  typedef enum 
  {
    BREADTH_FIRST      = 0,   // breadth-first traversal 
    DEPTH_FIRST        = 1,   // depth-first traversal
    DEPTH_FIRST_BY_EDGES,     // depth-first, but with every 
			      // edge is considered as a new branch of a tree.
  } GraphTraversalMode;
  // -----------------------------------------------------------------
 protected:
  std::list< GSE_GraphNode< R > *> m_lstNodes;
  std::list< GSE_GraphEdge< R > *> m_lstEdges;
  std::string m_sName;
  // -----------------------------------------------------------------
 public:  
  // -----------------------------------------------------------------
  GSE_Graph()
  {
    m_sName = "";
  }
  // -----------------------------------------------------------------
  ~GSE_Graph()
  {    
    GSE_DEBUG( "Deleting graph...");
    RemoveEdges();
    RemoveNodes();
    GSE_DEBUG( "graph deleted" );
  }
  // -----------------------------------------------------------------
  
  //GSE_GraphNode<R> * AddNode( GSE_GraphNode< R > *pNode );
  void DeleteNode( GSE_GraphNode< R > *pNode );
  // 
  // Adds an edge between pNodeFrom and pNodeTo.
  // Returns zero if ok, non-zero on error
  //
  int AddEdge( GSE_GraphNode< R > *pNodeFrom, GSE_GraphNode< R > *pNodeTo);
  //
  // Removes edge *pEdge
  //
  void DeleteEdge( GSE_GraphEdge<R> *pEdge );

  void RemoveLeavingEdgesFrom( GSE_GraphNode<R> *pNode );
  void RemoveArrivingEdgesFrom( GSE_GraphNode<R> *pNode );
  
  GSE_GraphNode<R> * SeekNodeByNameAndType( const char *szName, 
					    const R iType );
  
  

  
  // Sets each node as unvisited
  void  SetNodesUnvisited();
  // Sets each edge untraversed.
  void  SetEdgesUntraversed();
  /// Sets the color of all nodes.
  /// \param nColor The color which all nodes will be colored.
  void  SetColor( unsigned int nColor );
  
  template <class Type>  inline Type * CreateNode()
  {
    Type *t = new Type();
    m_lstNodes.push_back(t);
    t->m_pGraph = this;
    return t;
  }
  inline const unsigned int GetNodeCount() const
  {
    return m_lstNodes.size();
  }
  inline const unsigned int GetEdgeCount() const
  {
    return m_lstEdges.size();
  }
 protected:

  void RemoveNodes();
  void RemoveEdges();


};
// -----------------------------------------------------------------
template<typename R>
void 
GSE_Graph<R>::DeleteNode( GSE_GraphNode< R> *pNode )
{
  typename list<GSE_GraphNode<R> *>::iterator it;
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
    GSE_WARN( "Node " << pNode << " (" << pNode->GetName() << ") " 
	      << "is not part of this graph!" );
  }
  
}
// -----------------------------------------------------------------
template<typename R>
int 
GSE_Graph<R>::AddEdge( GSE_GraphNode<R> *pNodeFrom, GSE_GraphNode<R> *pNodeTo )
{
  if ( pNodeFrom == NULL )
  {
    GSE_ERR( "FromNode is NULL" );
    return 1;
  }
  
  if ( pNodeTo == NULL )
  {
    GSE_ERR( "ToNode is NULL" );
    return 1;
  }
  
  if ( pNodeTo->m_pGraph != pNodeFrom->m_pGraph )
  {
    GSE_ERR( "Nodes belong to different graphs!" );
    return 1;
  }
  
  GSE_GraphEdge<R> *pEdge = new GSE_GraphEdge<R>( pNodeFrom, pNodeTo);
  pNodeFrom->GetLeavingEdges().push_back( pEdge );
  pNodeTo->GetArrivingEdges().push_back( pEdge );
  m_lstEdges.push_back(pEdge);
  return 0;
}
// -----------------------------------------------------------------
template<typename R>
void
GSE_Graph<R>::DeleteEdge( GSE_GraphEdge<R> *pEdge)
{
  typename list< GSE_GraphEdge<R> *>::iterator it;
  it = find(m_lstEdges.begin(), m_lstEdges.end(), pEdge );

  if ( it != m_lstEdges.end())
  {
    // Delete edge from Graph edge list 
    m_lstEdges.erase(it);
    
    list< GSE_GraphEdge<R> *> &lstFromEdges = 
     pEdge->GetFromNode()->GetLeavingEdges();
    list< GSE_GraphEdge<R> *> &lstToEdges = 
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
// -----------------------------------------------------------------
template<typename R>
void
GSE_Graph<R>::RemoveNodes()
{
  GSE_DEBUG( "removing nodes (" << m_lstNodes.size() << ")...");
  while( !m_lstNodes.empty())
  {
    GSE_GraphNode<R> *pTmpNode = m_lstNodes.front();
    m_lstNodes.pop_front();
    delete pTmpNode;
  }
  GSE_DEBUG("done");
}
// -----------------------------------------------------------------
template<typename R>
void
GSE_Graph<R>::RemoveEdges()
{
  GSE_DEBUG( "removing edges (" << m_lstEdges.size() << ")...");
  while( !m_lstEdges.empty() )
  {
    GSE_GraphEdge<R> *pTmpEdge = m_lstEdges.front();
    m_lstEdges.pop_front();
    delete pTmpEdge;
  }
  GSE_DEBUG("done.");
}
// -----------------------------------------------------------------
template<typename R>
void 
GSE_Graph<R>::RemoveLeavingEdgesFrom( GSE_GraphNode<R> *pNode )
{

  typename list<GSE_GraphEdge<R> *>::iterator it;

  while( !pNode->GetLeavingEdges().empty())
  {
    it = pNode->GetLeavingEdges().begin();
    DeleteEdge( *it );
  }

}
// -----------------------------------------------------------------
template<typename R>
void 
GSE_Graph<R>::RemoveArrivingEdgesFrom( GSE_GraphNode<R> *pNode )
{
  typename list<GSE_GraphEdge<R> *>::iterator it;
  while( !pNode->GetArrivingEdges().empty())
  {
    it = pNode->GetArrivingEdges().begin();
    DeleteEdge( *it );
  }
}
// -----------------------------------------------------------------

#define STR_EQUALS( STR1, STR2 ) ( strncmp(STR1, STR2, NODE_NAME_MAX_SIZE) == 0 )

// -----------------------------------------------------------------
template<typename R>
GSE_GraphNode<R> * 
GSE_Graph<R>::SeekNodeByNameAndType( const char *szName, 
				     const R iType )
{
  typename list< GSE_GraphNode<R> *>::iterator it;
  it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++)
  {
    GSE_GraphNode<R> *pNode = *it;
    if ( pNode->GetType() == iType && STR_EQUALS( pNode->GetName(), szName )) 
    {
      return pNode;
    }
  }
  
  return NULL;
}

#undef STR_EQUALS
// -----------------------------------------------------------------
template <typename R>
void
GSE_Graph<R>::SetNodesUnvisited( )
{
  
  typename list<GSE_GraphNode<R> *>::iterator it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++)
  {
    (*it)->SetVisited(0);
  }
}
// -----------------------------------------------------------------
template <typename R>
void
GSE_Graph<R>::SetEdgesUntraversed( )
{

  typename list<GSE_GraphEdge<R> *>::iterator it = m_lstEdges.begin();

  for( ; it != m_lstEdges.end(); it++)
  {
    (*it)->SetTraversed(0);
  }
}
template <typename R>
void  
GSE_Graph<R>::SetColor( unsigned int nColor )
{
  typename list<GSE_GraphNode<R> *>::iterator it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++) 
  {
    (*it)->SetColor(nColor);
  }
}
// -----------------------------------------------------------------
#define HAS_UNTRAVERSED_EDGES( N ) ( (unsigned int)(N->GetColor()) < N->GetOutDegree())
#define IS_VISITED( N ) ( N->GetColor() > 0 )

namespace Graph {
  template <typename R>
  GSE_GraphNode<R> * SeekNextWithType( GSE_GraphNode<R> *pStartNode, R type )
  {
    GSE_GraphNode<R> *pCandidate = NULL;
    queue< GSE_GraphNode<R> *>  queueNodes;
    list< GSE_GraphEdge<R> *> listEdges;
    typename list< GSE_GraphEdge<R> *>::iterator it;

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
  TravelDF( GSE_GraphNode<R> *pStartNode, TravellerType * pTraveller )
  {
    stack< GSE_GraphNode<R> *> stckNodes;
    list< GSE_GraphEdge<R> *>  lstEdges;
    GSE_GraphEdge<R> *pEdge = NULL;
    if ( pStartNode ==NULL ) 
    {
      GSE_ERR("pStartNode is NULL");
      return;
    } 

    pStartNode->GetGraph()->SetColor(0);         // Clear number of processed nodes

    stckNodes.push(pStartNode);

    while ( !stckNodes.empty())
    {
      GSE_GraphNode<R> *pNode = stckNodes.top();
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
}


#endif
