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

/////////////////////////////////////////////////////////////////
//#include "MooseSDLScreen.h"
#include "MooseScreen.h"

#if !defined(MOOSE_APPLE_IPHONE)
#include "MooseLogger.h"
#include "MooseDefaultEntities.h"

#ifdef MOOSE_USE_OPENGL3
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <glew.h>
#include <SDL/SDL.h>
#include <string>
#include <sstream>
/////////////////////////////////////////////////////////////////
using namespace Moose::Window;
using namespace Moose::Core;
using namespace std;
/////////////////////////////////////////////////////////////////
int Moose::Window::CScreenParams::m_iRedSize;///< The number of red bits
int Moose::Window::CScreenParams::m_iGreenSize;///< The number of green bits
int Moose::Window::CScreenParams::m_iBlueSize;///< The number of blue bits
int Moose::Window::CScreenParams::m_bDoubleBuffer;///< The double buffer flag
int Moose::Window::CScreenParams::m_iDepthBufferSize;///< The bits for the depth buffer
int Moose::Window::CScreenParams::m_iWidth;///< The width of the screen
int Moose::Window::CScreenParams::m_iHeight;///< The height of the screen
int Moose::Window::CScreenParams::m_iScreenDepth;///< The depth of the screen
int Moose::Window::CScreenParams::m_iVideoModeFlags; ///< The video mode flags for SDL Screen
int Moose::Window::CScreenParams::m_iMultiSampleBuffers; ///< Number of multisample buffers
int Moose::Window::CScreenParams::m_iMultiSampleSamples;  ///< Number of samples.

int Moose::Window::CScreenParams::m_iGLMajorVersion;      ///< Requested OGL major version, defaults to 3. Applies only to SDL-1.3+
int Moose::Window::CScreenParams::m_iGLMinorVersion;      ///< Requested OGL minor version, defaults to 2. Applies only to SDL-1.3+
int Moose::Window::CScreenParams::m_bVerticalSync;        ///< Vertical sync on / off.
std::string Moose::Window::CScreenParams::m_ScreenName;        ///< Screen name for window
/////////////////////////////////////////////////////////////////
Moose::Window::CScreenParams::CScreenParams()
{
  m_iRedSize = 8;
  m_iGreenSize = 8;
  m_iBlueSize = 8;
  m_bDoubleBuffer = 1;
  m_iDepthBufferSize = 16;
  m_iScreenDepth = 24;
  m_iWidth  = 800;
  m_iHeight = 600;
  m_iVideoModeFlags = SDL_OPENGL;
  m_iMultiSampleBuffers = 0;
  m_iMultiSampleSamples = 0;
  m_iGLMajorVersion = 3;
  m_iGLMinorVersion = 2;
  m_bVerticalSync = 0;
  m_ScreenName = string("Moose App");
}
/////////////////////////////////////////////////////////////////
std::ostream &
Moose::Window::operator<<(std::ostream &stream, const CScreenParams & oglSP)
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
	   << "Blue pixels:\t" << oglSP.m_iBlueSize << " bits" << std::endl
	   << "Multisample buffers: \t" << oglSP.m_iMultiSampleBuffers << std::endl
	   << "Multisample samples: \t" << oglSP.m_iMultiSampleSamples << std::endl;

    stream << "OpenGL version " ;

    // print OpenGL version information:
    if      (GLEW_VERSION_4_1)	stream << "4.1";
    else if (GLEW_VERSION_4_0)	stream << "4.0";
    else if (GLEW_VERSION_3_3)	stream << "3.3";
    else if (GLEW_VERSION_3_2)	stream << "3.2";
    else if (GLEW_VERSION_3_1)	stream << "3.1";
    else if (GLEW_VERSION_3_0)	stream << "3.0";
    else if (GLEW_VERSION_2_1)	stream << "2.1";
    else if (GLEW_VERSION_2_0)	stream << "2.0";
    else if (GLEW_VERSION_1_5) stream << "1.5";
    else if (GLEW_VERSION_1_4) stream << "1.4";
    else if (GLEW_VERSION_1_3) stream << "1.3";
    else if (GLEW_VERSION_1_2) stream << "1.2";
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
/////////////////////////////////////////////////////////////////
std::string
Moose::Window::CScreenParams::ToString() const
{
  ostringstream s;
  s << *this;
  return s.str();
}
/////////////////////////////////////////////////////////////////
Moose::Window::CScreen::CScreen()
{
  
  const SDL_VideoInfo *sdlVideoInfo = 0;
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    g_Error << "Couldn't initialize screen (SDL_Init())!\n";
  }
  else
  {

    sdlVideoInfo = SDL_GetVideoInfo();

    if ( !sdlVideoInfo )
    {
      g_Error << "Couldn't initialize screen "
              << "( SDL_GetVideoInfo())!\n";

    }
    else
    {
      // Define which screen depth should be used, currently allowed are
      // 32, 24 and 16. Any other ScreenDepth will revert to desktop depth.
      switch ( m_iScreenDepth ) {

      case 32:
      case 24:
      case 16:
        break;

      default:
        m_iScreenDepth = sdlVideoInfo->vfmt->BitsPerPixel;
        break;
      }




      SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   m_iRedSize);
      SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, m_iGreenSize);
      SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  m_iBlueSize);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_iDepthBufferSize);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, m_iMultiSampleBuffers );
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m_iMultiSampleSamples );
      SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, m_bDoubleBuffer);
