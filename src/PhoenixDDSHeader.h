#ifndef __PhoenixDDSHeader_h__
#define __PhoenixDDSHeader_h__
/////////////////////////////////////////////////////////////////
#ifndef WORD 
typedef unsigned short WORD;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#ifndef LPVOID
typedef void *LPVOID;
#endif
#ifndef LONG
typedef long LONG;
#endif
/////////////////////////////////////////////////////////////////
namespace Phoenix
{
  namespace Graphics 
  {
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
  }
}
/////////////////////////////////////////////////////////////////
#endif
