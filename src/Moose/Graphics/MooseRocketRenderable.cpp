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
