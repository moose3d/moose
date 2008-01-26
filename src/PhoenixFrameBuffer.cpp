#include "PhoenixFrameBuffer.h"
#include <GL/GLee.h>
#include <GL/gl.h>
#include <GL/glu.h>
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CFrameBufferObject::~CFrameBufferObject()
{
  glDeleteFramebuffersEXT(1, &GetID());
  glDeleteRenderbuffersEXT(1, &GetDepthBufferId());
}
/////////////////////////////////////////////////////////////////
