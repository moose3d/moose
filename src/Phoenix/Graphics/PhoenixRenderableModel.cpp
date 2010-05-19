#include "PhoenixRenderable.h"
#include "PhoenixRenderableModel.h"
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
Phoenix::Graphics::CRenderableModel::CRenderableModel() :m_pModel(NULL)
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableModel::~CRenderableModel()
{
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderableModel & renderable)
{
	if ( renderable.m_pModel )
	{
		stream << *renderable.m_pModel;
	}
	else stream << "(no model)";
  return stream;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableModel::Render( COglRenderer & renderer )
{
  if ( !GetModel() ) return;

  // set renderstate first, because of lights, for example.
  renderer.CommitRenderState(GetRenderState());
  if ( m_pTransform != NULL ) {
    renderer.CommitTransform( *m_pTransform );
  }
  ////////////////////
  // Retrieve resources
  COglTexture *pTexture = NULL;
  CVertexDescriptor *pTemp = NULL;

  CModel & model = *GetModel();

  
  
  ////////////////////
  // Commit textures
#if !defined(PHOENIX_APPLE_IPHONE)
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    pTemp    = *model.GetTextureCoordinateHandle(i);
    pTexture = *GetRenderState().GetTextureHandle(i);



    // check that texcoord resources actually exist
    if ( pTemp == NULL )
    {
      glClientActiveTexture( GL_TEXTURE0 + i);
      glDisableClientState( GL_TEXTURE_COORD_ARRAY);
    }
    else
    {
      renderer.CommitVertexDescriptor( pTemp, i );
    }
    //// check that texture resource exists
    if ( pTexture  != NULL )
    {
      renderer.CommitTexture( i, pTexture );
      renderer.CommitFilters( GetRenderState().GetTextureFilters(i), pTexture->GetType() );
    }
    else
      renderer.DisableTexture(i, NULL);

    

  }  
#else

  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    //pTemp    = *model.GetTextureCoordinateHandle(i);
    pTexture = *GetRenderState().GetTextureHandle(i);

    
    //// check that texture resource exists
    if ( pTexture  != NULL )
    {
      
      renderer.CommitTexture( i, pTexture );
      renderer.CommitFilters( GetRenderState().GetTextureFilters(i), pTexture->GetType() );
    }
    else 
    { 
      
      renderer.DisableTexture(i, NULL);
    }
  } 
#endif

  CShader *pShader = *GetRenderState().GetShaderHandle();
  renderer.CommitShader( pShader );

  if ( !GetRenderState().GetShaderHandle().IsNull())
  {
    GetRenderState().GetShaderAttribs().Apply();
    GetRenderState().GetShaderUniforms().Apply();
    if ( renderer.GetCurrentCamera() )
    {
      // Update matrices 
      GetRenderState().GetShaderViewUniform().SetData(      &renderer.GetCurrentCamera()->GetViewMatrix());
      GetRenderState().GetShaderProjectionUniform().SetData(&renderer.GetCurrentCamera()->GetProjectionMatrix());
      GetRenderState().GetShaderModelUniform().SetData( &GetTransform()->GetMatrix() );
      // Send data to shader
      GetRenderState().GetShaderViewUniform().Apply();
      GetRenderState().GetShaderProjectionUniform().Apply();
      GetRenderState().GetShaderModelUniform().Apply();
    }
  }
  
  
#if !defined(PHOENIX_APPLE_IPHONE)
  // commit normals
  if ( model.GetNormalHandle().IsNull() )  glDisableClientState( GL_NORMAL_ARRAY );
  else	renderer.CommitVertexDescriptor( *model.GetNormalHandle() );

  // Commit colors
  if ( model.GetColorHandle().IsNull()  )
  {
    glDisableClientState( GL_COLOR_ARRAY );
    renderer.CommitColor( GetRenderState().GetBaseColor() );
  }
  else	renderer.CommitVertexDescriptor( *model.GetColorHandle() );

  // commit position data
  if ( model.GetVertexHandle().IsNull() )	glDisableClientState( GL_VERTEX_ARRAY );
  else	renderer.CommitVertexDescriptor ( *model.GetVertexHandle() );
#endif

  if ( !model.GetIndices().IsNull() )
    renderer.CommitPrimitive( *model.GetIndices() );

  if ( m_pTransform != NULL )
    renderer.RollbackTransform();
}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CModel *
Phoenix::Graphics::CRenderableModel::GetModel()
{
	return m_pModel;
}
/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableModel::SetModel(Phoenix::Graphics::CModel *pModel)
{
     m_pModel = pModel;
}
/////////////////////////////////////////////////////////////////
