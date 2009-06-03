#include "PhoenixRenderable.h"
#include "PhoenixOGLRenderer.h"
#include <assert.h>
#include <string>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using namespace Phoenix::Graphics;
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableModel::CRenderableModel()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableModel::~CRenderableModel()
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
Phoenix::Graphics::CRenderableModel::GetTextureHandle( unsigned int nId )
{
  return m_aTextureHandles[nId % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CRenderableModel::GetVertexHandle()
{
  return m_VertexDescriptorHandle;
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CRenderableModel::GetNormalHandle()
{
  return m_VertexNormalHandle;
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CRenderableModel::GetColorHandle()
{
  return m_VertexColorHandle;
}
/////////////////////////////////////////////////////////////////
INDEX_HANDLE &
Phoenix::Graphics::CRenderableModel::GetIndices()
{
  return m_hIndices;
}
/////////////////////////////////////////////////////////////////
VERTEX_HANDLE &
Phoenix::Graphics::CRenderableModel::GetTextureCoordinateHandle( unsigned int nId )
{
  return m_aTextureCoordinateHandles[nId % TEXTURE_HANDLE_COUNT];
}
/////////////////////////////////////////////////////////////////
vector<TEXTURE_FILTER> &
Phoenix::Graphics::CRenderableModel::GetTextureFilters( unsigned int nId )
{
  assert( nId < TEXTURE_HANDLE_COUNT && "nId Over TEXTURE_HANDLE_COUNT");
  return m_aTextureFilters[nId];
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableModel::AddTextureFilter( TEXTURE_FILTER tTexFilter, unsigned int nId )
{
  if ( nId < TEXTURE_HANDLE_COUNT )
  {
    m_aTextureFilters[nId].push_back(tTexFilter);
  }
}
/////////////////////////////////////////////////////////////////
SHADER_HANDLE &
Phoenix::Graphics::CRenderableModel::GetShaderHandle()
{
  return m_ShaderHandle;
}
/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderableModel & model )
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
Phoenix::Graphics::CRenderableModel::AddShaderParameter( const char *sName, VERTEX_HANDLE *pHandle )
{
  m_vShaderParams.push_back( std::make_pair( string(sName), pHandle)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableModel::AddShaderParameter( const char *sName, float fValue )
{
  m_vShaderFloatParams.insert( std::make_pair( string(sName), fValue)  );
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableModel::AddShaderParameter( const char *sName, int iValue )
{
  m_vShaderIntParams.insert( std::make_pair( string(sName), iValue)  );
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderParams &
Phoenix::Graphics::CRenderableModel::GetShaderParameters()
{
  return m_vShaderParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderIntParams &
Phoenix::Graphics::CRenderableModel::GetShaderIntParameters()
{
  return m_vShaderIntParams;
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::ShaderFloatParams &
Phoenix::Graphics::CRenderableModel::GetShaderFloatParameters()
{
  return m_vShaderFloatParams;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableModel::Render( COglRenderer & renderer )
{
  if ( m_pTransform != NULL )
    renderer.CommitTransform( *m_pTransform );
  ////////////////////
  // Retrieve resources
  COglTexture *pTexture = NULL;
  CVertexDescriptor *pTemp = NULL;
  // set renderstate
  renderer.CommitRenderState(GetRenderState());

  ////////////////////
  // Commit textures
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    pTemp    = *GetTextureCoordinateHandle(i);
    pTexture = *GetTextureHandle(i);

    // check that texcoord resources actually exist
    if ( pTemp == NULL )
    {
      glClientActiveTextureARB( GL_TEXTURE0_ARB + i);
      glDisableClientState( GL_TEXTURE_COORD_ARRAY);
    }
    else
    {
      renderer.CommitVertexDescriptor( pTemp, i );
    }
    // check that texture resource exists
    if ( pTexture  != NULL )
    {
      renderer.CommitTexture( i, pTexture );
      renderer.CommitFilters( GetTextureFilters(i), pTexture->GetType() );
    }
    else
      renderer.DisableTexture(i, NULL);
  }


  CShader *pShader = *GetShaderHandle();
  renderer.CommitShader( pShader );

  if ( !GetShaderHandle().IsNull())
  {
    CVertexDescriptor *pParam = NULL;
    // Go through all parameters and commit them
    for(unsigned int nSP=0; nSP< GetShaderParameters().size(); nSP++)
    {
      pParam = *( *GetShaderParameters()[nSP].second );
      if ( pParam != NULL )
      {
	renderer.CommitShaderParam( *pShader, GetShaderParameters()[nSP].first, *pParam );
      }
    }

    // Go through all int parameters and commit them
    {
      ShaderIntParams::iterator it = GetShaderIntParameters().begin();
      for(; it != GetShaderIntParameters().end(); it++)
      {
	renderer.CommitUniformShaderParam( *pShader, it->first, it->second );
      }
    }
    // Go through all float parameters and commit them
    {
      ShaderFloatParams::iterator it = GetShaderFloatParameters().begin();
      for( ; it != GetShaderFloatParameters().end(); it++)
      {
	renderer.CommitUniformShaderParam( *pShader, it->first, it->second );
      }
    }
  }


  // commit normals
  if ( GetNormalHandle().IsNull() )  glDisableClientState( GL_NORMAL_ARRAY );
  else	renderer.CommitVertexDescriptor( *GetNormalHandle() );

  // Commit colors
  if ( GetColorHandle().IsNull()  )
  {
    glDisableClientState( GL_COLOR_ARRAY );
    renderer.CommitColor( GetRenderState().GetBaseColor() );
  }
  else	renderer.CommitVertexDescriptor( *GetColorHandle() );

  // commit position data
  if ( GetVertexHandle().IsNull() )	glDisableClientState( GL_VERTEX_ARRAY );
  else	renderer.CommitVertexDescriptor ( *GetVertexHandle() );


  if ( !GetIndices().IsNull() )
    renderer.CommitPrimitive( *GetIndices() );

  if ( m_pTransform != NULL )
    renderer.RollbackTransform();
}
/////////////////////////////////////////////////////////////////

