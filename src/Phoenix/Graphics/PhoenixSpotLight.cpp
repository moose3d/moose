#include "PhoenixSpotLight.h"
#include "PhoenixOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CSpotLight::Render( Phoenix::Graphics::COglRenderer & renderer )
{
	if ( GetTransform() != NULL )
	{
		SetPosition( GetTransform()->GetTranslation() );
		// TODO implement light direction vector rotation in some manner; or ignore it completely.
	}

	renderer.CommitLight(*this);
}
///////////////////////////////////////////////////////////////////////////////
