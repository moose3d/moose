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

#ifndef __MooseRenderableModelShared_h__
#define __MooseRenderableModelShared_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseResourceManager.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseTexture.h"
#include "MooseDefaultEntities.h"
#include "MooseRenderable.h"
#include "MooseModel.h"
#include "MooseAPI.h"
#include <vector>
#include <iostream>
#include <utility>
#include <map>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    class COglRenderer;
    ////////////////////
    /// Renderable class for 3D models with texture and vertex data.
    class MOOSE_API CRenderableModelShared : public Moose::Graphics::CRenderable
    {
    protected:
      Moose::Default::MODEL_HANDLE m_hModel;
    public:
      ////////////////////
      /// Constructor.
      CRenderableModelShared();
      ////////////////////
      /// Destructor.
      virtual ~CRenderableModelShared();
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModelShared & renderable );
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( Moose::Graphics::COglRenderer & renderer );
      ////////////////////
      /// \returns model handle.
      Moose::Default::MODEL_HANDLE & GetModelHandle();
    };
    std::ostream & operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModelShared & renderable );

  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
