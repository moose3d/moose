/*
 * MooseRenderState.cpp
 *
 *  Created on: Nov 6, 2009
 *      Author: entity
 */
#include "MooseRenderState.h"
#include "MooseConvert.h"
#include "MooseLogger.h"
#include "MooseDefaultEntities.h"
#include <iostream>
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Core;
using namespace std;
///////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderState::CRenderState() :  m_iRenderLayer(kOpaque),
                                                    m_DepthTest(false),
																									 m_DepthWrite(true),
																									 m_FaceCulling(false),
																									 m_bLighting(false),
																									 m_bLightSource(false),
																									 m_nLightId(0),
																									 m_BaseColor(255,255,255,255)
{

}
///////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderState::~CRenderState()
{

}
///////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::SetRenderLayer( unsigned short kLayer )
{
    m_iRenderLayer = kLayer;
}
///////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::ParseFrom( Moose::Graphics::NameValueMap & map )
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
Moose::Default::TEXTURE_HANDLE &
Moose::Graphics::CRenderState::GetTextureHandle( unsigned int nId )
{
  return m_aTextureHandles[nId % TEXTURE_HANDLE_COUNT];
}
///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
vector<Moose::Graphics::TEXTURE_FILTER> &
Moose::Graphics::CRenderState::GetTextureFilters( unsigned int nId )
{
  assert( nId < TEXTURE_HANDLE_COUNT && "nId Over TEXTURE_HANDLE_COUNT");
  return m_aTextureFilters[nId];
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId )
{
  if ( nId < TEXTURE_HANDLE_COUNT )
  {
    m_aTextureFilters[nId].push_back(tTexFilter);
  }
}
/////////////////////////////////////////////////////////////////
Moose::Default::SHADER_HANDLE &
Moose::Graphics::CRenderState::GetShaderHandle()
{
  return m_ShaderHandle;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderAttrib( const char *sName, Moose::Default::VERTEX_HANDLE & handle )
{ 
  CShaderAttrib *pParam = new CShaderAttrib();
  pParam->SetName(sName);
  pParam->SetData( handle );  
  m_ShaderAttribs.Add( pParam ); 
  
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderAttrib( const char *sName, Moose::Graphics::CVertexDescriptor *pData )
{ 
    CShaderAttribPtr *pParam = new CShaderAttribPtr();
    pParam->SetName(sName);
    pParam->SetData( pData );  
    m_ShaderAttribs.Add( pParam ); 
    
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderUniform( const char *sName, Moose::Default::VERTEX_HANDLE & handle )
{ 
    CShaderUniform *pParam = new CShaderUniform();
    pParam->SetName(sName);
    pParam->SetData( handle );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderUniform( const char *sName, Moose::Graphics::CVertexDescriptor *pData )
{ 
    CShaderUniformPtr *pParam = new CShaderUniformPtr();
    pParam->SetName(sName);
    pParam->SetData( pData );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderUniform( const char *sName, const char *szResourceName )
{ 
    CShaderUniform *pParam = new CShaderUniform();
    pParam->SetName(sName);
    pParam->SetData( szResourceName );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderUniform( const char *sName, GLint iValue )
{ 
    CShaderUniformInt *pParam = new CShaderUniformInt();
    pParam->SetName(sName);
    pParam->SetData( iValue );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderUniform( const char *sName, GLfloat fValue )
{ 
    CShaderUniformFloat *pParam = new CShaderUniformFloat();
    pParam->SetName(sName);
    pParam->SetData( fValue );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::AddShaderUniform( const char *sName, Moose::Math::CMatrix4x4<float> *pMatrix )
{ 
    CShaderUniformMat4x4f *pParam = new CShaderUniformMat4x4f();
    pParam->SetName(sName);
    pParam->SetData( pMatrix );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
IShaderParam * 
Moose::Graphics::CRenderState::GetShaderAttrib( const char *szName)
{
    return m_ShaderAttribs.GetParameter(szName);
}
/////////////////////////////////////////////////////////////////
IShaderParam * 
Moose::Graphics::CRenderState::GetShaderUniform( const char *szName)
{
    return m_ShaderUniforms.GetParameter(szName);
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CShaderParamContainer &
Moose::Graphics::CRenderState::GetShaderAttribs()
{
  return m_ShaderAttribs;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CShaderParamContainer &
Moose::Graphics::CRenderState::GetShaderUniforms()
{
    return m_ShaderUniforms;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CShaderUniformMat4x4f &
Moose::Graphics::CRenderState::GetShaderViewUniform()
{
    return m_ViewUniform;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CShaderUniformMat4x4f &
Moose::Graphics::CRenderState::GetShaderProjectionUniform()
{
    return m_ProjUniform;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CShaderUniformMat4x4f &
Moose::Graphics::CRenderState::GetShaderModelUniform()
{
    return m_ModelUniform;
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CMaterial &
Moose::Graphics::CRenderState::GetMaterial()
{
	return m_Material;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::CRenderState::Prepare()
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
