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
 *  CBoxRenderable.cpp
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
///////////////////////////////////////////////////////////////////////
#include "MooseBoxRenderable.h"
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Volume;
using namespace Moose::Math;
namespace prefix=Moose::Graphics;
///////////////////////////////////////////////////////////////////////
prefix::CBoxRenderable::CBoxRenderable() : m_Vertices(ELEMENT_TYPE_VERTEX_3F,8), 
                                           m_Color( ELEMENT_TYPE_UNIFORM_4F,1)
{
    GetRenderState().AddShaderAttrib("a_vertex", &m_Vertices);
    m_Color.GetPointer<float>()[0] = 1.0;
    m_Color.GetPointer<float>()[0] = 0.0;
    m_Color.GetPointer<float>()[0] = 0.0;
    m_Color.GetPointer<float>()[0] = 1.0;
    GetRenderState().GetShaderHandle() = "moose_color_shader";
    assert( GetRenderState().GetShaderHandle().IsNull() == false);
    GetRenderState().AddShaderUniform("color", &m_Color);
    
    GetRenderState().SetDepthTest(true);
    GetRenderState().SetDepthWrite(true);

    
}
///////////////////////////////////////////////////////////////////////
void
prefix::CBoxRenderable::Render( COglRenderer & r )
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
    r.CommitPrimitive( g_IndexMgr->GetResource("moose_boxrenderable_indices") );

    GetRenderState().GetShaderAttribs().Disable(r);    
    if ( m_pTransform != NULL )
        r.RollbackTransform();
    
}
///////////////////////////////////////////////////////////////////////
void
prefix::CBoxRenderable::UseBox( const COrientedBox & box  )
{
    CVector3<float> vTmp = box.GetCorner(BOTTOM_LEFT_BACK);
    m_Vertices.GetPointer<float>(0)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(0)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(0)[2] = vTmp[2];
    
    vTmp = box.GetCorner(BOTTOM_RIGHT_BACK);
    m_Vertices.GetPointer<float>(1)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(1)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(1)[2] = vTmp[2];
    
    vTmp = box.GetCorner(TOP_RIGHT_BACK);
    m_Vertices.GetPointer<float>(2)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(2)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(2)[2] = vTmp[2];
    
    vTmp=box.GetCorner(TOP_LEFT_BACK);
    m_Vertices.GetPointer<float>(3)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(3)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(3)[2] = vTmp[2];
    
    vTmp = box.GetCorner(BOTTOM_LEFT_FRONT);
    m_Vertices.GetPointer<float>(4)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(4)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(4)[2] = vTmp[2];
    
    vTmp = box.GetCorner(BOTTOM_RIGHT_FRONT);
    m_Vertices.GetPointer<float>(5)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(5)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(5)[2] = vTmp[2];
    
    vTmp = box.GetCorner(TOP_RIGHT_FRONT);
    m_Vertices.GetPointer<float>(6)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(6)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(6)[2] = vTmp[2];
    
    vTmp=box.GetCorner(TOP_LEFT_FRONT);
    m_Vertices.GetPointer<float>(7)[0] = vTmp[0];
    m_Vertices.GetPointer<float>(7)[1] = vTmp[1];
    m_Vertices.GetPointer<float>(7)[2] = vTmp[2];
    
    if ( m_Vertices.IsCached() ) m_Vertices.SetState(Moose::Core::CACHE_REFRESH);
}
//////////////////////////////////////////////////////////////////////                                        
