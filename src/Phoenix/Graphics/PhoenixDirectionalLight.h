#ifndef __PhoenixDirectionalLight_h__
#define __PhoenixDirectionalLight_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixSpatial.h"
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
    class PHOENIX_API CDirectionalLight : public Phoenix::Graphics::CLightBase,
										  public Phoenix::Spatial::COneDirectional

    {
    public:
      ////////////////////
      /// Default constructor.
      CDirectionalLight();
      ///////////////////
      /// Renders this light using renderer.
      /// \param renderer Renderer to be used.
      void Render( COglRenderer & renderer );
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
