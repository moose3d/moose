#include "MooseDirectionalLightObject.h"
#include "MooseDirectionalLight.h"
#include <cassert>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Scene;
using namespace Moose::Graphics;
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CDirectionalLightObject::CDirectionalLightObject()
{
	InitializeRenderables(1);
	SetTag( LIGHT_TAG );
	SetEnabled(true);
	//GetBoundingSphere().SetRadius( g_Game->GetScene()->GetSpatialGraph().GetWorldHalfSize() * 1.732f);
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CDirectionalLightObject::~CDirectionalLightObject()
{
  
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CDirectionalLightObject::Update( float fSeconds )
{
  SetDirection(GetWorldTransform().GetForwardVector());
}
///////////////////////////////////////////////////////////////////////////////
