////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#include <list>
#include <iostream>
#include "PhoenixGlobals.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixTGAImage.h"
#include "PhoenixDDSImage.h"
#include "PhoenixDefaultEntities.h"
#include <fstream>
#include <ft2build.h>
// include freetype stuff
#include FT_FREETYPE_H
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics; 
using namespace Phoenix::Default; 
using namespace Phoenix::Spatial;
using std::endl;
using std::cerr;
using std::ifstream;
using std::ios;
/////////////////////////////////////////////////////////////////
#define BUFFER_OFFSET(i) ((char *)NULL+(i))
/////////////////////////////////////////////////////////////////
/// Famous last words: Eight color buffers is enough for anyone :)
const GLenum g_ColorBufferNames[] = { GL_COLOR_ATTACHMENT0_EXT, 
				      GL_COLOR_ATTACHMENT1_EXT,
				      GL_COLOR_ATTACHMENT2_EXT,
				      GL_COLOR_ATTACHMENT3_EXT,
				      GL_COLOR_ATTACHMENT4_EXT,
				      GL_COLOR_ATTACHMENT5_EXT,
				      GL_COLOR_ATTACHMENT6_EXT,
				      GL_COLOR_ATTACHMENT7_EXT };
/////////////////////////////////////////////////////////////////
#define DISABLE_ALL_TEXTURES(){			\
    glDisable(GL_TEXTURE_2D);			\
    glDisable(GL_TEXTURE_CUBE_MAP);		\
    glDisable(GL_TEXTURE_RECTANGLE_ARB);	\
}
/////////////////////////////////////////////////////////////////
/// Returns corresponding opengl texture type.
inline GLenum 
GetGLTextureType( const TEXTURE_TYPE &tType )
{
  GLenum iRetval = GL_TEXTURE_2D;
  switch ( tType )
  {
  case TEXTURE_2D:
  case TEXTURE_DEPTH2D:
    iRetval = GL_TEXTURE_2D;
    break;
  case TEXTURE_RECT:
    iRetval = GL_TEXTURE_RECTANGLE_ARB;
    break;
  case TEXTURE_CUBE:
    iRetval = GL_TEXTURE_CUBE_MAP;
    break;
  }
  return iRetval;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFontset::CFontset() : m_nDisplayLists(0)
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFontset::~CFontset()
{
  
  // delete textures
  delete [] m_ppTextures;
// Release lists
  glDeleteLists( GetDisplayList(), Phoenix::Globals::MAX_FONT_CHARACTERS );
}
/////////////////////////////////////////////////////////////////
GLuint &
Phoenix::Graphics::CFontset::GetDisplayList()
{
  return m_nDisplayLists;
}
/////////////////////////////////////////////////////////////////
const GLuint &
Phoenix::Graphics::CFontset::GetDisplayList() const
{
  return m_nDisplayLists;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture **
Phoenix::Graphics::CFontset::GetTextures()
{
  return m_ppTextures;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglRendererFeatures::COglRendererFeatures()
{
  Init();
  // Check for required extensions:
  m_bARB_vertex_program  = GLEE_ARB_vertex_program;
  m_bARB_vertex_shader   = GLEE_ARB_vertex_shader;
  m_bARB_fragment_shader = GLEE_ARB_fragment_shader;
  m_bARB_shader_objects  = GLEE_ARB_shader_objects;
  m_bEXT_vertex_array    = GLEE_EXT_vertex_array;
  m_bARB_multitexture    = GLEE_ARB_multitexture;

  m_bARB_vertex_buffer_object     = GLEE_ARB_vertex_buffer_object;
  m_bEXT_framebuffer_object       = GLEE_EXT_framebuffer_object;
  m_bEXT_texture_compression_s3tc = GLEE_EXT_texture_compression_s3tc;

  glGetIntegerv( GL_MAX_LIGHTS,            &m_iMaxLights );
  glGetIntegerv( GL_MAX_ELEMENTS_VERTICES, &m_iMaxElementsVertices);
  glGetIntegerv( GL_MAX_ELEMENTS_INDICES,  &m_iMaxElementsIndices);  
  glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS_EXT, &m_iMaxColorAttachments );
  glGetIntegerv( GL_MAX_DRAW_BUFFERS,      &m_iMaxDrawBuffers );
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
  m_bEXT_framebuffer_object = 0;

  m_iMaxLights = 0;
  m_iMaxElementsVertices = 0;
  m_iMaxElementsIndices = 0;
  m_iMaxColorAttachments = 0;
  m_iMaxDrawBuffers = 0;
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Graphics::COglRendererFeatures::HasVertexProgram() const
{
  return m_bARB_vertex_program;
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Graphics::COglRendererFeatures::HasVertexShader() const
{
  return m_bARB_vertex_shader;
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Graphics::COglRendererFeatures::HasFragmentShader() const
{
  return m_bARB_fragment_shader;
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Graphics::COglRendererFeatures::HasVertexArray() const
{
  return m_bEXT_vertex_array;
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Graphics::COglRendererFeatures::HasVertexBufferObject() const
{
  return m_bARB_vertex_buffer_object;
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Graphics::COglRendererFeatures::HasMultitexture() const
{
  return m_bARB_multitexture;
}
/////////////////////////////////////////////////////////////////
bool 
Phoenix::Graphics::COglRendererFeatures::HasShaderObjects() const
{
  return m_bARB_shader_objects;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Graphics::COglRendererFeatures::HasFramebufferObjects() const
{
  return m_bEXT_framebuffer_object;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Graphics::COglRendererFeatures::HasTextureCompressionS3TC() const
{
  return m_bEXT_texture_compression_s3tc;
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
int
Phoenix::Graphics::COglRendererFeatures::GetMaxColorAttachments() const
{
  return m_iMaxColorAttachments;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::COglRendererFeatures::GetMaxDrawBuffers() const
{
  return m_iMaxDrawBuffers;
}
/////////////////////////////////////////////////////////////////
const char *
Phoenix::Graphics::COglRendererFeatures::GetVendor() const
{
  return reinterpret_cast<const char*>(glGetString( GL_VENDOR ));
}
/////////////////////////////////////////////////////////////////
const char *
Phoenix::Graphics::COglRendererFeatures::GetRenderer() const
{
  return reinterpret_cast<const char *>(glGetString( GL_RENDERER ));
}
/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<(std::ostream &stream, const COglRendererFeatures &obj)
{
  stream << "OpenGL vendor:" << obj.GetVendor() << std::endl;
  stream << "OpenGL renderer:" << obj.GetRenderer() << std::endl;
  stream << "OpenGL extensions:" << std::endl;
  stream << "------------------" << std::endl;
  stream << "GL_ARB_vertex_program "  << ( obj.HasVertexProgram() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_vertex_shader "   << ( obj.HasVertexShader() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_fragment_shader " << ( obj.HasFragmentShader() ? "YES" : "NO" ) << endl;
  stream << "GL_EXT_vertex_array "    << ( obj.HasVertexArray() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_vertex_buffer_object " << ( obj.HasVertexBufferObject() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_multitexture " << ( obj.HasMultitexture() ? "YES" : "NO" ) << endl;
  stream << "GL_ARB_shader_objects " << ( obj.HasShaderObjects() ? "YES" : "NO" ) << endl;
  stream << "GL_EXT_framebuffer_object " << ( obj.HasFramebufferObjects() ? "YES" : "NO" ) << endl;
  
  stream << "------------------" << std::endl;
  stream << "OpenGL environment variables:" << std::endl;
  stream << "------------------" << std::endl;
  stream << "GL_MAX_LIGHTS = " << obj.GetMaxLights() << endl;
  stream << "GL_MAX_ELEMENTS_VERTICES = " << obj.GetMaxElementsVertices() << std::endl;
  stream << "GL_MAX_ELEMENTS_INDICES  = " << obj.GetMaxElementsIndices() << std::endl;
  stream << "GL_MAX_COLOR_ATTACHMENTS = " << obj.GetMaxColorAttachments() << std::endl;
  stream << "GL_MAX_DRAW_BUFFERS  = " << obj.GetMaxDrawBuffers() << std::endl;
  // GLenum err;
//   int iNumFormats = -1;
//   glGetIntegerv( GL_NUM_COMPRESSED_TEXTURE_FORMATS, &iNumFormats );
//   stream << "GL_NUM_COMPRESSED_TEXTURE_FORMATS  = " << iNumFormats << std::endl;
//   while ( (err = glGetError()) != GL_NO_ERROR )
//   {
//     stream << "ERR: " << gluErrorString( err ) << endl;
//   }
//   if ( iNumFormats > 0 )
//   {
//     GLint *aFormatEnums = new GLint[iNumFormats];
//     glGetIntegerv( GL_COMPRESSED_TEXTURE_FORMATS,     aFormatEnums);
    
//     for( int i=0;i<iNumFormats; ++i)
//     {
//       stream << "format " << aFormatEnums[i] << endl;
//       while ( (err = glGetError()) != GL_NO_ERROR )
//       {
// 	stream << "ERR: " << gluErrorString( err ) << endl;
//       }
//     }
//   }
  return stream;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglRenderer::COglRenderer() : m_pFeatures(NULL), m_pCamera(NULL),  m_pQuadric(NULL)
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglRenderer::~COglRenderer()
{
  if ( m_pFeatures )  delete m_pFeatures;
  if ( m_pQuadric  )  gluDeleteQuadric(m_pQuadric);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::ClearBuffer(Phoenix::Graphics::BUFFER_TYPE tType)
{
  switch( tType )
  {
  case Phoenix::Graphics::COLOR_BUFFER:
    glClear(GL_COLOR_BUFFER_BIT);
    break;
  case Phoenix::Graphics::DEPTH_BUFFER:
    glClear(GL_DEPTH_BUFFER_BIT);
    break;
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::Finalize()
{
  //glFinish();
  // disable textures, these tend to make nasty problems.
  for( int i=0;i<TEXTURE_HANDLE_COUNT;i++)
  {
    glActiveTextureARB( GL_TEXTURE0_ARB+i);
    DISABLE_ALL_TEXTURES();

  }
  // Reset camera.
  m_pCamera = NULL;
  // Remember call buffer swapping from another source.
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexDescriptor( CVertexDescriptor *pBuffer, unsigned int nId )
{
  // implemantation does not take into account if VBO is not supported.
  
  switch( pBuffer->GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
    glEnableClientState( GL_VERTEX_ARRAY );
    // check if this was previously set
    if ( GetRenderState().IsCurrentVertices( pBuffer ) ) break;

    if ( pBuffer->IsCached()) 
    {

      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glVertexPointer( 3, GL_FLOAT, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glVertexPointer(3, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    }

    GetRenderState().SetCurrentVertices(pBuffer);

    break;
  case ELEMENT_TYPE_COLOR_4UB:
    glEnableClientState( GL_COLOR_ARRAY );

    // check if this was previously set
    //if ( GetRenderState().m_pColorBuffer == pBuffer ) break;
    //else GetRenderState().m_pColorBuffer = pBuffer;

    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, pBuffer->GetPointer<unsigned char>());
    }
    break;
  case ELEMENT_TYPE_COLOR_3F:
    glEnableClientState( GL_COLOR_ARRAY );
    // check if this was previously set
    //if ( GetRenderState().m_pColorBuffer == pBuffer ) break;
    //else GetRenderState().m_pColorBuffer = pBuffer;

    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glColorPointer(3, GL_FLOAT, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glColorPointer(3, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    }
    break;
  case ELEMENT_TYPE_COLOR_4F:
    glEnableClientState( GL_COLOR_ARRAY );
    // check if this was previously set
    //if ( GetRenderState().m_pColorBuffer == pBuffer ) break;
    //else GetRenderState().m_pColorBuffer = pBuffer;

    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glColorPointer(4, GL_FLOAT, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glColorPointer(4, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    }
    break;
  case ELEMENT_TYPE_TEX_2F:
    if ( nId < TEXTURE_HANDLE_COUNT ) { glClientActiveTextureARB( GL_TEXTURE0_ARB + nId); }
    else                              { glClientActiveTextureARB( GL_TEXTURE0_ARB);       }

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    // check if this was previously set
    if ( GetRenderState().IsCurrentTexCoord( nId, pBuffer ) ) break;


    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glTexCoordPointer(2, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    }
    GetRenderState().SetCurrentTexCoord(nId,  pBuffer );
    break;
  case ELEMENT_TYPE_TEX_3F:
    if ( nId < TEXTURE_HANDLE_COUNT ) { glClientActiveTextureARB( GL_TEXTURE0_ARB + nId); }
    else                              { glClientActiveTextureARB( GL_TEXTURE0_ARB);       }

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    // check if this was previously set
    if ( GetRenderState().IsCurrentTexCoord( nId, pBuffer ) ) break;
    

    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glTexCoordPointer(3, GL_FLOAT, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glTexCoordPointer(3, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    }
    GetRenderState().SetCurrentTexCoord( nId, pBuffer );
    break;
  case ELEMENT_TYPE_TEX_4F:
    if ( nId < TEXTURE_HANDLE_COUNT ) { glClientActiveTextureARB( GL_TEXTURE0_ARB + nId); }
    else                              { glClientActiveTextureARB( GL_TEXTURE0_ARB);       }

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    // check if this was previously set
    if ( GetRenderState().IsCurrentTexCoord( nId, pBuffer ) ) break;


    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glTexCoordPointer(4, GL_FLOAT, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glTexCoordPointer(4, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    }
    GetRenderState().SetCurrentTexCoord( nId, pBuffer );
    break;
  case ELEMENT_TYPE_NORMAL_3F:
    glEnableClientState( GL_NORMAL_ARRAY );

    // check if this was previously set
    if ( GetRenderState().IsCurrentNormals( pBuffer ) ) break;    

    
    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );
      glNormalPointer( GL_FLOAT, 0, 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
      glNormalPointer( GL_FLOAT, 0, pBuffer->GetPointer<float>());
    }
    GetRenderState().SetCurrentNormals( pBuffer );
    break;

  case ELEMENT_TYPE_V3F_N3F_T2F:
    
    glClientActiveTextureARB( GL_TEXTURE0_ARB);
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_VERTEX_ARRAY );    
    glDisableClientState( GL_COLOR_ARRAY );
    // check if this was previously set

    if ( GetRenderState().IsCurrentTexCoord( 0, pBuffer ) && 
	 GetRenderState().IsCurrentNormals(  pBuffer ) &&
	 GetRenderState().IsCurrentVertices( pBuffer ) ) 
    {
      break;
    }
    

    if ( pBuffer->IsCached()) 
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, pBuffer->GetCache() );

      glTexCoordPointer(2, GL_FLOAT, pBuffer->GetElementByteSize(), BUFFER_OFFSET(sizeof(float)*6) );
      glNormalPointer(     GL_FLOAT, pBuffer->GetElementByteSize(), BUFFER_OFFSET(sizeof(float)*3) );
      glVertexPointer(3,   GL_FLOAT, pBuffer->GetElementByteSize(), 0);
    } 
    else
    {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);

      glTexCoordPointer(2, GL_FLOAT, pBuffer->GetElementByteSize(), pBuffer->GetPointer<float>()+sizeof(float)*6);
      glNormalPointer(     GL_FLOAT, pBuffer->GetElementByteSize(), pBuffer->GetPointer<float>()+sizeof(float)*3);
      glVertexPointer(3,   GL_FLOAT, pBuffer->GetElementByteSize(), pBuffer->GetPointer<float>());
    }

    GetRenderState().SetCurrentTexCoord( 0, pBuffer );
    GetRenderState().SetCurrentNormals(  pBuffer );
    GetRenderState().SetCurrentVertices( pBuffer );

    break;
  case ELEMENT_TYPE_UNIFORM_1F:
  case ELEMENT_TYPE_UNIFORM_2F:
  case ELEMENT_TYPE_UNIFORM_3F:
  case ELEMENT_TYPE_UNIFORM_4F:
  case ELEMENT_TYPE_UNIFORM_1I:
  case ELEMENT_TYPE_UNIFORM_2I:
  case ELEMENT_TYPE_UNIFORM_3I:
  case ELEMENT_TYPE_UNIFORM_4I:
  case ELEMENT_TYPE_UNIFORM_2X2F:
  case ELEMENT_TYPE_UNIFORM_3X3F:
  case ELEMENT_TYPE_UNIFORM_4X4F:
  case ELEMENT_TYPE_ATTRIB_1F:
  case ELEMENT_TYPE_ATTRIB_2F:
  case ELEMENT_TYPE_ATTRIB_3F:
  case ELEMENT_TYPE_ATTRIB_4F:
  case ELEMENT_TYPE_ATTRIB_1I:
  case ELEMENT_TYPE_ATTRIB_2I:
  case ELEMENT_TYPE_ATTRIB_3I:
  case ELEMENT_TYPE_ATTRIB_4I:
  case ELEMENT_TYPE_ATTRIB_1UB:
  case ELEMENT_TYPE_ATTRIB_2UB:
  case ELEMENT_TYPE_ATTRIB_3UB:
  case ELEMENT_TYPE_ATTRIB_4UB:
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::RollbackVertexDescriptor( CVertexDescriptor *pBuffer, unsigned int nId )
{
  // implemantation does not take into account if VBO is not supported.

  switch( pBuffer->GetType() )
  {
  case ELEMENT_TYPE_VERTEX_3F:
    glDisableClientState( GL_VERTEX_ARRAY );
    break;
  case ELEMENT_TYPE_COLOR_4UB:
  case ELEMENT_TYPE_COLOR_3F:
  case ELEMENT_TYPE_COLOR_4F:
    glDisableClientState( GL_COLOR_ARRAY );
    break;
  case ELEMENT_TYPE_TEX_2F:
  case ELEMENT_TYPE_TEX_3F:
  case ELEMENT_TYPE_TEX_4F:
    if ( nId < TEXTURE_HANDLE_COUNT ) { glClientActiveTextureARB( GL_TEXTURE0_ARB + nId); }
    else                              { glClientActiveTextureARB( GL_TEXTURE0_ARB);       }
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    break;
  case ELEMENT_TYPE_NORMAL_3F:
    glDisableClientState( GL_NORMAL_ARRAY );
    break;
  case ELEMENT_TYPE_V3F_N3F_T2F:
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    // this descriptor type uses first texunit
    glClientActiveTextureARB( GL_TEXTURE0_ARB);
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    break;
  case ELEMENT_TYPE_UNIFORM_1F:
  case ELEMENT_TYPE_UNIFORM_2F:
  case ELEMENT_TYPE_UNIFORM_3F:
  case ELEMENT_TYPE_UNIFORM_4F:
  case ELEMENT_TYPE_UNIFORM_1I:
  case ELEMENT_TYPE_UNIFORM_2I:
  case ELEMENT_TYPE_UNIFORM_3I:
  case ELEMENT_TYPE_UNIFORM_4I:
  case ELEMENT_TYPE_UNIFORM_2X2F:
  case ELEMENT_TYPE_UNIFORM_3X3F:
  case ELEMENT_TYPE_UNIFORM_4X4F:
  case ELEMENT_TYPE_ATTRIB_1F:
  case ELEMENT_TYPE_ATTRIB_2F:
  case ELEMENT_TYPE_ATTRIB_3F:
  case ELEMENT_TYPE_ATTRIB_4F:
  case ELEMENT_TYPE_ATTRIB_1I:
  case ELEMENT_TYPE_ATTRIB_2I:
  case ELEMENT_TYPE_ATTRIB_3I:
  case ELEMENT_TYPE_ATTRIB_4I:
  case ELEMENT_TYPE_ATTRIB_1UB:
  case ELEMENT_TYPE_ATTRIB_2UB:
  case ELEMENT_TYPE_ATTRIB_3UB:
  case ELEMENT_TYPE_ATTRIB_4UB:
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
  case PRIMITIVE_QUAD_STRIP:
    glPrimitive = GL_QUAD_STRIP;
    break;
  }
  //bool bIsActive = (GetRenderState().m_pIndexArray == pIndexBuffer ) ;
  GLenum iIndexBufferType;
  GLvoid *pIndices;

  ////////////////////
  // Check data type and render.
  if ( pIndexBuffer->IsShortIndices() )
  {
    iIndexBufferType = GL_UNSIGNED_SHORT;
    pIndices = pIndexBuffer->IsCached() ? 0 :  pIndexBuffer->GetPointer<unsigned short int>();
  }
  else
  {
    iIndexBufferType = GL_UNSIGNED_INT;
    pIndices = pIndexBuffer->IsCached() ? 0 :  pIndexBuffer->GetPointer<unsigned int>();
  }


  if ( !GetRenderState().IsCurrentIndices( pIndexBuffer ) )
  {
    if ( pIndices == 0 )
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->GetCache());
    else
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

    // store current indexbuffer pointer to renderstate
    GetRenderState().SetCurrentIndices(pIndexBuffer);
  }
  glDrawElements( glPrimitive, pIndexBuffer->GetDrawableCount(), 
		  iIndexBufferType,  pIndices);
  
  // if ( pIndexBuffer->IsShortIndices() )   
//   {
    
//     // Draw cached
//     if ( pIndexBuffer->IsCached())
//     {
//       //if ( !bIsActive )
//       {
// 	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->GetCache());
// 	//GetRenderState().m_pIndexArray = pIndexBuffer;
//       }
//       glDrawElements( glPrimitive, pIndexBuffer->GetDrawableCount(), 
// 		      GL_UNSIGNED_SHORT,  0);
//     }
//     else // Draw uncached
//     {
//       //if ( !bIsActive)
//       {
// 	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
// 	//GetRenderState().m_pIndexArray = pIndexBuffer;
//       }
//       glDrawElements( glPrimitive, pIndexBuffer->GetDrawableCount(), 
// 		      GL_UNSIGNED_SHORT, 
// 		      pIndexBuffer->GetPointer<unsigned short int>());
//     }
//   }
//   else
//   {
//     // Draw cached
//     if ( pIndexBuffer->IsCached())
//     {
//       //if ( !bIsActive)
//       {
// 	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->GetCache());
// 	//GetRenderState().m_pIndexArray = pIndexBuffer;
//       }
//       glDrawElements( glPrimitive, 
// 		      pIndexBuffer->GetDrawableCount(), 
// 		      GL_UNSIGNED_INT,  0);
//     }
//     else // Draw uncached
//     {
//       //if ( !bIsActive)
//       {
// 	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
// 	//GetRenderState().m_pIndexArray = pIndexBuffer;
//       }
//       glDrawElements( glPrimitive, 
// 		      pIndexBuffer->GetDrawableCount(), 
// 		      GL_UNSIGNED_INT, 
// 		      pIndexBuffer->GetPointer<unsigned int>());
//     }
//   }
  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertex( const Phoenix::Spatial::CVertex & rVertex, int iIgnoreFlags )
{
  // Set multitexture coordinates

  if ( !(iIgnoreFlags & VERTEX_COMP_TEXCOORD) )
  {
    for(int i=0;i<TEXTURE_HANDLE_COUNT;i++)
    {
      glMultiTexCoord2fARB( GL_TEXTURE0_ARB+i, 
			    rVertex.GetTextureCoordinates( i )[0],
			    rVertex.GetTextureCoordinates( i )[1] );
    }
  }
  /// Normal, color and position data
  if ( !(iIgnoreFlags & VERTEX_COMP_NORMAL) ) glNormal3fv( rVertex.GetNormal().GetArray()) ;
  if ( !(iIgnoreFlags & VERTEX_COMP_COLOR ) ) glColor4ubv( rVertex.GetColor().GetArray() );
  
  glVertex3fv( rVertex.GetPosition().GetArray()) ;
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
  case CLIENT_STATE_NORMAL_ARRAY:
    glDisableClientState( GL_NORMAL_ARRAY );
    break;
  case CLIENT_STATE_TEX0_ARRAY:
  case CLIENT_STATE_TEX1_ARRAY:
  case CLIENT_STATE_TEX2_ARRAY:
  case CLIENT_STATE_TEX3_ARRAY:
  case CLIENT_STATE_TEX4_ARRAY:
  case CLIENT_STATE_TEX5_ARRAY:
  case CLIENT_STATE_TEX6_ARRAY:
  case CLIENT_STATE_TEX7_ARRAY:
    glClientActiveTexture( GL_TEXTURE0_ARB + (tType-CLIENT_STATE_TEX0_ARRAY));
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
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
  case CLIENT_STATE_NORMAL_ARRAY:
    glEnableClientState( GL_NORMAL_ARRAY );
    break;
  case CLIENT_STATE_TEX0_ARRAY:
  case CLIENT_STATE_TEX1_ARRAY:
  case CLIENT_STATE_TEX2_ARRAY:
  case CLIENT_STATE_TEX3_ARRAY:
  case CLIENT_STATE_TEX4_ARRAY:
  case CLIENT_STATE_TEX5_ARRAY:
  case CLIENT_STATE_TEX6_ARRAY:
  case CLIENT_STATE_TEX7_ARRAY:
    glClientActiveTextureARB( GL_TEXTURE0_ARB + (tType-CLIENT_STATE_TEX0_ARRAY));
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    break;
  }
}
/////////////////////////////////////////////////////////////////


Phoenix::Graphics::COglTexture * 
Phoenix::Graphics::COglRenderer::CreateTexture( const std::string &strFilename, TEXTURE_TYPE tType  )
{
  ////////////////////
#define CLEANUP() { if ( pImage ) delete pImage; pImage = NULL; return pTexture; }
  ////////////////////
  CTGAImage *pImage = new CTGAImage();
  COglTexture *pTexture = NULL;
  switch ( pImage->Load( strFilename ) )
  {
  case IMG_OK:
    break;
  case IMG_ERR_NO_FILE:
    std::cerr << "No such file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  case IMG_ERR_UNSUPPORTED:
    std::cerr << "Unsupported format in file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  case IMG_ERR_MEM_FAIL:
    std::cerr << "Out of memory while loading file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  case IMG_ERR_BAD_FORMAT:
    std::cerr << "Bad format while loading file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  }
  ////////////////////
  int    iGLInternalFormat = 3;
  GLenum iGLformat = GL_RGB;
  ////////////////////
  // Check correct depth
  switch (pImage->GetBPP())
  {
  case 8:
    iGLInternalFormat = 1;
    iGLformat = GL_LUMINANCE;
    break;
  case 16:
    iGLInternalFormat = 2;
    iGLformat = GL_LUMINANCE_ALPHA;
    break;
  case 24:
    iGLInternalFormat = 3;
    iGLformat = GL_RGB;
    break;
  case 32:
    iGLInternalFormat = 4;
    iGLformat = GL_RGBA;
    std::cerr << "Texture " << strFilename << " has alpha channel." <<  std::endl;
    break;
  default:
    delete pImage;
    std::cerr << "Not 8, 16, 24 or 32 BBP image (was " << pImage->GetBPP() 
	      << "):  '" << strFilename << "'" << std::endl;
    return NULL;
  }

  // create texture
  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  pTexture = new COglTexture( iTexId, tType );

  // set texture dimensions
  pTexture->SetHeight( pImage->GetHeight());
  pTexture->SetWidth( pImage->GetWidth());

  // check memory allocation
  if ( !pTexture ) 
  {
    std::cerr << "Failed to allocate memory while loading file '" 
	      << strFilename << "'" << std::endl;
    return NULL;
  }
  
  GLenum iGLType = GetGLTextureType( tType );
  
  // create actual gl texture 
  glEnable( iGLType );
  glBindTexture(iGLType, pTexture->GetID());  
  // build mipmaps 
  glTexParameteri( iGLType, GL_GENERATE_MIPMAP, GL_TRUE); 
  glTexImage2D( iGLType, 0, iGLInternalFormat, 
		pImage->GetWidth(), pImage->GetHeight(), 0, 
		iGLformat, GL_UNSIGNED_BYTE, pImage->GetImg());
  // build mipmaps 
  /*gluBuild2DMipmaps(iGLType, iGLInternalFormat, 
		    pImage->GetWidth(), pImage->GetHeight(),
		    iGLformat, GL_UNSIGNED_BYTE, pImage->GetImg());*/
  
  
  glDisable( iGLType );

  // ReleaseMemory
  CLEANUP();

#undef CLEANUP  	   

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture * 
Phoenix::Graphics::COglRenderer::CreateCompressedTexture( const char *strFilename, TEXTURE_TYPE tType  )
{
  ////////////////////
#define CLEANUP() { if ( pImage ) delete pImage; pImage = NULL; return pTexture; }
  ////////////////////
  /// Sanity check.
  if ( !GetFeatures().HasTextureCompressionS3TC() ) return NULL;
  ////////////////////
  CDDSImage *pImage = new CDDSImage();
  COglTexture *pTexture = NULL;
  assert( strFilename != NULL && "Texture filename is NULL");
  switch ( pImage->Load( strFilename ) )
  {
  case IMG_OK:
    break;
  case IMG_ERR_NO_FILE:
    std::cerr << "No such file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  case IMG_ERR_UNSUPPORTED:
    std::cerr << "Unsupported format in file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  case IMG_ERR_MEM_FAIL:
    std::cerr << "Out of memory while loading file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  case IMG_ERR_BAD_FORMAT:
    std::cerr << "Bad format while loading file '" << strFilename << "'" << std::endl;
    CLEANUP();
    break;
  }

  ////////////////////
  size_t nBlockSize = 16;
  GLenum glFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
  ////////////////////
  // Check correct format:
  switch (pImage->GetFormat())
  {
  case DDS_FORMAT_DXT1:
    nBlockSize = 8;
    glFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    break;
  case DDS_FORMAT_DXT3:
    glFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    nBlockSize = 16;
    break;
  case DDS_FORMAT_DXT5:
    glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    nBlockSize = 16;
    break;
  default:
    delete pImage;
    std::cerr << "Not in DXT1, DXT3, DXT5 format (was " 
	      << (pImage->GetFormat() == DDS_FORMAT_DXT2  ? "DXT2" : "DXT4")
	      << "):  '" << strFilename << "'" << std::endl;
    return NULL;
  }

  // create texture
  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  pTexture = new COglTexture( iTexId, tType );
  // set texture dimensions
  pTexture->SetHeight( pImage->GetHeight());
  pTexture->SetWidth( pImage->GetWidth());

  // check memory allocation
  if ( !pTexture ) 
  {
    std::cerr << "Failed to allocate memory while loading file '" 
	      << strFilename << "'" << std::endl;
    return NULL;
  }
  

  
  GLenum iGLType = GetGLTextureType( tType );

  // create actual gl texture 
  glEnable( iGLType );
  glBindTexture(iGLType, pTexture->GetID());  

  // Set default texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,	GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,	GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  
  int nSize;
  int nOffset = 0;
  int nWidth = pImage->GetWidth();
  int nHeight = pImage->GetHeight();

  for( int i = 0; i < pImage->GetNumMipMaps(); i++ )
  {
    if ( nWidth == 0 ) nWidth = 1;
    if ( nHeight == 0 ) nHeight = 1;
		       
    nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;

    glCompressedTexImage2D( GL_TEXTURE_2D,  i,  glFormat,  nWidth,  nHeight,
			       0, nSize, pImage->GetPixelData() + nOffset );
    nOffset += nSize;

    // Half the image size for next mipmap
    nWidth  = (nWidth  / 2);
    nHeight = (nHeight / 2);
  }
  
  glDisable( iGLType );

  // ReleaseMemory
  CLEANUP();

#undef CLEANUP  	   

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture * 
Phoenix::Graphics::COglRenderer::CreateCubeTexture( const char * szFiles[6] )
{
  // Files must not be null
  if ( szFiles == NULL ) return NULL;
  
  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  COglTexture *pTexture = new COglTexture( iTexId, TEXTURE_CUBE );

  for( size_t i=0;i<6;i++)
  {
    ////////////////////
    CTGAImage *pImage = CTGAImage::LoadImage( szFiles[i] );
    assert ( pImage != NULL && "Unable to load TGA image for CUBE TEXTURE." );
    ////////////////////
    int    iGLInternalFormat = 3;
    GLenum iGLformat = GL_RGB;
    ////////////////////
    // Check correct depth
    switch (pImage->GetBPP())
    {
    case 8:
      iGLInternalFormat = 1;
      iGLformat = GL_LUMINANCE;
      break;
    case 16:
      iGLInternalFormat = 2;
      iGLformat = GL_LUMINANCE_ALPHA;
      break;
    case 24:
      iGLInternalFormat = 3;
      iGLformat = GL_RGB;
      break;
    case 32:
      iGLInternalFormat = 4;
      iGLformat = GL_RGBA;
      std::cerr << "Texture " << szFiles[i] << " has alpha channel." <<  std::endl;
      break;
    default:
      delete pImage;
      std::cerr << "Not 8, 16, 24 or 32 BBP image (was " 
		<< pImage->GetBPP() << "):  '" << szFiles[i] << "'" 
		<< std::endl;
      break;
    }
    GLenum iGLType = GL_TEXTURE_CUBE_MAP;
    ////////////////////
    // create actual gl texture 
    glEnable( iGLType );
    glBindTexture( iGLType, pTexture->GetID());  
    ////////////////////
    // Set default texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,	GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,	GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,	GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    ////////////////////
    /// Determine proper face id
    GLenum cubeFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X; 
    switch (i)
    {
    case 0:
      cubeFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
      break;
    case 1:
      cubeFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
      break;
    case 2:
      cubeFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
      break;
    case 3:
      cubeFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
      break;
    case 4:
      cubeFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
      break;
    case 5:
      cubeFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
      break;
    }
    ////////////////////
    // Create texture.
    glTexImage2D( cubeFace, 0, iGLInternalFormat, 
		  pImage->GetWidth(), pImage->GetHeight(), 0, 
		  iGLformat,  GL_UNSIGNED_BYTE, pImage->GetImg());
    ////////////////////
    // Cleanup data, it is not needed anymore.
    if ( pImage ) 
    {
      delete pImage; 
      pImage = NULL; 
    }

  } // for ( size_t
  return pTexture;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture * 
Phoenix::Graphics::COglRenderer::CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType, TEXTURE_FORMAT tFormat )
{
  
  // create texture
  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  COglTexture *pTexture = new COglTexture( iTexId, tType );
  // set texture dimensions
  pTexture->SetHeight( nHeight );
  pTexture->SetWidth( nWidth );

  // check memory allocation
  if ( !pTexture ) 
  {
    std::cerr << "Failed to allocate memory while creating Empty Texture." << std::endl;
    return NULL;
  }

  GLenum iGLType = GetGLTextureType(tType);
  // create texture dimensions

  glEnable( iGLType );
  glBindTexture( iGLType, pTexture->GetID());


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // depth texture requires special settings
  if ( tType == TEXTURE_DEPTH2D )
  {
    glTexParameteri( iGLType, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D( iGLType, 0, GL_DEPTH_COMPONENT, nWidth, nHeight, 0, 
		  GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
  }
  else
  {
    glTexImage2D( iGLType, 0, static_cast<GLenum>(tFormat), nWidth, nHeight, 0, 
		  static_cast<GLenum>(tFormat), GL_UNSIGNED_BYTE, NULL );
  }

  glDisable( iGLType);

  return pTexture;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture * 
Phoenix::Graphics::COglRenderer::CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType, void *pData, TEXTURE_FORMAT tFormat)
{
   // create texture
  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  COglTexture *pTexture = new COglTexture( iTexId, tType );
  // set texture dimensions
  pTexture->SetHeight( nHeight );
  pTexture->SetWidth( nWidth );
  // check memory allocation
  if ( !pTexture ) 
  {
    std::cerr << "Failed to allocate memory while creating Empty Texture." << std::endl;
    return NULL;
  }

  GLenum iGLType = GetGLTextureType(tType);
  // create texture dimensions

  glEnable( iGLType );
  glBindTexture( iGLType, pTexture->GetID());
  glTexImage2D( iGLType, 0, static_cast<GLenum>(tFormat), nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData );
  glDisable( iGLType);
  
  return pTexture;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitTexture( unsigned int nTexUnit, COglTexture *pTexture )
{
  glActiveTextureARB( GL_TEXTURE0_ARB + nTexUnit);
  
  if ( true || ! GetRenderState().IsCurrentTexture( nTexUnit, pTexture) )
  {
    // Bind texture
    glBindTexture( GetGLTextureType( pTexture->GetType() ), pTexture->GetID() ); 
    // Set texture pointer to renderstate 
    GetRenderState().SetCurrentTexture( nTexUnit, pTexture);
  }

  glEnable( GetGLTextureType( pTexture->GetType() ) );

}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::DisableTexture( unsigned int nTexUnit, COglTexture *pTexture )
{
  glActiveTextureARB( GL_TEXTURE0_ARB + nTexUnit);
  if ( pTexture != NULL )
  {
    glDisable( GetGLTextureType( pTexture->GetType()));
  } 
  else 
  {
    DISABLE_ALL_TEXTURES();
  }
  GetRenderState().SetCurrentTexture( nTexUnit, NULL);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitCamera( CCamera &camera )
{
  m_pCamera = &camera;
  ////////////////////
  // Handle updating of view and projection, if they are changed.
  if ( camera.IsProjectionChanged()) camera.UpdateProjection();
  if ( camera.IsViewChanged()) camera.UpdateView();

  int *pViewport = camera.GetViewport();
  glViewport(pViewport[0], pViewport[1], pViewport[2], pViewport[3]);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glMultTransposeMatrixf( camera.GetProjectionMatrix().GetArray());
  ////////////////////
  /// Set up proper position.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultTransposeMatrixf( camera.GetViewMatrix().GetArray());
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CCamera * 
Phoenix::Graphics::COglRenderer::GetCurrentCamera() const
{
  return m_pCamera;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitRenderable( CRenderable &renderable, int iExcludeOpts )
{

  CommitTransform( renderable.GetWorldTransform());
  // Retrieve resources
  COglTexture *pTexture = NULL;
  CVertexDescriptor *pTemp = NULL;


  CRenderState & state = renderable.GetRenderState();
  CommitBlending( state.GetBlendingOperation());

  // Check depth mask write flag.
  if ( state.GetDepthWrite()) {  CommitState( STATE_DEPTH_WRITE );  } 
  else DisableState( STATE_DEPTH_WRITE );

  // Check depth test flag.
  if ( state.GetDepthTest()) CommitState( STATE_DEPTH_TEST ); 
  else DisableState( STATE_DEPTH_TEST );
  // Check face culling flag.
  if ( state.GetFaceCulling()) CommitState( STATE_FACECULLING );
  else DisableState( STATE_FACECULLING );
  // Commit textures
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    pTemp    = *renderable.GetTextureCoordinateHandle(i);
    pTexture = *renderable.GetTextureHandle(i);
    
    // check that texcoord resources actually exist
    if ( pTemp == NULL ) 
    {
      glClientActiveTextureARB( GL_TEXTURE0_ARB + i);
      glDisableClientState( GL_TEXTURE_COORD_ARRAY);
    }
    else
    {
      CommitVertexDescriptor( pTemp, i );  
    }
    // check that texture resource exists
    if ( pTexture  != NULL ) 
    { 
      CommitTexture( i, pTexture ); 
      CommitFilters( renderable.GetTextureFilters(i), pTexture->GetType() );
    }
    else 
      DisableTexture(i, NULL);
  }


  CShader *pShader = *renderable.GetShaderHandle();
  CommitShader( pShader );

  if ( !renderable.GetShaderHandle().IsNull())
  {
    CVertexDescriptor *pParam = NULL;
    // Go through all parameters and commit them
    for(unsigned int nSP=0; nSP< renderable.GetShaderParameters().size(); nSP++)
    {
      pParam = *( *renderable.GetShaderParameters()[nSP].second );
      if ( pParam != NULL )
      {
	CommitShaderParam( *pShader, renderable.GetShaderParameters()[nSP].first, *pParam );
      }
    }

    // Go through all int parameters and commit them
    {
      ShaderIntParams::iterator it = renderable.GetShaderIntParameters().begin();
      for(; it != renderable.GetShaderIntParameters().end(); it++)
      {
	CommitUniformShaderParam( *pShader, it->first, it->second );
      }
    }
    // Go through all float parameters and commit them
    {
      ShaderFloatParams::iterator it = renderable.GetShaderFloatParameters().begin();
      for( ; it != renderable.GetShaderFloatParameters().end(); it++)
      {
	CommitUniformShaderParam( *pShader, it->first, it->second );
      }
    }
  }

  // check and commit resources
  if ( renderable.GetVertexHandle().IsNull() )	glDisableClientState( GL_VERTEX_ARRAY ); 
  else						CommitVertexDescriptor ( *renderable.GetVertexHandle() ); 

  // commit normals
  if ( renderable.GetNormalHandle().IsNull() )  glDisableClientState( GL_NORMAL_ARRAY );   
  else						CommitVertexDescriptor( *renderable.GetNormalHandle() ); 

  // Commit colors
  if ( renderable.GetColorHandle().IsNull()  )  
  {
    glDisableClientState( GL_COLOR_ARRAY );
    CommitColor( renderable.GetRenderState().GetBaseColor() );
  }
  else						CommitVertexDescriptor( *renderable.GetColorHandle() );
  
  if ( !renderable.GetIndices().IsNull() )     CommitPrimitive( *renderable.GetIndices() );

  
  RollbackTransform();
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitFilter( TEXTURE_FILTER tFilter, TEXTURE_TYPE tType )
{
  GLenum glTarget = GetGLTextureType( tType );
  ////////////////////
  switch( tFilter )
  {
  case ENV_MODULATE:
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    break;
  case ENV_DECAL:
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    break;
  case ENV_REPLACE:
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
    break;
  case ENV_BLEND:
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    break;
  case ENV_COMBINE_INCR:
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INCR);
    break;
  case ENV_COMBINE_REPLACE:
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
    break;
  case MIN_NEAREST:
    glTexParameteri( glTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    break;
  case MIN_LINEAR:
    glTexParameteri( glTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;
  case MIN_MIP_NEAREST:
    glTexParameteri( glTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    break;
  case MIN_MIP_LINEAR:
    glTexParameteri( glTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    break;
  case MAG_NEAREST:
    glTexParameteri( glTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    break;
  case MAG_LINEAR:
    glTexParameteri( glTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    break;
  case T_WRAP_REPEAT:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    break;
  case S_WRAP_REPEAT:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    break;
  case T_WRAP_CLAMP:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
    break;
  case S_WRAP_CLAMP:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
    break;
  case T_WRAP_CLAMP_TO_EDGE:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    break;
  case S_WRAP_CLAMP_TO_EDGE:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    break;
  }
  //  case ENV_COLOR:
  //glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, pNode->GetTexEnvColor().GetValues()); 
  //break;  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitFilters( const std::vector<Phoenix::Graphics::TEXTURE_FILTER> &vecFilters, Phoenix::Graphics::TEXTURE_TYPE tType )
{
  // Apply texture filters.
  for(unsigned int nFilter=0; nFilter<vecFilters.size(); nFilter++)
  {
    CommitFilter( vecFilters[nFilter], tType );
  }
}
/////////////////////////////////////////////////////////////////
int
LoadFile( const char * szFilename, std::string &sContents )
{

  // Temporary container for line 
  string sLine;
  // The file stream 
  ifstream fsFile;
  // open file 
  fsFile.open( szFilename, ios::in );
  // test for failure 
  if ( fsFile.is_open() ) 
  {
    // read line by line 
    while( ! fsFile.eof() )
    {
      getline( fsFile, sLine );
      sContents.append( sLine );
      sContents.append( "\n" );
    }
    // close the file 
    fsFile.close();
  } 
  else 
  {
    std::cerr << "The file " << szFilename
	      << " couldn't be opened."  << std::endl;
    return 1;
  }
  return 0;
}  
#define INFO_LOG_BUF_SIZE 1024
/////////////////////////////////////////////////////////////////
/// Parses shader/program info log into given string.
/// \param nObject From which shader/program log is retrieved.
/// \param strInfoLog String where log is stored.
/// \param bProgram True for retrieving program info log, False for shader info log
inline void 
GetInfoLog( unsigned int nObject, std::string &strInfoLog, int bProgram )
{
  int iInfologLength   = 0;      // total length of log
  int iCharsRetrievedBatch  = 0; // how many chars were retrived from log in current pass
  int iCharsRetrievedTotal = 0;  // how many chars in total is retrieved.
  char sInfoLog[INFO_LOG_BUF_SIZE];

  // Get length of the info log
  if ( bProgram )      glGetProgramiv(nObject, GL_INFO_LOG_LENGTH, &iInfologLength);
  else		       glGetShaderiv(nObject, GL_INFO_LOG_LENGTH, &iInfologLength);
  
  // if info log exists, we parse it
  if (iInfologLength > 0)
  {
    strInfoLog.clear(); // clear existing string
    strInfoLog.reserve(iInfologLength); // allocate sufficient memory

    // read chars up to info log length
    while ( iCharsRetrievedTotal < iInfologLength )
    {
      if ( bProgram ) glGetProgramInfoLog(nObject, INFO_LOG_BUF_SIZE, &iCharsRetrievedBatch, sInfoLog);
      else            glGetShaderInfoLog(nObject, INFO_LOG_BUF_SIZE, &iCharsRetrievedBatch, sInfoLog);

      if ( iCharsRetrievedBatch > 0 )
      {
	iCharsRetrievedTotal += iCharsRetrievedBatch; // increase number of read chars
	strInfoLog.append(sInfoLog, iCharsRetrievedBatch); // append data to string
      }
    } // end while
    // ok, done
  }
}
/////////////////////////////////////////////////////////////////
/// Wrapper for retrieving shader info log.
/// \param nShader From which shader log is retrieved.
/// \param strInfoLog String where log is stored.
inline void 
GetShaderInfoLog( unsigned int nShader, std::string &strInfoLog )
{
  GetInfoLog( nShader, strInfoLog, 0 );
}
/////////////////////////////////////////////////////////////////
/// Wrapper for retrieving program info log.
/// \param nProg From which program log is retrieved.
/// \param strInfoLog String where log is stored.
inline void 
GetProgramInfoLog( unsigned int nProg, std::string &strInfoLog )
{
  GetInfoLog( nProg, strInfoLog, 1 );
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShader * 
Phoenix::Graphics::COglRenderer::CreateShader( const std::string & strVertexShader, const std::string & strFragmentShader )
{
  return CreateShader(strVertexShader.c_str(), strFragmentShader.c_str());
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShader * 
Phoenix::Graphics::COglRenderer::CreateShader( const char * szVertexShader, const char * szFragmentShader )
{

 
  string strVSSource, strFSSource;
  ////////////////////
  // Vertex shader loading
  if ( szVertexShader != NULL && strlen(szVertexShader) > 0 )
  {
    if ( LoadFile( szVertexShader, strVSSource ))
    {
      std::cerr << "Failed to load vertex shader '" << szVertexShader << "'" << std::endl;
    }
    
  }
  ////////////////////
  // Fragment shader loading 
  if ( szFragmentShader != NULL && strlen(szFragmentShader) > 0 )
  {
    if ( LoadFile( szFragmentShader, strFSSource ))
    {
      std::cerr << "Failed to load fragment shader '" << szFragmentShader << "'" << std::endl;
    }
  }
  
  return CreateShaderFromSource( strVSSource.size() > 0 ? strVSSource.c_str() : NULL,
				 strFSSource.size() > 0 ? strFSSource.c_str() : NULL, szVertexShader, szFragmentShader );
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShader * 
Phoenix::Graphics::COglRenderer::CreateShaderFromSource( const char * szVertexShaderCode, const char * szFragmentShaderCode, 
							 const char * szVSname, const char * szFSname)
{
  int bHasShader = 0;
  int iState = 0; // compile and link status
  
  unsigned int nProgram = glCreateProgram();
  CShader *pShader = new CShader( nProgram );

  if ( pShader == NULL )
  {
    std::cerr << "Failed to allocate shader memory." << std::endl;
    return NULL;
  }

  ////////////////////
  // Vertex shader loading
  if ( szVertexShaderCode != NULL && strlen(szVertexShaderCode) > 0 )
  {

    unsigned int nVertexShader = glCreateShader( GL_VERTEX_SHADER );
    int nLength = strlen(szVertexShaderCode); // source code length
    const char *pStrCode = szVertexShaderCode;
    
    // compile source
    glShaderSource(nVertexShader,1, &pStrCode, &nLength );
    glCompileShader( nVertexShader );
    // get compile status
    glGetShaderiv( nVertexShader, GL_COMPILE_STATUS, &iState);
    if ( iState == GL_TRUE )
    {
      // compiling went ok
      pShader->SetVertexShader( nVertexShader );
      bHasShader = 1;	
    }
    else
    {
      string strLog;
      GetShaderInfoLog( nVertexShader, strLog );
      std::cerr << "Error in vertex shader " << (szVSname == NULL  ? "" : szVSname) << " : " << strLog << std::endl;      
    }

  }

  ////////////////////
  // Fragment shader loading 
  if ( szFragmentShaderCode != NULL && strlen(szFragmentShaderCode) > 0 )
  {
    
    unsigned int nFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    int nLength = strlen(szFragmentShaderCode); // source code length
    const char *pStrCode = szFragmentShaderCode;
      
    glShaderSource(nFragmentShader,1, &pStrCode, &nLength );
    glCompileShader( nFragmentShader );

    // get compile status
    glGetShaderiv( nFragmentShader, GL_COMPILE_STATUS, &iState);
    if ( iState == GL_TRUE )
    {
      // compiling went ok
      pShader->SetFragmentShader( nFragmentShader );
      bHasShader = 1;
    }
    else
    {
      string strLog;
      GetShaderInfoLog( nFragmentShader, strLog );
      std::cerr << "Error in fragment shader " << (szFSname == NULL ? "" : szFSname) << strLog << std::endl;      
    }


  }
  // check that shader code exists
  if ( !bHasShader )
  {
    delete pShader;
    return NULL;
  }
  
  // link shader program
  glLinkProgram(pShader->GetProgram());
  glGetProgramiv(pShader->GetProgram(), GL_LINK_STATUS, &iState);

  // show linking errors, if any
  if ( iState == GL_FALSE )
  {
    string strLog;
    GetProgramInfoLog(pShader->GetProgram(), strLog);
    std::cerr << strLog << std::endl;
    delete pShader;
    return NULL;
  }
  
  return pShader;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitShader( CShader *pShader )
{
  //if ( true || !GetRenderState().IsCurrentShader(pShader) ) 
  //{
    if ( pShader )
    {
      glUseProgram( pShader->GetProgram() );
    }
    else
    {
      glUseProgram( 0 );
    }
    
    GetRenderState().SetCurrentShader(pShader);
    //}
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const std::string &strParamName, int iValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName.c_str());
  if ( iLoc != -1 )
    glUniform1i( iLoc, iValue );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const std::string &strParamName, float fValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName.c_str());
  if ( iLoc != -1 )
    glUniform1f( iLoc, fValue );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitShaderParam( CShader &shader, const std::string &strParamName, const CVertexDescriptor & vParam)
{
  CommitShaderParam( shader, strParamName.c_str(), vParam );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const char *strParamName, int iValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName);
  if ( iLoc != -1 )
    glUniform1i( iLoc, iValue );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const char *strParamName, float fValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName);
  if ( iLoc != -1 )
    glUniform1f( iLoc, fValue );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitShaderParam( CShader &shader, const char *strParamName, const CVertexDescriptor & vParam )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName);
  if ( iLoc != -1 )
  {
    switch( vParam.GetType() )
    {
    case ELEMENT_TYPE_UNIFORM_1F:
      if ( vParam.GetSize() == 1) { glUniform1f( iLoc, vParam.GetPointer<float>()[0] ); }
      else { glUniform1fv( iLoc, vParam.GetSize(), vParam.GetPointer<float>() ); }
      break;
    case ELEMENT_TYPE_UNIFORM_2F:
      if ( vParam.GetSize() == 1) { glUniform2f( iLoc, vParam.GetPointer<float>()[0],vParam.GetPointer<float>()[1] );  }
      else { glUniform2fv( iLoc, vParam.GetSize(), vParam.GetPointer<float>() ); }
      break;
    case ELEMENT_TYPE_UNIFORM_3F:
      if ( vParam.GetSize() == 1) { glUniform3f( iLoc, vParam.GetPointer<float>()[0], vParam.GetPointer<float>()[1], vParam.GetPointer<float>()[2] );  } 
      else { glUniform3fv( iLoc, vParam.GetSize(), vParam.GetPointer<float>() ); }
      break;
    case ELEMENT_TYPE_UNIFORM_4F:
      if ( vParam.GetSize() == 1) { glUniform4f( iLoc, vParam.GetPointer<float>()[0], vParam.GetPointer<float>()[1], vParam.GetPointer<float>()[2], vParam.GetPointer<float>()[3] ); }
      else { glUniform4fv( iLoc, vParam.GetSize(), vParam.GetPointer<float>() ); }
      break;
    case ELEMENT_TYPE_UNIFORM_1I:
      if ( vParam.GetSize() == 1) { glUniform1i( iLoc, vParam.GetPointer<int>()[0] ); }
      else { glUniform1iv( iLoc, vParam.GetSize(), vParam.GetPointer<int>() ); }
      break;
    case ELEMENT_TYPE_UNIFORM_2I:
      if ( vParam.GetSize() == 1)  { glUniform2i( iLoc, vParam.GetPointer<int>()[0],vParam.GetPointer<int>()[1] );    }
      else {	glUniform2iv( iLoc, vParam.GetSize(), vParam.GetPointer<int>() );      }
      break;
    case ELEMENT_TYPE_UNIFORM_3I:
      if ( vParam.GetSize() == 1) { glUniform3i( iLoc, vParam.GetPointer<int>()[0], vParam.GetPointer<int>()[1], vParam.GetPointer<int>()[2] ); } 
      else { glUniform3iv( iLoc, vParam.GetSize(), vParam.GetPointer<int>() );      }
      break;
    case ELEMENT_TYPE_UNIFORM_4I:
      if ( vParam.GetSize() == 1) { glUniform4i( iLoc, vParam.GetPointer<int>()[0], vParam.GetPointer<int>()[1], vParam.GetPointer<int>()[2], vParam.GetPointer<int>()[3] );  }
      else                        { glUniform4iv( iLoc, vParam.GetSize(), vParam.GetPointer<int>() ); }
      break;
    case ELEMENT_TYPE_UNIFORM_2X2F:
      glUniformMatrix2fv( iLoc, vParam.GetSize(), 0, vParam.GetPointer<float>() );
      break;
    case ELEMENT_TYPE_UNIFORM_3X3F:
      glUniformMatrix3fv( iLoc, vParam.GetSize(), 0, vParam.GetPointer<float>() );
      break;
    case ELEMENT_TYPE_UNIFORM_4X4F:
      glUniformMatrix4fv( iLoc, vParam.GetSize(), 0, vParam.GetPointer<float>() );
      break;
    default:
      break;
    
    } 
  } 
  else if ( (iLoc = glGetAttribLocation(shader.GetProgram(), strParamName)) != -1 )
  {
    
    glEnableVertexAttribArray(iLoc);
    switch ( vParam.GetType())
    {
    case ELEMENT_TYPE_ATTRIB_1F:
      glVertexAttribPointer(iLoc, 1, GL_FLOAT, 0, 0, vParam.GetPointer<float>());
      break;
    case ELEMENT_TYPE_ATTRIB_2F:
      glVertexAttribPointer(iLoc, 2, GL_FLOAT, 0, 0, vParam.GetPointer<float>());
      break;
    case ELEMENT_TYPE_ATTRIB_3F:
      glVertexAttribPointer(iLoc, 3, GL_FLOAT, 0, 0, vParam.GetPointer<float>());
      break;
    case ELEMENT_TYPE_ATTRIB_4F:
      glVertexAttribPointer(iLoc, 4, GL_FLOAT, 0, 0, vParam.GetPointer<float>());
      break;
    case ELEMENT_TYPE_ATTRIB_1I:
      glVertexAttribPointer(iLoc, 1, GL_INT, 0, 0, vParam.GetPointer<int>());
      break;
    case ELEMENT_TYPE_ATTRIB_2I:
      glVertexAttribPointer(iLoc, 2, GL_INT, 0, 0, vParam.GetPointer<int>());
      break;
    case ELEMENT_TYPE_ATTRIB_3I:
      glVertexAttribPointer(iLoc, 3, GL_INT, 0, 0, vParam.GetPointer<int>());
      break;
    case ELEMENT_TYPE_ATTRIB_4I:
      glVertexAttribPointer(iLoc, 4, GL_INT, 0, 0, vParam.GetPointer<int>());
      break;
    case ELEMENT_TYPE_ATTRIB_1UB:
      glVertexAttribPointer(iLoc, 1, GL_UNSIGNED_BYTE, 0, 0, vParam.GetPointer<unsigned char>());
      break;
    case ELEMENT_TYPE_ATTRIB_2UB:
      glVertexAttribPointer(iLoc, 2, GL_UNSIGNED_BYTE, 0, 0, vParam.GetPointer<unsigned char>());
      break;
    case ELEMENT_TYPE_ATTRIB_3UB:
      glVertexAttribPointer(iLoc, 3, GL_UNSIGNED_BYTE, 0, 0, vParam.GetPointer<unsigned char>());
      break;
    case ELEMENT_TYPE_ATTRIB_4UB:
      glVertexAttribPointer(iLoc, 4, GL_UNSIGNED_BYTE, 0, 0, vParam.GetPointer<unsigned char>());
      break;
    default:
      break;
    }
  }
  else
  {
    cerr << "No such parameter '" << strParamName << "' in this shader!" << endl;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX, float fY, float fZ, float fW )
{
  if( iLoc != -1 )
  {
    glVertexAttrib4f( iLoc, fX, fY, fZ, fW );
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX, float fY, float fZ )
{
  if( iLoc != -1 )
  {
    glVertexAttrib3f( iLoc, fX, fY, fZ );
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX, float fY )
{
  if( iLoc != -1 )
  {
    glVertexAttrib2f( iLoc, fX, fY );
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc, float fX )
{
  if( iLoc != -1 )
  {
    glVertexAttrib1f( iLoc, fX );
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitLight( const CLight &light, unsigned int nLightId)
{
  GLenum iLightID = GL_LIGHT0+nLightId;
  glEnable(iLightID);
  float aTempVector[4];
  switch ( light.GetType())
  {
  case DIRECTIONAL:
    // if we have directional light, the position parameter 
    // actually defines the direction
    aTempVector[0] = -light.GetDirection()[0];
    aTempVector[1] = -light.GetDirection()[1];
    aTempVector[2] = -light.GetDirection()[2];
    aTempVector[3] = 0.0f;
    glLightf(iLightID, GL_SPOT_CUTOFF, 180.0);

    break;
  case SPOTLIGHT:
    aTempVector[0] = light.GetPosition()[0];
    aTempVector[1] = light.GetPosition()[1];
    aTempVector[2] = light.GetPosition()[2];
    aTempVector[3] = 1.0f;
      
    glLightf( iLightID, GL_SPOT_CUTOFF,    light.GetSpotAngle());
    glLightfv(iLightID, GL_SPOT_DIRECTION, const_cast<CVector3<float> &>(light.GetDirection()).GetArray());
      
    // Set the attenuation parameters
    glLightf(iLightID, GL_LINEAR_ATTENUATION,    light.GetLinearAttenuation());
    glLightf(iLightID, GL_QUADRATIC_ATTENUATION, light.GetQuadraticAttenuation());
    glLightf(iLightID, GL_CONSTANT_ATTENUATION,  light.GetConstantAttenuation());

    break;

  case POINTLIGHT:

    aTempVector[0] = light.GetPosition()[0];
    aTempVector[1] = light.GetPosition()[1];
    aTempVector[2] = light.GetPosition()[2];
    aTempVector[3] = 1.0f;

    glLightf(iLightID, GL_SPOT_CUTOFF, 180.0f);

    // Set the attenuation parameters
    glLightf(iLightID, GL_LINEAR_ATTENUATION,    light.GetLinearAttenuation());
    glLightf(iLightID, GL_QUADRATIC_ATTENUATION, light.GetQuadraticAttenuation());
    glLightf(iLightID, GL_CONSTANT_ATTENUATION,  light.GetConstantAttenuation());
      
    break;
  }


  // Set the light position
  glLightfv(iLightID, GL_POSITION, aTempVector);
  
  // Set the intensity distribution of the light.
  glLightf(iLightID, GL_SPOT_EXPONENT, light.GetSpotExponent());
  
  // Set diffuse RGBA intensity 
  aTempVector[0] = (float)light.GetDiffuseColor()[0]/255.0f;
  aTempVector[1] = (float)light.GetDiffuseColor()[1]/255.0f;
  aTempVector[2] = (float)light.GetDiffuseColor()[2]/255.0f;
  aTempVector[3] = (float)light.GetDiffuseColor()[3]/255.0f;
  glLightfv(iLightID, GL_DIFFUSE, aTempVector);

  // Set ambient RGBA intensity
  aTempVector[0] = (float)light.GetAmbientColor()[0]/255.0f;
  aTempVector[1] = (float)light.GetAmbientColor()[1]/255.0f;
  aTempVector[2] = (float)light.GetAmbientColor()[2]/255.0f;
  aTempVector[3] = (float)light.GetAmbientColor()[3]/255.0f;
  glLightfv(iLightID, GL_AMBIENT, aTempVector);

  // Set specular RGBA intensity
  aTempVector[0] = (float)light.GetSpecularColor()[0]/255.0f;
  aTempVector[1] = (float)light.GetSpecularColor()[1]/255.0f;
  aTempVector[2] = (float)light.GetSpecularColor()[2]/255.0f;
  aTempVector[3] = (float)light.GetSpecularColor()[3]/255.0f;
  glLightfv(iLightID, GL_SPECULAR, aTempVector);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::DisableLight ( unsigned int nLightId )
{
  glDisable(GL_LIGHT0+nLightId);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::DisableState( STATE_TYPE tState )
{
  switch(tState)
  {
  case STATE_DEPTH_WRITE:
    glDepthMask(GL_FALSE);
    break;
  default:
    glDisable(static_cast<GLenum>(tState));
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitState( STATE_TYPE tState )
{
  switch(tState)
  {
  case STATE_DEPTH_WRITE:
    glDepthMask(GL_TRUE);
    break;
  default:
    glEnable(static_cast<GLenum>(tState));
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitMaterial( const Phoenix::Graphics::CMaterial & material, int iFace )
{
  GLenum glFace = iFace ? GL_BACK : GL_FRONT;
  glMaterialfv( glFace, GL_DIFFUSE,   material.GetDiffuse().GetArray());
  glMaterialfv( glFace, GL_AMBIENT,   material.GetAmbient().GetArray());
  glMaterialfv( glFace, GL_SPECULAR,  material.GetSpecular().GetArray());
  glMaterialfv( glFace, GL_EMISSION,  material.GetEmission().GetArray());
  glMaterialf(  glFace, GL_SHININESS, material.GetShininess()*128.0f);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitAlpha(  Phoenix::Graphics::ALPHA_TEST_TYPE tType, float fReference )
{
  glAlphaFunc( static_cast<GLenum>(tType), fReference);
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitBlending( BLEND_SRC_TYPE tSource, BLEND_DST_TYPE tDestination)
{
  glBlendFunc( static_cast<GLenum>(tSource),static_cast<GLenum>(tDestination));
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitBlending( Phoenix::Graphics::CBlendingOperation & rBlendingOp )
{
  if ( rBlendingOp.IsEnabled() ) 
  {
    CommitState( STATE_BLENDING );
    CommitBlending( rBlendingOp.GetSourceOperation(), rBlendingOp.GetDestinationOperation());
  }
  else 
    DisableState( STATE_BLENDING );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitSkybox( Phoenix::Graphics::CSkybox & skybox, Phoenix::Graphics::CCamera &camera )
{

  // Get view matrix, reset translation part
  Phoenix::Math::CMatrix4x4<float> mView = camera.GetViewMatrix();
  mView(0,3) = 0.0f;
  mView(1,3) = 0.0f;
  mView(2,3) = 0.0f;

  glPushMatrix();
  glLoadTransposeMatrixf( mView.GetArray());
  
  COglTexture *pTexture = *skybox.GetTextureHandle(0);
  CIndexArray *pIndices = *skybox.GetIndices();
  CVertexDescriptor *pTexCoords = *skybox.GetTextureCoordinateHandle(0);
  CVertexDescriptor *pVertices  = *skybox.GetVertexHandle();  

  if ( pVertices )  CommitVertexDescriptor( pVertices );
  if ( pTexCoords ) CommitVertexDescriptor( pTexCoords );
  if ( pTexture )   CommitTexture( 0, pTexture ); 
  if ( pIndices )   CommitPrimitive( pIndices );
  /////////////////////////////////////////////////////////////////
  
  glPopMatrix();
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitTransform( const Phoenix::Math::CTransform &transform )
{
  glPushMatrix();
  glMultTransposeMatrixf( const_cast<Phoenix::Math::CTransform &>(transform).GetMatrix().GetArray());
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::RollbackTransform()
{
  glPopMatrix();
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitSphere( const Phoenix::Volume::CSphere &sphere, int bWireframe )
{
  if ( !m_pQuadric )
    m_pQuadric = gluNewQuadric();

  // Set drawing style.
  if ( bWireframe )	gluQuadricDrawStyle( m_pQuadric, GLU_LINE  );
  else			gluQuadricDrawStyle( m_pQuadric, GLU_FILL  );

  // Translate and render
  glPushMatrix();
    glTranslatef( sphere.GetPosition()[0], sphere.GetPosition()[1], sphere.GetPosition()[2]);
    gluSphere(m_pQuadric, sphere.GetRadius(), 16, 16);
  glPopMatrix();


}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitQuad( const Phoenix::Spatial::CVertex &vertexOne,
					     const Phoenix::Spatial::CVertex &vertexTwo,
					     const Phoenix::Spatial::CVertex &vertexThree,
					     const Phoenix::Spatial::CVertex &vertexFour)
{
  // Handy macro for multiple similar commands
#define COMMIT_VERTEX( V ) {			\
  glColor4ubv( V.GetColor().GetArray() );	\
  glTexCoord2f( V.GetTextureCoordinates()[0],	\
		V.GetTextureCoordinates()[1]);	\
  glNormal3fv( V.GetNormal().GetArray() );	\
  glVertex3fv( V.GetPosition().GetArray() );	\
}

  glBegin(GL_QUADS);
    COMMIT_VERTEX( vertexOne );
    COMMIT_VERTEX( vertexTwo );
    COMMIT_VERTEX( vertexThree );
    COMMIT_VERTEX( vertexFour );
  glEnd();
#undef COMMIT_VERTEX
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRenderer::CommitCache( Phoenix::Graphics::CVertexDescriptor & rVertexDescriptor, Phoenix::Graphics::CACHE_ACCESS_TYPE tType )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return 1;
  
  if(  !rVertexDescriptor.IsCached() )
  {
    glGenBuffersARB(1, &rVertexDescriptor.GetCache());
  }
  
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, rVertexDescriptor.GetCache());

  // determine proper buffer size.
  switch ( rVertexDescriptor.GetType())
  {
  case ELEMENT_TYPE_VERTEX_3F:
  case ELEMENT_TYPE_COLOR_3F:
  case ELEMENT_TYPE_NORMAL_3F:
  case ELEMENT_TYPE_TEX_3F:
  case ELEMENT_TYPE_COLOR_4F:
  case ELEMENT_TYPE_TEX_4F:
  case ELEMENT_TYPE_TEX_2F:
  case ELEMENT_TYPE_V3F_N3F_T2F:
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, 
		     rVertexDescriptor.GetByteSize(), 
		     rVertexDescriptor.GetPointer<float>(), 
		     static_cast<GLenum>(tType));
    
  case ELEMENT_TYPE_COLOR_4UB:
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, 
		     rVertexDescriptor.GetByteSize(), 
		     rVertexDescriptor.GetPointer<unsigned char>(), 
		     static_cast<GLenum>(tType));
    break;
  case ELEMENT_TYPE_UNIFORM_1F:
  case ELEMENT_TYPE_UNIFORM_2F:
  case ELEMENT_TYPE_UNIFORM_3F:
  case ELEMENT_TYPE_UNIFORM_4F:
  case ELEMENT_TYPE_UNIFORM_1I:
  case ELEMENT_TYPE_UNIFORM_2I:
  case ELEMENT_TYPE_UNIFORM_3I:
  case ELEMENT_TYPE_UNIFORM_4I:
  case ELEMENT_TYPE_UNIFORM_2X2F:
  case ELEMENT_TYPE_UNIFORM_3X3F:
  case ELEMENT_TYPE_UNIFORM_4X4F:
  case ELEMENT_TYPE_ATTRIB_1F:
  case ELEMENT_TYPE_ATTRIB_2F:
  case ELEMENT_TYPE_ATTRIB_3F:
  case ELEMENT_TYPE_ATTRIB_4F:
  case ELEMENT_TYPE_ATTRIB_1I:
  case ELEMENT_TYPE_ATTRIB_2I:
  case ELEMENT_TYPE_ATTRIB_3I:
  case ELEMENT_TYPE_ATTRIB_4I:
  case ELEMENT_TYPE_ATTRIB_1UB:
  case ELEMENT_TYPE_ATTRIB_2UB:
  case ELEMENT_TYPE_ATTRIB_3UB:
  case ELEMENT_TYPE_ATTRIB_4UB:
    // these types do not need caching.
    return 3;
    break;
  }

  // Prepare for case that data does not fit
  if ( glGetError() == GL_OUT_OF_MEMORY )
  {
    glDeleteBuffersARB( 1, &rVertexDescriptor.GetCache());
    rVertexDescriptor.SetState(Phoenix::Core::CACHE_NOCACHE);
    return 2;
  }
  else
  {
    rVertexDescriptor.SetState(Phoenix::Core::CACHE_UP2DATE);
  }
  // Unset current cache buffer.
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );

  return 0;
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRenderer::CommitCache( Phoenix::Graphics::CIndexArray & rIndexArray,
					      Phoenix::Graphics::CACHE_ACCESS_TYPE tType  )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return 1;
  
  if(  !rIndexArray.IsCached() ) 
  {
    glGenBuffersARB(1, &rIndexArray.GetCache());
  }
  
  glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, rIndexArray.GetCache());

  if ( rIndexArray.IsShortIndices() )
  {
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 
		     sizeof(unsigned short int)*rIndexArray.GetNumIndices(), 
		     rIndexArray.GetPointer<unsigned short int>(), 
		     static_cast<GLenum>(tType));
  } 
  else 
  {
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 
		     sizeof(unsigned int)*rIndexArray.GetNumIndices(), 
		     rIndexArray.GetPointer<unsigned int>(), 
		     static_cast<GLenum>(tType));
  }
  // Prepare for case that data does not fit
  if ( glGetError() == GL_OUT_OF_MEMORY )
  {
    glDeleteBuffersARB( 1, &(rIndexArray.GetCache()));
    rIndexArray.SetState(Phoenix::Core::CACHE_NOCACHE);
    return 2;
  }
  else
  {
    rIndexArray.SetState(Phoenix::Core::CACHE_UP2DATE);
  }
  // Unset current cache buffer.
  glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );

  return 0;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::RollbackCache( Phoenix::Graphics::CVertexDescriptor & rVertexDescriptor )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return;
  
  if(  !rVertexDescriptor.IsCached() )
  {
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glDeleteBuffersARB( 1, &rVertexDescriptor.GetCache());
    rVertexDescriptor.SetState(Phoenix::Core::CACHE_NOCACHE);
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::RollbackCache( Phoenix::Graphics::CIndexArray & rIndexArray )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return;
  
  if(  !rIndexArray.IsCached() )
  {
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
    glDeleteBuffersARB( 1, &rIndexArray.GetCache());
    rIndexArray.SetState(Phoenix::Core::CACHE_NOCACHE);
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::DisableCaches()
{
  // disable VBO objects
  glBindBufferARB( GL_ARRAY_BUFFER_ARB,		0 );
  glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc,
						     const Phoenix::Math::CVector4<float> &vValues )
{
  CommitVertexAttrib( shader, iLoc, vValues[0], vValues[1], vValues[2], vValues[3]);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc,
						     const Phoenix::Math::CVector3<float> &vValues )
{
  CommitVertexAttrib( shader, iLoc, vValues[0], vValues[1], vValues[2]);
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexAttrib( Phoenix::Graphics::CShader &shader, int iLoc,
						     const Phoenix::Math::CVector2<float> &vValues )
{
  CommitVertexAttrib( shader, iLoc, vValues[0], vValues[1] );
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRenderer::LocateAttribShaderParam( Phoenix::Graphics::CShader &shader, const char *strParamName )
{
  return glGetAttribLocation( shader.GetProgram(), strParamName );  
}
/////////////////////////////////////////////////////////////////
int 
Phoenix::Graphics::COglRenderer::LocateUniformShaderParam( Phoenix::Graphics::CShader &shader, const char *strParamName )
{
  return glGetUniformLocation( shader.GetProgram(), strParamName );  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrameBufferObject * 
Phoenix::Graphics::COglRenderer::CreateFramebuffer( unsigned int nWidth, unsigned int nHeight, 
						    int iBufferFlags )
{
  GLuint iFBO;
  CFrameBufferObject *pFBO = NULL;

  ////////////////////
  // Create and bind framebuffer
  glGenFramebuffersEXT(1, &iFBO);
  pFBO = new CFrameBufferObject( iFBO, nWidth, nHeight );

  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pFBO->GetID());
  
  if ( iBufferFlags & FBO_DEPTH_BUFFER )
  {
    ////////////////////
    // Create depth buffer
    glGenRenderbuffersEXT(1, &pFBO->GetDepthBufferId());
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, pFBO->GetDepthBufferId());
    
    ////////////////////
    // Attach depth buffer to frame buffer.
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 
			     (unsigned int)pFBO->GetWidth(), (unsigned int)pFBO->GetHeight() );
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
				 GL_RENDERBUFFER_EXT, pFBO->GetDepthBufferId());
  }
  
  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  // Return object pointer.
  return pFBO;
}
/////////////////////////////////////////////////////////////////
int
Phoenix::Graphics::COglRenderer::AttachTextureToFramebuffer( Phoenix::Graphics::CFrameBufferObject & rFBO, 
							     const Phoenix::Default::TEXTURE_HANDLE & hTexture, 
							     unsigned int nColorBuffer )
{
  int iRetval = 1;
  if ( !hTexture.IsNull() )
  {

    // bind frame buffer.
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rFBO.GetID());

    // Duplicate handle to texture
    g_DefaultTextureManager->DuplicateHandle( hTexture, rFBO.GetTextureHandle( nColorBuffer ) );  
    ////////////////////
    // Attach texture to framebuffer
    COglTexture *pTexture = g_DefaultTextureManager->GetResource( rFBO.GetTextureHandle(nColorBuffer) );

    // Determine texture type
    GLenum iTexType = GetGLTextureType(pTexture->GetType());

    unsigned int nBufferNumber = nColorBuffer % TEXTURE_HANDLE_COUNT;
    /////////////////////////////////////////////////////////////////
    glBindTexture(iTexType, pTexture->GetID());

    glTexParameterf( iTexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf( iTexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf( iTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( iTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, 
			       GL_COLOR_ATTACHMENT0_EXT+nBufferNumber, 
			       iTexType, pTexture->GetID(), 0);
    
    /////////////////////////////////////////////////////////////////
    /// THIS MIGHT REQUIRE TINKERING. I don't know should filters be set
    /// before actually using the texture or not... if there are problems, 
    /// this might be the culprit.
    glGenerateMipmapEXT( iTexType );
    /////////////////////////////////////////////////////////////////
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    /// If there were any errors, clean up and return NULL.
    if ( status !=  GL_FRAMEBUFFER_COMPLETE_EXT )
    {
      iRetval = 1;      
    }
    // disable frame buffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    iRetval = 0;
  }
  
  return iRetval;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitFrameBuffer( const Phoenix::Graphics::CFrameBufferObject & rFBO, unsigned int nColorBufferCount )
{
  // Bind frame buffer.
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rFBO.GetID());

  // Define viewport to texture size and store current settings so they can be retrieved.
  // (This might not be necessary, but sounds resonably now)
  glPushAttrib(GL_VIEWPORT_BIT);
  glViewport(0,0, (unsigned int)rFBO.GetWidth(), (unsigned int)rFBO.GetHeight());

  // select render targets.
  // glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT) or glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT) or etc.
  // but this is better; it allows multiple buffers to be rendered via parameter.
  // Output to buffers must be controlled via GLSL fragment shaders.
  glDrawBuffers( nColorBufferCount % TEXTURE_HANDLE_COUNT, g_ColorBufferNames );
  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitFrameBufferSingle( const Phoenix::Graphics::CFrameBufferObject & rFBO, unsigned int nColorBuffer )
{
  // Bind frame buffer.
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rFBO.GetID());

  // Define viewport to texture size and store current settings so they can be retrieved.
  // (This might not be necessary, but sounds resonably now)
  glPushAttrib(GL_VIEWPORT_BIT);
  glViewport(0,0, (unsigned int)rFBO.GetWidth(), (unsigned int)rFBO.GetHeight());

  // select render target
  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT+nColorBuffer);
  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::RollbackFrameBuffer( const Phoenix::Graphics::CFrameBufferObject & rFBO )
{
  // Reset viewport settings
  glPopAttrib();
  // Put render commands back to ordinary buffer
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
/////////////////////////////////////////////////////////////////
/// A handle macro for commiting vertices with N 
#define COMMIT_COORDINATES( POSX, POSY, TEXX, TEXY ){	\
   glMultiTexCoord2fARB( GL_TEXTURE0_ARB, TEXX, TEXY);	\
   glMultiTexCoord2fARB( GL_TEXTURE1_ARB, TEXX, TEXY);	\
   glMultiTexCoord2fARB( GL_TEXTURE2_ARB, TEXX, TEXY);	\
   glMultiTexCoord2fARB( GL_TEXTURE3_ARB, TEXX, TEXY);	\
   glMultiTexCoord2fARB( GL_TEXTURE4_ARB, TEXX, TEXY);	\
   glMultiTexCoord2fARB( GL_TEXTURE5_ARB, TEXX, TEXY);	\
   glMultiTexCoord2fARB( GL_TEXTURE6_ARB, TEXX, TEXY);	\
   glMultiTexCoord2fARB( GL_TEXTURE7_ARB, TEXX, TEXY);	\
   glVertex2f( POSX, POSY );				\
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitQuad( const Phoenix::Graphics::CFrameBufferObject & rFBO )
{
  
  glBegin(GL_QUADS);

    COMMIT_COORDINATES( 0.0f,            0.0f,		0.0f,            0.0f);
    COMMIT_COORDINATES( rFBO.GetWidth(), 0.0f,		rFBO.GetWidth(), 0.0f);
    COMMIT_COORDINATES( rFBO.GetWidth(), rFBO.GetHeight(),rFBO.GetWidth(), rFBO.GetHeight() );
    COMMIT_COORDINATES( 0.0f,            rFBO.GetHeight(),0.0f,            rFBO.GetHeight());

  glEnd();
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitQuad( const Phoenix::Graphics::CCamera & rCamera, const Phoenix::Graphics::CFrameBufferObject & rFBO)
{
  glBegin(GL_QUADS);

    COMMIT_COORDINATES( 0.0f,				 0.0f,				0.0f,            0.0f);
    COMMIT_COORDINATES( rCamera.GetViewport()[2],	 0.0f,				rFBO.GetWidth(), 0.0f);
    COMMIT_COORDINATES( rCamera.GetViewport()[2],	 rCamera.GetViewport()[3],	rFBO.GetWidth(), rFBO.GetHeight() );
    COMMIT_COORDINATES( 0.0f,				 rCamera.GetViewport()[3],	0.0f,            rFBO.GetHeight());

  glEnd();
}
/////////////////////////////////////////////////////////////////
#undef COMMIT_COORDINATES
/////////////////////////////////////////////////////////////////

Phoenix::Graphics::CFontset *
Phoenix::Graphics::COglRenderer::CreateFontset( const char *sPathToFontFile, unsigned int nFontSize)
{

#define WHITESPACE 32
  // Fontset to be created.
  CFontset *	pFontset = NULL;
  /////////////////////////////////////////////////////////////////
  // Freetype stuff
  FT_Library	ftLibrary;
  FT_Error	ftError;
  FT_Face	ftFace;  

  ////////////////////
  // Initialize the font library
  ftError = FT_Init_FreeType( &ftLibrary ); 
  if ( ftError ) 
  { 
    cerr << "Error initializing FT" << endl;
    return NULL;
  }
  ////////////////////
  // Create new face
  ftError = FT_New_Face( ftLibrary,sPathToFontFile, 0, &ftFace);
  if ( ftError )
  {
    cerr << "Error loading face" << endl;
    return NULL;
  }
  ////////////////////
  // Set the character size
  ftError = FT_Set_Char_Size( ftFace, nFontSize << 6, 0, 0, 0 );        

  if ( ftError )
  {
    cerr << "Error setting font size" << endl;
    return NULL;
  }
  ////////////////////
  // Create fontset.
  pFontset = new CFontset();
  
  ////////////////////
  // Create display lists.
  pFontset->GetDisplayList() = glGenLists(Phoenix::Globals::MAX_FONT_CHARACTERS);
  
  for(unsigned int n=0;n<Phoenix::Globals::MAX_FONT_CHARACTERS;n++)
  {
    // Whitespace has no dimension, it needs special treatment.
    if ( n == WHITESPACE )
    {
      glNewList(pFontset->GetDisplayList()+n,GL_COMPILE);
      glTranslatef(nFontSize*0.2, 0, 0);
      glEndList();
      continue;
    } 
    ////////////////////
    // For some characters, such as i, a width fixing is 
    // necessary to make it visible.
    int		bWidthFixApplied = 0;
    FT_UInt	ftGlyphIndex;    
    ftGlyphIndex = FT_Get_Char_Index( ftFace, n );
    ////////////////////
    ftError = FT_Load_Glyph( ftFace, ftGlyphIndex, FT_LOAD_DEFAULT ); 
    if ( ftError ) 
    { 
      continue; /* ignore errors */
    }
    ////////////////////
    // convert to an anti-aliased bitmap.
    ftError = FT_Render_Glyph( ftFace->glyph, FT_RENDER_MODE_NORMAL ); 
    if ( ftError ) 
    { 
      continue;
    }
    
    if ( ftFace->glyph->bitmap.width == 0 ) continue;
    ////////////////////
    // Round dimensions up to closes power of two.
    float fLog2Height = log(ftFace->glyph->bitmap.rows)/log(2);
    float fLog2Width  = log(ftFace->glyph->bitmap.width)/log(2);
    ////////////////////
    // Rounding error check.
    if ( (fLog2Height - (int)fLog2Height) < EPSILON ) fLog2Height = (int)fLog2Height;
    if ( (fLog2Width  - (int)fLog2Width)  < EPSILON ) fLog2Width  = (int)fLog2Width;
    int iWidth  = (int)pow( 2, ceil(fLog2Width));
    int iHeight = (int)pow( 2, ceil(fLog2Height));

    ////////////////////
    // A fix for really narrow characters.
    if ( iWidth == 1 ) 
    {
      bWidthFixApplied = 1;
      iWidth = 2;
    }
    ////////////////////
    // Create two-channel data.
    GLubyte* expanded_data = new GLubyte[ 2 * iWidth * iHeight];
    for(int j=0; j <iHeight;j++) 
    {
      for(int i=0; i < iWidth; i++)
      {
	expanded_data[2*(i+j*iWidth)]= expanded_data[2*(i+j*iWidth)+1] = 
	(i>=ftFace->glyph->bitmap.width || j>=ftFace->glyph->bitmap.rows) ?
	0 : ftFace->glyph->bitmap.buffer[i + ftFace->glyph->bitmap.width*j];
      }
    }
    ////////////////////
    // Create texture.
    /*CreateTexture( iWidth, iHeight, TEXTURE_2D, expanded_data );*/
    
    unsigned int iTexId;
    glGenTextures( 1, &iTexId);
    COglTexture *pTexture = new COglTexture( iTexId, TEXTURE_2D );

    glActiveTextureARB( GL_TEXTURE0_ARB );
    glEnable( GL_TEXTURE_2D);
    glBindTexture  ( GL_TEXTURE_2D, pTexture->GetID());
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 4, iWidth, iHeight, 0,
		  GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
    
    delete [] expanded_data;
    ////////////////////
    // Add pointer to fontset's array as well.
    pFontset->GetTextures()[n] = pTexture;

    // --------------------------------------------------
    // face->glyph->bitmap->buffer;  // pointer to data
    // face->glyph->bitmap->width;   // pixels on a line
    // face->glyph->bitmap->rows;   // #lines
    // face->glyph->bitmap->pitch; // #|bytes| in line
    // --------------------------------------------------

    ////////////////////
    // Compile new display list.
    glNewList(pFontset->GetDisplayList()+n,GL_COMPILE);
    ////////////////////
    // Bind texture
    glBindTexture( GL_TEXTURE_2D, pTexture->GetID());
    glPushMatrix();    
    ////////////////////
    // move new char left so we have correct spacing.
    glTranslatef( ftFace->glyph->bitmap_left, 0,0 );
    ////////////////////
    // fix g and y chars
    glTranslatef(0,ftFace->glyph->bitmap_top-ftFace->glyph->bitmap.rows,0);

    float   fX = (float)ftFace->glyph->bitmap.width / (float)iWidth;
    float   fY = (float)ftFace->glyph->bitmap.rows / (float)iHeight;

    ////////////////////
    // Render actual character.
    glBegin(GL_QUADS);
      glTexCoord2f(0,  0  ); glVertex2f( 0,ftFace->glyph->bitmap.rows );
      glTexCoord2f(0,  fY ); glVertex2f( 0,0 );
      glTexCoord2f(fX, fY ); glVertex2f( ftFace->glyph->bitmap.width, 0);
      glTexCoord2f(fX, 0  ); glVertex2f( ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows );
    glEnd();
    glPopMatrix();
    ////////////////////
    // Advance along x-axis so next letter will be 
    // in correct place.
    if ( bWidthFixApplied )   glTranslatef(ftFace->glyph->advance.x >> 7, 0, 0);
    else		      glTranslatef(ftFace->glyph->advance.x >> 6, 0, 0);

    ////////////////////
    // End list
    glEndList();

  }
  ////////////////////
  // Cleanup freetype stuff.
  FT_Done_Face    ( ftFace );
  FT_Done_FreeType( ftLibrary );
  ////////////////////
  // return new fontset.
  return pFontset;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitString( CFontset & rFontSet, float fX, float fY, const char *szText )
{
  
  ////////////////////
  /// Sanity check.
  if ( szText == NULL )
  {
    cerr << "I shall not render a NULL string!" << endl;
    return;
  }

  glActiveTextureARB( GL_TEXTURE0_ARB );
  glEnable( GL_TEXTURE_2D );
  ////////////////////
  /// Store display list settings
  glPushAttrib( GL_LIST_BIT );

    // Activate preferred fontset
    glListBase( rFontSet.GetDisplayList() );      
    // Store current position
    glPushMatrix();
      glTranslatef( fX, fY, 0.0f);

      // render test
      glCallLists( strlen(szText), GL_UNSIGNED_BYTE, szText );


    glPopMatrix();
  
  glPopAttrib();
  glDisable( GL_TEXTURE_2D );
}
/////////////////////////////////////////////////////////////////
const COglRendererFeatures & 
Phoenix::Graphics::COglRenderer::GetFeatures()
{
  if ( m_pFeatures == NULL )
  {
    m_pFeatures = new COglRendererFeatures();
    assert( m_pFeatures != NULL && "Cannot create renderer features object!" );
  }
  return *m_pFeatures;
}
/////////////////////////////////////////////////////////////////
