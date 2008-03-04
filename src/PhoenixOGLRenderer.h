/////////////////////////////////////////////////////////////////
#ifndef __PhoenixOGLRenderer_h__
#define __PhoenixOGLRenderer_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixVertexDescriptor.h"
#include "PhoenixIndexArray.h"
#include "PhoenixVector4.h"
#include "PhoenixCamera.h"
#include "PhoenixTexture.h"
#include "PhoenixModel.h"
#include "PhoenixShader.h"
#include "PhoenixLight.h"
#include "PhoenixMaterial.h"
#include "PhoenixSkybox.h"
#include "PhoenixTransform.h"
#include "PhoenixSpatial.h"
#include "PhoenixFrameBuffer.h"
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    enum CLIENT_STATE_TYPE 
    {
      CLIENT_STATE_VERTEX_ARRAY = 0,
      CLIENT_STATE_COLOR_ARRAY = 1,
      CLIENT_STATE_TEX0_ARRAY,
      CLIENT_STATE_TEX1_ARRAY,
      CLIENT_STATE_TEX2_ARRAY,
      CLIENT_STATE_TEX3_ARRAY,
      CLIENT_STATE_TEX4_ARRAY,
      CLIENT_STATE_TEX5_ARRAY,
      CLIENT_STATE_TEX6_ARRAY,
      CLIENT_STATE_TEX7_ARRAY
    };
    enum STATE_TYPE
    {
      STATE_LIGHTING = GL_LIGHTING,
      STATE_DEPTH_TEST = GL_DEPTH_TEST,
      STATE_ALPHA_TEST = GL_ALPHA_TEST,
      STATE_BLENDING   = GL_BLEND,
      STATE_DEPTH_WRITE = GL_TRUE,
      STATE_FACECULLING = GL_CULL_FACE
    };
    enum ALPHA_TEST_TYPE
    {
      ALPHA_ALWAYS           = GL_ALWAYS,
      ALPHA_NEVER	     = GL_NEVER,
      ALPHA_LESS	     = GL_LESS,
      ALPHA_EQUAL	     = GL_EQUAL,
      ALPHA_LESS_OR_EQUAL    = GL_LEQUAL,
      ALPHA_GREATER	     = GL_GREATER,
      ALPHA_NOT_EQUAL	     = GL_NOTEQUAL,
      ALPHA_GREATER_OR_EQUAL = GL_GEQUAL
    };
    enum BLEND_SRC_TYPE
    {
      BLEND_SRC_ZERO                = GL_ZERO,     
      BLEND_SRC_ONE                 = GL_ONE,
      BLEND_SRC_DST_COLOR           = GL_DST_COLOR,
      BLEND_SRC_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
      BLEND_SRC_SRC_ALPHA           = GL_SRC_ALPHA,
      BLEND_SRC_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
      BLEND_SRC_DST_ALPHA           = GL_DST_ALPHA,
      BLEND_SRC_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
      BLEND_SRC_SRC_ALPHA_SATURATE  = GL_SRC_ALPHA_SATURATE
    };
    enum BLEND_DST_TYPE
    {
      BLEND_DST_ZERO		    = GL_ZERO,
      BLEND_DST_ONE		    = GL_ONE,
      BLEND_DST_SRC_COLOR	    = GL_SRC_COLOR,
      BLEND_DST_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
      BLEND_DST_SRC_ALPHA	    = GL_SRC_ALPHA,
      BLEND_DST_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,    
      BLEND_DST_DST_ALPHA	    = GL_DST_ALPHA,
      BLEND_DST_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA
    };
    /////////////////////////////////////////////////////////////////
    /// BufferType
    enum BUFFER_TYPE 
    { 
      COLOR_BUFFER = 0,
      DEPTH_BUFFER = 1
    };
    /////////////////////////////////////////////////////////////////
    /// Frame buffer flags
    enum FBO_BUFFER_FLAG
    {
      FBO_COLOR_BUFFER = 1 << 0,
      FBO_DEPTH_BUFFER = 1 << 1
    };
    ////////////////////
    /// Texture formats
    enum TEXTURE_FORMAT
    {
      TEX_FORMAT_RGBA = GL_RGBA,
      TEX_FORMAT_RGB  = GL_RGB,
      TEX_FORMAT_DEPTH_COMPONENT = GL_DEPTH_COMPONENT
    };
    
    /////////////////////////////////////////////////////////////////
    /// Class for grouping blending operations. 
    class CBlendingOperation : public Phoenix::Core::CEnableable
    {
    private:
      BLEND_SRC_TYPE m_tBlendSrcType;
      BLEND_DST_TYPE m_tBlendDstType;
    public:
      ////////////////////
      /// Constructor. 
    CBlendingOperation() : Phoenix::Core::CEnableable(), m_tBlendSrcType(BLEND_SRC_ONE), m_tBlendDstType(BLEND_DST_ZERO) {}
      ////////////////////
      /// Assigns src operation for blending.
      /// \param tType Blending source operation;
      inline void SetSourceOperation( BLEND_SRC_TYPE tType)
      {
	m_tBlendSrcType = tType;
      }
      ////////////////////
      /// Assigns dest operation for blending.
      /// \param tType Blending dest operation;
      inline void SetDestinationOperation( BLEND_DST_TYPE tType)
      {
	m_tBlendDstType = tType;
      }
      ////////////////////
      /// Assigns operations for blending.
      /// \param tSource Source operation.
      /// \param tDestination Destination operation.
      inline void SetOperation( BLEND_SRC_TYPE tSource, BLEND_DST_TYPE tDestination )
      {
	m_tBlendSrcType = tSource;
	m_tBlendDstType = tDestination;
      }
    };
    /////////////////////////////////////////////////////////////////
    /// Class grouping alpha testing properties.
    class CAlphaTestOperation : public Phoenix::Core::CEnableable
    {
    private:
      ALPHA_TEST_TYPE m_tTestType;
      float	      m_fRefValue;
    public:
      ////////////////////
      /// Constructor.
    CAlphaTestOperation() : m_tTestType(ALPHA_ALWAYS), m_fRefValue(0.0f) {}
      ////////////////////
      /// Assigns test type.
      /// \param tType ALPHA_TEST_TYPE.
      inline void SetTest( ALPHA_TEST_TYPE tType )
      { 
	m_tTestType = tType; 
      }
      ////////////////////
      /// Returns current alpha test.
      /// \returns Current ALPHA_TEST_TYPE.
      inline ALPHA_TEST_TYPE GetTest() const 
      { 
	return m_tTestType; 
      }
      ////////////////////
      /// Assigns reference value.
      /// \param fValue Reference value to be set.
      inline void SetReference( float fValue )
      {
	m_fRefValue = fValue;
      }
      ////////////////////
      /// Returns reference value.
      /// \brief Returns current reference value.
      inline float GetReference () const
      {
	return m_fRefValue;
      }
    };
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
      /// framebuffer object support flag.
      int m_bEXT_framebuffer_object;
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
      /// \return 1 if supported, 0 otherwise.
      int HasVertexProgram() const;
      ////////////////////
      /// Is vertex_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexShader() const;
      ////////////////////
      /// Is fragment_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasFragmentShader() const;
      ////////////////////
      /// Is vertex_array supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexArray() const;
      ////////////////////
      /// Is vertex_buffer_object supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexBufferObject() const;
      ////////////////////
      /// Is multitexture supported.
      /// \return 1 if supported, 0 otherwise.
      int HasMultitexture() const;
      ////////////////////
      /// Is shader_objects supported.
      /// \return 1 if supported, 0 otherwise.
      int HasShaderObjects() const;
      ////////////////////
      /// Is EXT_framebuffer_object supported.
      /// \return 1 if supported, 0 otherwise.
      int HasFramebufferObjects() const;
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
      void CommitVertex ( const Phoenix::Spatial::CVertex & rVertex );
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
      /// Renders a complete model.
      /// \param model Model object. 
      void CommitModel( Phoenix::Graphics::CModel &model );
      ////////////////////
      /// Commits a texture filter.
      /// \param tFilter Which filter will be applied.
      /// \param tType TEXTURE_TYPE.
      void CommitFilter( Phoenix::Graphics::TEXTURE_FILTER tFilter, Phoenix::Graphics::TEXTURE_TYPE tType );
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
      /// \returns Pointer to shader object. NULL if shader could not be created.
      Phoenix::Graphics::CShader * CreateShaderFromSource( const char * szVertexShaderCode, const char * szFragmentShaderCode);
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
      /// \returns zero on success.
      /// \returns non-zero on error.
      int CommitCache( Phoenix::Graphics::CVertexDescriptor & rVertexDescriptor );
      ////////////////////
      /// Creates cache for IndexArray, or updates existing if necessary.
      /// \param rIndexArray Reference to IndexArray.
      /// \returns zero on success.
      /// \returns non-zero on error.
      int CommitCache( Phoenix::Graphics::CIndexArray & rIndexArray );
      ////////////////////
      /// Removes existing cache from VertexDescriptor.
      /// \param rVertexDescriptor Reference to VertexDescriptor.
      void RollbackCache( Phoenix::Graphics::CVertexDescriptor & rVertexDescriptor );
      ////////////////////
      /// Removes existing cache from IndexArray.
      /// \param rIndexArray Reference to IndexArray.
      void RollbackCache( Phoenix::Graphics::CIndexArray & rIndexArray );
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
#endif
