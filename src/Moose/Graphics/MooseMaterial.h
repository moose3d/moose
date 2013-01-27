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

#ifndef __MooseMaterial_h__
#define __MooseMaterial_h__
/////////////////////////////////////////////////////////////////
#include <MooseVector4.h>
#include <MooseVector3.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    // Readability tricks.
    typedef Moose::Math::CVector4<float> CColor4f;
    typedef Moose::Math::CVector3<float> CColor3f;
    typedef Moose::Math::CVector4<unsigned char> CColor4ub;
    typedef Moose::Math::CVector3<unsigned char> CColor3ub;
    /////////////////////////////////////////////////////////////////
    /// A material class which holds the attributes which define
    /// the appearance of an object.
    class MOOSE_API CMaterial 
    {
    protected:
      /// The diffuse color for this material.
      CColor4f m_vDiffuse;
      /// The ambient color for this material.
      CColor4f m_vAmbient;
      /// The specular color for this material.
      CColor4f m_vSpecular;
      /// The emissive color for this material.
      CColor4f m_vEmission;
      /// The shininess factor for this material ( 1.0f - 127.0f ).
      float	    m_fShininess;
    public:
      ////////////////////
      /// Default constructor.
      CMaterial();
      ////////////////////
      /// Destructor.
      virtual ~CMaterial() {}
      ////////////////////
      /// Returns diffuse color.
      /// \returns Diffuse 4-float vector.
      const CColor4f & GetDiffuse() const;
      ////////////////////
      /// Assigns diffuse color.
      /// \param vDiffuse 4-float vector.
      void	    SetDiffuse( const CColor4f & vDiffuse);
      ////////////////////
      /// Returns the Ambient color.
      /// \returns Ambient 4-vector float.
      const CColor4f & GetAmbient() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vAmbient 4-float vector.
      void	    SetAmbient( const CColor4f & vAmbient);
      ////////////////////
      /// Returns specular color.
      /// \returns Specular 4-float vector.
      const CColor4f & GetSpecular() const;
      ////////////////////
      /// Assigns specular color.
      /// \param vSpecular 4-float vector.
      void	    SetSpecular( const CColor4f & vSpecular);
      ////////////////////
      /// Returns emissive color.
      /// \returns Emission 4-float vector.
      const CColor4f & GetEmission() const;
      ////////////////////
      /// Assigns emissive color.
      /// \param vEmission 4-float vector.
      void	    SetEmission( const CColor4f & vEmission);
      ////////////////////
      /// Returns shininess factor.
      /// \returns Shininess value, range 0.0-1.0f
      float	    GetShininess() const;
      ////////////////////
      /// Assigns shininess factor.
      /// \param fShininess Shininess value, 0.0-1.0f.
      void	    SetShininess( float fShininess);
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
inline Moose::Graphics::CMaterial::CMaterial() : m_vDiffuse(0.8f,0.8f,0.8f,1.0f), m_vAmbient(0.2f,0.2f,0.2f,1.0f), 
						   m_vSpecular(0.0f,0.0f,0.0f,1.0f), m_vEmission(0.0f,0.0f,0.0f,1.0f),
						   m_fShininess(0.0f) 
{
}
/////////////////////////////////////////////////////////////////
inline const Moose::Graphics::CColor4f &
Moose::Graphics::CMaterial::GetDiffuse() const
{
  return m_vDiffuse;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetDiffuse( const Moose::Graphics::CColor4f & vDiffuse)
{
  m_vDiffuse = vDiffuse;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Graphics::CColor4f &
Moose::Graphics::CMaterial::GetAmbient() const
{
  return m_vAmbient;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetAmbient( const Moose::Graphics::CColor4f & vAmbient )
{
  m_vAmbient = vAmbient;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Graphics::CColor4f &
Moose::Graphics::CMaterial::GetSpecular() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetSpecular( const Moose::Graphics::CColor4f & vSpecular)
{
  m_vSpecular = vSpecular;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Graphics::CColor4f &
Moose::Graphics::CMaterial::GetEmission() const
{
  return m_vEmission;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetEmission( const Moose::Graphics::CColor4f & vEmission)
{
  m_vEmission = vEmission;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CMaterial::GetShininess() const
{
  return m_fShininess;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetShininess( float fShininess)
{
  if ( fShininess > 127.0f ) 
  {
    m_fShininess = 127.0f;
  } 
  else if ( fShininess < 1.0f )
  {
    m_fShininess = 1.0f;
  }
  else
  {
    m_fShininess = fShininess;
  }
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
