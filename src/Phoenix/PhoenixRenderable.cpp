#include "PhoenixRenderable.h"
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderable::CRenderable()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderable::~CRenderable()
{
  // nullify all handles
  for(unsigned int i=0;i<TEXTURE_HANDLE_COUNT;i++)
  {
    g_DefaultTextureManager->Release(m_aTextureHandles[i]);
    g_DefaultVertexManager->Release(m_aTextureCoordinateHandles[i]);
  }
  
  g_DefaultVertexManager->Release( m_VertexDescriptorHandle);
  g_DefaultVertexManager->Release( m_VertexNormalHandle );
  g_DefaultVertexManager->Release( GetListIndices() );
  g_DefaultVertexManager->Release( GetStripIndices() );
  
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
Phoenix::Graphics::CRenderable::GetTextureHandle( unsigned int nId ) 
{
  return m_aTextureHandles[nId % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CRenderable::GetVertexHandle() 
{
  return m_VertexDescriptorHandle;
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CRenderable::GetNormalHandle() 
{
  return m_VertexNormalHandle;
}
/////////////////////////////////////////////////////////////////
INDEX_HANDLE &	
Phoenix::Graphics::CRenderable::GetListIndices()
{
  return m_hTriListIndices;
}
/////////////////////////////////////////////////////////////////
INDEX_HANDLE &	
Phoenix::Graphics::CRenderable::GetStripIndices()
{
  return m_hTriStripIndices;
}
/////////////////////////////////////////////////////////////////
// std::vector<INDEX_HANDLE * > &
// Phoenix::Graphics::CRenderable::GetIndexHandles()
// {
//   return m_vecIndexArrayHandles;
// }
// /////////////////////////////////////////////////////////////////
// const std::vector<INDEX_HANDLE * > &
// Phoenix::Graphics::CRenderable::GetIndexHandles() const
// {
//   return m_vecIndexArrayHandles;
// }
/////////////////////////////////////////////////////////////////
// void
// Phoenix::Graphics::CRenderable::AddIndexHandle(INDEX_HANDLE *pHandle) 
// {
//   m_vecIndexArrayHandles.push_back( pHandle); 
// }
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CRenderable::GetTextureCoordinateHandle( unsigned int nId )
{
  return m_aTextureCoordinateHandles[nId % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
vector<TEXTURE_FILTER> & 
Phoenix::Graphics::CRenderable::GetTextureFilters( unsigned int nId )
{
  assert( nId < TEXTURE_HANDLE_COUNT && "nId Over TEXTURE_HANDLE_COUNT");
  return m_aTextureFilters[nId];
}
/////////////////////////////////////////////////////////////////
void   
Phoenix::Graphics::CRenderable::AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId )
{
  if ( nId < TEXTURE_HANDLE_COUNT )
  {
    m_aTextureFilters[nId].push_back(tTexFilter);
  }
}
/////////////////////////////////////////////////////////////////
SHADER_HANDLE &
Phoenix::Graphics::CRenderable::GetShaderHandle() 
{
  return m_ShaderHandle;
}
/////////////////////////////////////////////////////////////////
std::ostream & 
Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderable & model )
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
  stream << "list INDEX_HANDLE = "  << model.GetListIndices().GetIndex();
  stream << (model.GetListIndices().IsNull() ? "(null)" : "" ) << endl;

  stream << "strip INDEX_HANDLE = "  << model.GetStripIndices().GetIndex();
  stream << (model.GetStripIndices().IsNull() ? "(null)" : "" ) << endl;
  //}
  stream << "SHADER_HANDLE = " << model.m_ShaderHandle.GetIndex();
  stream << (model.m_ShaderHandle.IsNull() ? "(null)" : "" ) << endl;
  return stream;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderable::SetShaderParameter( const char *sName, VERTEX_HANDLE *pHandle )
{
  m_vShaderParams.push_back( std::make_pair( string(sName), pHandle)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderable::SetShaderParameter( const char *sName, float fValue )
{
  m_vShaderFloatParams.push_back( std::make_pair( string(sName), fValue)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderable::SetShaderParameter( const char *sName, int iValue )
{
  m_vShaderIntParams.push_back( std::make_pair( string(sName), iValue)  );
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderParams & 
Phoenix::Graphics::CRenderable::GetShaderParameters() 
{
  return m_vShaderParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderIntParams & 
Phoenix::Graphics::CRenderable::GetShaderIntParameters() 
{
  return m_vShaderIntParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderFloatParams & 
Phoenix::Graphics::CRenderable::GetShaderFloatParameters() 
{
  return m_vShaderFloatParams;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderable::SetTransparent( bool bFlag ) 
{
  m_bTransparent = bFlag;
}
/////////////////////////////////////////////////////////////////
bool
Phoenix::Graphics::CRenderable::IsTransparent() const
{
  return m_bTransparent;
}
/////////////////////////////////////////////////////////////////
