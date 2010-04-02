#ifndef __PhoenixRenderableVideo_h__
#define __PhoenixRenderableVideo_h__
///////////////////////////////////////////////////////////////////////////////
#include "PhoenixRenderable.h"
///////////////////////////////////////////////////////////////////////////////
namespace Phoenix
{

    namespace Graphics
    {
        class CVideoTexture;
        class COglRenderer;
        ///////////////////////////////////////////////////////////////////////
        /// Renderable quad for video stream.
        class CRenderableVideo: public Phoenix::Graphics::CRenderable
        {
        protected:
            Phoenix::Graphics::CVideoTexture * m_pVideoTexture;

        public:
            CRenderableVideo();
            virtual ~CRenderableVideo();
            void SetTexture( Phoenix::Graphics::CVideoTexture * pTexture );
            Phoenix::Graphics::CVideoTexture * GetTexture();
            void Render( Phoenix::Graphics::COglRenderer & r );
            void Update(unsigned int nMilliSeconds );
        };

    }

}

#endif /* PHOENIXRENDERABLEVIDEO_H_ */
