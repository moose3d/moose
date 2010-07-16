#include "MooseFrameBuffer.h"
#include "MooseDefaultEntities.h"
/////////////////////////////////////////////////////////////////
#if defined(MOOSE_APPLE_IPHONE)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(__APPLE__)
#include <GL/GLee.h>
#include <OpenGL/gl.h>
#else
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "MooseOGLConsts.h"
using namespace Moose::Graphics;
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
Moose::Graphics::CFrameBufferObject::CFrameBufferObject(  ) : 
    COglBase(0),
    CDimensional2D(0.0f,0.0f), 
    m_nColorBufferId(0),
    m_nDepthBufferId(0),
    m_nStencilBufferId(0)
{
    glGenFramebuffers(1, &GetID());
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CFrameBufferObject::~CFrameBufferObject()
{
  // delete buffers
  glDeleteFramebuffers(1,  &GetID());
  glDeleteRenderbuffers(1, &GetDepthBufferId()); 
  glDeleteRenderbuffers(1, &GetColorBufferId());
  glDeleteRenderbuffers(1, &GetStencilBufferId());
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CFrameBufferObject::Apply()
{
  glBindFramebuffer( GL_FRAMEBUFFER, GetID() );
}
/////////////////////////////////////////////////////////////////
GLuint &
Moose::Graphics::CFrameBufferObject::GetDepthBufferId()
{
    return m_nDepthBufferId;
}
/////////////////////////////////////////////////////////////////
const GLuint &
Moose::Graphics::CFrameBufferObject::GetDepthBufferId() const
{
    return m_nDepthBufferId;
}
/////////////////////////////////////////////////////////////////
GLuint &
Moose::Graphics::CFrameBufferObject::GetColorBufferId()
{
    return m_nColorBufferId;
}
/////////////////////////////////////////////////////////////////
const GLuint &
Moose::Graphics::CFrameBufferObject::GetColorBufferId() const
{
    return m_nColorBufferId;
}
/////////////////////////////////////////////////////////////////
GLuint &
Moose::Graphics::CFrameBufferObject::GetStencilBufferId()
{
    return m_nStencilBufferId;
}
/////////////////////////////////////////////////////////////////
const GLuint &
Moose::Graphics::CFrameBufferObject::GetStencilBufferId() const
{
    return m_nStencilBufferId;
}
/////////////////////////////////////////////////////////////////
const Moose::Default::TEXTURE_HANDLE &
Moose::Graphics::CFrameBufferObject::GetTextureHandle( size_t nUnit ) const
{
    return m_hTextures[nUnit % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
Moose::Default::TEXTURE_HANDLE &
Moose::Graphics::CFrameBufferObject::GetTextureHandle( size_t nUnit )
{
    return m_hTextures[nUnit % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::CFrameBufferObject::Validate()
{

  bool bRetval = true;

  glBindFramebuffer(GL_FRAMEBUFFER, GetID());   
  GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
  if ( status != GL_FRAMEBUFFER_COMPLETE )
  {
    bRetval = false;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);   
  return bRetval;
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::Graphics::CFrameBufferObject::AttachBuffer( int iBufferFlags, GLenum type, 
                                                     size_t width, size_t height )
{
  // Use this framebuffer 
    glBindFramebuffer( GL_FRAMEBUFFER, GetID() );

  if ( iBufferFlags & FBO_COLOR_BUFFER )
  {
    glGenRenderbuffers(1, &GetColorBufferId());
    glBindRenderbuffer( GL_RENDERBUFFER, GetColorBufferId() );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA4, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                              GL_RENDERBUFFER, GetColorBufferId());
        
  }

  if ( iBufferFlags & FBO_DEPTH_BUFFER )
  {

    glGenRenderbuffers(1, &GetDepthBufferId());
    glBindRenderbuffer(GL_RENDERBUFFER, GetDepthBufferId());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
                          width, height );
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, GetDepthBufferId());
  }
    
  if ( iBufferFlags & FBO_STENCIL_BUFFER )
  {
    glGenRenderbuffers(1, &GetStencilBufferId());
    glBindRenderbuffer(GL_RENDERBUFFER, GetStencilBufferId());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8,
                          width,height );
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, GetStencilBufferId());
  }
  
  return Validate();
}
////////////////////////////////////////////////////////////////////////////////
bool
Moose::Graphics::CFrameBufferObject::AttachTexture( const Moose::Default::TEXTURE_HANDLE & hTexture, unsigned int nColorBuffer )
{
#if defined(MOOSE_APPLE_IPHONE)
    nColorBuffer = 0; // OpenGL ES supports only one color buffer per framebuffer.
#endif
    
    if ( hTexture.IsNull() ) return false;
    
        
    Apply();        
    GetTextureHandle( nColorBuffer ) = hTexture;
    COglTexture *pTexture = *GetTextureHandle(nColorBuffer);
    
    // Determine texture type
    GLenum iTexType = GetGLTextureType(pTexture->GetType());
        
    nColorBuffer = nColorBuffer % TEXTURE_HANDLE_COUNT;
    
    glBindTexture(iTexType, pTexture->GetID());
        
    /* Should these be set? Dunno.
     
     glTexParameterf( iTexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf( iTexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf( iTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( iTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    */
    glFramebufferTexture2D( GL_FRAMEBUFFER,
                            GL_COLOR_ATTACHMENT0+nColorBuffer,
                            iTexType, pTexture->GetID(), 0);
        
    
    /////////////////////////////////////////////////////////////////
    /// THIS MIGHT REQUIRE TINKERING. I don't know should filters be set
    /// before actually using the texture or not... if there are problems,
    /// this might be the culprit.
    //glGenerateMipmap( iTexType );

    return Validate();    
            
}
//////////////////////////////////////////////////////////////////////////////////
