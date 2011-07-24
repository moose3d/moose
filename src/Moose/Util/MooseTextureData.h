#ifndef __MooseTextureData_h__
#define __MooseTextureData_h__
#include <MooseDimensional2D.h>
#include <cstdlib>
namespace Moose
{
    namespace Util
    {
        class MOOSE_API ITextureData : public Moose::Spatial::CDimensional2D
        {
        protected:
            unsigned char *     m_pData; ///< actual image pixel data.
            int                 m_iGLFormat; ///< format for GL texture
            int                 m_iGLInternalFormat; ///< internal format for GL texture (with GL ES same as the other format).
            int                 m_iBlockSize; ///< Only for DDS.
            int                 m_iNumMipMaps; ///< only for DDS.
            size_t              m_nDataByteSize;
        public:
            ITextureData();
            virtual ~ITextureData();
            virtual void        Load( const char *szFile ) = 0;
            int                 GetFormat();
            int                 GetInternalFormat();
            unsigned char *     GetData();

            int                 GetBlockSize(); ///< only for DDS
            int                 GetNumMipMaps(); ///< Only for DDS
            size_t              GetDataByteSize();
            void                AllocateEmpty( size_t bytes );
        };
    }
}

#endif
