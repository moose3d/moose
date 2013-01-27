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

#ifndef __MooseTGAImage_h__
#define __MooseTGAImage_h__
/////////////////////////////////////////////////////////////////
#include "MooseGlobals.h"
#include "MooseAPI.h"
#include <iostream>
#include <fstream>
#include <memory.h>
#include <string>
/////////////////////////////////////////////////////////////////
namespace Moose 
{
  namespace Graphics
  {
    class MOOSE_API CTGAImage
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
};// namespace Moose
#endif
