/******************************************************************************/
 * Moose3D is a game development framework.
 *
 * Copyright 2006-2013 Anssi Gröhn / entity at iki dot fi.
 *
 * This file is part of Moose3D.
 *
 * Moose3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moose3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moose3D.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef __MooseTextureData_h__
#define __MooseTextureData_h__
#include <MooseDimensional2D.h>
#include <MooseTexture.h>
#include <cstdlib>
namespace Moose
{
    namespace Graphics 
    {
      class COglTexture;
      class COglRenderer;
    }
  
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
            const char *        m_szFilename;
        public:
            ITextureData();
            virtual ~ITextureData();
            virtual void        Load( const char *szFile ) = 0;
            const char *        GetFilename() const;
            int                 GetFormat();
            int                 GetInternalFormat();
            unsigned char *     GetData();

            int                 GetBlockSize(); ///< only for DDS
            int                 GetNumMipMaps(); ///< Only for DDS
            size_t              GetDataByteSize();
            size_t              GetBytesPerPixel();
            void                AllocateEmpty( size_t bytes );
            virtual Moose::Graphics::COglTexture * CreateTexture( Moose::Graphics::COglRenderer & r, 
                                                                  Moose::Graphics::TEXTURE_TYPE t = Moose::Graphics::TEXTURE_2D);
        };
    }
}

#endif
