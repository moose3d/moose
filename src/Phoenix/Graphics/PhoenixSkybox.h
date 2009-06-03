#ifndef __PhoenixSkybox_h__
#define __PhoenixSkybox_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixDefaultEntities.h"
#include "PhoenixRenderableModel.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
#define PHOENIX_SKYBOX_TEXCOORDS    "PHOENIX_SKYBOX_TEXCOORDS"
#define PHOENIX_SKYBOX_VERTICES     "PHOENIX_SKYBOX_VERTICES"
#define PHOENIX_SKYBOX_INDICES      "PHOENIX_SKYBOX_INDICES"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    /////////////////////////////////////////////////////////////////
    /// Skybox class.
    class PHOENIX_API CSkybox : public Phoenix::Graphics::CRenderableModel
    {
    public:
      ////////////////////
      /// Constructor.
      CSkybox();
      ////////////////////
      /// Destructor.
      ~CSkybox();
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
