#ifndef __CALSampleTypes_h__
#define __CALSampleTypes_h__
/////////////////////////////////////////////////////////////////
#include "MooseSoundSampleBase.h"
#include "MooseALObjectArray.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API CALSample : public CSoundSampleBase,
		      public CALObjectArray<1>
    {
    public:
      CALSample();
      ~CALSample();
    };
    /////////////////////////////////////////////////////////////////
    class MOOSE_API CALStreamSample : public CSoundSampleBase,
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
