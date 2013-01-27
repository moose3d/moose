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

#include "MooseAABB.h"
#include "MooseGlobals.h"
#include "MooseVertexDescriptor.h"
using namespace Moose::Math;
/////////////////////////////////////////////////////////////////
Moose::Volume::CAxisAlignedBox::CAxisAlignedBox() :
								  CPositional(CVector3<float>(0.0f,0.0f,0.0f)),
						      CDimensional3D(0.0f,0.0f,0.0f),
						      m_vMin(0.0f,0.0f,0.0f), m_vMax(0.0f,0.0f,0.0f)
{

}
/////////////////////////////////////////////////////////////////
Moose::Volume::CAxisAlignedBox::CAxisAlignedBox( const CVector3<float> &vCenter,
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
Moose::Volume::CAxisAlignedBox::GetMin() const
{
  return m_vMin;
}
/////////////////////////////////////////////////////////////////
const CVector3<float> &
Moose::Volume::CAxisAlignedBox::GetMax() const
{
  return m_vMax;
}
/////////////////////////////////////////////////////////////////
Moose::Volume::CAxisAlignedBox
Moose::Volume::CalculateAlignedBox( const Moose::Graphics::CVertexDescriptor &vertices )
{
	if ( vertices.GetSize() > 1)
	{
		CVector3<float> vMin(	vertices.GetPointer<float>()[0],
													vertices.GetPointer<float>()[1],
													vertices.GetPointer<float>()[2] );

		CVector3<float> vMax( vertices.GetPointer<float>()[0],
													vertices.GetPointer<float>()[1],
													vertices.GetPointer<float>()[2] );

		for( unsigned int v=1; v<vertices.GetSize(); v++)
		{
			STORE_MAX_MIN(vertices.GetPointer<float>(v)[0], vMax[0], vMin[0]);
			STORE_MAX_MIN(vertices.GetPointer<float>(v)[1], vMax[1], vMin[1]);
			STORE_MAX_MIN(vertices.GetPointer<float>(v)[2], vMax[2], vMin[2]);
		}
		CVector3<float> vDimension = (vMax - vMin);
		CVector3<float> vCenter = vMin + vDimension*0.5f;
		return CAxisAlignedBox( vCenter, vDimension[0], vDimension[1], vDimension[2]);
	}
	else
	{
		return CAxisAlignedBox();
	}
}
/////////////////////////////////////////////////////////////////
