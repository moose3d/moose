/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
// file : GSE_Octree.h
// desc : contains headers for Octree. 
/////////////////////////////////////////////////////////////////
#ifndef __GSE_Octree_h__
#define __GSE_Octree_h__
/////////////////////////////////////////////////////////////////
#include <list>
#include "GSE_math.h"
#include "GSE_IndexBuffer.h"
#include "GSE_geometry.h"
#include "GSE_GraphNodes.h"
/////////////////////////////////////////////////////////////////
/// \brief The Generic Octree Class.
/////////////////////////////////////////////////////////////////
class GSE_Octree 
{
 public:
  /// The Index name mapping for the Octree children nodes.
  typedef enum 
  {
    /// corner index in cube.
    TOP_LEFT_FRONT     = 0,
    /// corner index in cube.
    TOP_LEFT_BACK      = 1,
    /// corner index in cube.
    TOP_RIGHT_FRONT    = 2,
    /// corner index in cube.
    TOP_RIGHT_BACK     = 3,
    /// corner index in cube.
    BOTTOM_LEFT_FRONT  = 4,
    /// corner index in cube.
    BOTTOM_LEFT_BACK   = 5,
    /// corner index in cube.
    BOTTOM_RIGHT_FRONT = 6,
    /// corner index in cube.
    BOTTOM_RIGHT_BACK  = 7
  } Octree_Section;
  /////////////////////////////////////////////////////////////////
 protected:
  /// The nodes of the Octree class.
  GSE_Octree *(m_pNodes[8]);
  /////////////////////////////////////////////////////////////////
 public:
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  GSE_Octree();
  /////////////////////////////////////////////////////////////////
  /// The deconstructor.
  ~GSE_Octree();
  /////////////////////////////////////////////////////////////////
  /// Index accessor method.
  GSE_Octree * operator[]( Octree_Section iSection );
  /////////////////////////////////////////////////////////////////
  /// Removes node from section iSection.
  void DeleteNode( Octree_Section iSection);
  /////////////////////////////////////////////////////////////////
  /// Assigns node p_Node to the section iSection.
  void SetNode   ( Octree_Section iSection, GSE_Octree *p_Node );
  /////////////////////////////////////////////////////////////////
  /// Returns pointer to the node array.
  GSE_Octree ** GetNodes();
  /////////////////////////////////////////////////////////////////
  /// Initializes the Octree to a virgin state.
  void Init();
  ////////////////////
  /// Determines does this octree node have children.
  /// \returns non-zero if it does, zero otherwise.
  inline int HasChildren()
  {
    return ( m_pNodes[0] != NULL || m_pNodes[1] != NULL || 
	     m_pNodes[2] != NULL || m_pNodes[3] != NULL ||
	     m_pNodes[4] != NULL || m_pNodes[5] != NULL ||
	     m_pNodes[6] != NULL || m_pNodes[7] != NULL   );
  }
};/// class GSE_Octree
/////////////////////////////////////////////////////////////////
/// A GSE_GeometryOctree class which splits faces.
/// This sorts the geometry into a spatial structure.
/// Comes in handy when large objects with only portion of it visible at at time, 
/// ie. terrain is rendered.
/////////////////////////////////////////////////////////////////

