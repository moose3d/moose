#include "MooseTransformIndicator.h"
#include "MooseGameObject.h"
#include "MooseDefaultEntities.h"
#include "MooseVector3.h"
#include "MooseOGLRenderer.h"
#include "MooseTransformGraph.h"
using namespace Moose::Graphics;
using namespace Moose::Math;
using namespace Moose::Scene;
////////////////////////////////////////////////////////////////////////////////
Moose::Util::CTransformIndicator::CTransformIndicator() : m_pTarget(NULL)
{
  if ( !g_Models("moose_transform_indicator_model") )
  {
    CModel *pModel = new CModel();
    
    CVertexDescriptor *pVertices = new CVertexDescriptor(ELEMENT_TYPE_VERTEX_3F, 6);
    pVertices->GetPointer<float>(0)[0] = 0.0f;
    pVertices->GetPointer<float>(0)[1] = 0.0f;
    pVertices->GetPointer<float>(0)[2] = 0.0f;
    pVertices->GetPointer<float>(1)[0] = 1.0f;
    pVertices->GetPointer<float>(1)[1] = 0.0f;
    pVertices->GetPointer<float>(1)[2] = 0.0f;

    pVertices->GetPointer<float>(2)[0] = 0.0f;
    pVertices->GetPointer<float>(2)[1] = 0.0f;
    pVertices->GetPointer<float>(2)[2] = 0.0f;
    pVertices->GetPointer<float>(3)[0] = 0.0f;
    pVertices->GetPointer<float>(3)[1] = 1.0f;
    pVertices->GetPointer<float>(3)[2] = 0.0f;

    pVertices->GetPointer<float>(4)[0] = 0.0f;
    pVertices->GetPointer<float>(4)[1] = 0.0f;
    pVertices->GetPointer<float>(4)[2] = 0.0f;
    pVertices->GetPointer<float>(5)[0] = 0.0f;
    pVertices->GetPointer<float>(5)[1] = 0.0f;
    pVertices->GetPointer<float>(5)[2] = 1.0f;

    CVertexDescriptor *pColors = new CVertexDescriptor(ELEMENT_TYPE_COLOR_3F, 6);
    pColors->GetPointer<float>(0)[0] = 1.0f;
    pColors->GetPointer<float>(0)[1] = 0.0f;
    pColors->GetPointer<float>(0)[2] = 0.0f;
    pColors->GetPointer<float>(1)[0] = 1.0f;
    pColors->GetPointer<float>(1)[1] = 0.0f;
    pColors->GetPointer<float>(1)[2] = 0.0f;

    pColors->GetPointer<float>(2)[0] = 0.0f;
    pColors->GetPointer<float>(2)[1] = 1.0f;
    pColors->GetPointer<float>(2)[2] = 0.0f;
    pColors->GetPointer<float>(3)[0] = 0.0f;
    pColors->GetPointer<float>(3)[1] = 1.0f;
    pColors->GetPointer<float>(3)[2] = 0.0f;

    pColors->GetPointer<float>(4)[0] = 0.0f;
    pColors->GetPointer<float>(4)[1] = 0.0f;
    pColors->GetPointer<float>(4)[2] = 1.0f;
    pColors->GetPointer<float>(5)[0] = 0.0f;
    pColors->GetPointer<float>(5)[1] = 0.0f;
    pColors->GetPointer<float>(5)[2] = 1.0f;

    CIndexArray *pIndices = new CIndexArray( PRIMITIVE_LINE_LIST, 6);
    unsigned short int indices[6] = { 0,1,2,3,4,5 };
    pIndices->Copy( indices);
    g_Vertices.Create(pVertices, "moose_transform_indicator_model_vertices", pModel->GetVertexHandle());
    g_Vertices.Create(pColors,   "moose_transform_indicator_model_colors",   pModel->GetColorHandle());
    g_Indices.Create( pIndices,  "moose_transform_indicator_model_indices",  pModel->GetIndices());
    g_Models.Create(  pModel,    "moose_transform_indicator_model",          GetModelHandle());
  } 
  else
  {
    GetModelHandle() = "moose_transform_indicator_model";
  }
  
  if ( g_Shaders("moose_vertcolor_shader") == NULL )
  {
    CShader *pShader= new CShader();
    pShader->LoadVertexShader( g_AssetBundle->GetAssetPath("Shaders/vertcolor.vert"));
    pShader->LoadFragmentShader( g_AssetBundle->GetAssetPath("Shaders/vertcolor.frag"));
    g_Shaders.Create( pShader, "moose_vertcolor_shader");
  }
  
  CRenderState &rs = GetRenderState();
  rs.SetShader( "moose_vertcolor_shader");
  
  rs.AddShaderAttrib("a_vertex", (*GetModelHandle())->GetVertexHandle() );
  rs.AddShaderAttrib("a_color",  (*GetModelHandle())->GetColorHandle());
  bool bVal = rs.Prepare();
  assert(bVal);
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Util::CTransformIndicator::Render( Moose::Graphics::COglRenderer & r )
{


  CTransform &t = *m_pTransform;
  r.CommitRenderState(GetRenderState());
  CCamera *pCam = r.GetCurrentCamera();
  float fScaleFudgeFactor = (pCam->GetPosition() - t.GetTranslation()).Length()*0.2f;
  

  // Transform must be the local transform of CTransformController.
  // scaling of coliders might be little behind (a frame or so, but it should not matter).
  t.SetScaling( fScaleFudgeFactor );

  if ( m_pTarget ) {
    m_pTarget->GetLocalTransform().SetScaling(fScaleFudgeFactor);
    m_pTarget->SetChanged(true);
  }

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
      r.CommitTexture( i, pTexture );
      r.CommitFilters( GetRenderState().GetTextureFilters(i), 
                              pTexture->GetType() );
    }
    else 
    { 
      
      r.DisableTexture(i, NULL);
    }
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
      if ( GetTransform() != NULL ) // model transform is optional.
      {
        GetRenderState().GetShaderModelUniform().SetData( &GetTransform()->GetMatrix() );
      }
      // Send data to shader
      GetRenderState().GetShaderViewUniform().Apply(r);
      GetRenderState().GetShaderProjectionUniform().Apply(r);
      GetRenderState().GetShaderModelUniform().Apply(r); 
    }
  }

  if ( !model.GetIndices().IsNull() )   r.CommitPrimitive( *model.GetIndices() );
  if ( m_pTransform != NULL )           r.RollbackTransform();

}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Util::CTransformIndicator::SetTarget( Moose::Scene::CTransformable *pTransformable )
{
  m_pTarget = pTransformable;
}
////////////////////////////////////////////////////////////////////////////////
