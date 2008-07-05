#ifndef __PhoenixSkybox_h__
#define __PhoenixSkybox_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixDefaultEntities.h"
#include "PhoenixRenderable.h"
/////////////////////////////////////////////////////////////////
#define PHOENIX_SKYBOX_TEXCOORDS "PHOENIX_SKYBOX_TEXCOORDS"
#define PHOENIX_SKYBOX_VERTICES  "PHOENIX_SKYBOX_VERTICES"
#define PHOENIX_SKYBOX_INDICES   "PHOENIX_SKYBOX_INDICES"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics
  {
    enum SKYBOX_WALL_TYPE
    {

      SKYBOX_FLOOR = 0,
      SKYBOX_CEILING = 1,
      SKYBOX_FRONT = 2,
      SKYBOX_REAR = 3,
      SKYBOX_LEFT = 4,
      SKYBOX_RIGHT = 5,
      SKYBOX_EXTRA1 = 6,
      SKYBOX_EXTRA2 = 7,
      SKYBOX_WALL_COUNT
    };

    /////////////////////////////////////////////////////////////////
    /// Skybox class.
    class CSkybox : public CRenderable
    {
    protected:
      
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
