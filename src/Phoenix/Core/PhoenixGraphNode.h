
#ifndef PHOENIXGRAPHNODE_H_
#define PHOENIXGRAPHNODE_H_
#include <stack>
#include <list>
#include <queue>
#include <algorithm>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "PhoenixAPI.h"
#include "PhoenixGraphEdge.h"
/////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
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

#define EdgeListType   std::list< Phoenix::Core::TGraphEdge<NODE_TYPE> * >
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
       /// Write your own destructor into your inherited graph and call necessary removal routines (leaving, arriving and the node itself from graph).
       virtual ~TGraphNode();
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
       // This causes some headaches in swig...
#ifndef SWIG
       ////////////////////
       ///  Adds an edge from this to given node
       /// \param pTo A pointer to CGraphNode.
       Phoenix::Core::TGraphEdge<NODE_TYPE> * AddEdge( NODE_TYPE *pTo )
       {
       	return m_pGraph->AddEdge( this, pTo );
       }
#endif
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
  }
}
/////////////////////////////////////////////////////////////
#endif /* PHOENIXGRAPHNODE_H_ */
