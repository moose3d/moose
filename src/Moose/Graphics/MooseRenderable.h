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

#ifndef __MooseRenderable_h__
#define __MooseRenderable_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseTagged.h"
#include "MooseTransform.h"
#include "MooseRenderState.h"
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
    /// Renderable interface.
    class MOOSE_API CRenderable
    {
    protected:
      Moose::Graphics::CRenderState	      m_RenderState; 	///<! Renderstate.
      Moose::Math::CTransform		      *m_pTransform; 			///<! How renderable should be transformed.
      int 																m_iId; 						///<! Identifier for marking renderables in some manner. Comes handy with CRenderableProperty and CGameObject.
    public:
      ////////////////////
      /// Constructor.
      CRenderable();
      ////////////////////
      /// Destructor.
      virtual ~CRenderable();
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderable & renderable );
      ////////////////////
      /// Returns renderstate.
      /// \returns Current renderstate settings for renderable.
      Moose::Graphics::CRenderState & GetRenderState();
      ////////////////////
      /// Renders this renderable using renderer.
      /// Overwrite this method to specialize rendering.
      /// \param renderer Renderer to be used.
      virtual void Render( COglRenderer & renderer ) = 0;

      Moose::Math::CTransform * GetTransform();
      void	   SetTransform( Moose::Math::CTransform *pTransform );
      ////////////////////
      /// Sets identifier number for this renderable.
      /// \param iId Identifier number to be set.
      void     SetId( int iId );
      ////////////////////
      /// \returns identifier number of this renderable.
      int      GetId() const;
    };
    std::ostream & operator<<( std::ostream &stream, const Moose::Graphics::CRenderable & renderable );
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
