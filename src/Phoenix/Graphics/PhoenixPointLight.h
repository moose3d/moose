#ifndef __PhoenixPointLight_h__
#define __PhoenixPointLight_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixPositional.h"
#include "PhoenixLight.h"
#include "PhoenixAPI.h"
#include "PhoenixRenderable.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    ////////////////////
    /// Class for lights.
    class PHOENIX_API CPointLight : public Phoenix::Graphics::CLightBase,
									public Phoenix::Spatial::CPositional
    {
    public:
      ////////////////////
      /// Positions light to origo.
      CPointLight();

      void Render( Phoenix::Graphics::COglRenderer & renderer );
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
