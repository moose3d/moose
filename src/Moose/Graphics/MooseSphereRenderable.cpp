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

/*
 *  CSphereRenderable.cpp
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MooseSphereRenderable.h"
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Math;
namespace prefix=Moose::Graphics;
///////////////////////////////////////////////////////////
prefix::CSphereRenderable::CSphereRenderable() : m_Vertices(ELEMENT_TYPE_VERTEX_3F,48), 
                                                 m_Color( ELEMENT_TYPE_UNIFORM_4F,1)
{
    
    SetColor(1.0, 0.0, 0.0, 1.0);
    GetRenderState().AddShaderAttrib("a_vertex", &m_Vertices);
    GetRenderState().GetShaderHandle() = "moose_color_shader";
    assert( GetRenderState().GetShaderHandle().IsNull() == false);
    GetRenderState().AddShaderUniform("color", &m_Color);
    GetRenderState().SetDepthTest(true);
    GetRenderState().SetDepthWrite(true);
    
}
//////////////////////////////////////////////////////////////////////////
void 
prefix::CSphereRenderable::SetColor( float fR, float fG, float fB, float fA)
{
    m_Color.GetPointer<float>()[0] = fR;
    m_Color.GetPointer<float>()[1] = fG;
    m_Color.GetPointer<float>()[2] = fB;
    m_Color.GetPointer<float>()[3] = fA; 
}
//////////////////////////////////////////////////////////////////////////
void
prefix::CSphereRenderable::Render( COglRenderer & r )
{
    r.CommitRenderState(GetRenderState());
    if ( m_pTransform != NULL ) {
        r.CommitTransform( *m_pTransform );
    }
    
    CShader *pShader = *GetRenderState().GetShaderHandle();
    r.CommitShader( pShader );
    
    if ( !GetRenderState().GetShaderHandle().IsNull())
    {
        GetRenderState().GetShaderAttribs().Apply(r);
        GetRenderState().GetShaderUniforms().Apply(r);
        if ( r.GetCurrentCamera() )
        {
            // Update matrices 
            GetRenderState().GetShaderViewUniform().SetData(      &r.GetCurrentCamera()->GetViewMatrix());
            GetRenderState().GetShaderProjectionUniform().SetData(&r.GetCurrentCamera()->GetProjectionMatrix());
            if ( GetTransform() )
            { 
              GetRenderState().GetShaderModelUniform().SetData( &GetTransform()->GetMatrix() );
            } 
            else 
            {
              GetRenderState().GetShaderModelUniform().SetData( &CMatrix4x4<float>::Identity );
            }
            // Send data to shader
            GetRenderState().GetShaderViewUniform().Apply(r);
            GetRenderState().GetShaderProjectionUniform().Apply(r);
            GetRenderState().GetShaderModelUniform().Apply(r);
        }
    }
    r.CommitPrimitive( g_IndexMgr->GetResource("moose_sphererenderable_indices") );
    GetRenderState().GetShaderAttribs().Disable(r);
    if ( m_pTransform != NULL )
        r.RollbackTransform();
    
}
////////////////////////////////////////////////////////////////
void 
prefix::CSphereRenderable::UseSphere( const Moose::Volume::CSphere & sphere )
{
    float fPart = ((Moose::Math::PI*2.0)/16.0f);
    for( int i=0;i<16;i++)
    {
        m_Vertices.GetPointer<float>(i)[0] = sinf(i*fPart)*sphere.GetRadius() + sphere.GetPosition()[0];
        m_Vertices.GetPointer<float>(i)[1] = cosf(i*fPart)*sphere.GetRadius() + sphere.GetPosition()[1];
        m_Vertices.GetPointer<float>(i)[2] = sphere.GetPosition()[2];
    }
    
    for( int i=16;i<32;i++)
    {
        m_Vertices.GetPointer<float>(i)[0] = sinf((i-16)*fPart)*sphere.GetRadius()+ sphere.GetPosition()[0];
        m_Vertices.GetPointer<float>(i)[1] = sphere.GetPosition()[1];
        m_Vertices.GetPointer<float>(i)[2] = cosf((i-16)*fPart)*sphere.GetRadius()+ sphere.GetPosition()[2];
    }
    for( int i=32;i<48;i++)
    {
        m_Vertices.GetPointer<float>(i)[0] = sphere.GetPosition()[0];
        m_Vertices.GetPointer<float>(i)[1] = sinf((i-32)*fPart)*sphere.GetRadius() + sphere.GetPosition()[1];
        m_Vertices.GetPointer<float>(i)[2] = cosf((i-32)*fPart)*sphere.GetRadius() + sphere.GetPosition()[2];
    }
    
    if ( m_Vertices.IsCached() ) m_Vertices.SetState(Moose::Core::CACHE_REFRESH);
}
////////////////////////////////////////////////////////////////
