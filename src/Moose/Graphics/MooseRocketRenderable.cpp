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

#include "MooseRocketRenderable.h"
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CRocketRenderable::CRocketRenderable( Rocket::Core::Context *pContext ) : m_pContext(pContext)
{
    GetRenderState().GetShaderHandle() = "moose_gui_shader";
    // Add necessary shader attributes
    CShaderAttribRocketPos *pParamPos =  new CShaderAttribRocketPos();
    pParamPos->SetName("a_vertex");
    GetRenderState().GetShaderAttribs().Add( pParamPos ); 

    CShaderAttribRocketColor *pParamCol =  new CShaderAttribRocketColor();
    pParamCol->SetName("a_color");
    GetRenderState().GetShaderAttribs().Add( pParamCol ); 

    CShaderAttribRocketTexCoord *pParamTC =  new CShaderAttribRocketTexCoord();
    pParamTC->SetName("a_texcoord");
    GetRenderState().GetShaderAttribs().Add( pParamTC ); 

    CShaderUniformVec2f *pTranslation = new CShaderUniformVec2f();
    pTranslation->SetName("a_position");
    GetRenderState().GetShaderUniforms().Add(pTranslation);
    
    GetRenderState().AddShaderUniform("diffusetex",0);

    GetRenderState().GetBlendingOperation().SetDefaultTransparency();
    GetRenderState().GetBlendingOperation().SetEnabled(true);
    GetRenderState().SetDepthWrite(true);
    GetRenderState().SetDepthTest(false);
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CRocketRenderable::Render( Moose::Graphics::COglRenderer & r )
{
    m_pContext->Render();
}
////////////////////////////////////////////////////////////////////////////////