#ifdef MOOSE_USE_OPENGL3
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, m_iGLMajorVersion);
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, m_iGLMinorVersion);

      m_pMainWindow = SDL_CreateWindow( m_ScreenName.c_str(),
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 
                                        m_iWidth,
                                        m_iHeight,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
      if ( m_pMainWindow == NULL)
      {
        g_Error << "Couldn't initialize screen "
                << "( SDL_CreateWindow: "
                << SDL_GetError() << ")!\n";
      }

      m_pGLContext = SDL_GL_CreateContext(m_pMainWindow);
      g_Log << "Ladies and gents, we have OpenGL 3.2!\n";
#else
      if ( SDL_SetVideoMode( m_iWidth,
                             m_iHeight,
                             m_iScreenDepth,
                             m_iVideoModeFlags) == NULL )
      {
        
        g_Error << "Couldn't initialize screen "
                << "( SDL_SetVideoMode: "
                << SDL_GetError() << ")!\n";
      }
      g_Log << "Ladies and gents, we have OpenGL 2.x\n";
#endif
      
    } // ..!sdlVideoInfo
  } // ..SDL_Init
  GLenum status = glewInit();
  if ( status != GLEW_OK  )
  {
    g_Error << "Error initializing GLEW: " << glewGetErrorString(status)<< "\n";
  }
#ifdef MOOSE_USE_OPENGL3

  SDL_GL_SetSwapInterval(m_bVerticalSync);
  // check actual state
  switch(SDL_GL_GetSwapInterval())
  {
  case 1:
    m_bVerticalSync = 1;
  default:
    m_bVerticalSync = 0;
  }
#endif
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
  //av_register_all();
}
/////////////////////////////////////////////////////////////////
Moose::Window::CScreen::~CScreen()
{
#ifdef MOOSE_USE_OPENGL3
  SDL_GL_DeleteContext(m_pGLContext);
  SDL_DestroyWindow(m_pMainWindow);
#endif
  SDL_Quit();
  g_Error << "Screen destroyed, exiting.\n";

}
/////////////////////////////////////////////////////////////////
void
Moose::Window::CScreen::SwapBuffers()
{
#if SDL_OPENGLV3
  SDL_GL_SwapWindow(m_pMainWindow);
#else
  SDL_GL_SwapBuffers();
#endif
}
/////////////////////////////////////////////////////////////////
#endif
