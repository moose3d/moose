#ifndef __MooseTGAData_h__
#define __MooseTGAData_h__
#include <MooseTextureData.h>
namespace Moose
{
    namespace Util
    {
        class MOOSE_API CTGAData : public Moose::Util::ITextureData
        {
        public:
            void Load( const char *szFile );
        };
    }
}
#endif
