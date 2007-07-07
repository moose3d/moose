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
// File: GSE_OglRenderer.h
// Desc: The OpenGL Renderer class which uses OpenGL library to
//	 display geometry on screen.
/////////////////////////////////////////////////////////////////
#ifndef __GSE_OglRenderer_h__
#define __GSE_OglRenderer_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include "GSE_graph.h"
#include "GSE_GraphNodes.h"
#include "GSE_structures.h"
#include "GSE_interfaces.h"
#include "GSE_Material.h"
#include <string>
#include <stack>
#include <vector>
#include "GSE_geometry.h"
class GSE_GeometryOctree;
/////////////////////////////////////////////////////////////////
struct GSE_OglRendererSelectionArea
{
  unsigned int m_nMouseX;
  unsigned int m_nMouseY;
  unsigned int m_nWidth;
  unsigned int m_nHeight;
  GSE_OglRendererSelectionArea()
  {
    m_nMouseX = m_nMouseY = 0;
    m_nWidth = m_nHeight = 1;
  }
};
/////////////////////////////////////////////////////////////////
// The fontset class which is used to store font textures and display lists 
// for drawing text.
/////////////////////////////////////////////////////////////////
class GSE_OglRendererFontset
{
 protected:
  // Pointers to font textures
  GSE_Texture * m_ppTextures[GSE_MAX_FONT_CHARACTERS];   
  // The display lists for font letters
  GLuint	 m_nDisplayLists; 
  // This will not be passed as a copy
  GSE_OglRendererFontset( const GSE_OglRendererFontset &fs )
  {
    
  }
 public:
  // The constructor
  GSE_OglRendererFontset();
  // The destructor
  ~GSE_OglRendererFontset();
  // Returns the reference to the display list
  GLuint &DisplayList();
  // Returns the pointer to array of texture pointers
  GSE_Texture * *Textures();  
};
/////////////////////////////////////////////////////////////////
typedef enum 
{
    
  OGL_RENDER_PASS_ONLY_OPAQUE = 0,	// For rendering every opaque object
  OGL_RENDER_PASS_ONLY_TRANSPARENT = 1,	// For rendering every transparent object
  // sorted back-to-front
  //OGL_RENDER_PASS_CREATE_LIGHTMAPS = 2, // For rendering lightmaps for lights
  // casting shadows
  //OGL_RENDER_PASS_CAST_SHADOWS = 3      // For rendering shadows
  OGL_RENDER_PASS_GET_SELECTION,		// For retrieving the objects that were pointed somehow
  OGL_RENDER_PASS_DEBUG_PRINT 
  
} GSE_OglRenderPass_t;
/////////////////////////////////////////////////////////////////
///
/// \brief A class which tells which OpenGL features are supported by underlying hardware
///
/////////////////////////////////////////////////////////////////
class GSE_OglRendererFeatures
{
 protected:
  /// vertex_program support flag.
  char m_bARB_vertex_program;
  /// vertex_shader support flag.
  char m_bARB_vertex_shader;
  /// fragment_shader support flag.
  char m_bARB_fragment_shader;
  /// vertex_array support flag.
  char m_bEXT_vertex_array;
  /// vertex_buffer_object support flag.
  char m_bARB_vertex_buffer_object;
  /// multitexture support flag.
  char m_bARB_multitexture;
  /// shader_objects support flag.
  char m_bARB_shader_objects;
  /// Max number of supported lights.
  int m_iMaxLights;
  /// GL_MAX_ELEMENTS_VERTICES, used to create optimal render batches.
  int m_iMaxElementsVertices; 
  /// GL_MAX_ELEMENTS_INDICES, used to create optimal render batches.
  int m_iMaxElementsIndices;  

 public:
  /// Default constructor.
  GSE_OglRendererFeatures();
  /// Is vertex_program extension supported.
  /// \return 1 if supported, 0 otherwise.
  char HasVertexProgram();
  /// Is vertex_shader extension supported.
  /// \return 1 if supported, 0 otherwise.
  char HasVertexShader();
  /// Is fragment_shader extension supported.
  /// \return 1 if supported, 0 otherwise.
  char HasFragmentShader();
  /// Is vertex_array supported.
  /// \return 1 if supported, 0 otherwise.
  char HasVertexArray();
  /// Is vertex_buffer_object supported.
  /// \return 1 if supported, 0 otherwise.
  char HasVertexBufferObject();
  /// Is multitexture supported.
  /// \return 1 if supported, 0 otherwise.
  char HasMultitexture();
  /// Is shader_objects supported.
  /// \return 1 if supported, 0 otherwise.
  char HasShaderObjects();
  /// Get maximum number of lights.
  /// \return maximum number of lights.
  int  GetMaxLights();
  /// Get max elements in vertex arrays.
  /// \return maximum number of elements in vertex arrays.
  int  GetMaxElementsVertices();
  /// Get max elements in index arrays.
  /// \return maximum number of elements in index arrays.
  int  GetMaxElementsIndices();
  /// for printing out supported features.
  friend std::ostream &operator<<(std::ostream &stream, GSE_OglRendererFeatures &obj);
 protected:
  /// Initializes the supported values.
  void Init();

  
};
/////////////////////////////////////////////////////////////////
class GSE_OglRenderer : public GSE_MultiPassRenderer < GSE_OglRenderPass_t >
{

