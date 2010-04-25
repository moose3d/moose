#include "PhoenixShader.h"
#include <GL/GLee.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShader::CShader( unsigned int nProgramId ) : m_nShaderProgram(nProgramId), 
								 m_nVertexShader(0), 
								 m_nFragmentShader(0), 
								 m_bIsFragShader(0),
								 m_bIsVertexShader(0)
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShader::~CShader()
{
  // detach shaders from program.
  if ( m_bIsVertexShader )
  {
    glDetachShader( GetProgram(), GetVertexShader() );
    glDeleteShader( GetVertexShader() );
  }
  if ( m_bIsFragShader ) 
  {
    glDetachShader( GetProgram(), GetFragmentShader() );
    glDeleteShader( GetFragmentShader() );
  }
  // delete program
  glDeleteProgram( GetProgram() );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::CShader::SetVertexShader(unsigned int nVertexShader ) 
{
  m_nVertexShader = nVertexShader;
  m_bIsVertexShader = 1;
  glAttachShader( GetProgram(), GetVertexShader());
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CShader::SetFragmentShader(unsigned int nFragmentShader ) 
{
  m_nFragmentShader = nFragmentShader;
  m_bIsFragShader = 1;
  glAttachShader( GetProgram(), GetFragmentShader());
}
/////////////////////////////////////////////////////////////////
