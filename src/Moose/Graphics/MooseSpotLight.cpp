#include "MooseSpotLight.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CSpotLight::CSpotLight()
{
  SetPosition(0.0f,0.0f,0.0f);
  SetDirection(0.0f,0.0f,-1.0f);
  SetSpotAngle(0.0f);
  SetSpotExponent(0.0f);
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CSpotLight::Render( Moose::Graphics::COglRenderer & renderer )
{

}
///////////////////////////////////////////////////////////////////////////////
