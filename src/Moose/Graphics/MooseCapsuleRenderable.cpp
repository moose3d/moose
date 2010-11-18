#include "MooseCapsuleRenderable.h"
#include <cassert>
using namespace Moose::Graphics;
using namespace Moose::Math;
using namespace Moose::Volume;
namespace prefix=Moose::Graphics;
////////////////////////////////////////////////////////////////////////////////
prefix::CCapsuleRenderable::CCapsuleRenderable() : m_Vertices(ELEMENT_TYPE_VERTEX_3F,42),
                                                   m_Color( ELEMENT_TYPE_UNIFORM_4F,1)
{
  SetColor(1.0, 0.0, 0.0, 1.0);
  GetRenderState().AddShaderAttrib("a_vertex", &m_Vertices);
  GetRenderState().GetShaderHandle() = "moose_color_shader";
  assert( GetRenderState().GetShaderHandle().IsNull() == false);
  GetRenderState().AddShaderUniform( "color", &m_Color);
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
  r.CommitPrimitive( g_IndexMgr->GetResource("moose_capsulerenderable_indices") );
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::CCapsuleRenderable::UseCapsule( const Moose::Volume::CCapsule & capsule )
{
  
  /*
    
    Vertices as follows
            3
        2       4
      1           5
    0               6
 
    13              7
      12          8
        11     9   
           10


   The other way around
            X
        16     17
      15          18
    14              19
 
    25              20
      24          21
        23     22 
           X
           
     Indices wound starting from 3.
     
     Horizontal rings use 4 shared vertices (each)
  high
              14 
           27   28
         26       29
        0           6
         33       30
           32   31
              19

              25 
           35   36
         34       37
        13          7
         41       38
           40   39
              20
   */ 
  CVector3<float> vEnd = capsule.GetEnd();
  CVector3<float> vStart = capsule.GetStart();
  CVector3<float> perp(1,0,0);
  if ( !TOO_CLOSE_TO_ZERO(perp.Dot(capsule.GetDirection())))
  {
    perp[0] = 0.0; perp[1] = 1.0;
  }
  // Get UP vector and scale to unit length   
  CVector3<float> up = capsule.GetDirection().Cross(perp);
  up.Normalize();
  
  CVector3<float> fwd = capsule.GetDirection()*capsule.GetRadius();
  
  // Get RIGHT vector and scale to unit length 
  CVector3<float> right = fwd.Cross(up);
  right.Normalize();
 
  float fPart = ((Moose::Math::PI)/6.0f);

  // ------------------------------------------------------------
  for ( int i=0;i<7;i++)
  {
    CVector3<float> tmp = capsule.GetEnd() + 
                          sinf(fPart*(float)i)*fwd*capsule.GetRadius() + 
                          cosf(fPart*(float)i)*up*capsule.GetRadius();

    m_Vertices.GetPointer<float>(i)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i)[2] = tmp[2];
  }
  for ( int i=6;i<13;i++) // extra vertex, same angle, different height
  {
    CVector3<float> tmp = capsule.GetStart() +
                          sinf(fPart*(float)i)*fwd*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();

    m_Vertices.GetPointer<float>(i+1)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+1)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+1)[2] = tmp[2];
  }
  // ------------------------------------------------------------
  // Same thing but the other way, reuse couple of vertices.
  // ------------------------------------------------------------
  for ( int i=0;i<3;i++)
  {
    CVector3<float> tmp = capsule.GetEnd() + 
                          sinf(fPart*(float)i)*fwd*capsule.GetRadius() + 
                          cosf(fPart*(float)i)*right*capsule.GetRadius();

    m_Vertices.GetPointer<float>(i+14)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+14)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+14)[2] = tmp[2];
  }
  // Skip topmost vertex (3rd)
  for ( int i=4;i<7;i++)
  {
    CVector3<float> tmp = capsule.GetEnd() + 
                          sinf(fPart*(float)i)*fwd*capsule.GetRadius() + 
                          cosf(fPart*(float)i)*right*capsule.GetRadius();

    m_Vertices.GetPointer<float>(i+13)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+13)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+13)[2] = tmp[2];
  }
  for ( int i=6;i<9;i++) // extra vertex, same angle, different height
  {
    CVector3<float> tmp = capsule.GetStart() +
                          sinf(fPart*(float)i)*fwd*capsule.GetRadius() +
                          cosf(fPart*(float)i)*right*capsule.GetRadius();

    m_Vertices.GetPointer<float>(i+14)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+14)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+14)[2] = tmp[2];
  }
  // skip bottom-most vertex (10th)
  for ( int i=10;i<13;i++)
  {
    CVector3<float> tmp = capsule.GetStart() +
                          sinf(fPart*(float)i)*fwd*capsule.GetRadius() +
                          cosf(fPart*(float)i)*right*capsule.GetRadius();

    m_Vertices.GetPointer<float>(i+13)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+13)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+13)[2] = tmp[2];
  }
  
  // Two round circles at the ends of line segment
  // ------------------------------------------------------------
  // Higher end
  // ------------------------------------------------------------
  for( int i=1;i<3;i++)
  {
    CVector3<float> tmp = capsule.GetEnd() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+25)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+25)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+25)[2] = tmp[2];
  }

  for( int i=4;i<6;i++)
  {
    CVector3<float> tmp = capsule.GetEnd() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+24)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+24)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+24)[2] = tmp[2];
  }
  
  for( int i=7;i<9;i++)
  {
    CVector3<float> tmp = capsule.GetEnd() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+23)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+23)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+23)[2] = tmp[2];
  }
  for( int i=10;i<12;i++)
  {
    CVector3<float> tmp = capsule.GetEnd() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+22)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+22)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+22)[2] = tmp[2];
  }
  // ------------------------------------------------------------
  // Lower end
  // ------------------------------------------------------------
  for( int i=1;i<3;i++)
  {
    CVector3<float> tmp = capsule.GetStart() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+33)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+33)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+33)[2] = tmp[2];
  }

  for( int i=4;i<6;i++)
  {
    CVector3<float> tmp = capsule.GetStart() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+32)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+32)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+32)[2] = tmp[2];
  }
  
  for( int i=7;i<9;i++)
  {
    CVector3<float> tmp = capsule.GetStart() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+31)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+31)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+31)[2] = tmp[2];
  }
  for( int i=10;i<12;i++)
  {
    CVector3<float> tmp = capsule.GetStart() +
                          sinf(fPart*(float)i)*right*capsule.GetRadius() +
                          cosf(fPart*(float)i)*up*capsule.GetRadius();
    m_Vertices.GetPointer<float>(i+30)[0] = tmp[0];
    m_Vertices.GetPointer<float>(i+30)[1] = tmp[1];
    m_Vertices.GetPointer<float>(i+30)[2] = tmp[2];
  }
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
