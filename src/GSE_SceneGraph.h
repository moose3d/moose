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
#ifndef __GSE_SceneGraph_h__
#define __GSE_SceneGraph_h__
/////////////////////////////////////////////////////////////////
#include "GSE_graph.h"
#include "GSE_GraphNodes.h"
#include "GSE_animation.h"
#include "GSE_SceneRenderer.h"
/////////////////////////////////////////////////////////////////
/// A class for scenegraph.
class GSE_SceneGraph : public GSE_Graph<Scene::NodeType>
{
  
 protected:
  /// Render graph.
  GSE_RenderGraph m_gRenderGraph;
  /// Animation graph.
  GSE_AnimationGraph m_gAnimationGraph;
  /// Animation renderer.
  GSE_AnimationRenderer m_AnimationRenderer;
  /// SceneGraph renderer.
  GSE_SceneRenderer m_SceneRenderer;
  /// Skybox for this scene.
  GSE_SceneObjectNode *m_pSkybox;

 public:
  /// Default constructor.
  GSE_SceneGraph() : GSE_Graph<Scene::NodeType>(), m_pSkybox(NULL)
  {
    m_gRenderGraph.CreateNode<GSE_RenderRoot>()->SetName("Root");
    m_gAnimationGraph.CreateNode<GSE_ARootNode>()->SetName("Root");
    CreateNode<GSE_SceneRootNode>()->SetName("Root");
  }
  /// Creates node according to template parameter.
  /// \return An object with class Type.
  template <class Type> inline Type * CreateNode()
  {
    Type *type = new Type();
    type->m_pGraph = this;
    type->m_pLocalTransform = m_gRenderGraph.CreateNode<GSE_TransformNode>();
    m_lstNodes.push_back( type );
    return type;
  }
  /// Accesses root node of render graph.
  /// \return A pointer to root node.
  inline GSE_RenderRoot * GetRenderRoot()
  {
    return (GSE_RenderRoot *)m_gRenderGraph.SeekNodeByNameAndType( "Root", RenderNode::ROOT );
  }
  /// Accesses root node of animation graph.
  /// \return a pointer to root node.
  inline GSE_ARootNode * GetAnimationRoot()
  {
    return (GSE_ARootNode *)m_gAnimationGraph.SeekNodeByNameAndType( "Root", Animation::ROOT );
  }
  /// Accesses animation graph.
  /// \return a referece to animation graph.
  inline GSE_AnimationGraph & GetAnimationGraph()
  {
    return m_gAnimationGraph;
  }
  /// Accesses render graph.
  /// \return a reference to render graph.
  inline GSE_RenderGraph & GetRenderGraph()
  {
    return m_gRenderGraph;
  }
  /// Returns the root node of scene graph.
  /// \return a pointer to root node.
  inline GSE_SceneRootNode * GetRoot()
  {
    return (GSE_SceneRootNode *)SeekNodeByNameAndType( "Root", Scene::ROOT );
  }
  /// Accesses animationrenderer.
  /// \return a reference to aniamtionrenderer.
  inline GSE_AnimationRenderer & GetAnimationRenderer()
  {
    return m_AnimationRenderer;
  }
  /// Accesses scenerenderer.
  /// \return a reference to scenerenderer..
  inline GSE_SceneRenderer & GetSceneRenderer()
  {
    return m_SceneRenderer;
  }
  /// Performs visibility determination and creates a render graph from current scene using view settings from a camera.
  /// \param pRenderer OpenGL renderer to be used in rendering.
  /// \param pCamera A camera of which view settings are used.
  void CollectRenderGraph( GSE_OglRenderer *pRenderer, GSE_CameraNode *pCamera )
  {

    m_SceneRenderer.SetRenderPass( SCN_RENDER_PASS_COLLECT_GRAPH );
    m_SceneRenderer.SetCurrentCameraNode(pCamera);
    m_SceneRenderer.SetOglRenderer(pRenderer);
    GetRenderRoot()->AddEdge( pCamera );
    m_SceneRenderer.SetAttachingNode( pCamera );


    if ( m_pSkybox != NULL )
    {
      // reset the  camera translation, keep rotation
      GSE_Matrix4x4f matrix = GSE_Matrix4x4f::Identity();
      matrix(0,3) = pCamera->GetCamera()->GetPosition().m_pValues[GSE_Vector3::X];
      matrix(1,3) = pCamera->GetCamera()->GetPosition().m_pValues[GSE_Vector3::Y];
      matrix(2,3) = pCamera->GetCamera()->GetPosition().m_pValues[GSE_Vector3::Z];
      m_pSkybox->SetLocalTransform(matrix);
      //m_pSkybox->SetWorldTransform(matrix);
      pCamera->AddEdge(m_pSkybox->m_pLocalTransform);
      
    }
    
    Graph::TravelDF<Scene::NodeType,GSE_SceneRenderer>( GetRoot(), &m_SceneRenderer);
    
  }
  /// Updates animations according to passed time.
  /// \param nPassedTimeInMS passed time in milliseconds.
  void UpdateAnimations( unsigned int nPassedTimeInMS )
  {
    m_AnimationRenderer.SetPassedTime(nPassedTimeInMS);
    m_AnimationRenderer.SetRenderPass( GSE_AnimationRenderer::RENDER_PASS_UPDATE);
    //GSE_AnimationGraph::TravelDF(GetAnimationRoot(), &m_AnimationRenderer);
    Graph::TravelDF<Animation::NodeType,GSE_AnimationRenderer>(GetAnimationRoot(), &m_AnimationRenderer);
  }
  /// Updates bounding volume transforms.
  void UpdateTransforms()
  {
    m_SceneRenderer.SetRenderPass( SCN_RENDER_PASS_UPDATE_TRANSFORMS );
    Graph::TravelDF<Scene::NodeType,GSE_SceneRenderer>( GetRoot(), &m_SceneRenderer);
  }
  /// \brief Updates the bounding volumes from leaf nodes towards root (merges them)
  /// Should be called when a new node is attached to the graph for rendering or position of a node has changed.
  void UpdateBoundingVolumes()
  {
    m_SceneRenderer.SetRenderPass( SCN_RENDER_PASS_UPDATE_BOUNDING_VOLUMES );
    Graph::TravelDF<Scene::NodeType,GSE_SceneRenderer>( GetRoot(), &m_SceneRenderer);
  }
  /// Performs cleaning up after rendering of a frame.
  void CleanUp()
  {
    m_SceneRenderer.SetRenderPass( SCN_RENDER_PASS_CLEANUP );
    // Remove edge pointing to camera.
    GetRenderRoot()->RemoveLeavingEdges();
    if ( m_pSkybox != NULL ) 
    {
      m_pSkybox->m_pLocalTransform->RemoveArrivingEdges();
    }
    //GSE_SceneGraph::TravelDF( GetRoot(), &m_SceneRenderer);
    Graph::TravelDF<Scene::NodeType,GSE_SceneRenderer>(GetRoot(), &m_SceneRenderer);
  }
  /////////////////////////////////////////////////////////////////  
  /// Performs a traversal with scenerenderer without setting any parameters. 
  void PerformTraversal()
  {
    Graph::TravelDF<Scene::NodeType,GSE_SceneRenderer>( GetRoot(),   
							&GetSceneRenderer());
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns the backdrop for this scene.
  /// \param pSkybox a pointer to a skybox object to be assigned.
  inline void SetSkybox( GSE_SceneObjectNode *pSkybox )
  {
    if ( m_pSkybox != NULL )
    {
      m_pSkybox->m_pLocalTransform->DeleteEdgeTo(m_pSkybox->GetRenderNode());
    }
    m_pSkybox = pSkybox;
    if ( m_pSkybox != NULL )
    {
      m_pSkybox->m_pLocalTransform->AddEdge(m_pSkybox->GetRenderNode());
    }
  }
  ////////////////////
  /// \brief Sets visibility property of all nodes to false.
  void ClearVisibility()
  {
    std::list< GSE_GraphNode<Scene::NodeType> *>::iterator it = m_lstNodes.begin();
    for( ;it!=m_lstNodes.end();it++)
    {
      static_cast<GSE_SceneNode *>((*it))->SetVisible(0);
    }
  }
};
/// A type-specific template method for GSE_SceneCameraNode,
/// which needs a GSE_CameraNode member.
template<> 
inline GSE_SceneCameraNode * 
GSE_SceneGraph::CreateNode<GSE_SceneCameraNode>()
{
  GSE_SceneCameraNode *type = new GSE_SceneCameraNode();
  type->m_pGraph = this;
  type->m_pLocalTransform = m_gRenderGraph.CreateNode<GSE_TransformNode>();
  type->m_pCameraNode     = m_gRenderGraph.CreateNode<GSE_CameraNode>();
  m_lstNodes.push_back(type);
  return type;
}
/// A type-specific template method for GSE_SceneParticleSystemNode,
/// which needs a GSE_ParticleSystemNode member.
template<> 
inline GSE_SceneParticleSystemNode * 
GSE_SceneGraph::CreateNode<GSE_SceneParticleSystemNode>()
{
  GSE_SceneParticleSystemNode *type = new GSE_SceneParticleSystemNode();
  type->m_pGraph = this;
  type->m_pLocalTransform = m_gRenderGraph.CreateNode<GSE_TransformNode>();
  type->m_pSystemNode     = m_gRenderGraph.CreateNode<GSE_ParticleSystemNode>();
  m_lstNodes.push_back(type);
  return type;
}
  
#endif

// -----------------------------------------------------------------
// $Log: GSE_SceneGraph.h,v $
// Revision 1.19  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.18  2007/05/14 09:42:48  entity
// clearvisibility added
//
// Revision 1.17  2007/04/03 12:54:22  entity
// new factory method for scneeparticlesystem node
//
// Revision 1.16  2007/04/02 11:11:21  entity
// skybox is translated correctly now
//
// Revision 1.15  2007/04/01 21:05:00  entity
// skybox improvements
//
// Revision 1.14  2007/03/31 10:54:32  entity
// World transform for skybox
//
// Revision 1.13  2007/03/30 12:57:04  entity
// skybox support
//
// Revision 1.12  2007/03/28 08:34:59  entity
// code cleanups
//
// Revision 1.11  2007/03/22 19:53:36  entity
// CREATE_GRAPH -> COLLECT_GRAPH
//
// Revision 1.10  2007/03/20 20:12:40  entity
// GetSceneRenderer() & PerformTraversal() added, removed initializeboundingvolumes
//
// Revision 1.9  2007/03/20 13:32:09  entity
// ForceOverrideCulling commented,
// UpdateBoundingVolumes() and InitializeBoundingVolumes()
// added.
//
// Revision 1.8  2007/03/15 18:38:13  entity
// Added camera to CurrentCameraNode on CollectRenderGraph
//
// Revision 1.7  2007/03/14 21:50:52  entity
// Changes in rendering
//
// Revision 1.6  2007/03/14 20:01:22  entity
// UPDATE_TRANSFORMS implemented
//
// Revision 1.5  2007/03/14 13:12:17  entity
// SceneGraph forces inclusion of objects for now
//
// Revision 1.4  2007/03/12 14:53:22  entity
// changes according to new TravelDF()
//
// Revision 1.3  2007/03/11 19:44:48  entity
// GetAnimationRenderer() added
//
// Revision 1.2  2007/03/11 16:58:20  entity
// GPL licence added
//
// -----------------------------------------------------------------
