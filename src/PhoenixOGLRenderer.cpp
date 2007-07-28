/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <list>
#include <iostream>
#include "PhoenixGlobals.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixTGAImage.h"
#include <fstream>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics; 
using std::endl;
using std::cerr;
using std::ifstream;
using std::ios;
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
  SDL_GL_SwapBuffers();
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitVertexDescriptor( CVertexDescriptor *pBuffer, unsigned int nId )
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
  case ELEMENT_TYPE_TEX_2F:
    if ( nId < TEXTURE_HANDLE_COUNT ) { glClientActiveTextureARB( GL_TEXTURE0_ARB + nId); }
    else                              { glClientActiveTextureARB( GL_TEXTURE0_ARB);       }
    glTexCoordPointer(2, GL_FLOAT, 0, pBuffer->GetPointer<float>());
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
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
Phoenix::Graphics::COglRenderer::CreateTexture( const std::string &strFilename )
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
    std::cerr << "Out of memory while loading file '" << strFilename << "'" << std::endl;
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
  case 24:
    break;
  case 32:
    iGLInternalFormat = 4;
    iGLformat = GL_RGBA;
    break;
  default:
    delete pImage;
    std::cerr << "Not 24 or 32 BBP image :  '" << strFilename << "'" << std::endl;
    return NULL;
  }

  // create texture
  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  pTexture = new COglTexture( iTexId, TEXTURE_2D );

  // check memory allocation
  if ( !pTexture ) 
  {
    std::cerr << "Failed to allocate memory while loading file '" << strFilename << "'" << std::endl;
    return NULL;
  }
  
  // create actual gl texture 
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pTexture->GetID());  
  glTexImage2D( GL_TEXTURE_2D, 0, iGLInternalFormat, 
		pImage->GetWidth(), pImage->GetHeight(), 0, 
		iGLformat, GL_UNSIGNED_BYTE, pImage->GetImg());
  // build mipmaps 
  gluBuild2DMipmaps(GL_TEXTURE_2D, iGLInternalFormat, 
		    pImage->GetWidth(), pImage->GetHeight(),
		    iGLformat, GL_UNSIGNED_BYTE, pImage->GetImg());
  
  glDisable(GL_TEXTURE_2D);

  // ReleaseMemory
  CLEANUP();

