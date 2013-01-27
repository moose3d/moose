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

#ifndef __MooseAmbientLight_h__
#define __MooseAmbientLight_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseAPI.h"
#include "MooseLight.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    class COglRenderer;
	////////////////////
    /// Class for global ambient light.
	class MOOSE_API CAmbientLight : public Moose::Graphics::CLight
    {
    protected:
      /// Global ambient color
      Moose::Math::CVector4<unsigned char> m_vColor;
    public:
      ////////////////////
      /// Sets default ambient value (0.2,0.2,0.2,1.0)
      CAmbientLight() : m_vColor(51,51,51,255 ) { }
      ////////////////////
      /// Sets ambient value according to parameters.
      /// \param r ambient red component 0-255
      /// \param g ambient green component 0-255
      /// \param b ambient blue component 0-255
      /// \param a ambient alpha component 0-255
      CAmbientLight(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : m_vColor(r,g,b,a) { }
      ////////////////////
      /// Sets ambient value according to parameter.
      /// \param vColor Color values as 4-vector.
      CAmbientLight(const Moose::Math::CVector4<unsigned char> & vColor ) : m_vColor(vColor) { }
      ////////////////////
      /// Returns ambient color.
      /// \returns Reference to color vector.
      const Moose::Math::CVector4<unsigned char> & GetColor() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vColor Color vector.
      void SetColor(const Moose::Math::CVector4<unsigned char> &vColor);
      ////////////////////
      /// Renders this light using renderer.
      /// \param renderer Renderer to be used in rendering.
      void Render( Moose::Graphics::COglRenderer & renderer );
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<unsigned char> &
Moose::Graphics::CAmbientLight::GetColor() const
{
  return m_vColor;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CAmbientLight::SetColor(const Moose::Math::CVector4<unsigned char> &vColor)
{
  m_vColor = vColor;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
