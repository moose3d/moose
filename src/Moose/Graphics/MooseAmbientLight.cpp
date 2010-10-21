#include "MooseAmbientLight.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CAmbientLight::Render( Moose::Graphics::COglRenderer & renderer )
{
	if ( IsEnabled() ) 	renderer.CommitLight(*this);
}
///////////////////////////////////////////////////////////////////////////////
