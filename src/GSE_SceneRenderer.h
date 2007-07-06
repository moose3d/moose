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
#ifndef __GSE_SceneRenderer__
#define __GSE_SceneRenderer__
/////////////////////////////////////////////////////////////////
#include <stack>
#include <list>
#include <string>
#include "GSE_GraphNodes.h"
/////////////////////////////////////////////////////////////////
using std::stack;
using std::list;
class GSE_OglRenderer;
typedef std::list<GSE_SceneNode *> ObjectPtrList;
/////////////////////////////////////////////////////////////////
/// The Render pass types
typedef enum {
  SCN_RENDER_PASS_UPDATE_BOUNDING_VOLUMES,	// If sceneobject is marked changed, the parent volumes are updated.
  SCN_RENDER_PASS_COLLECT_GRAPH,		// Generates the Render Graph. Visibility culling is performed in FrustumSphere->FrustumCone->Frustum KDOP order.
  SCN_RENDER_PASS_CLEANUP,			// Removes edges from first RenderNode in RenderNode chain.
  SCN_RENDER_PASS_DEBUG_PRINT,			// Writes the graph in stderr with
  SCN_RENDER_PASS_UPDATE_TRANSFORMS		// updates world transformations 
} GSE_SceneRenderPass_t;
/////////////////////////////////////////////////////////////////
/// The Scene Graph Renderer class.
/// It handles the scene graph nodes by calculating bounding volumes,
/// and constructs a culled render graph from the scene graph.
/////////////////////////////////////////////////////////////////
class GSE_SceneRenderer : public GSE_MultiPassRenderer< GSE_SceneRenderPass_t >
{
 private:
  
