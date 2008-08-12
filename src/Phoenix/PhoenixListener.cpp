#include "PhoenixListener.h"
#include <AL/al.h>
namespace libname = Phoenix::Sound;
/////////////////////////////////////////////////////////////////
libname::CListener::CListener()
{
  SetPosition( 0,0,0);
  SetVelocity( CVector3<float>(0,0,0));
  SetDirectionForwardUp( CVector3<float>(0,0,-1), CVector3<float>(0,1,0));
}
/////////////////////////////////////////////////////////////////
libname::CListener::~CListener()
{
  // nop
}
/////////////////////////////////////////////////////////////////
void
libname::CListener::SetVelocity(const Phoenix::Math::CVector3<float> & vDir )
{
  m_vVelocity = vDir;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CVector3<float> & 
libname::CListener::GetVelocity() const
{
  return m_vVelocity;
}
/////////////////////////////////////////////////////////////////
void 
libname::CListener::CommitChanges()
{
  ALfloat buf[6];
  memcpy( buf, GetForwardVector().GetArray(), sizeof(float)*3);
  memcpy( &buf[3], GetUpVector().GetArray(), sizeof(float)*3);

  // set properties
  alListenerfv( AL_POSITION, GetPosition().GetArray());
  alListenerfv( AL_VELOCITY, GetVelocity().GetArray());
  alListenerfv( AL_ORIENTATION, buf );
}
/////////////////////////////////////////////////////////////////
