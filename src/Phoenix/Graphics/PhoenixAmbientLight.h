#ifndef __PhoenixAmbientLight_h__
#define __PhoenixAmbientLight_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixAPI.h"
#include "PhoenixRenderable.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
	////////////////////
    /// Class for global ambient light.
	/// Differs from other lights per se, since this is only global ambient value. Nothing more.
    class PHOENIX_API CAmbientLight : public Phoenix::Core::CEnableable,
									  public Phoenix::Graphics::CRenderable
    {
    protected:
      /// Global ambient color
      Phoenix::Math::CVector4<unsigned char> m_vColor;
    public:
      ////////////////////
      /// Sets default ambient value (0.2,0.2,0.2,1.0)
      CAmbientLight() : m_vColor(51,51,51,255 ) { }
      ////////////////////
      /// Sets ambient value according to parameters.
      /// \param r ambient red component 0-255
      /// \param g ambient green component 0-255
      /// \param b ambient blue component 0-255
      /// \param a ambient alpha component 0-255
      CAmbientLight(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : m_vColor(r,g,b,a) { }
      ////////////////////
      /// Sets ambient value according to parameter.
      /// \param vColor Color values as 4-vector.
      CAmbientLight(const Phoenix::Math::CVector4<unsigned char> & vColor ) : m_vColor(vColor) { }
      ////////////////////
      /// Returns ambient color.
      /// \returns Reference to color vector.
      const Phoenix::Math::CVector4<unsigned char> & GetColor() const;
      ////////////////////
      /// Assigns ambient color.
      /// \param vColor Color vector.
      void SetColor(const Phoenix::Math::CVector4<unsigned char> &vColor);
      ////////////////////
      /// Renders this light using renderer.
      /// \param renderer Renderer to be used in rendering.
      void Render( COglRenderer & renderer );
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
