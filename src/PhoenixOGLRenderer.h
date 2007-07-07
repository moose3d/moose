/////////////////////////////////////////////////////////////////
#ifndef __PhoenixOGLRenderer_h__
#define __PhoenixOGLRenderer_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    
    /////////////////////////////////////////////////////////////////
    /// \brief A class which tells which OpenGL features are supported 
    /// by underlying hardware
    class COglRendererFeatures
    {
    protected:
      /// vertex_program support flag.
      int m_bARB_vertex_program;
      /// vertex_shader support flag.
      int m_bARB_vertex_shader;
      /// fragment_shader support flag.
      int m_bARB_fragment_shader;
      /// vertex_array support flag.
      int m_bEXT_vertex_array;
      /// vertex_buffer_object support flag.
      int m_bARB_vertex_buffer_object;
      /// multitexture support flag.
      int m_bARB_multitexture;
      /// shader_objects support flag.
      int m_bARB_shader_objects;
      /// Max number of supported lights.
      int m_iMaxLights;
      /// GL_MAX_ELEMENTS_VERTICES, used to create optimal render batches.
      int m_iMaxElementsVertices; 
      /// GL_MAX_ELEMENTS_INDICES, used to create optimal render batches.
      int m_iMaxElementsIndices;  

    public:
      /// Default constructor.
      COglRendererFeatures();
      /// Is vertex_program extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexProgram();
      /// Is vertex_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexShader();
      /// Is fragment_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasFragmentShader();
      /// Is vertex_array supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexArray();
      /// Is vertex_buffer_object supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexBufferObject();
      /// Is multitexture supported.
      /// \return 1 if supported, 0 otherwise.
      int HasMultitexture();
      /// Is shader_objects supported.
      /// \return 1 if supported, 0 otherwise.
      int HasShaderObjects();
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
      friend std::ostream &operator<<(std::ostream &stream, COglRendererFeatures &obj);
    protected:
      /// Initializes the supported values.
      void Init();

  
    };
    // The fontset class which is used to store font textures and display lists 
    // for drawing text.
    /////////////////////////////////////////////////////////////////
    class COglRendererFontset
    {
    protected:
      // Pointers to font textures
      COglTexture * m_ppTextures[Phoenix::Graphics::MAX_FONT_CHARACTERS];   
      // The display lists for font letters
      GLuint	 m_nDisplayLists; 
      ////////////////////
      // This will not be passed as a copy
      COglRendererFontset( const COglRendererFontset &fs )      {  }
    public:
      ////////////////////
      /// The constructor
      COglRendererFontset();
      ////////////////////
      /// The destructor
      ~COglRendererFontset();
      ////////////////////
      /// Returns the reference to the display list
      GLuint &GetDisplayList();
      /// Returns the pointer to array of texture pointers
      COglTexture * *Textures();  
    };
    /////////////////////////////////////////////////////////////////
    /// Renderer object for OpenGL.
    class COglRenderer
    {
    protected:
      /// Supported features.
      COglRendererFeatures *m_pFeatures;
      /// The fontsets which can be used to render text.
      std::vector<COglRendererFontset *> m_vFontsets;
    public:
      ////////////////////
      /// Default constructor
      COglRenderer();
      ////////////////////
      /// Destructor
      ~COglRenderer();
      ////////////////////
      /// Swaps the buffers if double buffering is used, otherwise 
      /// it doesn't do a thing.
      void Finalize();
      ////////////////////
      /// Creates a new fontset and adds it to m_vFontsets. 
      /// \param sPathToFontFile path to TTF font file.
      /// \parma nFontSize point size for font.
      /// \return on success zero. on error, non-zero is returned and m_vFontset is not changed.
      int CreateFontset( const std::string *sPathToFontFile, unsigned int nFontSize);
      ////////////////////
      /// Return the reference to the m_vFontsets.
      /// \returns fontsets in a vector.
      std::vector<COglRendererFontset *> & Fontsets();

      /// Sets up the arrays for vertices, normal vectors and texture coordinates.
      void SetupArrays( CGeometryData *m_pGeometryData );
      /// Draws the elements from previously set arrays.
      /// \param pIndexBuffer which indices are used and what primitives will be created.
      void DrawPrimitive( CIndexBuffer *pIndexBuffer );
      /// Applies the properties of a material. 
      /// \param iType GL_FRONT or GL_BACK
      /// \param pMaterial Material which properties are applied.
      void ApplyMaterial( GLenum iType, CMaterial *pMaterial );
      /// Disables the material properties set in ApplyMaterial.
      void DisableMaterial( CMaterial *pMaterial );
      /// Applies the view settings from a camera.
      /// \param pCamera a camera.
      void ApplyPerspective( CCamera *pCamera );
      /// Applies the light settings from a light.
      /// \param pLight a light.
      void ApplyLightParameters( CLightClusterNode *pLight );
      /// Applies the vertex attributes for shader usage.
      /// \param pVertexAttrib the vertex attributes.
      void SetupVertexAttrib( CVertexAttribStruct *pVertexAttrib);
      /// Finalizes the vertex attributes when render pass exists the node.
      void FinishVertexAttrib( CVertexAttribStruct *pVertexAttrib);
      /// Handles the global lighting parameters.
      void ApplyGlobalLighting( CGlobalLightingNode *pGlobalLightingNode );
      /// Handles the selection name.
      void SetSelectionName( CSelectionNameNode *pSelectionNameNode );
      /// Handles the textnode.
      void RenderText( CTextNode *pTextNode );
      /// Handles the pushing of attributes to the stack.
      void HandleStackPush( CRenderStateStackNode *pNode );
      /// Handles the particle system update - vertex transform.
      void Handle_ParticleSystemUpdate( CParticleSystemNode *pNode );
      /// Handles the utility quad drawing.
      void Handle_Draw_UtilQuad( CUtilQuadNode *pNode );
      /// Handles Cullingstate node.
      void Handle_Draw_CullingState( CCullingStateNode *pCullingStateNode );
      /// Debug print handler.
      int Handle_DebugPrint_RenderPass_Enter( CGraphNode<RenderNode::NodeType> *pNode );
      /// Debug print handler.
      int Handle_DebugPrint_RenderPass_Leave( CGraphNode<RenderNode::NodeType> *pNode );
      /// Handles the render passes of the OglCacheNode.
      int HandleOglCache_Enter( COglCacheNode *pNode );
      /// Handles the finish passes of the OglCacheNode.
      int HandleOglCache_Leave( COglCacheNode *pNode );
      /// Handles the render passes of the normal drawing process.
      int Handle_Drawing_RenderPass_Enter( CGraphNode<RenderNode::NodeType> *pNode );
      /// Handles the finish passes of the normal drawing process.
      int Handle_Drawing_RenderPass_Leave( CGraphNode<RenderNode::NodeType> *pNode );
      /// Handles the RENDER_PASS_CONSTRUCT_BOUNDING_VOLUMES render.
      int Handle_BoundingVolume_RenderPass_Enter( CGraphNode<RenderNode::NodeType> *pNode );
      /// Draws the polyline.
      void DrawPolyline( CPolyLineNode *pNode );
      /// Optimizes given IndexBuffer indices so that they can be send as optimally as possible by using glDrawRangeElements.
      void OptimizeVBOBatching( CIndexBuffer *pIndexBuffer );
      ////////////////////
      /// Handles textures on enter().
      int HandleTexture( CTextureNode *pNode );
  
  
    };  
  } // namespace Graphics
} // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
