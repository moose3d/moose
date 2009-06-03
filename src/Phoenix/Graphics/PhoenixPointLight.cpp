#include "PhoenixPointLight.h"
#include "PhoenixOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CPointLight::Render( Phoenix::Graphics::COglRenderer & renderer )
{
	if ( GetTransform() != NULL ) SetPosition( GetTransform()->GetTranslation() );
	renderer.CommitLight(*this);
}
///////////////////////////////////////////////////////////////////////////////
