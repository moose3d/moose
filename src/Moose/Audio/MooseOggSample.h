#ifndef __COggSample_h__
#define __COggSample_h__
#include "MooseAPI.h"
#if !defined(MOOSE_APPLE_IPHONE)
/////////////////////////////////////////////////////////////////
#include "MooseALSampleTypes.h"
#include "MooseAPI.h"
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Sound
  {
    class MOOSE_API COggSample : public CALSample
    {
    public:
      bool Load( const char *szFilename );
    };
  }
}
/////////////////////////////////////////////////////////////////
#endif
#endif
