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
      SKYBOX_FLOOR,
      SKYBOX_CEILING,
      SKYBOX_FRONT,
      SKYBOX_REAR,
      SKYBOX_LEFT,
      SKYBOX_RIGHT,
      SKYBOX_EXTRA1,
      SKYBOX_EXTRA2,
      SKYBOX_WALL_COUNT
    };

    /////////////////////////////////////////////////////////////////
    /// Skybox class.
    class CSkybox : public Phoenix::Graphics::CRenderable
    {
    protected:
      INDEX_HANDLE m_hIndices[6];
    public:
      ////////////////////
      /// Constructor.
      CSkybox();
      ////////////////////
      /// Destructor.
      ~CSkybox();
      
      INDEX_HANDLE & GetWallIndices( SKYBOX_WALL_TYPE nIndex );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
