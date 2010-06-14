#ifndef __COggStreamSample_h__
#define __COggStreamSample_h__
#include "PhoenixAPI.h"
#if !defined(PHOENIX_APPLE_IPHONE)
/////////////////////////////////////////////////////////////////
#include "PhoenixALSampleTypes.h"
#include <vorbis/vorbisfile.h>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    class PHOENIX_API COggStreamSample : public CALStreamSample
    {
    private:
      OggVorbis_File  m_OggFile;
      FILE *	  m_pFile;
      long		  m_lFileSize;
    public:
      COggStreamSample();
      ~COggStreamSample();
      bool Load( const char *szFilename );
      bool Stream( ALuint buffer );
      void Close();
      bool Rewind();
    };
    /////////////////////////////////////////////////////////////////
  }
}
#endif
#endif
