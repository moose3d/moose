#ifndef __MooseDirectionalLight_h__
#define __MooseDirectionalLight_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
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
    /// Class for lights.
    class MOOSE_API CDirectionalLight : public Moose::Graphics::CLight
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
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
