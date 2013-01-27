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

#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseOggStreamSample.h"
#include <iostream>
/////////////////////////////////////////////////////////////////
using namespace std;
namespace libname = Moose::Sound;
/////////////////////////////////////////////////////////////////
#define BUFFER_SIZE (4096 * 4)
#define OGG_LITTLE_ENDIAN 0
#define OGG_BIG_ENDIAN 1
#define SIGNED_DATA 1
#define UNSIGNED_DATA 0
#define PCM_SAMPLE_16BIT 2
#define PCM_SAMPLE_8BIT 1
/////////////////////////////////////////////////////////////////
libname::COggStreamSample::COggStreamSample() : CALStreamSample( BUFFER_SIZE ), m_pFile(NULL),m_lFileSize(0)
{
  // buffer is allocated by ALStreamSample
}
/////////////////////////////////////////////////////////////////
libname::COggStreamSample::~COggStreamSample()
{
  Close();
}
/////////////////////////////////////////////////////////////////
void 
libname::COggStreamSample::Close()
{
  ov_clear( &m_OggFile );
  m_pFile = NULL;
}
/////////////////////////////////////////////////////////////////
bool
libname::COggStreamSample::Load( const char *szFilename )
{

  vorbis_info * pInfo;
  if ( !m_pFile )
    m_pFile = fopen( szFilename, "rb");
  
  if( ov_open_callbacks( m_pFile , &m_OggFile, NULL, 0, OV_CALLBACKS_DEFAULT) < 0) 
  {
    cerr << "Input does not appear to be an Ogg bitstream." << endl;
    return false;
  }
  
  pInfo = ov_info(&m_OggFile,-1);

  SetNumChannels( pInfo->channels);  // number of channels
  SetFreq( pInfo->rate);             // The frequency of the sampling rate
  
  // Check the number of channels... always use 16-bit samples
  if (GetNumChannels() == 1)
    SetFormat( AL_FORMAT_MONO16 );
  else
    SetFormat( AL_FORMAT_STEREO16 );
  cerr << "Freq: "     << GetFreq() << endl;
  cerr << "Channels: " << GetNumChannels() << endl;
  cerr << "Encoded: "  << ov_comment(&m_OggFile,-1)->vendor << endl;
  
  m_lFileSize = ov_raw_total( &m_OggFile, -1);

  return true;
}
/////////////////////////////////////////////////////////////////
bool
libname::COggStreamSample::Stream( ALuint buffer )
{
  
  if ( !m_pFile ) return false;

  bool done = false;
  int current_section;
  long readBytes = 0;
  while( readBytes < BUFFER_SIZE )
  {
    long ret=ov_read(&m_OggFile, m_pBufferData+readBytes, BUFFER_SIZE-readBytes, 
		     OGG_LITTLE_ENDIAN, PCM_SAMPLE_16BIT, SIGNED_DATA, &current_section);
    
    if (ret == 0)  done=1;
    else if (ret < 0) 
    {     
      throw std::string( "error in stream");
    } 
    else 
    {
      readBytes+=ret;
    }
  }
  //cerr << "read from stream :" << readBytes << endl;
  // if there was no more data to be read, stop.
  

  if ( ov_raw_tell(&m_OggFile) >= m_lFileSize-1 )
  {
    m_bHasEnded = true;
  }

  if ( readBytes == 0 )
  { 
    return false;
  }

  // Upload sound data to buffer
  alBufferData( buffer, GetFormat(), m_pBufferData, readBytes, GetFreq());

  ALenum error = alGetError();
  if ( error != AL_NO_ERROR )
  {
    cerr << __FUNCTION__ << GetOpenALErrorString(error) << endl;
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////////////
bool
libname::COggStreamSample::Rewind()
{
  if ( ov_seekable(&m_OggFile) )
  {
    ov_raw_seek( &m_OggFile, 0 );
    m_bHasEnded = false;
    return true;
  }
  else
  {
    cerr << "file is not seekable" << endl;
    return false;
  }
}
/////////////////////////////////////////////////////////////////
#endif
