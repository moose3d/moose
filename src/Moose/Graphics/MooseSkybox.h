#ifndef __MooseSkybox_h__
#define __MooseSkybox_h__
/////////////////////////////////////////////////////////////////
#include "MooseDefaultEntities.h"
#include "MooseRenderableModelShared.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
#define MOOSE_SKYBOX_TEXCOORDS    "MOOSE_SKYBOX_TEXCOORDS"
#define MOOSE_SKYBOX_VERTICES     "MOOSE_SKYBOX_VERTICES"
#define MOOSE_SKYBOX_INDICES      "MOOSE_SKYBOX_INDICES"
#define MOOSE_SKYBOX_MODEL        "MOOSE_SKYBOX_MODEL"
#define MOOSE_SKYBOX_SHADER       "MOOSE_SKYBOX_SHADER"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics
  {
  	class COglRenderer;
    /////////////////////////////////////////////////////////////////
    /// Skybox class.
    class MOOSE_API CSkybox : public Moose::Graphics::CRenderableModelShared
    {
    public:
      ////////////////////
      /// Constructor.
      CSkybox();
      ////////////////////
      /// Destructor.
      ~CSkybox();
      void Render( Moose::Graphics::COglRenderer & renderer );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
