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
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Util::CTransformIndicator::Render( Moose::Graphics::COglRenderer & r )
{
  CTransform t;
  r.CommitRenderState(GetRenderState());
  if ( m_pTransform != NULL )  
  {
    t = *m_pTransform;
  }
  CCamera *pCam = r.GetCurrentCamera();
  float fScaleFudgeFactor = (pCam->GetPosition() - t.GetTranslation()).Length()*0.2f;


  // Transform must be the local transform of CTransformController.
  // scaling of coliders might be little behind (a frame or so, but it should not matter).
  t.SetScaling( fScaleFudgeFactor );
  r.CommitTransform(t);

  if ( m_pTarget ) {
    m_pTarget->GetLocalTransform().SetScaling(fScaleFudgeFactor);
    m_pTarget->SetChanged(true);
  }

  CModel & model = **GetModelHandle();
  r.CommitVertexDescriptor( *model.GetColorHandle() );
  r.CommitVertexDescriptor( *model.GetVertexHandle() );
  r.CommitPrimitive( *model.GetIndices() );

  if ( m_pTransform != NULL )  
    r.RollbackTransform();
}
////////////////////////////////////////////////////////////////////////////////
void
Moose::Util::CTransformIndicator::SetTarget( Moose::Scene::CTransformable *pTransformable )
{
  m_pTarget = pTransformable;
}
////////////////////////////////////////////////////////////////////////////////
