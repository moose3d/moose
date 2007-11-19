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
      STATE_BLENDING   = GL_BLEND
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
      int HasVertexProgram() const;
      /// Is vertex_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexShader() const;
      /// Is fragment_shader extension supported.
      /// \return 1 if supported, 0 otherwise.
      int HasFragmentShader() const;
      /// Is vertex_array supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexArray() const;
      /// Is vertex_buffer_object supported.
      /// \return 1 if supported, 0 otherwise.
      int HasVertexBufferObject() const;
      /// Is multitexture supported.
      /// \return 1 if supported, 0 otherwise.
      int HasMultitexture() const;
      /// Is shader_objects supported.
      /// \return 1 if supported, 0 otherwise.
      int HasShaderObjects() const;
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
      /// for printing out supported features.
      friend std::ostream &operator<<(std::ostream &stream, 
				      const COglRendererFeatures &obj);
    protected:
      /// Initializes the supported values.
      void Init();
    };
    
    /////////////////////////////////////////////////////////////////
    /// BufferType
    enum BUFFER_TYPE 
    { 
      COLOR_BUFFER = 0,
      DEPTH_BUFFER = 1
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
      /// Draws the elements from previously set arrays.
      /// \param pIndexBuffer which indices are used and what primitives will be created.
      void CommitPrimitive( Phoenix::Graphics::CIndexArray *pIndexBuffer );
      ////////////////////
      /// Sets color for drawing.
      /// \param vColor RGBA Color vector.
      void CommitColor( const Phoenix::Math::CVector4<unsigned char> &vColor );
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
      void DisableTexture( unsigned int nTexUnit, Phoenix::Graphics::COglTexture *pTexture );
      ////////////////////
      /// Creates new 2D texture from TGA image.
      /// \param strFilename filename for tga image.
      /// \returns Pointer to COglTexture.
      Phoenix::Graphics::COglTexture * CreateTexture( const std::string &strFilename );
      ////////////////////
      /// Creates new empty 2D texture.
      /// \param nWidth width of texture.
      /// \param nHeight height of texture.
      /// \param tType TEXTURE_TYPE 
      /// \returns Pointer to COglTexture.
      Phoenix::Graphics::COglTexture * CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType );
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
      /// Commits transform.
      /// \param transform Transform to be applied.
      void CommitTransform( const Phoenix::Math::CTransform &transform );
      ////////////////////
      /// Rolls back transform.
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
#endif
