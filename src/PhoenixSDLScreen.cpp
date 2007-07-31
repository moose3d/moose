/////////////////////////////////////////////////////////////////
#include "PhoenixSDLScreen.h"
#include <SDL.h>
#include <GL/GLee.h>
#include <GL/glu.h>
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Window;
/// declare visiblity for static member.
extern CSDLScreenParams Phoenix::Window::CSDLScreen::m_SDLScreenParams;
/////////////////////////////////////////////////////////////////
Phoenix::Window::CSDLScreenParams::CSDLScreenParams()
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
/////////////////////////////////////////////////////////////////
std::ostream &
operator<<(std::ostream &stream, 
	   CSDLScreenParams oglSP)
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
/////////////////////////////////////////////////////////////////
Phoenix::Window::CSDLScreen::~CSDLScreen() {
    
  SDL_Quit();
  std::cerr << "Screen destroyed, exiting."  
       << std::endl;
  
}
/////////////////////////////////////////////////////////////////
Phoenix::Window::CSDLScreen::CSDLScreen( )
{
  const SDL_VideoInfo *sdlVideoInfo = 0;
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
  {
    std::cerr << "Couldn't initialize screen (SDL_Init())!" 
	      << std::endl;   
  } 
  else 
  {
    
    sdlVideoInfo = SDL_GetVideoInfo();
    
    if ( !sdlVideoInfo ) 
    {
      std::cerr << "Couldn't initialize screen " 
		<< "( SDL_GetVideoInfo())!"
		<< std::endl;
    } 
    else 
    {
      // Define which screen depth should be used, currently allowed are 
      // 32, 24 and 16. Any other ScreenDepth will revert to desktop depth.
      switch ( m_SDLScreenParams.m_iScreenDepth ) {
	
      case 32:
      case 24:
      case 16:
	break;
	  
      default:
	m_SDLScreenParams.m_iScreenDepth = sdlVideoInfo->vfmt->BitsPerPixel;
	break;
      }

      SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   m_SDLScreenParams.m_iRedSize);
      SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, m_SDLScreenParams.m_iGreenSize);
      SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  m_SDLScreenParams.m_iBlueSize);
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 
			  m_SDLScreenParams.m_iDepthBufferSize);

      if ( SDL_SetVideoMode( m_SDLScreenParams.m_iWidth,
			     m_SDLScreenParams.m_iHeight,
			     m_SDLScreenParams.m_iScreenDepth,
			     m_SDLScreenParams.m_iVideoModeFlags) == NULL ){
	  
	std::cerr << "Couldn't initialize screen "
		  << "( SDL_SetVideoMode: "
		  << SDL_GetError() << ")!"
		  << std::endl;
	  
      } 
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
    //av_register_all();
    

}
/////////////////////////////////////////////////////////////////
Phoenix::Window::CSDLScreen::SwapBuffers()
{
  SDL_GL_SwapBuffers();
}
/////////////////////////////////////////////////////////////////
