#include "MooseDirectionalLight.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CDirectionalLight::CDirectionalLight()
{
  SetDirection(0.0f,0.0f,-1.0f);
  SetSpotAngle(-1.0);
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CDirectionalLight::Render( Moose::Graphics::COglRenderer & renderer )
{

}
///////////////////////////////////////////////////////////////////////////////
