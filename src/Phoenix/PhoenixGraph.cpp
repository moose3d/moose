#include "PhoenixGraph.h"
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////
Phoenix::Core::CGraphEdge::CGraphEdge( CGraphNode *pFrom,  CGraphNode *pTo )
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
/////////////////////////////////////////////////////////////////
void 
Phoenix::Core::CGraph::DeleteNode( CGraphNode *pNode )
{
  NodeListType::iterator it;
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
    std::cerr << "Node " << pNode 
	      << "is not part of this graph!" << std::endl;
  }
  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Core::CGraph::DeleteEdge( Phoenix::Core::CGraphEdge *pEdge)
{
  EdgeListType::iterator it;
  it = find(m_lstEdges.begin(), m_lstEdges.end(), pEdge );

  if ( it != m_lstEdges.end())
  {
    // Delete edge from Graph edge list 
    m_lstEdges.erase(it);
    
    EdgeListType &lstFromEdges = 
     pEdge->GetFromNode()->GetLeavingEdges();
    EdgeListType &lstToEdges = 
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
void
Phoenix::Core::CGraph::RemoveNodes()
{
  while( !m_lstNodes.empty())
  {
    CGraphNode *pTmpNode = m_lstNodes.front();
    m_lstNodes.pop_front();
    delete pTmpNode;
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Core::CGraph::RemoveEdges()
{
  while( !m_lstEdges.empty() )
  {
    CGraphEdge *pTmpEdge = m_lstEdges.front();
    m_lstEdges.pop_front();
    delete pTmpEdge;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Core::CGraph::RemoveLeavingEdgesFrom( CGraphNode *pNode )
{
  EdgeListType::iterator it;
  while( !pNode->GetLeavingEdges().empty())
  {
    it = pNode->GetLeavingEdges().begin();
    DeleteEdge( *it );
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Core::CGraph::RemoveArrivingEdgesFrom( CGraphNode *pNode )
{
  EdgeListType::iterator it;
  while( !pNode->GetArrivingEdges().empty())
  {
    it = pNode->GetArrivingEdges().begin();
    DeleteEdge( *it );
  }
}
/////////////////////////////////////////////////////////////////
/* #define STR_EQUALS( STR1, STR2 ) ( strncmp(STR1, STR2, Phoenix::Globals::NODE_NAME_MAX_SIZE) == 0 ) */
/* ///////////////////////////////////////////////////////////////// */

/* Phoenix::Core::CGraphNode *  */
/* Phoenix::Core::CGraph::SeekNodeByNameAndType( const char *szName,  */
/* 				     const R iType ) */
/* { */
/*   typename std::list< CGraphNode *>::iterator it; */
/*   it = m_lstNodes.begin(); */

/*   for( ; it != m_lstNodes.end(); it++) */
/*   { */
/*     CGraphNode *pNode = *it; */
/*     if ( pNode->GetType() == iType && STR_EQUALS( pNode->GetName(), szName ))  */
/*     { */
/*       return pNode; */
/*     } */
/*   } */
  
/*   return NULL; */
/* } */

/* #undef STR_EQUALS */
/////////////////////////////////////////////////////////////////
void
Phoenix::Core::CGraph::SetNodesUnvisited( )
{
  
  NodeListType::iterator it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++)
  {
    (*it)->SetVisited(0);
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Core::CGraph::SetEdgesUntraversed( )
{
  EdgeListType::iterator it = m_lstEdges.begin();

  for( ; it != m_lstEdges.end(); it++)
  {
    (*it)->SetTraversed(0);
  }
}
/////////////////////////////////////////////////////////////////
void  
Phoenix::Core::CGraph::SetColor( unsigned int nColor )
{
  NodeListType::iterator it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++) 
  {
    (*it)->SetColor(nColor);
  }
}
/////////////////////////////////////////////////////////////////
#define HAS_UNTRAVERSED_EDGES( N ) ( (unsigned int)(N->GetColor()) < N->GetOutDegree())
#define IS_VISITED( N ) ( N->GetColor() > 0 )
/////////////////////////////////////////////////////////////////
/* template <R, NODE_NAME, EDGE_NAME> */
/* Phoenix::Core::CGraphNode * SeekNextWithType( CGraphNode *pStartNode, R type ) */
/* { */
/*   CGraphNode *pCandidate = NULL; */
/*   std::queue< CGraphNode *>  queueNodes; */
/*   EdgeListType listEdges; */
/*   typename EdgeListType::iterator it; */

/*   // Sanity check */
/*   if ( pStartNode == NULL ) return NULL; */
      
/*   // Set all nodes unvisited */
/*   pStartNode->GetGraph()->SetColor(0); */
/*   queueNodes.push(pStartNode); */
/*   while( !queueNodes.empty() ) */
/*   { */
/*     pCandidate = queueNodes.front(); queueNodes.pop(); */
/*     pCandidate->SetColor(1); */
      
/*     if ( pCandidate->GetType() == type ) return pCandidate; */
/*     listEdges = pCandidate->GetLeavingEdges(); */
/*     // for each neighbor do */
/*     for( it = listEdges.begin(); it != listEdges.end(); it++) */
/*     { */
/*       // if neighbor is not yet visited, put it in the queue */
/*       if ( (*it)->GetToNode()->GetColor() == 0 ) queueNodes.push( (*it)->GetToNode() ); */
/*     } */
/*   } */
/*   // No such node was found */
/*   return NULL; */
/* }; */
////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/* Phoenix::Core::CGraphNode::CGraphNode( const CGraphNode &Node)  */
/* { */
/*   m_bVisited = Node.m_bVisited; */
/*   m_bCulled = Node.m_bCulled; */
/*   m_bChanged = Node.m_bChanged; */
/*   m_iType = Node.m_iType;  */
/*   m_pGraph = m_pGraph; */
/*   m_Name = Node.m_Name; */
  
/*   m_lstLeaving = Node.m_lstLeaving; */
/*   m_lstArriving = Node.m_lstArriving; */

/* } */
/////////////////////////////////////////////////////////////////
Phoenix::Core::CGraph *
Phoenix::Core::CGraphNode::GetGraph()
{
  return m_pGraph;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Core::CGraphNode::SetVisited(int bFlag )
{
  m_bVisited = bFlag;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Core::CGraphNode::IsVisited()
{
  return m_bVisited;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Core::CGraphNode::SetCulled( int bState )
{
  m_bCulled = bState;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Core::CGraphNode::IsCulled()
{
  return m_bCulled;
}
/////////////////////////////////////////////////////////////////
EdgeListType &
Phoenix::Core::CGraphNode::GetLeavingEdges()
{
  return m_lstLeaving;
}
/////////////////////////////////////////////////////////////////
EdgeListType &
Phoenix::Core::CGraphNode::GetArrivingEdges()
{
  return m_lstArriving;
}
/////////////////////////////////////////////////////////////////

int
Phoenix::Core::CGraphNode::HasLeavingEdges()
{
  return (m_lstLeaving.size() > 0 );
}
/////////////////////////////////////////////////////////////////

int
Phoenix::Core::CGraphNode::HasArrivingEdges()
{
  return (m_lstArriving.size() > 0 );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Core::CGraphNode::SetChanged( int bFlag )
{
  m_bChanged = bFlag;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Core::CGraphNode::IsChanged()
{
  return m_bChanged;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Core::CGraphNode::DeleteEdgeTo( CGraphNode *pTo )
{
  EdgeListType::iterator it = this->GetLeavingEdges().begin();
  // For each leaving edge in this 
  for(;it!=this->GetLeavingEdges().end();it++)
  {
    // if edge points to pTo, we delete it.
    if ( (*it)->GetToNode() == pTo )
    {
      CGraphEdge *pEdge = *it;
      m_pGraph->DeleteEdge( pEdge );
      break;
    }
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Core::CGraphNode::RemoveLeavingEdges()
{
  m_pGraph->RemoveLeavingEdgesFrom( this );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Core::CGraphNode::RemoveArrivingEdges()
{
  m_pGraph->RemoveArrivingEdgesFrom( this );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Core::CGraphNode::SetColor( int iColor )
{
  m_iColor = iColor;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Core::CGraphNode::GetColor()
{
  return m_iColor;
}
/////////////////////////////////////////////////////////////////
size_t
Phoenix::Core::CGraphNode::GetInDegree()
{
  return m_lstArriving.size();
}
/////////////////////////////////////////////////////////////////
size_t
Phoenix::Core::CGraphNode::GetOutDegree()
{
  return m_lstLeaving.size();
}
/////////////////////////////////////////////////////////////////
