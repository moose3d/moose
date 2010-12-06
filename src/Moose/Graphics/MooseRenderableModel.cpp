#include "MooseRenderable.h"
#include "MooseRenderableModel.h"
#include "MooseOGLRenderer.h"
#include <assert.h>
#include <string>
/////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using namespace Moose::Graphics;
using namespace Moose::Math;
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderableModel::CRenderableModel() : m_pModel(NULL), m_bDestroyModel(false)
{

}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderableModel::~CRenderableModel()
{
  if ( m_bDestroyModel) delete m_pModel;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
std::ostream &
Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModel & renderable)
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
Moose::Graphics::CRenderableModel::Render( COglRenderer & renderer )
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


  CModel & model = *GetModel();

  
  
  ////////////////////
  // Commit textures
 
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
    //pTemp    = *model.GetTextureCoordinateHandle(i);
    pTexture = *GetRenderState().GetTextureHandle(i);

    
    // check that texture resource exists
    if ( pTexture  != NULL )
    {
      renderer.CommitTexture( i, pTexture );
      renderer.CommitFilters( GetRenderState().GetTextureFilters(i), 
                              pTexture->GetType() );
    }
    else 
    { 
      renderer.DisableTexture(i, NULL);
    }
  } 
  //#endif

  CShader *pShader = *GetRenderState().GetShaderHandle();
  renderer.CommitShader( pShader );

  if ( !GetRenderState().GetShaderHandle().IsNull())
  {
    GetRenderState().GetShaderAttribs().Apply(renderer);
    GetRenderState().GetShaderUniforms().Apply(renderer);
    if ( renderer.GetCurrentCamera() )
    {
      // Update matrices 
      GetRenderState().GetShaderViewUniform().SetData(      &renderer.GetCurrentCamera()->GetViewMatrix());
      GetRenderState().GetShaderProjectionUniform().SetData(&renderer.GetCurrentCamera()->GetProjectionMatrix());

      if ( GetTransform() ) 
      {
        GetRenderState().GetShaderModelUniform().SetData( &GetTransform()->GetMatrix() );
      }
      else GetRenderState().GetShaderModelUniform().SetData( &CMatrix4x4<float>::Identity );

      // Send data to shader
      GetRenderState().GetShaderViewUniform().Apply(renderer);
      GetRenderState().GetShaderProjectionUniform().Apply(renderer);
      GetRenderState().GetShaderModelUniform().Apply(renderer);
    }
  }
  
 
  if ( !model.GetIndices().IsNull() )
    renderer.CommitPrimitive( *model.GetIndices() );

  if ( m_pTransform != NULL )
    renderer.RollbackTransform();
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CModel *
Moose::Graphics::CRenderableModel::GetModel()
{
	return m_pModel;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderableModel::SetModel(Moose::Graphics::CModel *pModel)
{
     m_pModel = pModel;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderableModel::SetDestroyModel( bool bValue )
{
  m_bDestroyModel = bValue;
}
////////////////////////////////////////////////////////////////////////////////
