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

#include "GSE_Screen.h"
#include <SDL.h>
#include <GL/GLee.h>
#include <GL/glu.h>
#include <ffmpeg/avformat.h> 

// Initialize static pointer
GSE_Screen * GSE_Screen::m_pScreen = NULL;

std::ostream &
operator<<(std::ostream &stream, 
	   GSE_OglScreenParams oglSP)
{
    int iVal = 0;
    const char *tmp = oglSP.m_bDoubleBuffer ? "yes" : "no";
    stream << "Width:\t" << oglSP.m_iWidth << std::endl
	   << "Height:\t" << oglSP.m_iHeight << std::endl
	   << "Depth:\t" << oglSP.m_iScreenDepth << std::endl
	   << "DoubleBuffer:\t" <<  tmp << std::endl
	   << "Depth buffer:\t" << oglSP.m_iDepthBufferSize 
	   << " bits" << std::endl
	   << "Red pixels:\t" << oglSP.m_iRedSize << " bits" << std::endl
	   << "Green pixels:\t" << oglSP.m_iGreenSize << " bits" << std::endl
	   << "Blue pixels:\t" << oglSP.m_iBlueSize << " bits" << std::endl;
    stream << "OpenGL version " ;

    // print OpenGL version information:
    if      (GLEE_VERSION_2_0)	stream << "2.0"; 
    else if (GLEE_VERSION_1_5) stream << "1.5";
    else if (GLEE_VERSION_1_4) stream << "1.4";
    else if (GLEE_VERSION_1_3) stream << "1.3";
    else if (GLEE_VERSION_1_2) stream << "1.2";
    else  stream << "<1.2";

    
    stream << " is supported." << std::endl;
    stream << "OpenGL renderer:\t" << glGetString(GL_RENDERER) << std::endl;
    stream << "OpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
    stream << "OpenGL vendor:\t" << glGetString(GL_VENDOR) << std::endl;

    glGetIntegerv(GL_MAX_VARYING_FLOATS, &iVal);
    stream << "--- GLSL shader specific ---" << std::endl;
    stream << "GL_MAX_VARYING_FLOATS:\t" << iVal << std::endl;

    return stream;
}

GSE_Screen::~GSE_Screen() {
    
  SDL_Quit();
  Core::GSE_Logger::Log() << "Screen destroyed, exiting."  
			  << std::endl;
  
}

GSE_Screen *
GSE_Screen::GetScreen()
{

  if ( m_pScreen == NULL ){
    GSE_OglScreenParams defaultParams;
    m_pScreen = GetScreen(defaultParams);
  }
  return m_pScreen;
}

GSE_Screen *
GSE_Screen::GetScreen( GSE_OglScreenParams &rOglScreenParams)
{
  const SDL_VideoInfo *sdlVideoInfo = 0;


  if ( m_pScreen == NULL ) {

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {

      Core::GSE_Logger::Error() << "Couldn't initialize screen (SDL_Init())!" 
				<< std::endl; 
      
    } else {
    
      sdlVideoInfo = SDL_GetVideoInfo();

      if ( !sdlVideoInfo ) {
	Core::GSE_Logger::Error() << "Couldn't initialize screen " 
				  << "( SDL_GetVideoInfo())!"
				  << std::endl;
      } else {

	// Define which screen depth should be used, currently allowed are 
	// 32, 24 and 16. Any other ScreenDepth will revert to desktop depth.
	switch ( rOglScreenParams.m_iScreenDepth ) {

	case 32:
	case 24:
	case 16:
	  break;
	  
	default:
	  rOglScreenParams.m_iScreenDepth = sdlVideoInfo->vfmt->BitsPerPixel;
	  break;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   rOglScreenParams.m_iRedSize);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, rOglScreenParams.m_iGreenSize);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  rOglScreenParams.m_iBlueSize);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 
			    rOglScreenParams.m_iDepthBufferSize);

	if ( SDL_SetVideoMode( rOglScreenParams.m_iWidth,
			       rOglScreenParams.m_iHeight,
			       rOglScreenParams.m_iScreenDepth,
			       rOglScreenParams.m_iVideoModeFlags) == NULL ){
	  
	  Core::GSE_Logger::Error() << "Couldn't initialize screen "
				    << "( SDL_SetVideoMode: "
				    << SDL_GetError() << ")!"
				    << std::endl;
	  
	} else {
	  
	  m_pScreen = new GSE_Screen();    
	  m_pScreen->m_OglScreenParams = rOglScreenParams;
	} // ..SDL_SetVideoMode
      } // ..!sdlVideoInfo
    } // ..SDL_Init

    
    // When images are loaded as textures they are flipped and mirrored over Y-axis. 
    // By manipulating the texture matrix accordingly we can get rid of the annoyance.
    // 
    // Unfortunately, this bugger messes up the GL_TEXTURE_RECTANGLE_ARB extension.
    // (It took Couple of hours of debugging until the culprit was ACCIDENTALY found)
    // So, a better ( only ) solution is to flip image directly using SDL_Surface.
    //
    //glMatrixMode(GL_TEXTURE);
    //glRotatef(180.0f,0.0f,0.0f,1.0f);
    //glScalef(-1.0f,1.0f,1.0f);
    //glMatrixMode(GL_MODELVIEW);

    // Register ffmpeg-provided codecs for video textures
    av_register_all();
    
  } // ..m_pScreen == NULL

  return m_pScreen;
  
}
