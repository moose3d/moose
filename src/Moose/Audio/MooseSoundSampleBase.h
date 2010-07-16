#ifndef __CSoundSampleBase_h__
#define __CSoundSampleBase_h__
/////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <al.h>
#include <alc.h>
#elif __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <stdlib.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    /////////////////////////////////////////////////////////////////
    /// Class for generic sound sample. All other sample classes should be inherited from this.
    class MOOSE_API CSoundSampleBase
    {
    protected:
      ALenum     m_Format;
      ALsizei    m_Size;
      ALsizei    m_Freq;
      ALboolean  m_bLoop;
      size_t     m_nChannels;
      char *     m_pBufferData;
    protected:
      /////////////////////////////////////////////////////////////////
      CSoundSampleBase( const CSoundSampleBase & buf ) {}
      CSoundSampleBase( size_t nBufSize = 0 );
      /////////////////////////////////////////////////////////////////
      void		SetFormat( ALenum format );
      void		SetFreq( ALsizei f );
      void		SetNumChannels( size_t nCount );
      void		SetBufferSize( size_t nSize );
    public:  
      virtual	~CSoundSampleBase();
      /////////////////////////////////////////////////////////////////
      ALenum	GetFormat();
      ALsizei	GetFreq();
      size_t	GetNumChannels();
      virtual bool	Load( const char *szFilename ) = 0;
      size_t	GetBufferSize() const;
    };
    /////////////////////////////////////////////////////////////////
  }
}
#endif
