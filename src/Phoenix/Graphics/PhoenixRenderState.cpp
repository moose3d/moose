/*
 * PhoenixRenderState.cpp
 *
 *  Created on: Nov 6, 2009
 *      Author: entity
 */
#include "PhoenixRenderState.h"
#include "PhoenixConvert.h"
#include "PhoenixLogger.h"
#include "PhoenixDefaultEntities.h"
#include <iostream>
#include <cassert>
using namespace Phoenix::Graphics;
using namespace Phoenix::Core;
using namespace std;
///////////////////////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderState::CRenderState() :  m_DepthTest(false),
																									 m_DepthWrite(true),
																									 m_FaceCulling(false),
																									 m_bLighting(false),
																									 m_bLightSource(false),
																									 m_nLightId(0),
																									 m_BaseColor(255,255,255,255)
{

}
///////////////////////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderState::~CRenderState()
{

}
///////////////////////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::ParseFrom( Phoenix::Graphics::NameValueMap & map )
{
	while( !map.empty() )
	{
		const std::string & key = map.begin()->first;
		const std::string & value = map.begin()->second;
		if      ( key == "UseLighting" ) GetLighting()   = convertTo<bool>( value );
		else if ( key == "DepthTest"  ) GetDepthTest()   = convertTo<bool>( value );
		else if ( key == "DepthWrite"  ) GetDepthWrite() = convertTo<bool>( value );
		else if ( key == "FaceCulling"  ) GetFaceCulling() = convertTo<bool>( value );
		else if ( key == "AlphaOperation") g_Error << "Alpha Not implemented" << endl;
		else if ( key == "BlendingOperation") g_Error << "Blending Not implemented" << endl;

		map.erase(map.begin());
	}
}
///////////////////////////////////////////////////////////////////////////////////
Phoenix::Default::TEXTURE_HANDLE &
Phoenix::Graphics::CRenderState::GetTextureHandle( unsigned int nId )
{
  return m_aTextureHandles[nId % TEXTURE_HANDLE_COUNT];
}
///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
vector<Phoenix::Graphics::TEXTURE_FILTER> &
Phoenix::Graphics::CRenderState::GetTextureFilters( unsigned int nId )
{
  assert( nId < TEXTURE_HANDLE_COUNT && "nId Over TEXTURE_HANDLE_COUNT");
  return m_aTextureFilters[nId];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId )
{
  if ( nId < TEXTURE_HANDLE_COUNT )
  {
    m_aTextureFilters[nId].push_back(tTexFilter);
  }
}
/////////////////////////////////////////////////////////////////
Phoenix::Default::SHADER_HANDLE &
Phoenix::Graphics::CRenderState::GetShaderHandle()
{
  return m_ShaderHandle;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderAttrib( const char *sName, Phoenix::Default::VERTEX_HANDLE & handle )
{ 
  CShaderAttrib *pParam = new CShaderAttrib();
  pParam->SetName(sName);
  pParam->SetData( handle );  
  m_ShaderAttribs.Add( pParam ); 
  
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderAttrib( const char *sName, Phoenix::Graphics::CVertexDescriptor *pData )
{ 
    CShaderAttribPtr *pParam = new CShaderAttribPtr();
    pParam->SetName(sName);
    pParam->SetData( pData );  
    m_ShaderAttribs.Add( pParam ); 
    
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderUniform( const char *sName, Phoenix::Default::VERTEX_HANDLE & handle )
{ 
    CShaderUniform *pParam = new CShaderUniform();
    pParam->SetName(sName);
    pParam->SetData( handle );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderUniform( const char *sName, Phoenix::Graphics::CVertexDescriptor *pData )
{ 
    CShaderUniformPtr *pParam = new CShaderUniformPtr();
    pParam->SetName(sName);
    pParam->SetData( pData );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderUniform( const char *sName, const char *szResourceName )
{ 
    CShaderUniform *pParam = new CShaderUniform();
    pParam->SetName(sName);
    pParam->SetData( szResourceName );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderUniform( const char *sName, GLint iValue )
{ 
    CShaderUniformInt *pParam = new CShaderUniformInt();
    pParam->SetName(sName);
    pParam->SetData( iValue );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderUniform( const char *sName, GLfloat fValue )
{ 
    CShaderUniformFloat *pParam = new CShaderUniformFloat();
    pParam->SetName(sName);
    pParam->SetData( fValue );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderUniform( const char *sName, Phoenix::Math::CMatrix4x4<float> *pMatrix )
{ 
    CShaderUniformMat4x4f *pParam = new CShaderUniformMat4x4f();
    pParam->SetName(sName);
    pParam->SetData( pMatrix );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
IShaderParam * 
Phoenix::Graphics::CRenderState::GetShaderAttrib( const char *szName)
{
    return m_ShaderAttribs.GetParameter(szName);
}
/////////////////////////////////////////////////////////////////
IShaderParam * 
Phoenix::Graphics::CRenderState::GetShaderUniform( const char *szName)
{
    return m_ShaderUniforms.GetParameter(szName);
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShaderParamContainer &
Phoenix::Graphics::CRenderState::GetShaderAttribs()
{
  return m_ShaderAttribs;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShaderParamContainer &
Phoenix::Graphics::CRenderState::GetShaderUniforms()
{
    return m_ShaderUniforms;
}
////////////////////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShaderUniformMat4x4f &
Phoenix::Graphics::CRenderState::GetShaderViewUniform()
{
    return m_ViewUniform;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShaderUniformMat4x4f &
Phoenix::Graphics::CRenderState::GetShaderProjectionUniform()
{
    return m_ProjUniform;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CShaderUniformMat4x4f &
Phoenix::Graphics::CRenderState::GetShaderModelUniform()
{
    return m_ModelUniform;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CMaterial &
Phoenix::Graphics::CRenderState::GetMaterial()
{
	return m_Material;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Graphics::CRenderState::Prepare()
{
    bool bRetval = true;
    if ( GetShaderHandle().IsNull() == false )
    {
        GetShaderAttribs().Bind( **GetShaderHandle());
        bRetval = (*GetShaderHandle())->Link();
        GetShaderUniforms().Bind( **GetShaderHandle());
        
        m_ViewUniform.SetName("m_viewMatrix");
        m_ViewUniform.Bind( **GetShaderHandle(), 0);  // index 0 is not used in uniform
        m_ProjUniform.SetName("m_projMatrix");
        m_ProjUniform.Bind( **GetShaderHandle(), 0);  // index 0 is not used in uniform
        m_ModelUniform.SetName("m_modelMatrix");
        m_ModelUniform.Bind( **GetShaderHandle(), 0 ); // index 0 is not used in uniform
        
    }
    return bRetval;
}
/////////////////////////////////////////////////////////////////
