///////////////////////////////////////////////////////////////////////////////
#include "PhoenixCompoundCollider.h"
#include "PhoenixCollision.h"
#include <algorithm>
namespace prefix = Phoenix::Collision;
using namespace Phoenix::Collision;
using namespace std;
///////////////////////////////////////////////////////////////////////////////
prefix::PhoenixCompoundCollider::PhoenixCompoundCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
prefix::PhoenixCompoundCollider::~PhoenixCompoundCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixCompoundCollider::Intersects( const Phoenix::Volume::CSphere & sphere ) const
{
  ColliderList::const_iterator it = m_lstColliders.begin();
  for( ; it != m_lstColliders.end(); it++ )
  {
    if ( (*it)->Intersects(sphere)) return true;
  }
  return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixCompoundCollider::Intersects( const Phoenix::Graphics::CFrustum & frustum ) const
{
  ColliderList::const_iterator it = m_lstColliders.begin();
  for( ; it != m_lstColliders.end(); it++ )
  {
    if ( (*it)->Intersects(frustum)) return true;
  }
  return false;

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixCompoundCollider::Intersects( const Phoenix::Volume::COrientedBox & box ) const
{
  ColliderList::const_iterator it = m_lstColliders.begin();
  for( ; it != m_lstColliders.end(); it++ )
  {
    if ( (*it)->Intersects(box)) return true;
  }
  return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixCompoundCollider::Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const
{
  ColliderList::const_iterator it = m_lstColliders.begin();
  for( ; it != m_lstColliders.end(); it++ )
  {
    if ( (*it)->Intersects(vPoint)) return true;
  }
  return false;
}
///////////////////////////////////////////////////////////////////////////////
