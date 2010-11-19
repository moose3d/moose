////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <list>
#include <iostream>
#include "MooseGlobals.h"
#include "MooseAPI.h"
#if defined(MOOSE_APPLE_IPHONE)
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>
#endif
#include "MooseOGLRenderer.h"
#include "MooseAmbientLight.h"
#include "MooseSpotLight.h"
#include "MooseDirectionalLight.h"
#include "MoosePointLight.h"
#include "MooseTGAImage.h"
#include "MooseDDSImage.h"
#include "MooseDefaultEntities.h"
#include "MooseCapsule.h"
#include "MooseSphere.h"
#include "MooseMath.h"
#include <fstream>
#include <cassert>
#include "MooseLogger.h"


#if !defined(MOOSE_APPLE_IPHONE)
// include freetype stuff
#include <ft2build.h>
#include FT_FREETYPE_H
#endif
/////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
using namespace Moose::Default;
using namespace Moose::Spatial;
using namespace Moose::Core;
using namespace Moose::Math;
using namespace Moose::Volume;
using std::endl;
using std::cerr;
using std::ifstream;
using std::ios;
using std::string;
/////////////////////////////////////////////////////////////////
#define BUFFER_OFFSET(i) ((char *)NULL+(i))
/////////////////////////////////////////////////////////////////
/// Famous last words: Eight color buffers is enough for anyone :)
const GLenum g_ColorBufferNames[] = { 
#if defined(MOOSE_APPLE_IPHONE)
    GL_COLOR_ATTACHMENT0
#else
    GL_COLOR_ATTACHMENT0,
                      GL_COLOR_ATTACHMENT1,
				      GL_COLOR_ATTACHMENT2,
				      GL_COLOR_ATTACHMENT3,
				      GL_COLOR_ATTACHMENT4,
				      GL_COLOR_ATTACHMENT5,
				      GL_COLOR_ATTACHMENT6,
				      GL_COLOR_ATTACHMENT7 
#endif
};
/////////////////////////////////////////////////////////////////
#if defined(MOOSE_APPLE_IPHONE)
#define DISABLE_ALL_TEXTURES(){			\
glDisable(GL_TEXTURE_2D);			\
glDisable(GL_TEXTURE_CUBE_MAP);		\
}
#else
#define DISABLE_ALL_TEXTURES(){			\
    glDisable(GL_TEXTURE_2D);			\
    glDisable(GL_TEXTURE_CUBE_MAP);		\
    glDisable(GL_TEXTURE_RECTANGLE_ARB);	\
}
#endif
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
#if !defined(MOOSE_APPLE_IPHONE)
      case TEXTURE_RECT:
    iRetval = GL_TEXTURE_RECTANGLE_ARB;
    break;
