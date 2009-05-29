#ifndef __PhoenixPointLight_h__
#define __PhoenixPointLight_h__
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
    class PHOENIX_API CPointLight : public Phoenix::Graphics::CLightBase,
											  public Phoenix::Spatial::CPositional
    {
    public:
      ////////////////////
      /// Positions light to origo.
      CPointLight()
      {
    	  SetPosition(0.0f,0.0f,0.0f);
	  }
    };
  }; // namespace Graphics
}; // namespace Phoenix
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
