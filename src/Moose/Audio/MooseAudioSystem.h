#ifndef __CAudioSystem_h__
#define __CAudioSystem_h__
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
#include "MooseSingleton.h"
#include "MooseListener.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API CAudioSystem : public Moose::Core::CSingleton<Moose::Sound::CAudioSystem>
    {
      friend class Moose::Core::CSingleton<Moose::Sound::CAudioSystem>;
    protected:
      ALCcontext *m_pContext;
      ALCdevice *m_pDevice;
      CListener  m_Listener;
      CAudioSystem();
      virtual ~CAudioSystem();
    public:
      /// Attempts to open device.
      /// \param szDevice Device name or path.
      /// \returns true, if successful.
      /// \returns false on failure.
      bool OpenDevice ( const char *szDevice = NULL );
      ////////////////////
      /// Checks is (some) device opened.
      bool IsOpen() const;
      CListener &	     GetListener();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
