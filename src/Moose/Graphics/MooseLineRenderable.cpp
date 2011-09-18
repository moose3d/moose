/*
 *  MooseLineRenderable.cpp
 *  iPhoneMoose
 *
 *  Created by eNtity on 6/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MooseLineRenderable.h"
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Math;
namespace prefix = Moose::Graphics;
prefix::CLineRenderable::CLineRenderable() : m_Vertices(ELEMENT_TYPE_ATTRIB_3F, 4),
                                             m_EndposThickness(ELEMENT_TYPE_ATTRIB_4F, 4),
                                             m_Color( ELEMENT_TYPE_UNIFORM_4F,1),
                                             m_fThickness(1.0f)
{
    SetColor(1,1,1,1);
    GetRenderState().AddShaderAttrib("a_vertex", &m_Vertices);
    GetRenderState().AddShaderAttrib("a_endpos_thickness", &m_EndposThickness);
    GetRenderState().GetShaderHandle() = "moose_line_shader";
    assert( GetRenderState().GetShaderHandle().IsNull() == false);
    GetRenderState().AddShaderUniform("color", &m_Color);
    GetRenderState().SetDepthTest(true);
    GetRenderState().SetDepthWrite(true);
    
}
//////////////////////////////////////////////////////////////////////////
void 
prefix::CLineRenderable::SetColor( float fR, float fG, float fB, float fA)
{
    m_Color.GetPointer<float>()[0] = fR;
    m_Color.GetPointer<float>()[1] = fG;
    m_Color.GetPointer<float>()[2] = fB;
    m_Color.GetPointer<float>()[3] = fA; 
}
//////////////////////////////////////////////////////////////////////////
void 
prefix::CLineRenderable::SetThickness( float fValue )
{
    m_fThickness = fValue;
}
//////////////////////////////////////////////////////////////////////////
void
prefix::CLineRenderable::Render( COglRenderer & r )
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
    r.CommitPrimitive( g_IndexMgr->GetResource("moose_linerenderable_indices") );
    GetRenderState().GetShaderAttribs().Disable(r);    
    if ( m_pTransform != NULL )
        r.RollbackTransform();
    
}
//////////////////////////////////////////////////////////////////////////////////////////
void
prefix::CLineRenderable::SetLine( const Moose::Math::CVector3<float> & vStartPos,
                                   const Moose::Math::CVector3<float> & vEndPos)
{
    m_Vertices.GetPointer<float>(0)[0] = vStartPos[0];
    m_Vertices.GetPointer<float>(0)[1] = vStartPos[1];
    m_Vertices.GetPointer<float>(0)[2] = vStartPos[2];
    
    m_Vertices.GetPointer<float>(1)[0] = vStartPos[0];
    m_Vertices.GetPointer<float>(1)[1] = vStartPos[1];
    m_Vertices.GetPointer<float>(1)[2] = vStartPos[2];
    
    m_Vertices.GetPointer<float>(2)[0] = vEndPos[0];
    m_Vertices.GetPointer<float>(2)[1] = vEndPos[1];
    m_Vertices.GetPointer<float>(2)[2] = vEndPos[2];
    
    m_Vertices.GetPointer<float>(3)[0] = vEndPos[0];
    m_Vertices.GetPointer<float>(3)[1] = vEndPos[1];
    m_Vertices.GetPointer<float>(3)[2] = vEndPos[2];
    
    m_EndposThickness.GetPointer<float>()[0] = vEndPos[0];
    m_EndposThickness.GetPointer<float>()[1] = vEndPos[1];
    m_EndposThickness.GetPointer<float>()[2] = vEndPos[2];
    m_EndposThickness.GetPointer<float>()[3] = GetThickness();
    
    m_EndposThickness.GetPointer<float>()[4] = vEndPos[0];
    m_EndposThickness.GetPointer<float>()[5] = vEndPos[1];
    m_EndposThickness.GetPointer<float>()[6] = vEndPos[2];
    m_EndposThickness.GetPointer<float>()[7] = -GetThickness();
    
    m_EndposThickness.GetPointer<float>()[8] =  vStartPos[0];
    m_EndposThickness.GetPointer<float>()[9] =  vStartPos[1];
    m_EndposThickness.GetPointer<float>()[10] = vStartPos[2];
    m_EndposThickness.GetPointer<float>()[11] = -GetThickness();
    
    m_EndposThickness.GetPointer<float>()[12] = vStartPos[0];
    m_EndposThickness.GetPointer<float>()[13] = vStartPos[1];
    m_EndposThickness.GetPointer<float>()[14] = vStartPos[2];
    m_EndposThickness.GetPointer<float>()[15] = GetThickness();
    
    if ( m_Vertices.IsCached() ) m_Vertices.SetState(Moose::Core::CACHE_REFRESH);
    if ( m_EndposThickness.IsCached() ) m_EndposThickness.SetState(Moose::Core::CACHE_REFRESH);
        
}
