#include "PhoenixFrameBuffer.h"
#include "PhoenixDefaultEntities.h"
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrameBufferObject::~CFrameBufferObject()
{
  // delete buffers
  glDeleteFramebuffersEXT(1,  &GetID());
  glDeleteRenderbuffersEXT(1, &GetDepthBufferId());
  // release texture handles
  for( size_t i=0;i<TEXTURE_HANDLE_COUNT;i++)
    g_DefaultTextureManager->Release( GetTextureHandle(i) );
}
/////////////////////////////////////////////////////////////////
