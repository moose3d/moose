#include "MooseDirectionalLightObject.h"
#include "MooseDirectionalLight.h"
#include <cassert>
///////////////////////////////////////////////////////////////////////////////
using namespace Moose::Scene;
using namespace Moose::Graphics;
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CDirectionalLightObject::CDirectionalLightObject()
{
	m_pLight = new CDirectionalLight();
	InitializeRenderables(1);
	// Light renderable is always present
	AddRenderable(m_pLight, 0, NULL);
	SetTag( LIGHT_TAG );
	SetEnabled(true);
	//GetBoundingSphere().SetRadius( g_Game->GetScene()->GetSpatialGraph().GetWorldHalfSize() * 1.732f);
}
///////////////////////////////////////////////////////////////////////////////
Moose::Scene::CDirectionalLightObject::~CDirectionalLightObject()
{

}
///////////////////////////////////////////////////////////////////////////////
Moose::Graphics::CDirectionalLight *
Moose::Scene::CDirectionalLightObject::GetLight()
{
	return m_pLight;
}
///////////////////////////////////////////////////////////////////////////////
void
Moose::Scene::CDirectionalLightObject::Update( float fSeconds )
{

}
///////////////////////////////////////////////////////////////////////////////
