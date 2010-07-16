#ifndef __MooseMaterial_h__
#define __MooseMaterial_h__
/////////////////////////////////////////////////////////////////
#include <MooseVector4.h>
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// A material class which holds the attributes which define
    /// the appearance of an object.
    class MOOSE_API CMaterial 
    {
    protected:
      /// The diffuse color for this material.
      Moose::Math::CVector4<float> m_vDiffuse;
      /// The ambient color for this material.
      Moose::Math::CVector4<float> m_vAmbient;
      /// The specular color for this material.
      Moose::Math::CVector4<float> m_vSpecular;
      /// The emissive color for this material.
      Moose::Math::CVector4<float> m_vEmission;
      /// The shininess factor for this material ( 0.0f - 1.0f ).
      float	    m_fShininess;
    public:
      ////////////////////
      /// Default constructor.
      CMaterial();
      ////////////////////
      /// Destructor.
      ~CMaterial() {}
      ////////////////////
      /// Returns diffuse color.
      /// \returns Diffuse 4-float vector.
      const Moose::Math::CVector4<float> & GetDiffuse() const;
      ////////////////////
      /// Assigns diffuse color.
      /// \param vDiffuse 4-float vector.
      void	    SetDiffuse( const Moose::Math::CVector4<float> & vDiffuse);
      ////////////////////
      /// Returns the Ambient color.
      /// \returns Ambient 4-vector float.
      const Moose::Math::CVector4<float> & GetAmbient() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vAmbient 4-float vector.
      void	    SetAmbient( const Moose::Math::CVector4<float> & vAmbient);
      ////////////////////
      /// Returns specular color.
      /// \returns Specular 4-float vector.
      const Moose::Math::CVector4<float> & GetSpecular() const;
      ////////////////////
      /// Assigns specular color.
      /// \param vSpecular 4-float vector.
      void	    SetSpecular( const Moose::Math::CVector4<float> & vSpecular);
      ////////////////////
      /// Returns emissive color.
      /// \returns Emission 4-float vector.
      const Moose::Math::CVector4<float> & GetEmission() const;
      ////////////////////
      /// Assigns emissive color.
      /// \param vEmission 4-float vector.
      void	    SetEmission( const Moose::Math::CVector4<float> & vEmission);
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
inline const Moose::Math::CVector4<float> &
Moose::Graphics::CMaterial::GetDiffuse() const
{
  return m_vDiffuse;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetDiffuse( const Moose::Math::CVector4<float> & vDiffuse)
{
  m_vDiffuse = vDiffuse;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<float> &
Moose::Graphics::CMaterial::GetAmbient() const
{
  return m_vAmbient;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetAmbient( const Moose::Math::CVector4<float> & vAmbient )
{
  m_vAmbient = vAmbient;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<float> &
Moose::Graphics::CMaterial::GetSpecular() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetSpecular( const Moose::Math::CVector4<float> & vSpecular)
{
  m_vSpecular = vSpecular;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<float> &
Moose::Graphics::CMaterial::GetEmission() const
{
  return m_vEmission;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CMaterial::SetEmission( const Moose::Math::CVector4<float> & vEmission)
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
  if ( fShininess > 1.0f ) 
  {
    m_fShininess = 1.0f;
  } 
  else if ( fShininess < 0.0f )
  {
    m_fShininess = 0.0f;
  }
  else
  {
    m_fShininess = fShininess;
  }
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
