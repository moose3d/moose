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
    class MOOSE_API CSpotLight : public Moose::Graphics::CLight
    {
    public:
      ////////////////////
      /// Initialize default params, positioned at origo, points towards negative z axis, spot angle and exponent are zero.
      CSpotLight();
      
      ////////////////////
      /// Renders this object using renderer.
      /// \param renderer Renderer to be used in rendering.
      void Render( COglRenderer & renderer );
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////

#endif
/////////////////////////////////////////////////////////////////
