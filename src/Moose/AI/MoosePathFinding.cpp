/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "MoosePathFinding.h"
#include <algorithm>
/////////////////////////////////////////////////////////////////
namespace libname = Moose::AI;
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
