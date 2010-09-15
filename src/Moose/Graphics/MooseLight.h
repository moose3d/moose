#ifndef __MooseLight_h__
#define __MooseLight_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseAPI.h"
#include "MooseRenderable.h"
#include "MooseVector4.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    /// Class for lights.
    class MOOSE_API CLightBase : public Moose::Core::CEnableable,
								   public Moose::Graphics::CRenderable
    {
    protected:
      /// Diffuse color for this light
      Moose::Math::CVector4<unsigned char> m_vDiffuseColor;
      /// Ambient color for this light
      Moose::Math::CVector4<unsigned char> m_vAmbientColor;
      /// Specular component for this light
      Moose::Math::CVector4<unsigned char> m_vSpecular;
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

      ////////////////////
      /// Enables light, sets attenuations off, and default colors.
      CLightBase()
      {
			SetEnabled(true);
			SetQuadraticAttenuation(0.0f);
			SetLinearAttenuation(0.0f);
			SetConstantAttenuation(0.0f);
			SetDiffuseColor( Moose::Math::CVector4<unsigned char>(125,125,125,255));
			SetAmbientColor( Moose::Math::CVector4<unsigned char>(25,25,25,255));
			SetSpecularColor( Moose::Math::CVector4<unsigned char>(225,225,225,255));
			SetBrightness(0.0f);
			GetRenderState().IsLightSource() = true;
            GetRenderState().SetRenderLayer(500);
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
      const Moose::Math::CVector4<unsigned char> & GetDiffuseColor() const;
      ////////////////////
      /// Assigns diffuse color.
      /// \param vColor Color vector.
      void SetDiffuseColor(const Moose::Math::CVector4<unsigned char> &vColor);
      ////////////////////
      /// Returns diffuse color.
      /// \returns Reference to color vector.
      const Moose::Math::CVector4<unsigned char> & GetAmbientColor() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vColor Color vector.
      void SetAmbientColor(const Moose::Math::CVector4<unsigned char> & vColor);
      ////////////////////
      /// Returns specular color.
      /// \returns Reference to color vector.
      const Moose::Math::CVector4<unsigned char> & GetSpecularColor() const;
      ////////////////////
      /// Assigns specular color.
      /// \param vColor Color vector.
      void SetSpecularColor(const Moose::Math::CVector4<unsigned char> & vColor);
      ///////////////////
      /// Computes distance where light is yet affecting the visual output (attenuation less than 0.001f)
      /// Useful for computing bounding volumes for lights.
      float ComputeAttenuationDistance();
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLightBase::SetConstantAttenuation(float fValue )
{
  m_fConstantAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLightBase::GetConstantAttenuation() const
{
  return m_fConstantAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLightBase::SetLinearAttenuation(float fValue )
{
  m_fLinearAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLightBase::GetLinearAttenuation() const
{
  return m_fLinearAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLightBase::SetQuadraticAttenuation(float fValue )
{
  m_fQuadraticAttenuation = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLightBase::GetQuadraticAttenuation() const
{
  return m_fQuadraticAttenuation;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLightBase::SetBrightness( float fValue )
{
  m_fBrightness = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLightBase::GetBrightness() const
{
  return m_fBrightness;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<unsigned char> &
Moose::Graphics::CLightBase::GetDiffuseColor() const
{
  return m_vDiffuseColor;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLightBase::SetDiffuseColor(const Moose::Math::CVector4<unsigned char> &vColor)
{
  m_vDiffuseColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<unsigned char> &
Moose::Graphics::CLightBase::GetAmbientColor() const
{
  return m_vAmbientColor;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLightBase::SetAmbientColor(const Moose::Math::CVector4<unsigned char> & vColor)
{
  m_vAmbientColor = vColor;
}
/////////////////////////////////////////////////////////////////
inline const Moose::Math::CVector4<unsigned char> &
Moose::Graphics::CLightBase::GetSpecularColor() const
{
  return m_vSpecular;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CLightBase::SetSpecularColor(const Moose::Math::CVector4<unsigned char> & vColor)
{
  m_vSpecular = vColor;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CLightBase::ComputeAttenuationDistance()
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
