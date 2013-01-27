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

#ifndef __MooseVideoTexture_h__
#define __MooseVideoTexture_h__
/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)

#include "MooseGL.h"
#include <vector>
#include <list>
#include "MooseOGLRenderer.h"
#include "MooseFFMpeg.h"
namespace Moose
{
    namespace Graphics
    {
        /////////////////////////////////////////////////////////////////
        /// /brief 2D Video texture class. Provides a updateable texture which displays video.
        class COglVideoTexture : public Moose::Graphics::COglTexture,
                              public Moose::Graphics::CFFMpegStream
        {
            friend class Moose::Graphics::COglRenderer;
        protected:
            /// Framebuffer name.
            GLuint m_FrameBuffer;
            /// Is frame_buffer_object extension supported.
            int    m_bFrameBufferObjectSupport;
            /// The time which is left in current frame.
            float m_fRemainingTimeInFrame;
            /// The parametrized constructor.
            COglVideoTexture( unsigned int id, const char *sFilename );
            /// Initializes texture.
            void Init();
        public:

            /// Destructor.
            ~COglVideoTexture();
            /// Updates the video texture to display correct frame.
            /// \param nPassedTimeMS passed time in milliseconds.
            void Update( unsigned int nPassedTimeMS );
            /// Destroys texture.
            void Destroy();

        };

    }
}
#endif
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
