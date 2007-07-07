/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <list>
#include <iostream>
#include "PhoenixGlobals.h"
#include "PhoenixOGLRenderer.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics; 
using std::endl;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglRendererFeatures::COglRendererFeatures()
{
Init();
  // Check for required extensions:
  if ( GLEE_ARB_vertex_program	)  m_bARB_vertex_program  = 1;
  if ( GLEE_ARB_vertex_shader	)  m_bARB_vertex_shader   = 1;
  if ( GLEE_ARB_fragment_shader )  m_bARB_fragment_shader = 1;
  if ( GLEE_ARB_shader_objects	)  m_bARB_shader_objects  = 1;
  if ( GLEE_EXT_vertex_array	)  m_bEXT_vertex_array    = 1;
  if ( GLEE_ARB_multitexture	)  m_bARB_multitexture     = 1;

  if ( GLEE_ARB_vertex_buffer_object	) m_bARB_vertex_buffer_object = 1;
  glGetIntegerv( GL_MAX_LIGHTS,            &m_iMaxLights );
  glGetIntegerv( GL_MAX_ELEMENTS_VERTICES, &m_iMaxElementsVertices);
  glGetIntegerv( GL_MAX_ELEMENTS_INDICES,  &m_iMaxElementsIndices);  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRendererFeatures::Init()
{
  m_bARB_vertex_program = 0;
  m_bARB_vertex_shader = 0;
  m_bARB_fragment_shader = 0;
  m_bEXT_vertex_array = 0;
  m_bARB_vertex_buffer_object = 0;
  m_bARB_multitexture = 0;
  m_bARB_shader_objects = 0;
  m_iMaxLights = 0;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRendererFeatures::HasVertexProgram() const
{
  return m_bARB_vertex_program;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRendererFeatures::HasVertexShader() const
{
  return m_bARB_vertex_shader;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRendererFeatures::HasFragmentShader() const
{
  return m_bARB_fragment_shader;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRendererFeatures::HasVertexArray() const
{
  return m_bEXT_vertex_array;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRendererFeatures::HasVertexBufferObject() const
{
  return m_bARB_vertex_buffer_object;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRendererFeatures::HasMultitexture() const
{
  return m_bARB_multitexture;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRendererFeatures::HasShaderObjects() const
{
  return m_bARB_shader_objects;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::COglRendererFeatures::GetMaxLights() const
{
  return m_iMaxLights;
}
/////////////////////////////////////////////////////////////////
int  
Phoenix::Graphics::COglRendererFeatures::GetMaxElementsVertices() const
{
  return m_iMaxElementsVertices;
}
/////////////////////////////////////////////////////////////////
int  
Phoenix::Graphics::COglRendererFeatures::GetMaxElementsIndices() const
{
  return m_iMaxElementsIndices;
}
/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<(std::ostream &stream, const COglRendererFeatures &obj)
{
  stream << "OpenGL extensions:" << std::endl;
  stream << "------------------" << std::endl;
  stream << "GL_ARB_vertex_program "  << ( obj.HasVertexProgram() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_vertex_shader "   << ( obj.HasVertexShader() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_fragment_shader " << ( obj.HasFragmentShader() ? "YES" : "NO" ) << endl;
  stream << "GL_EXT_vertex_array "    << ( obj.HasVertexArray() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_vertex_buffer_object " << ( obj.HasVertexBufferObject() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_multitexture " << ( obj.HasMultitexture() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_shader_objects " << ( obj.HasShaderObjects() ? "YES" : "NO" ) << endl;
  stream << "------------------" << std::endl;
  stream << "OpenGL environment variables:" << std::endl;
  stream << "------------------" << std::endl;
  stream << "GL_MAX_LIGHTS = " << obj.GetMaxLights() << endl;
  stream << "GL_MAX_ELEMENTS_VERTICES = " << obj.GetMaxElementsVertices() << std::endl;
  stream << "GL_MAX_ELEMENTS_INDICES  = " << obj.GetMaxElementsIndices() << std::endl;
  return stream;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglRenderer::COglRenderer()
{
  m_pFeatures = new COglRendererFeatures();
  std::cerr << "OpenGL information:" << std::endl << *m_pFeatures << std::endl;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglRenderer::~COglRenderer()
{
  delete m_pFeatures;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::ClearBuffer(BUFFER_TYPE tType)
{
  switch( tType )
  {
  case COLOR_BUFFER:
    glClear(GL_COLOR_BUFFER_BIT);
    break;
  case DEPTH_BUFFER:
    glClear(GL_DEPTH_BUFFER_BIT);
    break;
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::Finalize()
{
  SDL_GL_SwapBuffers();
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexDescriptor( CVertexDescriptor *pBuffer )
{
  switch( pBuffer->GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer(3, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    break;
  case ELEMENT_TYPE_COLOR_4UB:
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, pBuffer->GetPointer<unsigned char>());
    break;
  case ELEMENT_TYPE_NULL:
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitPrimitive( CIndexArray *pIndexBuffer )
{

  GLenum glPrimitive = GL_POINTS;
  ////////////////////
  switch ( pIndexBuffer->GetPrimitiveType())
  {
  case PRIMITIVE_NULL:
    /// NOP
    return;
  case PRIMITIVE_POINT_LIST:
    glPrimitive = GL_POINTS;
    break;
  case PRIMITIVE_TRI_LIST:
    glPrimitive = GL_TRIANGLES;
    break;
  case PRIMITIVE_TRI_STRIP:
    glPrimitive = GL_TRIANGLE_STRIP;
    break;
  case PRIMITIVE_LINE_LIST:
    glPrimitive = GL_LINES;
    break;
  case PRIMITIVE_LINE_STRIP:
    glPrimitive = GL_LINE_STRIP;
    break;
  case PRIMITIVE_QUAD_LIST:
    glPrimitive = GL_QUADS;
    break;
  }
  ////////////////////
  // Check data type and render.
  if ( pIndexBuffer->IsShortIndices() )   
  {
    glDrawElements( glPrimitive, pIndexBuffer->GetNumIndices(), 
		    GL_UNSIGNED_SHORT, 
		    pIndexBuffer->GetPointer<unsigned short int>());
  }
  else
  {
    glDrawElements( glPrimitive, pIndexBuffer->GetNumIndices(), 
		    GL_UNSIGNED_INT, 
		    pIndexBuffer->GetPointer<unsigned int>());
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitColor( CVector4<unsigned char> &vColor )
{
  glColor4ubv( vColor.GetArray());
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::DisableClientState( CLIENT_STATE_TYPE tType )
{
  switch ( tType)
  {
  case CLIENT_STATE_VERTEX_ARRAY:
    glDisableClientState( GL_VERTEX_ARRAY );
    break;
  case CLIENT_STATE_COLOR_ARRAY:
    glDisableClientState( GL_COLOR_ARRAY );
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::EnableClientState( CLIENT_STATE_TYPE tType )
{
  switch ( tType)
  {
  case CLIENT_STATE_VERTEX_ARRAY:
    glEnableClientState( GL_VERTEX_ARRAY );
    break;
  case CLIENT_STATE_COLOR_ARRAY:
    glEnableClientState( GL_COLOR_ARRAY );
    break;
  }
}
/////////////////////////////////////////////////////////////////

