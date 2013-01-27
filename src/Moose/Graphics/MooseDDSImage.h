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

#ifndef __MooseDDSHeader_h__
#define __MooseDDSHeader_h__
/////////////////////////////////////////////////////////////////
#include <fstream>
#include <sys/types.h>
#include "MooseAPI.h"
#ifndef WIN32
/////////////////////////////////////////////////////////////////
// Pointers are tricky, 64-bit ones wreak havoc unless we define them as 32-bit.
#define LPVOID  u_int32_t
#define LONG    int32_t
#define DWORD   u_int32_t
#define WORD    int16_t
/////////////////////////////////////////////////////////////////
/// FourCC's : 
///  http://en.wikipedia.org/wiki/FOURCC
///  http://www.fourcc.org/indexcod.htm
/////////////////////////////////////////////////////////////////
#define MAKE_FOURCC(A,B,C,D) ((A) | (B<<8) | (C<<16)| (D<<24))
#define FOURCC_DXT1 MAKE_FOURCC('D','X','T','1')
#define FOURCC_DXT2 MAKE_FOURCC('D','X','T','2')
#define FOURCC_DXT3 MAKE_FOURCC('D','X','T','3')
#define FOURCC_DXT4 MAKE_FOURCC('D','X','T','4')
#define FOURCC_DXT5 MAKE_FOURCC('D','X','T','5')
#define FOURCC(i) (((i & 0xff000000) >> 24) | ((i & 0x00ff0000) >> 8) | ((i & 0x0000ff00) << 8) | ((i & 0x000000ff) << 24))
#endif
/////////////////////////////////////////////////////////////////
namespace Moose
{
  namespace Graphics 
  {
#ifdef WIN32
#include <ddraw.h>
#else
    ////////////////////
    /// Structs as described in MSDN;
    /// http://msdn.microsoft.com/en-us/library/aa454679.aspx
    typedef struct _DDCOLORKEY{ 
      DWORD dwColorSpaceLowValue; 
      DWORD dwColorSpaceHighValue; 
    } DDCOLORKEY;//,FAR *LPDDCOLORKEY;  

    typedef struct _DDPIXELFORMAT {
      DWORD  dwSize;
      DWORD  dwFlags;
      DWORD  dwFourCC;
      union {
	DWORD  dwRGBBitCount;
	DWORD  dwYUVBitCount;
	DWORD  dwZBufferBitDepth;
	DWORD  dwAlphaBitDepth;
	DWORD  dwLuminanceBitCount;
	DWORD  dwBumpBitCount;
	DWORD  dwPrivateFormatBitCount;
      } ;
      union {
	DWORD  dwRBitMask;
	DWORD  dwYBitMask;
	DWORD  dwStencilBitDepth;
	DWORD  dwLuminanceBitMask;
	DWORD  dwBumpDuBitMask;
	DWORD  dwOperations;
      } ;
      union {
	DWORD  dwGBitMask;
	DWORD  dwUBitMask;
	DWORD  dwZBitMask;
	DWORD  dwBumpDvBitMask;
	struct {
	  WORD wFlipMSTypes;
	  WORD wBltMSTypes;
	} MultiSampleCaps;
      } ;
      union {
	DWORD  dwBBitMask;
	DWORD  dwVBitMask;
	DWORD  dwStencilBitMask;
	DWORD  dwBumpLuminanceBitMask;
      } ;
      union {
	DWORD  dwRGBAlphaBitMask;
	DWORD  dwYUVAlphaBitMask;
	DWORD  dwLuminanceAlphaBitMask;
	DWORD  dwRGBZBitMask;
	DWORD  dwYUVZBitMask;
      } ;
    } DDPIXELFORMAT;//, FAR* LPDDPIXELFORMAT;

    typedef struct _DDSCAPS {
      DWORD  dwCaps;
    } DDSCAPS;//, FAR* LPDDSCAPS;

    typedef struct _DDSCAPS2 {
      DWORD  dwCaps;
      DWORD  dwCaps2;
      DWORD  dwCaps3;
      DWORD  dwCaps4;
    } DDSCAPS2;//, FAR* LPDDSCAPS2;

    typedef struct _DDSURFACEDESC2 {
      DWORD  dwSize;
      DWORD  dwFlags;
      DWORD  dwHeight;
      DWORD  dwWidth; 
      union {
	LONG  lPitch;
	DWORD  dwLinearSize; 
      } ;
      DWORD  dwBackBufferCount; 
      union {
	DWORD  dwMipMapCount; 
	DWORD  dwRefreshRate;  
	DWORD  dwSrcVBHandle; 
      } ;
      DWORD  dwAlphaBitDepth;  
      DWORD  dwReserved; 
      LPVOID  lpSurface; 
      union {
	DDCOLORKEY  ddckCKDestOverlay;   
	DWORD  dwEmptyFaceColor;    
      } ;
      DDCOLORKEY  ddckCKDestBlt;     
      DDCOLORKEY  ddckCKSrcOverlay;    
      DDCOLORKEY  ddckCKSrcBlt;     
      union {
	DDPIXELFORMAT  ddpfPixelFormat;    
	DWORD  dwFVF;  
      } ;
      DDSCAPS2  ddsCaps;  
      DWORD  dwTextureStage;   
    } DDSURFACEDESC2;
    
#endif
    /////////////////////////////////////////////////////////////////
    /// Types for DDS image format
    enum DDS_FORMAT_TYPE
    {
      DDS_FORMAT_DXT1,
      DDS_FORMAT_DXT2,
      DDS_FORMAT_DXT3,
      DDS_FORMAT_DXT4,
      DDS_FORMAT_DXT5
    };
    /////////////////////////////////////////////////////////////////
    /// Actual data for DDS image
    class MOOSE_API CDDSImage
    {
    private:
      size_t		m_nWidth;
      size_t		m_nHeight;
      int		m_iComponents;
      DDS_FORMAT_TYPE	m_Format;
      int		m_iNumMipMaps;
      unsigned char *	m_pPixels;
      size_t            m_byteSize;
    public:
      ////////////////////
      /// Constructor.
      CDDSImage();
      ////////////////////
      /// Destructor.
      ~CDDSImage();
      ////////////////////
      /// Loads image from given path.
      /// \param szFilename Path
      /// \returns IMG_OK on success, something else on error.
      int Load( const char *szFilename );
      ////////////////////
      /// Returns pointer to bytes representing image.
      /// \returns Pointer to pixel data.
      unsigned char * GetPixelData();
      ////////////////////
      /// Returns current format.
      /// \return image format.
      DDS_FORMAT_TYPE GetFormat() const {	return m_Format; } 
      size_t	      GetWidth() const {	return m_nWidth; }
      size_t	      GetHeight() const {	return m_nHeight; }
      int	      GetNumMipMaps() const {	return m_iNumMipMaps; }
      int	      GetComponents() const { return m_iComponents; }
      size_t      GetByteSize() const { return m_byteSize; }
    private:
      ////////////////////
      /// Cleans up existing data.
      void Destroy();
    };
    /////////////////////////////////////////////////////////////////
  }
}
/////////////////////////////////////////////////////////////////
#endif
