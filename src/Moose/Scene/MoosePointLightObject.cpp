#include "MoosePointLightObject.h"
#include "MoosePointLight.h"
#include <cassert>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Scene;
using namespace Moose::Graphics;
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CPointLightObject::CPointLightObject()
{
	InitializeRenderables(1);
	SetTag( LIGHT_TAG );
	SetEnabled(true);
	//GetBoundingSphere().SetRadius( g_Game->GetScene()->GetSpatialGraph().GetWorldHalfSize() * 1.732f);
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CPointLightObject::~CPointLightObject()
{
  
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CPointLightObject::Update( float fSeconds )
{
  SetPosition(GetWorldTransform().GetTranslation());
  float fAtt = ComputeAttenuationDistance();
  if ( fAtt > 0.0 )  GetBoundingSphere().SetRadius( fAtt ); 
  else GetBoundingSphere().SetRadius(1000.0f);
}
///////////////////////////////////////////////////////////////////////////////