#undef CLEANUP  	   

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture * 
Phoenix::Graphics::COglRenderer::CreateTexture( size_t nWidth, size_t nHeight, TEXTURE_TYPE tType )
{
  
  // create texture
  unsigned int iTexId;
  glGenTextures( 1, &iTexId);
  COglTexture *pTexture = new COglTexture( iTexId, tType );
  
  // check memory allocation
  if ( !pTexture ) 
  {
    std::cerr << "Failed to allocate memory while creating Empty Texture." << std::endl;
    return NULL;
  }
  // create texture dimensions
  switch ( tType )
  {
  case TEXTURE_2D:
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, pTexture->GetID());
    glTexImage2D( GL_TEXTURE_2D, 0, 4, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    glDisable(GL_TEXTURE_2D);
    break;
  }
  return pTexture;
}
/////////////////////////////////////////////////////////////////
#define GL_ENABLE_TEXTURE( TYPE, ID ) { glBindTexture( TYPE, ID ); glEnable( TYPE );} 
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitTexture( unsigned int nTexUnit, COglTexture *pTexture )
{
  glActiveTextureARB( GL_TEXTURE0_ARB + nTexUnit);
  switch ( pTexture->GetType())
  {
  case TEXTURE_2D:
    GL_ENABLE_TEXTURE( GL_TEXTURE_2D, pTexture->GetID());
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::DisableTexture( unsigned int nTexUnit, COglTexture *pTexture )
{
  glActiveTextureARB( GL_TEXTURE0_ARB + nTexUnit);
  switch ( pTexture->GetType())
  {
  case TEXTURE_2D:
    glDisable( GL_TEXTURE_2D);
    break;
  }
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitCamera( CCamera &camera )
{
  ////////////////////
  // Handle updating of view and projection, if they are changed.
  if ( camera.IsProjectionChanged()) camera.UpdateProjection();
  if ( camera.IsViewChanged()) camera.UpdateView();

  int *pViewport = camera.GetViewport();
  glViewport(pViewport[0], pViewport[1], pViewport[2], pViewport[3]);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  ////////////////////
  // Set up perspective OR orthogonal projection
  if ( camera.IsOrthogonal() )
  {
    float *pOrthoPlanes = camera.GetOrthoPlanes();
    // Set the orthogonal mode
    glOrtho( pOrthoPlanes[0],		 pOrthoPlanes[1],
	     pOrthoPlanes[2],		 pOrthoPlanes[3],
	     camera.GetNearClipping(), camera.GetFarClipping());
  } 
  else 
  {
    glMultMatrixf( camera.GetProjection().GetTransposition().GetArray());
  }
  ////////////////////
  /// Set up proper position.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf( camera.GetView().GetTransposition().GetArray());
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitModel( CModel &model )
{
  // Retrieve resources
  COglTexture *pTexture = NULL;
  CVertexDescriptor *pTemp = NULL;
  CVertexDescriptor *pVertices = g_DefaultVertexManager->GetResource(model.GetVertexHandle());
  CIndexArray *pIndices = g_DefaultIndexManager->GetResource( model.GetIndexHandle());
  
  // Commit textures
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    pTemp    = g_DefaultVertexManager->GetResource(  model.GetTextureCoordinateHandle(i));
    pTexture = g_DefaultTextureManager->GetResource( model.GetTextureHandle(i) );
    
    // check that texcoord resources actually exist
    if ( pTemp     != NULL ) 
    { 
      CommitVertexDescriptor( pTemp, i ); 
    } 
    // check that texture resource exists
    if ( pTexture  != NULL ) 
    { 
      CommitTexture( i, pTexture ); 
      // Apply texture filters.
      std::vector<TEXTURE_FILTER> &vecFilters = model.GetTextureFilters(i);
      
      for(unsigned int nFilter=0; nFilter<vecFilters.size(); nFilter++)
      {
	CommitFilter( vecFilters[nFilter], pTexture->GetType() );
      }
    }
  }
  // check and commit resources
  if ( pVertices != NULL ) { CommitVertexDescriptor ( pVertices ); }
  if ( pIndices  != NULL ) { CommitPrimitive ( pIndices );         }
  
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglRenderer::CommitFilter( TEXTURE_FILTER tFilter, TEXTURE_TYPE tType )
{
  GLenum glTarget;
  ////////////////////
  switch( tType )
  {
  case TEXTURE_2D:
  default:
    glTarget = GL_TEXTURE_2D;
  }
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
int
LoadFile( const std::string & strFilename, std::string &sContents )
{

  // Temporary container for line 
  string sLine;
  // The file stream 
  ifstream fsFile;
  // open file 
  fsFile.open( strFilename.c_str() );
  // test for failure 
  if ( fsFile.is_open(), ios::in) 
  {
    // read line by line 
    while( ! fsFile.eof() )
    {
      getline( fsFile, sLine );
      sContents.append( sLine );
    }
    // close the file 
    fsFile.close();
  } 
  else 
  {
    std::cerr << "The file " << strFilename
	      << " couldn't be opened."  << std::endl;
    return 1;
  }
  return 0;
}  
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShader * 
Phoenix::Graphics::COglRenderer::CreateShader( std::string strVertexShader, std::string strFragmentShader )
{
  unsigned int nProgram = glCreateProgram();
  CShader *pShader = new CShader( nProgram );
  int bHasShader = 0;
  if ( pShader == NULL )
  {
    std::cerr << "Failed to allocate shader memory." << std::endl;
  }
  string strVSSource, strFSSource;
  ////////////////////
  // Vertex shader loading
  if ( strVertexShader.size() > 0 )
  {
    if ( LoadFile( strVertexShader, strVSSource ))
    {
      std::cerr << "Failed to load vertex shader '" << strVertexShader << "'" << std::endl;
    }
    else
    {
      unsigned int nVertexShader = glCreateShader( GL_VERTEX_SHADER );
      int nLength = strVSSource.size();
      const char *pStrCode = strVSSource.c_str();
      glShaderSource(nVertexShader,1, &pStrCode, &nLength );
      glCompileShader( nVertexShader );
      pShader->SetVertexShader( nVertexShader );
      bHasShader = 1;
    }
  }
  ////////////////////
  // Fragment shader loading 
  if ( strFragmentShader.size() > 0 )
  {
    if ( LoadFile( strFragmentShader, strFSSource ))
    {
      std::cerr << "Failed to load fragment shader '" << strFragmentShader << "'" << std::endl;
    }
    else
    {
      unsigned int nFragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
      int nLength = strFSSource.size();
      const char *pStrCode = strFSSource.c_str();
      glShaderSource(nFragmentShader,1, &pStrCode, &nLength );
      glCompileShader( nFragmentShader );
      pShader->SetFragmentShader( nFragmentShader );
      bHasShader = 1;
    }
  }
  // check that shader code exists
  if ( !bHasShader )
  {
    return NULL;
  }
  // compile and return shader
  glLinkProgram(pShader->GetProgram());
  return pShader;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::COglRenderer::CommitShader( CShader *pShader )
{
  if ( pShader )
  {
    glUseProgram( pShader->GetProgram() );
  }
  else
  {
    glUseProgram( 0 );
  }
}
/////////////////////////////////////////////////////////////////
