#ifndef __PhoenixIPhoneAudioSample_h__
#define __PhoenixIPhoneAudioSample_h__
#include "PhoenixALSampleTypes.h"
/////////////////////////////////////////////////////////////
namespace Phoenix
{
    namespace Sound
    {
        class CIPhoneAudioSample : public Phoenix::Sound::CALSample
        {
        public:
            bool Load( const char *szFilename );
        };
    }
}
#endif