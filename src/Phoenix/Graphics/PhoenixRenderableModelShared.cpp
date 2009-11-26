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
Phoenix::Graphics::CRenderableModelShared::CRenderableModelShared()
{

}
/////////////////////////////////////////////////////////////////
Phoenix::Graphics::CRenderableModelShared::~CRenderableModelShared()
{
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
std::ostream &
Phoenix::Graphics::operator<<( std::ostream &stream, const Phoenix::Graphics::CRenderableModelShared & renderable)
{
	CModel *pModel = *const_cast<Phoenix::Graphics::CRenderableModelShared &>(renderable).GetModelHandle();
	if ( pModel )
	{
		stream << *pModel;
	}
  return stream;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CRenderableModelShared::Render( COglRenderer & renderer )
{
	if ( GetModelHandle().IsNull() ) return;

	// set renderstate first, because of lights, for example.
	renderer.CommitRenderState(GetRenderState());
  if ( m_pTransform != NULL )
    renderer.CommitTransform( *m_pTransform );
  ////////////////////
  // Retrieve resources
  COglTexture *pTexture = NULL;
  CVertexDescriptor *pTemp = NULL;

  CModel & model = **m_hModel;
  ////////////////////
  // Commit textures
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    pTemp    = *model.GetTextureCoordinateHandle(i);
    pTexture = *GetRenderState().GetTextureHandle(i);

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
      renderer.CommitFilters( GetRenderState().GetTextureFilters(i), pTexture->GetType() );
    }
    else
      renderer.DisableTexture(i, NULL);
  }


  CShader *pShader = *GetRenderState().GetShaderHandle();
  renderer.CommitShader( pShader );

  if ( !GetRenderState().GetShaderHandle().IsNull())
  {
    CVertexDescriptor *pParam = NULL;
    // Go through all parameters and commit them
    for(unsigned int nSP=0; nSP< GetRenderState().GetShaderParameters().size(); nSP++)
    {
      pParam = *( *GetRenderState().GetShaderParameters()[nSP].second );
      if ( pParam != NULL )
      {
      	renderer.CommitShaderParam( *pShader, GetRenderState().GetShaderParameters()[nSP].first, *pParam );
      }
    }

    // Go through all int parameters and commit them
    {
      ShaderIntParams::iterator it = GetRenderState().GetShaderIntParameters().begin();
      for(; it != GetRenderState().GetShaderIntParameters().end(); it++)
      {
      	renderer.CommitUniformShaderParam( *pShader, it->first, it->second );
      }
    }
    // Go through all float parameters and commit them
    {
      ShaderFloatParams::iterator it = GetRenderState().GetShaderFloatParameters().begin();
      for( ; it != GetRenderState().GetShaderFloatParameters().end(); it++)
      {
      	renderer.CommitUniformShaderParam( *pShader, it->first, it->second );
      }
    }
  }


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


  if ( !model.GetIndices().IsNull() )
    renderer.CommitPrimitive( *model.GetIndices() );

  if ( m_pTransform != NULL )
    renderer.RollbackTransform();
}
/////////////////////////////////////////////////////////////////
MODEL_HANDLE &
Phoenix::Graphics::CRenderableModelShared::GetModelHandle()
{
	return m_hModel;
}
/////////////////////////////////////////////////////////////////

