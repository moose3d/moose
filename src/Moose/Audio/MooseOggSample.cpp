#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseOggSample.h"
#include <vorbis/vorbisfile.h>
#include <iostream>
using namespace std;
namespace libname = Moose::Sound;
/////////////////////////////////////////////////////////////////
bool
libname::COggSample::Load( const char *szFilename )
{
  OggVorbis_File oggFile;  
  //char		pcmout[4096];

  int		bits = 16;
  vorbis_info * pInfo;

  FILE *f = fopen( szFilename, "rb");
  if( ov_open_callbacks( f , &oggFile, NULL, 0, OV_CALLBACKS_DEFAULT) < 0) 
  {
    cerr << "Input does not appear to be an Ogg bitstream." << endl;
    return false;
  }



  //char **ptr=ov_comment(&oggFile,-1)->user_comments;
  // Get some information about the OGG file
  // while(*ptr)
  //     {
  //       fprintf(stderr,"%s\n",*ptr);
  //       ++ptr;
  //     }
  //cerr << endl;

  pInfo = ov_info(&oggFile,-1);
  // number of channels
  SetNumChannels( pInfo->channels);
  // The frequency of the sampling rate
  SetFreq( pInfo->rate);
    
  //cerr << "Bitstream is " <<  << ", " << pInfo->rate << "Hz" << endl;
  //cerr << "Decoded length: " << (long)ov_pcm_total(&oggFile,-1) << " samples" << endl;
  //cerr << "Encoded by: " << ov_comment(&oggFile,-1)->vendor << endl;

  // Allocate buffer size
  long samples = (long)ov_pcm_total(&oggFile,-1);
  long bufsize = (bits/8)*pInfo->channels*samples;
  m_pBufferData = new char[bufsize];

  // Check the number of channels... always use 16-bit samples
  if (GetNumChannels() == 1)
    SetFormat( AL_FORMAT_MONO16 );
  else
    SetFormat( AL_FORMAT_STEREO16 );

#define OGGVORBIS_PCM_SAMPLE_16BIT 2
#define OGGVORBIS_PCM_SAMPLE_8BIT 1


  bool eof;
  int current_section;
  long readBytes = 0;
  while(!eof)
  {
    long ret=ov_read(&oggFile, m_pBufferData+readBytes,bufsize-readBytes,0,OGGVORBIS_PCM_SAMPLE_16BIT,1,&current_section);
    if (ret == 0) {
      /* EOF */
      eof=1;
    } else if (ret < 0) {
      cerr << "error in stream" << endl;
    } else {
      readBytes+=ret;
    }
  }
  ov_clear( &oggFile );

  // closing not necessary, ov_clear does it.
  //fclose(f);

  //cerr << "uploading buffer data to : " << GetALObject() << endl;
  // Upload sound data to buffer
  alBufferData(GetALObject(), GetFormat(), m_pBufferData, bufsize, GetFreq());
  ALenum error = alGetError();
  if ( error != AL_NO_ERROR )
  {
    cerr << GetOpenALErrorString(error) << endl;
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////
#endif
