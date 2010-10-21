#ifndef __MooseDirectionalLightobject_h__
#define __MooseDirectionalLightobject_h__
///////////////////////////////////////////////////////////////////////////////
#include <MooseGameObject.h>
#include <MooseDirectionalLight.h>
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Scene
  {
    ////////////////
    /// Directional light to be inserted into a scene.
    class CDirectionalLightObject : public Moose::Scene::CGameObject,
                                    public Moose::Graphics::CDirectionalLight
    {
    public:

      CDirectionalLightObject( );
      virtual ~CDirectionalLightObject();
      void Update( float fSeconds );
      bool Intersects( const Moose::Volume::CSphere & sphere ) const { return true; }
      bool Intersects( const Moose::Graphics::CFrustum & frustum ) const { return true; }
      bool Intersects( const Moose::Volume::COrientedBox & box ) const { return true; }
      bool Intersects( const Moose::Math::CVector3<float> & vPoint ) const  { return true; }
      bool Intersects( const Moose::Math::CRay & ray, float *pfDistance = NULL ) const  { return true; }
      bool Intersects( const Moose::Collision::ICollider & collider ) const  { return true; }

    };
  }
}

	///////////////////////////////////////////////////////////////////////////////
#endif
