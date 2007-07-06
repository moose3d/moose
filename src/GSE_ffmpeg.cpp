/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gr�hn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
/////////////////////////////////////////////////////////////////
#include "GSE_ffmpeg.h"
#include "GSE_debug.h"
#include <iostream>
using std::cerr;
using std::endl;
#define CFFMPEG_USE_RGB_FORMAT
/////////////////////////////////////////////////////////////////
GSE_FFMpegStream::GSE_FFMpegStream() : m_bIsPlaying(0)
{
  Init();
}
/////////////////////////////////////////////////////////////////
GSE_FFMpegStream::~GSE_FFMpegStream()
{
  Destroy();
}
/////////////////////////////////////////////////////////////////
/* Allright, this code might be rather unstable and unpredictable since we actually 
   don't know how ffmpeg works internally.*/
GSE_FFMpegStream::GSE_FFMpegStream( const GSE_FFMpegStream &ref)
{

  m_bLooping = ref.m_bLooping;
  m_nSectionStart = ref.m_nSectionStart;
  m_nSectionEnd = ref.m_nSectionEnd;
  // open another stream context 
  if ( SetStream( ref.m_pFormatCtx->filename ) != 0 )
  {
    GSE_ERR("Cannot open stream!");
    // Move to same position
    av_seek_frame( m_pFormatCtx, m_iVideoStream, ref.m_pFormatCtx->timestamp, AVSEEK_FLAG_ANY );
    GetNextFrame();
  }
}
/////////////////////////////////////////////////////////////////
void
GSE_FFMpegStream::Init()
{
  m_pFormatCtx = NULL;
  m_pCodecCtx = NULL;
  m_iVideoStream = -1;
  m_bLooping = 0;
  m_pCurrentFrame = NULL;
  m_pBuffer = NULL;
  m_nFrameDisplayTimeMS = 0;
  
  
}
/////////////////////////////////////////////////////////////////
void
GSE_FFMpegStream::Destroy()
{
  // Delete the current frame
  if ( m_pCurrentFrame != NULL ) delete m_pCurrentFrame;
  m_pCurrentFrame = NULL;

  if ( m_pCodecCtx != NULL )  avcodec_close(m_pCodecCtx);
  m_pCodecCtx = NULL;

  if ( m_pFormatCtx != NULL ) av_close_input_file(m_pFormatCtx);
  m_pFormatCtx = NULL;

  if ( m_pBuffer != NULL ) delete m_pBuffer;
  m_pBuffer = NULL;

  m_iVideoStream = -1;
  m_bLooping = 0;  
}
/////////////////////////////////////////////////////////////////
GSE_FFMpegStream & 
GSE_FFMpegStream::operator=( GSE_FFMpegStream obj )
{

  Destroy();
  
  m_bLooping = obj.m_bLooping;
  m_nSectionStart = obj.m_nSectionStart;
  m_nSectionEnd = obj.m_nSectionEnd;
  // open another stream context 
  SetStream( obj.m_pFormatCtx->filename );
  // Move to same position
  av_seek_frame( m_pFormatCtx, m_iVideoStream, obj.m_pFormatCtx->timestamp, AVSEEK_FLAG_ANY );
  GetNextFrame();
  return *this;
}
/////////////////////////////////////////////////////////////////
int
GSE_FFMpegStream::SetStream( char *sFilename )
{

  GSE_DEBUG("Hello from SetStream");
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
  if(av_open_input_file(&m_pFormatCtx, sFilename, NULL, 0, NULL)!=0) 
  {
    GSE_ERR("Couldn't open file '" << sFilename  << "'");
    return -1;
  }
  m_strFilename = sFilename;

  // Retrieve stream information
  if(av_find_stream_info(m_pFormatCtx)<0)
  {
    GSE_ERR("Couldn't get stream information");
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
    if(m_pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
    {
      m_iVideoStream=i;
      break;
    }
  }
  // Check video strream
  if(m_iVideoStream==-1)
  {
    GSE_ERR("No video stream found");
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
    GSE_ERR("Couldn't open codec");
    return -1; 
  }
  
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

  cerr << "start time: " << m_pFormatCtx->start_time << endl;
  cerr << "duration: " << m_pFormatCtx->duration << endl;
  SetSection(0, m_pFormatCtx->duration);
  return 0;
}
/////////////////////////////////////////////////////////////////
// Returns the stream filename
string 
GSE_FFMpegStream::GetStream() const
{
  return m_strFilename;
}

/////////////////////////////////////////////////////////////////
FFMpegFrame * 
GSE_FFMpegStream::GetNextFrame()
{

  if ( m_pCurrentFrame == NULL )
  {
    m_pCurrentFrame = new FFMpegFrame();
  }
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
    GSE_DEBUG("Stream has ended!");
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
GSE_FFMpegStream::DecodeNextFrame( AVFrame *pFrame)
{

  static AVPacket packet;
  static int      bytesRemaining=0;
  static uint8_t  *rawData;
  static bool     fFirstTime=true;
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
      bytesDecoded=avcodec_decode_video(m_pCodecCtx, pFrame,
					&frameFinished, rawData, 
					bytesRemaining);
      
      // Was there an error?
      if(bytesDecoded < 0)
      {
	fprintf(stderr, "Error while decoding frame\n");
	return false;
      }

      bytesRemaining-=bytesDecoded;
      rawData+=bytesDecoded;

      // Did we finish the current frame? Then we can return
      if(frameFinished)
	return true;
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
    rawData=packet.data;
  }
  
 loop_exit:
  
  // Decode the rest of the last frame
  bytesDecoded=avcodec_decode_video(m_pCodecCtx, pFrame, &frameFinished, 
				    rawData, bytesRemaining);
  
  // Free last packet
  if(packet.data!=NULL)
    av_free_packet(&packet);
  
  return frameFinished!=0;
}
/////////////////////////////////////////////////////////////////
FFMpegFrame *
GSE_FFMpegStream::GetCurrentFrame()
{
  return m_pCurrentFrame;
}
/////////////////////////////////////////////////////////////////
int
GSE_FFMpegStream::HasStreamEnded()
{
  return GetCurrentFrame()->GetNumber() >= GetSectionEnd();
}
/////////////////////////////////////////////////////////////////
int 
GSE_FFMpegStream::IsLooping()
{
  return m_bLooping;
}
/////////////////////////////////////////////////////////////////
// Sets the looping mode - true for enabling the loop, false for disabling.
void 
GSE_FFMpegStream::SetLooping( int bLooping )
{
  m_bLooping = bLooping;
}
/////////////////////////////////////////////////////////////////
void 
GSE_FFMpegStream::Seek( unsigned int nFrame )
{
  int iFlags = 0;
  
  if ( nFrame < m_pCurrentFrame->GetAVFrame()->pts) iFlags = iFlags & AVSEEK_FLAG_BACKWARD;
  

  if ( av_seek_frame( m_pFormatCtx, m_iVideoStream, nFrame, iFlags ) < 0 )
  {
    GSE_ERR("Couldn't find frame " << nFrame );
    av_seek_frame( m_pFormatCtx, m_iVideoStream, 0, AVSEEK_FLAG_BACKWARD );
  }
  avcodec_flush_buffers(m_pFormatCtx->streams[m_iVideoStream]->codec);
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_FFMpegStream::UpdateBuffer()
{

  if ( m_pCurrentFrame == NULL ) return;


#ifdef CFFMPEG_USE_RGB_FORMAT
  AVFrame *pFrameRGB = avcodec_alloc_frame();
 
  // Assign appropriate parts of buffer to image planes in pFrameRGB
  avpicture_fill( reinterpret_cast<AVPicture *>(pFrameRGB),  
  		  GetBuffer(), PIX_FMT_RGB24,  
  		  m_pCodecCtx->width, m_pCodecCtx->height);
  
  // Setup YCbCr -> RGB color transform matrix
  //GSE_OglUtils::LoadColorMatrixYUV2RGB();
  
  // Convert YUV-frame into RGB-frame
  img_convert(reinterpret_cast<AVPicture *>(pFrameRGB), PIX_FMT_RGB24, 
  	      reinterpret_cast<AVPicture*>(m_pCurrentFrame->GetAVFrame()), 
  	      m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);
  av_free(pFrameRGB);

#else
  avpicture_fill( reinterpret_cast<AVPicture *>(m_pCurrentFrame->GetAVFrame()),  
  		  GetBuffer(), PIX_FMT_YUV420P,  
  		  m_pCodecCtx->width, m_pCodecCtx->height);
#endif
}
/////////////////////////////////////////////////////////////////
void 
GSE_FFMpegStream::SeekNextFrameFromStream( unsigned int nPassedTimeMS )
{
  // If stream is not playing, skip this.
  if ( !m_bIsPlaying ) 
  {
    return;
  }
  
  GetCurrentFrame()->AddToDisplayTimeMS(nPassedTimeMS);
  unsigned int nCurrFrameTime = GetCurrentFrame()->GetDisplayTimeMS();

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
GSE_FFMpegStream::SetSection( unsigned int nTime, unsigned int nDuration )
{
  m_nSectionStart = nTime;
  m_nSectionEnd   = nTime + nDuration;
  Seek(m_nSectionStart);
}
/////////////////////////////////////////////////////////////////
// $Log: GSE_ffmpeg.cpp,v $
// Revision 1.10  2007/05/18 10:24:05  entity
// playable and stoppable stream
//
// Revision 1.9  2007/05/17 20:13:37  entity
// initial section support
//
// Revision 1.8  2007/05/17 19:31:06  entity
// better design
//
// Revision 1.7  2007/05/16 13:02:11  entity
// stream name is stored now
//
// Revision 1.6  2007/03/21 08:20:43  entity
// Fixed unsigned int vs. int comparison warning,
// comments added
//
/////////////////////////////////////////////////////////////////
