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

#include "MoosePointLight.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CPointLight::CPointLight()
{
  SetPosition(0.0f,0.0f,0.0f);
  SetSpotAngle(180.0f);
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CPointLight::Render( Moose::Graphics::COglRenderer & renderer )
{

}
///////////////////////////////////////////////////////////////////////////////
