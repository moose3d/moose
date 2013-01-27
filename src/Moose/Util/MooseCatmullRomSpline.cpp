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

#include "MooseCatmullRomSpline.h"
#include "MooseLogger.h"
#include "MooseDefaultEntities.h"
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////
using namespace Moose::Math;
////////////////////////////////////////////////////////////////////////////////
Moose::Math::CVector3<float> 
Moose::Util::CCatmullRomSpline::GetPosition( float t ) const
{
  
  int seg = std::max(0.0f,ceilf(t - 1.0f));
  float fT = t - (float)((int)t);
  CVector3<float> vPos[4] = { this->at(seg), this->at(1+seg), this->at(2+seg), this->at(3+seg) };
  
  return 0.5f * (  (2.0f * vPos[1] ) +
                 ( -vPos[0] + vPos[2]  ) * fT +
                 ( 2.0f*vPos[0] - 5.0f * vPos[1] + 4.0f* vPos[2] -  vPos[3]) * fT*fT +
                 (     -vPos[0] + 3.0f * vPos[1] - 3.0f* vPos[2] +  vPos[3]) * fT*fT*fT);
}
////////////////////////////////////////////////////////////////////////////////
