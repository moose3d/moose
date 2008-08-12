#ifndef __CALSampleTypes_h__
#define __CALSampleTypes_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixSoundSampleBase.h"
#include "PhoenixALObjectArray.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    class CALSample : public CSoundSampleBase,
		      public CALObjectArray<1>
    {
    public:
      CALSample();
      ~CALSample();
    };
    /////////////////////////////////////////////////////////////////
    class CALStreamSample : public CSoundSampleBase,
			    public CALObjectArray<2>
    {
    protected:
      bool m_bHasEnded;
    public:
      CALStreamSample( size_t nBufSize = 0 );
      ~CALStreamSample();
      virtual bool Stream( ALuint buffer ) = 0;
      bool HasEnded();
      virtual bool Rewind() = 0;
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
