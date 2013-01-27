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

#include "MooseTGAData.h"
#include "MooseGL.h"
#include "MooseExceptions.h"
#include "MooseTGAImage.h"
#include <cstring>
#include "MooseOGLRenderer.h"
#include "MooseTexture.h"
////////////////////////////////////////////////////////////////////////////////
using namespace Moose::Exceptions;
using namespace Moose::Graphics;
namespace prefix = Moose::Util;
////////////////////////////////////////////////////////////////////////////////
void
prefix::CTGAData::Load( const char *szFile )
{
  m_szFilename = szFile;
  ////////////////////
  CTGAImage *pImage = new CTGAImage();
  CMooseRuntimeError err("");
  switch ( pImage->Load( szFile ) )
  {
  case IMG_OK:
    break;
  case IMG_ERR_NO_FILE:
      err << "No such file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  case IMG_ERR_UNSUPPORTED:
      err << "Unsupported format in file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  case IMG_ERR_MEM_FAIL:
      err << "Out of memory while loading file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  case IMG_ERR_BAD_FORMAT:
      err << "Bad format while loading file '" << szFile << "'";
      delete pImage;
      throw err;
    break;
  default:
    err << "Something unknown happened when loading TGA image.";
    throw err;
    break;
  }

  ////////////////////
  // Check correct depth
  switch (pImage->GetBPP())
  {
  case 8:
    m_iGLInternalFormat = 1;
    m_iGLFormat = GL_LUMINANCE;
    break;
  case 16:
    m_iGLInternalFormat = 2;
    m_iGLFormat = GL_LUMINANCE_ALPHA;
    break;
  case 24:
    m_iGLInternalFormat = 3;
    m_iGLFormat = GL_RGB;
    break;
  case 32:
    m_iGLInternalFormat = 4;
    m_iGLFormat = GL_RGBA;
    break;
  default:
      delete pImage;
      err << "Not 8, 16, 24 or 32 BBP image (was " << pImage->GetBPP()
	      << "):  '" << szFile << "'";
      throw err;
      break;
  }

  SetWidth(pImage->GetWidth());
  SetHeight(pImage->GetHeight());
  // width * height * bytes per pixel
  size_t nBytesize = GetWidth()*GetHeight()*(pImage->GetBPP()/8);
  m_pData = new unsigned char[nBytesize];
  memcpy(m_pData, pImage->GetImg(), nBytesize);
  m_nDataByteSize = nBytesize;
  delete pImage;
}
////////////////////////////////////////////////////////////////////////////////
