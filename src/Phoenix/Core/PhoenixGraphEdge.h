
#ifndef PHOENIXGRAPHEDGE_H_
#define PHOENIXGRAPHEDGE_H_
#include <stack>
#include <list>
#include <queue>
#include <algorithm>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////
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

  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXGRAPHEDGE_H_ */
