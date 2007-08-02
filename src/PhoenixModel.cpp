#include "PhoenixModel.h"
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
  for(unsigned int i=0;i<8;i++)
  {
    m_aTextureHandles[i].Nullify();
  }
  m_VertexDescriptorHandle.Nullify();
  m_IndexArrayHandle.Nullify();
  m_ShaderHandle.Nullify();
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
INDEX_HANDLE
Phoenix::Graphics::CModel::GetIndexHandle() const
{
  return m_IndexArrayHandle;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetIndexHandle(INDEX_HANDLE handle) 
{
  m_IndexArrayHandle = handle; 
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
  stream << "INDEX_HANDLE = "  << model.m_IndexArrayHandle.GetIndex();
  stream << (model.m_IndexArrayHandle.IsNull() ? "(null)" : "" ) << endl;
  stream << "SHADER_HANDLE = " << model.m_ShaderHandle.GetIndex();
  stream << (model.m_ShaderHandle.IsNull() ? "(null)" : "" ) << endl;
  return stream;
}
