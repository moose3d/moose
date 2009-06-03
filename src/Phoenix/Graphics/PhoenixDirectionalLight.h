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
										  public Phoenix::Spatial::COneDirectional,
										  public Phoenix::Graphics::CRenderable

    {
    public:
      ////////////////////
      /// Default constructor.
      CDirectionalLight()
      {
    	  SetDirection(0.0f,0.0f,-1.0f);
      }
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
