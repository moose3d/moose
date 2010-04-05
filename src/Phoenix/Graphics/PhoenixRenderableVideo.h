#ifndef __PhoenixRenderableVideo_h__
#define __PhoenixRenderableVideo_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixRenderable.h"
#include "PhoenixFFMpeg.h"
#include "PhoenixFrameBuffer.h"
#include "PhoenixOGLRenderer.h"
#include "PhoenixDefaultEntities.h"
#include <string>
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{

    namespace Graphics
    {

        ///////////////////////////////////////////////////////////////////////
        /// Renderable quad for video stream.
        class CRenderableVideo : public Phoenix::Graphics::CFFMpegStream,
	                         public Phoenix::Graphics::CRenderable,
	                         public Phoenix::Core::IUpdateable
        {
        protected:
            Phoenix::Graphics::CFrameBufferObject *m_pFrameBufferObj;
            Phoenix::Graphics::COglRenderer        m_Renderer;
        public:
            ////////////////////////7
            /// Creates renderable video object, framebufferobj must be supported.
            CRenderableVideo( const std::string & strFilename );
            virtual ~CRenderableVideo();
            void Render( Phoenix::Graphics::COglRenderer & r );
            void Update( float fSeconds );
        };

    }

}

#endif /* PHOENIXRENDERABLEVIDEO_H_ */
