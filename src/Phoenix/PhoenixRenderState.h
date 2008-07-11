#ifndef __PhoenixRenderState_h__
#define __PhoenixRenderState_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
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
      Phoenix::Core::CEnableable	     m_DepthTest;
      Phoenix::Core::CEnableable	     m_DepthWrite;
      Phoenix::Core::CEnableable	     m_FaceCulling;
    public:      
      inline Phoenix::Graphics::CAlphaTestOperation & GetAlphaOperation()    { return m_AlphaTestOp; }
      inline Phoenix::Graphics::CBlendingOperation  & GetBlendingOperation() { return m_BlendingOp;  }
      inline bool IsTransparent() const    { return m_BlendingOp.IsEnabled(); }
      inline Phoenix::Core::CEnableable & GetDepthTest()  { return m_DepthTest; }
      inline Phoenix::Core::CEnableable & GetDepthWrite() { return m_DepthWrite; }
      inline Phoenix::Core::CEnableable & GetFaceCulling() { return m_FaceCulling; }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
