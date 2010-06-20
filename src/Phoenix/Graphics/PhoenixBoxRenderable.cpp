/*
 *  CBoxRenderable.cpp
 *  MechanicInvaders
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
///////////////////////////////////////////////////////////////////////
#include "PhoenixBoxRenderable.h"
using namespace Phoenix::Graphics;
using namespace Phoenix::Volume;
using namespace Phoenix::Math;
namespace prefix=Phoenix::Graphics;
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
            GetRenderState().GetShaderModelUniform().SetData( &GetTransform()->GetMatrix() );
            // Send data to shader
            GetRenderState().GetShaderViewUniform().Apply(r);
            GetRenderState().GetShaderProjectionUniform().Apply(r);
            GetRenderState().GetShaderModelUniform().Apply(r);
        }
    }
    r.CommitPrimitive( g_IndexMgr->GetResource("moose_boxrenderable_indices") );
    
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
}
//////////////////////////////////////////////////////////////////////                                        