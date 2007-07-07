/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/

#ifndef __GSE_Screen_h__
#define __GSE_Screen_h__
#include "GSE_Logger.h"
// =============== WIN32 includes ================
#ifdef WIN32   
#include <SDL.h>
#else
// =============== LINUX includes ================
#include <SDL/SDL.h> 
#endif
// =============== Generic includes ================


//
// The Screen Singleton. Each application can have only one screen where
// drawing occurs. Hence, the Singleton nature.
//

class GSE_OglScreenParams 
{
 public:
  // The number of red bits  
  int m_iRedSize;
  // The number of green bits
  int m_iGreenSize;
  // The number of blue bits
  int m_iBlueSize;
  // The double buffer flag
  int m_bDoubleBuffer;
  // The bits for the depth buffer
  int m_iDepthBufferSize;
  // The width of the screen
  int m_iWidth;
  // The height of the screen
  int m_iHeight;
  // The depth of the screen
  int m_iScreenDepth;
  // The video mode flags for SDL Screen
  int m_iVideoModeFlags;

  // The default constructor, sets default values for members.
  GSE_OglScreenParams()
  {
    m_iRedSize = 8;
    m_iGreenSize = 8;
    m_iBlueSize = 8;
    m_bDoubleBuffer = 0;
    m_iDepthBufferSize = 16;
    m_iScreenDepth = 24;
    m_iWidth = 640;
    m_iHeight = 480;
    m_iVideoModeFlags = SDL_OPENGL;
  }

  friend std::ostream& operator<<( std::ostream &stream, 
				   GSE_OglScreenParams oglSP);
};



class GSE_Screen {

 private:
  // ------------------------------------------------
  static GSE_Screen *m_pScreen;
  // ------------------------------------------------

  // The constructor 
  GSE_Screen(){

  }
  
 public:  
  // ------------------------------------------------
  GSE_OglScreenParams m_OglScreenParams;
  // ------------------------------------------------
  static GSE_Screen *GetScreen(GSE_OglScreenParams &rOglScreenParams);
  static GSE_Screen *GetScreen();
  // The deconstructor
  ~GSE_Screen();


  

  
};

#endif
