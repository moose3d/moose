#include "MooseListener.h"
#include <cstring>
#ifdef WIN32
#include <al.h>
#elif __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
namespace libname = Moose::Sound;
using namespace Moose::Math;
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
libname::CListener::SetVelocity(const Moose::Math::CVector3<float> & vDir )
{
  m_vVelocity = vDir;
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CVector3<float> &
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
