#include "PhoenixModel.h"
#include <assert.h>
#include <string>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel::CModel()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel::CModel( const CModel & model )
{
	// Handles are smart, and know how to manage themselves.
	for( int i = 0; i<TEXTURE_HANDLE_COUNT; i++) {
	     //m_aTextureHandles[i] 					= model.m_aTextureHandles[i];
	     m_aTextureCoordinateHandles[i] = model.m_aTextureCoordinateHandles[i];
	     //m_aTextureFilters[i] 					= model.m_aTextureFilters[i];
	}

	m_VertexDescriptorHandle = model.m_VertexDescriptorHandle;
	m_VertexNormalHandle = model.m_VertexNormalHandle;
	m_VertexColorHandle = model.m_VertexColorHandle;
	m_hIndices = model.m_hIndices;
/*
	m_ShaderHandle = model.m_ShaderHandle;

	// Shader parameters have pointer, so they need more mangling
	ShaderParams::const_iterator it = model.m_vShaderParams.begin();
	m_vShaderParams.reserve( model.m_vShaderParams.size());
	// for each shader param in passed model
	for( ; it != model.m_vShaderParams.end(); it++ )
	{
		// add new shader param with empty vertex handle to our model with same name
		m_vShaderParams.push_back( std::make_pair(it->first, new VERTEX_HANDLE) );
		// assign parameter handle to our new handle from existing one
		(*m_vShaderParams.back().second) = *(it->second);

	}

	m_vShaderIntParams = model.m_vShaderIntParams;
	m_vShaderFloatParams = model.m_vShaderFloatParams;
*/
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel::~CModel()
{
  // nullify all handles
  for(unsigned int i=0;i<TEXTURE_HANDLE_COUNT;i++)
  {
    //g_DefaultTextureManager->Release(m_aTextureHandles[i]);
    g_DefaultVertexManager->Release(m_aTextureCoordinateHandles[i]);
  }
  g_DefaultVertexManager->Release( m_VertexDescriptorHandle);
  g_DefaultVertexManager->Release( m_VertexColorHandle);
  g_DefaultVertexManager->Release( m_VertexNormalHandle );
  g_DefaultIndexManager->Release( GetIndices() );


  //g_DefaultShaderManager->Release(m_ShaderHandle);


}
/////////////////////////////////////////////////////////////////
Phoenix::Default::VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetVertexHandle()
{
  return m_VertexDescriptorHandle;
}
/////////////////////////////////////////////////////////////////
Phoenix::Default::VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetNormalHandle()
{
  return m_VertexNormalHandle;
}
/////////////////////////////////////////////////////////////////
Phoenix::Default::VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetColorHandle()
{
  return m_VertexColorHandle;
}
/////////////////////////////////////////////////////////////////
Phoenix::Default::INDEX_HANDLE &
Phoenix::Graphics::CModel::GetIndices()
{
  return m_hIndices;
}
/////////////////////////////////////////////////////////////////
Phoenix::Default::VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetTextureCoordinateHandle( unsigned int nId )
{
  return m_aTextureCoordinateHandles[nId % TEXTURE_HANDLE_COUNT];
}

/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CModel & model )
{
  /*for( int i =0;i<TEXTURE_HANDLE_COUNT; i++)
  {
    stream << "TEXTURE_HANDLE " << i << " = " << model.m_aTextureHandles[i].GetIndex();
    stream << (model.m_aTextureHandles[i].IsNull() ? "(null)" : "" ) << endl;
  }*/
  for( int i =0;i<TEXTURE_HANDLE_COUNT; i++)
  {
    stream << "TEXTURE_COORD_HANDLE " << i << " = " << model.m_aTextureCoordinateHandles[i].GetIndex();
    stream << (model.m_aTextureCoordinateHandles[i].IsNull() ? "(null)" : "" ) << endl;
  }

  /*for( int i =0;i<TEXTURE_HANDLE_COUNT; i++)
  {
    stream << "TEXTURE_FILTERS for "<< i << " : " << endl;
    for( unsigned int k = 0; k < model.m_aTextureFilters[i].size(); k++)
    {
      stream << model.m_aTextureFilters[i][k] << endl;
    }
  }
  */
  stream << "VERTEX_HANDLE = " << model.m_VertexDescriptorHandle.GetIndex();
  stream << (model.m_VertexDescriptorHandle.IsNull() ? "(null)" : "" ) << endl;

  //for( unsigned int i=0;i<model.GetIndexHandles().size();i++)
  //{
  stream << "INDEX_HANDLE = "  << model.m_hIndices.GetIndex();
  stream << (model.m_hIndices.IsNull() ? "(null)" : "" ) << endl;


  //}
  //stream << "SHADER_HANDLE = " << model.m_ShaderHandle.GetIndex();
  //stream << (model.m_ShaderHandle.IsNull() ? "(null)" : "" ) << endl;
  return stream;
}

