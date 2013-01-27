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

#ifndef __MooseRocketRenderInterface_h__
#define __MooseRocketRenderInterface_h__
////////////////////////////////////////////////////////////////////////////////
#include <MooseAPI.h>
#include <MooseRenderable.h>
#include <Rocket/Core.h>
#include <Rocket/Core/RenderInterface.h>
#include <Rocket/Core/Platform.h>
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  { 
    ////////////////////
    /// Rocket render interface for Moose.
    class MOOSE_API CRocketRenderInterface : public Rocket::Core::RenderInterface
    {
    protected:
      Moose::Graphics::CRenderable *m_pRenderable; ///< RocketRenderable from scene.
    public:
      CRocketRenderInterface( );
      void SetRenderable( Moose::Graphics::CRenderable * pR );
      /// Called by Rocket when it wants to render geometry that it does not wish to optimise.
      void RenderGeometry(Rocket::Core::Vertex* vertices, 
                          int num_vertices, 
                          int* indices, 
                          int num_indices, 
                          Rocket::Core::TextureHandle texture, 
                          const Rocket::Core::Vector2f& translation);

      /// Called by Rocket when it wants to compile geometry it 
      /// believes will be static for the forseeable future.
      Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, 
                                                           int num_vertices, 
                                                           int* indices, 
                                                           int num_indices, 
                                                           Rocket::Core::TextureHandle texture);

      /// Called by Rocket when it wants to render application-compiled geometry.
      void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, 
                                  const Rocket::Core::Vector2f& translation);
      /// Called by Rocket when it wants to release application-
      /// compiled geometry.
      void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

      /// Called by Rocket when it wants to enable or disable 
      /// scissoring to clip content.
      void EnableScissorRegion(bool enable);
      /// Called by Rocket when it wants to change the scissor region.
      void SetScissorRegion(int x, int y, int width, int height);

      /// Called by Rocket when a texture is required by the library.
      bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, 
                       Rocket::Core::Vector2i& texture_dimensions, 
                       const Rocket::Core::String& source);
      /// Called by Rocket when a texture is required to be built 
      /// from an internally-generated sequence of pixels.
      bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, 
                           const Rocket::Core::byte* source, 
                           const Rocket::Core::Vector2i& source_dimensions);
      /// Called by Rocket when a loaded texture is no longer required.
      void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);
    }; /// CRocketRenderInterface
  } /// Graphics
} /// Moose

#endif
