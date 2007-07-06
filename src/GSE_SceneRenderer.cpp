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
#include "GSE_SceneRenderer.h"
#include "GSE_geometry.h"
#include "GSE_Logger.h"
#include "GSE_OglRenderer.h"
#include "GSE_Octree.h"
#include <list>
/////////////////////////////////////////////////////////////////
using Core::GSE_Logger;
using namespace Scene;
/////////////////////////////////////////////////////////////////
// Returns true if the node has geometry data; pSceneObj must be NON-NULL
#define OBJECT_HAS_GEOMETRY( pSceneObj ) ( pSceneObj->GetRenderNode() != NULL )
/////////////////////////////////////////////////////////////////
/// Template function for transforming the bounding volumes with slightly different types.
template <class Type>
inline void
TransformBoundingVolumes( Type * pTmpNode )
{
  pTmpNode->GetWorldBoundingBox().SetPosition( Math::Transform( pTmpNode->GetLocalBoundingBox().GetPosition(),
								pTmpNode->GetWorldTransform()));
  pTmpNode->GetWorldBoundingBox().SetDirectionForwardUp( Math::Rotate(pTmpNode->GetLocalBoundingBox().GetForwardVector(),
								      pTmpNode->GetWorldTransform()),
							 Math::Rotate(pTmpNode->GetLocalBoundingBox().GetUpVector(),
								      pTmpNode->GetWorldTransform()));
      
  pTmpNode->GetWorldBoundingBox().SetWidth(  pTmpNode->GetLocalBoundingBox().GetWidth()  );
  pTmpNode->GetWorldBoundingBox().SetLength( pTmpNode->GetLocalBoundingBox().GetLength() );
  pTmpNode->GetWorldBoundingBox().SetHeight( pTmpNode->GetLocalBoundingBox().GetHeight() );
  pTmpNode->GetWorldBoundingBox().CalculateCorners();
  pTmpNode->GetWorldBoundingBox().CalculatePlanes();
  pTmpNode->GetWorldBoundingSphere().SetPosition(Math::Transform(pTmpNode->GetLocalBoundingSphere().GetPosition(), 
								 pTmpNode->GetWorldTransform()));
  pTmpNode->GetWorldBoundingSphere().SetRadius( pTmpNode->GetLocalBoundingSphere().GetRadius());
}
/////////////////////////////////////////////////////////////////
GSE_SceneRenderer::GSE_SceneRenderer() : m_pCameraNode(NULL)
{
  SetRenderPass( SCN_RENDER_PASS_COLLECT_GRAPH );
  m_sPadding = "";
  m_sArrow ="|->";
}
/////////////////////////////////////////////////////////////////
GSE_SceneRenderer::~GSE_SceneRenderer()
{
  
}
/////////////////////////////////////////////////////////////////
int 
GSE_SceneRenderer::Enter( GSE_GraphNode<NodeType> *pNode )
{

  int iCulled = 0;  // The return value from handler methods

  if ( pNode == NULL )
  {
    GSE_Logger::Error() << DEBUG_HEADER << "Cannot handle NULL's" << std::endl;
    iCulled = 1;
  }
  
  else 
  {
    switch( m_iRenderPassType ) // Determine course of action
    {
    case SCN_RENDER_PASS_UPDATE_BOUNDING_VOLUMES:
      break;
    case SCN_RENDER_PASS_COLLECT_GRAPH:
      iCulled = Handle_CollectGraph_RenderPass_Enter( (GSE_SceneNode *)pNode );
      break;
    case SCN_RENDER_PASS_CLEANUP:
      iCulled = Handle_Cleanup_RenderPass_Enter( (GSE_SceneNode *)pNode );
      break;
    case SCN_RENDER_PASS_DEBUG_PRINT:
      iCulled = Handle_DebugPrint_RenderPass_Enter( (GSE_SceneNode *)pNode );
      break;
    case SCN_RENDER_PASS_UPDATE_TRANSFORMS:
      iCulled = Handle_UpdateTransforms_Enter( (GSE_SceneNode *)pNode );
      break;
    }
    // We have done what we can with this, push it into traveller stack 
    m_SceneTravellerStack.push((GSE_SceneNode *)pNode);
  }
  return iCulled;
}
/////////////////////////////////////////////////////////////////
int
GSE_SceneRenderer::Leave( GSE_GraphNode<NodeType> *pNode )
{
  int iRetval = 0;  // The return value from handler methods

  if ( pNode == NULL )
  {    
    GSE_Logger::Error() << DEBUG_HEADER << "Cannot handle NULL's" << std::endl;
    iRetval = 1;
  } 
  else 
  {
    switch( GetRenderPass() ) // Determine course of action
    {
    case SCN_RENDER_PASS_COLLECT_GRAPH:
      iRetval = Handle_CollectGraph_RenderPass_Leave( (GSE_SceneNode *)pNode );
      break;
    case SCN_RENDER_PASS_UPDATE_BOUNDING_VOLUMES:
      iRetval = Handle_BoundingVolume_RenderPass_Leave( (GSE_SceneNode *)pNode );      
      break;
    case SCN_RENDER_PASS_CLEANUP:
      iRetval = 0;      // NOP
      break;
    case SCN_RENDER_PASS_DEBUG_PRINT:
      iRetval = Handle_DebugPrint_RenderPass_Leave( (GSE_SceneNode *)pNode );
      break;
    case SCN_RENDER_PASS_UPDATE_TRANSFORMS:
      iRetval = 0;
      break;
    }
    // Remove node from traveller's stack.
    m_SceneTravellerStack.pop();
  }
  return iRetval;

}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::SetCurrentCameraNode( GSE_CameraNode *pCameraNode )
{
  m_pCameraNode = pCameraNode;
}
/////////////////////////////////////////////////////////////////
GSE_CameraNode * 
GSE_SceneRenderer::GetCurrentCameraNode()
{
  return m_pCameraNode;
}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::AddToGraphWithLighting( GSE_SceneObjectNode *pObjectNode )
{

  // if object is allowed to be lit
  if ( pObjectNode->GetLightClusterNode() != NULL ) 
  {
    if ( pObjectNode->GetIlluminationSet() != NULL &&
	 pObjectNode->GetIlluminationSet()->GetSize() > 0 )
    {
      GSE_Vector3 vObjectPos(pObjectNode->GetWorldTransform().At(0,3),
			     pObjectNode->GetWorldTransform().At(1,3),
			     pObjectNode->GetWorldTransform().At(2,3));
      // Sorts the lights of the illumination set and 
      // assigns them to lightcluster
      pObjectNode->GetIlluminationSet()->Apply( pObjectNode->GetLightClusterNode(), 
						vObjectPos );
      pObjectNode->GetLightClusterNode()->SetIlluminationEnabled( 1 );
    }
    else
    {
      pObjectNode->GetLightClusterNode()->ResetLights();
      pObjectNode->GetLightClusterNode()->SetIlluminationEnabled( 0 );
    }
    // insert lightcluster to rendernode stack 
    m_RenderNodeStack.top()->AddEdge(pObjectNode->GetLightClusterNode());
    m_RenderNodeStack.push( pObjectNode->GetLightClusterNode());
  }
  // In the end, add edge to the object itself
  m_RenderNodeStack.top()->AddEdge( pObjectNode->GetRenderNode());
  // Mark node rendered ( or actually inserted into the RenderGraph)
  pObjectNode->SetRendered(1);  
  AddCollectedObject(pObjectNode);
}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::AddToGraphWithLighting( GSE_SceneOctreeObjectNode *pObjectNode )
{


  // if object is allowed to be lit
  if ( pObjectNode->GetLightClusterNode() != NULL ) 
  {
    if ( pObjectNode->GetIlluminationSet() != NULL &&
	 pObjectNode->GetIlluminationSet()->GetSize() > 0 )
    {
      GSE_Vector3 vObjectPos(pObjectNode->GetWorldTransform().At(0,3),
			     pObjectNode->GetWorldTransform().At(1,3),
			     pObjectNode->GetWorldTransform().At(2,3));
      // Sorts the lights of the illumination set and 
      // assigns them to lightcluster
      pObjectNode->GetIlluminationSet()->Apply( pObjectNode->GetLightClusterNode(), 
						vObjectPos );
      pObjectNode->GetLightClusterNode()->SetIlluminationEnabled( 1 );
    }
    else
    {
      pObjectNode->GetLightClusterNode()->ResetLights();
      pObjectNode->GetLightClusterNode()->SetIlluminationEnabled( 0 );
    }
    // insert lightcluster to rendernode stack 
    m_RenderNodeStack.top()->AddEdge(pObjectNode->GetLightClusterNode());
    m_RenderNodeStack.push( pObjectNode->GetLightClusterNode());
  }

  // In the end, add edge to the object itself
  m_RenderNodeStack.top()->AddEdge( pObjectNode->GetRenderNode());
  // Mark node rendered ( or actually inserted into the RenderGraph)
  pObjectNode->SetRendered(1);  

  AddCollectedObject(pObjectNode);
}

