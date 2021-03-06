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

#include "MooseGlobals.h"
#include "MooseDDSImage.h"
#include "MooseLogger.h"
#include "MooseDefaultEntities.h"
#include <iostream>
#include <cstring>
#include <fstream>
/////////////////////////////////////////////////////////////////
Moose::Graphics::CDDSImage::CDDSImage() : 
  m_nWidth(0), m_nHeight(0), m_iComponents(0), m_Format(DDS_FORMAT_DXT1), 
  m_iNumMipMaps(0), m_pPixels(NULL)
{
  
}
/////////////////////////////////////////////////////////////////
Moose::Graphics::CDDSImage::~CDDSImage()
{
  
}
/////////////////////////////////////////////////////////////////
int
Moose::Graphics::CDDSImage::Load( const char *szFilename )
{
  int iRetval		= IMG_OK;
  int iMipmapFactor	= 0;
  int iBufferSize	= 0;
  size_t curPos,endPos;
  
  Destroy();
  using namespace std;
  // for identifying dds files.
  char fileHeader[4];

  ifstream file;
  file.open(szFilename,ios::binary);

  if ( file == NULL )
  {
    iRetval = IMG_ERR_NO_FILE;
    goto exit;
  }
  file.read( fileHeader, 4);
  if ( strncmp( fileHeader, "DDS ", 4) != 0)
  {
    iRetval = IMG_ERR_BAD_FORMAT;
    goto cleanup;
  }

  // create and read surface desciptor
  DDSURFACEDESC2 ddsd;
  file.read( (char *)&ddsd, sizeof(ddsd));

  ////////////////////
  // Determine file type.
  // compression ratios are 8:1 for DXT1, 4:1 for the rest.
  switch( ddsd.ddpfPixelFormat.dwFourCC )
  {
  case FOURCC_DXT1:
    m_Format = DDS_FORMAT_DXT1;
    iMipmapFactor = 2;
    //cerr << "we got DXT1" << endl;
    break;
  case FOURCC_DXT3:
    m_Format = DDS_FORMAT_DXT3;
    iMipmapFactor = 4;
    //cerr << "we got DXT3" << endl;
    break;
  case FOURCC_DXT5:
    m_Format = DDS_FORMAT_DXT5;
    iMipmapFactor = 4;
    //cerr << "we got DXT5" << endl;
    break;
  case FOURCC_DXT4:
    m_Format = DDS_FORMAT_DXT4;
    iMipmapFactor = 4;
    //cerr << "we got DXT4" << endl;
    break;
  case FOURCC_DXT2:
    m_Format = DDS_FORMAT_DXT2;
    iMipmapFactor = 4;
    //cerr << "we got DXT2" << endl;
    break;
  default:
    g_Error << "Uh-oh, we got " << FOURCC(ddsd.ddpfPixelFormat.dwFourCC) << endl;
    iRetval = IMG_ERR_BAD_FORMAT;
    goto cleanup;
    break;
  }

  ////////////////////
  /// sanity check
  if ( ddsd.dwLinearSize == 0 )
  {
    iRetval = IMG_ERR_BAD_FORMAT;
    goto cleanup;
  }

  ////////////////////
  /// Mipmap factor
  if( ddsd.dwMipMapCount > 1 )
    iBufferSize = ddsd.dwLinearSize * iMipmapFactor;
  else
    iBufferSize = ddsd.dwLinearSize;

  /*curPos = file.tellg();
  file.seekg(0,ios::end);
  endPos = file.tellg();
  iBufferSize = endPos-curPos;
  file.seekg( curPos, ios::beg);*/

  m_byteSize = iBufferSize;
  //g_Log << "buffersize is : " << m_byteSize << "\n";
  ////////////////////
  /// allocate memory
  m_pPixels = new unsigned char[iBufferSize];
  if ( m_pPixels == NULL )
  {
    iRetval = IMG_ERR_MEM_FAIL;
    goto cleanup;
  }

  // read actual image data
  file.read( (char *)m_pPixels, iBufferSize);
  if ( !file.eof())
  {
    cerr << "somethign weird is going on..." << endl;
  }
  file.close();
  
  // set attributes 
  m_nWidth	= ddsd.dwWidth;
  m_nHeight	= ddsd.dwHeight;
  m_iNumMipMaps = ddsd.dwMipMapCount;
  
  if ( m_Format == DDS_FORMAT_DXT1 ) { m_iComponents = 3; }
  else				     { m_iComponents = 4; }  
  ////////////////////
  // we're good to go!
  goto exit;
  // release handles & stuff
 cleanup:
  file.close();
  if ( GetPixelData() != NULL )  
  { 
    delete m_pPixels; 
    m_pPixels = NULL;
  }
 exit:

  return iRetval;
}
/////////////////////////////////////////////////////////////////
unsigned char *
Moose::Graphics::CDDSImage::GetPixelData()
{
  return m_pPixels;
}
/////////////////////////////////////////////////////////////////
void
Moose::Graphics::CDDSImage::Destroy()
{
  ////////////////////
  if ( GetPixelData() != NULL )
  {
    delete GetPixelData();
    m_pPixels = NULL;
  }
  ////////////////////
  m_nWidth	= 0;
  m_nHeight	= 0;
  m_iComponents = 0;
  m_Format	= DDS_FORMAT_DXT1;
  m_iNumMipMaps = 0;
  m_pPixels	= NULL;
  ////////////////////
  
}
/////////////////////////////////////////////////////////////////
