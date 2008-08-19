#include "PhoenixPathFinding.h"
#include <algorithm>
/////////////////////////////////////////////////////////////////
namespace libname = Phoenix::AI;
using namespace std;
/////////////////////////////////////////////////////////////////
libname::CAStarSearch::CAStarSearch()
{
  
}
/////////////////////////////////////////////////////////////////
libname::CAStarSearch::~CAStarSearch()
{
  
}
/////////////////////////////////////////////////////////////////
bool
libname::CAStarSearch::FindPath( CSearchNode *pStart, CSearchNode *pGoal, FinalPathList & path )
{
  GetOpen().clear();
  GetClosed().clear();
  
  CSearchNode *pStartNode = m_SearchGraph->CreateNode<CSearchNode>();
  pStartNode->SetCostToGoal( CalculateCost() );
  GetOpen().push_back( pStartNode );
  NodeListType::iterator itNewInOpen;
  NodeListType::iterator itNewInClosed;
	
  while( GetOpen().empty() == false )
  {
    CSearchNode *pNode = GetOpen().top(); GetOpen().pop();
   
    if( pNode == pGoal )  // path is found
    {

      while( !pNode->GetArrivingEdges().empty())
      {
	path.push_front( pNode );
	// Get parent node
	pNode = static_cast<CSearchNode *>(pNode->GetArrivingEdges().front()->GetFromNode());
      }

      // insert start node also to list.
      path.push_front( pNode );
      return true;
    }
    else
    {
      // for each neighbor of pNode
      EdgeListType::iterator it = pNode->GetLeavingEdges();

      for( ; it != pNode->GetLeavingEdges().end(); it++)
      {
	CSearchNode * pNewNode = static_cast<CSearchNode *>( (*it)->GetToNode() );
	int iNewCost = pNode->GetCostFromStart() + TraverseCost( pNode, pNewNode );

	itNewInOpen   = find( GetOpen().begin(),   GetOpen().end(),   pNewNode);
	itNewInClosed = find( GetClosed().begin(), GetClosed().end(), pNewNode);

	bool bNewInOpen   = ( itNewInOpen != GetOpen().end());
	bool bNewInClosed = ( itNewInClosed != GetClosed().end());
	
	if ( (bNewInOpen || bNewInClosed) && (pNewNode->GetCostFromStart() <= iNewCost) )
	{
	  continue;
	}
	else
	{
	  
	  pNewNode->SetParent( pNode );
	  pNewNode->SetCostFromStart(iNewCost);
	  pNewNode->SetCostToGoal( CalculateCost( newNode.loc, goalLoc ) );
	  
	  if ( bNewInClosed )
	  {
	    GetClosed().erase(itNewInClosed);
	  }
	  
	  if ( bNewInOpen )
	  {
	    // adjust newnode's location in Open ?
	  }
	  else
	  {
	    GetOpen().push_back( pNewNode );
	  }
	} // bNewInOpen || nNewInClosed
      } // for 
    } // node not goal

    GetClosed().push_back( pNode );
    
  } // while Open not empty
  
  // no dice, path not found.
  return false;
}
/////////////////////////////////////////////////////////////////
libname::OpenList
libname::CAStarSearch::GetOpen()
{
  return m_Open;
}
/////////////////////////////////////////////////////////////////
libname::ClosedList
libname::CAStarSearch::GetClosed()
{
  return m_Closed;
}
/////////////////////////////////////////////////////////////////
