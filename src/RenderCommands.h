/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of Phoenix.
 *
 *   Phoenix is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    Debris is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Phoenix; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
// file : GSE_datastructures.h
// desc : contains various Graph Nodes for different purposes.
/////////////////////////////////////////////////////////////////
#ifndef __RenderCommands_h__
#define __RenderCommands_h__
/////////////////////////////////////////////////////////////////
#include "GSE_math.h"
#include "GSE_interfaces.h"
#include "GSE_structures.h"
#include "GSE_Camera.h"
#include "GSE_graph.h"
#include "GSE_Light.h"
#include "GSE_Material.h"
#include "GSE_Texture.h"
#include "GSE_shaders.h"
#include "GSE_graph.h"
#include "GSE_debug.h"
#include "GSE_OglCache.h"
#include "GSE_IndexBuffer.h"
#include "GSE_geometry.h"
#include "GSE_IlluminationSet.h"
#include "GSE_GeometryData.h"
#include "GSE_particlesystems.h"
/////////////////////////////////////////////////////////////////
class GSE_GeometryOctree;
/////////////////////////////////////////////////////////////////
/// Render Graph Headers
/// Render Graph is implemented as a N-tree.
/////////////////////////////////////////////////////////////////
namespace RenderNode {
  /// The enumeration of types for classes which is used to reduce 
  /// the overhead of dynamic_cast<..:>
  typedef enum {
    UNDEFINED = 0,  
    TRANSLATION = 1, 
    ROTATION,        
    GEOMETRY,        
    CAMERA,          
    GROUP,           
    LIGHT_CLUSTER,   
    FRONT_MATERIAL,  
    BACK_MATERIAL,   
    INDEXBUFFER,     
    TEXTURE,
    SHADER,
    VERTEXATTRIB,
    GLOBAL_LIGHTING,
    PARTICLESYSTEM,
    POLYGONMODE,
    UTIL_SPHERE,
    UTIL_BOX,
    UTIL_CONE,
    UTIL_QUAD,
    UTIL_LINESTRIP,
    CACHE,
    SELECTION,
    TEXT,
    COLOR,
    SCISSOR,
    POLYLINE,
    CLEARBUFFER,
    RENDERSTATE_BLEND,
    RENDERSTATE_CULLING,
    RENDERSTATE_DEPTHBUFFER,
    RENDERSTATE_STACK,
    ALPHA_TEST,
    KDOP,
    SHADEMODEL,
    TRANSFORM,
    OCTREE,
    ROOT
  
  } NodeType;

} // end namespace RenderNode
/////////////////////////////////////////////////////////////////
typedef GSE_Graph<RenderNode::NodeType> GSE_RenderGraph;
typedef GSE_GraphNode<RenderNode::NodeType> GSE_RenderNode;
/////////////////////////////////////////////////////////////////
class GSE_OctreeNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  GSE_GeometryOctree *m_pOctree;
  GSE_OctreeNode() : m_pOctree(NULL) { SetType(RenderNode::OCTREE); }
  ~GSE_OctreeNode(){}
 public:
  ////////////////////
  void SetOctree( GSE_GeometryOctree *pOctree )
  {
    m_pOctree = pOctree;
  }
  ////////////////////
  GSE_GeometryOctree * GetOctree()
  {
    return m_pOctree;
  }
};
/////////////////////////////////////////////////////////////////
class GSE_PolyLineNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  vector< GSE_Vector3 > m_vecPoints;
  float			m_fWidth;
  GSE_PolyLineNode()
  {
    SetType(RenderNode::POLYLINE);
    m_fWidth		= 1.0f;
  }
  ~GSE_PolyLineNode()
  {
    m_vecPoints.clear();
  }
 public:
  inline const size_t GetPointCount() const 
  {
    return m_vecPoints.size();
  }
  inline GSE_Vector3 & operator[](unsigned int nIndex)
  {
    return m_vecPoints[nIndex % GetPointCount()];
  }
  inline void AddPoint( const GSE_Vector3 & ref )
  {
    m_vecPoints.push_back( ref );
  }
  inline void AddPoint( float fX, float fY, float fZ ) 
  {
    m_vecPoints.push_back( GSE_Vector3( fX, fY, fZ) );
  }
  inline void SetWidth( float fWidth ) 
  {
    m_fWidth = fWidth;
  }
  inline float GetWidth() const
  {
    return m_fWidth;
  }
  inline void Clear()
  {
    m_vecPoints.clear();
  }
};
/////////////////////////////////////////////////////////////////
/// \brief The Root node for RenderGraph
class GSE_RenderRoot : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief The constructor.
  GSE_RenderRoot()
  {
    SetType(RenderNode::ROOT);
  }
 public:
};
/////////////////////////////////////////////////////////////////
/// \brief The buffer clearing node.
/// When renderer encounters this node, it cleans the selected buffers.
class GSE_ClearBufferNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The color which screen is cleared with (RGBA)
  GSE_Color m_vClearColor;		
  /// Flag indicating should the color buffer be cleared
  char	    m_bClearColorBuffer;	
  /// The value which is used when clearing the depth buffer, range [0,1]
  float	    m_fClearDepthValue;		
  /// Flag indicating should the depth buffer be cleared
  char	    m_bClearDepthBuffer;	
  /// The value which is used to clear the stencil buffer
  int	    m_iClearStencilValue;	
  /// Flag indicating should the stencil buffer be cleared
  char	    m_bClearStencilBuffer;      
  /// The RGBA color which is used to clear the accumulation buffer.
  GSE_Color m_vClearAccumColor;		  
  /// Flag indicating should the accumulation buffer be cleared
  char	    m_bClearAccumBuffer;        

  /// \brief The construtor.
  GSE_ClearBufferNode() : GSE_RenderNode() 
  {
    SetType(RenderNode::CLEARBUFFER);

    SetClearStencilBuffer(0);
    SetClearStencilValue(1);
    
    // By default, the clear color for color buffer is black
    SetClearColorBuffer(0);
    m_vClearColor.Set(0.0, 0.0, 0.0, 1.0);

    SetClearDepthBuffer(1);
    SetClearDepthValue(1.0f);
    
    SetClearAccumBuffer(0);
    m_vClearAccumColor.Set(0.0, 0.0, 0.0, 0.0);
    
  }
 public:
  /// \brief  Returns the reference to the color used to clear the colorbuffer.
  /// \return GSE_Color The color which is used to clear the 
  /// colorbuffer.
  GSE_Color & ClearColor()
  {
    return m_vClearColor;
  }

  /// \brief Assigns the stencil buffer clearing.
  /// \param bFlag boolean 1 if should clear, 0 if not.
  inline void SetClearStencilBuffer( char bFlag )
  {
    m_bClearStencilBuffer = bFlag;
  }
  /// \brief Assigns the depth buffer clearing.
  /// \param bFlag boolean 1 if should clear, 0 if not.
  inline void SetClearDepthBuffer( char bFlag )
  {
    m_bClearDepthBuffer = bFlag;
  }
  /// \brief Assigns the color buffer clearing.
  /// \param bFlag boolean 1 if should clear, 0 if not.
  inline void SetClearColorBuffer( char bFlag )
  {
    m_bClearColorBuffer = bFlag;
  }
  /// \brief Assigns the accum buffer clearing.
  /// \param bFlag boolean 1 if should clear, 0 if not.
  inline void SetClearAccumBuffer( char bFlag )
  {
    m_bClearAccumBuffer = bFlag;
  }
  /// \brief Returns boolean should the depth buffer be cleared.
  /// \return bFlag boolean 1 if should clear, 0 if not.
  inline char IsDepthBufferCleared()
  {
    return m_bClearDepthBuffer;
  }
  /// \brief Returns boolean should the color buffer be cleared.
  /// \return bFlag boolean 1 if should clear, 0 if not.
  inline char IsColorBufferCleared()
  {
    return m_bClearColorBuffer;
  }
  /// \brief Returns boolean should the stencil buffer be cleared.
  /// \return bFlag boolean 1 if should clear, 0 if not.
  inline char IsStencilBufferCleared()
  {
    return m_bClearStencilBuffer;
  }
  /// \brief Returns boolean should the accumulation buffer be cleared.
  /// \return bFlag boolean 1 if should clear, 0 if not.
  inline char IsAccumBufferCleared()
  {
    return m_bClearAccumBuffer;
  }
  
  /// \brief Assigns the value used when clearing the depth buffer,
  /// clamped into the range [0,1].
  /// \param fValue The value which depth buffer should be cleared with,
  inline void SetClearDepthValue( float fValue  )
  {
    if      ( fValue < 0.0f )	m_fClearDepthValue = 0.0f;
    else if ( fValue > 1.0f)    m_fClearDepthValue = 1.0f;
    else		        m_fClearDepthValue = fValue;
    
  }
  /// \brief Assigns the value used when clearing the stencil buffer.
  /// \param iValue The value which stencil buffer is cleared with.
  inline void SetClearStencilValue( int iValue )
  {
    m_iClearStencilValue = iValue;
  }
  /// \brief Returns the reference to the accumulation buffer clear color.
  /// \return GSE_Color The accumulation buffer clear color.
  GSE_Color & ClearAccumColor()
  {
    return m_vClearAccumColor;
  }
  /// \brief Returns the value which stencil buffer is cleared with.
  /// \return int the value which stencil buffer is cleared with.
  inline int GetClearStencilValue()
  {
    return m_iClearStencilValue;
  }
  /// \brief Returns the value which depth buffer is cleared with.
  /// \return float the value which depth buffer is cleared with.
  inline float GetClearDepthBufferValue()
  {
    return m_fClearDepthValue;
  }
};


/// \brief A class for scissor test. Restricts rendering area.
class GSE_ScissorTestNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The lower-left corner of the scissor box ( x-coordinate in the window)
  int m_iX; 
  /// The lower-left corner of the scissor box ( y-coordinate in the window)
  int m_iY;
  /// The width of the scissor box
  unsigned int m_nWidth;
  /// The height of the scissor box
  unsigned int m_nHeight;
  /// Is the scissor test used 
  char  m_bScissorTestEnabled;
  /// \brief The constructor.
  GSE_ScissorTestNode()
  {
    Set(0,0,0,0);
    SetEnabled(0);
    SetType(RenderNode::SCISSOR);
  }
 public:

  /// \brief Assigns the scissor area.
  /// \param iX the lower-left corner x-coordinate.
  /// \param iY the lower-left corner y-coordinate.
  /// \param nWidth The width of the scissor box.
  /// \param nHeight The height of the scissor box.
  void Set( int iX, int iY, unsigned int nWidth, unsigned int nHeight )
  {
    m_iX = iX;
    m_iY = iY;
    m_nWidth  = nWidth;
    m_nHeight = nHeight;
  }
  /// \brief Assigns the scissor area.
  /// \param pViewport An array of integer values in order X,Y,WIDTH,HEIGHT.
  void Set( int *pViewport )
  {
    if ( pViewport != NULL )
    {
      m_iX = pViewport[0];
      m_iY = pViewport[1];
      m_nWidth  = pViewport[2];
      m_nHeight = pViewport[3];
    }
    else
    {
      GSE_Logger::Error() << DEBUG_HEADER 
			  << "Cannot set scissor test area, pViewport == NULL" 
			  << endl;
    }
  }
  /// \brief Returns the X-coordinate.
  /// \return int X-coordinate.
  inline int GetX()
  {
    return m_iX;
  }
  /// \brief Returns the Y-coordinate.
  /// \return int Y-coordinate.
  inline int GetY()
  {
    return m_iY;
  }
  /// \brief Returns the width if the scissor box.
  /// \return The width as unsigned int
  inline unsigned int GetWidth()
  {
    return m_nWidth;
  }
  /// \brief Returns the height if the scissor box.
  /// \return The height as unsigned int
  inline unsigned int GetHeight()
  {
    return m_nHeight;
  }
  /// \brief Assign the scissor test enabled or disabled.
  /// \param bFlag boolean 1 for enable, 0 for disable.
  inline void SetEnabled( char bFlag )
  {
    m_bScissorTestEnabled = bFlag;
  }
  /// \brief Returns boolean value is scissor test enabled.
  /// \return char boolean true if is enabled, false is disabled.
  inline char IsEnabled()
  {
    return m_bScissorTestEnabled;
  }
  
};

