#include "MooseSphereBound.h"
/////////////////////////////////////////////////////////////////
Moose::Volume::CSphereBound::CSphereBound()
{

}
/////////////////////////////////////////////////////////////////
Moose::Volume::CSphereBound::~CSphereBound()
{

}
/////////////////////////////////////////////////////////////////
Moose::Volume::CSphere &
Moose::Volume::CSphereBound::GetBoundingSphere()
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
const Moose::Volume::CSphere &
Moose::Volume::CSphereBound::GetBoundingSphere() const
{
  return m_BoundingSphere;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Volume::CSphereBound::SetBoundingSphere( const Moose::Volume::CSphere & s )
{
  m_BoundingSphere = s;
}
/////////////////////////////////////////////////////////////////
