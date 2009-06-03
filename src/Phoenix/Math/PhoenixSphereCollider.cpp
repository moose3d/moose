///////////////////////////////////////////////////////////////////////////////
#include "PhoenixSphereCollider.h"
namespace prefix = Phoenix::Collision;
///////////////////////////////////////////////////////////////////////////////
prefix::PhoenixSphereCollider::PhoenixSphereCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
prefix::PhoenixSphereCollider::~PhoenixSphereCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Volume::CSphere & sphere ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Volume::COrientedBox & box ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Volume::CCapsule & capsule ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Volume::CAxisAlignedBox & aabb ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Volume::CCone & cone ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Math::CRay & ray ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Math::CPlane & plane ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Math::CLineSegment & lineSegment ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Math::CLine & line ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::PhoenixSphereCollider::Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const
{
	return false;
}
///////////////////////////////////////////////////////////////////////////////
