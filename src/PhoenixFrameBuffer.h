#ifndef __PhoenixFrameBuffer_h__
#define __PhoenixFrameBuffer_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixTexture.h"
#include "PhoenixSpatial.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for framebuffer objects.
    class CFrameBufferObject : public Phoenix::Graphics::COglBase,
			       public Phoenix::Spatial::CDimensional2D
    {
    protected:
      /// Handle to a texture.
      Phoenix::Default::TEXTURE_HANDLE m_hTexture;
      /// Renderbuffer for depth buffer.
      unsigned int m_nDepthBufferId;
    public:
      ////////////////////
      /// Constructor.
      /// \param nFrameBufferId OpenGL frame buffer object id.
      CFrameBufferObject( unsigned int nFrameBufferId);       
      ////////////////////
      /// Constructor.
      /// \param nFrameBufferId OpenGL frame buffer object id.
      /// \param nWidth Width of this frame buffer in pixels.
      /// \param nHeight Height of this frame buffer in pixels.
      CFrameBufferObject( unsigned int nFrameBufferId,
			  unsigned int nWidth,
			  unsigned int nHeight); 

      ////////////////////
      /// Destructor.
      ~CFrameBufferObject();
      ////////////////////
      /// Returns texture handle of this framebuffer.
      /// \returns TEXTURE_HANDLE
      Phoenix::Default::TEXTURE_HANDLE & GetTextureHandle();
      ////////////////////
      /// Returns texture handle of this framebuffer.
      /// \returns TEXTURE_HANDLE
      const Phoenix::Default::TEXTURE_HANDLE & GetTextureHandle() const;
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      unsigned int & GetDepthBufferId();
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      const unsigned int & GetDepthBufferId() const;
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline 
Phoenix::Graphics::CFrameBufferObject::CFrameBufferObject( unsigned int nFrameBufferId ) : COglBase(nFrameBufferId), 
											   CDimensional2D(), 
											   m_nDepthBufferId(0)
{
  
}
/////////////////////////////////////////////////////////////////
inline
Phoenix::Graphics::CFrameBufferObject::CFrameBufferObject( unsigned int nFrameBufferId, unsigned int nWidth, unsigned int nHeight ) : COglBase(nFrameBufferId), 
																      CDimensional2D(nWidth, nHeight), 
																      m_nDepthBufferId(0)
{
  
}
/////////////////////////////////////////////////////////////////
inline unsigned int &
Phoenix::Graphics::CFrameBufferObject::GetDepthBufferId()
{
  return m_nDepthBufferId;
}
/////////////////////////////////////////////////////////////////
inline const unsigned int &
Phoenix::Graphics::CFrameBufferObject::GetDepthBufferId() const
{
  return m_nDepthBufferId;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Default::TEXTURE_HANDLE & 
Phoenix::Graphics::CFrameBufferObject::GetTextureHandle() const
{
  return m_hTexture;
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Default::TEXTURE_HANDLE & 
Phoenix::Graphics::CFrameBufferObject::GetTextureHandle()
{
  return m_hTexture;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////