class GSE_Camera;
class GSE_GeometryOctree : public GSE_Octree, 
			   public Geometry::GSE_AxisAlignedBox
{
protected:
/// The pointer to the elementsnode.
  GSE_IndexBufferNode *m_pIndexBufferNode;
 public:
  /////////////////////////////////////////////////////////////////
  /// The constructor. Initializes the parameters.
  GSE_GeometryOctree() : GSE_Octree(), Geometry::GSE_AxisAlignedBox(), m_pIndexBufferNode(NULL)
  {
  }
  /////////////////////////////////////////////////////////////////
  /// The destructor.
  ~GSE_GeometryOctree()
   {
     Init();
   }
  /////////////////////////////////////////////////////////////////
  /// Splits the given elements in the vertexarray into an Octree. Some faces will be in multiple nodes if a plane intersects them.
  /// \param lstIndices indices of the elements which are processed. Will be cleared in the process.
  /// \param nMaxTreeDepth The maximum depth of the octree.
  /// \param nMinFacesInNode The minimum number of faces which will be assigned to a node without splitting.
  /// \param fWidth the width of the current node.
  /// \param fHeight the height of the current node.
  /// \param fLength the length of the current node.
  /// \param vCenter the center point of the current node.
  /// \param rRenderGraph The RenderGraph where geometry belongs to.
  /// \returns A pointer to the ready octree node (may have children).
  static GSE_GeometryOctree *Split( VertexArray pVertices, 
				    std::list<unsigned int> & lstIndices,
				    unsigned int nMaxTreeDepth, 
				    unsigned int nMinFacesInNode,
				    float fWidth,
				    float fHeight,
				    float fLength,
				    const GSE_Vector3 &vCenter,
				    GSE_RenderGraph &rRenderGraph);
  /////////////////////////////////////////////////////////////////
  /// Assigns the elementsnode.
  /// \param pNode The elementsnode which is to be set.
  inline void SetIndexBufferNode( GSE_IndexBufferNode *pNode )
  {
    m_pIndexBufferNode = pNode;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns the elemensnode member.
  /// \returns The GSE_IndexBufferNode member.
  inline GSE_IndexBufferNode *GetIndexBufferNode()
  {
    return m_pIndexBufferNode;
  }
  /////////////////////////////////////////////////////////////////
};
#endif
// -----------------------------------------------------------------
// history:
//		$Log: GSE_Octree.h,v $
//		Revision 1.11  2007/06/01 08:55:43  entity
//		doxygen comments added
//
//		Revision 1.10  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.9  2007/03/31 15:56:35  entity
//		ElementList->IndexBuffer rename
//
//		Revision 1.8  2007/03/29 21:19:41  entity
//		code cleanups
//
//		Revision 1.7  2007/03/29 13:12:55  entity
//		geometryoctree inherits axisalignedbox
//
//		Revision 1.6  2007/03/29 06:52:27  entity
//		const & stuff added
//
//		Revision 1.5  2007/03/29 06:49:55  entity
//		Renamed few arguments better for Split(...)
//
//		Revision 1.4  2007/03/28 10:34:42  entity
//		CollectNodes moved to SceneRenderer
//
//		Revision 1.3  2007/03/28 08:30:39  entity
//		GSE_GeometryOctree improved
//
//		Revision 1.2  2007/03/22 06:51:23  entity
//		header fixes
//
//		Revision 1.1  2007/03/22 06:38:23  entity
//		renamed GSE_datastructures.{h,cpp} -> GSE_Octree.{h,cpp}
//
//		Revision 1.15  2007/03/08 20:19:46  entity
//		Unnecessary headers removed
//
//		Revision 1.14  2007/03/08 20:08:12  entity
//		Moved graphnode from GSE_datastructures.h to its own file
//
//		Revision 1.13  2007/03/08 18:07:58  entity
//		LightCluster
//
//		Revision 1.12  2007/03/08 12:58:04  entity
//		API changes
//
//		Revision 1.11  2007/03/07 13:38:11  entity
//		code cleanups
//
//		Revision 1.10  2007/03/06 14:11:29  entity
//		new particle system
//
//		Revision 1.9  2007/02/27 14:03:08  entity
//		UtilSphere reworked
//
//		Revision 1.8  2007/02/27 07:09:20  entity
//		fixed RotationNode matrix method
//
//		Revision 1.7  2007/02/26 20:28:48  entity
//		RotationNode rewritten with quaternions
//
//		Revision 1.6  2007/01/24 18:31:24  entity
//		about 50% of classes are documented using doxygen
//
//		Revision 1.5  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.4  2006/11/09 10:51:35  entity
//		m_bRendered flag added
//
//		Revision 1.3  2006/10/20 16:09:21  entity
//		cleaned up comments
//
//		Revision 1.2  2006/10/08 08:40:58  entity
//		GSE_SceneObjectNode will not free RenderNode anymore
//
//
//		27092006   AG/eNtity : removed sectornode pointer from PortalNode.
//		11092006   AG/eNtity : changed GSE_SceneObjectNode; 
//					SetRenderNode does not delete the previous node if it exists
//		<way back> AG/eNtity : initial version
// -----------------------------------------------------------------
