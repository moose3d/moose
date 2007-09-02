#include "PhoenixModel.h"
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
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
  }

  g_DefaultVertexManager->Release( m_VertexDescriptorHandle);

  for(unsigned int i=0;i<GetIndexHandles().size();i++)
  {
    g_DefaultIndexManager->Release(GetIndexHandles()[i]);
  }

  g_DefaultShaderManager->Release(m_ShaderHandle);
}
/////////////////////////////////////////////////////////////////
TEXTURE_HANDLE
Phoenix::Graphics::CModel::GetTextureHandle( unsigned int nId ) const
{
  if ( nId < TEXTURE_HANDLE_COUNT )
    return m_aTextureHandles[nId];
  else
  {
    // return null handle.
    return TEXTURE_HANDLE();
  }
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetTextureHandle(TEXTURE_HANDLE handle, unsigned int nId) 
{
  if ( nId < TEXTURE_HANDLE_COUNT )
    m_aTextureHandles[nId] = handle; 
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE
Phoenix::Graphics::CModel::GetVertexHandle() const
{
  return m_VertexDescriptorHandle;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetVertexHandle(VERTEX_HANDLE handle) 
{
  m_VertexDescriptorHandle = handle; 
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE
Phoenix::Graphics::CModel::GetNormalHandle() const
{
  return m_VertexNormalHandle;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetNormalHandle(VERTEX_HANDLE handle) 
{
  m_VertexNormalHandle = handle; 
}
/////////////////////////////////////////////////////////////////
std::vector<INDEX_HANDLE> &
Phoenix::Graphics::CModel::GetIndexHandles()
{
  return m_vecIndexArrayHandles;
}
/////////////////////////////////////////////////////////////////
const std::vector<INDEX_HANDLE> &
Phoenix::Graphics::CModel::GetIndexHandles() const
{
  return m_vecIndexArrayHandles;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::AddIndexHandle(INDEX_HANDLE handle) 
{
  m_vecIndexArrayHandles.push_back(handle); 
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetTextureCoordinateHandle( VERTEX_HANDLE handle, unsigned int nId )
{
  if ( nId < TEXTURE_HANDLE_COUNT )
  {
    m_aTextureCoordinateHandles[nId] = handle;
  }
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE    
Phoenix::Graphics::CModel::GetTextureCoordinateHandle( unsigned int nId )
{
  if ( nId < TEXTURE_HANDLE_COUNT )
  {
    return m_aTextureCoordinateHandles[nId];
  }
  // return null handle
  return VERTEX_HANDLE();
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
SHADER_HANDLE	
Phoenix::Graphics::CModel::GetShaderHandle() const
{
  return m_ShaderHandle;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetShaderHandle( SHADER_HANDLE handle )
{
  m_ShaderHandle = handle;
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

  for( unsigned int i=0;i<model.GetIndexHandles().size();i++)
  {
    stream << "INDEX_HANDLE = "  << model.GetIndexHandles()[i].GetIndex();
    stream << (model.GetIndexHandles()[i].IsNull() ? "(null)" : "" ) << endl;
  }
  stream << "SHADER_HANDLE = " << model.m_ShaderHandle.GetIndex();
  stream << (model.m_ShaderHandle.IsNull() ? "(null)" : "" ) << endl;
  return stream;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetShaderParameter( const char *sName, VERTEX_HANDLE handle )
{
  m_vShaderParams.push_back( std::make_pair( string(sName), handle)  );
}
/////////////////////////////////////////////////////////////////
std::vector< std::pair<std::string, VERTEX_HANDLE> > & 
Phoenix::Graphics::CModel::GetShaderParameters() 
{
  return m_vShaderParams;
}
/////////////////////////////////////////////////////////////////
