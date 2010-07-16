#ifndef __MoosePointLight_h__
#define __MoosePointLight_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MoosePositional.h"
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
    class MOOSE_API CPointLight : public Moose::Graphics::CLightBase,
									public Moose::Spatial::CPositional
    {
    public:
      ////////////////////
      /// Positions light to origo.
      CPointLight();

      void Render( Moose::Graphics::COglRenderer & renderer );
    };
  }; // namespace Graphics
}; // namespace Moose
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
