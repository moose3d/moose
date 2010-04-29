#include "PhoenixFrameBuffer.h"
#include "PhoenixDefaultEntities.h"
/////////////////////////////////////////////////////////////////
#if defined(PHOENIX_APPLE_IPHONE)
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
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrameBufferObject::~CFrameBufferObject()
{
  // delete buffers
  glDeleteFramebuffers(1,  &GetID());
  glDeleteRenderbuffers(1, &GetDepthBufferId());
  // release texture handles
  for( size_t i=0;i<TEXTURE_HANDLE_COUNT;i++)
    g_DefaultTextureManager->Release( GetTextureHandle(i) );
}
/////////////////////////////////////////////////////////////////
