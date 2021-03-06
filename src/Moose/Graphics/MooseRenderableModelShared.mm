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
Moose::Graphics::CRenderableModelShared::CRenderableModelShared()
{

}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CRenderableModelShared::~CRenderableModelShared()
{
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
std::ostream &
Moose::Graphics::operator<<( std::ostream &stream, const Moose::Graphics::CRenderableModelShared & renderable)
{
	CModel *pModel = *const_cast<Moose::Graphics::CRenderableModelShared &>(renderable).GetModelHandle();
	if ( pModel )
	{
		stream << *pModel;
	}
  return stream;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRenderableModelShared::Render( COglRenderer & renderer )
{
   if ( GetModelHandle().IsNull() ) return;

    // set renderstate first, because of lights, for example.
    renderer.CommitRenderState(GetRenderState());
  if ( m_pTransform != NULL )
    renderer.CommitTransform( *m_pTransform );
  ////////////////////
  // Retrieve resources
  COglTexture *pTexture = NULL;
  //CVertexDescriptor *pTemp = NULL;

  CModel & model = **m_hModel;
  
  for( unsigned int i=0; i<TEXTURE_HANDLE_COUNT; i++)
  {
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


  CShader *pShader = *GetRenderState().GetShaderHandle();
  renderer.CommitShader( pShader );

  if ( !GetRenderState().GetShaderHandle().IsNull())
  {
    GetRenderState().GetShaderAttribs().Apply(renderer);
    GetRenderState().GetShaderUniforms().Apply(renderer);

    // Setup all lights (3) for shaders
    LightList::iterator it = GetRenderState().GetLights().begin();

    for( int i=0; i < 3; it++,i++ )
    {
      if (it != GetRenderState().GetLights().end())
        GetRenderState().GetUniformLights().SetData(i,**it, renderer);
      else // disable lights that do not have parameters set
        GetRenderState().GetUniformLights().enabled[i].SetData(0);
    }

    GetRenderState().GetUniformLights().Apply(renderer);
    // Set global ambient (from Scene)
    GetRenderState().GetGlobalAmbient().SetData( renderer.GetRenderState().m_pGlobalAmbient);
    GetRenderState().GetGlobalAmbient().Apply(renderer);
    GetRenderState().GetMaterial().Apply(renderer);

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

  GetRenderState().GetShaderAttribs().Disable(renderer);
  if ( m_pTransform != NULL )
    renderer.RollbackTransform();
}
/////////////////////////////////////////////////////////////////
Moose::Default::MODEL_HANDLE &
Moose::Graphics::CRenderableModelShared::GetModelHandle()
{
	return m_hModel;
}
/////////////////////////////////////////////////////////////////

