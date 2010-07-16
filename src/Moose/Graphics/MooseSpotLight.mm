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
