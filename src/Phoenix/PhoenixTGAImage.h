#ifndef __PhoenixTGAImage_h__
#define __PhoenixTGAImage_h__
/////////////////////////////////////////////////////////////////
#include "PhoenixGlobals.h"
#include <iostream>
#include <fstream>
#include <memory.h>
#include <string>
/////////////////////////////////////////////////////////////////
namespace Phoenix 
{
  namespace Graphics
  {
    class CTGAImage
    {
    public:
      CTGAImage();
      ~CTGAImage();
      int Load(const std::string & szFilename);
      int GetBPP();
      int GetWidth();
      int GetHeight();
      unsigned char* GetImg();       // Return a pointer to image data
      unsigned char* GetPalette();   // Return a pointer to VGA palette
      /// Loads image.
      static CTGAImage * LoadTGAImage( const char *szFilename, std::ostream & stream = std::cerr );
    private:
      short int iWidth,iHeight,iBPP;
      unsigned long lImageSize;
      char bEnc;
      unsigned char *pImage, *pPalette, *pData;
   
      // Internal workers
      int ReadHeader();
      int LoadRawData();
      int LoadTgaRLEData();
      int LoadTgaPalette();
      void BGRtoRGB();
      void FlipImg();
    };
  };// namespace Graphics
};// namespace Phoenix
#endif