 public:
  
  
 protected:
  /// Supported features.
  GSE_OglRendererFeatures *m_pFeatures;
  /// The fontsets which can be used to render text.
  std::vector<GSE_OglRendererFontset *> m_vFontsets;
  /// Currently active camera. 
  /// \todo Rather obsolete, fix.
  GSE_Camera *m_pCurrentCamera;
  /// If true, Render()ing a root node will cause certain buffers to be cleared.
  char m_bClearBuffers;  
  
  /// Temporary bounding box for RENDER_PASS_GET_BOUNDING_VOLUME pass.
  GSE_OrientedBox  m_obBoundingBox;
  /// Temporary bounding sphere for RENDER_PASS_GET_BOUNDING_VOLUME pass.
  GSE_Sphere	   m_BoundingSphere;
  /// Has the Bounding Sphere been merged with some other sphere. 
  /// This is used for
  /// determining should the calculated Bounding sphere be assigned to the
  /// m_BoundingSphere or merged with it.
  char		   m_bSphereInitialized;
  /// Has the Bounding Box been merged with some other box This is used for
  /// determining should the calculated OBB be assigned to m_obBoundingBox or
  /// merged with it.
  char		   m_bOBBInitialized;
  
  /// If a display list is being compiled, this is non-empty. 
  /// Used to track the point where display list compilation began.
  std::stack<char> m_DisplayListStack;
  /// Used to track the selection point. 
  /// \todo *OBSOLETE* fix it.
  std::stack<char> m_SelectionStack;
  
  /// The selection buffer for selecting objects.
  /// \todo *OBSOLETE*, fix it.
  GLuint	   m_aSelectionBuffer[GSE_MAX_SELECTION_OBJECTS];
  /// The number of objects stored int the hit buffer.
  /// \todo *OBSOLETE*, fix it.
  unsigned int	   m_nHits;
  
  /// The selection area.
  /// \todo *OBSOLETE*, fix it.
  GSE_OglRendererSelectionArea	m_Selection;
  
  
  /////////////////////////////////////////////////////////////////
  /// Debug printing; strings which are used to display a graph with ASCII.
  std::string m_sPadding;  
  /// Arrow string "-->"
  std::string m_sArrow;
  
 public:

  /// default constructor
  GSE_OglRenderer();
  /// destructor
  ~GSE_OglRenderer();
  /// Sets the render pass mode for next render() call.
  void SetRenderPass(GSE_OglRenderPass_t iRenderPass);
  /// Renders the Subtree of scene graph beginning from node pNode .
  int Enter( GSE_GraphNode<RenderNode::NodeType> *pNode );
  /// Swaps the buffers if double buffering is used, otherwise it doesn't do a thing.
  void SwapBuffers();
  
  /// Should buffers be cleared when root node is encountered.
  void SetClearBuffers( char bFlag );

  /// Sets the point/area where selection occurs during RENDER_PASS_GET_SELECTION
  /// \todo *OBSOLETE*
  /// \param nMouseX x-coordinate where mouse was clicked.
  /// \param nMouseY y-coordinate where mouse was clicked.
  /// \param nWidth width of selection area (around clicking point).
  /// \param nHeight height of selection area (around clicking point).
  void SetSelection( unsigned int nMouseX, unsigned int nMouseY, 
		     unsigned int nWidth = 1, unsigned int nHeight = 1 );
  /// Returns the selection buffer.
  GLuint * GetSelectionBuffer();
  /// Returns the number of hits in the selection buffer.
  const unsigned int GetNumHits();
  /// Returns the bounding box, valid only after RENDER_PASS_GET_BOUNDING_VOLUME.
  Geometry::GSE_OrientedBox & GetBoundingBox();
  
