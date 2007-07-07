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
Phoenix::Graphics::COglRenderer::CommitVertexBuffer( CVertexBuffer *pBuffer )
{
  switch( pBuffer->GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer(3, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    break;
  case ELEMENT_TYPE_NULL:
    glDisableClientState( GL_VERTEX_ARRAY );
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::DrawPrimitive( CIndexBuffer *pIndexBuffer )
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
		    GL_UNSIGNED_SHORT, pIndexBuffer->GetShortIndices());
  }
  else
  {
    glDrawElements( glPrimitive, pIndexBuffer->GetNumIndices(), 
		    GL_UNSIGNED_INT, pIndexBuffer->GetIntIndices());
  }
}
/////////////////////////////////////////////////////////////////
