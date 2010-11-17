#include "MooseCapsuleRenderable.h"
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Math;
using namespace Moose::Volume;
namespace prefix=Moose::Graphics;
////////////////////////////////////////////////////////////////////////////////
prefix::CCapsuleRenderable::CCapsuleRenderable() : m_Vertices(ELEMENT_TYPE_VERTEX_3F,36),
                                                   m_Color( ELEMENT_TYPE_UNIFORM_4F,1)
{
  SetColor(1.0, 0.0, 0.0, 1.0);
  GetRenderState().AddShaderAttrib("a_vertex", &m_Vertices);
  GetRenderState().GetShaderHandle() = "moose_color_shader";
  assert( GetRenderState().GetShaderHandle().IsNull() == false);
  GetRenderState().AddShaderUniform( "color", & m_Color);
  GetRenderState().SetDepthTest(true);
  GetRenderState().SetDepthWrite(true);
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CCapsuleRenderable::Render( Moose::Graphics::COglRenderer & r )
{
  r.CommitRenderState(GetRenderState());
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
  r.CommitPrimitive( g_IndexMgr->GetResource("moose_sphererenderable_indices") );
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CCapsuleRenderable::UseCapsule( const Moose::Volume::CCapsule & capsule )
{
  float fPart = ((Moose::Math::PI)/6.0f);
  CVector3<float> vEnd = capsule.GetEnd();
  CVector3<float> vStart = capsule.GetStart();
  CVector3<float> perp(1,0,0);
  if ( !TOO_CLOSE_TO_ZERO(perp.Dot(capsule.GetDirection())))
  {
    perp[0] = 0.0; perp[1] = 1.0;
  }
  CVector3<float> up = capsule.GetDirection().Cross(perp);
  up.Normalize();
  up *= capsule.GetRadius();
  CVector3<float> fwd = capsule.GetDirection()*capsule.GetRadius();
  
  CVector3<float> right = up.Cross(fwd);
  right.Normalize();


  for( int i=0;i<4;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( up, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vEnd;
    m_Vertices.GetPointer<float>(i)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i)[2] = tmp[2];
  }
  m_Vertices.GetPointer<float>(4)[0] = m_Vertices.GetPointer<float>(3)[0] - vEnd[0] + vStart[0];
  m_Vertices.GetPointer<float>(4)[1] = m_Vertices.GetPointer<float>(3)[1] - vEnd[1] + vStart[1];
  m_Vertices.GetPointer<float>(4)[2] = m_Vertices.GetPointer<float>(3)[2] - vEnd[2] + vStart[2];

  for( int i=5;i<8;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( up, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vStart;
    m_Vertices.GetPointer<float>(i)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i)[2] = tmp[2];
  }

  for( int i=8;i<11;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( up, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vStart;
    m_Vertices.GetPointer<float>(i)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i)[2] = tmp[2];
  }
  m_Vertices.GetPointer<float>(11)[0] = m_Vertices.GetPointer<float>(10)[0] - vStart[0] + vEnd[0];
  m_Vertices.GetPointer<float>(11)[1] = m_Vertices.GetPointer<float>(10)[1] - vStart[1] + vEnd[1];
  m_Vertices.GetPointer<float>(11)[2] = m_Vertices.GetPointer<float>(10)[2] - vStart[2] + vEnd[2]; 
  
  for( int i=12;i<14;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( up, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vEnd;
    m_Vertices.GetPointer<float>(i)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i)[2] = tmp[2];
  }
  // Another way around
  for( int i=1;i<4;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( right, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vEnd;
    m_Vertices.GetPointer<float>(i+13)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+13)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+13)[2] = tmp[2];
  }
  m_Vertices.GetPointer<float>(17)[0] = m_Vertices.GetPointer<float>(16)[0] - vEnd[0] + vStart[0];
  m_Vertices.GetPointer<float>(17)[1] = m_Vertices.GetPointer<float>(16)[1] - vEnd[1] + vStart[1];
  m_Vertices.GetPointer<float>(17)[2] = m_Vertices.GetPointer<float>(16)[2] - vEnd[2] + vStart[2];
  
  for( int i=5;i<7;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( right, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vStart;
    m_Vertices.GetPointer<float>(i+13)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+13)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+13)[2] = tmp[2];
  }

  for( int i=8;i<11;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( right, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vStart;
    m_Vertices.GetPointer<float>(i+12)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+12)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+12)[2] = tmp[2];
  }

  m_Vertices.GetPointer<float>(23)[0] = m_Vertices.GetPointer<float>(22)[0] - vStart[0] + vEnd[0];
  m_Vertices.GetPointer<float>(23)[1] = m_Vertices.GetPointer<float>(22)[1] - vStart[1] + vEnd[1];
  m_Vertices.GetPointer<float>(23)[2] = m_Vertices.GetPointer<float>(22)[2] - vStart[2] + vEnd[2];

  for( int i=12;i<14;i++)
  {
    CVector3<float> tmp = fwd;
    CQuaternion q = CQuaternion::AxisAngle( right, fPart*(float)i);    
    RotateVector(q,tmp);
    tmp += vEnd;
    m_Vertices.GetPointer<float>(i+12)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+12)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+12)[2] = tmp[2];
  }
  
  // TODO Two round circles at the ends of line segment
  
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CCapsuleRenderable::SetColor( float fR, float fG, float fB, float fA)
{
  m_Color.GetPointer<float>()[0] = fR;
  m_Color.GetPointer<float>()[1] = fG;
  m_Color.GetPointer<float>()[2] = fB;
  m_Color.GetPointer<float>()[3] = fA; 
}
////////////////////////////////////////////////////////////////////////////////
