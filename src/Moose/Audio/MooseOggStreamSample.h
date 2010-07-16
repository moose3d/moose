#ifndef __COggStreamSample_h__
#define __COggStreamSample_h__
#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)
/////////////////////////////////////////////////////////////////
#include "MooseALSampleTypes.h"
#include <vorbis/vorbisfile.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API COggStreamSample : public CALStreamSample
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
