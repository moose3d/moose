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
