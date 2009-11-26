#ifndef __PhoenixDirectionalLightobject_h__
#define __PhoenixDirectionalLightobject_h__
///////////////////////////////////////////////////////////////////////////////
#include <PhoenixGameObject.h>
///////////////////////////////////////////////////////////////////////////////

namespace Phoenix
{
	namespace Graphics
	{
		class CDirectionalLight;
	}
	namespace Scene
	{
		////////////////
		/// Directional light to be inserted into a scene.
		class CDirectionalLightObject : public Phoenix::Scene::CGameObject
		{
		private:
			Phoenix::Graphics::CDirectionalLight *m_pLight;
		public:

			CDirectionalLightObject( );
			virtual ~CDirectionalLightObject();

			Phoenix::Graphics::CDirectionalLight *GetLight();
			void Update( float fSeconds );

			bool Intersects( const Phoenix::Volume::CSphere & sphere ) const { return true; }
			bool Intersects( const Phoenix::Graphics::CFrustum & frustum ) const { return true; }
			bool Intersects( const Phoenix::Volume::COrientedBox & box ) const { return true; }
			bool Intersects( const Phoenix::Math::CVector3<float> & vPoint ) const  { return true; }
			bool Intersects( const Phoenix::Math::CRay & ray, float *pfDistance = NULL ) const  { return true; }
			bool Intersects( const Phoenix::Collision::ICollider & collider ) const  { return true; }

		};
	}
}

	///////////////////////////////////////////////////////////////////////////////
#endif
