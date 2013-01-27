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

#ifndef __MoosePathFinding_h__
#define __MoosePathFinding_h__
/////////////////////////////////////////////////////////////////
#include "MooseGraph.h"
#include "MooseSpatial.h"
#include "MooseAPI.h"
#include <queue>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace AI
  {
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CSearchNode : public Moose::Core::CGraphNode,
				    public Moose::Spatial::CPositional
    {
      friend class Moose::Core::CGraph;
    private:
      int m_iCostFromStart;
      int m_iCostToGoal;
      CSearchNode *m_pParent;
    public:
      /// Constructor.
      CSearchNode();
      /// Destructor.
      virtual ~CSearchNode();

      int GetG();
      int GetCostFromStart();
      
      int GetH();
      int GetCostToGoal();

      int GetF();
      int GetTotalCost();
      
      void SetG( int iVal );
      void SetCostFromStart( int iVal );

      void SetH( int iVal );
      void SetCostToGoal( int iVal );
      void SetParent( CSearchNode *pNode );
      CSearchNode * GetParent();
    };
    /////////////////////////////////////////////////////////////////
    typedef std::list<CSearchNode *>	       OpenList;
    typedef std::list<CSearchNode *>	       ClosedList;
    typedef std::list<CSearchNode *>	       FinalPathList;
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CAStarSearch 
    {
    protected:

      OpenList	  m_Open;
      ClosedList  m_Closed;
      
      CGraph	  m_SearchGraph;
    public:

      CAStarSearch();
      virtual ~CAStarSearch();

      bool FindPath( CSearchNode *pStart, CSearchNode *pGoal, FinalPathList & path );
      
      OpenList &	GetOpen();
      ClosedList &	GetClosed();

    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
