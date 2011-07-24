#ifndef __MoosePNGData_h__
#define __MoosePNGData_h__

#include <MooseTextureData.h>

namespace Moose
{
    namespace Util
    {
        class MOOSE_API CPNGData : public Moose::Util::ITextureData
        {
        public:
            void Load( const char *szFile );
        };
    }
}
#endif
