#ifndef __PhoenixObjectUpdater_h__
#define __PhoenixObjectUpdater_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixResourceManager.h"
#include "PhoenixAPI.h"
#include <list>
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Core
  {
    ////////////////////
    /// Interface for all updateables.
    class PHOENIX_API IUpdateable
    {
    public:
      virtual ~IUpdateable() {};
      virtual void Update( float fSecondsPassed ) = 0;
    };
  }
};
/////////////////////////////////////////////////////////////////
#endif
