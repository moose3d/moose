/*
 *  CSphereRenderable.cpp
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "PhoenixSphereRenderable.h"

using namespace Phoenix::Graphics;
namespace prefix=Phoenix::Graphics;
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
        GetRenderState().GetShaderAttribs().Apply();
        GetRenderState().GetShaderUniforms().Apply();
        if ( r.GetCurrentCamera() )
        {
            // Update matrices 
            GetRenderState().GetShaderViewUniform().SetData(      &r.GetCurrentCamera()->GetViewMatrix());
            GetRenderState().GetShaderProjectionUniform().SetData(&r.GetCurrentCamera()->GetProjectionMatrix());
            GetRenderState().GetShaderModelUniform().SetData( &GetTransform()->GetMatrix() );
            // Send data to shader
            GetRenderState().GetShaderViewUniform().Apply();
            GetRenderState().GetShaderProjectionUniform().Apply();
            GetRenderState().GetShaderModelUniform().Apply();
        }
    }
    r.CommitPrimitive( g_IndexMgr->GetResource("moose_sphererenderable_indices") );
    
    if ( m_pTransform != NULL )
        r.RollbackTransform();
    
}
////////////////////////////////////////////////////////////////
void 
prefix::CSphereRenderable::UseSphere( const Phoenix::Volume::CSphere & sphere )
{
    float fPart = ((Phoenix::Math::PI*2.0)/16.0f);
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
}
////////////////////////////////////////////////////////////////