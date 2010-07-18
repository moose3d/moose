#ifndef __MooseGrid_h__
#define __MooseGrid_h__
#include <MooseRenderableModelShared.h>
namespace Moose
{
  namespace Util
  {
    class CGrid : public Moose::Graphics::CRenderableModelShared
    {
    public:
      CGrid();
      virtual ~CGrid();
    };
  }
}

#endif
