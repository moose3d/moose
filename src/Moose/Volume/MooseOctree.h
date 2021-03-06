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

#ifndef __MooseOctree_h__
#define __MooseOctree_h__
/////////////////////////////////////////////////////////////////
#include <list>
#include <iostream>
#include "MooseAACube.h"
#include "MooseCamera.h"
#include "MooseCollision.h"
#include "MooseGlobals.h"
#include "MooseAPI.h"
#include "MooseTagged.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Spatial
  {
    /// Symbolic indices for Octree children nodes.
    enum OCTREE_SECTION
    {
      O_TOP_LEFT_FRONT     = 0,
      O_TOP_LEFT_BACK      = 1,
      O_TOP_RIGHT_FRONT    = 2,
      O_TOP_RIGHT_BACK     = 3,
      O_BOTTOM_LEFT_FRONT  = 4,
      O_BOTTOM_LEFT_BACK   = 5,
      O_BOTTOM_RIGHT_FRONT = 6,
      O_BOTTOM_RIGHT_BACK  = 7
    };
    ////////////////////
    /// Symbolic indices for node neighbors.
    enum OCTREE_NEIGHBOR
    {
      O_LEFT   = 0,
      O_RIGHT  = 1,
      O_TOP    = 2,
      O_BOTTOM = 3,
      O_FRONT  = 4,
      O_BACK   = 5
    };
    /////////////////////////////////////////////////////////////////
    /// Octreenode class template.
    template<typename TYPE>
    class MOOSE_API COctreeNode : public Moose::Volume::CAxisAlignedCube
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
      // This must not be used, since all nodes go or nothing goes.
      //void DeleteNode( OCTREE_SECTION iSection);
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
    /// Octree class template. Width defines all dimensions, hence CDimensional1D.
    template<typename TYPE>
    class MOOSE_API COctree : protected Moose::Spatial::CDimensional1D
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
      unsigned int GetIndex1D( unsigned int nLevel, const Moose::Math::CVector3<float> & vPosition );
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
				 const Moose::Volume::CSphere & boundingSphere);

      ////////////////////
      /// Inserts object into tree.
      /// \param object Reference to object to be inserted.
      /// \param vPosition Position of bounding sphere of the object.
      /// \param fRadius Radius of bounding sphere.
      /// \returns Index where object was inserted.
      unsigned int InsertObject( const TYPE & object,
				 const Moose::Math::CVector3<float> & vPosition,
				 float fRadius);
      ////////////////////
      /// Delets object from tree.
      /// \param object Reference to object to be removed.
      /// \param boundingSphere Bounding sphere of object.
      /// \returns 1 On error.
      /// \returns 0 Otherwise.
      int  DeleteObject( const TYPE & object, const Moose::Volume::CSphere & boundingSphere);
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
		       const Moose::Math::CVector3<float> & vPosition,
		       COctreeNode<TYPE> *pNode);
    };
  } // namespace Spatial
} // namespace Moose
/////////////////////////////////////////////////////////////////
/// Macro for easier deletion
#define MOOSE_DELETE(A) delete A; A = NULL;
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Spatial::COctreeNode<TYPE>::COctreeNode() : CAxisAlignedCube()
{
  m_bChildrenContainObjects = 0;
  // initialize nodes
  m_pChildren[O_TOP_LEFT_FRONT] = NULL;
  m_pChildren[O_TOP_LEFT_BACK] = NULL;
  m_pChildren[O_TOP_RIGHT_FRONT] = NULL;
  m_pChildren[O_TOP_RIGHT_BACK] = NULL;
  m_pChildren[O_BOTTOM_LEFT_FRONT] = NULL;
  m_pChildren[O_BOTTOM_LEFT_BACK] = NULL;
  m_pChildren[O_BOTTOM_RIGHT_FRONT] = NULL;
  m_pChildren[O_BOTTOM_RIGHT_BACK] = NULL;

  m_pNeighbors[O_TOP] = NULL;
  m_pNeighbors[O_BOTTOM] = NULL;
  m_pNeighbors[O_LEFT] = NULL;
  m_pNeighbors[O_RIGHT] = NULL;
  m_pNeighbors[O_FRONT] = NULL;
  m_pNeighbors[O_BACK] = NULL;

  m_pParent = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Spatial::COctreeNode<TYPE>::~COctreeNode()
{
  /*MOOSE_DELETE(m_pChildren[O_TOP_LEFT_FRONT]);
  MOOSE_DELETE(m_pChildren[O_TOP_LEFT_BACK]);
  MOOSE_DELETE(m_pChildren[O_TOP_RIGHT_FRONT]);
  MOOSE_DELETE(m_pChildren[O_TOP_RIGHT_BACK]);
  MOOSE_DELETE(m_pChildren[O_BOTTOM_LEFT_FRONT]);
  MOOSE_DELETE(m_pChildren[O_BOTTOM_LEFT_BACK]);
  MOOSE_DELETE(m_pChildren[O_BOTTOM_RIGHT_FRONT]);
  MOOSE_DELETE(m_pChildren[O_BOTTOM_RIGHT_BACK]);
   */
  m_pNeighbors[O_TOP] = NULL;
  m_pNeighbors[O_BOTTOM] = NULL;
  m_pNeighbors[O_LEFT] = NULL;
  m_pNeighbors[O_RIGHT] = NULL;
  m_pNeighbors[O_FRONT] = NULL;
  m_pNeighbors[O_BACK] = NULL;

  m_pParent = NULL;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctreeNode<TYPE>::GetChild( Moose::Spatial::OCTREE_SECTION iSection )
{
  return m_pChildren[iSection];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctreeNode<TYPE>::GetChild( Moose::Spatial::OCTREE_SECTION iSection ) const
{
  return m_pChildren[iSection];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline int
Moose::Spatial::COctreeNode<TYPE>::ChildrenContainObjects() const
{
  return m_bChildrenContainObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Moose::Spatial::COctreeNode<TYPE>::CheckDoChildrenContainObjects()
{
  // if one of child nodes is NULL, others are too.
  if ( m_pChildren[O_TOP_LEFT_FRONT] == NULL)
  {
    m_bChildrenContainObjects = 0;
  }
  else
  {
    m_bChildrenContainObjects = (m_pChildren[O_TOP_LEFT_FRONT]->HasObjects()     || m_pChildren[O_TOP_LEFT_FRONT]->ChildrenContainObjects()     ||
				 m_pChildren[O_TOP_LEFT_BACK]->HasObjects()      || m_pChildren[O_TOP_LEFT_BACK]->ChildrenContainObjects()      ||
				 m_pChildren[O_TOP_RIGHT_FRONT]->HasObjects()    || m_pChildren[O_TOP_RIGHT_FRONT]->ChildrenContainObjects()    ||
				 m_pChildren[O_TOP_RIGHT_BACK]->HasObjects()     || m_pChildren[O_TOP_RIGHT_BACK]->ChildrenContainObjects()     ||
				 m_pChildren[O_BOTTOM_LEFT_FRONT]->HasObjects()  || m_pChildren[O_BOTTOM_LEFT_FRONT]->ChildrenContainObjects()  ||
				 m_pChildren[O_BOTTOM_LEFT_BACK]->HasObjects()   || m_pChildren[O_BOTTOM_LEFT_BACK]->ChildrenContainObjects()   ||
				 m_pChildren[O_BOTTOM_RIGHT_FRONT]->HasObjects() || m_pChildren[O_BOTTOM_RIGHT_FRONT]->ChildrenContainObjects() ||
				 m_pChildren[O_BOTTOM_RIGHT_BACK]->HasObjects()  || m_pChildren[O_BOTTOM_RIGHT_BACK]->ChildrenContainObjects() );
  }

}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Moose::Spatial::COctreeNode<TYPE>::SetChild( Moose::Spatial::OCTREE_SECTION iSection, COctreeNode<TYPE> *pNode)
{
  m_pChildren[iSection] = pNode;
}
/////////////////////////////////////////////////////////////////
//template<typename TYPE>
//void
//inline Moose::Spatial::COctreeNode<TYPE>::DeleteNode( Moose::Spatial::OCTREE_SECTION iSection )
//{
//  MOOSE_DELETE(m_pChildren[iSection]);
//}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Moose::Spatial::COctreeNode<TYPE> **
Moose::Spatial::COctreeNode<TYPE>::GetNodes(){

  return m_pChildren;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline std::list< TYPE > &
Moose::Spatial::COctreeNode<TYPE>::GetObjects()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const std::list< TYPE > &
Moose::Spatial::COctreeNode<TYPE>::GetObjects() const
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
int
Moose::Spatial::COctreeNode<TYPE>::DeleteObject( const TYPE & object)
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
Moose::Spatial::COctreeNode<TYPE>::AddObject( const TYPE &object )
{
  m_lstObjects.push_back(object);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Moose::Spatial::COctreeNode<TYPE>::SetNeighbor( Moose::Spatial::OCTREE_NEIGHBOR iNeighbor, COctreeNode<TYPE> *pNode )
{
  m_pNeighbors[iNeighbor] = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctreeNode<TYPE>::GetNeighbor( Moose::Spatial::OCTREE_NEIGHBOR iNeighbor )
{
  return m_pNeighbors[iNeighbor];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Moose::Spatial::COctreeNode<TYPE>::SetParent( Moose::Spatial::COctreeNode<TYPE> *pNode )
{
  m_pParent = pNode;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctreeNode<TYPE>::GetParent()
{
  return m_pParent;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Spatial::COctree<TYPE>::COctree( unsigned int nNumLevels, float fWorldSize )
{
  m_nNodeCount = (unsigned int)((1-powf(8,nNumLevels))/-7);
  // Maximum depth will be levels-1, ie. 4 = { 0, 1, 2, 3 }
  SetMaxDepth(nNumLevels-1);
  SetWorldSize(fWorldSize);
  // Allocate array for all nodes
  m_pAllNodes = new COctreeNode<TYPE>[m_nNodeCount];
  Initialize( 0, Moose::Math::CVector3<float>(0,0,0), &m_pAllNodes[0]);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
Moose::Spatial::COctree<TYPE>::~COctree()
{
	delete [] m_pAllNodes;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline size_t
Moose::Spatial::COctree<TYPE>::GetNodeCount() const
{
  return m_nNodeCount;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Moose::Spatial::COctree<TYPE>::GetObjectDepth( float fRadius )
{
  unsigned int nDepth = static_cast<unsigned int>(floorf( Moose::Math::Log2( GetWorldSize()/fRadius)));
  return nDepth < GetMaxDepth() ? nDepth : GetMaxDepth();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline float
Moose::Spatial::COctree<TYPE>::GetWorldSize() const
{
  return GetWidth();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline float
Moose::Spatial::COctree<TYPE>::GetWorldHalfSize() const
{
  return GetHalfWidth();
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline float
Moose::Spatial::COctree<TYPE>::GetEdgeLength( unsigned int nDepth )
{
  return GetWorldSize() / powf(2, nDepth);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Moose::Spatial::COctree<TYPE>::GetIndex( float fValue, unsigned nDepth )
{
  unsigned int nDimensions = (unsigned int)powf(8.0f, floorf(((float)nDepth) *0.333333333333333f));
  if (      fValue >= GetWorldHalfSize()-0.001f) return nDimensions-1;
  else if ( fValue <= -GetWorldHalfSize()) return 0;
  return static_cast<unsigned int>(floorf( ((fValue * m_fOneDivWorldSize) + 0.5f)* nDimensions));
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctree<TYPE>::GetRoot()
{
  return &m_pAllNodes[0];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline const Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctree<TYPE>::GetRoot() const
{
  return &m_pAllNodes[0];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Moose::Spatial::COctree<TYPE>::GetIndex1D( unsigned int nLevel, float fX, float fY, float fZ )
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
Moose::Spatial::COctree<TYPE>::GetIndex1D( unsigned int nLevel, const Moose::Math::CVector3<float> & vPosition )
{
  return GetIndex1D(nLevel, vPosition[0], vPosition[1], vPosition[2]);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctree<TYPE>::GetNode( unsigned int nLevel, float fX, float fY, float fZ )
{
  return &m_pAllNodes[GetIndex1D(nLevel, fX, fY, fZ)];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Moose::Spatial::COctree<TYPE>::SetWorldSize( float fSize )
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
Moose::Spatial::COctree<TYPE>::SetMaxDepth( unsigned int nDepth )
{
  m_nDepth = nDepth;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Moose::Spatial::COctree<TYPE>::GetMaxDepth()
{
  return m_nDepth;
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
void
Moose::Spatial::COctree<TYPE>::Initialize( unsigned int nLevel, const Moose::Math::CVector3<float> & vPosition, COctreeNode<TYPE> *pNode)
{
  //std::cerr << "Starting init of : " << pNode << " at level " << nLevel << std::endl;
  float fEdgeLength = GetEdgeLength( nLevel );
  pNode->SetPosition( vPosition );
  pNode->SetWidth( 2.0f*fEdgeLength);

  float fChildCenterPosDiff = fEdgeLength * 0.25f;

  COctreeNode<TYPE> *pChild = NULL;
  // When desired levels have been reached, we stop recursion.
  if ( nLevel == GetMaxDepth() ) return;

  Moose::Math::CVector3<float> vNewPos;

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
  pNode->SetChild( Moose::Spatial::O_BOTTOM_LEFT_BACK, pChild);
  pChild->SetParent( pNode );
  // Initialize child recursively.
  Initialize( nLevel + 1, vNewPos, pChild);

  // 2nd
  vNewPos[0] = vPosition[0] - fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] - fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Moose::Spatial::O_BOTTOM_LEFT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);

  // 3rd
  vNewPos[0] = vPosition[0] - fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] - fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Moose::Spatial::O_BOTTOM_RIGHT_BACK, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 4th
  vNewPos[0] = vPosition[0] - fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Moose::Spatial::O_BOTTOM_RIGHT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 5th
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] - fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] - fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Moose::Spatial::O_TOP_LEFT_BACK, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 6th
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] - fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Moose::Spatial::O_TOP_LEFT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 7th
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] - fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Moose::Spatial::O_TOP_RIGHT_BACK, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
  // 8th
  vNewPos[0] = vPosition[0] + fChildCenterPosDiff;
  vNewPos[1] = vPosition[1] + fChildCenterPosDiff;
  vNewPos[2] = vPosition[2] + fChildCenterPosDiff;
  pChild = GetNode( nLevel+1, vNewPos[0], vNewPos[1], vNewPos[2]);
  pNode->SetChild( Moose::Spatial::O_TOP_RIGHT_FRONT, pChild);
  pChild->SetParent( pNode );
  Initialize( nLevel + 1, vNewPos, pChild);
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline unsigned int
Moose::Spatial::COctree<TYPE>::InsertObject( const TYPE & object, const Moose::Volume::CSphere & boundingSphere)
{
  return InsertObject( object, boundingSphere.GetPosition(), boundingSphere.GetRadius());
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
unsigned int
Moose::Spatial::COctree<TYPE>::InsertObject( const TYPE & object,
					       const Moose::Math::CVector3<float> & vPosition,
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
  if ( pNode->GetObjects().empty() )
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
Moose::Spatial::COctree<TYPE>::DeleteObject( const TYPE & object, const Moose::Volume::CSphere & boundingSphere)
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
inline Moose::Spatial::COctreeNode<TYPE> *
Moose::Spatial::COctree<TYPE>::GetNodeAt(unsigned int nIndex)
{
  return &m_pAllNodes[nIndex];
}
/////////////////////////////////////////////////////////////////
template<typename TYPE>
inline void
Moose::Spatial::COctree<TYPE>::PropagateChildrenStatus( Moose::Spatial::COctreeNode<TYPE> *pNode )
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
Moose::Spatial::COctreeNode<TYPE>::HasObjects() const
{
  return !GetObjects().empty();
}
/////////////////////////////////////////////////////////////////
#endif
