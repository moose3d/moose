#ifndef __PhoenixVideoTexture_h__
#define __PhoenixVideoTexture_h__
/////////////////////////////////////////////////////////////////
#include <GL/GLee.h>
#include <vector>
#include <list>
#include "PhoenixOGLRenderer.h"
#include "PhoenixFFMpeg.h"
namespace Phoenix
{
    namespace Graphics
    {
        /////////////////////////////////////////////////////////////////
        /// /brief 2D Video texture class. Provides a updateable texture which displays video.
        class COglVideoTexture : public Phoenix::Graphics::COglTexture,
                              public Phoenix::Graphics::CFFMpegStream
        {
            friend class Phoenix::Graphics::COglRenderer;
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
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
