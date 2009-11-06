#ifndef __PhoenixOctree_h__
#define __PhoenixOctree_h__
/////////////////////////////////////////////////////////////////
#include <list>
#include <iostream>
#include "PhoenixSpatial.h"
#include "PhoenixVolume.h"
#include "PhoenixCamera.h"
#include "PhoenixCollision.h"
#include "PhoenixGlobals.h"
#include "PhoenixAPI.h"
#include "PhoenixTagged.h"
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
    class PHOENIX_API COctreeNode : public Phoenix::Volume::CAxisAlignedCube
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
      virtual ~COctreeNode();
      ////////////////////
      /// Index accessor method.
      /// \param iSection which node will be returned.
      /// \returns Pointer to child node.
      COctreeNode * GetChild( OCTREE_SECTION iSection );
      ////////////////////
      /// Index accessor method.
      /// \param iSection which node will be returned.
      /// \returns Pointer to child node.
      const COctreeNode * GetChild( OCTREE_SECTION iSection ) const;
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
      /// Returns a list of objects in this node.
      /// \returns Reference to a list of objects.
      const std::list< TYPE > & GetObjects() const;
      ////////////////////
      /// Deletes given object from this node.
      /// \param object Object to be removed.
      /// \returns Zero if successfull.
      /// \returns Non-zero if unsuccessfull.
      int DeleteObject( const TYPE & object );
      ////////////////////
      /// Adds object to this node.
      /// \param object Object to be removed.
      void AddObject( const TYPE & object );
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
      ////////////////////
      ///  Checks does this node have objects in it.
      /// \returns non-zero, if it does.
      /// \returns zero otherwise.
      int HasObjects() const;

    };/// class COctreeNode
    /////////////////////////////////////////////////////////////////
    /// Octree class template.
    template<typename TYPE>
    class PHOENIX_API COctree : protected Phoenix::Spatial::CDimensional1D
    {
    protected:
      /// Number of total nodes in this Octree.
      size_t		m_nNodeCount;
      unsigned int      m_nDepth;
      float	        m_fOneDivWorldSize;
      COctreeNode<TYPE>      *m_pAllNodes;
    public:
      ////////////////////
      /// Constructor.
      /// \param nNumLevels Maximum number of levels in this tree.
      /// \param fWorldSize Max world size.
      COctree( unsigned int nNumLevels, float fWorldSize );
      virtual ~COctree();
      ////////////////////
      /// Returns total node count in this octree.
      size_t GetNodeCount() const;
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
      /// Returns edge length of normal (non-loose) octree cube at given depth.
      /// \param nDepth From which depth the edge length is retrieved.
      /// \returns Edge length.
      float GetEdgeLength( unsigned int nDepth );
      ////////////////////
      /// Returns index of closest node to position value at depth.
      /// \param fValue Coordinate value, in range of (-world / 2.0, world/2.0)
      /// \param nDepth From which tree depth value is determined.
      unsigned int GetIndex( float fValue, unsigned nDepth );
      ////////////////////
      /// Returns index of closest node to position value at depth.
      /// \param nLevel Desired depth level
      /// \param fX X-coordinate. Must be within +-World/2
      /// \param fY Y-coordinate. Must be within +-World/2
      /// \param fZ Z-coordinate. Must be within +-World/2
      /// \returns Index in 1-dimensional array.
      unsigned int GetIndex1D( unsigned int nLevel, float fX, float fY, float fZ );
      ////////////////////
      /// Returns index of closest node to position value at depth.
      /// \param nLevel Desired depth level
      /// \param vPosition Object position, must be within +-World/2.
      /// \returns Index in 1-dimensional array.
      unsigned int GetIndex1D( unsigned int nLevel, const Phoenix::Math::CVector3<float> & vPosition );
      ////////////////////
      /// Returns root node.
      /// \returns Pointer to root node.
      COctreeNode<TYPE> *GetRoot();
      ////////////////////
      /// Returns root node.
      /// \returns Pointer to root node.
      const COctreeNode<TYPE> *GetRoot() const;
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
      /// \returns Index where object was inserted.
      unsigned int InsertObject( const TYPE & object,
				 const Phoenix::Volume::CSphere & boundingSphere);

      ////////////////////
      /// Inserts object into tree.
      /// \param object Reference to object to be inserted.
      /// \param vPosition Position of bounding sphere of the object.
      /// \param fRadius Radius of bounding sphere.
      /// \returns Index where object was inserted.
      unsigned int InsertObject( const TYPE & object,
				 const Phoenix::Math::CVector3<float> & vPosition,
				 float fRadius);
      ////////////////////
      /// Delets object from tree.
      /// \param object Reference to object to be removed.
      /// \param boundingSphere Bounding sphere of object.
      /// \returns 1 On error.
      /// \returns 0 Otherwise.
      int  DeleteObject( const TYPE & object, const Phoenix::Volume::CSphere & boundingSphere);
      ////////////////////
      /// Returns node at index.
      /// \param nIndex From which index node is returned.
      /// \warn This must be between 0 and size of m_pAllNodes, otherwise
      /// \warn segfaults are weird bugs shall cross your path!!
      /// \returns Pointer to OctreeNode.
      COctreeNode<TYPE> * GetNodeAt(unsigned int nIndex);
      ////////////////////
      /// Updates child statuses from this node towards root.
      /// \param pNode Recently added child node.
      void PropagateChildrenStatus( COctreeNode<TYPE> *pNode );
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
#define PHOENIX_DELETE(A) delete A; A = NULL;
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
  PHOENIX_DELETE(m_pChildren[TOP_LEFT_FRONT]);
  PHOENIX_DELETE(m_pChildren[TOP_LEFT_BACK]);
  PHOENIX_DELETE(m_pChildren[TOP_RIGHT_FRONT]);
  PHOENIX_DELETE(m_pChildren[TOP_RIGHT_BACK]);
  PHOENIX_DELETE(m_pChildren[BOTTOM_LEFT_FRONT]);
  PHOENIX_DELETE(m_pChildren[BOTTOM_LEFT_BACK]);
  PHOENIX_DELETE(m_pChildren[BOTTOM_RIGHT_FRONT]);
  PHOENIX_DELETE(m_pChildren[BOTTOM_RIGHT_BACK]);

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
inline Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctreeNode<TYPE>::GetChild( Phoenix::Spatial::OCTREE_SECTION iSection )
{
  return m_pChildren[iSection];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctreeNode<TYPE>::GetChild( Phoenix::Spatial::OCTREE_SECTION iSection ) const
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
    m_bChildrenContainObjects = (m_pChildren[TOP_LEFT_FRONT]->HasObjects()     || m_pChildren[TOP_LEFT_FRONT]->ChildrenContainObjects()     ||
				 m_pChildren[TOP_LEFT_BACK]->HasObjects()      || m_pChildren[TOP_LEFT_BACK]->ChildrenContainObjects()      ||
				 m_pChildren[TOP_RIGHT_FRONT]->HasObjects()    || m_pChildren[TOP_RIGHT_FRONT]->ChildrenContainObjects()    ||
				 m_pChildren[TOP_RIGHT_BACK]->HasObjects()     || m_pChildren[TOP_RIGHT_BACK]->ChildrenContainObjects()     ||
				 m_pChildren[BOTTOM_LEFT_FRONT]->HasObjects()  || m_pChildren[BOTTOM_LEFT_FRONT]->ChildrenContainObjects()  ||
				 m_pChildren[BOTTOM_LEFT_BACK]->HasObjects()   || m_pChildren[BOTTOM_LEFT_BACK]->ChildrenContainObjects()   ||
				 m_pChildren[BOTTOM_RIGHT_FRONT]->HasObjects() || m_pChildren[BOTTOM_RIGHT_FRONT]->ChildrenContainObjects() ||
				 m_pChildren[BOTTOM_RIGHT_BACK]->HasObjects()  || m_pChildren[BOTTOM_RIGHT_BACK]->ChildrenContainObjects() );
  }

}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Phoenix::Spatial::COctreeNode<TYPE>::SetChild( Phoenix::Spatial::OCTREE_SECTION iSection, COctreeNode<TYPE> *pNode)
{
  m_pChildren[iSection] = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
inline Phoenix::Spatial::COctreeNode<TYPE>::DeleteNode( Phoenix::Spatial::OCTREE_SECTION iSection )
{
  PHOENIX_DELETE(m_pChildren[iSection]);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Spatial::COctreeNode<TYPE> **
Phoenix::Spatial::COctreeNode<TYPE>::GetNodes(){

  return m_pChildren;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline std::list< TYPE > &
Phoenix::Spatial::COctreeNode<TYPE>::GetObjects()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const std::list< TYPE > &
Phoenix::Spatial::COctreeNode<TYPE>::GetObjects() const
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
int
Phoenix::Spatial::COctreeNode<TYPE>::DeleteObject( const TYPE & object)
{
  int iRetval = 1;
  typename std::list< TYPE >::iterator it = m_lstObjects.begin();
  for( ; it != m_lstObjects.end(); it++)
  {
    if ( *it == object )
    {
      //std::cerr << "erasing " << *it << std::endl;
      m_lstObjects.erase( it );
      iRetval = 0;
      break;
    }
  }
  return iRetval;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Phoenix::Spatial::COctreeNode<TYPE>::AddObject( const TYPE &object )
{
  m_lstObjects.push_back(object);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Phoenix::Spatial::COctreeNode<TYPE>::SetNeighbor( Phoenix::Spatial::OCTREE_NEIGHBOR iNeighbor, COctreeNode<TYPE> *pNode )
{
  m_pNeighbors[iNeighbor] = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Spatial::COctreeNode<TYPE> *
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
Phoenix::Spatial::COctree<TYPE>::COctree( unsigned int nNumLevels, float fWorldSize )
{
  m_nNodeCount = (unsigned int)((1-powf(8,nNumLevels))/-7);
  // Maximum depth will be levels-1, ie. 4 = { 0, 1, 2, 3 }
  SetMaxDepth(nNumLevels-1);
  SetWorldSize(fWorldSize);
  // Allocate array for all nodes
  m_pAllNodes = new COctreeNode<TYPE>[m_nNodeCount];
  Initialize( 0, Phoenix::Math::CVector3<float>(0,0,0), &m_pAllNodes[0]);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Phoenix::Spatial::COctree<TYPE>::~COctree()
{
  delete [] m_pAllNodes;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline size_t
Phoenix::Spatial::COctree<TYPE>::GetNodeCount() const
{
  return m_nNodeCount;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Phoenix::Spatial::COctree<TYPE>::GetObjectDepth( float fRadius )
{
  unsigned int nDepth = static_cast<unsigned int>(floorf( Phoenix::Math::Log2( GetWorldSize()/fRadius)));
  return nDepth < GetMaxDepth() ? nDepth : GetMaxDepth();
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
  unsigned int nDimensions = (unsigned int)powf(8.0f, floorf(((float)nDepth) *0.333333333333333f));
  if (      fValue >= GetWorldHalfSize()-0.001f) return nDimensions-1;
  else if ( fValue <= -GetWorldHalfSize()) return 0;
  return static_cast<unsigned int>(floorf( ((fValue * m_fOneDivWorldSize) + 0.5f)* nDimensions));
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
inline const Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctree<TYPE>::GetRoot() const
{
  return &m_pAllNodes[0];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Phoenix::Spatial::COctree<TYPE>::GetIndex1D( unsigned int nLevel, float fX, float fY, float fZ )
{
  //if ( nLevel == 0 ) return 0;
  unsigned int nNodeCountPrevLevels = (unsigned int)floor( (1-powf(8,nLevel))/-7);

  // How many cubes there are per axis, ie. level 0 = 1, level 1 = 2, level 2 = 4, ...
  float fDimensions = powf(8.0f, ((float)nLevel) *0.333333333333333f);
  unsigned int nDimensions = (unsigned int)fDimensions;

  unsigned int nX;
  unsigned int nY;
  unsigned int nZ;
  //std::cerr << "GetIndex1D: " << GetWorldHalfSize() << std::endl;
  // Sanity checks, too large coordinate will set indices way beyond array limits.
  if (      fX >=  GetWorldHalfSize()-0.001f) nX = nDimensions - 1;
  else if ( fX <= -GetWorldHalfSize()) nX = 0;
  else      nX = (unsigned int)floorf( ((fX * m_fOneDivWorldSize) + 0.5f) * fDimensions );

  if (      fY >=  GetWorldHalfSize()-0.001f) nY = nDimensions - 1;
  else if ( fY <= -GetWorldHalfSize()) nY = 0;
  else	    nY = (unsigned int)floorf( ((fY * m_fOneDivWorldSize) + 0.5f) * fDimensions );

  if (      fZ >=  GetWorldHalfSize()-0.001f) nZ = nDimensions - 1;
  else if ( fZ <= -GetWorldHalfSize()) nZ = 0;
  else      nZ = (unsigned int)floorf( ((fZ * m_fOneDivWorldSize) + 0.5f) * fDimensions );

  return nNodeCountPrevLevels + nX * nDimensions * nDimensions + nY * nDimensions + nZ;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Phoenix::Spatial::COctree<TYPE>::GetIndex1D( unsigned int nLevel, const Phoenix::Math::CVector3<float> & vPosition )
{
  return GetIndex1D(nLevel, vPosition[0], vPosition[1], vPosition[2]);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctree<TYPE>::GetNode( unsigned int nLevel, float fX, float fY, float fZ )
{
  return &m_pAllNodes[GetIndex1D(nLevel, fX, fY, fZ)];
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
  float fEdgeLength = GetEdgeLength( nLevel );
  pNode->SetPosition( vPosition );
  pNode->SetWidth( 2.0f*fEdgeLength);

  float fChildCenterPosDiff = fEdgeLength * 0.25f;

  COctreeNode<TYPE> *pChild = NULL;
  // When desired levels have been reached, we stop recursion.
  if ( nLevel == GetMaxDepth() ) return;

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
inline unsigned int
Phoenix::Spatial::COctree<TYPE>::InsertObject( const TYPE & object, const Phoenix::Volume::CSphere & boundingSphere)
{
  return InsertObject( object, boundingSphere.GetPosition(), boundingSphere.GetRadius());
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
unsigned int
Phoenix::Spatial::COctree<TYPE>::InsertObject( const TYPE & object,
					       const Phoenix::Math::CVector3<float> & vPosition,
					       float fRadius)
{
  unsigned int nLevel = GetObjectDepth( fRadius);
  //if ( nLevel >= GetMaxDepth() ) nLevel = GetMaxDepth()-1;

  unsigned int nIndex  = GetIndex1D( nLevel,
				     vPosition[0],
				     vPosition[1],
				     vPosition[2]);
  COctreeNode<TYPE> *pNode = &m_pAllNodes[nIndex];
  //std::cerr << "pushing object into " << pNode  << " at index:" << nIndex << std::endl;
  if ( pNode->GetObjects().size() == 0)
  {
    pNode->GetObjects().push_back( object );
    PropagateChildrenStatus( pNode );
  }
  else
  {
    //std::cerr << "already objects"  << std::endl;
    pNode->GetObjects().push_back( object );

  }

  return nIndex;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
int
Phoenix::Spatial::COctree<TYPE>::DeleteObject( const TYPE & object, const Phoenix::Volume::CSphere & boundingSphere)
{
  int iRetval = 1;
  unsigned int nLevel = GetObjectDepth( boundingSphere.GetRadius());

  //if ( nLevel >= GetMaxDepth() ) nLevel = GetMaxDepth()-1;

  COctreeNode<TYPE> *pNode = GetNode( nLevel,
				      boundingSphere.GetPosition()[0],
				      boundingSphere.GetPosition()[1],
				      boundingSphere.GetPosition()[2]);
  // No objects, no deleting.
  if ( pNode->GetObjects().empty() )
  {
    return iRetval;
  }
  // Delete it
  iRetval = pNode->DeleteObject( object );

  if ( pNode->GetObjects().empty() )
  {
    pNode->CheckDoChildrenContainObjects();
    PropagateChildrenStatus( pNode );
  }

  return iRetval;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Phoenix::Spatial::COctreeNode<TYPE> *
Phoenix::Spatial::COctree<TYPE>::GetNodeAt(unsigned int nIndex)
{
  return &m_pAllNodes[nIndex];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Phoenix::Spatial::COctree<TYPE>::PropagateChildrenStatus( Phoenix::Spatial::COctreeNode<TYPE> *pNode )
{
  if ( pNode == NULL ) return;
  pNode = pNode->GetParent();
  while( pNode != NULL )
  {
    pNode->CheckDoChildrenContainObjects();
    pNode = pNode->GetParent();
  }
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline int
Phoenix::Spatial::COctreeNode<TYPE>::HasObjects() const
{
  return !GetObjects().empty();
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
#endif
