#include "PhoenixDirectionalLightObject.h"
#include "PhoenixDirectionalLight.h"
#include <cassert>
///////////////////////////////////////////////////////////////////////////////
using namespace Phoenix::Scene;
using namespace Phoenix::Graphics;
///////////////////////////////////////////////////////////////////////////////
Phoenix::Scene::CDirectionalLightObject::CDirectionalLightObject()
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
Phoenix::Scene::CDirectionalLightObject::~CDirectionalLightObject()
{

}
///////////////////////////////////////////////////////////////////////////////
Phoenix::Graphics::CDirectionalLight *
Phoenix::Scene::CDirectionalLightObject::GetLight()
{
	return m_pLight;
}
///////////////////////////////////////////////////////////////////////////////
void
Phoenix::Scene::CDirectionalLightObject::Update( float fSeconds )
{

}
///////////////////////////////////////////////////////////////////////////////
