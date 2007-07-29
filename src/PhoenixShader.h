#ifndef __PhoenixShader_h__
#define __PhoenixShader_h__
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    ////////////////////
    /// Class for programmable shaders.
    class CShader
    {
    private:
      /// OpenGL program id.
      unsigned int m_nShaderProgram;
      /// Vertex shader id.
      unsigned int m_nVertexShader;
      /// Fragment shader id.
      unsigned int m_nFragmentShader;
      /// Is fragment shader used.
      int m_bIsFragShader;
      /// Is vertex shader used.
      int m_bIsVertexShader;
    public:
      ////////////////////
      /// Constructor.
      /// \param nProgramId Shader program id. Must be created with glCreateProgram.
      CShader( unsigned int nProgramId );
      ////////////////////
      /// Destructor.
      ~CShader();
      ////////////////////
      /// Assigns vertex shader.
      /// \param nVertexShader Vertex shader id. Must be created with glCreateShader( GL_VERTEX_SHADER );
      void SetVertexShader( unsigned int nVertexShader );
      ////////////////////
      /// Assigns fragment shader.
      /// \param nFragmentShader Fragment shader id. Must be created with glCreateShader( GL_FRAGMENT_SHADER );
      void SetFragmentShader( unsigned int nFragmentShader );
      ////////////////////
      /// Returns program id.
      /// \returns Program number.
      inline unsigned int GetProgram() const 
      { 
	return m_nShaderProgram; 
      }
      ////////////////////
      /// Returns vertex shader id.
      /// \returns Vertex shader number.
      inline unsigned int GetVertexShader() const
      {
	return m_nVertexShader;
      }
      ////////////////////
      /// Returns fragment shader id.
      /// \returns Fragment shader number.
      inline unsigned int GetFragmentShader() const
      {
	return m_nFragmentShader;
      }
    };
    /// Shader parameter types
    enum SHADER_PARAM_TYPE
    {
      SHADER_PARAM_U1F = 0,
      SHADER_PARAM_U2F,
      SHADER_PARAM_U3F,
      SHADER_PARAM_U4F,
      SHADER_PARAM_U1I,
      SHADER_PARAM_U2I,
      SHADER_PARAM_U3I,
      SHADER_PARAM_U4I
      SHADER_PARAM_U1FV,
      SHADER_PARAM_U2FV,
      SHADER_PARAM_U3FV,
      SHADER_PARAM_U4FV,
      SHADER_PARAM_U1IV,
      SHADER_PARAM_U2IV,
      SHADER_PARAM_U3IV,
      SHADER_PARAM_U4IV,
      SHADER_PARAM_U2X2M,
      SHADER_PARAM_U3X3M,
      SHADER_PARAM_U4X4M
    };
    ////////////////////
    /// Shader parameter class.
    class CShaderParam 
    {
    private:
      void *m_pData;
      size_t m_nCount;
      SHADER_PARAM_TYPE m_tType;
    public:
      CShaderParam( SHADER_PARAM_TYPE tType, size_t nCount );
      ~CShaderParam();
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