#endif
   case TEXTURE_CUBE:
    iRetval = GL_TEXTURE_CUBE_MAP;
    break;
  }
  return iRetval;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CFontset::CFontset() : m_nDisplayLists(0)
{
  // initialize texture pointers to NULL
  for( size_t t=0;t<Moose::Globals::MAX_FONT_CHARACTERS;++t)
  {
    m_ppTextures[t] = NULL;
  }
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CFontset::~CFontset()
{
  for( size_t t=0;t<Moose::Globals::MAX_FONT_CHARACTERS;++t)
  {
     // delete textures, this is static array of pointers. delete only pointed textures.
     delete m_ppTextures[0];
  }
#if !defined(MOOSE_APPLE_IPHONE)
    // Release lists
  glDeleteLists( GetDisplayList(), Moose::Globals::MAX_FONT_CHARACTERS );
#endif
}
/////////////////////////////////////////////////////////////////
GLuint &
Moose::Graphics::CFontset::GetDisplayList()
{
  return m_nDisplayLists;
}
/////////////////////////////////////////////////////////////////
const GLuint &
Moose::Graphics::CFontset::GetDisplayList() const
{
  return m_nDisplayLists;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture **
Moose::Graphics::CFontset::GetTextures()
{
  return m_ppTextures;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglRendererFeatures::COglRendererFeatures()
{
  Init();
#if defined(MOOSE_APPLE_IPHONE)
    m_bARB_vertex_program  = true;
    m_bARB_vertex_shader   = true;
    m_bARB_fragment_shader = true;
    m_bARB_shader_objects  = true;
    m_bEXT_vertex_array    = false;
    m_bARB_multitexture    = true;
    m_bARB_vertex_buffer_object     = false;
    m_bEXT_framebuffer_object       = true;
    m_iMaxLights  = 0;
    m_iMaxElementsVertices = 0;
    m_iMaxElementsIndices = 0;
    m_iMaxColorAttachments = 1;
    m_iMaxDrawBuffers = 1;
    
#else
  // Check for required extensions:
  m_bARB_vertex_program  = GLEW_ARB_vertex_program;
  m_bARB_vertex_shader   = GLEW_ARB_vertex_shader;
  m_bARB_fragment_shader = GLEW_ARB_fragment_shader;
  m_bARB_shader_objects  = GLEW_ARB_shader_objects;
  m_bEXT_vertex_array    = GLEW_EXT_vertex_array;
  m_bARB_multitexture    = GLEW_ARB_multitexture;

  m_bARB_vertex_buffer_object     = GLEW_ARB_vertex_buffer_object;
  m_bEXT_framebuffer_object       = GLEW_EXT_framebuffer_object;
  m_bEXT_texture_compression_s3tc = GLEW_EXT_texture_compression_s3tc;
    glGetIntegerv( GL_MAX_LIGHTS,            &m_iMaxLights );
    glGetIntegerv( GL_MAX_ELEMENTS_VERTICES, &m_iMaxElementsVertices);
    glGetIntegerv( GL_MAX_ELEMENTS_INDICES,  &m_iMaxElementsIndices);
    glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS_EXT, &m_iMaxColorAttachments );
    glGetIntegerv( GL_MAX_DRAW_BUFFERS,      &m_iMaxDrawBuffers );
    
#endif
    
  glGetIntegerv( GL_SAMPLE_BUFFERS, 	   &m_iMultiSampleBuffers);
  glGetIntegerv( GL_SAMPLES, 		   &m_iMultiSamples);
  glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,    &m_iMaxVertexAttribs);

}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRendererFeatures::Init()
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
  m_iMaxVertexAttribs = 0;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasVertexProgram() const
{
  return m_bARB_vertex_program;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasVertexShader() const
{
  return m_bARB_vertex_shader;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasFragmentShader() const
{
  return m_bARB_fragment_shader;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasVertexArray() const
{
  return m_bEXT_vertex_array;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasVertexBufferObject() const
{
  return m_bARB_vertex_buffer_object;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasMultitexture() const
{
  return m_bARB_multitexture;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasShaderObjects() const
{
  return m_bARB_shader_objects;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasFramebufferObjects() const
{
  return m_bEXT_framebuffer_object;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::COglRendererFeatures::HasTextureCompressionS3TC() const
{
  return m_bEXT_texture_compression_s3tc;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMaxLights() const
{
  return m_iMaxLights;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMaxElementsVertices() const
{
  return m_iMaxElementsVertices;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMaxElementsIndices() const
{
  return m_iMaxElementsIndices;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMaxColorAttachments() const
{
  return m_iMaxColorAttachments;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMaxDrawBuffers() const
{
  return m_iMaxDrawBuffers;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMultiSampleBuffers() const
{
	return m_iMultiSampleBuffers;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMultiSampleSamples() const
{
	return m_iMultiSamples;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRendererFeatures::GetMaxVertexAttribs() const
{
  return m_iMaxVertexAttribs;
}
/////////////////////////////////////////////////////////////////
const char *
Moose::Graphics::COglRendererFeatures::GetVendor() const
{
  return reinterpret_cast<const char*>(glGetString( GL_VENDOR ));
}
/////////////////////////////////////////////////////////////////
const char *
Moose::Graphics::COglRendererFeatures::GetRenderer() const
{
  return reinterpret_cast<const char *>(glGetString( GL_RENDERER ));
}
/////////////////////////////////////////////////////////////////
#ifndef SWIG
std::ostream &
Moose::Graphics::operator<<(std::ostream &stream, const COglRendererFeatures &obj)
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
  stream << "GL_SAMPLE_BUFFERS = " << obj.GetMultiSampleBuffers() << std::endl;
  stream << "GL_SAMPLES = " << obj.GetMultiSampleSamples() << std::endl;
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
#endif
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglRenderer::COglRenderer() : m_pFeatures(NULL), m_pCamera(NULL)
#if !defined(MOOSE_APPLE_IPHONE)
                                                ,  m_pQuadric(NULL)
#endif
{
    #if !defined(MOOSE_APPLE_IPHONE)
  GLenum status = glewInit();
  if ( status != GLEW_OK  )
  {
    g_Error << "COglRenderer: Error initializing GLEW: " << glewGetErrorString(status)<< "\n";
  }
#endif

}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglRenderer::~COglRenderer()
{
  if ( m_pFeatures )  delete m_pFeatures;
#if !defined(MOOSE_APPLE_IPHONE)
  if ( m_pQuadric  )  gluDeleteQuadric(m_pQuadric);
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::ClearBuffer(Moose::Graphics::BUFFER_TYPE tType)
{
  switch( tType )
  {
  case Moose::Graphics::COLOR_BUFFER:
    glClear(GL_COLOR_BUFFER_BIT);
    break;
  case Moose::Graphics::DEPTH_BUFFER:
    glClear(GL_DEPTH_BUFFER_BIT);
    break;
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::Finalize()
{
  //glFinish();
  // disable textures, these tend to make nasty problems.
  for( int i=0;i<TEXTURE_HANDLE_COUNT;i++)
  {
    glActiveTexture( GL_TEXTURE0+i);
    DISABLE_ALL_TEXTURES();

  }
  // Reset camera.
  m_pCamera = NULL;
  // Remember call buffer swapping from another source.
}
/////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
// For iPhone platform we do things in different manner; shaders are 
// required for everything so vertexdescriptors are bound as shader parameters.
void
Moose::Graphics::COglRenderer::CommitVertexDescriptor( CVertexDescriptor *pBuffer, unsigned int nId )
{

  return;


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
    if ( nId < TEXTURE_HANDLE_COUNT ) { glClientActiveTexture( GL_TEXTURE0 + nId); }
    else                              { glClientActiveTexture( GL_TEXTURE0);       }

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    // check if this was previously set
    if ( GetRenderState().IsCurrentTexCoord( nId, pBuffer ) ) break;


    if ( pBuffer->IsCached())
    {
      glBindBuffer( GL_ARRAY_BUFFER, pBuffer->GetCache() );
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

      glTexCoordPointer(2, GL_FLOAT, pBuffer->GetElementByteSize(), pBuffer->GetPointer<float>()+6);
      glNormalPointer(     GL_FLOAT, pBuffer->GetElementByteSize(), pBuffer->GetPointer<float>()+3);
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
Moose::Graphics::COglRenderer::RollbackVertexDescriptor( CVertexDescriptor *pBuffer, unsigned int nId )
{
  return;
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
#endif
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitPrimitive( CIndexArray *pIndexBuffer )
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
#if !defined(MOOSE_APPLE_IPHONE)
  case PRIMITIVE_QUAD_LIST:
    glPrimitive = GL_QUADS;
    break;
  case PRIMITIVE_QUAD_STRIP:
    glPrimitive = GL_QUAD_STRIP;
    break;
#endif
  }
  //bool bIsActive = (GetRenderState().m_pIndexArray == pIndexBuffer ) ;
  GLenum iIndexBufferType;
  GLvoid *pIndices = pIndexBuffer->IsCached() ? 0 :  pIndexBuffer->GetPointer<void>();
  if ( pIndexBuffer->IsCached() && 
       pIndexBuffer->GetState() == Moose::Core::CACHE_REFRESH ) 
  {
    pIndexBuffer->UpdateCache();
  }
  
  ////////////////////
  // Check data type and render.
  if ( pIndexBuffer->IsShortIndices() ) iIndexBufferType = GL_UNSIGNED_SHORT;
  else                                  iIndexBufferType = GL_UNSIGNED_INT;

  if ( true || !GetRenderState().IsCurrentIndices( pIndexBuffer ) )
  {
    /*#if !defined(MOOSE_APPLE_IPHONE)      
    if ( pIndices == 0 )
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, pIndexBuffer->GetCache());
    else
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
      #else*/
      if ( pIndices == 0 )
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer->GetCache());
      else
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
      
      //#endif
      // store current indexbuffer pointer to renderstate
    GetRenderState().SetCurrentIndices(pIndexBuffer);
  }
  glDrawElements( glPrimitive, pIndexBuffer->GetDrawableCount(),
		  iIndexBufferType,  pIndices);

}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitPrimitive( Moose::Graphics::PRIMITIVE_TYPE nPrimitive, size_t nStart, size_t nCount )
{
  GLenum glPrimitive = GL_POINTS;
  ////////////////////
  switch ( nPrimitive )
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
#if !defined(MOOSE_APPLE_IPHONE)
  case PRIMITIVE_QUAD_LIST:
    glPrimitive = GL_QUADS;
    break;
  case PRIMITIVE_QUAD_STRIP:
    glPrimitive = GL_QUAD_STRIP;
    break;
#endif
  }
  glDrawArrays( glPrimitive, nStart, nCount);
}
/////////////////////////////////////////////////////////////////
#if !defined(MOOSE_APPLE_IPHONE)
void
Moose::Graphics::COglRenderer::CommitVertex( const Moose::Spatial::CVertex & rVertex, int iIgnoreFlags )
{
  return;
  // Set multitexture coordinates

  if ( !(iIgnoreFlags & VERTEX_COMP_TEXCOORD) )
  {
    for(int i=0;i<TEXTURE_HANDLE_COUNT;i++)
    {
      glMultiTexCoord2f( GL_TEXTURE0+i,
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
Moose::Graphics::COglRenderer::DisableClientState( CLIENT_STATE_TYPE tType )
{
  return;
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
    glClientActiveTexture( GL_TEXTURE0 + (tType-CLIENT_STATE_TEX0_ARRAY));
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    break;
  }
}

/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::EnableClientState( CLIENT_STATE_TYPE tType )
{
  return;
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
    glClientActiveTexture( GL_TEXTURE0 + (tType-CLIENT_STATE_TEX0_ARRAY));
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    break;
  }
}
#endif
/////////////////////////////////////////////////////////////////

#if !defined(MOOSE_APPLE_IPHONE)
Moose::Graphics::COglTexture *
Moose::Graphics::COglRenderer::CreateTexture( const std::string &strFilename, TEXTURE_TYPE tType  )
{
  int             width, height;
  COglTexture *pTexture = NULL;

  ////////////////////
#define CLEANUP() { if ( pImage ) delete pImage; pImage = NULL; return pTexture; }
  ////////////////////
  CTGAImage *pImage = new CTGAImage();
  void *data = NULL;
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

  width = pImage->GetWidth();
  height = pImage->GetHeight();
  data =pImage->GetImg();

  // create texture
  GLuint iTexId;

  glGenTextures( 1, &iTexId);
  pTexture = new COglTexture( iTexId, tType );

  // set texture dimensions
  pTexture->SetHeight( height);
  pTexture->SetWidth( width);

  // check memory allocation
  if ( !pTexture )
  {
    std::cerr << "Failed to allocate memory while loading file '"
              << strFilename << "'" << std::endl;
    return NULL;
  }

  GLenum iGLType = GetGLTextureType( tType );

  // create actual gl texture
//  glEnable( iGLType );
  glBindTexture(iGLType, pTexture->GetID());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,	GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,	GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  /// ****************************************
  /// build mipmaps automatically.
  /// This is required because of nvidia 64-bit bug related to gluBuild2DMipmaps?
  /// Somehow it prevents it occurring.
  /// ****************************************

  glTexParameteri( iGLType, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexImage2D( iGLType, 0, iGLformat, /* OpenGL ES 2.0 requirement internal must be equal to glformat!*/
		width, height, 0,
		iGLformat, GL_UNSIGNED_BYTE, data);
  // build mipmaps
  /*gluBuild2DMipmaps(iGLType, iGLInternalFormat,
    pImage->GetWidth(), pImage->GetHeight(),
    iGLformat, GL_UNSIGNED_BYTE, pImage->GetImg());*/

  GLenum err = GL_NO_ERROR;
  glDisable( iGLType );
  if ( (err = glGetError() ) != GL_NO_ERROR )
  {
    g_Error << "Texture creation error: " << err << endl;
  }
  // ReleaseMemory
  CLEANUP();

#undef CLEANUP
}
#else
////////////////////////////////////////////////////////////////////////////////
/// Apple iPhone PNG implementation
#define CLEANUP() { CGContextRelease(cgContext); free(data); CGColorSpaceRelease(colorSpace); return pTexture; }
Moose::Graphics::COglTexture *
Moose::Graphics::COglRenderer::CreateTexture( const std::string &strFilename, TEXTURE_TYPE tType  )
{

int             width, height;
COglTexture *pTexture = NULL;


  /* APPLE IPHONE CODE */

  // Right, boys and girls, this is Objective-C++!
  UIImage *       image = nil;
  CGImageRef      cgImage;
  GLubyte *       data = nil;
  CGContextRef    cgContext;
  CGColorSpaceRef colorSpace;

  GLenum err;

  
  NSString *path =  [ [NSString alloc] initWithUTF8String:strFilename.c_str() ];
  
  image = [UIImage imageWithContentsOfFile:path];
  
  
  if (image == nil)
  {
    g_Error << "Failed to load '" << strFilename<< "'" << endl;
    return NULL;
  }
  
  cgImage = [image CGImage];
  width = CGImageGetWidth(cgImage);
  height = CGImageGetHeight(cgImage);
  colorSpace = CGColorSpaceCreateDeviceRGB();
  
  // Malloc may be used instead of calloc if your cg image has dimensions equal to the dimensions of the cg bitmap context
  data = (GLubyte *)calloc(width * height * 4, sizeof(GLubyte));
  cgContext = CGBitmapContextCreate(data, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedLast);
  if (cgContext == NULL) 
  {
    CLEANUP();
  }
  // Set the blend mode to copy. We don't care about the previous contents.
  CGContextSetBlendMode(cgContext, kCGBlendModeCopy);
  CGContextDrawImage(cgContext, CGRectMake(0.0f, 0.0f, width, height), cgImage);
  ////////////////////
  int    iGLInternalFormat = 3;
  GLenum iGLformat = GL_RGB;
  ////////////////////
  // Check correct depth
  switch ( CGImageGetBitsPerPixel(cgImage) )
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
    
    std::cerr << "Not 8, 16, 24 or 32 BBP image (was " 
              <<  CGImageGetBitsPerPixel(cgImage)
              << "):  '" << strFilename << "'" << std::endl;
    CLEANUP();
  }
  g_Error << "Internalformat: " << iGLInternalFormat << std::endl;
  g_Error << "size :"  << width << "x" << height << std::endl;
  // create texture
  GLuint iTexId;

  glGenTextures( 1, &iTexId);
  pTexture = new COglTexture( iTexId, tType );

  // set texture dimensions
  pTexture->SetHeight( height);
  pTexture->SetWidth( width);

  // check memory allocation
  if ( !pTexture )
  {
    std::cerr << "Failed to allocate memory while loading file '"
              << strFilename << "'" << std::endl;
    return NULL;
  }

  GLenum iGLType = GetGLTextureType( tType );

  // create actual gl texture
//  glEnable( iGLType );
  glBindTexture(iGLType, pTexture->GetID());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,	GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,	GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  /// ****************************************
  /// build mipmaps automatically.
  /// This is required because of nvidia 64-bit bug related to gluBuild2DMipmaps?
  /// Somehow it prevents it occurring.
  /// ****************************************
#if !defined(MOOSE_APPLE_IPHONE)
  glTexParameteri( iGLType, GL_GENERATE_MIPMAP, GL_TRUE);
#endif
  glTexImage2D( iGLType, 0, iGLformat, /* OpenGL ES 2.0 requirement internal must be equal to glformat!*/
		width, height, 0,
		iGLformat, GL_UNSIGNED_BYTE, data);
  // build mipmaps
#if defined(MOOSE_APPLE_IPHONE)
  //glGenerateMipmap(iGLType);
#endif
  /*gluBuild2DMipmaps(iGLType, iGLInternalFormat,
    pImage->GetWidth(), pImage->GetHeight(),
    iGLformat, GL_UNSIGNED_BYTE, pImage->GetImg());*/

  
  glDisable( iGLType );
  if ( (err = glGetError() ) != GL_NO_ERROR )
  {
    g_Error << "Texture creation error: " << err << endl;
  }
  // ReleaseMemory
  CLEANUP();

#undef CLEANUP

}
#endif
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture *
Moose::Graphics::COglRenderer::CreateCompressedTexture( const char *strFilename, TEXTURE_TYPE tType  )
{
#if defined(MOOSE_APPLE_IPHONE)
    g_Error << "CreateCompressedTexture: Not implemented" << endl;
    return NULL;
#else
    
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
#endif
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture *
Moose::Graphics::COglRenderer::CreateCubeTexture( const char * szFileNegX,
                                                  const char * szFilePosX,
                                                  const char * szFilePosY,
                                                  const char * szFileNegY,
                                                  const char * szFileNegZ,
                                                  const char * szFilePosZ )
{
  const char *szArray[6] = { szFileNegX,
                             szFilePosX,
                             szFilePosY,
                             szFileNegY,
                             szFileNegZ,
                             szFilePosZ };
  return CreateCubeTexture( szArray );
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture *
Moose::Graphics::COglRenderer::CreateCubeTexture( const char * szFiles[6] )
{
#define CLEANUP() { CGContextRelease(cgContext); free(data); CGColorSpaceRelease(colorSpace); return pTexture; }
  // Files must not be null
  if ( szFiles == NULL ) return NULL;

  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  COglTexture *pTexture = new COglTexture( iTexId, TEXTURE_CUBE );


  // Right, boys and girls, this is Objective-C++!


#if defined(MOOSE_APPLE_IPHONE)
  GLubyte *       data = nil;
  UIImage *       image = nil;
  CGImageRef      cgImage;
  CGContextRef    cgContext;
  CGColorSpaceRef colorSpace;

#endif
  //GLenum err = GL_NO_ERROR;
  int width, height;

  for( size_t i=0;i<6;i++)
  {
#if defined(MOOSE_APPLE_IPHONE)
    NSString *path =  [ [NSString alloc] initWithUTF8String:szFiles[i] ];
  
    image = [UIImage imageWithContentsOfFile:path];
  
  
    if (image == nil)
    {
      g_Error << "Failed to load '" << szFiles[i]<< "'" << endl;
      return NULL;
    }
  
    cgImage = [image CGImage];
    width = CGImageGetWidth(cgImage);
    height = CGImageGetHeight(cgImage);
    colorSpace = CGColorSpaceCreateDeviceRGB();
  
    // Malloc may be used instead of calloc if your cg image has dimensions equal to the dimensions of the cg bitmap context
    data = (GLubyte *)calloc(width * height * 4, sizeof(GLubyte));
    cgContext = CGBitmapContextCreate(data, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedLast);
    if (cgContext == NULL) 
    {
      CLEANUP();
    }
    // Set the blend mode to copy. We don't care about the previous contents.
    CGContextSetBlendMode(cgContext, kCGBlendModeCopy);
    CGContextDrawImage(cgContext, CGRectMake(0.0f, 0.0f, width, height), cgImage);
    ////////////////////
    int    iGLInternalFormat = 3;
    GLenum iGLformat = GL_RGB;
    ////////////////////
    // Check correct depth
    switch ( CGImageGetBitsPerPixel(cgImage) )
    {
    case 8:
      iGLInternalFormat = GL_LUMINANCE;
      iGLformat = GL_LUMINANCE;
      break;
    case 16:
      iGLInternalFormat = GL_LUMINANCE_ALPHA;
      iGLformat = GL_LUMINANCE_ALPHA;
      break;
    case 24:
      iGLInternalFormat = GL_RGB;
      iGLformat = GL_RGB;
      break;
    case 32:
      iGLInternalFormat = GL_RGBA;
      iGLformat = GL_RGBA;
      std::cerr << "Texture " << szFiles[i] << " has alpha channel." <<  std::endl;
      break;
    default:
    
      std::cerr << "Not 8, 16, 24 or 32 BBP image (was " 
                <<  CGImageGetBitsPerPixel(cgImage)
                << "):  '" << szFiles[i] << "'" << std::endl;
      CLEANUP();
    }

#else
    
    ////////////////////
    CTGAImage *pImage = CTGAImage::LoadTGAImage( szFiles[i] );
    if ( pImage == NULL )
    {
      g_Error << __FUNCTION__ << " : Unable to load TGA image for CUBE TEXTURE." << endl;
      delete pTexture;
      return NULL;
    }
    width = pImage->GetWidth();
    height = pImage->GetHeight();
    ////////////////////
    int    iGLInternalFormat = GL_RGB;
    GLenum iGLformat = GL_RGB;
    ////////////////////
    // Check correct depth
    switch (pImage->GetBPP())
    {
    case 8:
      iGLInternalFormat = GL_LUMINANCE;
      iGLformat = GL_LUMINANCE;
      break;
    case 16:
      iGLInternalFormat = GL_LUMINANCE_ALPHA;
      iGLformat = GL_LUMINANCE_ALPHA;
      break;
    case 24:
      iGLInternalFormat = GL_RGB;
      iGLformat = GL_RGB;
      break;
    case 32:
      iGLInternalFormat = GL_RGBA;
      iGLformat = GL_RGBA;
      //std::cerr << "Texture " << szFiles[i] << " has alpha channel." <<  std::endl;
      break;
    default:
      delete pImage;
      std::cerr << "Not 8, 16, 24 or 32 BBP image (was "
                << pImage->GetBPP() << "):  '" << szFiles[i] << "'"
                << std::endl;
      break;
    }
#endif
    //g_Error << "Internalformat: " << iGLInternalFormat << std::endl;
    //g_Error << "size :"  << width << "x" << height << std::endl;
    GLenum iGLType = GL_TEXTURE_CUBE_MAP;
    ////////////////////
    // create actual gl texture
    glEnable( iGLType );
    glBindTexture( iGLType, pTexture->GetID());
    ////////////////////
    // Set default texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,	GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,	GL_CLAMP_TO_EDGE);
#if !defined(MOOSE_APPLE_IPHONE)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,	GL_CLAMP_TO_EDGE);
#endif
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

#if defined(MOOSE_APPLE_IPHONE)
    ////////////////////
    // Create texture.
    glTexImage2D( cubeFace, 0, iGLInternalFormat,
                  width, height, 0,
                  iGLformat,  GL_UNSIGNED_BYTE, data);
    // release data

    free(data); 

    CGContextRelease(cgContext); 
    CGColorSpaceRelease(colorSpace);
#else
    glTexImage2D( cubeFace, 0, iGLInternalFormat,
                  width, height, 0,
                  iGLformat,  GL_UNSIGNED_BYTE, pImage->GetImg());
    delete pImage;
#endif

  } // for ( size_t
  return pTexture;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture *
Moose::Graphics::COglRenderer::CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType, TEXTURE_FORMAT tFormat )
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
#if !defined(MOOSE_APPLE_IPHONE)
    glTexParameteri( iGLType, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
#endif
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
Moose::Graphics::COglTexture *
Moose::Graphics::COglRenderer::CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType, void *pData, TEXTURE_FORMAT tFormat)
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
Moose::Graphics::COglRenderer::CommitTexture( unsigned int nTexUnit, COglTexture *pTexture )
{
  glActiveTexture( GL_TEXTURE0 + nTexUnit);
  glEnable( GetGLTextureType( pTexture->GetType() ) );
  if ( ! GetRenderState().IsCurrentTexture( nTexUnit, pTexture) )
  {
    // Bind texture
    glBindTexture( GetGLTextureType( pTexture->GetType() ), pTexture->GetID() );
    // Set texture pointer to renderstate
    GetRenderState().SetCurrentTexture( nTexUnit, pTexture);
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::DisableTexture( unsigned int nTexUnit, COglTexture *pTexture )
{
  glActiveTexture( GL_TEXTURE0 + nTexUnit);
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
Moose::Graphics::COglRenderer::CommitCamera( CCamera &camera )
{
  m_pCamera = &camera;
  ////////////////////
  // Handle updating of view and projection, if they are changed.
  if ( camera.IsProjectionChanged()) camera.UpdateProjection();
  if ( camera.IsViewChanged()) camera.UpdateView();

  Viewport_t & rViewport = camera.GetViewport();
  glViewport(rViewport.x, rViewport.y, rViewport.width, rViewport.height);

 #if !defined(MOOSE_APPLE_IPHONE)
  // these need to be fed to shaders.
  // glMatrixMode( GL_PROJECTION );
  //glLoadIdentity();
  //glMultTransposeMatrixf( camera.GetProjectionMatrix().GetArray());
  ////////////////////
  /// Set up proper position.
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //glMultTransposeMatrixf( camera.GetViewMatrix().GetArray());
#endif

}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CCamera *
Moose::Graphics::COglRenderer::GetCurrentCamera() const
{
  return m_pCamera;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitRenderable( CRenderable &renderable )
{
  renderable.Render( *this );

}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitFilter( TEXTURE_FILTER tFilter, TEXTURE_TYPE tType )
{
  GLenum glTarget = GetGLTextureType( tType );
  ////////////////////
  switch( tFilter )
  {

#if defined(MOOSE_APPLE_IPHONE)
  case ENV_MODULATE:
  case ENV_DECAL:
  case ENV_REPLACE:
  case ENV_BLEND:
  case ENV_COMBINE_INCR:
  case ENV_COMBINE_REPLACE:
    break;
#else
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
#endif
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
  case R_WRAP_REPEAT:
#if !defined(MOOSE_APPLE_IPHONE)
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);
#endif
    break;

  case T_WRAP_CLAMP:
#if !defined(MOOSE_APPLE_IPHONE)
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif
    break;
  case S_WRAP_CLAMP:
#if !defined(MOOSE_APPLE_IPHONE)
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
#endif
    break;
  case R_WRAP_CLAMP:
#if !defined(MOOSE_APPLE_IPHONE)
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_R, GL_CLAMP);
#endif
    break;
  case T_WRAP_CLAMP_TO_EDGE:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    break;
  case S_WRAP_CLAMP_TO_EDGE:
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    break;
  case R_WRAP_CLAMP_TO_EDGE:
#if !defined(MOOSE_APPLE_IPHONE)
    glTexParameteri( glTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
    break;
  }
  //  case ENV_COLOR:
  //glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, pNode->GetTexEnvColor().GetValues());
  //break;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitFilters( const std::vector<Moose::Graphics::TEXTURE_FILTER> &vecFilters, Moose::Graphics::TEXTURE_TYPE tType )
{
  // Apply texture filters.
  for(unsigned int nFilter=0; nFilter<vecFilters.size(); nFilter++)
  {
    CommitFilter( vecFilters[nFilter], tType );
  }
}
/////////////////////////////////////////////////////////////////
inline int
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
void
Moose::Graphics::COglRenderer::CommitShader( CShader *pShader )
{
  if ( pShader != NULL )
  {
    if ( !GetRenderState().IsCurrentShader(pShader) )
    {
      glUseProgram( pShader->GetProgram() );
      GetRenderState().SetCurrentShader(pShader);
    } 
  }
  else 
  {
    glUseProgram( 0 );
    GetRenderState().SetCurrentShader(NULL);
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const std::string &strParamName, int iValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName.c_str());
  if ( iLoc != -1 )
    glUniform1i( iLoc, iValue );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const std::string &strParamName, float fValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName.c_str());
  if ( iLoc != -1 )
    glUniform1f( iLoc, fValue );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitShaderParam( CShader &shader, const std::string &strParamName, const CVertexDescriptor & vParam)
{
  CommitShaderParam( shader, strParamName.c_str(), vParam );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const char *strParamName, int iValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName);
  if ( iLoc != -1 )
    glUniform1i( iLoc, iValue );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitUniformShaderParam( CShader &shader, const char *strParamName, float fValue )
{
  int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName);
  if ( iLoc != -1 )
    glUniform1f( iLoc, fValue );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitShaderParam( CShader &shader, const char *strParamName, const CVertexDescriptor & vParam )
{

  if ( vParam.GetType() >= ELEMENT_TYPE_ATTRIB_1F && vParam.GetType() <= ELEMENT_TYPE_ATTRIB_4UB )
  {
    int iLoc = glGetAttribLocation(shader.GetProgram(), strParamName);
    if ( iLoc == -1 )
    {
      cerr << "No such parameter '" << strParamName << "' in this shader! Are you sure it's vertex attribute param and not uniform parameter?" << endl;
    }
    else
    {
      glEnableVertexAttribArray(iLoc);
      glBindBuffer( GL_ARRAY_BUFFER, 0 );

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
	cerr << "for param" << strParamName << endl;
	assert( 0 && "Unknown parameter type (vertexattrib)" );
	break;
      }
    }
  }
  else if ( vParam.GetType() >= ELEMENT_TYPE_UNIFORM_1F && vParam.GetType() <= ELEMENT_TYPE_UNIFORM_4X4F )
  {
    int iLoc = glGetUniformLocation( shader.GetProgram(), strParamName);
    if ( iLoc == -1 )
    {
      cerr << "No such parameter '" << strParamName << "' in this shader! Are you sure that is uniform parameter and not vertex attribute?" << endl;
    }
    else
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
	cerr << "for param" << strParamName << endl;
	assert( 0 && "Unknown parameter type (uniform)" );
	break;
      }
    }
  }




}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitVertexAttrib( Moose::Graphics::CShader &shader, int iLoc, float fX, float fY, float fZ, float fW )
{
  if( iLoc != -1 )
  {
    glVertexAttrib4f( iLoc, fX, fY, fZ, fW );
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitVertexAttrib( Moose::Graphics::CShader &shader, int iLoc, float fX, float fY, float fZ )
{
  if( iLoc != -1 )
  {
    glVertexAttrib3f( iLoc, fX, fY, fZ );
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitVertexAttrib( Moose::Graphics::CShader &shader, int iLoc, float fX, float fY )
{
  if( iLoc != -1 )
  {
    glVertexAttrib2f( iLoc, fX, fY );
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitVertexAttrib( Moose::Graphics::CShader &shader, int iLoc, float fX )
{
  if( iLoc != -1 )
  {
    glVertexAttrib1f( iLoc, fX );
  }
}
/////////////////////////////////////////////////////////////////
/// Sets diffuse, ambient and specular values of a light.
inline void CommitLightColors( const CLight & light, GLenum iLightID )
{

#if !defined(MOOSE_APPLE_IPHONE)
  //float aTempVector[4];
	// // Set diffuse RGBA intensity
   	// aTempVector[0] = (float)light.GetDiffuseColor()[0]/255.0f;
	// aTempVector[1] = (float)light.GetDiffuseColor()[1]/255.0f;
	// aTempVector[2] = (float)light.GetDiffuseColor()[2]/255.0f;
	// aTempVector[3] = (float)light.GetDiffuseColor()[3]/255.0f;
	// glLightfv(iLightID, GL_DIFFUSE, aTempVector);

	// // Set ambient RGBA intensity
	// aTempVector[0] = (float)light.GetAmbientColor()[0]/255.0f;
	// aTempVector[1] = (float)light.GetAmbientColor()[1]/255.0f;
	// aTempVector[2] = (float)light.GetAmbientColor()[2]/255.0f;
	// aTempVector[3] = (float)light.GetAmbientColor()[3]/255.0f;
	// glLightfv(iLightID, GL_AMBIENT, aTempVector);

	// // Set specular RGBA intensity
	// aTempVector[0] = (float)light.GetSpecularColor()[0]/255.0f;
	// aTempVector[1] = (float)light.GetSpecularColor()[1]/255.0f;
	// aTempVector[2] = (float)light.GetSpecularColor()[2]/255.0f;
	// aTempVector[3] = (float)light.GetSpecularColor()[3]/255.0f;
	// glLightfv(iLightID, GL_SPECULAR, aTempVector);
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitLight( const CDirectionalLight &light, unsigned int nLightId)
{
#if !defined(MOOSE_APPLE_IPHONE)
  // GLenum iLightID = GL_LIGHT0+nLightId;
  // glEnable(iLightID);
  // float aTempVector[4];

  // // if we have directional light, the position parameter
  // // actually defines the direction
  // aTempVector[0] = -light.GetDirection()[0];
  // aTempVector[1] = -light.GetDirection()[1];
  // aTempVector[2] = -light.GetDirection()[2];
  // aTempVector[3] = 0.0f;
  // glLightf(iLightID, GL_SPOT_CUTOFF, 180.0f);
  // // Set the light position
  // glLightfv(iLightID, GL_POSITION, aTempVector);

  // // Set the intensity distribution of the light.
  // glLightf(iLightID, GL_SPOT_EXPONENT, 0.0f);

  // // Commit color colors
  // CommitLightColors( light, iLightID );
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitLight( const CSpotLight &light, unsigned int nLightId)
{
#if !defined(MOOSE_APPLE_IPHONE)
	// GLenum iLightID = GL_LIGHT0+nLightId;
	// glEnable(iLightID);
	// float aTempVector[4];

	// aTempVector[0] = light.GetPosition()[0];
	// aTempVector[1] = light.GetPosition()[1];
	// aTempVector[2] = light.GetPosition()[2];
	// aTempVector[3] = 1.0f;

	// glLightf( iLightID, GL_SPOT_CUTOFF,    light.GetSpotAngle());
	// glLightfv(iLightID, GL_SPOT_DIRECTION, const_cast<CVector3<float> &>(light.GetDirection()).GetArray());

	// // Set the attenuation parameters
	// glLightf(iLightID, GL_LINEAR_ATTENUATION,    light.GetLinearAttenuation());
	// glLightf(iLightID, GL_QUADRATIC_ATTENUATION, light.GetQuadraticAttenuation());
	// glLightf(iLightID, GL_CONSTANT_ATTENUATION,  light.GetConstantAttenuation());

	// // Set the light position
	// glLightfv(iLightID, GL_POSITION, aTempVector);
	// 	// Set the intensity distribution of the light.
	// glLightf(iLightID, GL_SPOT_EXPONENT, light.GetSpotExponent());

	// CommitLightColors( light, iLightID );
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitLight( const CPointLight &light, unsigned int nLightId)
{
#if !defined(MOOSE_APPLE_IPHONE)
  // GLenum iLightID = GL_LIGHT0+nLightId;
  // glEnable(iLightID);
  // float aTempVector[4];



  //   aTempVector[0] = light.GetPosition()[0];
  //   aTempVector[1] = light.GetPosition()[1];
  //   aTempVector[2] = light.GetPosition()[2];
  //   aTempVector[3] = 1.0f;

  //   glLightf(iLightID, GL_SPOT_CUTOFF, 180.0f);

  //   // Set the attenuation parameters
  //   glLightf(iLightID, GL_LINEAR_ATTENUATION,    light.GetLinearAttenuation());
  //   glLightf(iLightID, GL_QUADRATIC_ATTENUATION, light.GetQuadraticAttenuation());
  //   glLightf(iLightID, GL_CONSTANT_ATTENUATION,  light.GetConstantAttenuation());



  // // Set the light position
  // glLightfv(iLightID, GL_POSITION, aTempVector);

  // // Set the intensity distribution of the light.
  // glLightf(iLightID, GL_SPOT_EXPONENT,  0.0f);

  // CommitLightColors(light, iLightID);
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitLight( const CAmbientLight &light )
{
#if !defined(MOOSE_APPLE_IPHONE)
  //const float ONE_DIV_255 = 0.00392156862745098039;

	// ambient light is global mode
	/*float array[4] =  { light.GetColor()[0] * ONE_DIV_255,
						light.GetColor()[1] * ONE_DIV_255,
						light.GetColor()[2] * ONE_DIV_255,
						light.GetColor()[3] * ONE_DIV_255};*/

    //	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, array );
#endif
}
/////////////////////////////////////////////////////////////////
//void
//Moose::Graphics::COglRenderer::CommitLight( const CLight &light, unsigned int nLightId)
//{
//  GLenum iLightID = GL_LIGHT0+nLightId;
//  glEnable(iLightID);
//  float aTempVector[4];
//  switch ( light.GetType())
//  {
//  case DIRECTIONAL:
//    // if we have directional light, the position parameter
//    // actually defines the direction
//    aTempVector[0] = -light.GetDirection()[0];
//    aTempVector[1] = -light.GetDirection()[1];
//    aTempVector[2] = -light.GetDirection()[2];
//    aTempVector[3] = 0.0f;
//    glLightf(iLightID, GL_SPOT_CUTOFF, 180.0);
//
//    break;
//  case SPOTLIGHT:
//    aTempVector[0] = light.GetPosition()[0];
//    aTempVector[1] = light.GetPosition()[1];
//    aTempVector[2] = light.GetPosition()[2];
//    aTempVector[3] = 1.0f;
//
//    glLightf( iLightID, GL_SPOT_CUTOFF,    light.GetSpotAngle());
//    glLightfv(iLightID, GL_SPOT_DIRECTION, const_cast<CVector3<float> &>(light.GetDirection()).GetArray());
//
//    // Set the attenuation parameters
//    glLightf(iLightID, GL_LINEAR_ATTENUATION,    light.GetLinearAttenuation());
//    glLightf(iLightID, GL_QUADRATIC_ATTENUATION, light.GetQuadraticAttenuation());
//    glLightf(iLightID, GL_CONSTANT_ATTENUATION,  light.GetConstantAttenuation());
//
//    break;
//
//  case POINTLIGHT:
//
//    aTempVector[0] = light.GetPosition()[0];
//    aTempVector[1] = light.GetPosition()[1];
//    aTempVector[2] = light.GetPosition()[2];
//    aTempVector[3] = 1.0f;
//
//    glLightf(iLightID, GL_SPOT_CUTOFF, 180.0f);
//
//    // Set the attenuation parameters
//    glLightf(iLightID, GL_LINEAR_ATTENUATION,    light.GetLinearAttenuation());
//    glLightf(iLightID, GL_QUADRATIC_ATTENUATION, light.GetQuadraticAttenuation());
//    glLightf(iLightID, GL_CONSTANT_ATTENUATION,  light.GetConstantAttenuation());
//
//    break;
//  }
//
//
//  // Set the light position
//  glLightfv(iLightID, GL_POSITION, aTempVector);
//
//  // Set the intensity distribution of the light.
//  glLightf(iLightID, GL_SPOT_EXPONENT, light.GetSpotExponent());
//
//  // Set diffuse RGBA intensity
//  aTempVector[0] = (float)light.GetDiffuseColor()[0]/255.0f;
//  aTempVector[1] = (float)light.GetDiffuseColor()[1]/255.0f;
//  aTempVector[2] = (float)light.GetDiffuseColor()[2]/255.0f;
//  aTempVector[3] = (float)light.GetDiffuseColor()[3]/255.0f;
//  glLightfv(iLightID, GL_DIFFUSE, aTempVector);
//
//  // Set ambient RGBA intensity
//  aTempVector[0] = (float)light.GetAmbientColor()[0]/255.0f;
//  aTempVector[1] = (float)light.GetAmbientColor()[1]/255.0f;
//  aTempVector[2] = (float)light.GetAmbientColor()[2]/255.0f;
//  aTempVector[3] = (float)light.GetAmbientColor()[3]/255.0f;
//  glLightfv(iLightID, GL_AMBIENT, aTempVector);
//
//  // Set specular RGBA intensity
//  aTempVector[0] = (float)light.GetSpecularColor()[0]/255.0f;
//  aTempVector[1] = (float)light.GetSpecularColor()[1]/255.0f;
//  aTempVector[2] = (float)light.GetSpecularColor()[2]/255.0f;
//  aTempVector[3] = (float)light.GetSpecularColor()[3]/255.0f;
//  glLightfv(iLightID, GL_SPECULAR, aTempVector);
//}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::DisableLight ( unsigned int nLightId )
{
#if !defined(MOOSE_APPLE_IPHONE)
  //  glDisable(GL_LIGHT0+nLightId);
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::DisableState( STATE_TYPE tState )
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
Moose::Graphics::COglRenderer::CommitState( STATE_TYPE tState )
{
  switch(tState)
  {
  case STATE_DEPTH_WRITE:
    glDepthMask(GL_TRUE);
    break;
#if defined(MOOSE_APPLE_IPHONE)
  case STATE_LIGHTING:
    /*OpenGL version 2 won't use lighting (always shaders)*/
    break;
#endif
  default:
    glEnable(static_cast<GLenum>(tState));
    break;
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitMaterial( const Moose::Graphics::CMaterial & material, int iFace )
{
#if !defined(MOOSE_APPLE_IPHONE)
  // GLenum glFace = iFace ? GL_BACK : GL_FRONT;
  // glMaterialfv( glFace, GL_DIFFUSE,   material.GetDiffuse().GetArray());
  // glMaterialfv( glFace, GL_AMBIENT,   material.GetAmbient().GetArray());
  // glMaterialfv( glFace, GL_SPECULAR,  material.GetSpecular().GetArray());
  // glMaterialfv( glFace, GL_EMISSION,  material.GetEmission().GetArray());
  // glMaterialf(  glFace, GL_SHININESS, material.GetShininess()*128.0f);
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitAlpha(  Moose::Graphics::ALPHA_TEST_TYPE tType, float fReference )
{
#if !defined(MOOSE_APPLE_IPHONE)
  //glAlphaFunc( static_cast<GLenum>(tType), fReference);
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitBlending( BLEND_SRC_TYPE tSource, BLEND_DST_TYPE tDestination)
{
  glBlendFunc( static_cast<GLenum>(tSource),static_cast<GLenum>(tDestination));
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitBlending( Moose::Graphics::CBlendingOperation & rBlendingOp )
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
Moose::Graphics::COglRenderer::CommitSkybox( Moose::Graphics::CSkybox & skybox, Moose::Graphics::CCamera &camera )
{

  // Get view matrix, reset translation part
  Moose::Math::CMatrix4x4<float> mView = camera.GetViewMatrix();
  mView(0,3) = 0.0f;
  mView(1,3) = 0.0f;
  mView(2,3) = 0.0f;
  /*#if !defined(MOOSE_APPLE_IPHONE)
  glPushMatrix();
  glLoadTransposeMatrixf( mView.GetArray());

  CModel & model = **skybox.GetModelHandle();
  COglTexture *pTexture = *skybox.GetRenderState().GetTextureHandle(0);
  CIndexArray *pIndices = *model.GetIndices();
  CVertexDescriptor *pTexCoords = *model.GetTextureCoordinateHandle(0);
  CVertexDescriptor *pVertices  = *model.GetVertexHandle();

  if ( pVertices )  CommitVertexDescriptor( pVertices );
  if ( pTexCoords ) CommitVertexDescriptor( pTexCoords );
  if ( pTexture )
  {
	  CommitTexture( 0, pTexture );
	  CommitFilters( skybox.GetRenderState().GetTextureFilters(0), pTexture->GetType());
  }
  if ( pIndices )   CommitPrimitive( pIndices );
  /////////////////////////////////////////////////////////////////

  glPopMatrix();
  #endif*/
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitTransform( const Moose::Math::CTransform &transform )
{
#if !defined(MOOSE_APPLE_IPHONE) 
  //glPushMatrix();
  //glMultTransposeMatrixf( const_cast<Moose::Math::CTransform &>(transform).GetMatrix().GetArray());
#endif
}

/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::RollbackTransform()
{
#if !defined(MOOSE_APPLE_IPHONE)
  //glPopMatrix();
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitSphere( const Moose::Volume::CSphere &sphere, bool bWireframe )
{
#if !defined(MOOSE_APPLE_IPHONE)
  //   if ( !m_pQuadric )
  //   m_pQuadric = gluNewQuadric();

  // // Set drawing style.
  // if ( bWireframe )	gluQuadricDrawStyle( m_pQuadric, GLU_SILHOUETTE  );
  // else			    gluQuadricDrawStyle( m_pQuadric, GLU_FILL  );

  // // Translate and render
  // glPushMatrix();
  //   glTranslatef( sphere.GetPosition()[0], sphere.GetPosition()[1], sphere.GetPosition()[2]);

  //   gluDisk(m_pQuadric, 0.0, sphere.GetRadius(), 16, 1);
  //   glPushMatrix();
  //       glRotatef(90.0, 0.0, 1.0, 0.0);
  //       gluDisk(m_pQuadric, 0.0, sphere.GetRadius(), 16, 1);
  //   glPopMatrix();

  //   glPushMatrix();
  //       glRotatef(90.0, 1.0, 0.0, 0.0);
  //       gluDisk(m_pQuadric, 0.0, sphere.GetRadius(), 16, 1);
  //   glPopMatrix();



  // glPopMatrix();
  
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitCircle( const Moose::Math::CVector3<float> & vCenter,
					       float fRadius, const CVector3<float> & vRotation,
					       bool bWireframe )
{
#if !defined(MOOSE_APPLE_IPHONE)
  //   if ( !m_pQuadric )
  //   m_pQuadric = gluNewQuadric();
  // gluQuadricDrawStyle( m_pQuadric, (bWireframe ? GLU_SILHOUETTE : GLU_FILL) );
  // glPushMatrix();
  //   glTranslatef( vCenter[0], vCenter[1], vCenter[2] );
  //   glRotatef( vRotation[2], 0, 0, 1);
  //   glRotatef( vRotation[1], 0, 1, 0);
  //   glRotatef( vRotation[0], 1, 0, 0);
  //   gluDisk( m_pQuadric, 0.0, fRadius, 16, 1);
  //       glPopMatrix();
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitBox( const Moose::Volume::COrientedBox &box, bool bWireframe )
{
  //   if ( !m_pQuadric )
  //       m_pQuadric = gluNewQuadric();

//   // Set drawing style.
//   if ( bWireframe )	gluQuadricDrawStyle( m_pQuadric, GLU_LINE  );
//   else			gluQuadricDrawStyle( m_pQuadric, GLU_FILL  );

//   // Translate and render
//   glPushMatrix();
//     glTranslatef( sphere.GetPosition()[0], sphere.GetPosition()[1], sphere.GetPosition()[2]);
//     gluSphere(m_pQuadric, sphere.GetRadius(), 16, 16);
//   glPopMatrix();
#if !defined(MOOSE_APPLE_IPHONE)
  //   glPushAttrib( GL_POLYGON_BIT );
  //   if ( bWireframe ) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //   else 							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glBegin( GL_QUAD_STRIP );

  // using namespace Moose::Volume;
  // glNormal3fv( box.GetUpVector().GetArray() );
  // glVertex3fv( box.GetCorner( TOP_LEFT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( TOP_LEFT_BACK).GetArray() );
  // glVertex3fv( box.GetCorner( TOP_RIGHT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( TOP_RIGHT_BACK).GetArray() );
  // glNormal3fv( box.GetRightVector().GetArray() );
  // glVertex3fv( box.GetCorner( BOTTOM_RIGHT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( BOTTOM_RIGHT_BACK).GetArray() );
  // glNormal3fv( (-box.GetUpVector()).GetArray() );
  // glVertex3fv( box.GetCorner( BOTTOM_LEFT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( BOTTOM_LEFT_BACK).GetArray() );
  // glNormal3fv( (-box.GetRightVector()).GetArray());
  // glVertex3fv( box.GetCorner( TOP_LEFT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( TOP_LEFT_BACK).GetArray() );
  
  // glEnd();
  
  // glBegin(GL_QUADS);
  // glNormal3fv( box.GetForwardVector().GetArray());
  // glVertex3fv( box.GetCorner( TOP_LEFT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( BOTTOM_LEFT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( BOTTOM_RIGHT_FRONT).GetArray() );
  // glVertex3fv( box.GetCorner( TOP_RIGHT_FRONT).GetArray() );
  
  // glNormal3fv( (-box.GetForwardVector()).GetArray());
  // glVertex3fv( box.GetCorner( BOTTOM_RIGHT_BACK).GetArray() );
  // glVertex3fv( box.GetCorner( BOTTOM_LEFT_BACK).GetArray() );
  // glVertex3fv( box.GetCorner( TOP_LEFT_BACK).GetArray() );
  // glVertex3fv( box.GetCorner( TOP_RIGHT_BACK).GetArray() );
  // glEnd();
  // glPopAttrib();
#endif

}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitCapsule( const Moose::Volume::CCapsule &capsule, bool bWireframe )
{
#if !defined(MOOSE_APPLE_IPHONE)
    //   if ( !m_pQuadric )
    //     m_pQuadric = gluNewQuadric();
    // CQuaternion q;
    // CMatrix4x4<float> m;
    // glPushMatrix();
    //     // Create rotation from positive z-axis to capsule line segment
    //     RotationArc( CVector3<float>(0.0, 0.0, 1.0f), capsule.GetDirection(), q);
    //     QuaternionToMatrix(q, m);
    //     glMultTransposeMatrixf( m.GetArray());

    //     // Render end spheres
    //     CommitSphere( CSphere( CVector3<float>(0,0,0),                          capsule.GetRadius()), true );
    //     CommitSphere( CSphere( CVector3<float>(0,0,capsule.GetDistanceEnd()),   capsule.GetRadius()), true );
    //     // Render lines
    //     glBegin(GL_LINES);

    //         glVertex3f( 0.0, capsule.GetRadius(), 0.0);
    //         glVertex3f( 0.0, capsule.GetRadius(), capsule.GetDistanceEnd());

    //         glVertex3f( 0.0, -capsule.GetRadius(), 0.0);
    //         glVertex3f( 0.0, -capsule.GetRadius(), capsule.GetDistanceEnd());

    //         glVertex3f( capsule.GetRadius(), 0.0, 0.0);
    //         glVertex3f( capsule.GetRadius(), 0.0, capsule.GetDistanceEnd());

    //         glVertex3f( -capsule.GetRadius(), 0.0, 0.0);
    //         glVertex3f( -capsule.GetRadius(), 0.0, capsule.GetDistanceEnd());

    //     glEnd();
    //     glPopMatrix();
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitQuad( const Moose::Spatial::CVertex &vertexOne,
					     const Moose::Spatial::CVertex &vertexTwo,
					     const Moose::Spatial::CVertex &vertexThree,
					     const Moose::Spatial::CVertex &vertexFour)
{
#if !defined(MOOSE_APPLE_IPHONE)
//   // Handy macro for multiple similar commands
 #define COMMIT_VERTEX( V ) {			\
   glColor4ubv( V.GetColor().GetArray() );	\
   glTexCoord2f( V.GetTextureCoordinates()[0],	\
 		V.GetTextureCoordinates()[1]);	\
   glNormal3fv( V.GetNormal().GetArray() );	\
   glVertex3fv( V.GetPosition().GetArray() );	\
 }

//   glBegin(GL_QUADS);
//     COMMIT_VERTEX( vertexOne );
//     COMMIT_VERTEX( vertexTwo );
//     COMMIT_VERTEX( vertexThree );
//     COMMIT_VERTEX( vertexFour );
//   glEnd();
// #undef COMMIT_VERTEX
#endif
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRenderer::CommitCache( Moose::Graphics::CVertexDescriptor & rVertexDescriptor, Moose::Graphics::CACHE_ACCESS_TYPE tType )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return 1;

  if(  !rVertexDescriptor.IsCached() )
  {
    glGenBuffers(1, &rVertexDescriptor.GetCache());
  }

  glBindBuffer( GL_ARRAY_BUFFER, rVertexDescriptor.GetCache());

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
    glBufferData( GL_ARRAY_BUFFER,
		     rVertexDescriptor.GetByteSize(),
		     rVertexDescriptor.GetPointer<float>(),
		     static_cast<GLenum>(tType));

  case ELEMENT_TYPE_COLOR_4UB:
    glBufferData( GL_ARRAY_BUFFER,
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
    glDeleteBuffers( 1, &rVertexDescriptor.GetCache());
    rVertexDescriptor.SetState(Moose::Core::CACHE_NOCACHE);
    return 2;
  }
  else
  {
    rVertexDescriptor.SetState(Moose::Core::CACHE_UP2DATE);
  }
  // Unset current cache buffer.
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  return 0;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRenderer::CommitCache( Moose::Graphics::CIndexArray & rIndexArray,
					      Moose::Graphics::CACHE_ACCESS_TYPE tType  )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return 1;

  if(  !rIndexArray.IsCached() )
  {
    glGenBuffers(1, &rIndexArray.GetCache());
  }

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rIndexArray.GetCache());

  if ( rIndexArray.IsShortIndices() )
  {
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
		     sizeof(unsigned short int)*rIndexArray.GetNumIndices(),
		     rIndexArray.GetPointer<unsigned short int>(),
		     static_cast<GLenum>(tType));
  }
  else
  {
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
		     sizeof(unsigned int)*rIndexArray.GetNumIndices(),
		     rIndexArray.GetPointer<unsigned int>(),
		     static_cast<GLenum>(tType));
  }
  // Prepare for case that data does not fit
  if ( glGetError() == GL_OUT_OF_MEMORY )
  {
    glDeleteBuffers( 1, &(rIndexArray.GetCache()));
    rIndexArray.SetState(Moose::Core::CACHE_NOCACHE);
    return 2;
  }
  else
  {
    rIndexArray.SetState(Moose::Core::CACHE_UP2DATE);
  }
  // Unset current cache buffer.
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

  return 0;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::RollbackCache( Moose::Graphics::CVertexDescriptor & rVertexDescriptor )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return;

  if(  rVertexDescriptor.IsCached() )
  {
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &rVertexDescriptor.GetCache());
    rVertexDescriptor.SetState(Moose::Core::CACHE_NOCACHE);
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::RollbackCache( Moose::Graphics::CIndexArray & rIndexArray )
{
  if ( !GetFeatures().HasVertexBufferObject() ) return;

  if(  rIndexArray.IsCached() )
  {
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &rIndexArray.GetCache());
    rIndexArray.SetState(Moose::Core::CACHE_NOCACHE);
  }
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::DisableCaches()
{
  // disable VBO objects
  glBindBuffer( GL_ARRAY_BUFFER,		0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitVertexAttrib( Moose::Graphics::CShader &shader, int iLoc,
						     const Moose::Math::CVector4<float> &vValues )
{
  CommitVertexAttrib( shader, iLoc, vValues[0], vValues[1], vValues[2], vValues[3]);
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitVertexAttrib( Moose::Graphics::CShader &shader, int iLoc,
						     const Moose::Math::CVector3<float> &vValues )
{
  CommitVertexAttrib( shader, iLoc, vValues[0], vValues[1], vValues[2]);
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitVertexAttrib( Moose::Graphics::CShader &shader, int iLoc,
						     const Moose::Math::CVector2<float> &vValues )
{
  CommitVertexAttrib( shader, iLoc, vValues[0], vValues[1] );
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRenderer::LocateAttribShaderParam( Moose::Graphics::CShader &shader, const char *strParamName )
{
  return glGetAttribLocation( shader.GetProgram(), strParamName );
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRenderer::LocateUniformShaderParam( Moose::Graphics::CShader &shader, const char *strParamName )
{
  return glGetUniformLocation( shader.GetProgram(), strParamName );
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CFrameBufferObject *
Moose::Graphics::COglRenderer::CreateFramebuffer( unsigned int nWidth, unsigned int nHeight,
						    int iBufferFlags )
{


  GLuint iFBO;
  CFrameBufferObject *pFBO = NULL;

  ////////////////////
  // Create and bind framebuffer
  glGenFramebuffers(1, &iFBO);
  pFBO = new CFrameBufferObject( );


  glBindFramebuffer(GL_FRAMEBUFFER, pFBO->GetID());

  if ( iBufferFlags & FBO_DEPTH_BUFFER )
  {
    ////////////////////
    // Create depth buffer
    glGenRenderbuffers(1, &pFBO->GetDepthBufferId());
    glBindRenderbuffer(GL_RENDERBUFFER, pFBO->GetDepthBufferId());

    ////////////////////
    // Attach depth buffer to frame buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
			     (unsigned int)pFBO->GetWidth(), (unsigned int)pFBO->GetHeight() );
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				 GL_RENDERBUFFER, pFBO->GetDepthBufferId());
  }


  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // Return object pointer.
  return pFBO;
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::COglRenderer::AttachTextureToFramebuffer( Moose::Graphics::CFrameBufferObject & rFBO,
							     const Moose::Default::TEXTURE_HANDLE & hTexture,
							     unsigned int nColorBuffer )
{
  int iRetval = 1;
  if ( !hTexture.IsNull() )
  {

    // bind frame buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, rFBO.GetID());

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

    glFramebufferTexture2D( GL_FRAMEBUFFER,
			       GL_COLOR_ATTACHMENT0+nBufferNumber,
			       iTexType, pTexture->GetID(), 0);

    /////////////////////////////////////////////////////////////////
    /// THIS MIGHT REQUIRE TINKERING. I don't know should filters be set
    /// before actually using the texture or not... if there are problems,
    /// this might be the culprit.
    glGenerateMipmap( iTexType );
    /////////////////////////////////////////////////////////////////
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    /// If there were any errors, clean up and return NULL.
    if ( status !=  GL_FRAMEBUFFER_COMPLETE )
    {
      iRetval = 1;
    }
    // disable frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    iRetval = 0;
  }

  return iRetval;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitFrameBuffer( const Moose::Graphics::CFrameBufferObject & rFBO, unsigned int nColorBufferCount )
{
  // Bind frame buffer.
  glBindFramebuffer(GL_FRAMEBUFFER, rFBO.GetID());

  // Define viewport to texture size and store current settings so they can be retrieved.
  // (This might not be necessary, but sounds resonably now)
  //glPushAttrib(GL_VIEWPORT_BIT);
  glViewport(0,0, (unsigned int)rFBO.GetWidth(), (unsigned int)rFBO.GetHeight());

  // select render targets.
  // glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT) or glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT) or etc.
  // but this is better; it allows multiple buffers to be rendered via parameter.
  // Output to buffers must be controlled via GLSL fragment shaders.
#if !defined(MOOSE_APPLE_IPHONE)
  //  glDrawBuffers( nColorBufferCount % TEXTURE_HANDLE_COUNT, g_ColorBufferNames );
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitFrameBufferSingle( const Moose::Graphics::CFrameBufferObject & rFBO, unsigned int nColorBuffer )
{
  // Bind frame buffer.
  glBindFramebuffer(GL_FRAMEBUFFER, rFBO.GetID());

  // Define viewport to texture size and store current settings so they can be retrieved.
  // (This might not be necessary, but sounds resonably now)

  //glPushAttrib(GL_VIEWPORT_BIT);
  glViewport(0,0, (unsigned int)rFBO.GetWidth(), (unsigned int)rFBO.GetHeight());
#if !defined(MOOSE_APPLE_IPHONE)
  // select render target
  //glDrawBuffer(GL_COLOR_ATTACHMENT0+nColorBuffer);
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::RollbackFrameBuffer( const Moose::Graphics::CFrameBufferObject & rFBO )
{
  // Reset viewport settings
  //glPopAttrib();
  // Put render commands back to ordinary buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
Moose::Graphics::COglRenderer::CommitQuad( const Moose::Graphics::CFrameBufferObject & rFBO )
{
#if !defined(MOOSE_APPLE_IPHONE)
  // glBegin(GL_QUADS);

  //   COMMIT_COORDINATES( 0.0f,            0.0f,		0.0f,            0.0f);
  //   COMMIT_COORDINATES( rFBO.GetWidth(), 0.0f,		rFBO.GetWidth(), 0.0f);
  //   COMMIT_COORDINATES( rFBO.GetWidth(), rFBO.GetHeight(),rFBO.GetWidth(), rFBO.GetHeight() );
  //   COMMIT_COORDINATES( 0.0f,            rFBO.GetHeight(),0.0f,            rFBO.GetHeight());

  // glEnd();
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitQuad( const Moose::Graphics::CCamera & rCamera, const Moose::Graphics::CFrameBufferObject & rFBO)
{
#if !defined(MOOSE_APPLE_IPHONE) 
 // glBegin(GL_QUADS);

 //    COMMIT_COORDINATES( 0.0f,				             0.0f,				0.0f,            0.0f);
 //    COMMIT_COORDINATES( rCamera.GetViewport().width,	 0.0f,				rFBO.GetWidth(), 0.0f);
 //    COMMIT_COORDINATES( rCamera.GetViewport().width,	 rCamera.GetViewport().height,	rFBO.GetWidth(), rFBO.GetHeight() );
 //    COMMIT_COORDINATES( 0.0f,				             rCamera.GetViewport().height,	0.0f,            rFBO.GetHeight());

 //  glEnd();
#endif
}
/////////////////////////////////////////////////////////////////
#undef COMMIT_COORDINATES
/////////////////////////////////////////////////////////////////

Moose::Graphics::CFontset *
Moose::Graphics::COglRenderer::CreateFontset( const char *sPathToFontFile, unsigned int nFontSize)
{
#if defined(MOOSE_APPLE_IPHONE)
  return NULL;
#else
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
  pFontset->GetDisplayList() = glGenLists(Moose::Globals::MAX_FONT_CHARACTERS);

  for(unsigned int n=0;n<Moose::Globals::MAX_FONT_CHARACTERS;n++)
  {
    // Whitespace has no dimension, it needs special treatment.
    if ( n == WHITESPACE )
    {
      glNewList(pFontset->GetDisplayList()+n,GL_COMPILE);
      glTranslatef(nFontSize*0.2f, 0, 0);
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
    float fLog2Height = log(static_cast<float>(ftFace->glyph->bitmap.rows))/log(2.0f);
    float fLog2Width  = log(static_cast<float>(ftFace->glyph->bitmap.width))/log(2.0f);
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

    glActiveTexture( GL_TEXTURE0 );
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
#endif
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitString( CFontset & rFontSet, float fX, float fY, const char *szText )
{

  ////////////////////
  /// Sanity check.
  if ( szText == NULL )
  {
    cerr << "I shall not render a NULL string!" << endl;
    return;
  }
#if !defined(MOOSE_APPLE_IPHONE)
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
#endif
}
/////////////////////////////////////////////////////////////////
const COglRendererFeatures &
Moose::Graphics::COglRenderer::GetFeatures()
{
  if ( m_pFeatures == NULL )
  {
    m_pFeatures = new COglRendererFeatures();
    assert( m_pFeatures != NULL && "Cannot create renderer features object!" );
  }
  return *m_pFeatures;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::COglRenderer::CommitRenderState( const Moose::Graphics::CRenderState & state )
{
  CRenderState & s = const_cast< Moose::Graphics::CRenderState & >(state);
  CommitBlending( s.GetBlendingOperation());
  ////////////////////
  // Check depth mask write flag.
  if ( s.GetDepthWrite()) {  CommitState( STATE_DEPTH_WRITE );  }
  else DisableState( STATE_DEPTH_WRITE );
  ////////////////////
  // Check depth test flag.
  if ( s.GetDepthTest()) CommitState( STATE_DEPTH_TEST );
  else DisableState( STATE_DEPTH_TEST );
  ////////////////////
  // Check face culling flag.
  if ( s.GetFaceCulling()) CommitState( STATE_FACECULLING );
  else DisableState( STATE_FACECULLING );
  ////////////////////
  // Polygon offset.
  if ( s.GetPolygonOffset().IsEnabled() ) 
  {
    CommitState( STATE_POLYGONOFFSET );
    glPolygonOffset( s.GetPolygonOffset().GetFactor(), 
                     s.GetPolygonOffset().GetUnits());
  }
  else DisableState( STATE_POLYGONOFFSET );
	



}
/////////////////////////////////////////////////////////////////