  // Returns the bounding sphere, valid only after RENDER_PASS_GET_BOUNDING_VOLUME.
  Geometry::GSE_Sphere & GetBoundingSphere();
  /// Creates a new fontset and adds it to m_vFontsets. 
  /// \param sPathToFontFile path to TTF font file.
  /// \parma nFontSize point size for font.
  /// \return on success zero. on error, non-zero is returned and m_vFontset is not changed.
  int CreateFontset( const char *sPathToFontFile, unsigned int nFontSize);
  /// Return the reference to the m_vFontsets.
  std::vector<GSE_OglRendererFontset *> & Fontsets();
  /// Performs some operations when a node with its child nodes have been handled.
  /// \param pNode Node to be handled.
  int Leave( GSE_GraphNode<RenderNode::NodeType> *pNode );
 protected:
  /// Draws octree node.
  void DrawOctree( GSE_GeometryOctree *pGeomOctree );
  /// Sets up the arrays for vertices, normal vectors and texture coordinates.
  void SetupArrays( GSE_GeometryData *m_pGeometryData );
  /// Draws the elements from previously set arrays.
  /// \param pIndexBuffer which indices are used and what primitives will be created.
  void DrawPrimitive( GSE_IndexBuffer *pIndexBuffer );
  /// Applies the properties of a material. 
  /// \param iType GL_FRONT or GL_BACK
  /// \param pMaterial Material which properties are applied.
  void ApplyMaterial( GLenum iType, GSE_Material *pMaterial );
  /// Disables the material properties set in ApplyMaterial.
  void DisableMaterial( GSE_Material *pMaterial );
  /// Applies the view settings from a camera.
  /// \param pCamera a camera.
  void ApplyPerspective( GSE_Camera *pCamera );
  /// Applies the light settings from a light.
  /// \param pLight a light.
  void ApplyLightParameters( GSE_LightClusterNode *pLight );
  /// Applies the vertex attributes for shader usage.
  /// \param pVertexAttrib the vertex attributes.
  void SetupVertexAttrib( GSE_VertexAttribStruct *pVertexAttrib);
  /// Finalizes the vertex attributes when render pass exists the node.
  void FinishVertexAttrib( GSE_VertexAttribStruct *pVertexAttrib);
  /// Handles the global lighting parameters.
  void ApplyGlobalLighting( GSE_GlobalLightingNode *pGlobalLightingNode );
  /// Handles the selection name.
  void SetSelectionName( GSE_SelectionNameNode *pSelectionNameNode );
  /// Handles the textnode.
  void RenderText( GSE_TextNode *pTextNode );
  /// Handles the pushing of attributes to the stack.
  void HandleStackPush( GSE_RenderStateStackNode *pNode );
  /// Handles the particle system update - vertex transform.
  void Handle_ParticleSystemUpdate( GSE_ParticleSystemNode *pNode );
  /// Handles the utility quad drawing.
  void Handle_Draw_UtilQuad( GSE_UtilQuadNode *pNode );
  /// Handles Cullingstate node.
  void Handle_Draw_CullingState( GSE_CullingStateNode *pCullingStateNode );
  /// Debug print handler.
  int Handle_DebugPrint_RenderPass_Enter( GSE_GraphNode<RenderNode::NodeType> *pNode );
  /// Debug print handler.
  int Handle_DebugPrint_RenderPass_Leave( GSE_GraphNode<RenderNode::NodeType> *pNode );
  /// Handles the render passes of the OglCacheNode.
  int HandleOglCache_Enter( GSE_OglCacheNode *pNode );
  /// Handles the finish passes of the OglCacheNode.
  int HandleOglCache_Leave( GSE_OglCacheNode *pNode );
  /// Handles the render passes of the normal drawing process.
  int Handle_Drawing_RenderPass_Enter( GSE_GraphNode<RenderNode::NodeType> *pNode );
  /// Handles the finish passes of the normal drawing process.
  int Handle_Drawing_RenderPass_Leave( GSE_GraphNode<RenderNode::NodeType> *pNode );
  /// Handles the RENDER_PASS_CONSTRUCT_BOUNDING_VOLUMES render.
  int Handle_BoundingVolume_RenderPass_Enter( GSE_GraphNode<RenderNode::NodeType> *pNode );
  /// Draws the polyline.
  void DrawPolyline( GSE_PolyLineNode *pNode );
  /// Optimizes given IndexBuffer indices so that they can be send as optimally as possible by using glDrawRangeElements.
  void OptimizeVBOBatching( GSE_IndexBuffer *pIndexBuffer );
  ////////////////////
  /// Handles textures on enter().
  int HandleTexture( GSE_TextureNode *pNode );
  
  
};  
/////////////////////////////////////////////////////////////////
// $Log: GSE_OglRenderer.h,v $
// Revision 1.27  2007/06/04 09:03:25  entity
// GLee fix
//
// Revision 1.26  2007/06/02 10:23:12  entity
// fixed comments.
//
// Revision 1.25  2007/06/01 08:55:43  entity
// doxygen comments added
//
// Revision 1.24  2007/05/21 12:50:00  entity
// glew to GLee
//
// Revision 1.23  2007/04/02 11:10:32  entity
// some comments
//
// Revision 1.22  2007/04/01 15:25:05  entity
// texture system overhaul, Elements->Indexbuffer rename
//
// Revision 1.21  2007/03/31 16:20:43  entity
// ObjStruct -> GeometryData rename
//
// Revision 1.20  2007/03/31 15:56:35  entity
// ElementList->IndexBuffer rename
//
// Revision 1.19  2007/03/30 12:59:13  entity
// DrawOctree() added
//
// Revision 1.18  2007/03/26 12:59:42  entity
// commented obsolete WindowCoordinatesToLine
//
// Revision 1.17  2007/03/20 12:23:01  entity
// code cleanups
//
//              10082006 AG/eNtity : added font support
//		Way back AG/eNtity : initial version
//           
/////////////////////////////////////////////////////////////////
#endif
