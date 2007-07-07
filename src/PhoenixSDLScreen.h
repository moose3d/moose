/////////////////////////////////////////////////////////////////
#ifndef __CSDLScreen_h__
#define __CSDLScreen_h__
/////////////////////////////////////////////////////////////////
#include <SDL.h> 
#include "PhoenixCore.h"
/////////////////////////////////////////////////////////////////
using namespace Phoenix::Core;
namespace Phoenix
{
  namespace Window
  {
    /////////////////////////////////////////////////////////////////
    /// The Screen Singleton. Each application can have only one screen where
    /// drawing occurs. Hence, the Singleton nature.
    class CSDLScreenParams 
    {
    public:
      /// The number of red bits  
      int m_iRedSize;
      /// The number of green bits
      int m_iGreenSize;
      /// The number of blue bits
      int m_iBlueSize;
      /// The double buffer flag
      int m_bDoubleBuffer;
      /// The bits for the depth buffer
      int m_iDepthBufferSize;
      /// The width of the screen
      int m_iWidth;
      //7 The height of the screen
      int m_iHeight;
      /// The depth of the screen
      int m_iScreenDepth;
      /// The video mode flags for SDL Screen
      int m_iVideoModeFlags;
      ////////////////////
      /// The default constructor, sets default values for members.
      CSDLScreenParams()
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
      ////////////////////
      /// Prints out screen parameters to output stream.
      /// \param stream output stream
      /// \param oglSP SDL screne object.
      /// \retruns reference to output stream.
      friend std::ostream& operator<<( std::ostream &stream, 
				       const CSDLScreenParams & oglSP);
    };
    /////////////////////////////////////////////////////////////////
    class CSDLScreen 
    {
    private:

      ////////////////////
      /// The constructor 
      CScreen() {}
      ////////////////////
      /// The deconstructor
      ~CScreen(){}
    public:  
      /// Screen parameters.
      static CSDLScreenParams m_SDLScreenParams;
    };
    /////////////////////////////////////////////////////////////////
  }; // namespace Window
}; // namespace Phoenix
#endif
/////////////////////////////////////////////////////////////////
