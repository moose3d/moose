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
    enum LIGHT_TYPE 
    {
      SPOTLIGHT = 0,
      DIRECTIONAL,
      POINTLIGHT
    };
    ////////////////////
    /// Class for lights.
    class PHOENIX_API CLight : public Phoenix::Spatial::CPositional,
                   public Phoenix::Spatial::COneDirectional,
                   public Phoenix::Core::CEnableable
    {
    protected:
      /// Light type.
      LIGHT_TYPE   m_tLightType;
      /// Diffuse color for this light
      Phoenix::Math::CVector4<unsigned char> m_vDiffuseColor;
      /// Ambient color for this light
      Phoenix::Math::CVector4<unsigned char> m_vAmbientColor;
      /// Specular component for this light
      Phoenix::Math::CVector4<unsigned char> m_vSpecular;
      /// Spot angle; accepted values [0.0 .. 90.0],180
      float		m_fSpotAngle;
      /// Spot exponent value.
      float		m_fSpotExponent;
      // Attenuation; v*distance²
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
      /// Default constructor.
      CLight()
      {
	SetPosition(0.0f,0.0f,0.0f);
	SetDirection(0.0f,0.0f,-1.0f);
	SetEnabled(1);
	SetSpotAngle(0.0f);
	SetSpotExponent(0.0f);
	SetType(DIRECTIONAL);
	SetQuadraticAttenuation(0.0f);
	SetLinearAttenuation(0.0f);
	SetConstantAttenuation(0.0f);
	SetDiffuseColor( CVector4<unsigned char>(125,125,125,255));
	SetAmbientColor( CVector4<unsigned char>(25,25,25,255));
	SetSpecularColor( CVector4<unsigned char>(225,225,225,255));
	SetBrightness(0.0f);
      }
      ////////////////////
      /// Sets spot angle.
      /// \param fAngle Angle in degrees.
      void SetSpotAngle( float fAngle );
      ////////////////////
      /// Returns spot angle.
      /// \returns  Angle in degrees.
      float GetSpotAngle() const;
      ////////////////////
      /// Sets spot angle.
      /// \param fValue Exponent value.
      void SetSpotExponent( float fValue );
      ////////////////////
      /// Returns spot exponent.
      /// \returns  Exponent value.
      float GetSpotExponent() const;
      ////////////////////
      /// Sets light type.
      /// \param tType LIGHT_TYPE.
      void SetType( LIGHT_TYPE tType );
      ////////////////////
      /// Returns light type.
      /// \returns One of LIGHT_TYPEs.
      LIGHT_TYPE GetType() const;
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
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetSpotAngle( float fAngle )
{
  m_fSpotAngle    = fAngle;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Graphics::CLight::GetSpotAngle() const
{
  return m_fSpotAngle;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetSpotExponent( float fValue )
{
  m_fSpotExponent    = fValue;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Graphics::CLight::GetSpotExponent() const
{
  return m_fSpotExponent;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetType( LIGHT_TYPE tType )
{
  m_tLightType = tType;
}
/////////////////////////////////////////////////////////////////
inline Phoenix::Graphics::LIGHT_TYPE 
Phoenix::Graphics::CLight::GetType() const
{
  return m_tLightType;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetConstantAttenuation(float fValue )
{
  m_fConstantAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Graphics::CLight::GetConstantAttenuation() const
{
  return m_fConstantAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetLinearAttenuation(float fValue )
{
  m_fLinearAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Graphics::CLight::GetLinearAttenuation() const
{
  return m_fLinearAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetQuadraticAttenuation(float fValue )
{
  m_fQuadraticAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float 
Phoenix::Graphics::CLight::GetQuadraticAttenuation() const
{
  return m_fQuadraticAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetBrightness( float fValue )
{
  m_fBrightness = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CLight::GetBrightness() const
{
  return m_fBrightness;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<unsigned char> & 
Phoenix::Graphics::CLight::GetDiffuseColor() const
{
  return m_vDiffuseColor;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetDiffuseColor(const Phoenix::Math::CVector4<unsigned char> &vColor)
{
  m_vDiffuseColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<unsigned char> & 
Phoenix::Graphics::CLight::GetAmbientColor() const
{
  return m_vAmbientColor;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetAmbientColor(const Phoenix::Math::CVector4<unsigned char> & vColor)
{
  m_vAmbientColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<unsigned char> & 
Phoenix::Graphics::CLight::GetSpecularColor() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
inline void 
Phoenix::Graphics::CLight::SetSpecularColor(const Phoenix::Math::CVector4<unsigned char> & vColor) 
{
  m_vSpecular = vColor;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
