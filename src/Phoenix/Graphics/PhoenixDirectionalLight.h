#ifndef __PhoenixDirectionalLight_h__
#define __PhoenixDirectionalLight_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixSpatial.h"
#include "PhoenixLight.h"
#include "PhoenixAPI.h"
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
      CDirectionalLight()
      {
    	  SetDirection(0.0f,0.0f,-1.0f);
      }
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
