#include "PhoenixALSoundTypes.h"
#include <iostream>
using namespace std;
namespace libname = Phoenix::Sound;
/////////////////////////////////////////////////////////////////
bool 
libname::CALSoundOperations::IsLooping()
{
  ALint state;
  alGetSourcei( GetALObject(), AL_LOOPING, &state);
  return (state == AL_TRUE);
}
/////////////////////////////////////////////////////////////////
void 
libname::CALSoundOperations::SetLooping( bool flag)
{
  alSourcei( GetALObject(), AL_LOOPING, flag ? AL_TRUE : AL_FALSE );
}
/////////////////////////////////////////////////////////////////  
bool 
libname::CALSoundOperations::IsPlaying() const
{
  ALenum state;
  alGetSourcei( const_cast<CALSoundOperations *>(this)->GetALObject(), AL_SOURCE_STATE, &state );
  return ( state == AL_PLAYING );
}  
/////////////////////////////////////////////////////////////////
void 
libname::CALSoundOperations::Play()
{
  //cerr << "playing object: " << GetALObject() << endl;
  alSourcePlay( GetALObject());
}
/////////////////////////////////////////////////////////////////
void 
libname::CALSoundOperations::Pause()
{
  alSourcePause( GetALObject());
}
/////////////////////////////////////////////////////////////////
void
libname::CALSoundOperations::Stop()
{
  alSourceStop( GetALObject()); 
}
/////////////////////////////////////////////////////////////////
void 
libname::CALSoundOperations::Rewind()
{
  alSourceRewind( GetALObject());
}
/////////////////////////////////////////////////////////////////
void 
libname::CALSoundOperations::CommitChanges()
{
  alSourcefv( GetALObject(), AL_POSITION, GetPosition().GetArray() );
  alSourcefv( GetALObject(), AL_VELOCITY, GetVelocity().GetArray() );
  alSourcefv( GetALObject(), AL_DIRECTION, GetDirection().GetArray() );
}
/////////////////////////////////////////////////////////////////
libname::CALSound::CALSound()
{
  alGenSources(1, &GetALObject());
  //cerr << "Creating source : " << GetALObject() << endl;
  ReportAndHaltOnALErrors();
}
/////////////////////////////////////////////////////////////////
libname::CALSound::~CALSound()
{
  alDeleteSources(1, &GetALObject());
  alGetError();
}
/////////////////////////////////////////////////////////////////
void
libname::CALSound::UseSample( CALSample & sample )
{
  alSourcei(GetALObject(), AL_BUFFER, sample.GetALObject() );
  //cerr << "setting sample: " << sample.GetALObject() << " to sound " 
  //<< GetALObject() << endl;
}
/////////////////////////////////////////////////////////////////
libname::CALStreamSound::CALStreamSound() :  m_pStream(NULL)
{
  alGenSources(1, &GetALObject());
  ReportAndHaltOnALErrors();
}
/////////////////////////////////////////////////////////////////
libname::CALStreamSound::~CALStreamSound()
{
  alDeleteSources(1, &GetALObject());
  alGetError();
}
/////////////////////////////////////////////////////////////////
void 
libname::CALStreamSound::UseStream( CALStreamSample * pSample )
{
  m_pStream = pSample;
  cerr << "Using: " << GetALObject() << " with streams: " << m_pStream->GetALObject(0) << " and " << m_pStream->GetALObject(1) << endl;
  // enqueue two buffers
  alSourceQueueBuffers(GetALObject(), 2, &m_pStream->GetALObject());
  ReportAndHaltOnALErrors();
  // get data into buffer one
  m_pStream->Stream(m_pStream->GetALObject(0));
  ReportAndHaltOnALErrors();
  // get data into buffer two
  m_pStream->Stream(m_pStream->GetALObject(1));
  ReportAndHaltOnALErrors();
}
/////////////////////////////////////////////////////////////////
void
libname::CALStreamSound::Clear()
{
  int queued;
  alGetSourcei(GetALObject(), AL_BUFFERS_QUEUED, &queued);

  ALuint buffer;    
  while(queued--)
  {
    alSourceUnqueueBuffers(GetALObject(), 1, &buffer);
    ReportALErrors();
  }
}
/////////////////////////////////////////////////////////////////
bool
libname::CALStreamSound::Update()
{
  bool state = true;
  int iProcessed = 0;
  alGetSourcei(GetALObject(), AL_BUFFERS_PROCESSED, &iProcessed);

  // If stream is in end and loop is on, we go back to start
  if ( m_pStream->HasEnded() && IsLooping() ) 
  {
    //cerr << "rewinding" << endl;
    m_pStream->Rewind();
  }
  else if ( m_pStream->HasEnded() && !IsLooping() && iProcessed == 1 ) 
  {
    // no processing after 
    //cerr << "stream has ended." << endl;
    return false;
  }
  
  while( iProcessed--)
  {
    ALuint buffer;
        
    alSourceUnqueueBuffers(GetALObject(), 1, &buffer);
    ReportALErrors();
    
    state = m_pStream->Stream(buffer);

    alSourceQueueBuffers(GetALObject(), 1, &buffer);
    ReportALErrors();

  }
  return state;
}
/////////////////////////////////////////////////////////////////
void 
libname::CALStreamSound::Rewind()
{
  Stop();
  Clear();
  UseStream(m_pStream);
}
/////////////////////////////////////////////////////////////////
