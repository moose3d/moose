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

#include "MooseSpotLight.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CSpotLight::CSpotLight()
{
  SetPosition(0.0f,0.0f,0.0f);
  SetDirection(0.0f,0.0f,-1.0f);
  SetSpotAngle(0.0f);
  SetSpotExponent(0.0f);
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CSpotLight::Render( Moose::Graphics::COglRenderer & renderer )
{

}
///////////////////////////////////////////////////////////////////////////////
