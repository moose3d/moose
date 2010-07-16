#ifndef __MooseShader_h__
#define __MooseShader_h__
/////////////////////////////////////////////////////////////////
//#include <MooseRenderable.h>

#include "MooseAPI.h"
#if defined(MOOSE_APPLE_IPHONE)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(__APPLE__)
#include <GL/GLee.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/GLee.h>
#include <GL/gl.h>
#endif
#include <list>
#include <string>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    /// Class for programmable shaders.
    class MOOSE_API CShader
    {
    private:
      /// OpenGL program id.
      GLuint m_nShaderProgram;
      std::list<GLuint> m_lstVertexShaders;
      std::list<GLuint> m_lstFragmentShaders;
    public:
      ////////////////////
      /// Creates a program id (assuming that glContext is set up properly).
      CShader();
      ////////////////////
      /// Destructor.
      ~CShader();
      ////////////////////
      /// Assigns vertex shader.
      /// \param nVertexShader Vertex shader id. Must be created with glCreateShader( GL_VERTEX_SHADER );
      void AttachVertexShader( GLuint nVertexShader );
      ////////////////////
      /// Assigns fragment shader.
      /// \param nFragmentShader Fragment shader id. Must be created with glCreateShader( GL_FRAGMENT_SHADER );
      void AttachFragmentShader( GLuint nFragmentShader );
      ////////////////////
      /// Returns program id.
      /// \returns Program number.
      inline unsigned int GetProgram() const 
      { 
	return m_nShaderProgram; 
      }
      ////////////////////
      /// Links shader.
      bool Link();
      void LoadVertexShader( const std::string & strVertexShader );
      void LoadVertexShader( const char * szVertexShader );
      void LoadFragmentShader( const std::string & strFragmentShader );
      void LoadFragmentShader( const char * szFragmentShader );
      void CreateVertexShaderFromSource( const char * szVertexShaderCode, const char * szVSname );
      void CreateFragmentShaderFromSource( const char * szFragmentShaderCode, const char * szFSname);
      bool Validate();
      void Apply();
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
