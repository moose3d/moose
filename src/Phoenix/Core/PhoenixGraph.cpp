//#include "PhoenixGraph.h"
//#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
Phoenix::Core::TGraphEdge<NODE_TYPE>::TGraphEdge( NODE_TYPE *pFrom, NODE_TYPE *pTo )
{
  m_pFrom = pFrom;
  m_pTo   = pTo;
  // safety check, we do not allow links to be added to ourselves
  assert( pFrom != pTo );
  // Both nodes must be non-NULL
  assert( pFrom != NULL );
  assert( pTo != NULL );
    
  // Both nodes must belong to same graph
  assert( pFrom->m_pGraph == pTo->m_pGraph );
    
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void 
Phoenix::Core::TGraph<NODE_TYPE>::DeleteNode( NODE_TYPE *pNode )
{
  typename NodeListType::iterator it;
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
template<class NODE_TYPE>
void
Phoenix::Core::TGraph<NODE_TYPE>::DeleteEdge( Phoenix::Core::TGraphEdge<NODE_TYPE> *pEdge)
{
  typename EdgeListType::iterator it;
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
template<class NODE_TYPE>
void
Phoenix::Core::TGraph<NODE_TYPE>::RemoveNodes()
{
  while( !m_lstNodes.empty())
  {
    NODE_TYPE *pTmpNode = m_lstNodes.front();
    m_lstNodes.pop_front();
    delete pTmpNode;
  }
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void
Phoenix::Core::TGraph<NODE_TYPE>::RemoveEdges()
{
  while( !m_lstEdges.empty() )
  {
    TGraphEdge<NODE_TYPE> *pTmpEdge = m_lstEdges.front();
    m_lstEdges.pop_front();
    delete pTmpEdge;
  }
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void 
Phoenix::Core::TGraph<NODE_TYPE>::RemoveLeavingEdgesFrom( NODE_TYPE *pNode )
{
  typename EdgeListType::iterator it;
  while( !pNode->GetLeavingEdges().empty())
  {
    it = pNode->GetLeavingEdges().begin();
    DeleteEdge( *it );
  }
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void 
Phoenix::Core::TGraph<NODE_TYPE>::RemoveArrivingEdgesFrom( NODE_TYPE *pNode )
{
  typename EdgeListType::iterator it;
  while( !pNode->GetArrivingEdges().empty())
  {
    it = pNode->GetArrivingEdges().begin();
    DeleteEdge( *it );
  }
}
/////////////////////////////////////////////////////////////////
// template<class NODE_TYPE>
// void
// Phoenix::Core::TGraph<NODE_TYPE>::SetNodesUnvisited( )
// {
  
//   typename NodeListType::iterator it = m_lstNodes.begin();

//   for( ; it != m_lstNodes.end(); it++)
//   {
//     (*it)->SetVisited(0);
//   }
// }
// /////////////////////////////////////////////////////////////////
// template<class NODE_TYPE>
// void
// Phoenix::Core::TGraph<NODE_TYPE>::SetEdgesUntraversed( )
// {
//   typename EdgeListType::iterator it = m_lstEdges.begin();

//   for( ; it != m_lstEdges.end(); it++)
//   {
//     (*it)->SetTraversed(0);
//   }
// }
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void  
Phoenix::Core::TGraph<NODE_TYPE>::SetColor( unsigned int nColor )
{
  typename NodeListType::iterator it = m_lstNodes.begin();

  for( ; it != m_lstNodes.end(); it++) 
  {
    (*it)->SetColor(nColor);
  }
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void
Phoenix::Core::TGraph<NODE_TYPE>::RegisterNode( NODE_TYPE *pNode )
{
  if ( pNode == NULL ) return;
  typename NodeListType::iterator it = find( m_lstNodes.begin(), m_lstNodes.end(), pNode );
  if ( it == m_lstNodes.end() ) 
  {
    m_lstNodes.push_back( pNode );
    pNode->m_pGraph = this;
  }
  else
  {
    std::cerr << "Attempt to Re-register existing node! (" << pNode << ")" << std::endl;
  }
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
Phoenix::Core::TGraph<NODE_TYPE> *
Phoenix::Core::TGraphNode<NODE_TYPE>::GetGraph()
{
  return m_pGraph;
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
EdgeListType &
Phoenix::Core::TGraphNode<NODE_TYPE>::GetLeavingEdges()
{
  return m_lstLeaving;
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
EdgeListType &
Phoenix::Core::TGraphNode<NODE_TYPE>::GetArrivingEdges()
{
  return m_lstArriving;
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
int
Phoenix::Core::TGraphNode<NODE_TYPE>::HasLeavingEdges()
{
  return (m_lstLeaving.size() > 0 );
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
int
Phoenix::Core::TGraphNode<NODE_TYPE>::HasArrivingEdges()
{
  return (m_lstArriving.size() > 0 );
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
int 
Phoenix::Core::TGraphNode<NODE_TYPE>::DeleteEdgeTo( NODE_TYPE *pTo )
{
  typename EdgeListType::iterator it = this->GetLeavingEdges().begin();
  // For each leaving edge in this 
  for(;it!=this->GetLeavingEdges().end();it++)
  {
    // if edge points to pTo, we delete it.
    if ( (*it)->GetToNode() == pTo )
    {
      TGraphEdge<NODE_TYPE> *pEdge = *it;
      m_pGraph->DeleteEdge( pEdge );
      break;
    }
  }
  return 0;
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void 
Phoenix::Core::TGraphNode<NODE_TYPE>::RemoveLeavingEdges()
{
  m_pGraph->RemoveLeavingEdgesFrom( this );
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void 
Phoenix::Core::TGraphNode<NODE_TYPE>::RemoveArrivingEdges()
{
  m_pGraph->RemoveArrivingEdgesFrom( this );
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
void
Phoenix::Core::TGraphNode<NODE_TYPE>::SetColor( int iColor )
{
  m_iColor = iColor;
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
int 
Phoenix::Core::TGraphNode<NODE_TYPE>::GetColor()
{
  return m_iColor;
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
size_t
Phoenix::Core::TGraphNode<NODE_TYPE>::GetInDegree()
{
  return m_lstArriving.size();
}
/////////////////////////////////////////////////////////////////
template<class NODE_TYPE>
size_t
Phoenix::Core::TGraphNode<NODE_TYPE>::GetOutDegree()
{
  return m_lstLeaving.size();
}
/////////////////////////////////////////////////////////////////
