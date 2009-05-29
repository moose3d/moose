#ifndef __PhoenixLight_h__
#define __PhoenixLight_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixSpatial.h"
#include "PhoenixMathGeometry.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    ////////////////////
    /// Class for lights.
    class PHOENIX_API CLightBase : public Phoenix::Core::CEnableable
    {
    protected:
      /// Diffuse color for this light
      Phoenix::Math::CVector4<unsigned char> m_vDiffuseColor;
      /// Ambient color for this light
      Phoenix::Math::CVector4<unsigned char> m_vAmbientColor;
      /// Specular component for this light
      Phoenix::Math::CVector4<unsigned char> m_vSpecular;
      // Attenuation; v*distance
      float		m_fQuadraticAttenuation;
      // Attenuation; v*distance
      float		m_fLinearAttenuation;
      // Attenuation; v
      float		m_fConstantAttenuation;
      /// Distance to be used when determining closest lights to an object.
      float		m_fBrightness;

    protected:
      ////////////////////
      /// Sets brightness value.
      /// \param fValue Brightness value.
      void SetBrightness( float fValue );
      ////////////////////
      /// Returns brightness value.
      /// \returns Brightness value.
      float GetBrightness() const;
    public:
      ////////////////////
      /// Enables light, sets attenuations off, and default colors.
      CLightBase()
      {
			SetEnabled(true);
			SetQuadraticAttenuation(0.0f);
			SetLinearAttenuation(0.0f);
			SetConstantAttenuation(0.0f);
			SetDiffuseColor( CVector4<unsigned char>(125,125,125,255));
			SetAmbientColor( CVector4<unsigned char>(25,25,25,255));
			SetSpecularColor( CVector4<unsigned char>(225,225,225,255));
			SetBrightness(0.0f);
      }
      ////////////////////
      /// Sets constant attenuation.
      /// \param fValue Constant attenuation value.
      void SetConstantAttenuation(float fValue );
      ////////////////////
      /// Returns constant attenuation.
      /// \returns Constant attenuation value.
      float GetConstantAttenuation() const;
      ////////////////////
      /// Sets linear attenuation.
      /// \param fValue Linear attenuation value.
      void SetLinearAttenuation(float fValue );
      ////////////////////
      /// Returns linear attenuation.
      /// \returns Linear attenuation value.
      float GetLinearAttenuation() const;
      ////////////////////
      /// Sets quadratic attenuation.
      /// \param fValue Quadratic attenuation value.
      void SetQuadraticAttenuation(float fValue );
      ////////////////////
      /// Returns quadratic attenuation.
      /// \returns Quadratic attenuation value.
      float GetQuadraticAttenuation() const;
      ////////////////////
      /// Returns diffuse color.
      /// \returns Reference to color vector.
      const Phoenix::Math::CVector4<unsigned char> & GetDiffuseColor() const;
      ////////////////////
      /// Assigns diffuse color.
      /// \param vColor Color vector.
      void SetDiffuseColor(const Phoenix::Math::CVector4<unsigned char> &vColor);
      ////////////////////
      /// Returns diffuse color.
      /// \returns Reference to color vector.
      const Phoenix::Math::CVector4<unsigned char> & GetAmbientColor() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vColor Color vector.
      void SetAmbientColor(const Phoenix::Math::CVector4<unsigned char> & vColor);
      ////////////////////
      /// Returns specular color.
      /// \returns Reference to color vector.
      const Phoenix::Math::CVector4<unsigned char> & GetSpecularColor() const;
      ////////////////////
      /// Assigns specular color.
      /// \param vColor Color vector.
      void SetSpecularColor(const Phoenix::Math::CVector4<unsigned char> & vColor);
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CLightBase::SetConstantAttenuation(float fValue )
{
  m_fConstantAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CLightBase::GetConstantAttenuation() const
{
  return m_fConstantAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CLightBase::SetLinearAttenuation(float fValue )
{
  m_fLinearAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CLightBase::GetLinearAttenuation() const
{
  return m_fLinearAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CLightBase::SetQuadraticAttenuation(float fValue )
{
  m_fQuadraticAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CLightBase::GetQuadraticAttenuation() const
{
  return m_fQuadraticAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CLightBase::SetBrightness( float fValue )
{
  m_fBrightness = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CLightBase::GetBrightness() const
{
  return m_fBrightness;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<unsigned char> &
Phoenix::Graphics::CLightBase::GetDiffuseColor() const
{
  return m_vDiffuseColor;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CLightBase::SetDiffuseColor(const Phoenix::Math::CVector4<unsigned char> &vColor)
{
  m_vDiffuseColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<unsigned char> &
Phoenix::Graphics::CLightBase::GetAmbientColor() const
{
  return m_vAmbientColor;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CLightBase::SetAmbientColor(const Phoenix::Math::CVector4<unsigned char> & vColor)
{
  m_vAmbientColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<unsigned char> &
Phoenix::Graphics::CLightBase::GetSpecularColor() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CLightBase::SetSpecularColor(const Phoenix::Math::CVector4<unsigned char> & vColor)
{
  m_vSpecular = vColor;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
