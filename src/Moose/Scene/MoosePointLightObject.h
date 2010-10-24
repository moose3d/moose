#ifndef __MoosePointLightobject_h__
#define __MoosePointLightobject_h__
///////////////////////////////////////////////////////////////////////////////
#include <MooseGameObject.h>
#include <MoosePointLight.h>
///////////////////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Scene
  {
    ////////////////
    /// Point light to be inserted into a scene.
    class CPointLightObject : public Moose::Scene::CGameObject,
                              public Moose::Graphics::CPointLight
    {
    public:

      CPointLightObject( );
      virtual ~CPointLightObject();
      void Update( float fSeconds );
      
    };
  }
}

	///////////////////////////////////////////////////////////////////////////////
#endif
