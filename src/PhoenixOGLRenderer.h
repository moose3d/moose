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
      STATE_LIGHTING = 0
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
      void CommitColor( Phoenix::Math::CVector4<unsigned char> &vColor );
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
      Phoenix::Graphics::CShader * CreateShader( std::string strVertexShader, std::string strFragmentShader );
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
    };
    /////////////////////////////////////////////////////////////////  
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::COglRenderer::CommitColor( CVector4<unsigned char> &vColor )
{
  glColor4ubv( vColor.GetArray());
}
/////////////////////////////////////////////////////////////////
#endif
