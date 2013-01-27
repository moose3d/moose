/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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

