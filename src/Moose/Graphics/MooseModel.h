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

#ifndef __MooseModel_h__
#define __MooseModel_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseResourceManager.h"
#include "MooseVertexDescriptor.h"
#include "MooseIndexArray.h"
#include "MooseTexture.h"
#include "MooseDefaultEntities.h"
#include "MooseRenderable.h"
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
  	////////////////////
    /// class for 3D models with texture and vertex data.
    class MOOSE_API CModel
    {
    protected:
      /// Handle to texture coordinates.
  	  Moose::Default::VERTEX_HANDLE			      m_aTextureCoordinateHandles[TEXTURE_HANDLE_COUNT];
      /// Handle to vertices. (position data)
  	  Moose::Default::VERTEX_HANDLE			      m_VertexDescriptorHandle;
      /// Handle to vertex normals.
  	  Moose::Default::VERTEX_HANDLE			      m_VertexNormalHandle;
      /// Handle to vertex colors.
  	  Moose::Default::VERTEX_HANDLE			      m_VertexColorHandle;
      /// Handle to index arrays for triangle list and/or Triangle strips.
      Moose::Default::INDEX_HANDLE			      m_hIndices;


    public:
      ////////////////////
      /// Constructor.
      CModel();
      ////////////////////
      /// For duplicating properly.
      /// \param model Existing model.
      CModel( const CModel & model );
      ////////////////////
      /// Destructor.
      virtual ~CModel();

      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      Moose::Default::VERTEX_HANDLE     & GetVertexHandle();
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      Moose::Default::VERTEX_HANDLE    & GetNormalHandle();
      ////////////////////
      /// Returns handle to vertex descriptor.
      /// \returns VERTEX_HANDLE.
      Moose::Default::VERTEX_HANDLE    & GetColorHandle();
      ////////////////////
      /// Returns reference to a handle with triangle lists indices .
      /// \returns INDEX_HANDLE.
      Moose::Default::INDEX_HANDLE &	GetIndices();
      ////////////////////
      /// Returns handle to Texture Coordinate vertex descriptor.
      /// \param nId From which texture unit these coordinates are retrieved. By default, the first one (zero).
      /// \returns VERTEX_HANDLE.
      Moose::Default::VERTEX_HANDLE & GetTextureCoordinateHandle( unsigned int nId = 0 );
#ifndef SWIG
      ////////////////////
      /// Debugging output.
      /// \param stream Output stream.
      /// \param renderable Renderable object.
      /// \returns Reference to output stream.
      friend std::ostream & operator<<( std::ostream &stream, const Moose::Graphics::CModel & model );
#endif
    };

  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
