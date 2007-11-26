#include "PhoenixTexture.h"
#include <GL/GLee.h>
#include <GL/gl.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglBase::COglBase( unsigned int nId ) : m_nOglId(nId) 
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture::COglTexture( unsigned int nId, TEXTURE_TYPE tType ) : COglBase(nId), 
										      m_tTextureType(tType)
{
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::COglTexture::~COglTexture()
{
  glDeleteTextures( 1, &m_nOglId );
}
/////////////////////////////////////////////////////////////////
TEXTURE_TYPE 
Phoenix::Graphics::COglTexture::GetType() const
{
  return m_tTextureType;
}
// /////////////////////////////////////////////////////////////////
// Phoenix::Graphics::CFrameBufferObject::CFrameBufferObject( unsigned int nFrameBufferId, unsigned int nDepthBufferId )
// {
//   m_nFrameBufferId = nFrameBufferId;
//   m_nDepthBufferId = nDepthBufferId;
// }
// /////////////////////////////////////////////////////////////////
// Phoenix::Graphics::CFrameBufferObject::~CFrameBufferObject()
// {
//   glDeleteFrameBuffersEXT( Phoenix::Graphics::CFrameBufferObject::GetID());
// }
// /////////////////////////////////////////////////////////////////

      
