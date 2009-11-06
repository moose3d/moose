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
Phoenix::Graphics::CModel::~CModel()
{
  // nullify all handles
  for(unsigned int i=0;i<TEXTURE_HANDLE_COUNT;i++)
  {
    g_DefaultTextureManager->Release(m_aTextureHandles[i]);
    g_DefaultVertexManager->Release(m_aTextureCoordinateHandles[i]);
  }
  g_DefaultVertexManager->Release( m_VertexDescriptorHandle);
  g_DefaultVertexManager->Release( m_VertexColorHandle);
  g_DefaultVertexManager->Release( m_VertexNormalHandle );
  g_DefaultIndexManager->Release( GetIndices() );


  g_DefaultShaderManager->Release(m_ShaderHandle);

  // Releases dynamically allocatated VERTEX_HANDLEs
  for(unsigned int i=0;i<m_vShaderParams.size();i++)
  {
    g_DefaultVertexManager->Release( *m_vShaderParams[i].second);
    delete m_vShaderParams[i].second;
  }
}
/////////////////////////////////////////////////////////////////
TEXTURE_HANDLE &
Phoenix::Graphics::CModel::GetTextureHandle( unsigned int nId )
{
  return m_aTextureHandles[nId % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetVertexHandle()
{
  return m_VertexDescriptorHandle;
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetNormalHandle()
{
  return m_VertexNormalHandle;
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetColorHandle()
{
  return m_VertexColorHandle;
}
/////////////////////////////////////////////////////////////////
INDEX_HANDLE &
Phoenix::Graphics::CModel::GetIndices()
{
  return m_hIndices;
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CModel::GetTextureCoordinateHandle( unsigned int nId )
{
  return m_aTextureCoordinateHandles[nId % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
vector<TEXTURE_FILTER> &
Phoenix::Graphics::CModel::GetTextureFilters( unsigned int nId )
{
  assert( nId < TEXTURE_HANDLE_COUNT && "nId Over TEXTURE_HANDLE_COUNT");
  return m_aTextureFilters[nId];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId )
{
  if ( nId < TEXTURE_HANDLE_COUNT )
  {
    m_aTextureFilters[nId].push_back(tTexFilter);
  }
}
/////////////////////////////////////////////////////////////////
SHADER_HANDLE &
Phoenix::Graphics::CModel::GetShaderHandle()
{
  return m_ShaderHandle;
}
/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CModel & model )
{
  for( int i =0;i<TEXTURE_HANDLE_COUNT; i++)
  {
    stream << "TEXTURE_HANDLE " << i << " = " << model.m_aTextureHandles[i].GetIndex();
    stream << (model.m_aTextureHandles[i].IsNull() ? "(null)" : "" ) << endl;
  }
  for( int i =0;i<TEXTURE_HANDLE_COUNT; i++)
  {
    stream << "TEXTURE_COORD_HANDLE " << i << " = " << model.m_aTextureCoordinateHandles[i].GetIndex();
    stream << (model.m_aTextureCoordinateHandles[i].IsNull() ? "(null)" : "" ) << endl;
  }

  for( int i =0;i<TEXTURE_HANDLE_COUNT; i++)
  {
    stream << "TEXTURE_FILTERS for "<< i << " : " << endl;
    for( unsigned int k = 0; k < model.m_aTextureFilters[i].size(); k++)
    {
      stream << model.m_aTextureFilters[i][k] << endl;
    }
  }
  stream << "VERTEX_HANDLE = " << model.m_VertexDescriptorHandle.GetIndex();
  stream << (model.m_VertexDescriptorHandle.IsNull() ? "(null)" : "" ) << endl;

  //for( unsigned int i=0;i<model.GetIndexHandles().size();i++)
  //{
  stream << "INDEX_HANDLE = "  << model.m_hIndices.GetIndex();
  stream << (model.m_hIndices.IsNull() ? "(null)" : "" ) << endl;


  //}
  stream << "SHADER_HANDLE = " << model.m_ShaderHandle.GetIndex();
  stream << (model.m_ShaderHandle.IsNull() ? "(null)" : "" ) << endl;
  return stream;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::AddShaderParameter( const char *sName, VERTEX_HANDLE *pHandle )
{
  m_vShaderParams.push_back( std::make_pair( string(sName), pHandle)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::AddShaderParameter( const char *sName, float fValue )
{
  m_vShaderFloatParams.insert( std::make_pair( string(sName), fValue)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::AddShaderParameter( const char *sName, int iValue )
{
  m_vShaderIntParams.insert( std::make_pair( string(sName), iValue)  );
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderParams &
Phoenix::Graphics::CModel::GetShaderParameters()
{
  return m_vShaderParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderIntParams &
Phoenix::Graphics::CModel::GetShaderIntParameters()
{
  return m_vShaderIntParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderFloatParams &
Phoenix::Graphics::CModel::GetShaderFloatParameters()
{
  return m_vShaderFloatParams;
}
/////////////////////////////////////////////////////////////////

