#ifndef __CALSoundTypes_h__
#define __CALSoundTypes_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixSoundBase.h"
#include "PhoenixALObjectArray.h"
#include "PhoenixALSampleTypes.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    class PHOENIX_API CALSoundOperations : virtual public CALObjectArray<1>,
			       virtual public CSoundBase
    {
    public:
      bool IsLooping();
      void SetLooping( bool flag);
  
      bool IsPlaying() const;  
      void Play();
  
      void Pause();
      void Stop();
      void Rewind();
      void CommitChanges();
    };
    /////////////////////////////////////////////////////////////////
    class PHOENIX_API CALSound : public CALSoundOperations
    {
    public:
      CALSound();
      virtual ~CALSound();
      void UseSample( CALSample & sample );
  
    };
    /////////////////////////////////////////////////////////////////
    class PHOENIX_API CALStreamSound : public CALSoundOperations
    {
    protected:
      CALStreamSample *m_pStream;
    public:
      CALStreamSound();
      virtual ~CALStreamSound();
      void UseStream( CALStreamSample * pSample );
      bool Update();
      void Clear();
      void Rewind();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
