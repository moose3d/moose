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

#include "MooseFrustum.h"
#include "MooseCollision.h"
#include <vector>
/////////////////////////////////////////////////////////////////
using std::vector;
using namespace Moose::Graphics;
using namespace Moose::Math;
using namespace Moose::Volume;
/////////////////////////////////////////////////////////////////
Moose::Graphics::CFrustum::CFrustum()
{
  // Add planes to the vector
  CPlane plane;
  AddPlane( plane);
  AddPlane( plane);
  AddPlane( plane );
  AddPlane( plane );
  AddPlane( plane );
  AddPlane( plane );
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CFrustum::SetPlane(FRUSTUM_PLANE nIndex, CPlane &vPlane)
{
  m_Planes[nIndex] = vPlane;
}
/////////////////////////////////////////////////////////////////
CPlane &
Moose::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex )
{
  return m_Planes[nIndex];
}
/////////////////////////////////////////////////////////////////
const CPlane &
Moose::Graphics::CFrustum::GetPlane( FRUSTUM_PLANE nIndex ) const
{
  return m_Planes[nIndex];
}
/////////////////////////////////////////////////////////////////
CVector3<float>
Moose::Graphics::CFrustum::GetCorner( FRUSTUM_CORNER nIndex )
{
  return m_aCorners[nIndex];
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CFrustum::SetCorner( FRUSTUM_CORNER nIndex, const CVector3<float> & vPoint )
{
  m_aCorners[nIndex] = vPoint;  
}
/////////////////////////////////////////////////////////////////
