#ifndef __MooseSpotLight_h__
#define __MooseSpotLight_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MoosePositional.h"
#include "MooseOneDirectional.h"
#include "MooseLight.h"
#include "MooseAPI.h"
#include "MooseRenderable.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    /// Class for spotlights.
    class MOOSE_API CSpotLight : public Moose::Graphics::CLightBase,
								   public Moose::Spatial::CPositional,
								   public Moose::Spatial::COneDirectional
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
}; // namespace Moose
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CSpotLight::SetSpotAngle( float fAngle )
{
  m_fSpotAngle = fAngle;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CSpotLight::GetSpotAngle() const
{
  return m_fSpotAngle;
}
/////////////////////////////////////////////////////////////////
inline void
Moose::Graphics::CSpotLight::SetSpotExponent( float fValue )
{
  m_fSpotExponent = fValue;
}
/////////////////////////////////////////////////////////////////
inline float
Moose::Graphics::CSpotLight::GetSpotExponent() const
{
  return m_fSpotExponent;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
