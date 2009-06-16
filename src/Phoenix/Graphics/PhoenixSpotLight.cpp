#include "PhoenixSpotLight.h"
#include "PhoenixOGLRenderer.h"
///////////////////////////////////////////////////////////////////////////////
Phoenix::Graphics::CSpotLight::CSpotLight()
{
   	  SetPosition(0.0f,0.0f,0.0f);
   	  SetDirection(0.0f,0.0f,-1.0f);
   	  SetSpotAngle(0.0f);
   	  SetSpotExponent(0.0f);
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Graphics::CSpotLight::Render( Phoenix::Graphics::COglRenderer & renderer )
{
	if ( IsEnabled() )
	{
		if ( GetTransform() != NULL )
		{
			SetPosition( GetTransform()->GetTranslation() );
			// TODO implement light direction vector rotation in some manner; or ignore it completely.
		}

		renderer.CommitLight(*this, GetRenderState().GetLightId());
	}
}
///////////////////////////////////////////////////////////////////////////////
