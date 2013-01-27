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

#ifndef __MooseSpotLight_h__
#define __MooseSpotLight_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MoosePositional.h"
#include "MooseOneDirectional.h"
#include "MooseLight.h"
#include "MooseAPI.h"
#include "MooseRenderable.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    /// Class for spotlights.
    class MOOSE_API CSpotLight : public Moose::Graphics::CLight
    {
    public:
      ////////////////////
      /// Initialize default params, positioned at origo, points towards negative z axis, spot angle and exponent are zero.
      CSpotLight();
      
      ////////////////////
      /// Renders this object using renderer.
      /// \param renderer Renderer to be used in rendering.
      void Render( COglRenderer & renderer );
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////

#endif
/////////////////////////////////////////////////////////////////
