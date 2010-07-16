#ifndef __MooseFrameBuffer_h__
#define __MooseFrameBuffer_h__
/////////////////////////////////////////////////////////////////
#include "MooseTexture.h"
#include "MooseDimensional2D.h"
#include "MooseAPI.h"
#include "MooseDefaultEntities.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Class for framebuffer objects.
    class MOOSE_API CFrameBufferObject : public Moose::Graphics::COglBase,
					   public Moose::Spatial::CDimensional2D
    {
    protected:
      /// Handles to a textures.
      Moose::Default::TEXTURE_HANDLE m_hTextures[TEXTURE_HANDLE_COUNT];
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
      Moose::Default::TEXTURE_HANDLE & GetTextureHandle( size_t nColorBuffer = 0);
      ////////////////////
      /// Returns texture handle of selected colorbuffer.
      /// \param nColorBuffer Which color buffer.
      /// \returns TEXTURE_HANDLE
      const Moose::Default::TEXTURE_HANDLE & GetTextureHandle( size_t nColorBuffer = 0) const;
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
        bool AttachTexture( const Moose::Default::TEXTURE_HANDLE & hTexture, unsigned int nColorBuffer=0 );
      ///////////////////
      /// Enables current render buffer as render target.
      void Apply();
      ////////////////////
      /// Checks framebuffer completeness.
      bool Validate();
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////

