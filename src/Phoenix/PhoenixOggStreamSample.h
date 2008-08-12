#ifndef __COggStreamSample_h__
#define __COggStreamSample_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixALSampleTypes.h"
#include <vorbis/vorbisfile.h>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    class COggStreamSample : public CALStreamSample
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
