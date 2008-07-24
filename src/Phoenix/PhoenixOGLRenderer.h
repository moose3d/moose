/////////////////////////////////////////////////////////////////
#ifndef __PhoenixOGLRenderer_h__
#define __PhoenixOGLRenderer_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
/////////////////////////////////////////////////////////////////
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixVector4.h"
#include "PhoenixCamera.h"
#include "PhoenixTexture.h"
#include "PhoenixRenderable.h"
#include "PhoenixShader.h"
#include "PhoenixLight.h"
#include "PhoenixMaterial.h"
#include "PhoenixSkybox.h"
#include "PhoenixTransform.h"
#include "PhoenixSpatial.h"
#include "PhoenixFrameBuffer.h"
#include <PhoenixRenderable.h>
#include "PhoenixGlobals.h"
#include "PhoenixRenderState.h"
#include "PhoenixBlendingOperation.h"
#include "PhoenixAlphaTestOperation.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Holds information about current renderstates.
    class CInternalRenderState
    {
    public:
      Phoenix::Graphics::COglTexture *m_pTexture[TEXTURE_HANDLE_COUNT];
      Phoenix::Graphics::CShader     *m_pShader;
      Phoenix::Graphics::CVertexDescriptor *m_pVertices;
      Phoenix::Graphics::CVertexDescriptor *m_pNormals;
      Phoenix::Graphics::CVertexDescriptor *m_pTexCoords[TEXTURE_HANDLE_COUNT];
      Phoenix::Graphics::CIndexArray       *m_pIndices;
    public:      
      
      CInternalRenderState()
      {
	for( size_t i=0;i<TEXTURE_HANDLE_COUNT;i++)
	{
	  m_pTexture[i] = NULL;
	  m_pTexCoords[i] = NULL;
	}
	m_pShader   = NULL;
	m_pVertices = NULL;
	m_pNormals  = NULL;
	m_pIndices  = NULL;
      }
      ////////////////////
      /// Tests is texture unit bound to current texture.
      /// \param nTexUnit Texture unit where texture binding is evaluated.
      /// \param pTexture Pointer to evaluated texture.
      inline bool IsCurrentTexture( size_t nTexUnit, COglTexture *pTexture ) const
      {
	assert( nTexUnit < TEXTURE_HANDLE_COUNT );
	return (m_pTexture[nTexUnit] == pTexture);
      }
      ////////////////////
      /// Sets currently assigned texture.
      /// \param nTexUnit Texture unit where texture will be bound.
      /// \param pTexture Pointer to texture.
      inline void SetCurrentTexture( size_t nTexUnit, COglTexture *pTexture )
      {
	assert( nTexUnit < TEXTURE_HANDLE_COUNT );
	m_pTexture[nTexUnit] = pTexture;
      }
      ////////////////////
      /// 
      inline void SetCurrentShader( CShader *pShader )
      {
	m_pShader = pShader;
      }
      ////////////////////
      /// 
      inline bool IsCurrentShader( CShader *pShader ) const
      {
	return (m_pShader == m_pShader);
      }

      inline bool IsCurrentVertices ( CVertexDescriptor *pVD ) const
      {
	return (m_pVertices == pVD);
      }
      inline void SetCurrentVertices ( CVertexDescriptor *pVD )
      {
	m_pVertices = pVD;
      }

      inline bool IsCurrentNormals ( CVertexDescriptor *pVD ) const
      {
	return (m_pNormals == pVD);
      }

      inline void SetCurrentNormals ( CVertexDescriptor *pVD )
      {
	m_pNormals = pVD;
      }
      
      ////////////////////
      /// tests currently assigned texture coordinates.
      /// \param nTexUnit Texture unit where texture binding is evaluated.
      /// \param pVD Pointer to vertexdescriptor.
      inline bool IsCurrentTexCoord( size_t nTexUnit, CVertexDescriptor *pVD ) const
      {
	assert( nTexUnit < TEXTURE_HANDLE_COUNT );
	return (m_pTexCoords[nTexUnit] == pVD);
      }
      ////////////////////
      /// Sets currently assigned texture coordinates.
      /// \param nTexUnit Texture unit where texture will be bound.
      /// \param pVD Pointer to vertexdescriptor.
      inline void SetCurrentTexCoord( size_t nTexUnit, CVertexDescriptor *pVD )
      {
	assert( nTexUnit < TEXTURE_HANDLE_COUNT );
	m_pTexCoords[nTexUnit] = pVD;
      }
      
      inline bool IsCurrentIndices( CIndexArray *pIndices )
      {
	return (m_pIndices == pIndices);
      }

      inline void SetCurrentIndices( CIndexArray *pIndices )
      {
	m_pIndices = pIndices;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// \brief A class which tells which OpenGL features are supported 
    /// by underlying hardware
    class COglRendererFeatures
    {
    protected:
      /// vertex_program support flag.
      bool m_bARB_vertex_program;
      /// vertex_shader support flag.
      bool m_bARB_vertex_shader;
      /// fragment_shader support flag.
      bool m_bARB_fragment_shader;
      /// vertex_array support flag.
      bool m_bEXT_vertex_array;
      /// vertex_buffer_object support flag.
      bool m_bARB_vertex_buffer_object;
      /// multitexture support flag.
      bool m_bARB_multitexture;
      /// shader_objects support flag.
      bool m_bARB_shader_objects;
      /// framebuffer object support flag.
      bool m_bEXT_framebuffer_object;
      /// GL_EXT_texture_compression_s3tc, used for compressed DDS textures
      bool m_bEXT_texture_compression_s3tc;
      /// Max number of supported lights.
      int m_iMaxLights;
      /// GL_MAX_ELEMENTS_VERTICES, used to create optimal render batches.
      int m_iMaxElementsVertices; 
      /// GL_MAX_ELEMENTS_INDICES, used to create optimal render batches.
      int m_iMaxElementsIndices;  
      /// GL_MAX_COLOR_ATTACHMENTS, used in frame buffers.
      int m_iMaxColorAttachments;
      /// GL_MAX_DRAW_BUFFERS, used in frame buffers.
      int m_iMaxDrawBuffers;
      
    public:
      ////////////////////
      /// Default constructor.
      COglRendererFeatures();
      ////////////////////
      /// Is vertex_program extension supported.
      /// \return true if supported, false otherwise.
      bool HasVertexProgram() const;
      ////////////////////
      /// Is vertex_shader extension supported.
      /// \return true if supported, false otherwise.
      bool HasVertexShader() const;
      ////////////////////
      /// Is fragment_shader extension supported.
      /// \return true if supported, false otherwise.
      bool HasFragmentShader() const;
      ////////////////////
      /// Is vertex_array supported.
      /// \return true if supported, false otherwise.
      bool HasVertexArray() const;
      ////////////////////
      /// Is vertex_buffer_object supported.
      /// \return true if supported, false otherwise.
      bool HasVertexBufferObject() const;
      ////////////////////
      /// Is multitexture supported.
      /// \return true if supported, false otherwise.
      bool HasMultitexture() const;
      ////////////////////
      /// Is shader_objects supported.
      /// \return true if supported, false otherwise.
      bool HasShaderObjects() const;
      ////////////////////
      /// Is EXT_framebuffer_object supported.
      /// \return true if supported, false otherwise.
      bool HasFramebufferObjects() const;
      ////////////////////
      /// Is EXT_texture_compression_s3tc supported.
      /// \return true if supported, false otherwise.
      bool HasTextureCompressionS3TC() const;
      ////////////////////
      /// Get maximum number of lights.
      /// \return maximum number of lights.
      int  GetMaxLights() const;
      ////////////////////
      /// Get max elements in vertex arrays.
      /// \return maximum number of elements in vertex arrays.
      int  GetMaxElementsVertices() const;
      ////////////////////
      /// Get max elements in index arrays.
      /// \return maximum number of elements in index arrays.
      int  GetMaxElementsIndices() const;
      ////////////////////
      /// Get max color attachments for frame buffers.
      /// \returns Maximum number of color attachments for current hardware.
      int  GetMaxColorAttachments() const;
      ////////////////////
      /// Get max draw buffers.
      /// \returns Maximum number of draw buffers for current hardware.
      int  GetMaxDrawBuffers() const;
      ////////////////////
      /// for printing out supported features.
      friend std::ostream &operator<<(std::ostream &stream, 
				      const COglRendererFeatures &obj);
    protected:
      /// Initializes the supported values.
      void Init();
    };
    /////////////////////////////////////////////////////////////////
    /// Fontset for rendering arbitrary text on screen.
    /// \warn Not tested properly!!!
    class CFontset
    {
    protected:
      /// Pointers to font textures.
      Phoenix::Graphics::COglTexture * m_ppTextures[Phoenix::Globals::MAX_FONT_CHARACTERS];   
      /// Width of each character
      unsigned char	m_aCharWidths[Phoenix::Globals::MAX_FONT_CHARACTERS];
      /// Display lists for font letters.
      GLuint		m_nDisplayLists; 
      /// Height of line
      float		m_fLineHeight;
      ////////////////////
      /// This will not be passed as a copy.
      CFontset( const CFontset &fs ) {}
    public:
      ////////////////////
      /// Default constructor.
      CFontset();
      ////////////////////
      /// Destructor.
      ~CFontset();
      ////////////////////
      /// Returns reference to display list.
      /// \returns Reference to display list id.
      GLuint & GetDisplayList();
      ////////////////////
      /// Returns reference to display list.
      /// \returns Reference to display list id.
      const GLuint & GetDisplayList() const;
      ////////////////////
      /// Returns the pointer to array of texture pointers.
      /// \returns Pointer to array of pointers to textures.
      Phoenix::Graphics::COglTexture * *GetTextures();  
      ////////////////////
      /// Returns line height with this font.
      inline float GetLineHeight() const
      {
	return m_fLineHeight;
      }
      ////////////////////
      /// Returns line height with this font.
      inline void SetLineHeight(float fHeight)
      {
	m_fLineHeight = fHeight;
      }
    };
    
    /////////////////////////////////////////////////////////////////
    /// Renderer object for OpenGL.
    class COglRenderer
    {
    protected:
      /// Supported features.
      Phoenix::Graphics::COglRendererFeatures *m_pFeatures;
      /// Alpha test operation
      CAlphaTestOperation		       m_AlphaTest;
      Phoenix::Graphics::CCamera	       *m_pCamera;
      Phoenix::Graphics::CInternalRenderState  m_RenderState;
      GLUquadric *			       m_pQuadric;
      
    public:
      ////////////////////
      /// Default constructor
      COglRenderer();
      ////////////////////
      /// Destructor
      ~COglRenderer();
      ////////////////////
      /// Clears buffer
      void ClearBuffer( BUFFER_TYPE tType);
      ////////////////////
      /// Forces the rendering commands to be completed. 
      /// Returns after currently given commands have been executed.
      void Finalize();
      ////////////////////
      /// Commits vertex descriptor.
      /// \param pBuffer Buffer to be applied.
      /// \param nId Number for active texture unit. Used only in conjunction of ELEMENT_TYPE_TEX_*. If value is omitted, it defaults to zero.
      void CommitVertexDescriptor( Phoenix::Graphics::CVertexDescriptor *pBuffer, unsigned int nId = 0);
      ////////////////////
      /// Disables vertex descriptor.
      /// \param pBuffer Buffer to be applied.
      /// \param nId Number for active texture unit. Used only in conjunction of ELEMENT_TYPE_TEX_*. If value is omitted, it defaults to zero.
      void RollbackVertexDescriptor( Phoenix::Graphics::CVertexDescriptor *pBuffer, unsigned int nId = 0);
      ////////////////////
      /// Draws the elements from previously set arrays.
      /// \param pIndexBuffer which indices are used and what primitives will be created.
      void CommitPrimitive( Phoenix::Graphics::CIndexArray *pIndexBuffer );
      ////////////////////
      /// Sets color for drawing.
      /// \param vColor RGBA Color vector.
      void CommitColor( const Phoenix::Math::CVector4<unsigned char> &vColor );
      ////////////////////
      /// Sets color for drawing.
      /// \param bR Red component.
      /// \param bG Green component.
      /// \param bB Blue component.
      /// \param bA Alpha component.
      void CommitColor( unsigned char bR, unsigned char bG, unsigned char bB, unsigned char bA);
      ////////////////////
      /// Commits vertex with position, texcoords, normal.
      /// \param rVertex Vertex to be committed.
      /// \param iIgnoreFlags What vertex data is ignored - specified by VERTEX_COMP_FLAGS - by default everything is included.
      void CommitVertex ( const Phoenix::Spatial::CVertex & rVertex, int iIgnoreFlags = 0);
      ////////////////////
      /// Disable client states, such as vertex array.
      /// \param tType CLIENT_STATE_TYPE to be disabled.
      void DisableClientState( CLIENT_STATE_TYPE tType );
      ////////////////////
      /// Enable client states, such as vertex array.
      /// \param tType CLIENT_STATE_TYPE to be enabled.
      void EnableClientState( CLIENT_STATE_TYPE tType );
      ////////////////////
      /// Enables texture.
      /// \param nTexUnit which texture unit does this texture belong to ( 0 - 7)
      /// \param pTexture Texture object
      void CommitTexture( unsigned int nTexUnit, Phoenix::Graphics::COglTexture *pTexture );
      ////////////////////
      /// Disables texture.
      /// \param nTexUnit which texture unit does this texture belong to ( 0 - 7)
      /// \param pTexture Texture object.
      void DisableTexture( unsigned int nTexUnit, Phoenix::Graphics::COglTexture *pTexture = NULL);
      ////////////////////
      /// Creates new texture from TGA image.
      /// \param strFilename filename for tga image.
      /// \param tType Texture type for new texture, defaults to TEXTURE_2D.
      /// \returns Pointer to COglTexture.
      Phoenix::Graphics::COglTexture * CreateTexture( const std::string &strFilename, TEXTURE_TYPE tType = TEXTURE_2D );
      ////////////////////
      /// Creates new texture from DDS image.
      /// \param strFilename filename for DDS image.
      /// \param tType Texture type for new texture, defaults to TEXTURE_2D.
      /// \returns Pointer to COglTexture.
      Phoenix::Graphics::COglTexture * CreateCompressedTexture( const char * strFilename, TEXTURE_TYPE tType = TEXTURE_2D );
      ////////////////////
      /// Creates new cube texture from TGA images.
      /// \param szFiles texture filenames for tga image.
      /// \returns Pointer to COglTexture.
      Phoenix::Graphics::COglTexture * CreateCubeTexture( const char * szFiles[6] );
      ////////////////////
      /// Creates new empty texture.
      /// \param nWidth width of texture.
      /// \param nHeight height of texture.
      /// \param tType TEXTURE_TYPE 
      /// \param tFormat TEXTURE FORMAT, by default RGB with alpha channel.
      /// \returns Pointer to COglTexture.
      Phoenix::Graphics::COglTexture * CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType, TEXTURE_FORMAT tFormat = TEX_FORMAT_RGBA );
      ////////////////////
      /// Creates new texture from existing data.
      /// \param nWidth width of texture.
      /// \param nHeight height of texture.
      /// \param tType TEXTURE_TYPE 
      /// \param pData Pointer to data.
      /// \param tFormat TEXTURE FORMAT, by default RGB with alpha channel.
      /// \returns Pointer to COglTexture.
      Phoenix::Graphics::COglTexture * CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType, void *pData, TEXTURE_FORMAT tFormat = TEX_FORMAT_RGBA );
      ////////////////////
      /// Sets view using camera.
      /// \param camera Camera.
      void CommitCamera( Phoenix::Graphics::CCamera &camera );
      ////////////////////
      /// Returns pointer to active camera.
      /// \returns Pointer to camera. NULL if not set.
      Phoenix::Graphics::CCamera * GetCurrentCamera() const;
      ////////////////////
      /// Renders a complete model.
      /// \param renderable Renderable object. 
      /// \param iExcludeOpt Which model data should not be renderered.
      void CommitRenderable( Phoenix::Graphics::CRenderable &renderable, int iExcludeOpt = 0 );
      ////////////////////
      /// Commits a texture filter.
      /// \param tFilter Which filter will be applied.
      /// \param tType TEXTURE_TYPE.
      void CommitFilter( Phoenix::Graphics::TEXTURE_FILTER tFilter, Phoenix::Graphics::TEXTURE_TYPE tType );
      ////////////////////
      /// Commits multiple filters stored in vector into given texture type.
      void CommitFilters( const std::vector<Phoenix::Graphics::TEXTURE_FILTER> &vecFilters, Phoenix::Graphics::TEXTURE_TYPE tType );
      ////////////////////
      /// Creates a shader from files.
      /// \param strVertexShader Path to vertex shader source.
      /// \param strFragmentShader Path to fragment shader source.
      /// \returns Pointer to shader object. NULL if shader could not be created.
      Phoenix::Graphics::CShader * CreateShader( const std::string & strVertexShader, const std::string & strFragmentShader );
      ////////////////////
      /// Creates a shader from files.
      /// \param szVertexShader Path to vertex shader source.
      /// \param szFragmentShader Path to fragment shader source.
      /// \returns Pointer to shader object. NULL if shader could not be created.
      Phoenix::Graphics::CShader * CreateShader( const char * szVertexShader, const char * szFragmentShader );
      ////////////////////
      /// Creates a shader from source codes.
      /// \param szVertexShaderCode Vertex shader source.
      /// \param szFragmentShaderCode Fragment shader source.
      /// \param szVSname Optional name for vertex shader ( for error messages ).
      /// \param szFSname Optional name for fragment shader ( for error messages ).
      /// \returns Pointer to shader object. NULL if shader could not be created.
      Phoenix::Graphics::CShader * CreateShaderFromSource( const char * szVertexShaderCode, const char * szFragmentShaderCode, 
							   const char * szVSname = NULL, const char * szFSname = NULL);
      ////////////////////
      /// Commits shader.
      /// \param pShader Shader object. If NULL, default rendering pipeline is activated.
      void CommitShader( Phoenix::Graphics::CShader *pShader );
      ////////////////////
      /// Commits integer shader parameter. 
      /// \param shader Shader object where parameter is passed.
      /// \param strParamName Variable name in shader.
      /// \param iValue Integer parameter value.
      void CommitUniformShaderParam( Phoenix::Graphics::CShader &shader, const std::string &strParamName, int iValue );
      ////////////////////
      /// Commits float shader parameter. 
      /// \param shader Shader object where parameter is passed.
      /// \param strParamName Variable name in shader.
      /// \param fValue Float parameter value.
      void CommitUniformShaderParam( Phoenix::Graphics::CShader &shader, const std::string &strParamName, float fValue );
      ////////////////////
      /// Commits either uniform or vertex attribute shader parameter. 
      /// \param shader Shader object where parameter is passed.
      /// \param strParamName Variable name in shader.
      /// \param vValue VertexDescriptor with uniform or vertex attribute param values.
      void CommitShaderParam( Phoenix::Graphics::CShader &shader, const std::string &strParamName, 
			      const Phoenix::Graphics::CVertexDescriptor & vParam);
      ////////////////////
      /// Commits integer shader parameter. 
      /// \param shader Shader object where parameter is passed.
      /// \param strParamName Variable name in shader.
      /// \param iValue Integer parameter value.
      void CommitUniformShaderParam( Phoenix::Graphics::CShader &shader, const char *strParamName, int iValue );
      ////////////////////
      /// Commits float shader parameter. 
      /// \param shader Shader object where parameter is passed.
      /// \param strParamName Variable name in shader.
      /// \param fValue Float parameter value.
      void CommitUniformShaderParam( Phoenix::Graphics::CShader &shader, const char *strParamName, float fValue );
      ////////////////////
      /// Commits either uniform or vertex attribute shader parameter. 
      /// \param shader Shader object where parameter is passed.
      /// \param strParamName Variable name in shader.
      /// \param vValue VertexDescriptor with uniform or vertex attribute param values.
      void CommitShaderParam( Phoenix::Graphics::CShader &shader, const char *strParamName, 
			      const Phoenix::Graphics::CVertexDescriptor & vParam);

      ////////////////////
      /// Seeks shader parameter name.
      /// \param shader Shader object where parameter is passed.
      /// \param strParamName Variable name in shader.
      /// \returns -1 if not found, otherwise location.
      int LocateAttribShaderParam( Phoenix::Graphics::CShader &shader, const char *strParamName );
      ////////////////////
      /// Seeks shader parameter name. This is rather unnecessary, since CommitShaderParam will do 
      /// just this - but for completeness, it exists.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader
      /// \returns -1 if not found, otherwise location.
      int LocateUniformShaderParam( Phoenix::Graphics::CShader &shader, const char *strParamName );
      ////////////////////
      /// Commits vertex attribute.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader.
      /// \param fX First value.
      void CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX );
      ////////////////////
      /// Commits vertex attribute.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader
      /// \param fX First value.
      /// \param fY Second value.
      void CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX, float fY );
      ////////////////////
      /// Commits vertex attribute.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader
      /// \param fX First value.
      /// \param fY Second value.
      /// \param fZ Third value.
      void CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX, float fY, float fZ );
      ////////////////////
      /// Commits vertex attribute.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader
      /// \param fX First value.
      /// \param fY Second value.
      /// \param fZ Third value.
      /// \param fW Fourth value.
      void CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX, float fY, float fZ, float fW );
      ////////////////////
      /// Commits vertex attribute.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader
      /// \param vValues Attrib values.
      void CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, const Phoenix::Math::CVector4<float> &vValues );
      ////////////////////
      /// Commits vertex attribute.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader
      /// \param vValues Attrib values.
      void CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, const Phoenix::Math::CVector3<float> &vValues );
      ////////////////////
      /// Commits vertex attribute.
      /// \param shader Shader object where parameter is passed.
      /// \param iLoc Parameter location in shader
      /// \param vValues Attrib values.
      void CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, const Phoenix::Math::CVector2<float> &vValues );
      ////////////////////
      /// Commits light object parameters to given light unit.
      /// \param light Light reference.
      /// \param nLightId Light unit, accepted values 0-7, default 0.
      void CommitLight( const Phoenix::Graphics::CLight &light, unsigned int nLightId = 0);
      ////////////////////
      /// Disables light unit.
      /// \param nLightId Light unit to be disabled.
      void DisableLight ( unsigned int nLightId );
      ////////////////////
      /// Activates render state.
      /// \param tState Render state to be activated.
      void CommitState( STATE_TYPE tState);
      ////////////////////
      /// Disables render state.
      /// \param tState Render state to be disabled.
      void DisableState( STATE_TYPE tState);
      ////////////////////
      /// Commits material.
      /// \param material Material to be used.
      /// \param iFace For which face material is applied, 0 for front, non-zero for back. By default, front.
      void CommitMaterial( const Phoenix::Graphics::CMaterial &material, int iFace = 0 );
      ////////////////////
      /// Commits alpha comparison.
      /// \param tType Alpha test
      /// \param fReference Reference value, which fragment alpha is compared to. By default, it is zero.
      void CommitAlpha( ALPHA_TEST_TYPE tType, float fReference = 0.0f );
      ////////////////////
      /// Commits blending.
      /// \param tSource Source operation.
      /// \param tDestination Destination operation.
      void CommitBlending( BLEND_SRC_TYPE tSource, BLEND_DST_TYPE tDestination);
      ////////////////////
      /// Commits blending.
      /// \param rBlendingOp Blending operation.
      void CommitBlending( Phoenix::Graphics::CBlendingOperation & rBlendingOp );
      ////////////////////
      /// Commits skybox.
      /// \param skybox Skybox to be committed for rendering.
      /// \param camera Current camera where skybox will be applied.
      void CommitSkybox( Phoenix::Graphics::CSkybox & skybox, Phoenix::Graphics::CCamera &camera );
      ////////////////////
      /// Commits transform. Applies stack push of current modelview matrix.
      /// \param transform Transform to be applied.
      void CommitTransform( const Phoenix::Math::CTransform &transform );
      ////////////////////
      /// Rolls back transform. Pops modelview stack.
      void RollbackTransform();
      ////////////////////
      /// Renders a sphere.
      /// \param sphere Sphere to be rendered.
      /// \param bWireframe Is sphere rendered as wireframe. Default true.
      void CommitSphere( const Phoenix::Volume::CSphere &sphere, int bWireframe = 1 );
      ////////////////////
      /// Renders a quad with texcoords, color and normal. Vertices must be given in counter-clockwise order.
      /// \param vertexOne First vertex.
      /// \param vertexTwo Second vertex.
      /// \param vertexThree Third vertex.
      /// \param vertexFour Fourth vertex.
      void CommitQuad( const Phoenix::Spatial::CVertex &vertexOne,
		       const Phoenix::Spatial::CVertex &vertexTwo,
		       const Phoenix::Spatial::CVertex &vertexThree,
		       const Phoenix::Spatial::CVertex &vertexFour);
      ////////////////////
      /// Creates cache for VertexDescriptor, or updates existing if necessary.
      /// \param rVertexDescriptor Reference to VertexDescriptor.
      /// \param tType Cache access performance hint, by default CACHE_STATIC_DRAW ( data transfer strictly from app to GL )
      /// \returns zero on success.
      /// \returns non-zero on error.
      int CommitCache( Phoenix::Graphics::CVertexDescriptor & rVertexDescriptor, 
		       Phoenix::Graphics::CACHE_ACCESS_TYPE tType = Phoenix::Graphics::CACHE_STATIC_DRAW  );
      ////////////////////
      /// Creates cache for IndexArray, or updates existing if necessary.
      /// \param rIndexArray Reference to IndexArray.
      /// \param tType Cache access performance hint, by default CACHE_STATIC_DRAW ( data transfer strictly from app to GL )
      /// \returns zero on success.
      /// \returns non-zero on error.
      int CommitCache( Phoenix::Graphics::CIndexArray & rIndexArray, 
		       Phoenix::Graphics::CACHE_ACCESS_TYPE tType = Phoenix::Graphics::CACHE_STATIC_DRAW);
      ////////////////////
      /// Removes existing cache from VertexDescriptor.
      /// \param rVertexDescriptor Reference to VertexDescriptor.
      void RollbackCache( Phoenix::Graphics::CVertexDescriptor & rVertexDescriptor );
      ////////////////////
      /// Removes existing cache from IndexArray.
      /// \param rIndexArray Reference to IndexArray.
      void RollbackCache( Phoenix::Graphics::CIndexArray & rIndexArray );
      ////////////////////
      /// Disables possibly active cache buffers.
      void DisableCaches();
      ////////////////////
      /// Creates a framebuffer.
      /// \param nWidth Framebuffer width.
      /// \param nHeight Framebuffer height.
      /// \param iBufferFlags Which buffers should be included in the framebuffer, use binary OR with FBO_BUFFER_FLAG values.
      /// \returns Pointer to new Frame Buffer Object.
      Phoenix::Graphics::CFrameBufferObject * CreateFramebuffer( unsigned int nWidth, unsigned int nHeight, 
								 int iBufferFlags = 0 );
      ////////////////////
      /// Attaches texture to a framebuffer.
      /// \param rFBO Framebuffer where texture is attached to.
      /// \param hTexture Reference to texture which is attached.
      /// \param nColorBuffer To which colorbuffer this texture will be attahed. Default is first, 0.
      /// \returns Zero on success, non-zero on error.
      int AttachTextureToFramebuffer( Phoenix::Graphics::CFrameBufferObject & rFBO, 
				      const Phoenix::Default::TEXTURE_HANDLE & hTexture, 
				      unsigned int nColorBuffer = 0 );
      ////////////////////
      /// Attaches framebuffer for rendering to it.
      /// \warn Framebuffers cannot be used consequently, only one can be commited/rolled back at a time.
      /// \param rFBO Framebuffer to render to.
      /// \param nColorBufferCount Number of color buffers that will be rendered.  By default, only first one is rendered.
      /// \warn Each of the first #nColorBufferCount buffers must have a texture assigned!
      void CommitFrameBuffer( const Phoenix::Graphics::CFrameBufferObject & rFBO, unsigned int nColorBufferCount = 1);
      ////////////////////
      /// Attaches framebuffer for rendering to it.
      /// \warn Framebuffers cannot be used consequently, only one can be commited/rolled back at a time.
      /// \param rFBO Framebuffer to render to.
      /// \param nColorBuffer Which of the color buffers is used.
      void CommitFrameBufferSingle( const Phoenix::Graphics::CFrameBufferObject & rFBO, unsigned int nColorBuffer );
      ////////////////////
      /// Detaches (any) framebuffer from rendering.
      /// \param rFBO Framebuffer to detach.
      /// \warn Given FBO must be previously attached.
      void RollbackFrameBuffer( const Phoenix::Graphics::CFrameBufferObject & rFBO);
      ////////////////////
      /// Draws framebuffer-sized quad, assuming that view is orthogonally
      /// set from 0,0 to width, height.
      /// \param rFBO Framebuffer object where quad coordinates are retrieved.
      void CommitQuad( const Phoenix::Graphics::CFrameBufferObject & rFBO );
      ////////////////////
      /// Draws screen-sized quad, with texture coordinates from FBO dimensions.
      /// \param rCamera Camera used for current screen.
      /// \param rFBO Framebuffer, of which texture is used.
      void CommitQuad( const Phoenix::Graphics::CCamera & rCamera, const Phoenix::Graphics::CFrameBufferObject & rFBO );
      ////////////////////
      /// Creates new fontset from given TTF file.
      /// \param sPathToFontFile Path to TTF file.
      /// \param nFontSize Desired font size.
      /// \returns Pointer to CFontset or NULL on failure.
      CFontset * CreateFontset( const char *sPathToFontFile, unsigned int nFontSize);
      ////////////////////
      /// Renders given string into passed coordinates.
      /// \param fFontset Fontset to be used.
      /// \param fX Text lower left corner x coordinate.
      /// \param fY Text lower left corner y coordinate.
      /// \param szText Text itself.
      void CommitString( CFontset & rFontSet, float fX, float fY, const char *szText );
      ////////////////////
      /// Returns information about current rendering hardware and opengl.
      /// \returns Reference to features object.
      const COglRendererFeatures & GetFeatures();
      ////////////////////
      /// Returns current renderstate.
      /// \returns Reference to current renderstate.
      Phoenix::Graphics::CInternalRenderState & GetRenderState();
    };
    /////////////////////////////////////////////////////////////////  
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::COglRenderer::CommitColor( const CVector4<unsigned char> &vColor )
{
  glColor4ubv( vColor.GetArray());
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::COglRenderer::CommitColor( unsigned char bR, unsigned char bG, unsigned char bB, unsigned char bA)
{
  glColor4ub( bR,bG,bB,bA );
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Graphics::CInternalRenderState & 
Phoenix::Graphics::COglRenderer::GetRenderState()
{
  return m_RenderState;
}
/////////////////////////////////////////////////////////////////
#endif
