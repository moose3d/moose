#include "PhoenixPointLight.h"
#include "PhoenixOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
Phoenix::Graphics::CPointLight::CPointLight()
{
	SetPosition(0.0f,0.0f,0.0f);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CPointLight::Render( Phoenix::Graphics::COglRenderer & renderer )
{
	if ( IsEnabled() )
	{
		if ( GetTransform() != NULL ) SetPosition( GetTransform()->GetTranslation() );
		renderer.CommitLight(*this, GetRenderState().GetLightId());
	}
}
///////////////////////////////////////////////////////////////////////////////
