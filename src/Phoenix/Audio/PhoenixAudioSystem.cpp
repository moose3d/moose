#include "PhoenixAudioSystem.h"
#include "PhoenixListener.h"
#include <iostream>
using namespace std;
namespace libname = Phoenix::Sound;
/////////////////////////////////////////////////////////////////
#define CHECK_ALC( A ) {			        \
  const ALCchar * tmp = alcGetString( NULL, A );\
  cerr << #A << " : " << (tmp ? tmp : "NULL");	\
  if ( alcGetError(NULL) == ALC_INVALID_ENUM )		\
  {							\
    cerr << "No such enum: '" << #A << "'";		\
  }							\
  cerr << endl;						\
}
/////////////////////////////////////////////////////////////////
#define CHECK_AL( A ) {			\
  const ALchar * tmp = alGetString( A );\
  cerr << #A << " : " << (tmp ? tmp : "NULL");	\
  if ( alGetError() == AL_INVALID_ENUM )		\
  {							\
    cerr << "No such enum: '" << #A << "'";		\
  }							\
  cerr << endl;						\
}
/////////////////////////////////////////////////////////////////
bool
libname::CAudioSystem::IsOpen() const
{
  return (alcGetCurrentContext() != NULL);
}
/////////////////////////////////////////////////////////////////
bool
libname::CAudioSystem::OpenDevice( const char *szDev )
{
  m_pDevice = alcOpenDevice( szDev );
  if (m_pDevice == NULL)
  {
    if ( szDev != NULL )
    {
      cerr << "Failure, device \'" << szDev << "\' cannot be opened" << endl; 
    }
    else 
    {
      cerr << "Failure, default device cannot be opened." << endl; 
    }
    return false;
  }

  //Create context(s)
  m_pContext=alcCreateContext(m_pDevice, NULL);

  //Set active context
  alcMakeContextCurrent(m_pContext);
  
  //alutInit(0, NULL);
  // Clear Error Code
  alGetError();
  cerr << "==== OpenAL System  ==== " << endl;
  CHECK_ALC( ALC_DEVICE_SPECIFIER );
  CHECK_ALC( ALC_DEFAULT_DEVICE_SPECIFIER);
  CHECK_ALC( ALC_EXTENSIONS);

  CHECK_AL( AL_VENDOR);
  CHECK_AL( AL_VERSION);
  CHECK_AL( AL_RENDERER);
  CHECK_AL( AL_EXTENSIONS);

  return true;
}
/////////////////////////////////////////////////////////////////
libname::CAudioSystem::CAudioSystem()
{
  
}
/////////////////////////////////////////////////////////////////
libname::CAudioSystem::~CAudioSystem()
{
  if ( m_pContext )
    alcDestroyContext( m_pContext );
  m_pContext = NULL;
  if ( m_pDevice )
    alcCloseDevice( m_pDevice );
  m_pDevice = NULL;
}

/////////////////////////////////////////////////////////////////
libname::CListener &
libname::CAudioSystem::GetListener()
{
  return m_Listener;
}
/////////////////////////////////////////////////////////////////
