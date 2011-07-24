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
#include "MooseOGLRenderer.h"
#include <iostream>
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Core;
using namespace std;
#define RegLightParam(NAME) NAME.SetName(#NAME);

///////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderState::CRenderState() :  m_iRenderLayer(kOpaque),
                                                 m_DepthTest(false),
                                                 m_DepthWrite(true),
                                                 m_FaceCulling(false),
                                                 m_bLighting(false),
                                                 m_BaseColor(255,255,255,255)
{
    RegLightParam( m_Lights.diffuse[0] );
    RegLightParam( m_Lights.ambient[0] );
    RegLightParam( m_Lights.specular[0] );
    RegLightParam( m_Lights.position[0] );
    RegLightParam( m_Lights.halfVector[0] );
    RegLightParam( m_Lights.direction[0] );
    RegLightParam( m_Lights.spotAngle[0] );
    RegLightParam( m_Lights.spotExponent[0] );
    RegLightParam( m_Lights.constantAttenuation[0] );
    RegLightParam( m_Lights.linearAttenuation[0] );
    RegLightParam( m_Lights.quadraticAttenuation[0] );
    RegLightParam( m_Lights.enabled[0] );

    RegLightParam( m_Lights.diffuse[1] );
    RegLightParam( m_Lights.ambient[1] );
    RegLightParam( m_Lights.specular[1] );
    RegLightParam( m_Lights.position[1] );
    RegLightParam( m_Lights.halfVector[1] );
    RegLightParam( m_Lights.direction[1] );
    RegLightParam( m_Lights.spotAngle[1] );
    RegLightParam( m_Lights.spotExponent[1] );
    RegLightParam( m_Lights.constantAttenuation[1] );
    RegLightParam( m_Lights.linearAttenuation[1] );
    RegLightParam( m_Lights.quadraticAttenuation[1] );
    RegLightParam( m_Lights.enabled[1] );

    RegLightParam( m_Lights.diffuse[2] );
    RegLightParam( m_Lights.ambient[2] );
    RegLightParam( m_Lights.specular[2] );
    RegLightParam( m_Lights.position[2] );
    RegLightParam( m_Lights.halfVector[2] );
    RegLightParam( m_Lights.direction[2] );
    RegLightParam( m_Lights.spotAngle[2] );
    RegLightParam( m_Lights.spotExponent[2] );
    RegLightParam( m_Lights.constantAttenuation[2] );
    RegLightParam( m_Lights.linearAttenuation[2] );
    RegLightParam( m_Lights.quadraticAttenuation[2] );
    RegLightParam( m_Lights.enabled[2] );

    m_GlobalAmbient.SetName("m_globalAmbient");
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
////////////////////////////////////////////////////////////////////////////////
void 
Moose::Graphics::CRenderState::SetTexture( unsigned int nId, const char *szName )
{
  GetTextureHandle(nId) = szName;
}
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
////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::SetShader( const char *szName )
{
  GetShaderHandle() = szName;
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
void
Moose::Graphics::CRenderState::AddShaderUniform( const char *sName, const Moose::Math::CVector4<float> *pData )
{ 
    CShaderUniformVec4fPtr *pParam = new CShaderUniformVec4fPtr();
    pParam->SetName(sName);
    pParam->SetData( pData );  
    m_ShaderUniforms.Add( pParam ); 
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::ApplyShaderLights()
{
    
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
////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderState::SetGlobalAmbient( Moose::Graphics::CColor4f & color )
{
  m_GlobalAmbient.SetData(&color);
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CShaderUniformVec4fPtr & 
Moose::Graphics::CRenderState::GetGlobalAmbient()
{
  return m_GlobalAmbient;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CShaderUniformMaterial &
Moose::Graphics::CRenderState::GetMaterial()
{
	return m_Material;
}
/////////////////////////////////////////////////////////////////
bool
Moose::Graphics::CRenderState::Prepare()
{
    bool bRetval = true;
    CShader & s = **GetShaderHandle();
    if ( GetShaderHandle().IsNull() == false )
    {
        GetShaderAttribs().Bind( s);
        bRetval = (*GetShaderHandle())->Link();
        GetShaderUniforms().Bind( s);
    
        for(int i=0;i<NUM_LIGHTS;i++)
        {
            m_Lights.diffuse[i].Bind(s,0);
            m_Lights.ambient[i].Bind(s,0) ;
            m_Lights.specular[i].Bind(s,0);
            m_Lights.position[i].Bind(s,0);
            m_Lights.halfVector[i].Bind(s,0);
            m_Lights.direction[i].Bind(s,0);
            m_Lights.spotAngle[i].Bind(s,0);
            m_Lights.spotExponent[i].Bind(s,0);
            m_Lights.constantAttenuation[i].Bind(s,0);
            m_Lights.linearAttenuation[i].Bind(s,0);
            m_Lights.quadraticAttenuation[i].Bind(s,0);
            m_Lights.enabled[i].Bind(s,0);
        }
        m_GlobalAmbient.Bind(s,0);
        m_Material.Bind(s,0);

        m_ViewUniform.SetName("m_viewMatrix");
        m_ViewUniform.Bind( s, 0);  // index 0 is not used in uniform
        m_ProjUniform.SetName("m_projMatrix");
        m_ProjUniform.Bind( s, 0);  // index 0 is not used in uniform
        m_ModelUniform.SetName("m_modelMatrix");
        m_ModelUniform.Bind( s, 0 ); // index 0 is not used in uniform
        
    }
    return bRetval;
}
/////////////////////////////////////////////////////////////////
void 
Moose::Graphics::UniformLights::Apply( Moose::Graphics::COglRenderer & r )
{
  for(int i=0;i<NUM_LIGHTS;i++)
  {
    diffuse[i].Apply(r);
    ambient[i].Apply(r);
    specular[i].Apply(r);
    position[i].Apply(r);
    direction[i].Apply(r);
    spotAngle[i].Apply(r);
    spotExponent[i].Apply(r);
    constantAttenuation[i].Apply(r);
    linearAttenuation[i].Apply(r);
    quadraticAttenuation[i].Apply(r);
    enabled[i].Apply(r);
  }
}
////////////////////////////////////////////////////////////////////////////////            
void 
Moose::Graphics::UniformLights::SetData(int index, CLight & l, COglRenderer & r )
{
  enabled[index].SetData(l.IsEnabled());
  diffuse[index].SetData(&l.GetDiffuseColor());
  ambient[index].SetData(&l.GetAmbientColor());
  specular[index].SetData(&l.GetSpecularColor());
  position[index].SetData(&l.GetPosition());
  direction[index].SetData(&l.GetDirection());
  halfVector[index].SetData((r.GetCurrentCamera()->GetPosition()-l.GetPosition()).GetNormalized());
  spotAngle[index].SetData(l.GetSpotAngle());
  spotExponent[index].SetData(l.GetSpotExponent());
  constantAttenuation[index].SetData(l.GetConstantAttenuation());
  linearAttenuation[index].SetData(l.GetLinearAttenuation());
  quadraticAttenuation[index].SetData(l.GetQuadraticAttenuation());


}
////////////////////////////////////////////////////////////////////////////////

