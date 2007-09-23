#ifndef __PhoenixOctree_h__
#define __PhoenixOctree_h__
/////////////////////////////////////////////////////////////////
#include <list>
#include "PhoenixSpatial.h"
#include "PhoenixVolume.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Spatial
  {
    /// Symbolic indices for Octree children nodes.
    enum OCTREE_SECTION
    {
      TOP_LEFT_FRONT     = 0,
      TOP_LEFT_BACK      = 1,
      TOP_RIGHT_FRONT    = 2,
      TOP_RIGHT_BACK     = 3,
      BOTTOM_LEFT_FRONT  = 4,
      BOTTOM_LEFT_BACK   = 5,
      BOTTOM_RIGHT_FRONT = 6,
      BOTTOM_RIGHT_BACK  = 7
    };
    ////////////////////
    /// Symbolic indices for node neighbors.
    enum OCTREE_NEIGHBOR
    {
      LEFT   = 0,
      RIGHT  = 1,
      TOP    = 2,
      BOTTOM = 3,
      FRONT  = 4,
      BACK   = 5
    };
    /////////////////////////////////////////////////////////////////
    /// Generic Octree class template.
    template<typename TYPE>
    class COctree : public Phoenix::Spatial::CDimensional1D,
                    public Phoenix::Spatial::CPositional
    {
    protected:
      std::list< TYPE > m_lstObjects;
      /// Children of this node.
      COctree * m_pChildren[8];
      /// Neighbor objects of this node.
      COctree * m_pNeighbors[6];
    public:
      ////////////////////
      /// The constructor.
      COctree();
      ////////////////////
      /// Destructor.
      ~COctree();
      ////////////////////
      /// Index accessor method.
      /// \param iSection which node will be returned.
      /// \returns Pointer to child node.
      COctree * GetChild( OCTREE_SECTION iSection );
      ////////////////////
      /// Removes node from section iSection.
      /// \param iSection Section to be deleted.
      void DeleteNode( OCTREE_SECTION iSection);
      ////////////////////
      /// Assigns node into section.
      /// \param iSection Section to be inserted.
      /// \param pNode Pointer to a node to be inserted.
      void SetChild( OCTREE_SECTION iSection, COctree *pNode );
      ////////////////////
      /// Assigns node neighbor.
      /// \param iNeighbor As which neightbor node is put.
      /// \param pNode Pointer to a octree node.
      void SetNeighbor( OCTREE_NEIGHBOR iNeighbor, COctree *pNode );
      ////////////////////
      /// Returns neighbor node.
      /// \param iNeighbor Which neighbor is returned.
      /// \returns Pointer to neighbor node.
      COctree * GetNeighbor( OCTREE_NEIGHBOR iNeighbor );
      ////////////////////
      /// Returns children.
      /// \returns Pointer to the node array.
      COctree ** GetNodes();
      ////////////////////
      /// Returns a list of objects in this node.      
      /// \returns Reference to a list of objects.
      std::list< TYPE > & GetObjects();
    };/// class COctree
  } // namespace Spatial
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
/// Macro for easier deletion
#define DELETE(A) \
 {\
  if ( A != NULL ){\
    delete A;\
    A = NULL;\
  }\
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctree<TYPE>::COctree() : CDimensional1D(), CPositional()
{
  // initialize nodes 
  m_pChildren[TOP_LEFT_FRONT] = NULL;
  m_pChildren[TOP_LEFT_BACK] = NULL;
  m_pChildren[TOP_RIGHT_FRONT] = NULL;
  m_pChildren[TOP_RIGHT_BACK] = NULL;
  m_pChildren[BOTTOM_LEFT_FRONT] = NULL;
  m_pChildren[BOTTOM_LEFT_BACK] = NULL;
  m_pChildren[BOTTOM_RIGHT_FRONT] = NULL;
  m_pChildren[BOTTOM_RIGHT_BACK] = NULL;

  m_pNeighbors[TOP] = NULL;
  m_pNeighbors[BOTTOM] = NULL;
  m_pNeighbors[LEFT] = NULL;
  m_pNeighbors[RIGHT] = NULL;
  m_pNeighbors[FRONT] = NULL;
  m_pNeighbors[BACK] = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctree<TYPE>::~COctree()
{
  DELETE(m_pChildren[TOP_LEFT_FRONT]);
  DELETE(m_pChildren[TOP_LEFT_BACK]);
  DELETE(m_pChildren[TOP_RIGHT_FRONT]);
  DELETE(m_pChildren[TOP_RIGHT_BACK]);
  DELETE(m_pChildren[BOTTOM_LEFT_FRONT]);
  DELETE(m_pChildren[BOTTOM_LEFT_BACK]);
  DELETE(m_pChildren[BOTTOM_RIGHT_FRONT]);
  DELETE(m_pChildren[BOTTOM_RIGHT_BACK]);

  m_pNeighbors[TOP] = NULL;
  m_pNeighbors[BOTTOM] = NULL;
  m_pNeighbors[LEFT] = NULL;
  m_pNeighbors[RIGHT] = NULL;
  m_pNeighbors[FRONT] = NULL;
  m_pNeighbors[BACK] = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctree<TYPE> *
Phoenix::Spatial::COctree<TYPE>::GetChild( Phoenix::Spatial::OCTREE_SECTION iSection )
{
  return m_pChildren[iSection];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctree<TYPE>::SetChild( Phoenix::Spatial::OCTREE_SECTION iSection, COctree<TYPE> *pNode)
{
  m_pChildren[iSection] = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctree<TYPE>::DeleteNode( Phoenix::Spatial::OCTREE_SECTION iSection )
{
  DELETE(m_pChildren[iSection]);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctree<TYPE> **
Phoenix::Spatial::COctree<TYPE>::GetNodes(){

  return m_pChildren;
} 
/////////////////////////////////////////////////////////////////
template<typename TYPE>
std::list< TYPE > & 
Phoenix::Spatial::COctree<TYPE>::GetObjects()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctree<TYPE>::SetNeighbor( Phoenix::Spatial::OCTREE_NEIGHBOR iNeighbor, COctree<TYPE> *pNode )
{
  m_pNeighbors[iNeighbor] = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctree<TYPE>::COctree<TYPE> * 
Phoenix::Spatial::COctree<TYPE>::GetNeighbor( Phoenix::Spatial::OCTREE_NEIGHBOR iNeighbor )
{
  return m_pNeighbors[iNeighbor];
}
/////////////////////////////////////////////////////////////////
#endif
