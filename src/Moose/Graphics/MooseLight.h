#ifndef __MooseLight_h__
#define __MooseLight_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseAPI.h"
#include "MooseVector4.h"
#include "MooseOneDirectional.h"
#include "MoosePositional.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    /// Class for lights.
    class MOOSE_API CLight : public virtual Moose::Core::CEnableable,
                             public Moose::Spatial::COneDirectional,
                             public Moose::Spatial::CPositional
    {
    protected:
      /// Diffuse color for this light
      Moose::Math::CVector4<float> m_vDiffuseColor;
      /// Ambient color for this light
      Moose::Math::CVector4<float> m_vAmbientColor;
      /// Specular component for this light
      Moose::Math::CVector4<float> m_vSpecular;
      // Attenuation; v*distance
      float		m_fQuadraticAttenuation;
      // Attenuation; v*distance
      float		m_fLinearAttenuation;
      // Attenuation; v
      float		m_fConstantAttenuation;
      /// Distance to be used when determining closest lights to an object.
      float		m_fBrightness;
      /// Spot angle; accepted values [0.0 .. 90.0],180. Negative indicates a directional light.
      float		m_fSpotAngle;
      /// Spot exponent value.
      float		m_fSpotExponent;

    protected:
      ////////////////////
      /// Sets brightness value.
      /// \param fValue Brightness value.
      void SetBrightness( float fValue );
      ////////////////////
      /// Returns brightness value.
      /// \returns Brightness value.
      float GetBrightness() const;

      ////////////////////
      /// Enables light, sets attenuations off, and default colors.
      CLight()
      {
        SetEnabled(true);
        SetPosition(0.0f,0.0f,0.0f);
        SetDirection(0.0f,0.0f,-1.0f);
        SetQuadraticAttenuation(0.0f);
        SetLinearAttenuation(0.0f);
        SetConstantAttenuation(0.0f);
        SetDiffuseColor( Moose::Math::CVector4<float>(0.5,0.5,0.5,1.0));
        SetAmbientColor( Moose::Math::CVector4<float>(0.1,0.1,0.1,1.0));
        SetSpecularColor( Moose::Math::CVector4<float>(0.9,0.9,0.9,1.0));
        SetBrightness(0.0f);
      }

    public:

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
      const Moose::Math::CVector4<float> & GetDiffuseColor() const;
      ////////////////////
      /// Assigns diffuse color.
      /// \param vColor Color vector.
      void SetDiffuseColor(const Moose::Math::CVector4<float> &vColor);
      ////////////////////
      /// Returns diffuse color.
      /// \returns Reference to color vector.
      const Moose::Math::CVector4<float> & GetAmbientColor() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vColor Color vector.
      void SetAmbientColor(const Moose::Math::CVector4<float> & vColor);
      ////////////////////
      /// Returns specular color.
      /// \returns Reference to color vector.
      const Moose::Math::CVector4<float> & GetSpecularColor() const;
      ////////////////////
      /// Assigns specular color.
      /// \param vColor Color vector.
      void SetSpecularColor(const Moose::Math::CVector4<float> & vColor);
      ///////////////////
      /// Computes distance where light is yet affecting the visual output (attenuation less than 0.001f)
      /// Useful for computing bounding volumes for lights.
      float ComputeAttenuationDistance();
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
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetConstantAttenuation(float fValue )
{
  m_fConstantAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLight::GetConstantAttenuation() const
{
  return m_fConstantAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetLinearAttenuation(float fValue )
{
  m_fLinearAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLight::GetLinearAttenuation() const
{
  return m_fLinearAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetQuadraticAttenuation(float fValue )
{
  m_fQuadraticAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLight::GetQuadraticAttenuation() const
{
  return m_fQuadraticAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetBrightness( float fValue )
{
  m_fBrightness = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLight::GetBrightness() const
{
  return m_fBrightness;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<float> &
Moose::Graphics::CLight::GetDiffuseColor() const
{
  return m_vDiffuseColor;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetDiffuseColor(const Moose::Math::CVector4<float> &vColor)
{
  m_vDiffuseColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<float> &
Moose::Graphics::CLight::GetAmbientColor() const
{
  return m_vAmbientColor;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetAmbientColor(const Moose::Math::CVector4<float> & vColor)
{
  m_vAmbientColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<float> &
Moose::Graphics::CLight::GetSpecularColor() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetSpecularColor(const Moose::Math::CVector4<float> & vColor)
{
  m_vSpecular = vColor;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetSpotAngle( float fAngle )
{
  m_fSpotAngle = fAngle;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLight::GetSpotAngle() const
{
  return m_fSpotAngle;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLight::SetSpotExponent( float fValue )
{
  m_fSpotExponent = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLight::GetSpotExponent() const
{
  return m_fSpotExponent;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLight::ComputeAttenuationDistance()
{
	if ( GetQuadraticAttenuation() >= 0.001f )
	{
		if ( GetLinearAttenuation() >= 0.001f )
		{
			// use 2nd order formula
			float fD = GetLinearAttenuation() * GetLinearAttenuation() - 4.0f * GetQuadraticAttenuation() * (GetConstantAttenuation()-1000.0f);

			if ( fD >= 0.0f )
			{
				return (-GetLinearAttenuation() + sqrtf(fD)) / (GetQuadraticAttenuation()*2.0f);
			} else return -1.0;
		}
		else return sqrtf( (1000.0f- GetConstantAttenuation()) / GetQuadraticAttenuation() );
	}
	else
	{
		if ( GetLinearAttenuation() >= 0.001f )
		{
			// use linear formula
			return (1000.0f - GetConstantAttenuation() ) / GetLinearAttenuation();
		}
		else
		{
			if ( GetConstantAttenuation() < 1000.0f )
				return -1.0f;
			else return 0.0f;
		}
	}
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
