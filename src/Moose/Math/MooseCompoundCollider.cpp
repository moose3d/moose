#include "MooseCompoundCollider.h"
#include "MooseOGLRenderer.h"
#include <algorithm>
///////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Collision;
using namespace Moose::Collision;
using namespace std;
///////////////////////////////////////////////////////////////////////////////
prefix::CCompoundCollider::CCompoundCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
prefix::CCompoundCollider::~CCompoundCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCompoundCollider::Intersects( const Moose::Volume::CSphere & sphere ) const
{
	ColliderVector::const_iterator it  = m_vecColliders.begin();
	for ( ; it != m_vecColliders.end(); it++)
	{
		bool bCollides = (*it)->Intersects(sphere);
		if ( bCollides ) return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCompoundCollider::Intersects( const Moose::Graphics::CFrustum & frustum ) const
{
	ColliderVector::const_iterator it  = m_vecColliders.begin();
	for ( ; it != m_vecColliders.end(); it++)
	{
		bool bCollides = (*it)->Intersects(frustum);
		if ( bCollides ) return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCompoundCollider::Intersects( const Moose::Volume::COrientedBox & box ) const
{
	ColliderVector::const_iterator it  = m_vecColliders.begin();
	for ( ; it != m_vecColliders.end(); it++)
	{
		bool bCollides = (*it)->Intersects(box);
		if ( bCollides ) return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCompoundCollider::Intersects( const Moose::Math::CRay & ray, float *pfValue ) const
{
	ColliderVector::const_iterator it  = m_vecColliders.begin();
	for ( ; it != m_vecColliders.end(); it++)
	{
		bool bCollides = (*it)->Intersects(ray, pfValue);
		if ( bCollides ) return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCompoundCollider::Intersects( const Moose::Math::CVector3<float> & vPoint ) const
{
	ColliderVector::const_iterator it  = m_vecColliders.begin();
	for ( ; it != m_vecColliders.end(); it++)
	{
		bool bCollides = (*it)->Intersects(vPoint);
		if ( bCollides ) return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCompoundCollider::Intersects( const Moose::Collision::ICollider & collider ) const
{
	// Betweem very large collision compound objects this is not very efficient;
	// O(n*m), where n is the number of colliders in this, and m the number of colliders in collider.
	ColliderVector::const_iterator it  = m_vecColliders.begin();
	for ( ; it != m_vecColliders.end(); it++)
	{
		bool bCollides = (*it)->Intersects(collider);
		if ( bCollides ) return true;
	}
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::CCompoundCollider::Intersects( const Moose::Volume::CCapsule & capsule ) const
{
    ColliderVector::const_iterator it  = m_vecColliders.begin();
    for ( ; it != m_vecColliders.end(); it++)
    {
       bool bCollides = (*it)->Intersects(capsule);
       if ( bCollides ) return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////////////////////
struct CallRender
{
	Moose::Graphics::COglRenderer & r;
	CallRender( Moose::Graphics::COglRenderer & rend ) : r(rend) {}
	void operator()( ICollider *pCollider )
	{
		pCollider->Render(r);
	}
};
///////////////////////////////////////////////////////////////////////////////
void
prefix::CCompoundCollider::Render( Moose::Graphics::COglRenderer & renderer )
{
	if ( m_pTransform )	renderer.CommitTransform(*m_pTransform);
	for_each( m_vecColliders.begin(), m_vecColliders.end(), CallRender(renderer) );
	if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CCompoundCollider::AddCollider( ICollider *pCollider )
{
	// Do not allow compound colliders to be chained, otherwise BAD things will happen.
	if ( dynamic_cast<CCompoundCollider *>(pCollider) != NULL ) return;
	pCollider->SetColliderTransform( m_pTransform );
	m_vecColliders.push_back(pCollider);
}
///////////////////////////////////////////////////////////////////////////////
ColliderVector &
prefix::CCompoundCollider::GetColliders()
{
	return m_vecColliders;
}
///////////////////////////////////////////////////////////////////////////////
const ColliderVector &
prefix::CCompoundCollider::GetColliders() const
{
	return m_vecColliders;
}
///////////////////////////////////////////////////////////////////////////////
ICollider *
prefix::CCompoundCollider::At( int index )
{
	return m_vecColliders[index];
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::CCompoundCollider::SetColliderTransform( Moose::Math::CTransform *pTransform )
{
	m_pTransform = pTransform;

	// Set transform for each sub-collider as well.
	ColliderVector::iterator it = m_vecColliders.begin();
	for( ; it != m_vecColliders.end(); it++)
	{
		(*it)->SetColliderTransform( pTransform );
	}
}
///////////////////////////////////////////////////////////////////////////////
