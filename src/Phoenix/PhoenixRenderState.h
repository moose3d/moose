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
      bool	     m_DepthTest;
      bool	     m_DepthWrite;
      bool	     m_FaceCulling;
    public:      
      inline Phoenix::Graphics::CAlphaTestOperation & GetAlphaOperation()    { return m_AlphaTestOp; }
      inline Phoenix::Graphics::CBlendingOperation  & GetBlendingOperation() { return m_BlendingOp;  }
      inline bool IsTransparent() const    { return m_BlendingOp.IsEnabled(); }
      inline bool & GetDepthTest()  { return m_DepthTest; }
      inline bool & GetDepthWrite() { return m_DepthWrite; }
      inline bool & GetFaceCulling() { return m_FaceCulling; }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
