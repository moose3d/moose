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
										      m_tTextureType(tType), m_nWidth(0), m_nHeight(0)
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
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ResolutionType 
Phoenix::Graphics::COglTexture::GetWidth() const
{
  return m_nWidth;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ResolutionType 
Phoenix::Graphics::COglTexture::GetHeight() const
{
  return m_nHeight;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglTexture::SetWidth( Phoenix::Graphics::ResolutionType width )
{
  m_nWidth = width;
}
/////////////////////////////////////////////////////////////////
void 
Phoenix::Graphics::COglTexture::SetHeight( Phoenix::Graphics::ResolutionType height )
{
  m_nHeight = height;
}
/////////////////////////////////////////////////////////////////
