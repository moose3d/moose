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

#ifndef __MooseDirectionalLightobject_h__
#define __MooseDirectionalLightobject_h__
///////////////////////////////////////////////////////////////////////////////
#include <MooseGameObject.h>
#include <MooseDirectionalLight.h>
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Scene
  {
    ////////////////
    /// Directional light to be inserted into a scene.
    class CDirectionalLightObject : public Moose::Scene::CGameObject,
                                    public Moose::Graphics::CDirectionalLight
    {
    public:

      CDirectionalLightObject( );
      virtual ~CDirectionalLightObject();
      void Update( float fSeconds );
      bool Intersects( const Moose::Volume::CSphere & sphere ) const { return true; }
      bool Intersects( const Moose::Graphics::CFrustum & frustum ) const { return true; }
      bool Intersects( const Moose::Volume::COrientedBox & box ) const { return true; }
      bool Intersects( const Moose::Math::CVector3<float> & vPoint ) const  { return true; }
      bool Intersects( const Moose::Math::CRay & ray, float *pfDistance = NULL ) const  { return true; }
      bool Intersects( const Moose::Collision::ICollider & collider ) const  { return true; }

    };
  }
}

	///////////////////////////////////////////////////////////////////////////////
#endif
