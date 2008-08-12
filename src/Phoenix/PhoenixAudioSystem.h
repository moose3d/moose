#ifndef __CAudioSystem_h__
#define __CAudioSystem_h__
/////////////////////////////////////////////////////////////////
#include <AL/al.h>
#include <AL/alc.h>
#include "PhoenixCore.h"
#include "PhoenixListener.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    class CAudioSystem : public Phoenix::Core::CSingleton<CAudioSystem>
    {
      friend class Phoenix::Core::CSingleton<CAudioSystem>;
    protected:
      ALCcontext *m_pContext;
      ALCdevice *m_pDevice;
      CListener  m_Listener;
      CAudioSystem();
      virtual ~CAudioSystem();
    public:
      void OpenDevice ( const char *szDevice = NULL );
      CListener &	     GetListener();
    };
  }
}
/////////////////////////////////////////////////////////////////
#define g_AudioManager  ( CAudioSystem::GetInstance()) 
#define g_StreamManager ( Phoenix::Core::CResourceManager<CALStreamSample, CHandle<CALStreamSample> >::GetInstance()) 
#define g_SampleManager ( Phoenix::Core::CResourceManager<CALSample, CHandle<CALSample> >::GetInstance()) 
/////////////////////////////////////////////////////////////////
#endif
