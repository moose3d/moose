#ifndef __MooseObjectUpdater_h__
#define __MooseObjectUpdater_h__
/////////////////////////////////////////////////////////////////
#include "MooseResourceManager.h"
#include "MooseAPI.h"
#include <list>
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Core
  {
    ////////////////////
    /// Interface for all updateables.
    class MOOSE_API IUpdateable
    {
    public:
      virtual ~IUpdateable() {};
      virtual void Update( float fSecondsPassed ) = 0;
    };
  }
};
/////////////////////////////////////////////////////////////////
#endif
