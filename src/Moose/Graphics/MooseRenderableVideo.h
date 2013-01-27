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

#ifndef __MooseRenderableVideo_h__
#define __MooseRenderableVideo_h__
///////////////////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseRenderable.h"
#include "MooseFFMpeg.h"
#include "MooseFrameBuffer.h"
#include "MooseOGLRenderer.h"
#include "MooseDefaultEntities.h"
#include <string>
///////////////////////////////////////////////////////////////////////////////

namespace Moose
{

    namespace Graphics
    {
      
        ///////////////////////////////////////////////////////////////////////
        /// Renderable quad for video stream.
        class CRenderableVideo : public Moose::Graphics::CFFMpegStream,
	                         public Moose::Graphics::CRenderable,
	                         public Moose::Core::IUpdateable
        {
        protected:
            Moose::Graphics::CFrameBufferObject *m_pFrameBufferObj;
            Moose::Graphics::COglRenderer        m_Renderer;
        public:
            ////////////////////////7
            /// Creates renderable video object, framebufferobj must be supported.
            CRenderableVideo( const std::string & strFilename );
            virtual ~CRenderableVideo();
            void Render( Moose::Graphics::COglRenderer & r );
            void Update( float fSeconds );
        };

    }

}
#endif 
#endif /* MOOSERENDERABLEVIDEO_H_ */
