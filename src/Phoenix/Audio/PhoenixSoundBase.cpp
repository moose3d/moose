#include "PhoenixSoundBase.h"
namespace libname = Phoenix::Sound;
/////////////////////////////////////////////////////////////////
libname::CSoundBase::CSoundBase()
{
  SetPosition(0,0,0);
  SetDirection(0,0,-1);
  SetVelocity(CVector3<float>(0,0,0));
}
/////////////////////////////////////////////////////////////////
void
libname::CSoundBase::SetVelocity(const Phoenix::Math::CVector3<float> & vDir )
{
  m_vVelocity = vDir;
}
/////////////////////////////////////////////////////////////////
const Phoenix::Math::CVector3<float> & 
libname::CSoundBase::GetVelocity() const
{
  return m_vVelocity;
}
/////////////////////////////////////////////////////////////////
