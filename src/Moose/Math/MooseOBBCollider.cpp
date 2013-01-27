/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
#include "MooseOBBCollider.h"
#include "MooseCollision.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
namespace prefix=Moose::Collision;
using namespace Moose::Collision;
using namespace Moose::Volume;
using namespace Moose::Math;
///////////////////////////////////////////////////////////////////////////////
prefix::COBBCollider::COBBCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
prefix::COBBCollider::~COBBCollider()
{

}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Volume::CSphere & sphere ) const
{
	if ( this->m_pTransform )
	{
        COrientedBox box = GetBoundingBox();
		box.Move( m_pTransform->GetTranslation());
		box.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		box.SetWidth(box.GetWidth()*vScale[0]);
        box.SetHeight(box.GetHeight()*vScale[1]);
        box.SetLength(box.GetLength()*vScale[2]);
        
		//box.CalculateCorners();
		//box.CalculatePlanes();
		return SphereIntersectsOBB( sphere, box );
	} else return SphereIntersectsOBB( sphere, GetBoundingBox() );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Graphics::CFrustum & frustum ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox box = GetBoundingBox();
		box.Move( m_pTransform->GetTranslation());
		box.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		box.SetWidth(box.GetWidth()*vScale[0]);
        box.SetHeight(box.GetHeight()*vScale[1]);
        box.SetLength(box.GetLength()*vScale[2]);
        //box.CalculateCorners();
		//box.CalculatePlanes();
		return OBBIntersectsPolytope( box, frustum);
	} else return OBBIntersectsPolytope( GetBoundingBox(), frustum );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Volume::COrientedBox & box ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);
        //thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return OBBIntersectsOBB( thisbox, box);
	} else return OBBIntersectsOBB( GetBoundingBox(), box );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Math::CRay & ray, float *pfValue ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);//thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return RayIntersectsOBB( ray, thisbox, pfValue);
	} else return RayIntersectsOBB( ray, GetBoundingBox(), pfValue );
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Math::CVector3<float> & vPoint ) const
{
	if ( this->m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);//thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return PointIntersectsOBB( vPoint, thisbox);
	} else return PointIntersectsOBB( vPoint, GetBoundingBox());
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Collision::ICollider & collider ) const
{
	if ( m_pTransform )
	{
		COrientedBox thisbox = GetBoundingBox();
		thisbox.Move( m_pTransform->GetTranslation());
		thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);//thisbox.CalculateCorners();
		//thisbox.CalculatePlanes();
		return collider.Intersects(thisbox);
	} else return collider.Intersects(GetBoundingBox());
}
///////////////////////////////////////////////////////////////////////////////
void
prefix::COBBCollider::Render( Moose::Graphics::COglRenderer & renderer )
{
	if ( m_pTransform ) renderer.CommitTransform( *m_pTransform );
	renderer.CommitBox( GetBoundingBox(), true );
	if ( m_pTransform ) renderer.RollbackTransform();
}
///////////////////////////////////////////////////////////////////////////////
bool
prefix::COBBCollider::Intersects( const Moose::Volume::CCapsule & capsule ) const
{
    if ( m_pTransform )
    {

        COrientedBox thisbox = GetBoundingBox();
        thisbox.Move( m_pTransform->GetTranslation());
        thisbox.AppendToRotation( m_pTransform->GetRotation() );
        CVector3<float> vScale = m_pTransform->GetScaling();
		thisbox.SetWidth(thisbox.GetWidth()*vScale[0]);
        thisbox.SetHeight(thisbox.GetHeight()*vScale[1]);
        thisbox.SetLength(thisbox.GetLength()*vScale[2]);
        // thisbox.CalculateCorners();
        //thisbox.CalculatePlanes();
        return Moose::Collision::OBBIntersectsCapsule( thisbox, capsule);
    } else return Moose::Collision::OBBIntersectsCapsule( GetBoundingBox(), capsule);
}
///////////////////////////////////////////////////////////////////////////////
