#include "PhoenixModel.h"
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
