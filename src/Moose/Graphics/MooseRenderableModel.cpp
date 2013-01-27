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
  GetRenderState().GetShaderAttribs().Disable(renderer);
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
