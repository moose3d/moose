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

#include "MooseDirectionalLightObject.h"
#include "MooseDirectionalLight.h"
#include <cassert>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Scene;
using namespace Moose::Graphics;
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CDirectionalLightObject::CDirectionalLightObject()
{
	InitializeRenderables(1);
	SetTag( LIGHT_TAG );
	SetEnabled(true);
	//GetBoundingSphere().SetRadius( g_Game->GetScene()->GetSpatialGraph().GetWorldHalfSize() * 1.732f);
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CDirectionalLightObject::~CDirectionalLightObject()
{
  
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CDirectionalLightObject::Update( float fSeconds )
{
  SetDirection(GetWorldTransform().GetForwardVector());
}
///////////////////////////////////////////////////////////////////////////////