//
// A Generic TransformNode with GSE_Matrix4x4.
//
class GSE_TransformNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  
  GSE_Matrix4x4f m_mMatrix;
  GSE_Matrix4x4f *m_pMatrixHook;

  // Initially assign the m_mTranslation to be unit matrix.  
  GSE_TransformNode() : GSE_RenderNode(), m_mMatrix(GSE_Matrix4x4f::Identity())
  { 
    SetType(RenderNode::TRANSFORM);
    m_pMatrixHook = NULL;
    m_bChanged = 0;

  }
 public:
  // Assigns the m_mMatrix the values from mMatrix
  inline void SetMatrix( const GSE_Matrix4x4f & mMatrix )
  {
    m_mMatrix = mMatrix;
    SetChanged(1);
  }
  // Returns reference to m_mMatrix
  inline const GSE_Matrix4x4f & GetMatrix() const
  {
    if ( m_pMatrixHook != NULL ) return *m_pMatrixHook;
    return m_mMatrix;
  }
  inline void SetHook( GSE_Matrix4x4f *pMatrix )
  {
    m_pMatrixHook = pMatrix;
  }

};
/////////////////////////////////////////////////////////////////
/// A Geometry node. Containes a pointer to the GeometryData.
class GSE_GeometryNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The GSE_GeometryData connected to this node.
  GSE_GeometryData *m_pGeometryData;
  /// The GSE_GeometryData which is edited.
  GSE_GeometryData *m_pGeometryDataWorld;
  /// \brief The constructor.
  GSE_GeometryNode() : GSE_RenderNode()
  {
    m_pGeometryData = NULL; 
    m_pGeometryDataWorld = NULL;
    SetType(RenderNode::GEOMETRY);
  }
 public:

  /// \brief Assigns the GSE_GeometryData to this node.
  /// \param pGeometryData the GSE_GeometryData to be assigned.
  void SetGeometryData( GSE_GeometryData *pGeometryData ) 
  { 
    m_pGeometryData = pGeometryData; 
  }
  /// \brief Returns the assigned GSE_GeometryData.
  /// \return GSE_GeometryData which is assigned.
  GSE_GeometryData *GetGeometryData() 
  { 
    return m_pGeometryData; 
  }
  /// \brief Assigns the GSE_GeometryData to this node which is the edited version.
  /// \param pGeometryData the GSE_GeometryData to be assigned.
  void SetGeometryDataWorld( GSE_GeometryData *pGeometryData )
  {
    m_pGeometryDataWorld = pGeometryData;
  }
  /// \brief Returns the assigned edited GSE_GeometryData.
  /// \return GSE_GeometryData which is assigned.
  GSE_GeometryData *GetGeometryDataWorld()
  {
    return m_pGeometryDataWorld;
  }  
};
/////////////////////////////////////////////////////////////////
/// \brief A class for camera node. 
class GSE_CameraNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief  The pointer to the camera in GSE_CameraMgr's array 
  GSE_Camera *m_pCamera;
  /// \brief  The Constructor
  GSE_CameraNode() : GSE_RenderNode() 
  {
    m_pCamera = NULL;
    SetType(RenderNode::CAMERA);
  }
  /// \brief  The Destructor
  ~GSE_CameraNode() {}
 public:

  /// \brief  Sets the camera used by this node
  /// \param pCamera A pointer to GSE_Camera.
  void SetCamera( GSE_Camera *pCamera )
  {
    m_pCamera = pCamera;
  }
  /// \brief Returns the Index of the Camera
  /// \return A pointer to GSE_Camera which is held by this node.
  GSE_Camera *GetCamera()
  {
    return m_pCamera;
  }

};
/////////////////////////////////////////////////////////////////
/// \brief A simple grouping node which is a dummy holder of several object.
/// It comes in handy when a 3d model consists of several objects which are
/// to be grouped in somehow.
class GSE_GroupNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief The constructor.
  GSE_GroupNode() : GSE_RenderNode() 
  {
    SetType(RenderNode::GROUP);
  }
  
};
/////////////////////////////////////////////////////////////////
/// \brief A LightClusterNode class which determines which of the lights are 
/// activated during rendering this node. Since hardware acceleration imposes 
/// limits to the number of lights used, the cluster contains the GSE_MAX_ACTIVE_LIGHTS
/// lights which are the brightest (and hence the most contributing) lights to the 
/// object.
class GSE_LightClusterNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief A pointer to the light which is assigned to this node.
  GSE_Light *m_pLight[GSE_MAX_ACTIVE_LIGHTS];
  /// \brief Are child nodes illuminated.
  int	     m_bIlluminated;
  /// Default constructor.
  GSE_LightClusterNode() : GSE_RenderNode()
  {
    SetType(RenderNode::LIGHT_CLUSTER);
    ResetLights();
  }  
 public:
  /// Destructor.
  GSE_LightClusterNode(const GSE_LightClusterNode &Node)
  {
    SetType(RenderNode::LIGHT_CLUSTER);
    memcpy(m_pLight,Node.m_pLight,sizeof(GSE_Light *)*GSE_MAX_ACTIVE_LIGHTS);
  }
  /// \brief Assigns pLight to this node
  /// \param nArrayIndex which light in the array is handled.
  /// \param pLight A pointer to GSE_Light object.
  inline void SetLight( unsigned int nArrayIndex, GSE_Light *pLight )
  {
    m_pLight[nArrayIndex % GSE_MAX_ACTIVE_LIGHTS] = pLight;
  }
  /// Returns a pointer to GSE_Light object assigned to this node.
  /// \return A pointer to GSE_Light object assigned to this node.
  inline GSE_Light * GetLight(unsigned int nArrayIndex ) 
  {
    return m_pLight[nArrayIndex % GSE_MAX_ACTIVE_LIGHTS];
  }
  /// \brief Nullifies every light.
  inline void ResetLights()
  {
    memset(m_pLight, 0, sizeof(GSE_Light *)*GSE_MAX_ACTIVE_LIGHTS);
  }
  /// \brief Sets the illumination state.
  /// \param bFlag 1 for enable, 0 for disable.
  inline void SetIlluminationEnabled( int bFlag )
  {
    m_bIlluminated = bFlag;
  }
  /// Returns boolean indicating illumination state.
  /// \returns 1 is enabled, 0 if not.
  int IsIlluminationEnabled()
  {
    return m_bIlluminated;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A baseclass for material node which applies material information 
/// to faces of the children nodes. 
class GSE_MaterialNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>; 
 protected:
  /// \brief The pointer to the material in MaterialMgr.
  GSE_Material *m_pMaterial;
  /// The constructor.
  GSE_MaterialNode() : GSE_RenderNode()
  {
    m_pMaterial = NULL;
  }
 public:

  /// \brief Assigns a material to this node.
  /// \param pMaterial A pointer to GSE_Material. 
  inline void SetMaterial( GSE_Material *pMaterial )
  {
    m_pMaterial = pMaterial;
  }
  /// \brief Returns a pointer to the assigned material.
  /// \return A pointer to the GSE_Material.
  inline GSE_Material* GetMaterial()
  {
    return m_pMaterial;
  }

};
/////////////////////////////////////////////////////////////////
/// \brief A class for material node which applies material information 
/// to front-facing faces of the children.
class GSE_FrontMaterialNode : public GSE_MaterialNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief The constructor.
  GSE_FrontMaterialNode() : GSE_MaterialNode()
  {
    SetType(RenderNode::FRONT_MATERIAL);
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A class for material node which applies material information 
/// to front-facing faces of the children
class GSE_BackMaterialNode : public GSE_MaterialNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief The constructor.
  GSE_BackMaterialNode() : GSE_MaterialNode()
  {
    SetType(RenderNode::BACK_MATERIAL);
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A class for setting a color for rendered primitives.
class GSE_ColorNode : public GSE_RenderNode,
		      public GSE_Color
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief  The constructor
  GSE_ColorNode() : GSE_RenderNode(), GSE_Color()
  {
    Set( 0.0, 0.0, 0.0, 1.0);
    SetType( RenderNode::COLOR );
  }
  
};
/////////////////////////////////////////////////////////////////
/// \brief A node which holds the indices of elements to be drawn from 
/// specified arrays. 
class GSE_IndexBufferNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief The pointer to the IndexBufferMgr's array 
  GSE_IndexBuffer *m_pIndexBuffer;

  /// \brief The constructor.
  GSE_IndexBufferNode()
  {
    SetType(RenderNode::INDEXBUFFER);
    m_pIndexBuffer = NULL;
  }
 public:
  /// \brief Returns the pointer to the GSE_IndexBuffer.
  /// \return The pointer to the GSE_IndexBuffer.
  GSE_IndexBuffer *GetIndexBuffer()
  {
    return m_pIndexBuffer;
  }
  /// \brief Assigns the element list to be used while rendering.
  /// \param pIndexBuffer A pointer to GSE_IndexBuffer.
  void SetIndexBuffer( GSE_IndexBuffer *pIndexBuffer )
  {
    m_pIndexBuffer = pIndexBuffer;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A node which hints renderer about face culling. Default values for 
/// this node are culling enabled with backfaces.
class GSE_CullingStateNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// \brief Is face culling enabled
  char m_bFaceCullingEnabled;
  /// \brief Will the front faces be culled
  char m_bCullFrontFaces;
  /// \brief Will the back faces be culled
  char m_bCullBackFaces;
  /// \brief The constructor

  GSE_CullingStateNode() : GSE_RenderNode() 
  {
    
    m_bFaceCullingEnabled = 1;
    m_bCullFrontFaces = 0;
    m_bCullBackFaces = 1;
    SetType(RenderNode::RENDERSTATE_CULLING);
  }
 public:
  /// \brief Sets the backface culling on/off.
  /// \param bFlag boolean value for enabling/disabling backface culling.
  inline void SetBackfaceCulling( char bFlag )  
  {
    m_bCullBackFaces = bFlag;
  }
  /// \brief Sets the frontface culling on/off.
  /// \param bFlag boolean value for enabling/disabling frontface culling.
  inline void SetFrontfaceCulling( char bFlag ) 
  {
    m_bCullFrontFaces = bFlag;
  }
  /// \brief Returns boolean value are backfaces culled.
  /// \return 1 if culled, 0 if not.
  inline char IsBackfaceCulled() const
  {
    return m_bCullBackFaces;
  }
  /// \brief Returns boolean value are frontfaces culled.
  /// \return 1 if culled, 0 if not.
  inline char IsFrontfaceCulled() const
  {
    return m_bCullFrontFaces;
  }
  /// \brief Sets face culling enabled/disabled.
  /// \param bFlag 1 if enabled, 0 if not.
  inline void SetFaceCulling( char bFlag ) 
  {
    m_bFaceCullingEnabled = bFlag;
  }
  /// \brief Returns boolean value is face culling enabled.
  /// \return 1 if enabled, 0 if not.
  inline char IsFaceCullingEnabled() const 
  {
    return m_bFaceCullingEnabled;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A node which controls the global lighting modes, 
/// ie. global ambient, reflection angle calculations and two-sided lighting
/// By default, global ambient is (0,0,0,1), reflection angles are calculated from
/// eye coordinates and two-sided lighting is disabled.
class GSE_GlobalLightingNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The color for the global ambient
  GSE_Color m_GlobalAmbient;
  /// The way how reflection angles are computed, if non-zero angles 
  /// are calculated from eye coordinates. Otherwise view direction is
  /// assumed to be oriented along negative z axis.
  char m_bLocalViewerModeEnabled;
  /// Whether the two-sided lighting is enabled. Non-zero indicates enabled state.
  char m_bTwoSidedLightingEnabled;
  /// Should specular color be separate
  char m_bSeparateSpecularColor;

  /// The constructor.
  GSE_GlobalLightingNode() : GSE_RenderNode()
  {
    SetType(RenderNode::GLOBAL_LIGHTING);
    m_GlobalAmbient.Set(0.0, 0.0, 0.0, 1.0);
    m_bLocalViewerModeEnabled = 1;
    m_bTwoSidedLightingEnabled = 0;
    m_bSeparateSpecularColor = 1;
  }
 public:
  /// \brief Sets the separate specular color enabled/disabled.
  /// \param bFlag 1 if enabled, 0 if disabled.
  void SetSeparateSpecularColorEnabled( char bFlag )
  {
    m_bSeparateSpecularColor = bFlag;
  }
  /// Is separate specular color enabled.
  /// \return true if so, false otherwise.
  inline const char IsSeparateSpecularColorEnabled() const
  {
    return m_bSeparateSpecularColor;
  }
  /// \brief Sets the two-sided lighting enabled/disabled.
  /// \param bFlag 1 if enabled, 0 if disabled.
  void SetTwoSidedLightingEnabled( char bFlag )
  {
    m_bTwoSidedLightingEnabled = bFlag;
  }
  /// Is two sided lighting enabled.
  /// \return true if so, false otherwise.
  inline const char IsTwoSidedLightingEnabled() const
  {
    return m_bTwoSidedLightingEnabled;
  }
  /// \brief Sets the local viewer mode enabled/disabled.
  /// \param bFlag 1 if enabled, 0 if disabled.
  void SetLocalViewerModeEnabled( char bFlag )
  {
    m_bLocalViewerModeEnabled = bFlag;
  }
  /// Is local viewer mode enabled.
  /// \return true if so, false otherwise.
  inline const char IsLocalViewerModeEnabled() const
  {
    return m_bLocalViewerModeEnabled;
  }
  /// \brief Sets the global ambient color.
  /// \param color The GSE_Color for global ambient.
  inline void SetGlobalAmbient( GSE_Color &color )
  {
    m_GlobalAmbient = color;
  }
  /// Returns global ambient color.
  /// \return a reference to global ambient color.
  inline const GSE_Color & GetGlobalAmbient()
  {
    return m_GlobalAmbient;
  }
};
/////////////////////////////////////////////////////////////////
/// A shader node which can be used to activate specific shader program.
class GSE_ShaderNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The shader program controlled by this node.
  GSE_ShaderProgram *m_pShaderProgram;
  /// Boolean; is shader enabled or not
  char m_bEnabled;


  /// The constructor.
  GSE_ShaderNode() : GSE_RenderNode() 
  {
    SetType(RenderNode::SHADER);
    m_pShaderProgram	= NULL;
    m_bEnabled		= 0;
  }
 public:
  /// \brief Returns the GSE_ShaderProgram controlled by this node.
  /// \return A pointer to GSE_ShaderProgram controlled by this node.
  GSE_ShaderProgram * GetShaderProgram()
  {
    return m_pShaderProgram;
  }
  /// \brief Sets the GSE_ShaderProgram controlled by this node.
  /// \param pShaderProgram A pointer to GSE_ShaderProgram.
  void SetShaderProgram( GSE_ShaderProgram * pShaderProgram )
  {
    m_pShaderProgram = pShaderProgram;
  }
  /// \brief Enables/disables the shader program
  /// \param bValue boolean 1 for enabled, 0 for disabled.
  inline void SetEnabled(char bValue)
  {
    m_bEnabled = bValue;
  }
  /// \brief Returns boolean value if shader is enabled.
  /// \return Boolean 1 if enabled, 0 if disabled.
  inline char IsEnabled()
  {
    return m_bEnabled;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A class for vertex attribute node.
class GSE_VertexAttribNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// A pointer to the vertex attribute structure.
  GSE_VertexAttribStruct * m_pVertexAttrib;

  /// The constructor.
  GSE_VertexAttribNode() : GSE_RenderNode()
  {
    SetType(RenderNode::VERTEXATTRIB);
    m_pVertexAttrib = NULL;
  }
 public:
  /// \brief Returns the GSE_VertexAttribStruct controlled by this node.
  /// \return A pointer to a GSE_VertexAttribStruct controlled by this node.
  GSE_VertexAttribStruct *GetVertexAttrib()
  {
    return m_pVertexAttrib;
  }
  /// \brief Assigns the GSE_VertexAttrbStruct controlled by this node.
  /// \param pVertexAttrib a pointer to GSE_VertexAttribStruct.
  void SetVertexAttrib( GSE_VertexAttribStruct *pVertexAttrib )
  {
    m_pVertexAttrib = pVertexAttrib;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A node for particlesystems. This node holds pointer to the 
/// GSE_ParticleSystem and to the GeometryData which is used in rendering
/// the system's particles.
class GSE_ParticleSystemNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The GSE_GeometryData which contains the vertices for particles.
  GSE_GeometryData *m_pGeometryData;
  /// The ParticleSystem controlled by this node.
  GSE_ParticleSystemBase *m_pParticleSystem;
  /// The constructor.
  GSE_ParticleSystemNode() : GSE_RenderNode()
  {

    SetType(RenderNode::PARTICLESYSTEM);
    m_pGeometryData = NULL;
    m_pParticleSystem = NULL;
    
  }
 public:
  /// \brief Assigns the GSE_GeometryData which holds the vertices for particles.
  /// \param pGeometryData A pointer to GSE_GeometryData.
  void SetGeometryData( GSE_GeometryData *pGeometryData )
  {
    m_pGeometryData = pGeometryData;
  }
  /// \brief Returns the GSE_GeometryData used for particle rendering.
  /// \return A pointer to GSE_GeometryData used for particle rendering.
  GSE_GeometryData * GetGeometryData()
  {
    return m_pGeometryData;
  }
  /// \brief Sets the particlesystem controlled by this node.
  /// \param pParticleSystem A pointer to GSE_ParticleSystem.
  void SetParticleSystem( GSE_ParticleSystemBase *pParticleSystem )
  {
    m_pParticleSystem = pParticleSystem;
  }
  /// \brief Returns the GSE_ParticleSystem controlled by this node.
  /// \return A pointer to GSE_ParticleSystem controlled by this node.
  GSE_ParticleSystemBase *GetParticleSystem()
  {
    return m_pParticleSystem;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A node for rendering a K-DOP.
class GSE_KDopUtilNode : public GSE_RenderNode
			 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// A GSE_KDOP object to be rendered.
  Geometry::GSE_KDOP m_kDOP;

  /// The constructor.
  GSE_KDopUtilNode()
  {
    SetType( RenderNode::KDOP );
  }
 public:
  /// \brief Assigns the GSE_KDOP to be controlled by this node.
  /// \param kDop GSE_KDOP object.
  void SetKDOP( Geometry::GSE_KDOP kDop )
  {
    m_kDOP = kDop;
  }
  /// \brief Returns the GSE_KDOP controlled by this node.
  /// \return GSE_KDOP controlled by this node.
  Geometry::GSE_KDOP & GetKDOP()
  {
    return m_kDOP;
  }
  
};
/////////////////////////////////////////////////////////////////
/// \brief A node for changing polygon mode
class GSE_PolygonModeNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 public:
  /// Possible modes for polygon rendering.
  typedef enum 
  {
    /// Polygons are rendered filled.
    FILL = 0, 
    /// Polygons are rendered with lines.
    LINE = 1,
    /// Only points are rendered.
    POINT = 2
  } PolyMode_t;
 protected:
  /// Mode for front-facing polygons.
  PolyMode_t  m_iFrontFaceMode;
  /// Mode for back-facing polygons.
  PolyMode_t  m_iBackFaceMode;
 protected:
  /// Default constructor.
  GSE_PolygonModeNode() 
  { 
    SetFrontFaceMode(FILL);
    SetBackFaceMode(LINE); 
    SetType( RenderNode::POLYGONMODE);
  }
 public:
  /// Sets rendering mode for front-facing polygons.
  /// \param mode rendering mode.
  inline void SetFrontFaceMode( PolyMode_t mode )
  {
    m_iFrontFaceMode = mode;
  }
  /// Sets rendering mode for back-facing polygons.
  /// \param mode rendering mode.
  inline void SetBackFaceMode( PolyMode_t mode )
  {
    m_iBackFaceMode = mode;
  }
  /// Returns mode for back-facing polygons.
  inline PolyMode_t GetBackFaceMode() const
  {
    return m_iBackFaceMode;
  }
  /// Returns mode for front-facing polygons.
  inline PolyMode_t GetFrontFaceMode() const
  {
    return m_iFrontFaceMode;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A utility class for drawing a simple sphere
class GSE_UtilSphereNode : public GSE_RenderNode,
			   public Geometry::GSE_Sphere
{
  friend class GSE_Graph<RenderNode::NodeType>;
 public:
  /// The drawing modes for UtilSphere. They are only hints and might not work always.
  typedef enum {
    /// Should be drawn as solid.
    FILLED = 0, 
    /// Only a silhouette should be drawn.
    SILHOUETTE, 
    /// Draw only wireframe.
    WIREFRAME   
  } DrawMode_t;
 protected:
  /// Which drawmode should the sphere be drawn with
  DrawMode_t m_iDrawMode;
  /// The color of the sphere
  GSE_Color m_vColor;
  
  /// Default constructor.
  GSE_UtilSphereNode() : GSE_RenderNode(), GSE_Sphere()
  {
    SetType(RenderNode::UTIL_SPHERE);
    m_vColor.Set(1.0, 1.0, 1.0, 1.0);
    SetDrawMode( FILLED );
  }
  /// The constructor.
  /// \param vPosition the center point of the sphere. 
  /// \param fRadius the radius of the sphere.
  GSE_UtilSphereNode(GSE_Vector3 vPosition, float fRadius) : GSE_RenderNode(), GSE_Sphere(vPosition, fRadius)
  {
    SetType(RenderNode::UTIL_SPHERE);
    m_vColor.Set(1.0, 1.0, 1.0, 1.0);
    SetDrawMode( FILLED );
  }
  /// \brief The copy constructor.
  /// \param sphere A reference to GSE_Sphere
  GSE_UtilSphereNode( GSE_Sphere &sphere ) : GSE_RenderNode(), GSE_Sphere(sphere)
  {
    SetType(RenderNode::UTIL_SPHERE);
    m_vColor.Set(1.0, 1.0, 1.0, 1.0);
    SetDrawMode( FILLED );
    
  }
 public:
  /// \brief The assignment operator for sphere. Gets radius and position 
  /// from the it.
  /// \param ref A sphere object where position and radius is copied.
  /// \returns GSE_UtilSphereNode & reference to this object.
  inline GSE_UtilSphereNode & operator=( Geometry::GSE_Sphere & ref )
  {
    SetPosition( ref.GetPosition());
    SetRadius( ref.GetRadius());
    return *this;
  }
  /// \brief Sets the drawmode.
  /// \param iMode (FILLED, SILHOUETTE, WIREFRAME)
  void SetDrawMode( DrawMode_t iMode )
    {
    m_iDrawMode = iMode;
  }
  /// \brief Returns the drawmode.
  /// \return DrawMode_t (FILLED, SILHOUETTE, WIREFRAME)
  DrawMode_t GetDrawMode()
  {
    return m_iDrawMode;
  }
  /// \brief Returns the color of the sphere.
  /// \return GSE_Color the color of the sphere.
  GSE_Color &GetColor(){
    return m_vColor;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A utility class for drawing a simple box
class GSE_UtilBoxNode : public GSE_RenderNode,
			public Geometry::GSE_OrientedBox
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// Should box be rendered in wireframe.
  char m_bWireframe;
  /// The color of the box.
  GSE_Color m_vColor;
  GSE_UtilBoxNode() : GSE_RenderNode(), GSE_OrientedBox()
  {
    SetType(RenderNode::UTIL_BOX);
    m_vColor.Set(1.0, 1.0, 1.0, 1.0);
    m_bWireframe = 1;
  }
  
  /// \brief The constructor.
  /// \param ob A reference to GSE_OrientedBox.
  GSE_UtilBoxNode(GSE_OrientedBox &ob) : GSE_RenderNode(), GSE_OrientedBox(ob)
  {
    SetType(RenderNode::UTIL_BOX);
    m_vColor.Set(1.0, 1.0, 1.0, 1.0);
    m_bWireframe = 1;
  }
 public:
  /// \brief Sets the wireframe mode enabled/disabled.
  /// \param bFlag boolean 1 for enabled, 0 for disabled.
  void SetWireframe( char bFlag )
  {
    m_bWireframe = bFlag;
  }
  /// \brief Returns boolean value is wireframe mode enabled/disabled.
  /// \return boolean 1 for enabled, 0 for disabled.
  char IsWireframe()
  {
    return m_bWireframe;
  }
  /// \brief Returns the box color
  /// \return A reference to the box color.
  GSE_Color &GetColor(){
    return m_vColor;
  }
};
/////////////////////////////////////////////////////////////////
/// Class for drawing a simple cone
class GSE_UtilConeNode : public GSE_RenderNode,
			 public Geometry::GSE_Cone
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// Should cone be drawn as wireframe.
  char m_bWireframe;
  /// Drawing color.
  GSE_Color m_vColor;
  /// Parametrized constructor.
  /// \param cone a cone which is you want to render.
  GSE_UtilConeNode(GSE_Cone &cone) : GSE_RenderNode(), GSE_Cone(cone)
  {
    SetType(RenderNode::UTIL_CONE);
    m_vColor.Set(1.0, 1.0, 1.0, 1.0);
    m_bWireframe = 1;
  }
 public:
  /// Assigns wireframe drawing flag.
  /// \param bFlag True/False, Draw wireframe/don't.
  void SetWireframe( char bFlag )
  {
    m_bWireframe = bFlag;
  }
  /// Returns boolean should this be drawn as wireframe.
  /// \return 1 if yes, 0 if not.
  char IsWireframe()
  {
    return m_bWireframe;
  }
  /// Provides access to drawing color.
  /// \return a reference to drawing color.
  GSE_Color &GetColor(){
    return m_vColor;
  }
};
////////////////////
/// An utility quad for Hud Elements etc.
class GSE_UtilQuadNode : public GSE_RenderNode,
			 public Geometry::GSE_Quad
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// Texture coordinate array.
  float m_arrTexCoords[8];
  /// Default constructor.
  GSE_UtilQuadNode()
  {
    // Set default texture coordinates
    m_arrTexCoords[0] = 0.0f;
    m_arrTexCoords[1] = 0.0f;
    m_arrTexCoords[2] = 1.0f;
    m_arrTexCoords[3] = 0.0f;
    m_arrTexCoords[4] = 1.0f;
    m_arrTexCoords[5] = 1.0f;
    m_arrTexCoords[6] = 0.0f;
    m_arrTexCoords[7] = 1.0f;
    SetType( RenderNode::UTIL_QUAD );
  }
 public:
  /// Assigns texture coordinates for corners.
  /// \param nCoord which corner.
  /// \param fS Texture S coordinate.
  /// \param fT Texture T coordinate.
  inline void SetTexCoords( unsigned int nCoord, float fS, float fT )
  {
    int iIndex = nCoord*2;
    m_arrTexCoords[iIndex] = fS;
    m_arrTexCoords[iIndex+1] = fT;
  }
  /// Returns desired texture coordinate. 
  /// \param nCoord coordinate index.
  /// \todo this should be probably reworked some day, since current way is hardly intuitive.
  inline float GetTexCoord(unsigned int nCoord) const
  {
    return m_arrTexCoords[nCoord];
  }
};
/////////////////////////////////////////////////////////////////
/// The utility line strip node for debugging portals etc.
class GSE_UtilLineStripNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// vertex list.
  std::list<GSE_Vector3> m_lstVertices;
  /// Default constructor.
  GSE_UtilLineStripNode()
  {
    SetType( RenderNode::UTIL_LINESTRIP);
  }
 public:
  /// Returns a reference to vertex list.
  std::list<GSE_Vector3> & Vertices()
  {
    return m_lstVertices;
  }
};			      
/////////////////////////////////////////////////////////////////
/// The cache node for OpenGL display lists. 
/// \todo Either remove or redesign, this does not work very well with VBOs.
class GSE_OglCacheNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The actual managed cache object.
  GSE_OglCache *m_pOglCache;


  /// Default constructor.
  GSE_OglCacheNode() : GSE_RenderNode()
  {
    m_pOglCache = NULL;
    SetType(RenderNode::CACHE );
  }
  /// Destructor.
  ~GSE_OglCacheNode()
  {
    // Yes, this is correct. The OglCacheMgr takes care of deleting it.
    m_pOglCache = NULL;
  }
 public:
  /// Assigns the cache object for this node.
  /// \param pCache pointer to cache object.
  void SetCache( GSE_OglCache *pCache )
  {
    m_pOglCache = pCache;
  }
  /// Returns the pointer to the cache object.
  /// \return Cache pointer.
  GSE_OglCache *GetCache()
  {
    return m_pOglCache;
  }
};
/////////////////////////////////////////////////////////////////
/// The class for node which can be used to put in front of a render nodes
/// to name them for selection.
/// \todo OBSOLETE. A _far_ better way to do things is to use methods in class GSE_Camera.
class GSE_SelectionNameNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// Selection nam.
  GLuint m_nName;
  /// Parametrized constructor.
  /// \param nName OpenGL compliant name for selection stuff.
  GSE_SelectionNameNode( GLuint nName) : GSE_RenderNode()
  {
    m_nName = nName;
    SetType(RenderNode::SELECTION);
  }
 public:
  /// Returns OpenGL compliant name for selection stuff.
  GLuint GetSelectionName()
  {
    return m_nName;
  }
};
/////////////////////////////////////////////////////////////////
/// Renders text.
class GSE_TextNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// The text to be rendered.
  char *m_pString;         
  /// The index of the Renderer fontset to be used in rendering process, by default 0.
  unsigned int m_nFontset; 

  /// Default constructor
  GSE_TextNode() : GSE_RenderNode()
  {
    m_pString = NULL;
    m_nFontset = 0; 
    SetType(RenderNode::TEXT);
  }
 public:
  /// Copy constructor.
  GSE_TextNode( const GSE_TextNode &node )
  {
    SetString( node.m_pString );
    m_nFontset = node.m_nFontset;
    SetType(RenderNode::TEXT);
  }
  /// Assignment operator.
  /// \param node text node to be assigned.
  /// \return a reference to this.
  GSE_TextNode & operator=( GSE_TextNode node )
  {
    SetString( node.m_pString );
    m_nFontset = node.m_nFontset;
    SetType(RenderNode::TEXT);
    return *this;
  }
  /// Returns the text string.
  const char *GetString()
  {
    return m_pString;
  }
  /// Assigns rendered text.
  /// \param pString string to be rendered.
  void SetString( char *pString )
  {
    // Free previous string, if it exists 
    if ( m_pString != NULL ) delete m_pString;
    
    if ( pString != NULL )
      m_pString = strdup( pString );
    else
      m_pString = NULL;
  }
  /// Assigns the index of the preferred fontset.
  /// \param nFontsetIndex index of fontset in GSE_OglRenderer.
  void SetFontset( unsigned int nFontsetIndex )
  {
    m_nFontset = nFontsetIndex;
  }
  /// Returns the preferred fontset for this text.
  /// \return fontset index in GSE_OglRenderer.
  unsigned int GetFontset()
  {
    return m_nFontset;
  }
};
/////////////////////////////////////////////////////////////////
/// Translation node.
class GSE_TranslationNode : public GSE_RenderNode, public GSE_Vector3
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// Default constructor.
  GSE_TranslationNode() : GSE_Vector3(0.0f,0.0f,0.0f)
  {
    SetType( RenderNode::TRANSLATION );
  }
 public:  
  /// Sets translation parameters.
  /// \param fX X-axis translation value.
  /// \param fY Y-axis translation value.
  /// \param fZ Zaxis translation value.
  void  SetTranslation( float fX, float fY, float fZ )
  {
    m_pValues[0] = fX;
    m_pValues[1] = fY;
    m_pValues[2] = fZ;
  }
  /// Sets translation parameters.
  /// \param vTmp Vector that contains axis (X,Y,Z) translation values.
  void  SetTranslation( const GSE_Vector3 & vTmp )
  {
    m_pValues[0] = vTmp.m_pValues[GSE_Vector3::X];
    m_pValues[1] = vTmp.m_pValues[GSE_Vector3::Y];
    m_pValues[2] = vTmp.m_pValues[GSE_Vector3::Z];
  }
};
/////////////////////////////////////////////////////////////////
/// Rotation node.
class GSE_RotationNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// Rotation quaternion.
  GSE_Quaternion qQuat;
  /// Default constructor.
  GSE_RotationNode() 
  {
    SetType(RenderNode::ROTATION);
  }
 public:
  /// Accesses rotation quaternion.
  /// \return A reference to rotation quaternion.
  GSE_Quaternion & Quaternion()
  {
    return qQuat;
  }
  /// Converts rotation in this node to 4x4 Matrix.
  /// \return 4x4 rotation matrix.
  GSE_Matrix4x4f ToMatrix()
  {
    return qQuat.ToMatrix();
  }
};
/////////////////////////////////////////////////////////////////
/// Alpha test node.
class GSE_AlphaTestNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 public:

 protected:
  /// Is Alpha test enabled.
  int m_bEnabled;
  /// The alpha comparison function.
  AlphaTest::Type m_AlphaFunc;
  /// The alpha reference value which incoming value is compared to.
  float m_fAlphaReference;
  ////////////////////
  /// Constructor.
  GSE_AlphaTestNode() : m_bEnabled(0)
  {
    m_AlphaFunc = AlphaTest::ALWAYS;
    SetType(RenderNode::ALPHA_TEST);
    SetReference(0.0f);
    
  }
  ////////////////////
  /// Destructor.
  ~GSE_AlphaTestNode()
  {
    
  }  
 public:
  ////////////////////
  /// Sets alpha test enabled or disabled.
  /// \param bFlag boolean true for enabled, false for disabled.
  inline void SetEnabled( int bFlag )
  {
    m_bEnabled = bFlag;
  }
  ////////////////////
  /// Is alpha test enabled.
  /// \return 1 if enabled, 0 if not.
  inline int IsEnabled() const
  {
    return m_bEnabled;
  }
  ////////////////////
  /// Sets alpha test function.
  /// \param type Alpha test function type.
  inline void SetFunc( AlphaTest::Type type )
  {
    m_AlphaFunc = type;
  }
  ////////////////////
  /// Gets Alpha test function type.
  /// \returns function type.
  inline AlphaTest::Type GetFunc() const
  {
    return m_AlphaFunc;
  }
  ////////////////////
  /// Sets reference value used in comparison.
  /// \param fValue new reference value.
  inline void SetReference( float fValue )
  {
    m_fAlphaReference = fValue;
  }
  ////////////////////
  /// Gets comparision reference value.
  /// This value is used with comparison function, ie. 
  /// LESS_THAN [referencevalue]
  /// \return reference value.
  inline float GetReference() const
  {
    return m_fAlphaReference;
  }
};
/////////////////////////////////////////////////////////////////
/// Blending state node.
/// \todo remove OpenGL-specific things and replace them with more general solution.
class GSE_BlendingStateNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:  
  /// Is blending enabled.
  char m_bEnabled;
  /// Source operation.
  GLenum m_nSrcOp;
  /// destination operation.
  GLenum m_nDestOp;
  /// Default constructor.
  GSE_BlendingStateNode()
  {
    SetType(RenderNode::RENDERSTATE_BLEND);
    m_bEnabled = 0;
  }
 public:
  /// Sets blending enabled or disabled.
  /// \param bFlag true for enabling, false for disabling.
  void SetEnabled( char bFlag )
  {
    m_bEnabled = bFlag;
  }
  /// Is blending enabled.
  /// \return true if enabled, false otherwise.
  char IsEnabled()
  {
    return m_bEnabled;
  }
  /// Assigns destination operation.
  /// \param nOp operation.
  void SetDestOp( GLenum nOp )
  {
    m_nDestOp = nOp;
  }
  /// Assigns source operation.
  /// \param nOp operation.
  void SetSrcOp( GLenum nOp )
  {
    m_nSrcOp = nOp;
  }
  /// Returns current source operation.
  /// \return source operation.
  GLenum GetSrcOp()
  {
    return m_nSrcOp;
  
  }
  /// Returns current destination operation.
  /// \return destination operation.
  GLenum GetDestOp()
  {
    return m_nDestOp;
  }
};
/////////////////////////////////////////////////////////////////
class GSE_DepthBufferStateNode : public GSE_RenderNode 
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  int m_bDepthTest;
  int m_bDepthBufferWriting;

  GSE_DepthBufferStateNode()
  {
    SetDepthTest( 0 );    
    SetDepthBufferWriting( 1 );
    SetType(RenderNode::RENDERSTATE_DEPTHBUFFER);
  }
 public:
  void SetDepthBufferWriting( int bFlag )
  {
    m_bDepthBufferWriting = bFlag;
  }
  int IsDepthBufferWriting()
  {
    return m_bDepthBufferWriting;
  }
  void SetDepthTest( int bFlag )
  {
    m_bDepthTest = bFlag;
  }
  int IsDepthTest()
  {
    return m_bDepthTest;
  }
};
/////////////////////////////////////////////////////////////////
/// The class which stores the defined state values into an opengl stack.
/// \todo write better comment for LIST.
class GSE_RenderStateStackNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 public:
  /// which states will be stored.
  enum {
    /// Store accumulation buffer settings.
    ACCUM_BUFFER  = 1, 
    /// Store color buffer settings.
    COLOR_BUFFER  = 2,
    /// Store stuff which is marked OpenGL CURRENT_BIT settings.
    CURRENT,
    /// Store depth buffer settings.
    DEPTH_BUFFER,
    /// Store enable settings.
    ENABLE,
    /// Store parametric curve/surface evaluation settings.
    EVAL,
    /// Store fog settings.
    FOG,
    /// Store hinting settings.
    HINT,
    /// Store lighting settings.
    LIGHTING,
    /// Store line width settings.
    LINE,
    /// Store some weird list settings
    LIST,
    /// Store pixel mode settings.
    PIXEL_MODE,
    /// Store point size settings.
    POINT,
    /// Store polygon mode settings.
    POLYGON,
    /// Store polygon stipple settings.
    POLYGON_STIPPLE,
    /// Store scissor test settings.
    SCISSOR,
    /// Store stencil buffer test settings.
    STENCIL_BUFFER,
    /// Store texture settings.
    TEXTURE,
    /// Store transformation settings.
    TRANSFORM,
    /// Store viewport settings-
    VIEWPORT,
    /// The number of state storing options.
    NUM_STACK_OPTIONS
  };
 protected:
  /// Storage settings.
  unsigned int m_nSavedBits;
  /// Default constructor.
  GSE_RenderStateStackNode()
  {
    m_nSavedBits = 0;
    SetType( RenderNode::RENDERSTATE_STACK );
  }
 public:
  /// Sets a setting to be stored.
  /// \param nBit a setting.
  void Store( unsigned int nBit )
  {
    m_nSavedBits |= (1 << nBit);
  }
  /// Returns true if given setting is stored.
  /// \param nBit a setting.
  unsigned int IsStored(unsigned int nBit )
  {
    return (m_nSavedBits & (1 << nBit));
  }
};
/////////////////////////////////////////////////////////////////
class GSE_TextureNode : public GSE_RenderNode,
			public GSE_ValueList<TextureFilter::Type>
{
  friend class GSE_Graph<RenderNode::NodeType>;
 protected:
  /// Texture color env value.
  GSE_Color    m_colorTexEnv;
  /// Pointer to a texture.
  GSE_Texture *m_pTexture;
  /// Texture Id.
  TextureId::Type m_nTexId;
  /// Default constructor.
  GSE_TextureNode()
  {
    SetType( RenderNode::TEXTURE );
    m_colorTexEnv.Set(0, 0, 0, 0);
    SetTexId( TextureId::TEX0 );
  }
 public:
  /// Accesses texture env color.
  /// \return A reference to texture env color.
  const GSE_Color & GetTexEnvColor() const
  {
    return m_colorTexEnv;
  }
  /// Assings texture env color.
  /// \param color desired color.
  void SetTexEnvColor( GSE_Color & color )
  {
    m_colorTexEnv = color;
  }
  /// Assigns texture.
  /// \param pTexture a pointer to texture which is assigned.
  void SetTexture( GSE_Texture *pTexture )
  {
    m_pTexture = pTexture;
  }
  /// Returns assigned texture.
  /// \return a pointer to the texture.
  GSE_Texture *GetTexture()
  {
    return m_pTexture;
  }
  /// Assigns texture id.
  /// \param nTexId texture id.
  void SetTexId( TextureId::Type nTexId )
  {
    m_nTexId = nTexId;
  }
  /// Returns texture id.
  /// \return texture id.
  TextureId::Type GetTexId()
  {
    return m_nTexId;
  }
};
/////////////////////////////////////////////////////////////////
class GSE_ShadeModelNode : public GSE_RenderNode
{
  friend class GSE_Graph<RenderNode::NodeType>;
 public:
  typedef enum 
  {
    SMOOTH = 0,
    FLAT
  } GSE_ShadeModel_t;

