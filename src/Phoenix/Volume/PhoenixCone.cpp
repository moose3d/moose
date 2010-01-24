#include "PhoenixCone.h"
using namespace Phoenix::Math;
using namespace Phoenix::Volume;
/////////////////////////////////////////////////////////////////
void 
Phoenix::Volume::CCone::SetAngle( float fAngle )
{
  m_fAngle = fAngle;
  m_fSinAngle = sinf(m_fAngle);
  m_fCosAngle = cosf(m_fAngle);
  m_fSinReciprocal = (1.0f / m_fSinAngle);
  m_fCosAngleSqr = m_fCosAngle * m_fCosAngle;
  m_fSinAngleSqr = m_fSinAngle * m_fSinAngle;
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CCone::CCone() : CPositional(), m_vDirection(CVector3<float>(0.0f,0.0f,-1.0f)),m_fLength(1.0f)
{
  SetAngle(0.0f);
}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CCone::CCone( const CVector3<float> &vPos, const CVector3<float> &vDir, float fAngle, float fLength ) : CPositional(vPos),m_vDirection(vDir)
{
  SetAngle(fAngle);
  m_fLength = fLength;
}
/////////////////////////////////////////////////////////////////
