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

#include "MooseAxisObject.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseModel.h"
#include "MooseBoxRenderable.h"
////////////////////////////////////////////////////////////////////////////////
using namespace Moose::Graphics;
using namespace Moose::Scene;
using namespace Moose::Volume;
////////////////////////////////////////////////////////////////////////////////
Moose::Util::CAxisObject::CAxisObject()
{
  InitializeRenderables(1);
  CBoxRenderable *pBox = new CBoxRenderable(); // this will be dealt later when object it destroyed.
  COrientedBox b;
  b.SetWidth(0.1);
  b.SetHeight(0.1);
  b.SetLength(0.1);
  b.SetPosition(0,0,0);
  pBox->UseBox(b);
  
  AddRenderable( pBox, 0, &GetWorldTransform() );
  
}
////////////////////////////////////////////////////////////////////////////////
