#ifndef __PhoenixMaterial_h__
#define __PhoenixMaterial_h__
/////////////////////////////////////////////////////////////////
#include <PhoenixVector4.h>
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// A material class which holds the attributes which define
    /// the appearance of an object.
    class PHOENIX_API CMaterial 
    {
    protected:
      /// The diffuse color for this material.
      Phoenix::Math::CVector4<float> m_vDiffuse;
      /// The ambient color for this material.
      Phoenix::Math::CVector4<float> m_vAmbient;
      /// The specular color for this material.
      Phoenix::Math::CVector4<float> m_vSpecular;
      /// The emissive color for this material.
      Phoenix::Math::CVector4<float> m_vEmission;
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
      const Phoenix::Math::CVector4<float> & GetDiffuse() const;
      ////////////////////
      /// Assigns diffuse color.
      /// \param vDiffuse 4-float vector.
      void	    SetDiffuse( const Phoenix::Math::CVector4<float> & vDiffuse);
      ////////////////////
      /// Returns the Ambient color.
      /// \returns Ambient 4-vector float.
      const Phoenix::Math::CVector4<float> & GetAmbient() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vAmbient 4-float vector.
      void	    SetAmbient( const Phoenix::Math::CVector4<float> & vAmbient);
      ////////////////////
      /// Returns specular color.
      /// \returns Specular 4-float vector.
      const Phoenix::Math::CVector4<float> & GetSpecular() const;
      ////////////////////
      /// Assigns specular color.
      /// \param vSpecular 4-float vector.
      void	    SetSpecular( const Phoenix::Math::CVector4<float> & vSpecular);
      ////////////////////
      /// Returns emissive color.
      /// \returns Emission 4-float vector.
      const Phoenix::Math::CVector4<float> & GetEmission() const;
      ////////////////////
      /// Assigns emissive color.
      /// \param vEmission 4-float vector.
      void	    SetEmission( const Phoenix::Math::CVector4<float> & vEmission);
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
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline Phoenix::Graphics::CMaterial::CMaterial() : m_vDiffuse(0.8f,0.8f,0.8f,1.0f), m_vAmbient(0.2f,0.2f,0.2f,1.0f), 
						   m_vSpecular(0.0f,0.0f,0.0f,1.0f), m_vEmission(0.0f,0.0f,0.0f,1.0f),
						   m_fShininess(0.0f) 
{
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<float> &
Phoenix::Graphics::CMaterial::GetDiffuse() const
{
  return m_vDiffuse;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CMaterial::SetDiffuse( const Phoenix::Math::CVector4<float> & vDiffuse)
{
  m_vDiffuse = vDiffuse;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<float> &
Phoenix::Graphics::CMaterial::GetAmbient() const
{
  return m_vAmbient;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CMaterial::SetAmbient( const Phoenix::Math::CVector4<float> & vAmbient )
{
  m_vAmbient = vAmbient;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<float> &
Phoenix::Graphics::CMaterial::GetSpecular() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CMaterial::SetSpecular( const Phoenix::Math::CVector4<float> & vSpecular)
{
  m_vSpecular = vSpecular;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<float> &
Phoenix::Graphics::CMaterial::GetEmission() const
{
  return m_vEmission;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CMaterial::SetEmission( const Phoenix::Math::CVector4<float> & vEmission)
{
  m_vEmission = vEmission;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CMaterial::GetShininess() const
{
  return m_fShininess;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CMaterial::SetShininess( float fShininess)
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
