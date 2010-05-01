#ifndef __PhoenixRenderState_h__
#define __PhoenixRenderState_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixCore.h"
#include "PhoenixLogger.h"
#include "PhoenixBlendingOperation.h"
#include "PhoenixAlphaTestOperation.h"
#include "PhoenixAPI.h"
#include "PhoenixVector4.h"
#include "PhoenixDefaultEntities.h"
#include "PhoenixMaterial.h"
#include <map>
#include <iostream>
#include <cassert>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {

	  class CRenderable;

	  typedef std::list< Phoenix::Graphics::CRenderable * > LightRenderableList;
	  typedef std::map< std::string, std::string > 				  NameValueMap;
	  /////////////////////////////////////////////////////////////////////////
      class PHOENIX_API IShaderParam : public Phoenix::Core::CNamed 
      {
      protected:
          Phoenix::Default::VERTEX_HANDLE       m_hData;
      public:
          virtual ~IShaderParam() {}
          
          void SetData( Phoenix::Default::VERTEX_HANDLE & handle ) 
          {
              m_hData = handle;
          }
          
          virtual void Bind( CShader & s, size_t nIndex ) = 0;
                   
          virtual void Apply(size_t nIndex) = 0;
          
      };
      class PHOENIX_API CShaderUniform : public Phoenix::Graphics::IShaderParam
      {
      protected:
          GLuint  m_iLocation;
      public:
          CShaderUniform() : m_iLocation(-1) {}
          void Bind( CShader & s, size_t nIndex ) 
          {
              m_iLocation = glGetUniformLocation(s.GetProgram(), GetName().c_str());
          }
          
          void Apply( size_t nIndex )
          {
              
              Phoenix::Graphics::CVertexDescriptor *pData = *m_hData;
              switch( pData->GetType() )
              {
                  case ELEMENT_TYPE_UNIFORM_1F:
                      glUniform1fv( m_iLocation, pData->GetSize(), pData->GetPointer<float>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_2F:
                      glUniform2fv( m_iLocation, pData->GetSize(), pData->GetPointer<float>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_3F:
                      glUniform3fv( m_iLocation, pData->GetSize(), pData->GetPointer<float>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_4F:
                      glUniform4fv( m_iLocation, pData->GetSize(), pData->GetPointer<float>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_1I:
                      glUniform1iv( m_iLocation, pData->GetSize(), pData->GetPointer<int>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_2I:
                      glUniform2iv( m_iLocation, pData->GetSize(), pData->GetPointer<int>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_3I:
                      glUniform3iv( m_iLocation, pData->GetSize(), pData->GetPointer<int>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_4I:
                      glUniform4iv( m_iLocation, pData->GetSize(), pData->GetPointer<int>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_2X2F:
                      glUniformMatrix2fv(m_iLocation, pData->GetSize(), GL_FALSE, pData->GetPointer<float>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_3X3F:
                      glUniformMatrix3fv(m_iLocation, pData->GetSize(), GL_FALSE, pData->GetPointer<float>());
                      break;
                  case ELEMENT_TYPE_UNIFORM_4X4F:     
                      glUniformMatrix4fv(m_iLocation, pData->GetSize(), GL_FALSE, pData->GetPointer<float>());
                      break;
                  default:
                      assert( false && "Uniform parameter is not proper!");
                      break;
              }
          }
      };
      ///////////////////////////////////////////////////////////////
      class PHOENIX_API CShaderAttrib : public Phoenix::Graphics::IShaderParam 
      {
      public:
          
          void Bind( CShader & s, size_t nIndex ) 
          {
              glBindAttribLocation(s.GetProgram(), nIndex, GetName().c_str());
          }
          
          void Apply(size_t nIndex)
          {
              Phoenix::Graphics::CVertexDescriptor *pData = *m_hData;
              switch( pData->GetType() )
              {
                  case ELEMENT_TYPE_VERTEX_3F:
                  case ELEMENT_TYPE_NORMAL_3F:
                  case ELEMENT_TYPE_COLOR_3F:
                  case ELEMENT_TYPE_COLOR_4F:
                  case ELEMENT_TYPE_TEX_2F:
                  case ELEMENT_TYPE_TEX_3F:
                  case ELEMENT_TYPE_TEX_4F:
                  case ELEMENT_TYPE_ATTRIB_1F:
                  case ELEMENT_TYPE_ATTRIB_2F:
                  case ELEMENT_TYPE_ATTRIB_3F:
                  case ELEMENT_TYPE_ATTRIB_4F:
                      glVertexAttribPointer(nIndex, pData->GetSize(), GL_FLOAT, 0, 0, pData->GetPointer<float>());
                      break;
                  case ELEMENT_TYPE_COLOR_4UB:
                  case ELEMENT_TYPE_ATTRIB_1UB:
                  case ELEMENT_TYPE_ATTRIB_2UB:
                  case ELEMENT_TYPE_ATTRIB_3UB:
                  case ELEMENT_TYPE_ATTRIB_4UB:
                      glVertexAttribPointer(nIndex, pData->GetSize(), GL_UNSIGNED_BYTE, 0, 0, pData->GetPointer<unsigned char>());
                      break;
                  case ELEMENT_TYPE_ATTRIB_1I:
                  case ELEMENT_TYPE_ATTRIB_2I:
                  case ELEMENT_TYPE_ATTRIB_3I:
                  case ELEMENT_TYPE_ATTRIB_4I:
                      glVertexAttribPointer(nIndex, pData->GetSize(), GL_INT, 0, 0, pData->GetPointer<int>());
                      break;
                  case ELEMENT_TYPE_V3F_N3F_T2F:
                      g_Error << "ELEMENT_TYPE_V3F_N3F_T2F Not supported" << std::endl; 
                      abort();
                      break;
              }
              glEnableVertexAttribArray(nIndex);
          }
      };
      ///////////////////////////////////////////////////////////////
      class PHOENIX_API CShaderParamContainer 
      {
      public:
          std::vector<Phoenix::Graphics::IShaderParam*> m_vecParams;
          virtual ~CShaderParamContainer() {
                
          }
          void Bind( CShader & s)
          {
              for (size_t i=0;i<m_vecParams.size();i++) {
                  m_vecParams[i]->Bind(s,i);
              }
          }
          
          void Apply()
          {
              for (size_t i=0;i<m_vecParams.size();i++) {
                  m_vecParams[i]->Apply(i);
              }
          }
          
          void Add( IShaderParam *pParam )
          {
              m_vecParams.push_back(pParam);
          }
          
      };
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

      Phoenix::Default::SHADER_HANDLE     m_ShaderHandle;    ///!< Handle to a shader
      CShaderParamContainer               m_ShaderAttribs;   ///!< All attrib parameters;
      CShaderParamContainer               m_ShaderUniforms;  ///!< All uniform parameters;
        
        /// Texture filters for each texture.
      std::vector<Phoenix::Graphics::TEXTURE_FILTER>	   m_aTextureFilters[TEXTURE_HANDLE_COUNT];
      /// Handle to textures.
      Phoenix::Default::TEXTURE_HANDLE			      	   m_aTextureHandles[TEXTURE_HANDLE_COUNT];
      Phoenix::Graphics::CMaterial                         m_Material;
        
    public:
        
      CRenderState();
      virtual ~CRenderState();
        
      inline Phoenix::Graphics::CAlphaTestOperation & GetAlphaOperation()    { return m_AlphaTestOp; }
      inline void SetAlphaOperation(Phoenix::Graphics::CAlphaTestOperation & op)    { m_AlphaTestOp = op; }
      inline Phoenix::Graphics::CBlendingOperation  & GetBlendingOperation() { return m_BlendingOp;  }
      inline void SetBlendingOperation(Phoenix::Graphics::CBlendingOperation  & op) { m_BlendingOp = op;  }

      inline bool IsTransparent() const    { return m_BlendingOp.IsEnabled(); }
      inline bool & GetDepthTest()  { return m_DepthTest; }
      inline void SetDepthTest(bool bFlag)  { m_DepthTest = bFlag; }

      inline bool & GetDepthWrite() { return m_DepthWrite; }
      inline void SetDepthWrite(bool bFlag) { m_DepthWrite = bFlag; }

      inline bool & GetFaceCulling() { return m_FaceCulling; }
      inline void SetFaceCulling(bool bFlag) { m_FaceCulling = bFlag; }

      inline bool & IsLightSource() { return m_bLightSource; }
      inline void SetLightSource(bool bFlag) { m_bLightSource = bFlag; }

      inline Phoenix::Math::CVector4<unsigned char> & GetBaseColor() { return m_BaseColor; }
      inline void SetBaseColor(Phoenix::Math::CVector4<unsigned char> & col) { m_BaseColor = col; }

      inline bool & GetLighting() { return m_bLighting; }
      inline void SetLighting(bool bFlag) { m_bLighting = bFlag; }

      inline LightRenderableList & GetLights() { return m_lstLights; }
      inline unsigned int & GetLightId() { return m_nLightId; }
      inline void SetLightId(unsigned int nVal) { m_nLightId = nVal; }
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
      /// \param pAttrib Shader attrib.
      void		AddShaderAttrib( const char *sName, Phoenix::Default::VERTEX_HANDLE & handle );
      void		AddShaderUniform( const char *sName, Phoenix::Default::VERTEX_HANDLE & handle );
      ////////////////////
      /// Returns reference to shader attribs.
      /// \returns Shader attribute container.
      CShaderParamContainer & GetShaderAttribs();
      ////////////////////
      /// Returns reference to shader uniforms.
      /// \returns Shader uniform container.
      CShaderParamContainer & GetShaderUniforms();
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
