#ifndef __PhoenixRenderState_h__
#define __PhoenixRenderState_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixBlendingOperation.h"
#include "PhoenixAlphaTestOperation.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Renderstate object. Helps to sort things by transparency, for instance.
    class CRenderState
    {
    private:
      Phoenix::Graphics::CAlphaTestOperation m_AlphaTestOp;
      Phoenix::Graphics::CBlendingOperation  m_BlendingOp;
    public:      
      inline Phoenix::Graphics::CAlphaTestOperation & GetAlphaOperation()    { return m_AlphaTestOp; }
      inline Phoenix::Graphics::CBlendingOperation  & GetBlendingOperation() { return m_BlendingOp;  }
      inline bool IsTransparent() const { return m_BlendingOp.IsEnabled(); }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
