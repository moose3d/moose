#ifndef __CCapsuleRenderable_h__
#define __CCapsuleRenderable_h__
////////////////////////////////////////////////////////////////////////////////
#include "MooseVertexDescriptor.h"
#include "MooseRenderable.h"
#include "MooseCapsule.h"
#include "MooseOGLRenderer.h"
////////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
    ////////////////////
    /// Renderable for all capsules. 
    class CCapsuleRenderable : public Moose::Graphics::CRenderable
    {
    protected:
      Moose::Graphics::CVertexDescriptor m_Vertices;
      Moose::Graphics::CVertexDescriptor m_Color;
    public:
      CCapsuleRenderable();
      void Render( Moose::Graphics::COglRenderer & r );
      void UseCapsule( const Moose::Volume::CCapsule & capsule );
      void SetColor( float fR, float fG, float fB, float fA);
    };
  } // Graphics
} // Moose
////////////////////////////////////////////////////////////////////////////////
#endif