  ////////////////////
  /// The list of collected objects.
  ObjectPtrList m_lstObjects;
  /////////////////////////////////////////////////////////////////
  // The OglRenderer used for determining the bounding volumes
  GSE_OglRenderer *m_pOglRenderer;
  /////////////////////////////////////////////////////////////////
  // The "frustum" which is used to perform visibility culling
  GSE_KDOP	   m_kDOP;
  /////////////////////////////////////////////////////////////////
  std::string		m_sPadding;
  std::string		m_sArrow;
  /////////////////////////////////////////////////////////////////
  // The currently used camera.
  GSE_CameraNode *	m_pCameraNode;
  /////////////////////////////////////////////////////////////////
  /// The top of the stack contains the parent 
  /// node where we came. This is useful in situations when
  /// SceneGraph node has multiple parents.
  stack<GSE_SceneNode *>  m_SceneTravellerStack;
  /////////////////////////////////////////////////////////////////
  // The stack which is used in creating the graph with proper grouping, 
  // ie. CameraNode will receive directly all subobjects from the plain groups and
  // objects affected by light will be under the LightNode which is child 
  // of CameraNode. The bottom node in this stack will always be GSE_CameraNode
  stack<GSE_RenderNode *> m_RenderNodeStack;
  /////////////////////////////////////////////////////////////////
  // When this is set to true, it will cause automatic inclusion of every node
  // when encountering them. It is used when RENDER_PASS_CREATE_GRAPH is set
  // and group of objects is determined to be completely within the frustum.
#define IS_OVERRIDE_CULLING_SET() ( !m_OverrideCullingStack.empty() )
  /////////////////////////////////////////////////////////////////
  // The stack is used to determine when OverrideCulling should be deactivated,
  // ie. when we Finish() the node that started it.
  stack<char> m_OverrideCullingStack;
 private:
  /////////////////////////////////////////////////////////////////
  // The methods which Render()s the node when 
  // the RENDER_PASS_CREATE_GRAPH is set.  
  int Handle_CollectGraph_RenderPass_Enter( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  // The method which Render()s the node when 
  // the RENDER_PASS_CONSTRUCT_BOUNDING_VOLUMES is set.
  int Handle_BoundingVolume_RenderPass_Enter( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  // Updates bounding volumes according to changed flags. If SceneNode is set to
  // 'changed' ( node->SetChanged(1) ) the pass will update bounding volumes
  // towards the root from this node. The pass will not do anything if 
  // there are no changes.
  int Handle_BoundingVolumeUpdate_RenderPass_Leave( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  // The prints the node into stderr
  int Handle_DebugPrint_RenderPass_Enter( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  // The methods which Finish()es the node when 
  // the RENDER_PASS_CREATE_GRAPH is set.  
  int Handle_CollectGraph_RenderPass_Leave( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  // The method which handles the node 
  // the RENDER_PASS_CONSTRUCT_BOUNDING_VOLUMES is set.
  int Handle_BoundingVolume_RenderPass_Leave( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  // Finish() call for debug print pass
  int Handle_DebugPrint_RenderPass_Leave( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  // Handles the cleaning render pass
  int Handle_Cleanup_RenderPass_Enter( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  int Handle_UpdateTransforms_Enter( GSE_SceneNode *pNode );
  /////////////////////////////////////////////////////////////////
  void AddToGraphWithLighting( GSE_SceneOctreeObjectNode *pNode );
  /////////////////////////////////////////////////////////////////
  void AddToGraphWithLighting( GSE_SceneObjectNode *pNode );
  /////////////////////////////////////////////////////////////////
  /// Cull nodes and link proper elementlistnodes to the proper geometry node.
  /// \param pCamera The camera which is used in the culling.
  /// \param pGeometryNode The Geometrynode where Indexbuffernodes are linked as children.
  /// \param pGeometryOctree The octree which is traversed.
  /// \param bUseCulling Initially true. If set to false, pGeometryOctree and
  /// all children nodes are included without performing culling test.
  void CollectOctreeNodes( GSE_Camera *pCamera, 
			   GSE_GeometryNode *pGeometryNode,
			   GSE_GeometryOctree *pGeometryOctree,
			   int bUseCulling = 1);
  ////////////////////
  /// \brief Adds given node to list of collected objects.
  /// \param pNode A pointer to object which is collected.
  void AddCollectedObject( GSE_SceneNode *pNode );
  ////////////////////
  /// \brief Clears the list of collected objects.
  void ClearCollectedObjects();
  
 public:
  /////////////////////////////////////////////////////////////////
  // The Constructor
  GSE_SceneRenderer();
  /////////////////////////////////////////////////////////////////
  // The Destructor
  virtual ~GSE_SceneRenderer();
  /////////////////////////////////////////////////////////////////
  /// The principal method which handles the pNode.
  int Enter( GSE_GraphNode<Scene::NodeType> *pNode );
  /////////////////////////////////////////////////////////////////
  /// Called when the node and its children have been Render()ed.
  int Leave( GSE_GraphNode<Scene::NodeType> *pNode );
  /////////////////////////////////////////////////////////////////
  /// Defines the camera which is used to cull the objects.
  void SetCurrentCameraNode( GSE_CameraNode *pCameraNode );
  /////////////////////////////////////////////////////////////////
  /// Returns the CameraNode currently used.
  GSE_CameraNode * GetCurrentCameraNode();
  /////////////////////////////////////////////////////////////////
  /// Assigns the K-DOP which is used to cull objects (GSE_Frustum is a GSE_KDOP).
  void SetVisibilityCullingKDOP( GSE_KDOP &kDop );
  /////////////////////////////////////////////////////////////////  
  /// Assigns the node where other nodes are attached to.
  /// \param pNode node where other nodes are atteched to.
  void SetAttachingNode( GSE_RenderNode *pNode );
  /////////////////////////////////////////////////////////////////
  /// Enables override culling (every object will be included).
  void ForceOverrideCulling();
  /////////////////////////////////////////////////////////////////
  /// Sets OpenGL renderer.
  /// \param pRenderer a pointer to opengl renderer.
  void SetOglRenderer( GSE_OglRenderer *pRenderer );
  ////////////////////
  /// \brief Returns a reference to the list of collected objects.
  /// \return List of collected objects.
  ObjectPtrList & GetCollectedObjects() ;
  /////////////////////////////////////////////////////////////////
  /// \brief Returns the number of objects collected after COLLECT_GRAPH call.
  /// \returns The number of objects.
  inline unsigned int GetCollectedObjectCount() const 
  {
    return m_lstObjects.size();
  }
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// $Log: GSE_SceneRenderer.h,v $
// Revision 1.21  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.20  2007/05/22 08:29:56  entity
// Collected Objects List added
//
// Revision 1.19  2007/04/02 11:12:09  entity
// new CollectOctreenNodes()
//
// Revision 1.18  2007/03/28 10:36:52  entity
// added CollectOctreeNodes add overloaded AddToGraphWithLighting
//
// Revision 1.17  2007/03/25 15:33:29  entity
// Added the collected object count
//
// Revision 1.16  2007/03/22 06:29:56  entity
// comments fixed, renamed render pass
//
// Revision 1.15  2007/03/21 09:52:03  entity
// removed unneeded stuff and added comments
//
/////////////////////////////////////////////////////////////////
