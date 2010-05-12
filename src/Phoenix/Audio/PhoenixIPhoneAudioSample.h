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