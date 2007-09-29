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
    /// Octreenode class template.
    template<typename TYPE>
    class COctreeNode : public Phoenix::Volume::CAxisAlignedCube
    {
    protected:
      std::list< TYPE > m_lstObjects;
      /// Children of this node.
      COctreeNode * m_pChildren[8];
      /// Neighbor objects of this node.
      COctreeNode * m_pNeighbors[6];
      /// Parent of this node.
      COctreeNode * m_pParent;
      /// Do (grand)child nodes have objects in them.
      int	    m_bChildrenContainObjects;
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
      ////////////////////
      /// Assigns parent node.
      /// \param pNode Pointer to parent node.
      void SetParent( COctreeNode *pNode );
      ////////////////////
      /// Returns pointer to parent node.
      /// \returns Pointer to parent node.
      COctreeNode * GetParent();
      ////////////////////
      /// Do child nodes have objects in them.
      /// \returns Boolean true if children contain objects
      /// \returns Boolean false if children do not contain objects. 
      int ChildrenContainObjects() const;
      ////////////////////
      /// Checks do child nodes contain nodes.
      void CheckDoChildrenContainObjects();
    };/// class COctreeNode
    /////////////////////////////////////////////////////////////////
    /// Octree class template.
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
      /// \param nDepth Maximum depth of this tree.
      /// \param fWorldSize Max world size.
      COctree( unsigned int nDepth, float fWorldSize );
      ////////////////////
      /// Returns depth of tree.
      /// \param fRadius Bounding sphere radius.
      /// \returns Lowest level of tree to accomodate radius.
      unsigned int GetObjectDepth( float fRadius );
      ////////////////////
      /// Returns world size.
      /// \returns World size.
      float GetWorldSize() const;
      ////////////////////
      /// Returns world size / 2
      /// \returns Half of world size.
      float GetWorldHalfSize() const;
      ////////////////////
      /// Returns edge length of octree cube at tree depth.
      /// \param nDepth From which depth the edge length is retrieved.
      /// \returns Edge length.
      float GetEdgeLength( unsigned int nDepth );
      ////////////////////
      /// Returns index of closest node to position value at depth.
      /// \param fValue Coordinate value, in range of (-world / 2.0, world/2.0)
      /// \param nDepth From which tree depth value is determined.
      unsigned int GetIndex( float fValue, unsigned nDepth );
      ////////////////////
      /// Returns root node.
      /// \returns Pointer to root node.
      COctreeNode<TYPE> *GetRoot();
      ////////////////////
      /// Returns node at desired level, closest to given coordinates.
      /// \param nLevel Desired depth level
      /// \param fX X-coordinate. Must be +-World/2
      /// \param fY Y-coordinate. Must be +-World/2
      /// \param fZ Z-coordinate. Must be +-World/2
      /// \returns Pointer to node.
      COctreeNode<TYPE> * GetNode( unsigned int nLevel, float fX, float fY, float fZ );
      ////////////////////
      /// Inserts object into tree.
      /// \param object Reference to object to be inserted.
      /// \param boundingSphere Bounding sphere of object.
      void InsertObject( const TYPE & object, const Phoenix::Volume::CSphere & boundingSphere);
      ////////////////////
      /// Delets object from tree.
      /// \param object Reference to object to be removed.
      /// \param boundingSphere Bounding sphere of object.
      /// \returns 1 On error.
      /// \returns 0 Otherwise.
      int  DeleteObject( const TYPE & object, const Phoenix::Volume::CSphere & boundingSphere);
    protected:
      ////////////////////
      /// Sets world size.
      /// \param fSize Size of world.
      void SetWorldSize( float fSize );
      ////////////////////
      /// Sets maximum depth of this tree.
      /// \param nDepth Max depth.
      void SetMaxDepth( unsigned int nDepth );
      ////////////////////
      /// Returns max depth of tree.
      /// \return Maximum depth.
      unsigned int GetMaxDepth();
      ////////////////////
      /// Initializes nodes recursively.
      /// \param nLevel Which level is used.
      /// \param vPosition Center position for node.
      /// \param pNode Which node is initialized.
      void Initialize( unsigned int nLevel, 
		       const Phoenix::Math::CVector3<float> & vPosition, 
		       COctreeNode<TYPE> *pNode);
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
Phoenix::Spatial::COctreeNode<TYPE>::COctreeNode() : CAxisAlignedCube()
{
  m_bChildrenContainObjects = 0;
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
inline int
Phoenix::Spatial::COctreeNode<TYPE>::ChildrenContainObjects() const
{
  return m_bChildrenContainObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctreeNode<TYPE>::CheckDoChildrenContainObjects() 
{
  // if one of child nodes is NULL, others are too.
  if ( m_pChildren[TOP_LEFT_FRONT] == NULL)
  {
    m_bChildrenContainObjects = 0;
  }
  else
  {
    m_bChildrenContainObjects = (m_pChildren[TOP_LEFT_BACK]->ChildrenContainObjects()      ||
				 m_pChildren[TOP_RIGHT_FRONT]->ChildrenContainObjects()    ||
				 m_pChildren[TOP_RIGHT_BACK]->ChildrenContainObjects()     ||
				 m_pChildren[BOTTOM_LEFT_FRONT]->ChildrenContainObjects()  ||
				 m_pChildren[BOTTOM_LEFT_BACK]->ChildrenContainObjects()   ||
				 m_pChildren[BOTTOM_RIGHT_FRONT]->ChildrenContainObjects() ||
				 m_pChildren[BOTTOM_RIGHT_BACK]->ChildrenContainObjects() );
  }
  
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
template<typename TYPE>
inline void
Phoenix::Spatial::COctreeNode<TYPE>::SetParent( Phoenix::Spatial::COctreeNode<TYPE> *pNode )
{
  m_pParent = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctreeNode<TYPE>::GetParent()
{
  return m_pParent;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctree<TYPE>::COctree( unsigned int nDepth, float fWorldSize )
{
  unsigned int nNodeCount = (unsigned int)((1-powf(8,nDepth))/-7);
  SetMaxDepth(nDepth);
  SetWorldSize(fWorldSize);
  // Allocate array for all nodes
  m_pAllNodes = new COctreeNode<TYPE>[nNodeCount];
  Initialize( 0, Phoenix::Math::CVector3<float>(0,0,0), &m_pAllNodes[0]);  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int 
Phoenix::Spatial::COctree<TYPE>::GetObjectDepth( float fRadius )
{
  return static_cast<unsigned int>(floorf( Phoenix::Math::Log2( GetWorldSize()/fRadius)));
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline float 
Phoenix::Spatial::COctree<TYPE>::GetWorldSize() const
{
  return GetWidth();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline float 
Phoenix::Spatial::COctree<TYPE>::GetWorldHalfSize() const
{
  return GetHalfWidth();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline float 
Phoenix::Spatial::COctree<TYPE>::GetEdgeLength( unsigned int nDepth )
{
  return GetWorldSize() / powf(2, nDepth);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int 
Phoenix::Spatial::COctree<TYPE>::GetIndex( float fValue, unsigned nDepth )
{
  unsigned int nDimensions = floorf(((float)nDepth) *0.333333333333333f);
  if ( fValue >= GetWorldHalfSize()) return nDimensions-1;
  else if ( fValue <= -GetWorldHalfSize()) return 0;
  return static_cast<unsigned int>(floorf( ((fValue * m_fOneDivWorldSize) + 0.5f)* powf(8.0f, nDimensions)));  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctree<TYPE>::GetRoot()
{
  return &m_pAllNodes[0];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctree<TYPE>::GetNode( unsigned int nLevel, float fX, float fY, float fZ )
{
  unsigned int nNodeCountPrevLevels = (unsigned int)floor( (1-powf(8,nLevel))/-7);
  //std::cerr << "at level : " << nLevel << " #prevnodes : " << nNodeCountPrevLevels << std::endl;
  unsigned int nDimensions = (unsigned int)powf(8.0f, ((float)nLevel) *0.333333333333333f);
  float fDimensions = (float)nDimensions;
  unsigned int nX = (unsigned int)floorf( ((fX * m_fOneDivWorldSize) + 0.5f) * fDimensions );
  unsigned int nY = (unsigned int)floorf( ((fY * m_fOneDivWorldSize) + 0.5f) * fDimensions );
  unsigned int nZ = (unsigned int)floorf( ((fZ * m_fOneDivWorldSize) + 0.5f) * fDimensions );
  unsigned int nTotalIndex = nNodeCountPrevLevels + nX * nDimensions * nDimensions + nY * nDimensions + nZ;
  //std::cerr << "final index : " << nTotalIndex << std::endl;
  return &m_pAllNodes[nTotalIndex];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Phoenix::Spatial::COctree<TYPE>::SetWorldSize( float fSize )
{
  SetWidth(fSize);
  if ( TOO_CLOSE_TO_ZERO(fSize))
    m_fOneDivWorldSize = 0.0f;
  else
    m_fOneDivWorldSize = 1.0f / fSize;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Phoenix::Spatial::COctree<TYPE>::SetMaxDepth( unsigned int nDepth )
{
  m_nDepth = nDepth;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int 
Phoenix::Spatial::COctree<TYPE>::GetMaxDepth()
{
  return m_nDepth;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void 
Phoenix::Spatial::COctree<TYPE>::Initialize( unsigned int nLevel, const Phoenix::Math::CVector3<float> & vPosition, COctreeNode<TYPE> *pNode)
{
  //std::cerr << "Starting init of : " << pNode << " at level " << nLevel << std::endl;
  float fEdgeLength  = GetEdgeLength( nLevel );
  pNode->SetPosition( vPosition );
  pNode->SetWidth( 2.0f*fEdgeLength);
  
  float fChildCenterPosDiff = fEdgeLength * 0.25f;

  COctreeNode<TYPE> *pChild = NULL;
  // When desired levels have been reached, we stop recursion.
  if ( (nLevel + 1) >= m_nDepth ) return;

  Phoenix::Math::CVector3<float> vNewPos;
  
  ////////////////////
  // 1st	
  // calculate center positions for each child node 
  vNewPos[0] = vPosition[0] - fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] - fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] - fChildCenterPosDiff;


  // Get correct node from next level
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);

  //std::cerr << "node at " << nLevel+1 << " at " << vNewPos << " is " << pChild << std::endl;  
  // Set child/parent pointers
  pNode->SetChild( Phoenix::Spatial::BOTTOM_LEFT_BACK, pChild);
  pChild->SetParent( pNode );
  // Initialize child recursively.
  Initialize( nLevel + 1, vNewPos, pChild);

  // 2nd	
  vNewPos[0] = vPosition[0] - fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] - fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Phoenix::Spatial::BOTTOM_LEFT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);

  // 3rd	
  vNewPos[0] = vPosition[0] - fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] - fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Phoenix::Spatial::BOTTOM_RIGHT_BACK, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 4th
  vNewPos[0] = vPosition[0] - fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Phoenix::Spatial::BOTTOM_RIGHT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 5th	
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] - fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] - fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Phoenix::Spatial::TOP_LEFT_BACK, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 6th	
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] - fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Phoenix::Spatial::TOP_LEFT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 7th	
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] - fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Phoenix::Spatial::TOP_RIGHT_BACK, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 8th	
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Phoenix::Spatial::TOP_RIGHT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Phoenix::Spatial::COctree<TYPE>::InsertObject( const TYPE & object, const Phoenix::Volume::CSphere & boundingSphere)
{
  unsigned int nLevel = GetObjectDepth( boundingSphere.GetRadius());
  if ( nLevel >= GetMaxDepth() ) nLevel = GetMaxDepth()-1;
  COctreeNode<TYPE> *pNode = GetNode( nLevel, 
				      boundingSphere.GetPosition()[0], 
				      boundingSphere.GetPosition()[1], 
				      boundingSphere.GetPosition()[2]);
  
  if ( pNode->GetObjects().size() == 0)
  {
    pNode->GetObjects().push_back( object );
    pNode = pNode->GetParent();
    while ( pNode != NULL )
    {
      std::cerr << " Currently processing " << pNode << std::endl;
      pNode->CheckDoChildrenContainObjects();
      pNode = pNode->GetParent();

    }
    
  }
  else
  {
    pNode->GetObjects().push_back( object );
  }
  
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
int
Phoenix::Spatial::COctree<TYPE>::DeleteObject( const TYPE & object, const Phoenix::Volume::CSphere & boundingSphere)
{
  int iRetval = 1;
  unsigned int nLevel = GetObjectDepth( boundingSphere.GetRadius());
 if ( nLevel >= GetMaxDepth() ) nLevel = GetMaxDepth()-1;
  COctreeNode<TYPE> *pNode = GetNode( nLevel, 
				      boundingSphere.GetPosition()[0], 
				      boundingSphere.GetPosition()[1], 
				      boundingSphere.GetPosition()[2]);
  // No objects, no deleting.
  if ( pNode->GetObjects().empty() ) return 1;
  
  typename std::list<TYPE>::iterator it = pNode->GetObjects().begin();
  for( ; it != pNode->GetObjects().end();it++)
  {
    if ( *it == object )
    {
      pNode->GetObjects().erase(it);
      iRetval = 0;
      break;
    }
  }
  
  if ( pNode->GetObjects().empty() )
  {
    pNode->CheckDoChildrenContainObjects();
    pNode = pNode->GetParent();
    while ( pNode != NULL )
    {
      pNode->CheckDoChildrenContainObjects();
      pNode = pNode->GetParent();
    }
  }

  return iRetval;
}
/////////////////////////////////////////////////////////////////
#endif