/////////////////////////////////////////////////////////////////
int
GSE_SceneRenderer::Handle_CollectGraph_RenderPass_Enter( GSE_SceneNode *pNode )
{
  int iCulled = 0;

  // Each transform must be taken into account.
  m_RenderNodeStack.top()->AddEdge( pNode->m_pLocalTransform );
  m_RenderNodeStack.push(pNode->m_pLocalTransform);

  switch ( pNode->GetType())
  {
  case ROOT:
    ClearCollectedObjects();
  case LIMB:
    break;
  case RENDERSETTINGS:
    {
      GSE_SceneRenderSettingsChainNode *pTmp = static_cast<GSE_SceneRenderSettingsChainNode *>(pNode);
      pTmp->GetBegin();
      m_RenderNodeStack.top()->AddEdge( pTmp->GetBegin());
      m_RenderNodeStack.push(pTmp->GetEnd());
    }
    break;
  case CAMERA:
    break;
  case OBJECT:
    {
      GSE_SceneObjectNode *pTmpNode = static_cast<GSE_SceneObjectNode *>(pNode);
      
      // Add this node as child immediately
      if ( IS_OVERRIDE_CULLING_SET() )
      {       

	m_OverrideCullingStack.push('o');
	// If the object has geometry and has not been rendered yet
	if ( OBJECT_HAS_GEOMETRY(pTmpNode) && !pTmpNode->IsRendered() )
	{
	  AddToGraphWithLighting( pTmpNode );
	} 
	else 
	{
	  GSE_DEBUG("object node is a grouping node ");
	}
	break;
      } 
     
      // If there's no camera to cull against, we take it.
      if ( GetCurrentCameraNode() == NULL )
      {
	AddToGraphWithLighting( pTmpNode );
      } 
      else 
      {
	// Check intersections before adding
	if ( !Geometry::SphereIntersectsSphere( pTmpNode->GetHierarchialBoundingSphere(), 
						m_pCameraNode->GetCamera()->FrustumSphere() ))
	{
	  GSE_DEBUG("Culled by sphere-sphere");
	  iCulled = 1;
	  break;
	} 
	if ( !Geometry::SphereIntersectsCone( pTmpNode->GetHierarchialBoundingSphere(), 
					      m_pCameraNode->GetCamera()->FrustumCone() ))
	{
	  GSE_DEBUG("Culled by sphere-cone");
	  iCulled = 1;
	  break;
	}
	// ---------------------------------------------------------------------------
	// Check directly against frustum
	switch ( m_pCameraNode->GetCamera()->Frustum().IntersectsSphere( pTmpNode->GetHierarchialBoundingSphere() ))
	{
	  /////////////////////////////////////////////////////////////////
	  // In this case, all child object are outside the frustum.
	case GSE_Frustum::OUTSIDE: 
	  GSE_DEBUG("Culled by sphere-frusum");
	  iCulled = 1;    
	  break;
	  /////////////////////////////////////////////////////////////////
	  // In this case, all child object are inside the frustum and will be 
	  // accepted without checks.
	case GSE_Frustum::INSIDE:
	  if ( OBJECT_HAS_GEOMETRY(pTmpNode) && !pTmpNode->IsRendered())
	  {
	    AddToGraphWithLighting( pTmpNode );
	  }
	  m_OverrideCullingStack.push('o');
	  break;
	  /////////////////////////////////////////////////////////////////
	  // Intersection requires individual checks for each child.
	case GSE_Frustum::INTERSECTION:      
	  /////////////////////////////////////////////////////////////////
	  // BoundingBox check; Children nodes should be checked individually.
	  switch ( m_pCameraNode->GetCamera()->Frustum().IntersectsOrientedBox( pTmpNode->GetHierarchialBoundingBox()))
	  {
	  case GSE_Frustum::INTERSECTION:
	  case GSE_Frustum::INSIDE:
	    if ( OBJECT_HAS_GEOMETRY(pTmpNode) && !pTmpNode->IsRendered() )
	    {
	      AddToGraphWithLighting( pTmpNode );
	    }
	    break;
	  case GSE_Frustum::OUTSIDE:
	    GSE_DEBUG("Culled by OBB-frustum");
	    iCulled = 1;
	    break;
	  }
	  break;

	} // switch
      } //if ( GetCameraNode() == NULL ) ...
    } // case OBJECT
    break;
  case OCTREEOBJECT:
    {
      GSE_SceneOctreeObjectNode *pTmpNode = static_cast<GSE_SceneOctreeObjectNode *>(pNode);
      ////////////////////
      // Check intersections before adding
      if ( !Geometry::SphereIntersectsSphere( pTmpNode->GetHierarchialBoundingSphere(), 
					      m_pCameraNode->GetCamera()->FrustumSphere() ))
      {
	GSE_DEBUG("Culled by sphere-sphere");
	iCulled = 1;
	break;
      } 
      ////////////////////
      if ( !Geometry::SphereIntersectsCone( pTmpNode->GetHierarchialBoundingSphere(), 
					    m_pCameraNode->GetCamera()->FrustumCone() ))
      {
	GSE_DEBUG("Culled by sphere-cone");
	iCulled = 1;
	break;
      }
      ////////////////////
      switch ( m_pCameraNode->GetCamera()->Frustum().IntersectsOrientedBox( pTmpNode->GetHierarchialBoundingBox()))
      {
      case GSE_Frustum::INTERSECTION:
      case GSE_Frustum::INSIDE:
	AddToGraphWithLighting( pTmpNode );
	CollectOctreeNodes( m_pCameraNode->GetCamera(), pTmpNode->GetGeometryNode(),
			    pTmpNode->GetGeometryOctree(), 1);	
	break;
      case GSE_Frustum::OUTSIDE:
	GSE_DEBUG("Culled by OBB-frustum");
	iCulled = 1;
	break;
      }
    } // case OCTREEOBJECT
    break; 
  case PARTICLESYSTEM:
    {
      GSE_SceneParticleSystemNode *pTmpNode = static_cast<GSE_SceneParticleSystemNode *>(pNode);

      // Add this node as child immediately
      if ( IS_OVERRIDE_CULLING_SET() )
      {       
	m_OverrideCullingStack.push('o');
	AddCollectedObject( pTmpNode );
	if ( OBJECT_HAS_GEOMETRY(pTmpNode) && !pTmpNode->IsRendered() )
	{
	  m_RenderNodeStack.top()->AddEdge( pTmpNode->GetRenderNode());
	} 
	break;
      } 
     
      // If there's no camera to cull against, we take it.
      if ( GetCurrentCameraNode() == NULL )
      {
	m_RenderNodeStack.top()->AddEdge( pTmpNode->GetRenderNode());
      } 
      else 
      {
	// Check intersections before adding
	if ( !Geometry::SphereIntersectsSphere( pTmpNode->GetHierarchialBoundingSphere(), 
						m_pCameraNode->GetCamera()->FrustumSphere() ))
	{
	  iCulled = 1;
	  break;
	} 
	if ( !Geometry::SphereIntersectsCone( pTmpNode->GetHierarchialBoundingSphere(), 
					      m_pCameraNode->GetCamera()->FrustumCone() ))
	{
	  iCulled = 1;
	  break;
	}
	// ---------------------------------------------------------------------------
	// Check directly against frustum
	switch ( m_pCameraNode->GetCamera()->Frustum().IntersectsSphere( pTmpNode->GetHierarchialBoundingSphere() ))
	{
	  /////////////////////////////////////////////////////////////////
	  // In this case, all child object are outside the frustum.
	case GSE_Frustum::OUTSIDE: 
	  GSE_DEBUG("Culled by sphere-frusum");
	  iCulled = 1;    
	  break;
	  /////////////////////////////////////////////////////////////////
	  // In this case, all child object are inside the frustum and will be 
	  // accepted without checks.
	case GSE_Frustum::INSIDE:
	  if ( OBJECT_HAS_GEOMETRY(pTmpNode) && !pTmpNode->IsRendered())
	  {
	    m_RenderNodeStack.top()->AddEdge( pTmpNode->GetRenderNode() );
	    AddCollectedObject( pTmpNode );
	  }
	  m_OverrideCullingStack.push('o');
	  break;
	  /////////////////////////////////////////////////////////////////
	  // Intersection requires individual checks for each child.
	case GSE_Frustum::INTERSECTION:      
	  /////////////////////////////////////////////////////////////////
	  // BoundingBox check; Children nodes should be checked individually.
	  switch ( m_pCameraNode->GetCamera()->Frustum().IntersectsOrientedBox( pTmpNode->GetHierarchialBoundingBox()))
	  {
	  case GSE_Frustum::INTERSECTION:
	  case GSE_Frustum::INSIDE:
	    if ( OBJECT_HAS_GEOMETRY(pTmpNode) && !pTmpNode->IsRendered() )
	    {
	      m_RenderNodeStack.top()->AddEdge( pTmpNode->GetRenderNode() );
	      AddCollectedObject( pTmpNode );
	    }
	    break;
	  case GSE_Frustum::OUTSIDE:
	    GSE_DEBUG("Culled by OBB-frustum");
	    iCulled = 1;
	    break;
	  }
	  break;
	} // switch
      } //if ( GetCameraNode() == NULL ) ...
    }
    break;
  case LIGHT:
    {
      /// The light can be checked against a cone ( spotlight ) or a sphere (point light)
      /// If the bounding volume scheme is used. 
    } // case LIGHT
    break;
  case UNDEFINED:
    // NOP
    break;
  }
  pNode->SetVisible(!iCulled);
  return iCulled;
}
/////////////////////////////////////////////////////////////////
int 
GSE_SceneRenderer::Handle_CollectGraph_RenderPass_Leave( GSE_SceneNode *pNode )
{
  int iRetval = 0;

  switch ( pNode->GetType())
  {
    // Object and Octree _might_ have a lightclusternode 
  case OBJECT:
    if ( static_cast<GSE_SceneObjectNode *>(pNode)->GetLightClusterNode() != NULL )
      m_RenderNodeStack.pop();
    break;
  case OCTREEOBJECT:
    if ( static_cast<GSE_SceneOctreeObjectNode *>(pNode)->GetLightClusterNode() != NULL )
      m_RenderNodeStack.pop();
    break;
  case ROOT:
  case LIMB:
  case LIGHT:
  case UNDEFINED:
  case CAMERA:
  case RENDERSETTINGS:
  case PARTICLESYSTEM:
    break;
  }
  if ( IS_OVERRIDE_CULLING_SET() )
  {
    m_OverrideCullingStack.pop();
  }
  // Each node contains a transform, which must be removed 
  m_RenderNodeStack.pop();
  return iRetval;
}
/////////////////////////////////////////////////////////////////
int 
GSE_SceneRenderer::Handle_BoundingVolume_RenderPass_Leave( GSE_SceneNode *pNode )
{
  int  iRetval    = 0;
  /////////////////////////////////////////////////////////////////
  switch ( pNode->GetType())
  {
  case LIMB:
    // The hierarchial bounding volume will be at least the size of 
    // world bounding volume.
    pNode->SetHierarchialBoundingBox(    static_cast<GSE_SceneLimbNode *>(pNode)->GetWorldBoundingBox());
    pNode->SetHierarchialBoundingSphere( static_cast<GSE_SceneLimbNode *>(pNode)->GetWorldBoundingSphere());
    // Merge hierarchial bounding volumes of the children to the 
    // object's world bounding volume
    if ( pNode->HasLeavingEdges())
    {
      std::list<GSE_GraphEdge < NodeType > * >::iterator nodeIt = pNode->GetLeavingEdges().begin();
      GSE_OrientedBox box;
      GSE_Sphere sphere;
      for( ; nodeIt != pNode->GetLeavingEdges().end(); nodeIt++)
      {
	GSE_SceneNode *pChild = static_cast<GSE_SceneNode *>( (*nodeIt)->GetToNode() );
	// Calculate merged volume with child an self
	
	box = Geometry::MergeOrientedBoxes( pNode->GetHierarchialBoundingBox(),
					    pChild->GetHierarchialBoundingBox());
	
       	sphere = Geometry::MergeSpheres( pNode->GetHierarchialBoundingSphere(),
					 pChild->GetHierarchialBoundingSphere());
	// Assign volume to self
	pNode->SetHierarchialBoundingBox( box );
	pNode->SetHierarchialBoundingSphere( sphere );
	
      }
    }
    break;
  case OBJECT:
    // The hierarchial bounding volume will be at least the size of 
    // world bounding volume.
    pNode->SetHierarchialBoundingBox(    static_cast<GSE_SceneObjectNode *>(pNode)->GetWorldBoundingBox() );
    pNode->SetHierarchialBoundingSphere( static_cast<GSE_SceneObjectNode *>(pNode)->GetWorldBoundingSphere() );
    // Merge hierarchial bounding volumes of the children to the 
    // object's world bounding volume
    if ( pNode->HasLeavingEdges())
    {
      
      std::list<GSE_GraphEdge < NodeType > * >::iterator nodeIt = pNode->GetLeavingEdges().begin();
      GSE_OrientedBox box;
      GSE_Sphere sphere;
      for( ; nodeIt != pNode->GetLeavingEdges().end(); nodeIt++)
      {
	GSE_SceneNode *pChild = static_cast<GSE_SceneNode *>( (*nodeIt)->GetToNode() );
	// Calculate merged volume with child an self
	box = Geometry::MergeOrientedBoxes( pNode->GetHierarchialBoundingBox(),
					    pChild->GetHierarchialBoundingBox());
	
       	sphere = Geometry::MergeSpheres( pNode->GetHierarchialBoundingSphere(),
					 pChild->GetHierarchialBoundingSphere());
	// Assign volume to self
	pNode->SetHierarchialBoundingBox( box );
	pNode->SetHierarchialBoundingSphere( sphere );
	
      }
    }
    break;
  case OCTREEOBJECT:
    // The hierarchial bounding volume will be at least the size of 
    // world bounding volume.
    pNode->SetHierarchialBoundingBox(    static_cast<GSE_SceneOctreeObjectNode *>(pNode)->GetWorldBoundingBox() );
    pNode->SetHierarchialBoundingSphere( static_cast<GSE_SceneOctreeObjectNode *>(pNode)->GetWorldBoundingSphere() );
    // Merge hierarchial bounding volumes of the children to the 
    // object's world bounding volume
    if ( pNode->HasLeavingEdges())
    {
      std::list<GSE_GraphEdge < NodeType > * >::iterator nodeIt = pNode->GetLeavingEdges().begin();
      GSE_OrientedBox box;
      GSE_Sphere sphere;
      for( ; nodeIt != pNode->GetLeavingEdges().end(); nodeIt++)
      {
	GSE_SceneNode *pChild = (GSE_SceneNode *)((*nodeIt)->GetToNode());
	// Calculate merged volume with child an self
	box = Geometry::MergeOrientedBoxes( pNode->GetHierarchialBoundingBox(),
					    pChild->GetHierarchialBoundingBox());
       	sphere = Geometry::MergeSpheres( pNode->GetHierarchialBoundingSphere(),
					 pChild->GetHierarchialBoundingSphere());
	// Assign volume to self
	pNode->SetHierarchialBoundingBox( box );
	pNode->SetHierarchialBoundingSphere( sphere );
      }
    }
    break;
  case PARTICLESYSTEM:
    {
      GSE_ParticleSystemNode *pRenderParticleSystemNode = static_cast<GSE_SceneParticleSystemNode *>(pNode)->GetRenderParticleSystemNode();
      if ( pRenderParticleSystemNode  != NULL && pRenderParticleSystemNode->GetParticleSystem() != NULL)
      {
	pNode->SetHierarchialBoundingBox( pRenderParticleSystemNode->GetParticleSystem()->GetBoundingBox() );
	pNode->SetHierarchialBoundingSphere( pRenderParticleSystemNode->GetParticleSystem()->GetBoundingSphere() );
      }
    }
    break;
  case ROOT:
  case UNDEFINED:
  case RENDERSETTINGS:
  case CAMERA:
  case LIGHT: // Although light has a bounding volume, it is used per-object basis instead.
    {
      /////////////////////////////////////////////////////////////////
      GSE_OrientedBox box;         // The temp bounding box 
      GSE_Sphere      sphere;      // The temp bounding sphere
      /////////////////////////////////////////////////////////////////
      if ( pNode->HasLeavingEdges())
      {
	std::list<GSE_GraphEdge < NodeType > * >::iterator nodeIt = 
	  pNode->GetLeavingEdges().begin();

	// Volume of the first child will be used as a basis
	GSE_SceneNode *pChild = static_cast<GSE_SceneNode *>( (*nodeIt)->GetToNode() );
	// Assign volume to self
	pNode->SetHierarchialBoundingBox(    pChild->GetHierarchialBoundingBox()    );
	pNode->SetHierarchialBoundingSphere( pChild->GetHierarchialBoundingSphere() );
	// Next child
	nodeIt++;

	for( ; nodeIt != pNode->GetLeavingEdges().end(); nodeIt++)
	{
	  pChild = static_cast<GSE_SceneNode *>( (*nodeIt)->GetToNode() );
	  // Calculate merged volume with child an self
	  box    = Geometry::MergeOrientedBoxes( pNode->GetHierarchialBoundingBox(),
						 pChild->GetHierarchialBoundingBox());
	  sphere = Geometry::MergeSpheres( pNode->GetHierarchialBoundingSphere(),
					   pChild->GetHierarchialBoundingSphere());
	  // Assign volume to self
	  pNode->SetHierarchialBoundingBox( box );
	  pNode->SetHierarchialBoundingSphere( sphere );
	
	}
      }
      else
      {
	// Collapse volumes 
	box.SetWidth(0.0f);
	box.SetHeight(0.0f);
	box.SetLength(0.0f);
	sphere.SetRadius(0.0f);
	pNode->SetHierarchialBoundingBox( box );
	pNode->SetHierarchialBoundingSphere( sphere );
      }
    }
    break;
  }
  return iRetval;
}
/////////////////////////////////////////////////////////////////
int
GSE_SceneRenderer::Handle_DebugPrint_RenderPass_Enter( GSE_SceneNode *pNode )
{
  // Bounding volume construction procedure MUST NOT CULL any objects, Duh!
  int iCulled = 0;

  GSE_Logger::Error() << m_sPadding <<  "|" << std::endl;
  switch ( pNode->GetType())
  {
  case ROOT:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "Scene Root";
    break;
  case OBJECT:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "Object";
    break;
  case LIMB:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "Limb";
    break;
  case LIGHT:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "Light";
    break;
  case UNDEFINED:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "Undefined" ;
    break;
  case RENDERSETTINGS:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "Rendersettings" ;
    break;
  case CAMERA:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "Camera" ;
    break;
  case OCTREEOBJECT:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "OctreeObject" ;
    break;
  case PARTICLESYSTEM:
    GSE_Logger::Error() << m_sPadding << m_sArrow << "ParticleSystem" ;
    break;
  }
  GSE_Logger::Error() << " " << pNode->GetName() << "(" << pNode << ")" << endl;
  GSE_Logger::Error() << "OBB:"    << pNode->GetHierarchialBoundingBox()    << std::endl;
  GSE_Logger::Error() << "Sphere:" << pNode->GetHierarchialBoundingSphere() << std::endl;
  
  m_sPadding.append(3,' ');  
  return iCulled;
}
/////////////////////////////////////////////////////////////////
int 
GSE_SceneRenderer::Handle_DebugPrint_RenderPass_Leave( GSE_SceneNode *pNode )
{
  int iRetval = 0;
  m_sPadding.erase(0,3);
  return iRetval;
}
/////////////////////////////////////////////////////////////////
void
GSE_SceneRenderer::ForceOverrideCulling()
{
  if ( !IS_OVERRIDE_CULLING_SET()) {
    m_OverrideCullingStack.push('o');
  }
}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::SetAttachingNode( GSE_RenderNode *pNode )
{
  
  m_RenderNodeStack.push( pNode );
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::SetOglRenderer( GSE_OglRenderer *pRenderer )
{
  m_pOglRenderer = pRenderer;
}
/////////////////////////////////////////////////////////////////
int 
GSE_SceneRenderer::Handle_Cleanup_RenderPass_Enter( GSE_SceneNode *pNode )
{
  int bCulled = 0;
  /////////////////////////////////////////////////////////////////
  // Determine what kind of fish we're dealing with
  // and remove arriving edges from the first render / light node 
  pNode->SetRendered(0);
  pNode->m_pLocalTransform->RemoveArrivingEdges();
  /////////////////////////////////////////////////////////////////
  switch( pNode->GetType() )
  {
  case OBJECT:
    {
      GSE_SceneObjectNode *pSceneObjNode = (GSE_SceneObjectNode *)pNode;
      if ( pSceneObjNode->GetRenderNode() != NULL )
      {
	pSceneObjNode->GetRenderNode()->RemoveArrivingEdges();
      }
      if ( pSceneObjNode->GetLightClusterNode() != NULL )
      {
	pSceneObjNode->GetLightClusterNode()->RemoveArrivingEdges();
      }
    }
    break;
  case OCTREEOBJECT:
    {
      GSE_SceneOctreeObjectNode *pSceneObjNode = (GSE_SceneOctreeObjectNode *)pNode;
      if ( pSceneObjNode->GetRenderNode() != NULL )
      {
	// We remove all edges that point into this node.
	pSceneObjNode->GetRenderNode()->RemoveArrivingEdges();
	// We also remove all other indexbuffer nodes, which 
	// have been attached here.
	pSceneObjNode->GetGeometryNode()->RemoveLeavingEdges();
      }
      if ( pSceneObjNode->GetLightClusterNode() != NULL )
      {
	pSceneObjNode->GetLightClusterNode()->RemoveArrivingEdges();
      }
    }
    break;
  case LIGHT:
    break;
  case PARTICLESYSTEM:
    {
      GSE_SceneParticleSystemNode *pPSNode = (GSE_SceneParticleSystemNode *)pNode;
      if ( pPSNode->GetRenderNode() != NULL )
      {
	pPSNode->GetRenderNode()->RemoveArrivingEdges();
      }
    }
    break;
  case CAMERA:
    ((GSE_SceneCameraNode *)pNode)->m_pCameraNode->RemoveArrivingEdges();
    break;
  case RENDERSETTINGS:
    ((GSE_SceneRenderSettingsChainNode *)pNode)->GetBegin()->RemoveArrivingEdges();
    ((GSE_SceneRenderSettingsChainNode *)pNode)->GetEnd()->RemoveLeavingEdges();
    break;
  case ROOT:
  case LIMB:
    break;
  case UNDEFINED:
    //GSE_ERR("Undefined scenenode! Aargh!");
    break;
  }
  /////////////////////////////////////////////////////////////////
  return bCulled;
}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::SetVisibilityCullingKDOP( GSE_KDOP &kDop )
{
  m_kDOP = kDop;
}
/////////////////////////////////////////////////////////////////
int 
GSE_SceneRenderer::Handle_UpdateTransforms_Enter( GSE_SceneNode *pNode )
{
  int bCulled = 0;
  /////////////////////////////////////////////////////////////////
  // The actual transform calculation
  /////////////////////////////////////////////////////////////////
  if ( pNode->GetType() == ROOT )  
  {
    // Root has World = Local;
    pNode->SetWorldTransform(pNode->m_pLocalTransform->GetMatrix() );
  } 
  else 
  {
    pNode->SetWorldTransform( m_SceneTravellerStack.top()->GetWorldTransform() *
			      pNode->GetLocalTransform() );
  }
  /////////////////////////////////////////////////////////////////
  // Transformation is applied to different nodes on type-basis
  /////////////////////////////////////////////////////////////////
  switch ( pNode->GetType())
  {
  case CAMERA:
    {
      GSE_Camera *pCamera = ((GSE_SceneCameraNode *)pNode)->GetCamera();
      /////////////////////////////////////////////////////////////////
      // Translate camera according to world transformation
      pCamera->SetPosition(pNode->GetWorldTransform().At(0,3),
			   pNode->GetWorldTransform().At(1,3),
			   pNode->GetWorldTransform().At(2,3));
      /////////////////////////////////////////////////////////////////
      // Rotate camera accorinding to world rotation.
      pCamera->SetRotation(pNode->GetWorldTransform());
      pCamera->UpdateView();

    }
    break;
  case LIGHT:
    {

      /////////////////////////////////////////////////////////////////
      // Transform the bounding volumes for light
      /////////////////////////////////////////////////////////////////
      GSE_SceneLightNode *pTmpNode = static_cast<GSE_SceneLightNode *>(pNode);
      TransformBoundingVolumes<GSE_SceneLightNode>(pTmpNode);
      /////////////////////////////////////////////////////////////////
      /// Transform the light 
      if ( pTmpNode->GetLight() != NULL )
      {
	/////////////////////////////////////////////////////////////////
	// Position translation
	pTmpNode->GetLight()->SetPosition( pTmpNode->GetWorldTransform().At(0,3),
					   pTmpNode->GetWorldTransform().At(1,3),
					   pTmpNode->GetWorldTransform().At(2,3) );
	/////////////////////////////////////////////////////////////////
	// Light direction is by default negative z-axis.
	pTmpNode->GetLight()->m_vDirection = GSE_Vector3(0,0,-1);
	/////////////////////////////////////////////////////////////////
	// We rotate it by world transform, ignoring the original setting.
 	pTmpNode->GetLight()->m_vDirection = Math::Rotate(pTmpNode->GetLight()->m_vDirection,
							  pTmpNode->GetWorldTransform());

      }
      if ( IS_OVERRIDE_CULLING_SET())
      { 
	m_OverrideCullingStack.push('o');
      }
    }    
    break;
  case OBJECT:
    /////////////////////////////////////////////////////////////////
    // Transform the bounding volumes for an object
    /////////////////////////////////////////////////////////////////
    TransformBoundingVolumes<GSE_SceneObjectNode>(static_cast<GSE_SceneObjectNode *>(pNode));
    break;
  case OCTREEOBJECT:
    /////////////////////////////////////////////////////////////////
    // Transform the bounding volumes for an object
    /////////////////////////////////////////////////////////////////
    TransformBoundingVolumes<GSE_SceneOctreeObjectNode>( static_cast<GSE_SceneOctreeObjectNode *>(pNode));
    break;
  case LIMB:
    /////////////////////////////////////////////////////////////////
    // Transform the bounding volumes for a limb
    /////////////////////////////////////////////////////////////////
    TransformBoundingVolumes<GSE_SceneLimbNode>( static_cast<GSE_SceneLimbNode *>(pNode));
    break;
  case PARTICLESYSTEM:
  case ROOT:
  case UNDEFINED:
  case RENDERSETTINGS:
    break;
  }

  return bCulled;
}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::CollectOctreeNodes( GSE_Camera *pCamera, GSE_GeometryNode *pGeometryNode,
				       GSE_GeometryOctree *pGeometryOctree, int bUseCulling)
{
  if ( pGeometryOctree == NULL ) return;
  if ( bUseCulling )
  {
    /////////////////////////////////////////////////////////////////
    switch ( pCamera->Frustum().IntersectsAABB( static_cast<GSE_AxisAlignedBox>(*pGeometryOctree) ))
    {
    case GSE_Frustum::INSIDE:
      if ( !pGeometryOctree->HasChildren())
      {
	//assert(pGeometryOctree->GetElementsNode() != NULL )
	pGeometryNode->AddEdge( pGeometryOctree->GetIndexBufferNode());
	// No need to test children
	bUseCulling = 0;
      }
      break;
    case GSE_Frustum::INTERSECTION:
      if ( !pGeometryOctree->HasChildren())
      {
	pGeometryNode->AddEdge( pGeometryOctree->GetIndexBufferNode());
      }
      break;
    case GSE_Frustum::OUTSIDE:
      return;
    }
  } 
  else
  {
    // All kids are included without testing
    if ( !pGeometryOctree->HasChildren())
    {
      pGeometryNode->AddEdge( pGeometryOctree->GetIndexBufferNode());
      return;
    }
  }
  // Collect children, if any
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::TOP_LEFT_FRONT]    ), bUseCulling);
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::TOP_LEFT_BACK]     ), bUseCulling);
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::TOP_RIGHT_FRONT]   ), bUseCulling);
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::TOP_RIGHT_BACK]    ), bUseCulling);
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::BOTTOM_LEFT_FRONT] ), bUseCulling);
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::BOTTOM_LEFT_BACK]  ), bUseCulling);
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::BOTTOM_RIGHT_FRONT]), bUseCulling);
  CollectOctreeNodes( pCamera, pGeometryNode, (GSE_GeometryOctree *)(pGeometryOctree->GetNodes()[GSE_Octree::BOTTOM_RIGHT_BACK] ), bUseCulling);
}
///////////////////////////////////////////////////////////////// 
void 
GSE_SceneRenderer::AddCollectedObject( GSE_SceneNode *pNode )
{
  m_lstObjects.push_back(pNode);
}
///////////////////////////////////////////////////////////////// 
ObjectPtrList & 
GSE_SceneRenderer::GetCollectedObjects()
{
  return m_lstObjects;
}
/////////////////////////////////////////////////////////////////
void 
GSE_SceneRenderer::ClearCollectedObjects()
{
  m_lstObjects.clear();
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_SceneRenderer.cpp,v $
// Revision 1.48  2007/05/22 08:29:56  entity
// Collected Objects List added
//
// Revision 1.47  2007/05/14 09:48:13  entity
// CollectGraph sets now visiblity flag
//
// Revision 1.46  2007/04/03 20:25:01  entity
// working particlesystem in scenegraph
//
// Revision 1.45  2007/04/03 12:53:31  entity
// SceneParticleSystem changes
//
// Revision 1.44  2007/04/02 11:12:09  entity
// new CollectOctreenNodes()
//
// Revision 1.43  2007/04/01 21:05:16  entity
// static_cast
//
// Revision 1.42  2007/04/01 15:25:05  entity
// texture system overhaul, Elements->Indexbuffer rename
//
// Revision 1.41  2007/03/30 12:59:28  entity
// static_cast added
//
// Revision 1.40  2007/03/29 21:16:35  entity
// Octree culling enabled
//
// Revision 1.39  2007/03/29 13:13:58  entity
// fixed invalid casting of pointer
//
// Revision 1.38  2007/03/28 10:33:41  entity
// CollectOctreeNodes added,
// put huge blocks of similar code behind template function
//
// Revision 1.37  2007/03/27 12:06:07  entity
// some comments added
//
// Revision 1.36  2007/03/25 15:34:16  entity
// Collected object count in use
//
// Revision 1.35  2007/03/23 13:29:45  entity
// LIMB node added
//
// Revision 1.34  2007/03/22 06:30:06  entity
// renamed a render pass
//
// Revision 1.33  2007/03/21 10:36:35  entity
// Unused code removed,
// Math::Translate on UpdateTransforms to Math::Transform so
/////////////////////////////////////////////////////////////////
