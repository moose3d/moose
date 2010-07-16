#ifndef __MooseIPhoneAudioSample_h__
#define __MooseIPhoneAudioSample_h__
#include "MooseALSampleTypes.h"
/////////////////////////////////////////////////////////////
namespace Moose
{
    namespace Sound
    {
        class CIPhoneAudioSample : public Moose::Sound::CALSample
        {
        public:
            bool Load( const char *szFilename );
        };
    }
}
#endif