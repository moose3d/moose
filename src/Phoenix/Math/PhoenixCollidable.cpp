#include "PhoenixCollidable.h"
////////////////////////////////////////////////////////////////////////////////
namespace prefix = Phoenix::Collision;
////////////////////////////////////////////////////////////////////////////////
prefix::CCollidable::CCollidable() : m_pCollider(NULL)
{
  
}
////////////////////////////////////////////////////////////////////////////////
prefix::CCollidable::~CCollidable()
{
  delete m_pCollider;
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CCollidable::SetCollider( Phoenix::Collision::ICollider *pCollider )
{
  m_pCollider = pCollider;
}
////////////////////////////////////////////////////////////////////////////////
Phoenix::Collision::ICollider *
prefix::CCollidable::GetCollider() 
{
  return m_pCollider;
}
////////////////////////////////////////////////////////////////////////////////
bool 
prefix::CCollidable::Intersects( const Phoenix::Volume::CSphere & sphere ) const
{
  if ( m_pCollider ) return m_pCollider->Intersects(sphere);
  else CSphereCollider::Intersects(sphere);
}
////////////////////////////////////////////////////////////////////////////////
bool 
prefix::CCollidable::Intersects( const Phoenix::Graphics::CFrustum & frustum ) const
{
  if ( m_pCollider ) return m_pCollider->Intersects(frustum);
  else CSphereCollider::Intersects(frustum);
}
////////////////////////////////////////////////////////////////////////////////
bool 
prefix::CCollidable::Intersects( const Phoenix::Volume::COrientedBox & box ) const
{
  if ( m_pCollider ) return m_pCollider->Intersects(box);
  else CSphereCollider::Intersects(box);
}
////////////////////////////////////////////////////////////////////////////////
bool 
prefix::CCollidable::Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const
{
  if ( m_pCollider ) return m_pCollider->Intersects(vPoint);
  else CSphereCollider::Intersects(vPoint);
}
////////////////////////////////////////////////////////////////////////////////
