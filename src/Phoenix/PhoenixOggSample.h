#ifndef __COggSample_h__
#define __COggSample_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixALSampleTypes.h"
#include "PhoenixAPI.h"
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Sound
  {
    class PHOENIX_API COggSample : public CALSample
    {
    public:
      bool Load( const char *szFilename );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
