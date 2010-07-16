#ifndef __MooseFFMpeg_h__
#define __MooseFFMpeg_h__
///////////////////////////////////////////////////////////////////////////////
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}
#include <string>
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Graphics
    {
        /////////////////////////////////////////////////////////////////
        /// A Frame class which holds the ffmpeg AVFrame and the time it will be displayed on screen in seconds.
        class CFFMpegFrame
        {
        private:
            // The actual frame containing image information
            AVFrame *m_pFrame;
            // Passed time that this frame has been shown
            unsigned int m_nFrameDisplayedMS;
            unsigned int m_nFrameTime;
            unsigned int m_nCurrentFrame;

        public:
            ////////////////////
            /// The constructor
            CFFMpegFrame()
            {
                m_pFrame = NULL;
                m_nFrameDisplayedMS = 0;
                m_nCurrentFrame = 0;
            }
            ////////////////////
            /// Destructor.
            ~CFFMpegFrame()
            {
                if (m_pFrame != NULL ) av_free(m_pFrame);
                m_pFrame =NULL;
                m_nFrameDisplayedMS = 0;
            }
            ////////////////////
            /// Adds given time to display time.
            inline void AddToDisplayTimeMS( unsigned int nTimeMS )
            {
                m_nFrameDisplayedMS += nTimeMS;
            }
            ////////////////////
            /// Resets display time to 0.
            inline void ResetDisplayTime()
            {
                m_nFrameDisplayedMS = 0;
            }
            ////////////////////
            /// How long this frame has been displayed, in milliseconds.
            inline unsigned int GetDisplayTimeMS()
            {
                return m_nFrameDisplayedMS;
            }
            ////////////////////
            /// Returns the AVFrame.
            inline AVFrame *GetAVFrame() const
            {
                return m_pFrame;
            }
            ////////////////////
            /// Sets the AVFrame.
            inline void SetAVFrame( AVFrame *pFrame )
            {
                m_pFrame = pFrame;
            }
            ////////////////////
            /// Returns the current frame number.
            /// \return frame number.
            inline unsigned int GetNumber() const
            {
                if ( m_pFrame != NULL ) return m_pFrame->pts;
                return 0;
            }
        };
        /////////////////////////////////////////////////////////////////
        /// The class which decodes video stream using ffmpeg library.
        class CFFMpegStream
        {
        protected:
            /// Frame display time in milliseconds.
            unsigned int m_nFrameDisplayTimeMS;
	    /// Current time since playback has started.
	    int64_t m_currentTime;
            /// RGB / YUV image data.
            uint8_t *m_pBuffer;
            /// Stream filename.
            std::string m_strFilename;
            /// The format context for video.
            AVFormatContext *m_pFormatCtx;
            /// The codec context for video.
            AVCodecContext  *m_pCodecCtx;
            /// The videostream which is decoded from the file.
            int m_iVideoStream;
            /// Should video be looped, default false.
            int m_bLooping;
            /// The frame which was returned by latest GetNextFrame() call.
            CFFMpegFrame *m_pCurrentFrame;
            /// Retrieves the next frame from the video stream.
            int DecodeNextFrame( AVFrame *pFrame);
            /// Performs cleanup stuff.
            void Destroy();
            /// Initializes members variables.
            void Init();
            /// Subsection start.
            unsigned int m_nSectionStart;
            /// Subsection end.
            unsigned int m_nSectionEnd;
            /// Gets the time how long a frame must be displayed.
            /// \return frame display time in milliseconds.
            inline unsigned int GetFrameDisplayTimeMS() const
            {
                return m_nFrameDisplayTimeMS;
            }
            /// Sets frame display time.
            /// \param nTimeMS frame display time in milliseconds.
            inline void SetFrameDisplayTimeMS( unsigned int nTimeMS )
            {
                m_nFrameDisplayTimeMS = nTimeMS;
            }
            ////////////////////
            /// Returns the next frame from the stream and deletes the previously returned one (if one exists).
            /// When stream ends, NULL is returned.
            CFFMpegFrame *	GetNextFrame();
            /// Is stream playing.
            int m_bIsPlaying;

        public:
            /// Default constructor.
            CFFMpegStream();
            /// Destructor.
            ~CFFMpegStream();
            /// Copy constructor.
            CFFMpegStream( const CFFMpegStream &ref);
            /// Assignment operator.
            /// \param obj ffmpeg stream to be assigned.
            CFFMpegStream& operator=( const CFFMpegStream & obj );
            ////////////////////
            /// Sets the video stream filename to be read.
            /// \param sFilename path to video file.
            int SetStream( const char *sFilename );
            ////////////////////
            /// Returns the stream filename
            /// \return filename string.
            const std::string & GetStream() const;
            ////////////////////
            /// Has stream ended.
            // \return true if stream is in the end. (if looping, never true)
            int HasStreamEnded();
            ////////////////////
            /// Is stream looping.
            /// \return true if stream loops.
            int IsLooping();
            ////////////////////
            /// Sets the looping mode.
            /// \param bLooping true for enabling the loop, false for disabling.
            void SetLooping( int bLooping );
            ////////////////////
            /// Gets the pointer to the currently displayed frame.
            /// \return pointer to FFMpegFrame.
            CFFMpegFrame *	GetCurrentFrame();
            ////////////////////
            /// Seeks frame nFrame in stream.
            /// \param nFrame frame position.
            void Seek( unsigned int nFrame );
            ////////////////////
            /// Fills buffer with image data.
            void UpdateBuffer();
            ////////////////////
            /// Returns image data buffer.
            /// \returns pointer to m_pBuffer.
            inline uint8_t *GetBuffer() { return m_pBuffer; }
            ////////////////////
            /// Seek next frame to be displayed.
            /// \param nPassedTimeMS forwards stream by that many milliseconds.
            void SeekNextFrameFromStream( unsigned int nPassedTimeMS );
            /// Sets subsection of the stream to be played.
            /// \param nTime Subsection start time milliseconds.
            /// \param nDuration Subsection duration in milliseconds.
            void SetSection( unsigned int nTime, unsigned int nDuration );
            /// Returns subsection start time.
            inline unsigned int GetSectionStart() const
                    {
                return m_nSectionStart;
                    }
            /// Returns subsection end time.
            inline unsigned int GetSectionEnd() const
                    {
                return m_nSectionEnd;
                    }
            /// Sets stream to playing mode.
            inline void Play() { m_bIsPlaying = 1; m_currentTime = 0;}
            /// Stops playing.
            inline void Stop() { m_bIsPlaying = 0; }
            /// Is stream playing now.
            /// \return true if so, false otherwise.
            inline int IsPlaying() const { return m_bIsPlaying; }
            inline int GetWidth() const { return m_pCodecCtx->width; }
            inline int GetHeight() const { return m_pCodecCtx->height; }
        };
    } // Graphics
} // Moose
/////////////////////////////////////////////////////////////////
#endif
