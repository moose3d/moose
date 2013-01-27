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

#include <MooseQuad.h>
#include <MooseVector2.h>
#include <iostream>
using namespace Moose::Math;
using namespace std;
/////////////////////////////////////////////////////////////////
std::ostream&
Moose::Math::operator<<( std::ostream &stream, const CQuad & quad )
{
  stream << quad.GetPosition() << "," << quad.GetWidth() << "x" << quad.GetHeight() << endl;
  return stream;
}
////////////////////////////////////////////////////////////////
void
Moose::Math::CQuad::CreateFrom2DCoords( int iX1, int iY1, int iX2, int iY2 )
{
	int xMin, yMin, xMax, yMax;
	if ( iX1 < iX2 ) { xMin = iX1; xMax = iX2; }
	else						 { xMin = iX2; xMax = iX1; }
	if ( iY1 < iY2 ) { yMin = iY1; yMax = iY2; }
	else						 { yMin = iY2; yMax = iY1; }

	CVector2<float> vMin( xMin, yMin );
	CVector2<float> vMax( xMax, yMax );
	CVector2<float> vCenter = vMin + (vMax * 0.5f);
	CVector2<float> vDim     = vMax - vMin;
	SetPosition( vCenter[0], vCenter[1], 0.0f );
	SetWidth(  vDim[0] );
	SetHeight( vDim[1] );
}
////////////////////////////////////////////////////////////////
