#ifndef __MooseDDSData_h__
#define __MooseDDSData_h__
#include <MooseTextureData.h>
namespace Moose
{
    namespace Util
    {
        class MOOSE_API CDDSData : public Moose::Util::ITextureData
        {
        public:
            void Load( const char *szFile );
        };
    }
}

#endif
