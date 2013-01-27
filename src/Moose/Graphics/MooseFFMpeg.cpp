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

/////////////////////////////////////////////////////////////////
#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseFFMpeg.h"
#include "MooseDefaultEntities.h"
#include "MooseLogger.h"
#include <iostream>
#include <cassert>
extern "C" {
  #include "libswscale/swscale.h"
}
using std::cerr;
using std::endl;
using namespace Moose::Graphics;
using namespace Moose::Default;
namespace prefix=Moose::Graphics;
/////////////////////////////////////////////////////////////////
#define CFFMPEG_USE_RGB_FORMAT
//#ifdef CFFMPEG_USE_RGB_FORMAT
//#undef CFFMPEG_USE_RGB_FORMAT
//#endif
/////////////////////////////////////////////////////////////////
prefix::CFFMpegStream::CFFMpegStream() : m_bIsPlaying(0)
{
  Init();
}
/////////////////////////////////////////////////////////////////
prefix::CFFMpegStream::~CFFMpegStream()
{
  Destroy();
}
/////////////////////////////////////////////////////////////////
/* Allright, this code might be rather unstable and unpredictable since we actually 
   don't know how ffmpeg works internally.*/
prefix::CFFMpegStream::CFFMpegStream( const CFFMpegStream &ref)
{

  m_bLooping = ref.m_bLooping;
  m_nSectionStart = ref.m_nSectionStart;
  m_nSectionEnd = ref.m_nSectionEnd;
  // open another stream context 
  if ( SetStream( ref.m_pFormatCtx->filename ) != 0 )
  {
    g_Error << "Cannot open stream!" << endl;
    // Move to same position
    av_seek_frame( m_pFormatCtx, m_iVideoStream, ref.m_pFormatCtx->start_time, AVSEEK_FLAG_ANY );
    GetNextFrame();
  }
}
/////////////////////////////////////////////////////////////////
void
prefix::CFFMpegStream::Init()
{
  m_pFormatCtx = NULL;
  m_pCodecCtx = NULL;
  m_iVideoStream = -1;
  m_bLooping = 0;
  m_pBuffer = NULL;
  m_nFrameDisplayTimeMS = 0;
  m_pCurrentFrame = NULL;
}
/////////////////////////////////////////////////////////////////
void
prefix::CFFMpegStream::Destroy()
{
  // Delete the current frame
  if ( m_pCurrentFrame != NULL ) delete m_pCurrentFrame;
  m_pCurrentFrame = NULL;

  if ( m_pCodecCtx != NULL )  avcodec_close(m_pCodecCtx);
  m_pCodecCtx = NULL;

  if ( m_pFormatCtx != NULL ) avformat_close_input(&m_pFormatCtx);
  m_pFormatCtx = NULL;

  if ( m_pBuffer != NULL ) delete m_pBuffer;
  m_pBuffer = NULL;

  m_iVideoStream = -1;
  m_bLooping = 0;  

}
/////////////////////////////////////////////////////////////////
prefix::CFFMpegStream &
prefix::CFFMpegStream::operator=( const prefix::CFFMpegStream & obj )
{

  Destroy();
  
  m_bLooping = obj.m_bLooping;
  m_nSectionStart = obj.m_nSectionStart;
  m_nSectionEnd = obj.m_nSectionEnd;
  // open another stream context 
  SetStream( obj.m_pFormatCtx->filename );
  // Move to same position
  av_seek_frame( m_pFormatCtx, m_iVideoStream, obj.m_pFormatCtx->start_time, AVSEEK_FLAG_ANY );
  GetNextFrame();
  return *this;
}
/////////////////////////////////////////////////////////////////
int
prefix::CFFMpegStream::SetStream( const char *sFilename )
{


  unsigned int             i;
  AVCodec         *pCodec;
  // sanity check
  if ( sFilename == NULL ) 
  {
    return -1;
  }
  // Clean previous stuff
  Destroy();

  // Open file
  if(avformat_open_input(&m_pFormatCtx, sFilename, NULL, NULL)!=0) 
  {
    g_Error << "Couldn't open file '" << sFilename  << "'" << endl;
    return -1;
  }
  m_strFilename = sFilename;

  // Retrieve stream information
  if(av_find_stream_info(m_pFormatCtx)<0)
  {
    g_Error << "Couldn't get stream information" << endl;
    return -1;
  }

  ////////////////////
  // Dump information about file onto standard error
  dump_format(m_pFormatCtx, 0, sFilename, false);
  ////////////////////

  // Find the first video stream
  m_iVideoStream=-1;
  for(i=0; i<m_pFormatCtx->nb_streams; i++)
  {
    if(m_pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
    {
      m_iVideoStream=i;
      break;
    }
  }
  // Check video strream
  if(m_iVideoStream==-1)
  {
    g_Error << "No video stream found" << endl;
    return -1; 
  }
  
  // Get a pointer to the codec context for the video stream
  m_pCodecCtx=m_pFormatCtx->streams[m_iVideoStream]->codec;

  // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(m_pCodecCtx->codec_id);

  if(pCodec==NULL) 
  {
    return -1; // Codec not found
  }
  
  // Inform the codec that we can handle truncated bitstreams -- i.e.,
  // bitstreams where frame boundaries can fall in the middle of packets
  if(pCodec->capabilities & CODEC_CAP_TRUNCATED)
    m_pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
  
  // Open codec
  if(avcodec_open(m_pCodecCtx, pCodec)<0)
  {
    g_Error << "Couldn't open codec" << endl;
    return -1; 
  }
  // if this would be fixed framerate content...
  SetFrameDisplayTimeMS( 1000 / m_pCodecCtx->time_base.den);

  // Determine required buffer size, and allocate
#ifdef CFFMPEG_USE_RGB_FORMAT
  int iNumBytes = avpicture_get_size(PIX_FMT_RGB24,
				     m_pCodecCtx->width, 
				     m_pCodecCtx->height);
#else
  int iNumBytes = avpicture_get_size(PIX_FMT_YUV420P,
				     m_pCodecCtx->width, 
				     m_pCodecCtx->height);
#endif
  m_pBuffer = new uint8_t[iNumBytes];

  g_Log << "start time: " << m_pFormatCtx->start_time << endl;
  g_Log << "duration: " << m_pFormatCtx->duration << endl;
  SetSection(0, m_pFormatCtx->duration);
  // Create a new frame 
  m_pCurrentFrame = new CFFMpegFrame();
  return 0;
}
/////////////////////////////////////////////////////////////////
// Returns the stream filename
const std::string &
prefix::CFFMpegStream::GetStream() const
{
  return m_strFilename;
}

/////////////////////////////////////////////////////////////////
prefix::CFFMpegFrame *
prefix::CFFMpegStream::GetNextFrame()
{
  AVFrame *pFrameYUV = NULL;
  /// Allocate frame only once, and reuse.
  if ( m_pCurrentFrame->GetAVFrame()== NULL )
  {
    pFrameYUV = avcodec_alloc_frame();
    m_pCurrentFrame->SetAVFrame(pFrameYUV);
  } 
  else 
  {
    pFrameYUV = m_pCurrentFrame->GetAVFrame();
  }
  // On error, stop stream
  if ( DecodeNextFrame( pFrameYUV ) == 0 ) 
  {
    Stop();
  } 
  else if ( HasStreamEnded() )
  {
  // Decode next frame and observe stream state
    //g_Debug << "Stream has ended!" << endl;
    if ( IsLooping())
    {
      Seek( GetSectionStart());
      DecodeNextFrame( pFrameYUV);
    }
    else 
    {
      Stop();
    }
  }
  if ( !HasStreamEnded() && IsPlaying() )  m_pCurrentFrame->ResetDisplayTime();
  
  return m_pCurrentFrame;
}
/////////////////////////////////////////////////////////////////
int 
prefix::CFFMpegStream::DecodeNextFrame( AVFrame *pFrame)
{
  AVPacket packet;
  int      bytesRemaining=0;

  bool     fFirstTime=true;
  int             bytesDecoded;
  int             frameFinished;

  // First time we're called, set packet.data to NULL to indicate it
  // doesn't have to be freed
  if(fFirstTime)
  {
    fFirstTime=false;
    packet.data=NULL;
  }

  // Decode packets until we have decoded a complete frame
  while(true)
  {
    // Work on the current packet until we have decoded all of it
    while(bytesRemaining > 0)
    {
      // Decode the next chunk of data
      bytesDecoded=avcodec_decode_video2(m_pCodecCtx, pFrame,
                                         &frameFinished, &packet);
      
      // Was there an error?
      if(bytesDecoded < 0)
      {
        g_Error << "Error while decoding frame\n";
        return false;
      }

      bytesRemaining-=bytesDecoded;
      //rawData+=bytesDecoded;

      // Did we finish the current frame? Then we can return
      if(frameFinished)
      {
        //packet->pts*(m_pCodecCtx.time_base.num/m_pCodecCtx.time_base.den);
        av_free_packet(&packet);
        return true;
      }
    }
    
    // Read the next packet, skipping all packets that aren't for this
    // stream
    do
    {
      // Free old packet
      if(packet.data!=NULL)
        av_free_packet(&packet);
      
      // Read new packet
      if(av_read_frame(m_pFormatCtx, &packet)<0)
        goto loop_exit;
    } while(packet.stream_index!=m_iVideoStream);
    
    bytesRemaining=packet.size;
    //rawData=packet.data;
  }
  
 loop_exit:  
  // Decode the rest of the last frame
  //bytesDecoded=avcodec_decode_video2(m_pCodecCtx, pFrame, &frameFinished,
  //				    &packet);
  
  // Free last packet
  if(packet.data!=NULL)
    av_free_packet(&packet);
 
  return frameFinished!=0;
}
/////////////////////////////////////////////////////////////////
prefix::CFFMpegFrame *
prefix::CFFMpegStream::GetCurrentFrame()
{
  return m_pCurrentFrame;
}
/////////////////////////////////////////////////////////////////
int
prefix::CFFMpegStream::HasStreamEnded()
{
  return GetCurrentFrame()->GetNumber() >= GetSectionEnd();
}
/////////////////////////////////////////////////////////////////
int 
prefix::CFFMpegStream::IsLooping()
{
  return m_bLooping;
}
/////////////////////////////////////////////////////////////////
// Sets the looping mode - true for enabling the loop, false for disabling.
void 
prefix::CFFMpegStream::SetLooping( int bLooping )
{
  m_bLooping = bLooping;
}
/////////////////////////////////////////////////////////////////
void 
prefix::CFFMpegStream::Seek( unsigned int nFrame )
{
  int iFlags = 0;
  
  if ( nFrame < m_pCurrentFrame->GetAVFrame()->pts) iFlags = iFlags & AVSEEK_FLAG_BACKWARD;
  

  if ( av_seek_frame( m_pFormatCtx, m_iVideoStream, nFrame, iFlags ) < 0 )
  {
    g_Error << "Couldn't find frame " << nFrame  << endl;
    av_seek_frame( m_pFormatCtx, m_iVideoStream, 0, AVSEEK_FLAG_BACKWARD );
  }
  avcodec_flush_buffers(m_pFormatCtx->streams[m_iVideoStream]->codec);
  
}
/////////////////////////////////////////////////////////////////
void 
prefix::CFFMpegStream::UpdateBuffer()
{

  if ( m_pCurrentFrame == NULL ) return;
  if ( m_pCurrentFrame->GetAVFrame() == NULL ) return;

#ifdef CFFMPEG_USE_RGB_FORMAT
  AVFrame *pFrameRGB = avcodec_alloc_frame();
 
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  avpicture_fill( reinterpret_cast<AVPicture *>(pFrameRGB),  
  		  GetBuffer(), PIX_FMT_RGB24,  
  		  m_pCodecCtx->width, m_pCodecCtx->height);
  
  // Setup YCbCr -> RGB color transform matrix
  //GSE_OglUtils::LoadColorMatrixYUV2RGB();
  
  // Convert YUV-frame into RGB-frame
  // Deprecated code:
  /*img_convert(reinterpret_cast<AVPicture *>(pFrameRGB), PIX_FMT_RGB24,
  	      reinterpret_cast<AVPicture*>(m_pCurrentFrame->GetAVFrame()), 
   	      m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);*/
  struct SwsContext *pSwsCtx = sws_getContext(GetWidth(), GetHeight(), 
					      m_pCodecCtx->pix_fmt, 
					      GetWidth(), GetHeight(), 
					      PIX_FMT_RGB24, SWS_BICUBIC, 
					      NULL, NULL, NULL);
  
  
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  sws_scale(pSwsCtx, 
	    m_pCurrentFrame->GetAVFrame()->data, 
	    m_pCurrentFrame->GetAVFrame()->linesize, 
	    0, GetHeight(), 
	    pFrameRGB->data, 
	    pFrameRGB->linesize);

  
  av_free(pFrameRGB);
  sws_freeContext(pSwsCtx);

#else

  /*memcpy( GetBuffer(), 
	  m_pCurrentFrame->GetAVFrame()->data[0], 
	  avpicture_get_size(PIX_FMT_YUV420P,
			     m_pCodecCtx->width, 
			     m_pCodecCtx->height));*/
  avpicture_fill( reinterpret_cast<AVPicture *>(GetCurrentFrame()->GetAVFrame()),  
  		  GetBuffer(), PIX_FMT_YUV420P,  
   		  m_pCodecCtx->width, m_pCodecCtx->height);
  
#endif
}
/////////////////////////////////////////////////////////////////
void 
prefix::CFFMpegStream::SeekNextFrameFromStream( unsigned int nPassedTimeMS )
{
  // If stream is not playing, skip this.
  if ( !m_bIsPlaying ) 
  {
    return;
  }
  assert ( GetCurrentFrame() != NULL );
  GetCurrentFrame()->AddToDisplayTimeMS(nPassedTimeMS);
  unsigned int nCurrFrameTime = GetCurrentFrame()->GetDisplayTimeMS();
  m_currentTime += nPassedTimeMS;
  
  // If enough time has passed, get new frame and update buffer
  if ( nCurrFrameTime >= GetFrameDisplayTimeMS() )
  {
    while ( nCurrFrameTime >= GetFrameDisplayTimeMS())
    {
      GetNextFrame();
      nCurrFrameTime-=GetFrameDisplayTimeMS();
      
    } 
    UpdateBuffer();
  }

}
/////////////////////////////////////////////////////////////////
void 
prefix::CFFMpegStream::SetSection( unsigned int nTime, unsigned int nDuration )
{
  m_nSectionStart = nTime;
  m_nSectionEnd   = nTime + nDuration;
  Seek(m_nSectionStart);
}
/////////////////////////////////////////////////////////////////

#endif