 protected:

  GSE_ShadeModel_t m_nShadeModel;

  GSE_ShadeModelNode()
  {
    SetType(RenderNode::SHADEMODEL);
    SetShadeModel( FLAT );
  }

 public:
  void SetShadeModel( GSE_ShadeModel_t nType )
  {
    m_nShadeModel = nType;
  }
  GSE_ShadeModel_t GetShadeModel()
  {
    return m_nShadeModel;
  }
};
/////////////////////////////////////////////////////////////////
// End Render Graph 
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Run-time type identification values for animation graph nodes
/////////////////////////////////////////////////////////////////
namespace Animation 
{
  typedef enum 
  { 
    UNDEFINED = 0,
    JOINT = 1,
    PARTICLESYSTEM = 2,
    ROOT
  } NodeType;
} // end namespace Animation
/////////////////////////////////////////////////////////////////
typedef GSE_Graph<Animation::NodeType> GSE_AnimationGraph;
/////////////////////////////////////////////////////////////////
//typedef GSE_GraphNode<Animation::NodeType> GSE_AnimationNode;
class GSE_AnimationNode : public GSE_GraphNode<Animation::NodeType>
{
  friend class GSE_Graph<Animation::NodeType>;
 protected:
  
  GSE_AnimationNode() : GSE_GraphNode<Animation::NodeType>()
  {
    SetType(Animation::UNDEFINED);
  }
  ~GSE_AnimationNode()
  {
    
  }
};
/////////////////////////////////////////////////////////////////
/// The root node for animation graph.
class GSE_ARootNode : public GSE_AnimationNode
{
  friend class GSE_Graph<Animation::NodeType>;
 protected:
  GSE_ARootNode() : GSE_AnimationNode()
  {
    SetType( Animation::ROOT );
  }
};
/////////////////////////////////////////////////////////////////
using Math::GSE_Matrix4x4f;
/////////////////////////////////////////////////////////////////
class GSE_Joint;
/////////////////////////////////////////////////////////////////
class GSE_JointNode : public GSE_AnimationNode
{
  friend class GSE_Graph<Animation::NodeType>;
 protected:
  
