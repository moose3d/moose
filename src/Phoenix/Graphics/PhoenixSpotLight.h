#ifndef __PhoenixSpotLight_h__
#define __PhoenixSpotLight_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixPositional.h"
#include "PhoenixOneDirectional.h"
#include "PhoenixLight.h"
#include "PhoenixAPI.h"
#include "PhoenixRenderable.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    ////////////////////
    /// Class for spotlights.
    class PHOENIX_API CSpotLight : public Phoenix::Graphics::CLightBase,
								   public Phoenix::Spatial::CPositional,
								   public Phoenix::Spatial::COneDirectional
    {
    protected:
      /// Spot angle; accepted values [0.0 .. 90.0],180
      float		m_fSpotAngle;
      /// Spot exponent value.
      float		m_fSpotExponent;
    public:
      ////////////////////
      /// Initialize default params, positioned at origo, points towards negative z axis, spot angle and exponent are zero.
      CSpotLight();
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
      /// Renders this object using renderer.
      /// \param renderer Renderer to be used in rendering.
      void Render( COglRenderer & renderer );
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CSpotLight::SetSpotAngle( float fAngle )
{
  m_fSpotAngle = fAngle;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CSpotLight::GetSpotAngle() const
{
  return m_fSpotAngle;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CSpotLight::SetSpotExponent( float fValue )
{
  m_fSpotExponent = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Phoenix::Graphics::CSpotLight::GetSpotExponent() const
{
  return m_fSpotExponent;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
