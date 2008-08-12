#ifndef __CSoundSampleBase_h__
#define __CSoundSampleBase_h__
#include <AL/al.h>
#include <AL/alc.h>
#include <stdlib.h>
namespace Phoenix
{
  namespace Sound
  {
    /////////////////////////////////////////////////////////////////
    /// Class for generic sound sample. All other sample classes should be inherited from this.
    class CSoundSampleBase
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
