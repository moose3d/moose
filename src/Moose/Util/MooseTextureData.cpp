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

#include "MooseTextureData.h"
#include "MooseGL.h"
#include "MooseOGLRenderer.h"
#include "MooseTexture.h"
////////////////////////////////////////////////////////////////////////////////
namespace prefix = Moose::Util;
////////////////////////////////////////////////////////////////////////////////
prefix::ITextureData::ITextureData() : m_pData(NULL), 
                                       m_iGLFormat(GL_RGB), 
                                       m_iGLInternalFormat(GL_RGB),
                                       m_iBlockSize(16),
                                       m_iNumMipMaps(1),
                                       m_nDataByteSize(0),
                                       m_szFilename(NULL)
{
    
}
////////////////////////////////////////////////////////////////////////////////
prefix::ITextureData::~ITextureData()
{
    delete m_pData;
}
////////////////////////////////////////////////////////////////////////////////
int
prefix::ITextureData::GetFormat()
{
    return m_iGLFormat;
}
////////////////////////////////////////////////////////////////////////////////
int
prefix::ITextureData::GetInternalFormat()
{
    return m_iGLInternalFormat;
}
////////////////////////////////////////////////////////////////////////////////
unsigned char *
prefix::ITextureData::GetData()
{
    return m_pData;
}
////////////////////////////////////////////////////////////////////////////////
int
prefix::ITextureData::GetBlockSize()
{
    return m_iBlockSize;
}
////////////////////////////////////////////////////////////////////////////////
int
prefix::ITextureData::GetNumMipMaps()
{
    return m_iNumMipMaps;
}
////////////////////////////////////////////////////////////////////////////////
size_t
prefix::ITextureData::GetDataByteSize()
{
    return m_nDataByteSize;
}
////////////////////////////////////////////////////////////////////////////////
size_t
prefix::ITextureData::GetBytesPerPixel()
{
  return GetDataByteSize() / (GetWidth()*GetHeight());
}
////////////////////////////////////////////////////////////////////////////////
void
prefix::ITextureData::AllocateEmpty( size_t bytes )
{
    m_pData = new unsigned char[bytes];
    m_nDataByteSize = bytes;
}
////////////////////////////////////////////////////////////////////////////////
const char *
prefix::ITextureData::GetFilename() const
{
  return m_szFilename;
}
////////////////////////////////////////////////////////////////////////////////
Moose::Graphics::COglTexture * 
prefix::ITextureData::CreateTexture( Moose::Graphics::COglRenderer & r, Moose::Graphics::TEXTURE_TYPE t )
{
  return r.CreateTexture(this, t);
}
////////////////////////////////////////////////////////////////////////////////
