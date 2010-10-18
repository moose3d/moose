#ifndef __MooseRenderState_h__
#define __MooseRenderState_h__
/////////////////////////////////////////////////////////////////
#include "MooseCore.h"
#include "MooseLogger.h"
#include "MooseBlendingOperation.h"
#include "MooseAlphaTestOperation.h"
#include "MoosePolygonOffset.h"
#include "MooseAPI.h"
#include "MooseVector4.h"
#include "MooseDefaultEntities.h"
#include "MooseMaterial.h"
#include "MooseMatrix4x4.h"

#include <map>
#include <iostream>
#include <cassert>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {

    class CRenderable;
    class COglRenderer;
    typedef std::list< Moose::Graphics::CRenderable * > LightRenderableList;
    typedef std::map< std::string, std::string > 				  NameValueMap;
    /////////////////////////////////////////////////////////////////////////
    class MOOSE_API IShaderParam : public Moose::Core::CNamed 
    {
    public:
      virtual ~IShaderParam() {}
      virtual void Bind( CShader & s, size_t nIndex ) = 0;
      virtual void Apply(Moose::Graphics::COglRenderer & r, size_t nIndex ) = 0;
      virtual void *GetData() = 0;
    };
      
    ///////////////////////////////////////////////////////////////////
    /// Shader parameter for generic uniform case.
    class MOOSE_API CShaderUniform : public Moose::Graphics::IShaderParam
    {
    protected:
      GLint  m_iLocation;
      Moose::Default::VERTEX_HANDLE       m_hData;
          
    public:
      CShaderUniform() : m_iLocation(-1) {}
          
      void Bind( CShader & s, size_t nIndex ) 
      {
        //g_Log << "UNIFORM: Binding " << GetName() << std::endl; 
        m_iLocation = glGetUniformLocation(s.GetProgram(), GetName().c_str());
      }
      void SetData( Moose::Default::VERTEX_HANDLE & handle ) 
      {
        m_hData = handle;
      }
      void SetData( const char *szResourceName )
      {
        m_hData = szResourceName;
      }
      void Apply(  Moose::Graphics::COglRenderer & r, size_t nIndex = 0)
      {
        if ( m_hData.IsNull() || m_iLocation == -1 ) 
        {
          if ( m_hData.IsNull()) g_Error << GetName() << ": Data is null" << std::endl;
          else g_Error << GetName() << ": Location is null" << std::endl;
          return;
        }
#if defined(DEBUG)
        g_Log << "UNIFORM: applying " << GetName() << std::endl;
#endif
        Moose::Graphics::CVertexDescriptor *pData = *m_hData;
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
      void * GetData() 
      { 
        return *m_hData; 
      }
    };
    ///////////////////////////////////////////////////////////////////
    /// Shader parameter for generic uniform case.
    class MOOSE_API CShaderUniformPtr : public Moose::Graphics::IShaderParam
    {
    protected:
      GLint  m_iLocation;
      Moose::Graphics::CVertexDescriptor *       m_pData;
          
    public:
      CShaderUniformPtr() : m_iLocation(-1), m_pData(NULL) {}
          
      void Bind( CShader & s, size_t nIndex ) 
      {
        //g_Log << "UNIFORM: Binding " << GetName() << std::endl; 
        m_iLocation = glGetUniformLocation(s.GetProgram(), GetName().c_str());
      }
      void SetData( Moose::Graphics::CVertexDescriptor * pData ) 
      {
        m_pData = pData;
      }
          
      void Apply(  Moose::Graphics::COglRenderer & r, size_t nIndex = 0)
      {
        if ( m_pData == NULL || m_iLocation == -1 ) 
        {
          if ( !m_pData ) g_Error << GetName() << ": Data is null" << std::endl;
          else g_Error << GetName() << ": Location is null" << std::endl;
          return;
        }
#if defined(DEBUG)
        g_Log << "UNIFORM: applying " << GetName() << std::endl;
#endif
        Moose::Graphics::CVertexDescriptor *pData = m_pData;
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
      void * GetData() 
      { 
        return m_pData; 
      }
    };
    //////////////////////////////////////////////////////////////////
    /// Special case for transforms.
    class MOOSE_API CShaderUniformMat4x4f : public Moose::Graphics::IShaderParam
    {
    protected:
      GLint  m_iLocation;
      /// This matrix is always somewhere else, it should not be freed.
      const Moose::Math::CMatrix4x4<float> *m_pMatrix;
    public:
      CShaderUniformMat4x4f() : m_iLocation(-1),m_pMatrix(NULL) {}
          
      void Bind( CShader & s, size_t nIndex ) 
      {

        m_iLocation = glGetUniformLocation(s.GetProgram(), GetName().c_str());
        //g_Log << "UNIFORM: Bound " << GetName() << " to " << m_iLocation << std::endl; 
      }
      void SetData( const Moose::Math::CMatrix4x4<float> *pMatrix )
      {
        m_pMatrix = pMatrix;
      } 
      void Apply( Moose::Graphics::COglRenderer & r, size_t nIndex = 0 )
      {
        if ( m_pMatrix == NULL || m_iLocation == -1 )  return;

        //g_Log << "applying: " << m_pMatrix->GetTransposition() << "\n";
        glUniformMatrix4fv(m_iLocation, 1, GL_FALSE, m_pMatrix->GetTransposition().GetArray());
      }
      void * GetData()
      {
        return (void *)m_pMatrix;
      }
    };
    //////////////////////////////////////////////////////////////////
    /// Special case for transforms.
    class MOOSE_API CShaderUniformInt : public Moose::Graphics::IShaderParam
    {
    protected:
      GLint  m_iLocation;
      GLint  m_iValue;

    public:
      CShaderUniformInt() : m_iLocation(-1) {}
          
      void Bind( CShader & s, size_t nIndex ) 
      {
        //g_Log << "UNIFORM: Binding " << GetName() << std::endl; 
        m_iLocation = glGetUniformLocation(s.GetProgram(), GetName().c_str());
      }
      void SetData( GLint iValue )
      {
        m_iValue = iValue;
      } 
      void Apply( Moose::Graphics::COglRenderer & r, size_t nIndex = 0 )
      {
        if ( m_iLocation == -1 ) return;
        glUniform1i(m_iLocation, m_iValue);
      }
      void * GetData()
      {
        return (void *)&m_iValue;
      }
    };
    /////////////////////////////////////////
    class MOOSE_API CShaderUniformFloat : public Moose::Graphics::IShaderParam
    {
    protected:
      GLint  m_iLocation;
      GLfloat  m_fValue;
          
    public:
      CShaderUniformFloat() : m_iLocation(-1) {}
          
      void Bind( CShader & s, size_t nIndex ) 
      {
        //g_Log << "UNIFORM: Binding " << GetName() << std::endl; 
        m_iLocation = glGetUniformLocation(s.GetProgram(), GetName().c_str());
      }
      void SetData( GLfloat fValue )
      {
        m_fValue = fValue;
      } 
      void Apply( Moose::Graphics::COglRenderer & r, size_t nIndex = 0 )
      {
        if ( m_iLocation == -1 ) return;
        glUniform1f(m_iLocation, m_fValue);
      }
      void * GetData()
      {
        return (void *)&m_fValue;
      }
    };
    /////////////////////////////////////////
    class MOOSE_API CShaderUniformVec4fPtr : public Moose::Graphics::IShaderParam
    {
    protected:
      GLint  m_iLocation;
      /// points somewhere else, is never freed.
      const Moose::Math::CVector4<float>  *m_pData; 
          
    public:
      CShaderUniformVec4fPtr() : m_iLocation(-1),m_pData(NULL) {}
          
      void Bind( CShader & s, size_t nIndex ) 
      {
        //g_Log << "UNIFORM: Binding " << GetName() << std::endl; 
        m_iLocation = glGetUniformLocation(s.GetProgram(), GetName().c_str());
        g_Error << "Bound to " << m_iLocation << "\n";
      }
      void SetData( const Moose::Math::CVector4<float> * pData )
      {
        m_pData = pData;
      } 
      void Apply( Moose::Graphics::COglRenderer & r, size_t nIndex = 0 )
      {
        if ( m_iLocation == -1 ) 
        {
          g_Error << "no location bound!\n";
          return;
        }
        glUniform4fv( m_iLocation, 1, m_pData->GetArray());
      }
      void * GetData()
      {
        return (void *)m_pData;
      }
    };
    ///////////////////////////////////////////////////////////////
    /// Case for shader attribs.
    class MOOSE_API CShaderAttribPtr : public Moose::Graphics::IShaderParam 
    {
    protected:
      Moose::Graphics::CVertexDescriptor *       m_pData;
    public:
      CShaderAttribPtr() : m_pData(NULL) {}
      void Bind( CShader & s, size_t nIndex ) 
      {
        //g_Log << "ATTRIB: binding " << GetName() << " to index : " << nIndex << std::endl;
        glBindAttribLocation(s.GetProgram(), nIndex, GetName().c_str());
      }
          
      void SetData( Moose::Graphics::CVertexDescriptor * pData ) 
      {
        m_pData = pData;
      }
          
      void Apply(Moose::Graphics::COglRenderer & r, size_t nIndex)
      {
        if ( m_pData == NULL ) return;
              
        GLvoid *tmp = 0;
              
        // check whether VBO is used.
        if ( !m_pData->IsCached() ) m_pData->CreateCache( GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, m_pData->GetCache());              

        switch( m_pData->GetType() )
        {
        case ELEMENT_TYPE_ATTRIB_1F:
          glVertexAttribPointer(nIndex, 1, GL_FLOAT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_TEX_2F:
        case ELEMENT_TYPE_ATTRIB_2F:
          glVertexAttribPointer(nIndex, 2, GL_FLOAT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_VERTEX_3F:
        case ELEMENT_TYPE_NORMAL_3F:
        case ELEMENT_TYPE_COLOR_3F:
        case ELEMENT_TYPE_TEX_3F:
        case ELEMENT_TYPE_ATTRIB_3F:
          glVertexAttribPointer(nIndex, 3, GL_FLOAT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_COLOR_4F:
        case ELEMENT_TYPE_TEX_4F:
        case ELEMENT_TYPE_ATTRIB_4F:
          glVertexAttribPointer(nIndex, 4, GL_FLOAT, 0, 0, tmp);
          break;
                      
        case ELEMENT_TYPE_ATTRIB_1UB:
          glVertexAttribPointer(nIndex, 1, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_2UB:
          glVertexAttribPointer(nIndex, 2, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
                      
        case ELEMENT_TYPE_ATTRIB_3UB:
          glVertexAttribPointer(nIndex, 3, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
                      
        case ELEMENT_TYPE_ATTRIB_4UB:
        case ELEMENT_TYPE_COLOR_4UB:
          glVertexAttribPointer(nIndex, 4, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_1I:
          glVertexAttribPointer(nIndex, 1, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_2I:
          glVertexAttribPointer(nIndex, 2, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_3I:
          glVertexAttribPointer(nIndex, 3, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_4I:
          glVertexAttribPointer(nIndex, 4, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_V3F_N3F_T2F:
          g_Error << "ELEMENT_TYPE_V3F_N3F_T2F Not supported" << std::endl; 
          abort();
          break;
        default:
          assert(NULL && "This should not be here.");
          break;
        }
        glEnableVertexAttribArray(nIndex);
        //g_Log << "ATTRIB: applied index" << nIndex << std::endl;
      }
          
      void * GetData()
      {
        return m_pData;
      }
    };
      
      
      
    ///////////////////////////////////////////////////////////////
    /// Case for shader attribs.
    class MOOSE_API CShaderAttrib : public Moose::Graphics::IShaderParam 
    {
    protected:
      Moose::Default::VERTEX_HANDLE       m_hData;
    public:
          
      void Bind( CShader & s, size_t nIndex ) 
      {
        //g_Log << "ATTRIB: binding " << GetName() << " to index : " << nIndex << std::endl;
        glBindAttribLocation(s.GetProgram(), nIndex, GetName().c_str());
      }
      void SetData( Moose::Default::VERTEX_HANDLE & handle ) 
      {
        m_hData = handle;
      }
      void Apply(Moose::Graphics::COglRenderer & r, size_t nIndex)
      {
              
        if ( m_hData.IsNull() ) return;
        GLvoid *tmp = 0;
        Moose::Graphics::CVertexDescriptor *pData = *m_hData;
        // check whether VBO is used.
        if ( !pData->IsCached() ) pData->CreateCache( GL_STATIC_DRAW );
        glBindBuffer(GL_ARRAY_BUFFER, pData->GetCache());      
              
        switch( pData->GetType() )
        {
        case ELEMENT_TYPE_ATTRIB_1F:
          glVertexAttribPointer(nIndex, 1, GL_FLOAT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_TEX_2F:
        case ELEMENT_TYPE_ATTRIB_2F:
          glVertexAttribPointer(nIndex, 2, GL_FLOAT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_VERTEX_3F:
        case ELEMENT_TYPE_NORMAL_3F:
        case ELEMENT_TYPE_COLOR_3F:
        case ELEMENT_TYPE_TEX_3F:
        case ELEMENT_TYPE_ATTRIB_3F:
          glVertexAttribPointer(nIndex, 3, GL_FLOAT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_COLOR_4F:
        case ELEMENT_TYPE_TEX_4F:
        case ELEMENT_TYPE_ATTRIB_4F:
          glVertexAttribPointer(nIndex, 4, GL_FLOAT, 0, 0, tmp);
          break;
                      
        case ELEMENT_TYPE_ATTRIB_1UB:
          glVertexAttribPointer(nIndex, 1, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_2UB:
          glVertexAttribPointer(nIndex, 2, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
                      
        case ELEMENT_TYPE_ATTRIB_3UB:
          glVertexAttribPointer(nIndex, 3, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
                      
        case ELEMENT_TYPE_ATTRIB_4UB:
        case ELEMENT_TYPE_COLOR_4UB:
          glVertexAttribPointer(nIndex, 4, GL_UNSIGNED_BYTE, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_1I:
          glVertexAttribPointer(nIndex, 1, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_2I:
          glVertexAttribPointer(nIndex, 2, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_3I:
          glVertexAttribPointer(nIndex, 3, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_ATTRIB_4I:
          glVertexAttribPointer(nIndex, 4, GL_INT, 0, 0, tmp);
          break;
        case ELEMENT_TYPE_V3F_N3F_T2F:
          g_Error << "ELEMENT_TYPE_V3F_N3F_T2F Not supported" << std::endl; 
          abort();
          break;
        default:
          assert(NULL && "This should not be here.");
          break;
        }
              
        glEnableVertexAttribArray(nIndex);
        //g_Log << "ATTRIB: applied index" << nIndex << std::endl;
      }
          
      void * GetData()
      {
        return *m_hData;
      }
    };
    ///////////////////////////////////////////////////////////////
    class MOOSE_API CShaderParamContainer 
    {
    public:
      std::map<std::string, Moose::Graphics::IShaderParam*> m_mapNameToParam;
      std::vector<Moose::Graphics::IShaderParam*> m_vecParams;
      virtual ~CShaderParamContainer() {
        while ( !m_vecParams.empty())
        {
          delete m_vecParams.back();// deletes only shaderparam structure, not necessarily the actual data.
          m_vecParams.pop_back();
        }
        // pointers deleted earlier, no need to do again.
        m_mapNameToParam.clear();
      }
      void Bind( CShader & s)
      {
        for (size_t i=0;i<m_vecParams.size();i++) {
          m_vecParams[i]->Bind(s,i);
        }
      }
          
      void Apply( COglRenderer & r)
      {
        for (size_t i=0;i<m_vecParams.size();i++) {
          m_vecParams[i]->Apply(r, i);
        }
      }
          
      void Add( IShaderParam *pParam )
      {
        m_mapNameToParam[pParam->GetName()] = pParam;
        m_vecParams.push_back(pParam);
      }
          
      Moose::Graphics::IShaderParam * GetParameter( const char *szName )
      {
        std::map<std::string, Moose::Graphics::IShaderParam*>::iterator it;
        if ( (it=m_mapNameToParam.find(std::string(szName))) != m_mapNameToParam.end() )
        {
          return it->second;
        } 
        else 
        {
          return NULL;
        }
      }
          
    };
    enum RenderLayerTag {
      kBackground = 1000,
      kOpaque = 2000,
      kTransparent = 3000,
      kOverlay = 4000,
      kGUI = 5000
    };
    /////////////////////////////////////////////////////////////////
    /// Renderstate object. Helps to sort things by transparency, for instance.
    ///
    /// Renderlayer values used for sorting are as follows:
    /// - Background : 1000 - 1999
    /// - Opaque : 2000 - 2999
    /// - Transparent : 3000- 3999
    /// - Overlay :  4000- 4999
    ///
    /// By default, renderlayer is set to Opaque.
    class MOOSE_API CRenderState
    {
    private:
        
      unsigned short m_iRenderLayer; ///!< To which render layer this renderable belongs (rendered from 0->65536).
        
      Moose::Graphics::CAlphaTestOperation m_AlphaTestOp; ///!< Is alpha test enabled and what comparison to use.
      Moose::Graphics::CBlendingOperation  m_BlendingOp; ///!< Is blending enabled and what operation to use.
      Moose::Graphics::CPolygonOffset    m_PolyOffset; ///!< Is polygon offset enabled and using which parameters.
      bool	     m_DepthTest;    ///!< Is Depth testing enabled.
      bool	     m_DepthWrite;   ///!< Is Depth write enabled
      bool	     m_FaceCulling;  ///!< Is faceculling enabled.
      bool	     m_bLighting;    ///!< Is lighting enabled.
      bool 		 m_bLightSource; ///!< Hint for detecting a light source renderable.
      unsigned int m_nLightId; ///!< Used in conjunction with LightSource member, hint for Renderer which light to set.
      
      LightRenderableList m_lstLights; ///!< Which lights apply to this renderstate. If renderstate belongs to a light, these must not be set.
      Moose::Graphics::CColor4ub m_BaseColor;

      Moose::Default::SHADER_HANDLE     m_ShaderHandle;    ///!< Handle to a shader
      CShaderParamContainer               m_ShaderAttribs;   ///!< All attrib parameters;
      CShaderParamContainer               m_ShaderUniforms;  ///!< All uniform parameters;
      CShaderUniformMat4x4f               m_ViewUniform;   ///!< View transform.  
      CShaderUniformMat4x4f               m_ProjUniform;   ///!< Projection transform.
      CShaderUniformMat4x4f               m_ModelUniform;   ///!< Model transform.
      /// Texture filters for each texture.
      std::vector<Moose::Graphics::TEXTURE_FILTER>	   m_aTextureFilters[TEXTURE_HANDLE_COUNT];
      /// Handle to textures.
      Moose::Default::TEXTURE_HANDLE			      	   m_aTextureHandles[TEXTURE_HANDLE_COUNT];
      Moose::Graphics::CMaterial                         m_Material;
    public:
        
      CRenderState();
      virtual ~CRenderState();
      void SetRenderLayer( unsigned short iLayerValue );
      unsigned short GetRenderLayer() const;
      inline Moose::Graphics::CAlphaTestOperation & GetAlphaOperation()    { return m_AlphaTestOp; }
      inline void SetAlphaOperation(Moose::Graphics::CAlphaTestOperation & op)    { m_AlphaTestOp = op; }
      inline Moose::Graphics::CBlendingOperation  & GetBlendingOperation() { return m_BlendingOp;  }
      inline void SetBlendingOperation(Moose::Graphics::CBlendingOperation  & op) { m_BlendingOp = op;  }

      inline bool IsTransparent() const    { return m_BlendingOp.IsEnabled(); }
      inline bool & GetDepthTest()  { return m_DepthTest; }
      inline void SetDepthTest(bool bFlag)  { m_DepthTest = bFlag; }

      inline bool & GetDepthWrite() { return m_DepthWrite; }
      inline void SetDepthWrite(bool bFlag) { m_DepthWrite = bFlag; }

      inline bool & GetFaceCulling() { return m_FaceCulling; }
      inline void SetFaceCulling(bool bFlag) { m_FaceCulling = bFlag; }

      inline bool & IsLightSource() { return m_bLightSource; }
      inline void SetLightSource(bool bFlag) { m_bLightSource = bFlag; }

      inline Moose::Graphics::CColor4ub & GetBaseColor() { return m_BaseColor; }
      inline void SetBaseColor(const Moose::Graphics::CColor4ub & col) { m_BaseColor = col; }

      inline bool & GetLighting() { return m_bLighting; }
      inline void SetLighting(bool bFlag) { m_bLighting = bFlag; }

      inline LightRenderableList & GetLights() { return m_lstLights; }
      inline unsigned int & GetLightId() { return m_nLightId; }
      inline void SetLightId(unsigned int nVal) { m_nLightId = nVal; }
      void   ParseFrom( NameValueMap & map );
      inline CPolygonOffset & GetPolygonOffset() { return m_PolyOffset; }
      inline void SetPolygonOffset( Moose::Graphics::CPolygonOffset & offset ) { m_PolyOffset = offset; }
      ////////////////////
      /// Returns handle to Texture.
      /// \param nId Which texture unit is handled. By default it is zero, the first.
      /// \returns TEXTURE_HANDLE
      Moose::Default::TEXTURE_HANDLE    & GetTextureHandle( unsigned int nId = 0);
      ////////////////////
      /// Sets texture handle by name.
      void SetTexture( unsigned int nId, const char *szName );

      ////////////////////
      /// Returns handle to shader.
      /// \returns SHADER_HANDLE.
      Moose::Default::SHADER_HANDLE  & GetShaderHandle();
      ////////////////////
      /// Inserts shader parameter with name.
      /// \param sName Parameter name in shaders.
      /// \param pAttrib Shader attrib.
      void		AddShaderAttrib( const char *sName, Moose::Default::VERTEX_HANDLE & handle );
      void		AddShaderAttrib( const char *sName, Moose::Graphics::CVertexDescriptor *pData );
      void		AddShaderUniform( const char *sName, Moose::Default::VERTEX_HANDLE & handle );
      void		AddShaderUniform( const char *sName, Moose::Graphics::CVertexDescriptor *pData );
        
      void		AddShaderUniform( const char *sName, const char *szResourceName );
      void		AddShaderUniform( const char *sName, Moose::Math::CMatrix4x4<float> *pMatrix );
      void		AddShaderUniform( const char *sName, GLint iValue );
      void		AddShaderUniform( const char *sName, GLfloat fValue );
      void		AddShaderUniform( const char *sName, const Moose::Math::CVector4<float> *pData );
      IShaderParam * GetShaderAttrib( const char *szName);
      IShaderParam * GetShaderUniform( const char *szName);
      ////////////////////
      /// Returns reference to shader attribs.
      /// \returns Shader attribute container.
      CShaderParamContainer & GetShaderAttribs();
      ////////////////////
      /// Returns reference to shader uniforms.
      /// \returns Shader uniform container.
      CShaderParamContainer & GetShaderUniforms();
      ////////////////////
      /// Returns reference to view transform param in shader.
      CShaderUniformMat4x4f & GetShaderViewUniform();
      ////////////////////
      /// Returns reference to view transform param in shader.
      CShaderUniformMat4x4f & GetShaderProjectionUniform();
      ////////////////////
      /// Returns reference to view transform param in shader.
      CShaderUniformMat4x4f & GetShaderModelUniform();
        
      ////////////////////
      /// Returns texture filters for given texture.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1.  By default, it is first (zero).
      /// \returns Reference to texture filter vector.
      std::vector<Moose::Graphics::TEXTURE_FILTER> & GetTextureFilters( unsigned int nId = 0 );
      ////////////////////
      /// Adds texture filter to given texture.
      /// \param tTexFilter TEXTURE_FILTER to insert.
      /// \param nId Optional texture number, from 0 to TEXTURE_HANDLE_COUNT-1. By default, it is first (zero).
      void		AddTextureFilter( Moose::Graphics::TEXTURE_FILTER tTexFilter, unsigned int nId = 0 );
      ////////////////////
      /// \returns material reference.
      Moose::Graphics::CMaterial & GetMaterial();
      ////////////////////
      /// Prepares shader attributes and uniform paramters.
      /// \return true on success, false on failure. 
      bool Prepare();
    };
  }
}
///////////////////////////////////////////////////////////////////////////////////
inline unsigned short
Moose::Graphics::CRenderState::GetRenderLayer() const
{
    return m_iRenderLayer;
}
/////////////////////////////////////////////////////////////////
#endif