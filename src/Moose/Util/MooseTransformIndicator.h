#ifndef __MooseTransformIndicator_h__
#define __MooseTransformIndicator_h__
////////////////////////////////////////////////////////////////////////////////
#include "MooseRenderableModelShared.h"
#include "MooseTransformGraph.h"
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Util
  {
    class CTransformIndicator : public Moose::Graphics::CRenderableModelShared
    {
    protected:
      Moose::Scene::CTransformable *m_pTarget;
    public:
      CTransformIndicator();
      void SetTarget( Moose::Scene::CTransformable *pTransformable );
      void Render( Moose::Graphics::COglRenderer & r );
    };
  }
}
////////////////////////////////////////////////////////////////////////////////
#endif
