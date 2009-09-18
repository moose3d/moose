#include "PhoenixAABB.h"
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CAxisAlignedBox::CAxisAlignedBox() :
								  CPositional(CVector3<float>(0.0f,0.0f,0.0f)),
						      CDimensional3D(0.0f,0.0f,0.0f),
						      m_vMin(0.0f,0.0f,0.0f), m_vMax(0.0f,0.0f,0.0f)
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Volume::CAxisAlignedBox::CAxisAlignedBox( const CVector3<float> &vCenter,
						   float fWidth, float fHeight, float fLength ) :  CPositional( vCenter ),
												   CDimensional3D( fWidth, fHeight, fLength)
{
  m_vMin[0] = GetPosition()[0] - GetHalfWidth();
  m_vMin[1] = GetPosition()[1] - GetHalfHeight();
  m_vMin[2] = GetPosition()[2] - GetHalfLength();

  m_vMax[0] = GetPosition()[0] + GetHalfWidth();
  m_vMax[1] = GetPosition()[1] + GetHalfHeight();
  m_vMax[2] = GetPosition()[2] + GetHalfLength();
}
/////////////////////////////////////////////////////////////////
const CVector3<float> &
Phoenix::Volume::CAxisAlignedBox::GetMin() const
{
  return m_vMin;
}
/////////////////////////////////////////////////////////////////
const CVector3<float> &
Phoenix::Volume::CAxisAlignedBox::GetMax() const
{
  return m_vMax;
}
