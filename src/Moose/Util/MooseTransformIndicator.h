#ifndef __MooseTransformIndicator_h__
#define __MooseTransformIndicator_h__
////////////////////////////////////////////////////////////////////////////////
#include "MooseRenderableModelShared.h"
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Util
  {
    class CTransformIndicator : public Moose::Graphics::CRenderableModelShared
    {
    public:
      CTransformIndicator();
      void Render( Moose::Graphics::COglRenderer & r );
    };
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
