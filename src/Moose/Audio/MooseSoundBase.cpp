#include "MooseSoundBase.h"
#include "MooseVector3.h"
namespace libname = Moose::Sound;
/////////////////////////////////////////////////////////////////
libname::CSoundBase::CSoundBase()
{
  SetPosition(0,0,0);
  SetDirection(0,0,-1);
  SetVelocity(Moose::Math::CVector3<float>(0,0,0));
}
/////////////////////////////////////////////////////////////////
void
libname::CSoundBase::SetVelocity(const Moose::Math::CVector3<float> & vDir )
{
  m_vVelocity = vDir;
}
/////////////////////////////////////////////////////////////////
const Moose::Math::CVector3<float> & 
libname::CSoundBase::GetVelocity() const
{
  return m_vVelocity;
}
/////////////////////////////////////////////////////////////////