  GSE_Joint *m_pJoint;
  // Which vertices in GeometryDatas are connected to this joint.
  GSE_IndexBuffer *m_pIndexBuffer;
  // Pointer to objectstruct which holds the values to original vertex positions
  GSE_GeometryData *m_pGeometryDataOrig;
  // Pointer to objectstruct which olds the values to the interpolated vertex positions
  GSE_GeometryData *m_pGeometryDataWorld;  
  
  GSE_JointNode() : GSE_AnimationNode()
  {
    m_pJoint = NULL;
    SetType(Animation::JOINT);
    
    m_pGeometryDataOrig = NULL;
    m_pGeometryDataWorld = NULL;
    m_pIndexBuffer = NULL;
  }
 public:
  void SetJoint( GSE_Joint *pJoint )
  {
    m_pJoint = pJoint;
  }
  GSE_Joint *GetJoint()
  {
    return m_pJoint;
  }
  void SetElements( GSE_IndexBuffer *pIndexBuffer )
  {
    m_pIndexBuffer = pIndexBuffer;
  }

  GSE_IndexBuffer *GetElements()
  {
    return m_pIndexBuffer;
  }
  
  GSE_GeometryData *GetGeometryDataOrig()
  {
    return m_pGeometryDataOrig;
  }

  GSE_GeometryData *GetGeometryDataWorld()
  {
    return m_pGeometryDataWorld;
  }

