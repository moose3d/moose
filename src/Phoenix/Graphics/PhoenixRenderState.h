#ifndef __PhoenixRenderState_h__
#define __PhoenixRenderState_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixBlendingOperation.h"
#include "PhoenixAlphaTestOperation.h"
#include "PhoenixAPI.h"
#include "PhoenixVector4.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixMaterial.h"
#include <map>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {

	  class CRenderable;

	  typedef std::list< Phoenix::Graphics::CRenderable * > LightRenderableList;
	  typedef std::map< std::string, std::string > 				  NameValueMap;
	  // typedefs for readability (and maintanence)
	  typedef std::vector< std::pair<std::string, Phoenix::Default::VERTEX_HANDLE * > > ShaderParams;
	  typedef std::map< std::string, int >	       										ShaderIntParams;
	  typedef std::map< std::string, float >	       									ShaderFloatParams;

	  /////////////////////////////////////////////////////////////////
    /// Renderstate object. Helps to sort things by transparency, for instance.
    class PHOENIX_API CRenderState
    {
    private:
      Phoenix::Graphics::CAlphaTestOperation m_AlphaTestOp; ///!< Is alpha test enabled and what comparison to use.
      Phoenix::Graphics::CBlendingOperation  m_BlendingOp; ///!< Is blending enabled and what operation to use.
      bool	     m_DepthTest;    ///!< Is Depth testing enabled.
      bool	     m_DepthWrite;   ///!< Is Depth write enabled
      bool	     m_FaceCulling;  ///!< Is faceculling enabled.
      bool	     m_bLighting;    ///!< Is lighting enabled.
      bool 		 m_bLightSource; ///!< Hint for detecting a light source renderable.
      unsigned int m_nLightId; ///!< Used in conjunction with LightSource member, hint for Renderer which light to set.
      LightRenderableList m_lstLights; ///!< Which lights apply to this renderstate. If renderstate belongs to a light, these must not be set.
      Phoenix::Math::CVector4<unsigned char> m_BaseColor;

      Phoenix::Default::SHADER_HANDLE			      m_ShaderHandle;  			///!< Handle to a shader
      ShaderParams			      m_vShaderParams; 			///!< Shader parameters
      ShaderIntParams			    m_vShaderIntParams; 	///!< Shader integer parameters
      ShaderFloatParams			  m_vShaderFloatParams; ///!< Shader float parameters

      /// Texture filters for each texture.
      std::vector<Phoenix::Graphics::TEXTURE_FILTER>	      m_aTextureFilters[TEXTURE_HANDLE_COUNT];
      /// Handle to textures.
      Phoenix::Default::TEXTURE_HANDLE			      				m_aTextureHandles[TEXTURE_HANDLE_COUNT];
      Phoenix::Graphics::CMaterial      m_Material;
    public:
      CRenderState();
      virtual ~CRenderState();
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

      ////////////////////
      /// Returns handle to Texture.
      /// \param nId Which texture unit is handled. By default it is zero, the first.
      /// \returns TEXTURE_HANDLE
      Phoenix::Default::TEXTURE_HANDLE    & GetTextureHandle( unsigned int nId = 0);
      ////////////////////
      /// Returns handle to shader.
      /// \returns SHADER_HANDLE.
      Phoenix::Default::SHADER_HANDLE  & GetShaderHandle();
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param handle Handle to vertexdescriptor.
      void		AddShaderParameter( const char *sName, Phoenix::Default::VERTEX_HANDLE * pHandle );
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param fValue Parameter value.
      void		AddShaderParameter( const char *sName, float fValue );
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param iValue Parameter value.
      void		AddShaderParameter( const char *sName, int iValue );
      ////////////////////
      /// Returns reference to shader parameters.
      /// \returns Vector of string-VERTEX_HANDLE pairs.
      ShaderParams & GetShaderParameters();
      ////////////////////
      /// Returns reference to shader parameters.
      /// \returns Vector of string-int pairs.
      ShaderIntParams & GetShaderIntParameters();
      ////////////////////
      /// Returns reference to shader parameters.
      /// \returns Vector of string-float pairs.
      ShaderFloatParams & GetShaderFloatParameters();
      ////////////////////
      /// Returns texture filters for given texture.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1.  By default, it is first (zero).
      /// \returns Reference to texture filter vector.
      std::vector<Phoenix::Graphics::TEXTURE_FILTER> & GetTextureFilters( unsigned int nId = 0 );
      ////////////////////
      /// Adds texture filter to given texture.
      /// \param tTexFilter TEXTURE_FILTER to insert.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1. By default, it is first (zero).
      void		AddTextureFilter( Phoenix::Graphics::TEXTURE_FILTER tTexFilter, unsigned int nId = 0 );
      ////////////////////
      /// \returns material reference.
      Phoenix::Graphics::CMaterial & GetMaterial();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
