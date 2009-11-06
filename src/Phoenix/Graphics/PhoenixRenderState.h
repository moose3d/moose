#ifndef __PhoenixRenderState_h__
#define __PhoenixRenderState_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixBlendingOperation.h"
#include "PhoenixAlphaTestOperation.h"
#include "PhoenixAPI.h"
#include "PhoenixVector4.h"
#include <map>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
	  class CRenderable;
	  typedef std::list< Phoenix::Graphics::CRenderable * > LightRenderableList;
	  typedef std::map< std::string, std::string > 				  NameValueMap;
    /////////////////////////////////////////////////////////////////
    /// Renderstate object. Helps to sort things by transparency, for instance.
    class PHOENIX_API CRenderState
    {
    private:
      Phoenix::Graphics::CAlphaTestOperation m_AlphaTestOp; ///< Is alpha test enabled and what comparison to use.
      Phoenix::Graphics::CBlendingOperation  m_BlendingOp; ///< Is blending enabled and what operation to use.
      bool	     m_DepthTest;    ///< Is Depth testing enabled.
      bool	     m_DepthWrite;   ///< Is Depth write enabled
      bool	     m_FaceCulling;  ///< Is faceculling enabled.
      bool	     m_bLighting;    ///< Is lighting enabled.
      bool 		 m_bLightSource; ///< Hint for detecting a light source renderable.
      unsigned int m_nLightId; ///< Used in conjunction with LightSource member, hint for Renderer which light to set.
      LightRenderableList m_lstLights; ///< Which lights apply to this renderstate. If renderstate belongs to a light, these must not be set.
      Phoenix::Math::CVector4<unsigned char> m_BaseColor;
    public:
      CRenderState() : m_DepthTest(false),
		       m_DepthWrite(true),
		       m_FaceCulling(false),
		       m_bLighting(false),
		       m_bLightSource(false),
		       m_nLightId(0),
		       m_BaseColor(255,255,255,255) {}
      inline Phoenix::Graphics::CAlphaTestOperation & GetAlphaOperation()    { return m_AlphaTestOp; }
      inline Phoenix::Graphics::CBlendingOperation  & GetBlendingOperation() { return m_BlendingOp;  }
      inline bool IsTransparent() const    { return m_BlendingOp.IsEnabled(); }
      inline bool & GetDepthTest()  { return m_DepthTest; }
      inline bool & GetDepthWrite() { return m_DepthWrite; }
      inline bool & GetFaceCulling() { return m_FaceCulling; }
      inline bool & IsLightSource() { return m_bLightSource; }
      inline Phoenix::Math::CVector4<unsigned char> & GetBaseColor() { return m_BaseColor; }
      inline bool & GetLighting() { return m_bLighting; }
      inline LightRenderableList & GetLights() { return m_lstLights; }
      inline unsigned int & GetLightId() { return m_nLightId; }
      void   ParseFrom( NameValueMap & map );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