  void SetGeometryDataOrig( GSE_GeometryData *pGeometryData )
  {
    m_pGeometryDataOrig = pGeometryData;
  }

  void SetGeometryDataWorld( GSE_GeometryData *pGeometryData )
  {
    m_pGeometryDataWorld = pGeometryData;
  }  

  unsigned int GetVertexCount()
  {
    if ( m_pIndexBuffer != NULL ){
      return m_pIndexBuffer->m_nNumIndices;
    } else {
      return 0;
    }
  }
};
/////////////////////////////////////////////////////////////////
/// Node for particlesystem animation.
class GSE_AnimParticleSystemNode : public GSE_AnimationNode
{
  friend class GSE_Graph<Animation::NodeType>;
 protected:
  /// particle system.
  GSE_ParticleSystemBase *m_pParticleSystem;
  /// indexbuffer.
  GSE_IndexBuffer *	m_pIndexBuffer;
  /// Default constructor.
  GSE_AnimParticleSystemNode() : GSE_AnimationNode()
  {
    SetType(Animation::PARTICLESYSTEM);
    m_pParticleSystem = NULL;
    m_pIndexBuffer = NULL;
  }
 public:
  /// Assigns a particlesystem.
  /// \param pParticleSystem to be assigned.
  void SetParticleSystem( GSE_ParticleSystemBase *pParticleSystem )
  {
    m_pParticleSystem = pParticleSystem;
  }
  /// Gets currently assigned particlesystem.
  /// \return a pointer to particlesystem.
  GSE_ParticleSystemBase *GetParticleSystem()
  {
    return m_pParticleSystem;
  }
  /// Sets indexbuffer.
  /// \param pIndexBuffer a pointer to indexbuffer to be set.
  void SetElements( GSE_IndexBuffer *pIndexBuffer )
  {
    m_pIndexBuffer = pIndexBuffer;
  }
  /// Gets currently assigned indexbuffer.
  /// \returns a pointer to indexbuffer.
  GSE_IndexBuffer *GetElements()
  {
    return m_pIndexBuffer;
  }
  
};
/////////////////////////////////////////////////////////////////
/// Class for visibility stuff.
class GSE_Visible 
{
 private:
  /// visibility state
  int m_bVisible;
 protected:
  /// Default constructor.
  GSE_Visible() : m_bVisible(0) {}
 public:
  /// Sets visibility.
  /// \param bFlag boolean true for visible, false for invisible.
  inline void SetVisible( int bFlag ) { m_bVisible = bFlag; }
  /// Gets current visibility.
  // \return true for visible, false for invisible.
  inline int IsVisible() const { return m_bVisible; }
};
/////////////////////////////////////////////////////////////////
// Run-time type identification values for scene graph nodes
/////////////////////////////////////////////////////////////////
namespace Scene {
  typedef enum 
  { 
    UNDEFINED = 0,
    OBJECT = 1,
    LIMB,
    LIGHT,
    CAMERA,
    RENDERSETTINGS,
    OCTREEOBJECT,
    PARTICLESYSTEM,
    ROOT
  } NodeType;
} // end namespace Scene
/////////////////////////////////////////////////////////////////
class GSE_SceneRenderer;
class GSE_SceneGraph;
/////////////////////////////////////////////////////////////////
class GSE_SceneNode : public GSE_GraphNode<Scene::NodeType>,
		      public GSE_Visible
{
  /////////////////////////////////////////////////////////////////
  friend class GSE_SceneGraph;
  friend class GSE_SceneRenderer;
 protected:
  /////////////////////////////////////////////////////////////////
  /// Hierarchial bounding volume for a sphere ( this.volume + volumes of all children).
  Geometry::GSE_Sphere	       m_HierarchialBoundingSphere;
  /////////////////////////////////////////////////////////////////
  /// Hierarchial bounding volume for a box ( this.volume + volumes of all children).
  Geometry::GSE_OrientedBox    m_HierarchialBoundingBox;
  /////////////////////////////////////////////////////////////////
  /// Flag indicating whether this scenenode has been rendered already.
  int				m_bRendered;
  /////////////////////////////////////////////////////////////////
  /// The world transform (actual transform in world)
  Math::GSE_Matrix4x4f		m_mWorldTransform;
  /////////////////////////////////////////////////////////////////
  /// The local transform (in respect to parent node).
  GSE_TransformNode *           m_pLocalTransform;
  /////////////////////////////////////////////////////////////////
  // The constructor
  GSE_SceneNode() : GSE_GraphNode<Scene::NodeType>(), 
                    m_mWorldTransform(GSE_Matrix4x4f::Identity())
  {
    SetType(Scene::UNDEFINED);
    SetRendered(0);
    // This will be created by the SceneGraph entity.
    m_pLocalTransform = NULL;
    
  }
  /////////////////////////////////////////////////////////////////
  // The copy constructor
  GSE_SceneNode( const GSE_SceneNode &Node)
  {
    m_HierarchialBoundingSphere = Node.m_HierarchialBoundingSphere;
    m_HierarchialBoundingBox    = Node.m_HierarchialBoundingBox;
    m_bRendered			= Node.m_bRendered;
    m_mWorldTransform		= Node.m_mWorldTransform;
    m_pLocalTransform		= Node.m_pLocalTransform;
    SetType( Node.m_iType );
  }
  /////////////////////////////////////////////////////////////////
  // The destructor.
  ~GSE_SceneNode()
  {
    
  }  
 public:  
  /////////////////////////////////////////////////////////////////
  // The assignment operator.
  GSE_SceneNode & operator=(GSE_SceneNode Node)
  {
    m_HierarchialBoundingSphere = Node.m_HierarchialBoundingSphere;
    m_HierarchialBoundingBox    = Node.m_HierarchialBoundingBox;
    m_mWorldTransform		= Node.m_mWorldTransform;
    m_pLocalTransform		= Node.m_pLocalTransform;
    SetType( Node.m_iType );
    return *this;
  }
  /////////////////////////////////////////////////////////////////
  // Sets value of the m_bRendered flag
  void SetRendered( char bFlag )
  {
    m_bRendered = bFlag;
  }
  /////////////////////////////////////////////////////////////////
  // Returns the value of the m_bRendered flag
  char IsRendered()
  {
    return m_bRendered;
  }
  /////////////////////////////////////////////////////////////////
  inline void AddTransformHook( GSE_Matrix4x4f *pMatrix )
  {
    m_pLocalTransform->SetHook(pMatrix);
  }
  /////////////////////////////////////////////////////////////////
  inline void SetLocalTransform( const GSE_Matrix4x4f & matrix )
  {
    m_pLocalTransform->SetMatrix(matrix);
  }
  /////////////////////////////////////////////////////////////////
  inline const GSE_Matrix4x4f & GetLocalTransform()
  {
    return m_pLocalTransform->GetMatrix();
  }
  /////////////////////////////////////////////////////////////////
  inline void SetWorldTransform( const GSE_Matrix4x4f & matrix)
  {
    m_mWorldTransform=matrix;
  }
  /////////////////////////////////////////////////////////////////
  inline const GSE_Matrix4x4f & GetWorldTransform()
  {
    return m_mWorldTransform;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns the hierarchial bounding sphere.
  /// \param sphere A sphere to be used as a bounding volume.
  inline void SetHierarchialBoundingSphere( const GSE_Sphere &sphere )
  {
    m_HierarchialBoundingSphere = sphere;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns the hierarchial bounding sphere.
  /// \returns GSE_Sphere representing the bounding volume.
  inline const GSE_Sphere & GetHierarchialBoundingSphere() const 
  {
    return m_HierarchialBoundingSphere;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns the hierarchial bounding box.
  /// \returns GSE_OrientedBox representing the bounding volume.
  inline const GSE_OrientedBox & GetHierarchialBoundingBox() const 
  {
    return m_HierarchialBoundingBox;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns the hierarchial bounding box.
  /// \param box An oriented box to be used as a bounding volume.
  inline void SetHierarchialBoundingBox( const GSE_OrientedBox &box )
  {
    m_HierarchialBoundingBox = box;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns the hierarchial bounding box.
  /// \param box An axis-aligned box to be used as a bounding volume.
  inline void SetHierarchialBoundingBox( const GSE_AxisAlignedBox &box )
  {
    m_HierarchialBoundingBox = box;
    /*m_HierarchialBoundingBox.SetOrientation( GSE_Vector3::GetWorldY(),
					     GSE_Vector3::GetWorldZ(),
					     GSE_Vector3::GetWorldX());
					     m_HierarchialBoundingBox = */
  }
  /////////////////////////////////////////////////////////////////
  inline GSE_SceneGraph * GetGraph()
  {
    return reinterpret_cast<GSE_SceneGraph *>(m_pGraph);
  }
};
/////////////////////////////////////////////////////////////////
class GSE_SceneCameraNode : public GSE_SceneNode 
{
  friend class GSE_SceneGraph;
  friend class GSE_SceneRenderer;
 protected:
  GSE_CameraNode *m_pCameraNode;
  GSE_SceneCameraNode() : GSE_SceneNode()
  { 
    // This will be set by SceneGraph CreateNode()
    m_pCameraNode = NULL;
    SetType( Scene::CAMERA );
  }
 public:
  void SetCamera( GSE_Camera *pCamera )
  {
    m_pCameraNode->SetCamera(pCamera);
  }
  GSE_Camera * GetCamera()
  {
    return m_pCameraNode->GetCamera();
  }
  GSE_CameraNode * GetRenderCameraNode()
  {
    return m_pCameraNode;
  }
};
/////////////////////////////////////////////////////////////////
/// A class for illuminatable objects.
class GSE_Illuminated 
{
 protected:
  /////////////////////////////////////////////////////////////////
   GSE_Illuminated()
   { 
     m_pIlluminationSet = NULL;
     m_pLightCluster = NULL;
   }
  ~GSE_Illuminated(){}
  /////////////////////////////////////////////////////////////////
  /// A pointer to the illuminationset which is used.
  GSE_IlluminationSet *  m_pIlluminationSet;
  /// A pointer to the LightCluster to be used. This is set to
  /// contain the #GSE_MAX_ACTIVE_LIGHTS lights that are most 
  /// contributing to the object.
  GSE_LightClusterNode * m_pLightCluster;  

 public:
  /////////////////////////////////////////////////////////////////
  /// Assigns a lightclusternode.
  /// \param pNode a lightclusternode.
  inline void SetLightClusterNode( GSE_LightClusterNode *pNode )
  {
    m_pLightCluster = pNode;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns current lightclusternode.
  /// \return a pointer to lightclusternode.
  inline GSE_LightClusterNode * GetLightClusterNode()
  {
    return m_pLightCluster;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns an illumination set.
  /// \param pSet an illumination set.
  inline void SetIlluminationSet( GSE_IlluminationSet *pSet )
  {
    m_pIlluminationSet = pSet;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns current illumination set.
  /// \return a pointer to an illumination set.
  inline GSE_IlluminationSet * GetIlluminationSet()
  {
    return m_pIlluminationSet;
  }
};
/////////////////////////////////////////////////////////////////
/// A class for nodes with local bounding volume.
class GSE_LocalBoundingVolume
{
 protected:
  /////////////////////////////////////////////////////////////////
  /// The bounding volume for object data in local coordinates.
  Geometry::GSE_Sphere		m_LocalBoundingSphere;
  /////////////////////////////////////////////////////////////////
  /// The bounding volume for object data in local coordinates.
  Geometry::GSE_OrientedBox	m_LocalBoundingBox;
  /////////////////////////////////////////////////////////////////
  /// The bounding volume for object data in world coordinates.
  Geometry::GSE_Sphere		m_WorldBoundingSphere;
  /////////////////////////////////////////////////////////////////
  /// The bounding volume for object data in world coordinates.
  Geometry::GSE_OrientedBox     m_WorldBoundingBox;
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  GSE_LocalBoundingVolume() {}
 public:
  /////////////////////////////////////////////////////////////////
  /// Assigns the local bounding sphere.
  /// \param sphere The sphere to be used as a bounding volume.
  inline void SetLocalBoundingSphere( const GSE_Sphere &sphere )
  {
    m_LocalBoundingSphere = sphere;
  }
  /////////////////////////////////////////////////////////////////
  /// Accesses the local bounding sphere.
  /// \return a reference to local bounding sphere.
  inline GSE_Sphere & GetLocalBoundingSphere()
  {
    return m_LocalBoundingSphere;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns the local bounding box.
  /// \param box The box which is to be used as a bounding volume.
  inline void SetLocalBoundingBox( const GSE_OrientedBox &box)
  {
    m_LocalBoundingBox = box;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns the local bounding box.
  /// \returns GSE_OrientedBox the bounding volume.
  inline GSE_OrientedBox & GetLocalBoundingBox()
  {
    return m_LocalBoundingBox;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns the world bounding sphere.
  /// \param sphere A bouding sphere.
  inline void SetWorldBoundingSphere( const GSE_Sphere &sphere )
  {
    m_WorldBoundingSphere = sphere;
  }
  /////////////////////////////////////////////////////////////////
  /// Accesses the world bounding sphere.
  /// \return World bounding sphere.
  inline GSE_Sphere & GetWorldBoundingSphere()
  {
    return m_WorldBoundingSphere;
  }
  /////////////////////////////////////////////////////////////////
  /// Assings world bounding box.
  /// \param box an oriented box.
  inline void SetWorldBoundingBox( const GSE_OrientedBox &box)
  {
    m_WorldBoundingBox = box;
  }
  /////////////////////////////////////////////////////////////////
  /// Accesses world bounding box.
  /// \return world bounding box.
  inline GSE_OrientedBox & GetWorldBoundingBox()
  {
    return m_WorldBoundingBox;
  }
};
/////////////////////////////////////////////////////////////////
class GSE_SceneRenderSettingsChainNode : public GSE_SceneNode 
{
  friend class GSE_SceneGraph;
 protected:
  /// Start of the chain.
  GSE_RenderNode *m_pBegin;
  /// End of the chain.
  GSE_RenderNode *m_pEnd;
  /// Default constructor.
  GSE_SceneRenderSettingsChainNode()
  {
    SetType(Scene::RENDERSETTINGS);
    m_pEnd = m_pBegin = NULL;
  }

 public:
  /// Inserts new node to chain.
  /// \param pNode a pointer to a node to be added.
  inline void AddLink( GSE_RenderNode *pNode )
  {
    if ( pNode != NULL )
    {
      if ( m_pEnd == NULL )
      {
	m_pBegin = pNode;
	m_pEnd  = pNode;
      } 
      else
      {
	m_pEnd->AddEdge( pNode );
	m_pEnd = pNode;
      }
    }
  }
  /// Returns a pointer to start of chain.
  /// \return a pointer to a rendernode.
  inline GSE_RenderNode * GetBegin()
  {
    return m_pBegin;
  }
  /// Returns a pointer to end of chain.
  /// \return a pointer to a rendernode.
  inline GSE_RenderNode * GetEnd()
  {
    return m_pEnd;
  }
};
/////////////////////////////////////////////////////////////////
// The root node for scene graph
/////////////////////////////////////////////////////////////////
class GSE_SceneRootNode : public GSE_SceneNode
{
  friend class GSE_SceneGraph;
 protected:
  /// Default constructor.
  GSE_SceneRootNode() : GSE_SceneNode()
  {
    SetType(Scene::ROOT);
  }
};
/////////////////////////////////////////////////////////////////
// The object node class for scene graph
/////////////////////////////////////////////////////////////////
class GSE_SceneObjectNode : public GSE_SceneNode,
			    public GSE_Illuminated,
			    public GSE_LocalBoundingVolume
{
  /////////////////////////////////////////////////////////////////
  friend class GSE_SceneGraph;
 protected:
  /////////////////////////////////////////////////////////////////
  /// The object data which is attached to Render Graph eventually
  GSE_RenderNode *		m_pRenderNode;
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  GSE_SceneObjectNode() : GSE_SceneNode()
  {
    m_pRenderNode = NULL;
    SetType(Scene::OBJECT);
  }
  /////////////////////////////////////////////////////////////////
  /// The destructor.
  ~GSE_SceneObjectNode()
  {
    if ( m_pRenderNode != NULL )
    {
      // RenderNodes belong to specific graph, which will take care of 
      // their deletion.
      // GSE_RenderNode::DeleteSubtree( m_pRenderNode );
    }
  }
 public:

  /////////////////////////////////////////////////////////////////
  /// The copy constructor.
  /// \param Node A reference to the GSE_SceneObjectNode.
  GSE_SceneObjectNode( const GSE_SceneObjectNode &Node) 
  {
    m_pRenderNode = Node.m_pRenderNode;
    SetType(Scene::OBJECT);
  }
  /////////////////////////////////////////////////////////////////
  /// The assignment operator.
  /// \param Node Another SceneObjectNode.
  /// \returns A reference to the current object.
  GSE_SceneObjectNode & operator=(const GSE_SceneObjectNode &Node)
  {
    m_pRenderNode = Node.m_pRenderNode;
    SetType(Scene::OBJECT);
    return *this;
  }
  /////////////////////////////////////////////////////////////////
  /// Assigns the Object data.
  /// \param pNode The node which contains renderable stuff.
  void SetRenderNode( GSE_RenderNode *pNode )
  {
    // Previous node should NOT be deleted
    //if ( m_pRenderNode != NULL ) delete m_pRenderNode;
    m_pRenderNode = pNode;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns the object data
  /// \returns A pointer to GSE_RenderNode.
  GSE_RenderNode * GetRenderNode()
  {
    return m_pRenderNode;
  }
};
/////////////////////////////////////////////////////////////////
/// The node for LARGE objects.
class GSE_SceneOctreeObjectNode : public GSE_SceneObjectNode
{
  friend class GSE_SceneGraph;
 protected:
  /// a pointer to octree.
  GSE_GeometryOctree *m_pOctree;
  /// a pointer to geometrynode.
  GSE_GeometryNode *m_pGeometryNode;
  /////////////////////////////////////////////////////////////////
  /// The constructor.
  GSE_SceneOctreeObjectNode() : GSE_SceneObjectNode(), m_pOctree(NULL)
  {
    SetType(Scene::OCTREEOBJECT);
  }
 public:
  /////////////////////////////////////////////////////////////////
  /// Assigns the octree member.
  /// \param pOctree A pointer to the octree instance.
  inline void SetGeometryOctree( GSE_GeometryOctree *pOctree )
  {
    m_pOctree = pOctree;
  }
  /////////////////////////////////////////////////////////////////
  /// Returns a pointer to the octree member.
  /// \returns A pointer to the member.
  inline GSE_GeometryOctree * GetGeometryOctree()
  {
    return m_pOctree;
  }
  /// Returns pointer to geometrynode.
  /// \return A pointer to geometrynode.
  inline GSE_GeometryNode * GetGeometryNode()
  {
    return m_pGeometryNode;
  }
  /// Assigns geometry node.
  /// \param pGeometryNode a pointer to a geometrynode to be assigned.
  inline void SetGeometryNode( GSE_GeometryNode *pGeometryNode )
  {
    m_pGeometryNode = pGeometryNode;
  }
};
/////////////////////////////////////////////////////////////////
/// \brief A class for representing limbs of the animated objects.
/// Limbs do not have the renderable data, but the bounding volume 
/// of it.
class GSE_SceneLimbNode : public GSE_SceneNode,
			  public GSE_LocalBoundingVolume
{
  friend class GSE_SceneGraph;
 protected:
  /// Default constructor.
  GSE_SceneLimbNode() : GSE_SceneNode(), GSE_LocalBoundingVolume()
  {
    SetType( Scene::LIMB );
  }
  
};
/////////////////////////////////////////////////////////////////
/// The light node class for scene graph; positions the light in the scene.
class GSE_SceneLightNode : public GSE_SceneNode,
			   public GSE_LocalBoundingVolume
			  
{
  friend class GSE_SceneGraph;
 protected:
  /// Member pointer to the GSE_LightNode.
  GSE_Light *m_pLight;
  
  /// Default constructor.
  GSE_SceneLightNode() : GSE_SceneNode()
  {
    SetLight(NULL);
    SetType(Scene::LIGHT);
    m_pLight = NULL;
  }
  /// Destructor.
  ~GSE_SceneLightNode()
  {
    m_pLight = NULL;
  }
 public:
  
  /// Copy constructor.
  GSE_SceneLightNode( const GSE_SceneLightNode &Node)
  {
    SetLight(Node.m_pLight);
    SetType(Scene::LIGHT);
  }
  /// Assignment operator.
  /// \param Node node to be assigned.
  inline GSE_SceneLightNode &operator=(GSE_SceneLightNode Node)
  {
    SetLight( Node.m_pLight );
    return *this;
  }
  /// Assigns the light.
  /// \param pLight a pointer to a light.
  inline void SetLight( GSE_Light *pLight )
  {
    m_pLight = pLight;
  }
  // Returns lightnode.
  /// \return a pointer to light node.
  inline GSE_Light *GetLight()
  {
    return m_pLight;
  }
};
/////////////////////////////////////////////////////////////////
class GSE_SceneParticleSystemNode : public GSE_SceneObjectNode
{
  friend class GSE_SceneGraph;
 protected:
  /// The renderable particle system node.
  GSE_ParticleSystemNode *m_pSystemNode;
  ////////////////////
  /// Default constructor.
  GSE_SceneParticleSystemNode() : m_pSystemNode(NULL)
  {
    SetType( Scene::PARTICLESYSTEM );
  }
  ////////////////////
  /// Destructor.
  ~GSE_SceneParticleSystemNode()
  {
  }
 public:
  ////////////////////
  /// Assigns a particlesystemnode to this scenenode.
  /// \param pPSNode a pointer to a particlesystemnode to be assigned.
  inline void SetRenderParticleSystemNode( GSE_ParticleSystemNode *pPSNode )
  {
    m_pSystemNode = pPSNode;
  }
  ////////////////////
  /// Accesses current particlesystemnode.
  /// \return A pointer to renderparticlesystemnode.
  inline GSE_ParticleSystemNode * GetRenderParticleSystemNode()
  {
    return m_pSystemNode;
  }
};
/////////////////////////////////////////////////////////////////
//
// PortalGraph nodes
//
/////////////////////////////////////////////////////////////////
#include "GSE_geometry.h"
/////////////////////////////////////////////////////////////////
// Types of sector nodes
namespace Portal 
{
  typedef enum 
  {
    UNDEFINED = 0,
    SECTOR = 1,
    SECTORGROUP, 
    PORTAL,
    ROOT
  } NodeType;

} // end namespace Portal;
/////////////////////////////////////////////////////////////////
#define NUM_FLOATS_FOR_FOUR_CORNERS 12
typedef GSE_Graph<Portal::NodeType > GSE_PortalGraph;
typedef GSE_GraphNode< Portal::NodeType > GSE_PortalGraphNode;
/////////////////////////////////////////////////////////////////
using Geometry::GSE_Plane;
using Geometry::GSE_OrientedBox;
/////////////////////////////////////////////////////////////////
// The Portal Sector class. This node contains portals which lead to other 
// sectors, which lead to sectors of their own, and so forth. 
// By culling the portals we can perform efficient occlusion culling - but 
// the sectors and portals must be defined reasonably to do so.
/////////////////////////////////////////////////////////////////
using Geometry::GSE_KDOP;
/////////////////////////////////////////////////////////////////
class GSE_SectorNode : public GSE_PortalGraphNode
{
  friend class GSE_Graph<Portal::NodeType>;
 protected:
  
  GSE_SceneNode *		m_pSceneNode;   // The scenenode which this portalcellnode is related to
  Geometry::GSE_KDOP		m_kDOP;      // The volume which contains this sector.
  ////////////////////
  GSE_SectorNode()
  {
    SetType( Portal::SECTOR );
    m_pSceneNode = NULL;
    //m_pKDopUtilNode = new GSE_KDopUtilNode();
    SetVisited(0);
  }
  ////////////////////
  ~GSE_SectorNode()
  {
    m_pSceneNode = NULL; // scenenodes are released with the scene graph.
  }
 public:
  ////////////////////
  void SetSceneNode( GSE_SceneNode *pNode )
  {
    m_pSceneNode = pNode;
  }
  ////////////////////
  GSE_SceneNode * GetSceneNode()
  {
    return m_pSceneNode;
  }
  ////////////////////
  void SetKDOP( GSE_KDOP kDOP )
  {
    m_kDOP = kDOP;
  }
  ////////////////////
  GSE_KDOP &GetKDOP()
  {
    return m_kDOP;
  }
};
/////////////////////////////////////////////////////////////////
//
// A portal class. Portal is a "window" which leads to a SceneNode
// This portal consists only of four vertices forming a convex polygon
// 
/////////////////////////////////////////////////////////////////
class GSE_PortalNode : public GSE_PortalGraphNode
{
  friend class GSE_Graph<Portal::NodeType>;
 public:
  /////////////////////////////////////////////////////////////////
  GSE_UtilLineStripNode *m_pLineStripNode;
  GSE_Plane		m_PolygonPlane;
  GSE_OrientedBox	m_BoundingBox;
  float			m_aCorners[NUM_FLOATS_FOR_FOUR_CORNERS];
 protected:
  ////////////////////
  /// The constructor.
  GSE_PortalNode()
  {
    SetType( Portal::PORTAL );
    memset(m_aCorners,0,sizeof(float)*NUM_FLOATS_FOR_FOUR_CORNERS);
    m_pLineStripNode = NULL;
  }
  ////////////////////
  // The destructor
  ~GSE_PortalNode()
  {
    
  }
 public:
  ////////////////////
  GSE_OrientedBox &BoundingBox()
  {
    return m_BoundingBox;
  }
  
}; // class GSE_PortalNode
////////////////////
// A grouping node which children are other grouping nodes or portal nodes.
// This way a vast set of sectors can be grouped in a smart way so searching 
// the initial portal node where camera is will be faster.
class GSE_SectorGroup : public GSE_PortalGraphNode
{
  friend class GSE_Graph<Portal::NodeType>;
 protected:
  Geometry::GSE_KDOP m_kDOP;      


  GSE_SectorGroup()
  {
    SetType(Portal::SECTORGROUP);
  }
 public:
  void SetKDOP( GSE_KDOP kDOP )
  {
    m_kDOP = kDOP;
  }
  
  GSE_KDOP &GetKDOP()
  {
    return m_kDOP;
  }
};
/////////////////////////////////////////////////////////////////
class GSE_PortalGraphRoot : public GSE_PortalGraphNode
{
  friend class GSE_Graph<Portal::NodeType>;
 protected:
  GSE_PortalGraphRoot()
  {
    SetType( Portal::ROOT );
  }
};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// history:
//		$Log: GSE_GraphNodes.h,v $
//		Revision 1.44  2007/06/02 10:38:45  entity
//		doxy comments added
//
//		Revision 1.43  2007/06/01 08:55:43  entity
//		doxygen comments added
//
//		Revision 1.42  2007/05/18 08:08:09  entity
//		Vector3 constructor check & fixes
//
//		Revision 1.41  2007/05/16 13:01:33  entity
//		utilquad with texture coords
//
//		Revision 1.40  2007/05/14 09:47:20  entity
//		SceneNode inherits GSE_Visible
//
//		Revision 1.39  2007/05/10 11:08:00  entity
//		polygonmodenode added
//
//		Revision 1.38  2007/04/09 09:40:30  entity
//		AlphaTest added
//
//		Revision 1.37  2007/04/03 20:23:23  entity
//		working particlesystem implementation on scengraph
//
//		Revision 1.36  2007/04/03 12:55:44  entity
//		lighting scheme changes
//
//		Revision 1.35  2007/04/02 11:14:12  entity
//		char -> int retvals
//
//		Revision 1.34  2007/04/01 15:25:05  entity
//		texture system overhaul, Elements->Indexbuffer rename
//
//		Revision 1.33  2007/04/01 08:14:55  entity
//		indexbuffer
//
//		Revision 1.32  2007/03/31 16:30:38  entity
//		code cleanups
//
//		Revision 1.31  2007/03/31 16:20:43  entity
//		ObjStruct -> GeometryData rename
//
//		Revision 1.30  2007/03/31 15:56:35  entity
//		ElementList->IndexBuffer rename
//
//		Revision 1.29  2007/03/30 12:54:55  entity
//		Added geometryoctree for debugging
//
//		Revision 1.28  2007/03/29 13:16:17  entity
//		octree declaration instead of header
//
//		Revision 1.27  2007/03/29 07:23:51  entity
//		overrided GetGraph() in SceneNode
//
//		Revision 1.26  2007/03/28 10:37:48  entity
//		removed debugnode
//
//		Revision 1.25  2007/03/28 08:27:50  entity
//		GSE_SceneOctreeObjectNode improved
//
//		Revision 1.24  2007/03/27 12:33:01  entity
//		octreeobject added
//
//		Revision 1.23  2007/03/23 13:31:21  entity
//		LIMB added
//
//		Revision 1.22  2007/03/21 13:06:48  entity
//		fixed comments
//
//		Revision 1.21  2007/03/21 08:23:49  entity
//		hierarchial bounding volume scheme with some refactoring
//
//		Revision 1.20  2007/03/21 06:37:08  entity
//		Doxygen comments added.
//
//		Revision 1.19  2007/03/20 20:41:21  entity
//		world and local bounding volumes
//
//		Revision 1.18  2007/03/20 20:11:23  entity
//		Scenenode with local and world bounding volumes
//
//		Revision 1.17  2007/03/19 15:39:55  entity
//		new GSE_Illuminated class added
//
//		Revision 1.16  2007/03/16 08:13:49  entity
//		SceneLightNode::IlluminationSet is initialized to NULL
//
//		Revision 1.15  2007/03/15 21:30:11  entity
//		IlluminationSet added to SceneLightNode
//
//		Revision 1.14  2007/03/14 21:51:21  entity
//		GetRenderCameraNode() for GSE_SceneCameraNode
//
//		Revision 1.13  2007/03/14 20:01:55  entity
//		const & added for Set{Local,World}Transform()
//
//		Revision 1.12  2007/03/14 13:11:48  entity
//		SceneNode is no longer positional
//
//		Revision 1.11  2007/03/13 21:03:23  entity
//		Better transform hook on scenenodes
//
//		Revision 1.10  2007/03/12 14:50:51  entity
//		Removed unneeded virtual functions
//
//		Revision 1.9  2007/03/11 17:04:32  entity
//		scenegraph node overhaul
//
//		Revision 1.8  2007/03/11 12:01:26  entity
//		RenderNode::TransformNode
//		Animation::-TransformNode
//		SceneNode has now GSE_Transform as localtransform
//		SceneGraph encapsulates its own AnimGraph & RenderGraph
//
//		Revision 1.7  2007/03/10 14:54:19  entity
//		More polyline node stuff
//
//		Revision 1.6  2007/03/10 10:10:28  entity
//		namespace mangling
//
//		Revision 1.5  2007/03/09 12:23:50  entity
//		Graph as a friend class, lights set to NULL initially and pointers copied whencopy constructor is
//		called
//
//		Revision 1.4  2007/03/08 21:47:09  entity
//		new lighting scheme complete
//
//		Revision 1.3  2007/03/08 21:08:56  entity
//		constructors of nodes are visible to graph objects only
//
//		Revision 1.2  2007/03/08 20:20:49  entity
//		Added missing headers
//
//		Revision 1.1  2007/03/08 20:07:18  entity
//		Moved graphnode from GSE_datastructures.h to its own file
//
/////////////////////////////////////////////////////////////////
