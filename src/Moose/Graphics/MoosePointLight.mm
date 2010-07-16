#include "MoosePointLight.h"
#include "MooseOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CPointLight::CPointLight()
{
	SetPosition(0.0f,0.0f,0.0f);
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Graphics::CPointLight::Render( Moose::Graphics::COglRenderer & renderer )
{
	if ( IsEnabled() )
	{
		if ( GetTransform() != NULL ) SetPosition( GetTransform()->GetTranslation() );
		renderer.CommitLight(*this, GetRenderState().GetLightId());
	}
}
///////////////////////////////////////////////////////////////////////////////
