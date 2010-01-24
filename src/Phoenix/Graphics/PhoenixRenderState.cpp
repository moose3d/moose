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

	// Releases dynamically allocatated VERTEX_HANDLEs
	for(unsigned int i=0;i<m_vShaderParams.size();i++)
	{
		g_VertexMgr->Release( *m_vShaderParams[i].second);
		delete m_vShaderParams[i].second;
	}
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
Phoenix::Graphics::CRenderState::AddShaderParameter( const char *sName, Phoenix::Default::VERTEX_HANDLE *pHandle )
{
  m_vShaderParams.push_back( std::make_pair( string(sName), pHandle)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderParameter( const char *sName, float fValue )
{
  m_vShaderFloatParams.insert( std::make_pair( string(sName), fValue)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderState::AddShaderParameter( const char *sName, int iValue )
{
  m_vShaderIntParams.insert( std::make_pair( string(sName), iValue)  );
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderParams &
Phoenix::Graphics::CRenderState::GetShaderParameters()
{
  return m_vShaderParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderIntParams &
Phoenix::Graphics::CRenderState::GetShaderIntParameters()
{
  return m_vShaderIntParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderFloatParams &
Phoenix::Graphics::CRenderState::GetShaderFloatParameters()
{
  return m_vShaderFloatParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CMaterial &
Phoenix::Graphics::CRenderState::GetMaterial()
{
	return m_Material;
}
/////////////////////////////////////////////////////////////////

