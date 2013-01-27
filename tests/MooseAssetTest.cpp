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

#include <UnitTest++/UnitTest++.h>
#include <MooseCore.h>
#include <MoosePointLight.h>
#include <MooseAssetBundle.h>
#include <MooseExceptions.h>
#include <string>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Core;
using namespace Moose::Graphics;
using namespace Moose::Exceptions;
///////////////////////////////////////////////////////////////////////////////
TEST( AssetBundle_throw )
{  
  CHECK_THROW( CAssetBundle::GetInstance()->GetAssetPath("hihihi"), CNoSuchAssetException);
}
///////////////////////////////////////////////////////////////////////////////
TEST( AssetBundle_found )
{
  CHECK_EQUAL( CAssetBundle::GetInstance()->GetAssetPath("Shaders/skybox.vert"), std::string("/usr/share/Moose/Assets/Shaders/skybox.vert"));
}
////////////////////////////////////////////////////////////////////////////////
