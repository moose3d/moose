#include "PhoenixModel.h"
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel::CModel()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel::~CModel()
{
  m_TextureHandle.Nullify();
  m_VertexDescriptorHandle.Nullify();
  m_IndexArrayHandle.Nullify();
}
/////////////////////////////////////////////////////////////////
TEXTURE_HANDLE
Phoenix::Graphics::CModel::GetTextureHandle() const
{
  return m_TextureHandle;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CModel::SetTextureHandle(TEXTURE_HANDLE handle) 
{
  m_TextureHandle = handle; 
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
