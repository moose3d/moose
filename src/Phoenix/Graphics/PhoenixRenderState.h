#ifndef __PhoenixRenderState_h__
#define __PhoenixRenderState_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixBlendingOperation.h"
#include "PhoenixAlphaTestOperation.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    
    /////////////////////////////////////////////////////////////////
    /// Renderstate object. Helps to sort things by transparency, for instance.
    class PHOENIX_API CRenderState
    {
    private:
      Phoenix::Graphics::CAlphaTestOperation m_AlphaTestOp;
      Phoenix::Graphics::CBlendingOperation  m_BlendingOp;
      bool	     m_DepthTest;
      bool	     m_DepthWrite;
      bool	     m_FaceCulling;
      bool	     m_bLighting;
      Phoenix::Math::CVector4<unsigned char> m_BaseColor;
    public:      
      CRenderState() : m_DepthTest(false), 
		       m_DepthWrite(true), 
		       m_FaceCulling(false), 
		       m_bLighting(false), 
		       m_BaseColor(255,255,255,255) {}
      inline Phoenix::Graphics::CAlphaTestOperation & GetAlphaOperation()    { return m_AlphaTestOp; }
      inline Phoenix::Graphics::CBlendingOperation  & GetBlendingOperation() { return m_BlendingOp;  }
      inline bool IsTransparent() const    { return m_BlendingOp.IsEnabled(); }
      inline bool & GetDepthTest()  { return m_DepthTest; }
      inline bool & GetDepthWrite() { return m_DepthWrite; }
      inline bool & GetFaceCulling() { return m_FaceCulling; }
      inline CVector4<unsigned char> & GetBaseColor() { return m_BaseColor; }
      inline bool & GetLighting() { return m_bLighting; }
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif