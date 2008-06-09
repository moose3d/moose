#ifndef __PhoenixShader_h__
#define __PhoenixShader_h__
/////////////////////////////////////////////////////////////////
//#include <PhoenixRenderable.h>
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
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
