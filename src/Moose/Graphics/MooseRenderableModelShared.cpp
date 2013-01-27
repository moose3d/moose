/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

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

