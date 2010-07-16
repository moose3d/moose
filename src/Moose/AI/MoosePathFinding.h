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
