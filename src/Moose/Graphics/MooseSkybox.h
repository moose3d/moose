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

#ifndef __MooseSkybox_h__
#define __MooseSkybox_h__
/////////////////////////////////////////////////////////////////
#include "MooseDefaultEntities.h"
#include "MooseRenderableModelShared.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
#define MOOSE_SKYBOX_TEXCOORDS    "MOOSE_SKYBOX_TEXCOORDS"
#define MOOSE_SKYBOX_VERTICES     "MOOSE_SKYBOX_VERTICES"
#define MOOSE_SKYBOX_INDICES      "MOOSE_SKYBOX_INDICES"
#define MOOSE_SKYBOX_MODEL        "MOOSE_SKYBOX_MODEL"
#define MOOSE_SKYBOX_SHADER       "MOOSE_SKYBOX_SHADER"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
  	class COglRenderer;
    /////////////////////////////////////////////////////////////////
    /// Skybox class.
    class MOOSE_API CSkybox : public Moose::Graphics::CRenderableModelShared
    {
    public:
      ////////////////////
      /// Constructor.
      CSkybox();
      ////////////////////
      /// Destructor.
      ~CSkybox();
      void Render( Moose::Graphics::COglRenderer & renderer );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
