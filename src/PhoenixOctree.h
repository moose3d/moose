#ifndef __PhoenixOctree_h__
#define __PhoenixOctree_h__
/////////////////////////////////////////////////////////////////
#include <list>
#include "PhoenixSpatial.h"
#include "PhoenixVolume.h"
#include <iostream>
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
    class COctreeNode : public Phoenix::Spatial::CDimensional1D,
                        public Phoenix::Spatial::CPositional
    {
    protected:
      std::list< TYPE > m_lstObjects;
      /// Children of this node.
      COctreeNode * m_pChildren[8];
      /// Neighbor objects of this node.
      COctreeNode * m_pNeighbors[6];
      /// Parent of this node.
      COctreeNode * m_pParent;
      
    public:
      ////////////////////
      /// The constructor.
      COctreeNode();
      ////////////////////
      /// Destructor.
      ~COctreeNode();
      ////////////////////
      /// Index accessor method.
      /// \param iSection which node will be returned.
      /// \returns Pointer to child node.
      COctreeNode * GetChild( OCTREE_SECTION iSection );
      ////////////////////
      /// Removes node from section iSection.
      /// \param iSection Section to be deleted.
      void DeleteNode( OCTREE_SECTION iSection);
      ////////////////////
      /// Assigns node into section.
      /// \param iSection Section to be inserted.
      /// \param pNode Pointer to a node to be inserted.
      void SetChild( OCTREE_SECTION iSection, COctreeNode *pNode );
      ////////////////////
      /// Assigns node neighbor.
      /// \param iNeighbor As which neightbor node is put.
      /// \param pNode Pointer to a octree node.
      void SetNeighbor( OCTREE_NEIGHBOR iNeighbor, COctreeNode *pNode );
      ////////////////////
      /// Returns neighbor node.
      /// \param iNeighbor Which neighbor is returned.
      /// \returns Pointer to neighbor node.
      COctreeNode * GetNeighbor( OCTREE_NEIGHBOR iNeighbor );
      ////////////////////
      /// Returns children.
      /// \returns Pointer to the node array.
      COctreeNode ** GetNodes();
      ////////////////////
      /// Returns a list of objects in this node.      
      /// \returns Reference to a list of objects.
      std::list< TYPE > & GetObjects();

      void SetParent( COctreeNode *pNode )
      {
	m_pParent = pNode;
      }
    };/// class COctreeNode
    
    template<typename TYPE>
    class COctree : protected Phoenix::Spatial::CDimensional1D
    {
    protected:
      unsigned int      m_nDepth;
      float	        m_fOneDivWorldSize;
      COctreeNode<TYPE>      *m_pAllNodes;
    public:
      ////////////////////
      /// Constructor.
      COctree( unsigned int nDepth, float fWorldSize ) 
      {
	unsigned int nNodeCount = (unsigned int)((1-powf(8,nDepth))/-7);
	SetMaxDepth(nDepth);
	SetWorldSize(fWorldSize);
	// Allocate array for all nodes
	m_pAllNodes = new COctreeNode<TYPE>[nNodeCount];
	
	Initialize( 0, Phoenix::Math::CVector3<float>(0,0,0), &m_pAllNodes[0]);
	
      }
      ////////////////////
      /// Returns depth of tree.
      /// \param fRadius Bounding sphere radius.
      /// \returns Lowest level of tree to accomodate radius.
      unsigned int GetObjectDepth( float fRadius )
      {
	return static_cast<unsigned int>(floorf( Phoenix::Math::Log2( GetWorldSize()/fRadius)));
      }
      ////////////////////
      /// Returns world size.
      /// \returns World size.
      inline float GetWorldSize() const
      {
	return GetWidth();
      }
      ////////////////////
      /// Returns world size / 2
      /// \returns Half of world size.
      inline float GetWorldHalfSize() const
      {
	return GetHalfWidth();
      }
      ////////////////////
      /// Returns edge length of octree cube at tree depth.
      /// \param nDepth From which depth the edge length is retrieved.
      /// \returns Edge length.
      inline float GetEdgeLength( unsigned int nDepth )
      {
	return GetWorldSize() / powf(2, nDepth);
      }
      ////////////////////
      /// Returns index of closest node to position value at depth.
      /// \param fValue Coordinate value, in range of (-world / 2.0, world/2.0)
      /// \param nDepth From which tree depth value is determined.
      inline unsigned int GetIndex( float fValue, unsigned nDepth )
      {
	unsigned int nDimensions = floorf(((float)nDepth) *0.333333333333333f);
	if ( fValue >= GetWorldHalfSize()) return nDimensions-1;
	else if ( fValue <= -GetWorldHalfSize()) return 0;
	return static_cast<unsigned int>(floorf( ((fValue * m_fOneDivWorldSize) + 0.5f)* powf(8.0f, nDimensions)));
      }
      inline COctreeNode<TYPE> *GetRoot()
      {
	return &m_pAllNodes[0];
      }
      ////////////////////
      /// Returns node at desired level, closest to given coordinates.
      /// \param nLevel Desired depth level
      /// \param fX X-coordinate. Must be +-World/2
      /// \param fY Y-coordinate. Must be +-World/2
      /// \param fZ Z-coordinate. Must be +-World/2
      /// \returns Pointer to node.
      inline COctreeNode<TYPE> * GetNode( unsigned int nLevel, float fX, float fY, float fZ )
      {
	unsigned int nNodeCountPrevLevels = (unsigned int)(1-floorf(powf(8.0f,nLevel))/-7);

	unsigned int nDimensions = (unsigned int)powf(8.0f, ((float)nLevel) *0.333333333333333f);
	std::cerr << "Dimensions: " << nDimensions << std::endl;
	unsigned int nNodeCountThisLevel = (unsigned int)powf(8.0f, nLevel);
	std::cerr << "#nodes: " << nNodeCountThisLevel << std::endl;
	unsigned int nX = static_cast<unsigned int>(floorf( ((fX * m_fOneDivWorldSize) + 0.5f)* nNodeCountThisLevel ));
	unsigned int nY = static_cast<unsigned int>(floorf( ((fY * m_fOneDivWorldSize) + 0.5f)* nNodeCountThisLevel ));
	unsigned int nZ = static_cast<unsigned int>(floorf( ((fZ * m_fOneDivWorldSize) + 0.5f)* nNodeCountThisLevel ));
	
	return &m_pAllNodes[nNodeCountPrevLevels + nX * nDimensions * nDimensions + nY * nDimensions + nZ];
      }
    protected:
      ////////////////////
      /// Sets world size.
      /// \param fSize Size of world.
      inline void SetWorldSize( float fSize )
      {
	SetWidth(fSize);
	if ( TOO_CLOSE_TO_ZERO(fSize))
	  m_fOneDivWorldSize = 0.0f;
	else
	  m_fOneDivWorldSize = 1.0f / fSize;
      }
      ////////////////////
      /// Sets maximum depth of this tree.
      /// \parm nDepth Max depth.
      inline void SetMaxDepth( unsigned int nDepth )
      {
	m_nDepth = nDepth;
      }
      
      void Initialize( unsigned int nLevel, const Phoenix::Math::CVector3<float> & vPosition, COctreeNode<TYPE> *pNode)
      {
	pNode->SetPosition( vPosition );
	float fEdgeLengthChild = GetEdgeLength( nLevel ) * 0.5f;

	COctreeNode<TYPE> *pChild = NULL;
	// When desired levels have been reached, we stop recursion.
	if ( (nLevel + 1) >= m_nDepth ) return;

	Phoenix::Math::CVector3<float> vNewPos;
	
	// 1st	
	vNewPos[0] = vPosition[0] - fEdgeLengthChild;
	vNewPos[1] = vPosition[1] - fEdgeLengthChild;
	vNewPos[2] = vPosition[2] - fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::BOTTOM_LEFT_BACK, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);

	// 2nd	
	vNewPos[0] = vPosition[0] - fEdgeLengthChild;
	vNewPos[1] = vPosition[1] - fEdgeLengthChild;
	vNewPos[2] = vPosition[2] + fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::BOTTOM_LEFT_FRONT, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);



	// 3rd	
	vNewPos[0] = vPosition[0] - fEdgeLengthChild;
	vNewPos[1] = vPosition[1] + fEdgeLengthChild;
	vNewPos[2] = vPosition[2] - fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::BOTTOM_RIGHT_BACK, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);


	// 4th
	vNewPos[0] = vPosition[0] - fEdgeLengthChild;
	vNewPos[1] = vPosition[1] + fEdgeLengthChild;
	vNewPos[2] = vPosition[2] + fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::BOTTOM_RIGHT_FRONT, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);


	// 5th	
	vNewPos[0] = vPosition[0] + fEdgeLengthChild;
	vNewPos[1] = vPosition[1] - fEdgeLengthChild;
	vNewPos[2] = vPosition[2] - fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::TOP_LEFT_BACK, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);

	// 6th	
	vNewPos[0] = vPosition[0] + fEdgeLengthChild;
	vNewPos[1] = vPosition[1] - fEdgeLengthChild;
	vNewPos[2] = vPosition[2] + fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::TOP_LEFT_FRONT, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);


	// 7th	
	vNewPos[0] = vPosition[0] + fEdgeLengthChild;
	vNewPos[1] = vPosition[1] + fEdgeLengthChild;
	vNewPos[2] = vPosition[2] - fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::TOP_RIGHT_BACK, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);


	// 8th	
	vNewPos[0] = vPosition[0] + fEdgeLengthChild;
	vNewPos[1] = vPosition[1] + fEdgeLengthChild;
	vNewPos[2] = vPosition[2] + fEdgeLengthChild;
	pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
	pNode->SetChild( Phoenix::Spatial::TOP_RIGHT_FRONT, pChild);
	pChild->SetParent( pNode );
	Initialize( nLevel + 1, vNewPos, pChild);

      }
    };
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
Phoenix::Spatial::COctreeNode<TYPE>::COctreeNode() : CDimensional1D(), CPositional()
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

  m_pParent = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctreeNode<TYPE>::~COctreeNode()
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

  m_pParent = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctreeNode<TYPE>::GetChild( Phoenix::Spatial::OCTREE_SECTION iSection )
{
  return m_pChildren[iSection];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctreeNode<TYPE>::SetChild( Phoenix::Spatial::OCTREE_SECTION iSection, COctreeNode<TYPE> *pNode)
{
  m_pChildren[iSection] = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctreeNode<TYPE>::DeleteNode( Phoenix::Spatial::OCTREE_SECTION iSection )
{
  DELETE(m_pChildren[iSection]);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctreeNode<TYPE> **
Phoenix::Spatial::COctreeNode<TYPE>::GetNodes(){

  return m_pChildren;
} 
/////////////////////////////////////////////////////////////////
template<typename TYPE>
std::list< TYPE > & 
Phoenix::Spatial::COctreeNode<TYPE>::GetObjects()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctreeNode<TYPE>::SetNeighbor( Phoenix::Spatial::OCTREE_NEIGHBOR iNeighbor, COctreeNode<TYPE> *pNode )
{
  m_pNeighbors[iNeighbor] = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctreeNode<TYPE>::COctreeNode<TYPE> * 
Phoenix::Spatial::COctreeNode<TYPE>::GetNeighbor( Phoenix::Spatial::OCTREE_NEIGHBOR iNeighbor )
{
  return m_pNeighbors[iNeighbor];
}
/////////////////////////////////////////////////////////////////
#endif
