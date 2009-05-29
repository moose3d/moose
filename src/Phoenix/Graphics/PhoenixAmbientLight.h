#ifndef __PhoenixAmbientLight_h__
#define __PhoenixAmbientLight_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    ////////////////////
    /// Class for global ambient light.
	/// Differs from other lights per se, since this is only global ambient value. Nothing more.
    class PHOENIX_API CAmbientLight : public Phoenix::Core::CEnableable
    {
    protected:
      /// Global ambient color
      Phoenix::Math::CVector4<unsigned char> m_vColor;
    public:
      ////////////////////
      /// Sets default ambient value (0.2,0.2,0.2,1.0)
      CAmbientLight() : m_vColor(51,51,51,255 ) { }
      ////////////////////
      /// Returns ambient color.
      /// \returns Reference to color vector.
      const Phoenix::Math::CVector4<unsigned char> & GetColor() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vColor Color vector.
      void SetColor(const Phoenix::Math::CVector4<unsigned char> &vColor);
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
inline const Phoenix::Math::CVector4<unsigned char> &
Phoenix::Graphics::CAmbientLight::GetColor() const
{
  return m_vColor;
}
/////////////////////////////////////////////////////////////////
inline void
Phoenix::Graphics::CAmbientLight::SetColor(const Phoenix::Math::CVector4<unsigned char> &vColor)
{
  m_vColor = vColor;
}
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
