#ifndef __PhoenixFrameBuffer_h__
#define __PhoenixFrameBuffer_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixTexture.h"
#include "PhoenixDimensional2D.h"
#include "PhoenixAPI.h"
#include "PhoenixDefaultEntities.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for framebuffer objects.
    class PHOENIX_API CFrameBufferObject : public Phoenix::Graphics::COglBase,
					   public Phoenix::Spatial::CDimensional2D
    {
    protected:
      /// Handles to a textures.
      Phoenix::Default::TEXTURE_HANDLE m_hTextures[TEXTURE_HANDLE_COUNT];
      GLuint m_nColorBufferId;
      GLuint m_nDepthBufferId;        
      GLuint m_nStencilBufferId;
    public:
      ////////////////////
      /// Constructor.
      /// \param nWidth Width of this frame buffer in pixels.
      /// \param nHeight Height of this frame buffer in pixels.
      CFrameBufferObject();
      ////////////////////
      /// Destructor.
      ~CFrameBufferObject();
      ////////////////////
      /// Returns texture handle of selected colorbuffer.
      /// \param nColorBuffer Which color buffer.
      /// \returns TEXTURE_HANDLE
      Phoenix::Default::TEXTURE_HANDLE & GetTextureHandle( size_t nColorBuffer = 0);
      ////////////////////
      /// Returns texture handle of selected colorbuffer.
      /// \param nColorBuffer Which color buffer.
      /// \returns TEXTURE_HANDLE
      const Phoenix::Default::TEXTURE_HANDLE & GetTextureHandle( size_t nColorBuffer = 0) const;
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      GLuint & GetDepthBufferId();
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      const GLuint & GetDepthBufferId() const;
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      GLuint & GetColorBufferId();
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      const GLuint & GetColorBufferId() const;
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      GLuint & GetStencilBufferId();
      ////////////////////
      /// Returns reference to depth buffer id.
      /// \returns Depth Buffer id.
      const GLuint & GetStencilBufferId() const;
      ////////////////////
      /// Attach certain type of buffer to this framebuffer object.      
      bool AttachBuffer( int iBufferFlag, GLenum type, size_t width, size_t height );
      ////////////////////
        /// Attach texture to framebuffer.
        bool AttachTexture( const Phoenix::Default::TEXTURE_HANDLE & hTexture, unsigned int nColorBuffer=0 );
      ///////////////////
      /// Enables current render buffer as render target.
      void Apply();
      ////////////////////
      /// Checks framebuffer completeness.
      bool Validate();
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////

